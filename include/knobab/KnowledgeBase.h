//
// Created by giacomo on 26/12/20.
//

#ifndef BZDB_SMALLDATABASE_H
#define BZDB_SMALLDATABASE_H

#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <knobab/CountTemplate.h>
#include <knobab/ActTable.h>
#include <unordered_map>
#include <knobab/AttributeTable.h>
#include <yaucl/bpm/structures/log/trace_visitor.h>
#include <yaucl/structures/any_to_uint_bimap.h>
#include <SimplifiedFuzzyStringMatching.h>
#include <yaucl/bpm/structures/commons/DataPredicate.h>
#include <variant>
#include <knobab/dataStructures/TraceData.h>

enum ParsingState {
    LogParsing,
    TraceParsing,
    EventParsing,
    FinishParsing
};

#include <bitset>

using trace_set = std::bitset<sizeof(uint32_t)>;
using act_set = std::bitset<sizeof(uint16_t)>;
const uint16_t MAX_UINT16 = std::pow(2, 16) - 1;
using dataContainer = std::vector<std::pair<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>>;

class KnowledgeBase : public trace_visitor {
    CountTemplate                                   count_table;
    std::unordered_map<std::string, AttributeTable> attribute_name_to_table;
    SimplifiedFuzzyStringMatching                   string_values;

    bool alreadySet;
    std::string source;
    std::string name;
    size_t noTraces;
    size_t currentEventId;
    ParsingState status;
    size_t actId;

public:
    ActTable                                        act_table_by_act_id;

    std::pair<std::unordered_map<std::string, AttributeTable>::iterator,
            std::unordered_map<std::string, AttributeTable>::iterator> getAttrNameTableIt() {
        return {attribute_name_to_table.begin(), attribute_name_to_table.end()};
    }

    union_type resolveRecord(const ActTable::record* eventFromTrace,
                             const std::unordered_map<std::string, AttributeTable>::iterator& attr_table) const {

        return attr_table->second.resolve(*attr_table->second.resolve_record_if_exists(eventFromTrace - act_table_by_act_id.table.data()));
    }

    union_minimal resolveMinimalRecord(const ActTable::record* eventFromTrace,
                                       const std::unordered_map<std::string, AttributeTable>::iterator& attr_table) const {
        return resolveUnionMinimal(attr_table->second,
                                   *attr_table->second.resolve_record_if_exists(eventFromTrace - act_table_by_act_id.table.data()));
    }


    /**
     * Collects the values contained in the knowledge base as single instances
     * @param trace_id          If the trace id is set
     * to -1, then the values are collected from all the traces.
     *                          Otherwise, the value is collected from a specific trace id, if it exists
     * @param act_id            If the act id is set to -1, the values are collected from all the attributes.
     *                          Otherwise, the value is collected from a specific event label, if it exists
     * @param attribute_name    If the attribute name is set to an empty string, the values are collected from all the attributes.
     *                          Otherwise, only the values associated to a specific value are returned
     * @return The collected values as above
     */
    void collectValuesFrom(std::set<union_type> &S, ssize_t trace_id, uint16_t act_id,
                           const std::string &attribute_name) const;
    void collectValuesFrom(std::set<union_type> &S, const std::unordered_set<size_t> &trace_ids, uint16_t act_id,
                           const std::string &attribute_name) const;

    /**
     *
     * @param result                    Mapping the event labels (act) to the attributes we are interested on, which are associated to the resulting values
     * @param resultOtherValues         Mapping any variable to the values being collected from the knowledge base
     * @param trace_ids                 If the set is empty, the values will be collected from all the traces; otherwise, will only be collected the values from the given trace ids
     * @param actToTables               Mapping from act id to the variables
     * @param otherValues               All the variables, if we do not want to collect them
     */
    void collectValuesFrom(std::unordered_map<std::string, std::unordered_map<std::string, std::set<union_type>>>& result,
                           std::unordered_map<std::string, std::set<union_type>>& resultOtherValues,
                           const std::unordered_set<trace_t>& trace_ids = {},
                           const std::unordered_map<std::string, std::unordered_set<std::string>>& actToVariables = {},
                           const std::unordered_set<std::string>& otherValues = {}
    ) const;


    void clear();

    yaucl::structures::any_to_uint_bimap<std::string> event_label_mapper;
    std::unordered_map<size_t, size_t> counting_reference;

    void reconstruct_trace_no_data(std::ostream& os) const;
    void reconstruct_trace_with_data(std::ostream& os) const;

    KnowledgeBase();
    void index_data_structures();
    void enterLog(const std::string &source, const std::string &name) override;
    void exitLog(const std::string &source, const std::string &name) override;
    size_t enterTrace(const std::string &trace_label) override;
    void exitTrace(size_t traceId) override;
    size_t enterEvent(size_t chronos_tick, const std::string &event_label) override;
    void exitEvent(size_t event_id) override;
    void enterData_part(bool isEvent) override;
    void exitData_part(bool isEvent) override;
    void visitField(const std::string &key, bool value) override;
    void visitField(const std::string &key, double value) override;
    void visitField(const std::string &key, const std::string &value) override;
    void visitField(const std::string &key, size_t value) override;

    using no_antlr_event = std::pair<std::string, std::unordered_map<std::string, std::variant<bool, double, std::string, size_t>>>; // <act, payload>
    using no_antlr_trace = std::vector<no_antlr_event>;
    using no_antlr_log = std::vector<no_antlr_trace>;


    void load_data_without_antlr4(const no_antlr_log& L, const std::string &source, const std::string &name);


    // First part of the pipeline
    uint16_t getMappedValueFromAction(const std::string &act) const;
    std::pair<const uint32_t, const uint32_t> resolveCountingData(const std::string &act) const;

    // Second part of the pipeline
    std::unordered_map<uint32_t, double> exists(const std::pair<const uint32_t, const uint32_t>& indexes, const uint16_t& amount) const;

    template <typename traceIdentifier, typename traceValue>
    TraceData<traceIdentifier, traceValue> init(const std::string& act, const double minThreshold = 1) const{
        return existsAt<traceIdentifier, traceValue>(act, 0, minThreshold);
    }

    template <typename traceIdentifier, typename traceValue>
    TraceData<traceIdentifier, traceValue> ends(const std::string& act, const double minThreshold = 1) const{
        return existsAt<traceIdentifier, traceValue>(act, MAX_UINT16, minThreshold);
    }

    template <typename traceIdentifier, typename traceValue>
    TraceData<traceIdentifier, traceValue> existsAt(const std::string& act, const uint16_t& eventId, const double minThreshold = 1) const{
        TraceData<traceIdentifier, traceValue> foundData;

        std::pair<traceIdentifier, traceValue> tracePair;
        const uint16_t& mappedVal = getMappedValueFromAction(act);

        if(mappedVal < 0){
            return foundData;
        }

        std::pair<const uint32_t , const uint32_t> indexes = act_table_by_act_id.resolve_index(mappedVal);

        if(indexes.first < 0){
            return foundData;
        }

        for (auto it = act_table_by_act_id.table.begin() + indexes.first; it != act_table_by_act_id.table.begin() + indexes.second + 1; ++it) {
            uint16_t approxConstant = MAX_UINT16 / 2;

            float satisfiability = getSatisifiabilityBetweenValues(eventId, it->entry.id.parts.event_id, approxConstant);

            if(satisfiability >= minThreshold) {
                const uint16_t newEventID = getPositionFromEventId({it->entry.id.parts.trace_id, it->entry.id.parts.event_id});
                foundData.getTraceApproximations().emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{it->entry.id.parts.trace_id,  newEventID}, {satisfiability, {newEventID}}});
            }
        }

        return foundData;
    }

    /* First is traceID, second is eventID */
    uint16_t getPositionFromEventId(const std::pair<uint32_t, uint16_t> pair) const;

    const std::unordered_map<std::string, AttributeTable> getAttributeNameToTable() const{
        return attribute_name_to_table;
    }

private:
    void collectValuesAmongTraces(std::set<union_type> &S, size_t trace_id, act_t acts, bool HasNoAct,
                                  const std::string &attribute_name, bool hasNoAttribute) const;
    void collectValuesAmongTraces(
            std::unordered_map<std::string, std::unordered_map<std::string, std::set<union_type>>> &result,
            std::unordered_map<std::string, std::set<union_type>> &resultOtherValues,
            const std::unordered_map<std::string, std::unordered_set<std::string>> &actToTables,
            const std::unordered_set<std::string> &otherValues, trace_t traceId) const;

    float getSatisifiabilityBetweenValues(const uint16_t& val1, const uint16_t& val2, const uint16_t& approxConstant) const;

public:
    const dataContainer getLastElements() const {
        dataContainer elems{};
        for (const std::pair<ActTable::record *, ActTable::record *> &rec: act_table_by_act_id.secondary_index) {
            const uint32_t traceId = rec.second->entry.id.parts.trace_id;
            uint16_t eventId = rec.second->entry.id.parts.event_id;
            eventId = getPositionFromEventId({traceId, eventId});
            const std::pair<uint32_t, uint16_t> traceEventPair{traceId, eventId};

            elems.push_back(
                    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{traceEventPair,
                                                                                                       {1, {eventId}}});
        }

        return elems;
    }
};




#endif //BZDB_SMALLDATABASE_H
