//
// Created by giacomo on 26/12/20.
//

#ifndef BZDB_SMALLDATABASE_H
#define BZDB_SMALLDATABASE_H

#include <vector>
#include <string>
#include <set>
#include <unordered_set>
#include <yaucl/numeric/ssize_t.h>
#include <knobab/server/tables/CountTemplate.h>
#include <knobab/server/tables/ActTable.h>
#include <unordered_map>
#include <knobab/server/tables/AttributeTable.h>
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
#include "knobab/mining/CountTableFPTree.h"
#include "knobab/algorithms/mining/DataMiningMetrics.h"
#include "knobab/algorithms/mining/RulesFromFrequentItemset.h"
#include "yaucl/functional/iterators.h"
#include <ostream>


using trace_set = std::bitset<sizeof(uint32_t)>;
using act_set = std::bitset<sizeof(uint16_t)>;

//union_minimal resolveUnionMinimal(const AttributeTable &table, const AttributeTable::record &x);
const uint16_t MAX_UINT16 = std::pow(2, 16) - 1;
const size_t HYBRID_QUERY_THRESHOLD = std::pow(10, 3) / 2;
const size_t HYBRID_LOG_QUERY_THRESHOLD = std::pow(10, 3) / 2;

template <typename T>
struct pattern_mining_result {
    T clause;
    double           support_generating_original_pattern;
    double           support_declarative_pattern;
    double           restrictive_support_declarative_pattern;
    double           restrictive_confidence_plus_declarative_pattern;


    bool operator==(const pattern_mining_result<T> &rhs) const {
        return clause == rhs.clause && support_generating_original_pattern == rhs.support_generating_original_pattern &&
        support_declarative_pattern == rhs.support_declarative_pattern && restrictive_confidence_plus_declarative_pattern == rhs.restrictive_confidence_plus_declarative_pattern;
    }

    pattern_mining_result(const pattern_mining_result& orig, const T& clause) : clause(clause),
                                                                                support_generating_original_pattern(
                                                                                       orig.support_generating_original_pattern),
                                                                                support_declarative_pattern(
                                                                                       orig.support_declarative_pattern),
                                                                                restrictive_confidence_plus_declarative_pattern(orig.restrictive_confidence_plus_declarative_pattern),
                                                                                restrictive_support_declarative_pattern(orig.restrictive_support_declarative_pattern) {

    }
    pattern_mining_result(double support,
                          const T &clause) : pattern_mining_result(clause, support, support, -1.0,-1.0) {}
    pattern_mining_result(const T &clause,
                          double supportGeneratingOriginalPattern,
                          double supportDeclarativePattern,
                          double confidenceDeclarativePattern,
                          double restrictiveSupportDeclarative) : clause(clause),
                                                                  support_generating_original_pattern(
                                                                         supportGeneratingOriginalPattern),
                                                                  support_declarative_pattern(
                                                                         supportDeclarativePattern),
                                                                  restrictive_confidence_plus_declarative_pattern(confidenceDeclarativePattern),
                                                                  restrictive_support_declarative_pattern(restrictiveSupportDeclarative) {}
    pattern_mining_result(const T &clause,
                          double supportGeneratingOriginalPattern,
                          const std::tuple<double,double,double>& triplet) : clause(clause),
                                                                             support_generating_original_pattern(
                                                                          supportGeneratingOriginalPattern),
                                                                             support_declarative_pattern(
                                                                          std::get<0>(triplet)),
                                                                             restrictive_confidence_plus_declarative_pattern(std::get<1>(triplet)),
                                                                             restrictive_support_declarative_pattern(std::get<2>(triplet)) {}


    DEFAULT_CONSTRUCTORS(pattern_mining_result);
    friend std::ostream &operator<<(std::ostream &os, const pattern_mining_result &result){
        os << "Clause: " << result.clause << std::endl
           << "\t - Pattern Matching Support: " << result.support_generating_original_pattern << std::endl;
//        if (result.support_declarative_pattern >= 0.0)
            os << "\t - Declarative Pattern's Support: " << result.support_declarative_pattern << std::endl;
//        if (result.confidence_declarative_pattern >= 0.0)
            os << "\t - Declarative Pattern's Confidence: " << result.restrictive_confidence_plus_declarative_pattern << std::endl;
        return os << std::endl;
    }
};

namespace std {
    template <typename T> struct hash<pattern_mining_result<T>> {
        size_t operator()(const pattern_mining_result<T>& x) const {
            static std::hash<T> hx;
            return hx(x.clause) ^ *((size_t*)((double*)&(x.support_generating_original_pattern)));
        }
    };
}

class KnowledgeBase : public trace_visitor {
    CountTemplate                                   count_table;
    SimplifiedFuzzyStringMatching                   string_values;
    std::unordered_map<std::string, std::unordered_set<std::string>> registerEventLabelSchema;

    std::string currentEventLabel;
    bool alreadySet;
    std::string source;
    std::string name;
    size_t currentEventId;
    ParsingState status;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> universe, universeApprox;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> empty;
    size_t maximumStringLength = 0;

public:
    size_t average_trace_length;
    size_t noTraces;
    size_t actId;
    std::unordered_map<std::string, std::string> schema_def;
    std::unordered_map<std::string, std::vector<std::string>> hierarchy_def;
    static constexpr double    default_double   = 0.0;
    static constexpr size_t    default_size_t   = 0;
    static constexpr long long default_longlong = 0;
    static constexpr bool      default_bool     = false;
    static std::string         default_string;//= "";
    static double    maximum_reliability_for_insertion;

    size_t doTraceCounting(const std::vector<size_t>& toTest) {
        std::unordered_map<std::vector<size_t>, size_t> result;
        size_t nTraces = count_table.nTraces();
        size_t M = toTest.size();
        size_t count = 0;
        for (size_t i = 0; i<nTraces; i++) {
            bool allEventsInTrace = true;
            for (size_t j = 0; j<M; j++) {
                if (count_table.table.at((nTraces)*toTest.at(j)+i).id.parts.event_id == 0) {
                    allEventsInTrace = false;
                    break;
                }
            }
            if (allEventsInTrace) count++;
        }
        return count;
    }

    std::multimap<size_t, std::string> doActCounting() {
        auto res = count_table.actCounting();
        std::multimap<size_t, std::string> Result;
        for (size_t i = 0, N = res.size(); i<N; i++)
            Result.emplace(res.at(i), event_label_mapper.get(i));
        //Result.size();
        return Result;
    }

    std::multimap<size_t, std::string> doTraceCounting() {
        auto res = count_table.traceCounting();
        std::multimap<size_t, std::string> Result;
        for (size_t i = 0, N = res.size(); i<N; i++)
            Result.emplace(res.at(i), event_label_mapper.get(i));
        return Result;
    }


    ActTable                                        act_table_by_act_id;
    yaucl::structures::any_to_uint_bimap<std::string> event_label_mapper;
    std::unordered_map<size_t, size_t> counting_reference;

    std::unordered_map<std::string, AttributeTable> attribute_name_to_table, approximate_attribute_to_table;

    /************************
     * Processing Functions *
     ************************/

    KnowledgeBase();
    void index_data_structures(bool missingDataIndexing);
    void clear();


    uint32_t nTraces() const { return count_table.nTraces(); }
    uint16_t nAct() const { return count_table.nAct(); }
    const CountTemplate& getCountTable() const { return count_table; }


    /***************************
     * Utility print functions *
     ***************************/

    void reconstruct_trace_no_data(std::ostream& os) const;
    void reconstruct_trace_with_data(std::ostream& os) const;
    void dump_for_sqlminer(std::ostream& log, std::ostream& payload, std::ostream& schema_configuration);
    void print_count_table(std::ostream& os) const;
    void print_act_table(std::ostream& os) const;
    void print_attribute_tables(std::ostream& os) const;
    void dump_tab_format(std::ostream& tab) const;
    void dump_xes_format(std::ostream& xes) const;

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


    /***************************
     * Data visiting functions *
     ***************************/
    void index_data_structures();

    void enterLog(const std::string &source, const std::string &name) override;
    void exitLog(const std::string &source, const std::string &name) override;
    size_t enterTrace(const std::string &trace_label) override;
    void exitTrace(size_t traceId) override;
    size_t enterEvent(size_t chronos_tick, const std::string &event_label, size_t consecutivePolyadicEvent, event_t span) override;
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

//    const Result getLastElements(LeafType leafType = NoneLeaf) const;
//    const Result getNotFirstElements(LeafType leafType);

    uint16_t getMappedValueFromAction(const std::string &act) const;
    std::pair<const uint32_t, const uint32_t> resolveCountingData(const std::string &act) const;
    PartialResult range_query(DataPredicate prop, double min_threshold = 1.0, const double c = 2.0) const;

    // Second part of the pipeline
    PartialResult untimed_dataless_exists(const std::pair<const uint32_t, const uint32_t>& indexes, const uint16_t& amount) const;
    PartialResult untimed_dataless_absence(const std::pair<const uint32_t, const uint32_t>& indexes, const uint16_t& amount) const;


    PartialResult exists(const std::pair<const uint32_t, const uint32_t>& indexes) const {
        std::vector<std::pair<std::pair<trace_t, event_t>, double>> foundElems;
        for (auto it = count_table.table.begin() + indexes.first; it != count_table.table.begin() + indexes.second + 1; ++it) {
            foundElems.emplace_back(std::pair<trace_t, event_t>{it->id.parts.trace_id, 0}, 1.0);
        }
        return foundElems;
    }

    Result init(const std::string& act, bool doExtractEvent, const double minThreshold = 1) const;
    Result ends(const std::string& act, bool doExtractEvent, const double minThreshold = 1) const;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> timed_dataless_exists(const std::string& act) const;

    std::pair<ActTable::record*, ActTable::record*> timed_dataless_exists(uint16_t mappedVal) const {
        std::pair<ActTable::record*, ActTable::record*> foundData{nullptr, nullptr};
        auto indexes = act_table_by_act_id.resolve_index(mappedVal);
        if(indexes.first < 0){
            return {nullptr, nullptr};
        }
#if 1
        return {((ActTable::record*)&(*act_table_by_act_id.table.begin())+ indexes.first), ((ActTable::record*)&(*act_table_by_act_id.table.begin())+ indexes.second+1)};
#else
        return {((ActTable::record*)act_table_by_act_id.table.begin().base()+ indexes.first), ((ActTable::record*)act_table_by_act_id.table.begin().base()+ indexes.second+1)};
#endif
    }

    /**
     *
     * @author Samuel 'Sam' Appleby
     *
     * @param isFirst
     * @return
     */
    PartialResult getFirstLastOtherwise(const bool isFirst) const;

    [[deprecated]] Result timed_dataless_exists(const std::string& act, LeafType leafType) const;

    //template <typename traceIdentifier, typename traceValue>
    PartialResult initOrig(const std::string& act, const double minThreshold = 1) const{
        return existsAtOrig(act, 0, minThreshold);
    }

    //template <typename traceIdentifier, typename traceValue>
    PartialResult endsOrig(const std::string& act, const double minThreshold = 1) const{
        return existsAtOrig(act, MAX_UINT16, minThreshold);
    }

    PartialResult existsAtOrig(const std::string& act, const uint16_t& eventId, const double minThreshold = 1) const {
        uint16_t approxConstant = MAX_UINT16 / 2;
        PartialResult foundData;
        PartialResultRecord tracePair;

        const uint16_t& mappedVal = getMappedValueFromAction(act);
        if(mappedVal < 0){
            return foundData;
        }
        std::pair<const uint32_t , const uint32_t> indexes = act_table_by_act_id.resolve_index(mappedVal);
        if(indexes.first < 0){
            return foundData;
        }
        for (auto it = act_table_by_act_id.table.begin() + indexes.first; it != act_table_by_act_id.table.begin() + indexes.second + 1; ++it) {
            auto L = act_table_by_act_id.trace_length.at(it->entry.id.parts.trace_id);
            tracePair.second = getSatisifiabilityBetweenValues(((L <= 1) ? 0 : eventId), cast_to_float2(it->entry.id.parts.event_id, L), approxConstant);
            if(tracePair.second >= minThreshold) {
                tracePair.first.first = it->entry.id.parts.trace_id;
                tracePair.first.second = it->entry.id.parts.event_id;
                foundData.emplace_back(tracePair);
            }
        }
        return foundData;
    }


    Result initOrEnds(const std::string& act, bool beginOrEnd, bool doExtractEvent, const double minThreshold = 1) const;

    std::vector<std::pair<trace_t, event_t>> exact_range_query(DataPredicate prop) const;

    void exact_range_query(const std::string &field_name,
                           const std::unordered_map<std::string, std::vector<size_t>> &ActNameToPredicates,
                           std::vector<std::pair<DataQuery, PartialResult>> &Qs,
                           const std::optional<uint16_t> &temporalTimeMatch = std::optional<uint16_t>{})  const;
private:
    void collectValuesAmongTraces(std::set<union_type> &S, size_t trace_id, act_t acts, bool HasNoAct,
                                  const std::string &attribute_name, bool hasNoAttribute) const;
    void collectValuesAmongTraces(
            std::unordered_map<std::string, std::unordered_map<std::string, std::set<union_type>>> &result,
            std::unordered_map<std::string, std::set<union_type>> &resultOtherValues,
            const std::unordered_map<std::string, std::unordered_set<std::string>> &actToTables,
            const std::unordered_set<std::string> &otherValues, trace_t traceId) const;


    float getSatisifiabilityBetweenValues(const uint16_t& val1, const uint16_t& val2, const uint16_t& approxConstant) const;

    uint16_t getPositionFromEventId(const oid* event) const;
    uint16_t getPositionFromEventId(const std::pair<uint32_t, uint16_t> pair) const;


    std::pair<int, std::vector<std::pair<std::pair<trace_t, event_t>, double>>>
    range_query(DataPredicate &prop, double min_threshold, double correction, const double c, bool forExistingData = true) const;

};




#endif //BZDB_SMALLDATABASE_H
