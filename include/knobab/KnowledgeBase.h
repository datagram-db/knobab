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

enum ParsingState {
    LogParsing,
    TraceParsing,
    EventParsing,
    FinishParsing
};

#include <bitset>

using trace_set = std::bitset<sizeof(uint32_t)>;
using act_set = std::bitset<sizeof(uint16_t)>;

class KnowledgeBase : public trace_visitor {
    CountTemplate                                   count_table;
    ActTable                                        act_table_by_act_id;
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



    /**
     * Collects the values contained in the knowledge base as single instances
     * @param trace_id          If the trace id is set to -1, then the values are collected from all the traces.
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



    /*
     * void
KnowledgeBase::collectValuesAmongTraces(std::set<union_type> &S, ssize_t trace_id, uint16_t act_id,
                                 const std::string &attribute_name) const {
    bool hasNoAttribute = attribute_name.empty();
    bool hasNoActId = act_id == (uint16_t)-1;
    ssize_t N = act_table_by_act_id.secondary_index.size();
    if (trace_id == -1) {
        for (size_t currentTraceId = 0; currentTraceId < N; currentTraceId++) {
            collectValuesAmongTraces(S, currentTraceId, act_id, hasNoActId, attribute_name, hasNoAttribute);
        }
    } else if ((trace_id >= 0) && (trace_id < N)) {
        collectValuesAmongTraces(S, trace_id, act_id, hasNoActId, attribute_name, hasNoAttribute);
    }
}

#include <bitset>

void KnowledgeBase::collectValuesAmongTraces(std::set<union_type> &S, size_t trace_id, act_t acts, bool hasNoActId,
                                             const std::string &attribute_name, bool hasNoAttribute) const {
    std::bitset<sizeof(uint16_t)> b;
    const auto& ref = act_table_by_act_id.secondary_index[trace_id];
    auto ptr = ref.first;
    while (ptr) {
        if (hasNoActId || (ptr->entry.id.parts.act == acts)) {
            for (const auto& attr_table : attribute_name_to_table) {
                ptrdiff_t offset = ptr - act_table_by_act_id.table.data();
                const AttributeTable::record* recordPtr = attr_table.second.resolve_record_if_exists(offset);
                if (recordPtr &&  (hasNoAttribute || (attribute_name == attr_table.first))) {
                    S.insert(attr_table.second.resolve(*recordPtr));
                }
            }
        }
        ptr = ptr->next;
    };
}
     */

    yaucl::structures::any_to_uint_bimap<std::string> event_label_mapper;
    std::unordered_map<size_t, size_t> counting_reference;

    void reconstruct_trace_no_data(std::ostream& os);
    void reconstruct_trace_with_data(std::ostream& os);

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

private:
    void collectValuesAmongTraces(std::set<union_type> &S, size_t trace_id, act_t acts, bool HasNoAct,
                                  const std::string &attribute_name, bool hasNoAttribute) const;
    void collectValuesAmongTraces(
            std::unordered_map<std::string, std::unordered_map<std::string, std::set<union_type>>> &result,
            std::unordered_map<std::string, std::set<union_type>> &resultOtherValues,
            const std::unordered_map<std::string, std::unordered_set<std::string>> &actToTables,
            const std::unordered_set<std::string> &otherValues, trace_t traceId) const;
};


#endif //BZDB_SMALLDATABASE_H
