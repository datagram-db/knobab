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

enum ParsingState {
    LogParsing,
    TraceParsing,
    EventParsing,
    FinishParsing,
    MissingDataParsing
};

#include <bitset>

using trace_set = std::bitset<sizeof(uint32_t)>;
using act_set = std::bitset<sizeof(uint16_t)>;
union_minimal resolveUnionMinimal(const AttributeTable &table, const AttributeTable::record &x);


class KnowledgeBase : public trace_visitor {
    CountTemplate                                   count_table;
    std::unordered_map<std::string, AttributeTable> attribute_name_to_table, approximate_attribute_to_table;
    SimplifiedFuzzyStringMatching                   string_values;

    std::unordered_map<std::string, std::unordered_set<std::string>> registerEventLabelSchema;

    std::string currentEventLabel;
    bool alreadySet;
    std::string source;
    std::string name;
    size_t noTraces;
    size_t currentEventId;
    ParsingState status;
    size_t actId;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> universe, universeApprox;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> empty;
    size_t maximumStringLength = 0;

public:
    static constexpr double    default_double   = 0.0;
    static constexpr size_t    default_size_t   = 0;
    static constexpr long long default_longlong = 0;
    static constexpr bool      default_bool     = false;
    static std::string         default_string;//= "";
    static double    maximum_reliability_for_insertion;

    ActTable                                        act_table_by_act_id;
    yaucl::structures::any_to_uint_bimap<std::string> event_label_mapper;
    std::unordered_map<size_t, size_t> counting_reference;


    /************************
     * Processing Functions *
     ************************/

    KnowledgeBase();
    void index_data_structures(bool missingDataIndexing);
    void clear();




    /***************************
     * Utility print functions *
     ***************************/

    void reconstruct_trace_no_data(std::ostream& os) const;
    void reconstruct_trace_with_data(std::ostream& os) const;
    void print_count_table(std::ostream& os) const;
    void print_act_table(std::ostream& os) const;
    void print_attribute_tables(std::ostream& os) const;




    /***************************
     * Other Utility Functions *
     ***************************/

    std::pair<std::unordered_map<std::string, AttributeTable>::iterator,
            std::unordered_map<std::string, AttributeTable>::iterator> getAttrNameTableIt();
    union_type resolveRecord(const ActTable::record* eventFromTrace,
                             const std::unordered_map<std::string, AttributeTable>::iterator& attr_table) const;
    union_minimal resolveMinimalRecord(const ActTable::record* eventFromTrace,
                                       const std::unordered_map<std::string, AttributeTable>::iterator& attr_table) const;
    size_t getMaximumStringLength() const { return maximumStringLength+1; }

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






    /***************************
     * Data visiting functions *
     ***************************/

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





    /*******************************
     * Backup data reading measure *
     *******************************/

    using no_antlr_event = std::pair<std::string, std::unordered_map<std::string, std::variant<bool, double, std::string, size_t>>>; // <act, payload>
    using no_antlr_trace = std::vector<no_antlr_event>;
    using no_antlr_log = std::vector<no_antlr_trace>;
    void load_data_without_antlr4(const no_antlr_log& L, const std::string &source, const std::string &name);





    /******************************
     * First part of the pipeline *
     ******************************/

    uint16_t getMappedValueFromAction(const std::string &act) const;
    std::pair<const oid*, const oid*> resolveCountingData(const std::string &act, uint32_t& start, uint32_t& end) const;
    std::pair<const ActTable::record*, const ActTable::record*> resolveActData(const std::string &act, uint32_t& start, uint32_t& end) const;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> range_query(DataPredicate prop, double min_threshold = 1.0, const double c = 2.0) const;





    /******************************
     * Second part of the pipeline *
     ******************************/

    std::unordered_map<uint32_t, float> exists(const std::pair<const oid *, const oid *>& subsection,  const uint32_t& start, const uint32_t& end, const uint16_t& amount = 1, const bool& isExact = false) const;
    std::vector<uint32_t> init(const std::string& act) const;
    std::vector<uint32_t> ends(const std::string& act) const;



private:
    void collectValuesAmongTraces(std::set<union_type> &S, size_t trace_id, act_t acts, bool HasNoAct,
                                  const std::string &attribute_name, bool hasNoAttribute) const;
    void collectValuesAmongTraces(
            std::unordered_map<std::string, std::unordered_map<std::string, std::set<union_type>>> &result,
            std::unordered_map<std::string, std::set<union_type>> &resultOtherValues,
            const std::unordered_map<std::string, std::unordered_set<std::string>> &actToTables,
            const std::unordered_set<std::string> &otherValues, trace_t traceId) const;

    std::pair<int, std::vector<std::pair<std::pair<trace_t, event_t>, double>>>
    range_query(DataPredicate &prop, double min_threshold, double correction, const double c, bool forExistingData = true) const;
};




#endif //BZDB_SMALLDATABASE_H
