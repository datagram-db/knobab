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
    FinishParsing,
    MissingDataParsing
};

#include <bitset>
#include "knobab/dataStructures/CountTableFPTree.h"
#include "knobab/algorithms/mining/DataMiningMetrics.h"
#include "knobab/algorithms/mining/RulesFromFrequentItemset.h"
#include "yaucl/functional/iterators.h"

#include <knobab/trace_repairs/DataQuery.h>
#include <knobab/operators/LTLFOperators.h>


using trace_set = std::bitset<sizeof(uint32_t)>;
using act_set = std::bitset<sizeof(uint16_t)>;

//union_minimal resolveUnionMinimal(const AttributeTable &table, const AttributeTable::record &x);
const uint16_t MAX_UINT16 = std::pow(2, 16) - 1;


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
    size_t noTraces;
    size_t actId;
    static constexpr double    default_double   = 0.0;
    static constexpr size_t    default_size_t   = 0;
    static constexpr long long default_longlong = 0;
    static constexpr bool      default_bool     = false;
    static std::string         default_string;//= "";
    static double    maximum_reliability_for_insertion;


    std::multimap<size_t, std::string> doActCounting() {
        auto res = count_table.actCounting();
        std::multimap<size_t, std::string> Result;
        for (size_t i = 0, N = res.size(); i<N; i++)
            Result.emplace(res.at(i), event_label_mapper.get(i));
        Result.size();
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




    /***************************
     * Utility print functions *
     ***************************/

    void reconstruct_trace_no_data(std::ostream& os) const;
    void reconstruct_trace_with_data(std::ostream& os) const;
    void dump_for_sqlminer(std::ostream& log, std::ostream& payload, std::ostream& schema_configuration);
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



    /** Pattern mining **/
    std::vector<std::pair<double,DeclareDataAware>> pattern_mining(double support,
                        bool naif,
                        bool init_end) {
        support = std::max(std::min(support, 1.0), 0.0); // forcing the value to be between 0 and 1.
        size_t log_size = count_table.nTraces();
        uint64_t minimum_support_threshold = std::min((uint64_t)std::ceil((double)log_size * support), log_size);
        uint64_t max_act_id = count_table.nAct();
        FPTree t{count_table, minimum_support_threshold, max_act_id};
        std::vector<std::pair<double,DeclareDataAware>> declarative_clauses;
        bool doInitA = false;
        auto result = fptree_growth(t, 2);
        std::set<Pattern> binary_patterns;
        std::unordered_set<act_t> unary_patterns_for_non_exact_support;
        for (const auto& x : result) {
            if (x.first.size() == 1) {
                if (support == 1.0) {
                    // If the support is actually one, then we can infer that if an event
                    // exists in all of the events, that should always happen
                    if (naif) {
                        // The naif algorithm will just state that, if the pattern exists,
                        // then it exists at least once
                        DeclareDataAware clause;
                        clause.left_act = event_label_mapper.get(*x.first.begin());
                        clause.n = 1;
                        clause.casusu = "Exists1";
                        declarative_clauses.emplace_back(support, clause);
                    } else {
                        // The non-naif version is exploiting the couting information from
                        // the counting table, and also providing an expected number of times
                        // the event should happen. You can also apply the same reasoning
                        // for mining the absence pattern
                        DeclareDataAware clause;
                        clause.left_act = event_label_mapper.get(*x.first.begin());
                        event_t n = std::numeric_limits<event_t>::max(),
                                N = 0;
                        auto cp = count_table.resolve_primary_index2(*x.first.begin());
                        while (cp.first != cp.second) {
                            if (cp.first->id.parts.event_id > 0) {
                                if (cp.first->id.parts.event_id < n) {
                                    n = cp.first->id.parts.event_id;
                                }
                                if (cp.first->id.parts.event_id > N) {
                                    N = cp.first->id.parts.event_id;
                                }
                            }
                            cp.first++;
                        }
                        clause.n = n;
                        clause.casusu = "Exists";
                        declarative_clauses.emplace_back(support, clause);
                        clause.n = N+1;
                        clause.casusu = "Absence";
                        declarative_clauses.emplace_back(support, clause);
                    }
                } else {
                    // If the support is less than one, then we cannot state that
                    // all of these events actually exist in all of the possible traces, and
                    // we need to relax such conditions into a choice between the patterns,
                    // as this will maximise the score, precision-wise.
                    // We are postponing such discussion into point A)
                    unary_patterns_for_non_exact_support.insert(*x.first.begin());
                    doInitA = true;
                }
            } else {
                binary_patterns.insert(x);
            }
        }
        result.clear();

        // Point A): initialisation.
        // Please observe that, given the support definition for traditional
        // event mining, I can extract a Choice pattern only when the support is
        // less than one, otherwise this choice can be rewritten simply as an exists
        std::vector<std::vector<act_t>> map;
        std::vector<std::vector<trace_t>> inv_map;
        std::set<std::vector<act_t>> S;
        if (doInitA) {
            map.insert(map.begin(), (log_size), std::vector<act_t>{});
            inv_map.insert(inv_map.begin(), max_act_id, std::vector<trace_t>{});
            for (const act_t& act_id : unary_patterns_for_non_exact_support) {
                for (size_t trace_id = 0; trace_id < log_size; trace_id++) {
                    event_t count = count_table.resolve_length(act_id, trace_id);
                    if (count > 0) {
                        map[trace_id].emplace_back(act_id);
                        inv_map[act_id].emplace_back(trace_id);
                    }
                }
            }
        }
        unary_patterns_for_non_exact_support.clear();

        for (auto& v : map) {
            std::sort(v.begin(), v.end());
        }
        for (auto& v : inv_map) {
            std::sort(v.begin(), v.end());
        }
        std::pair<act_t, act_t> curr_pair, inv_pair;
        std::unordered_set<std::pair<act_t, act_t>> visited_pairs;
        // Point A)
        for (const auto& v : map)
            S.insert(v);
        for (const auto& x : S) {
            for (const auto& y : S) {
                if (x != y) {
                    for (const auto& a : x) {
                        curr_pair.first = inv_pair.second = a;
                        for (const auto& b : y) {
                            if (b == a) continue;
                            curr_pair.second = inv_pair.first = b;
                            if ((!visited_pairs.emplace(curr_pair).second) ||
                                    (!visited_pairs.emplace(inv_pair).second)) continue;
                            const auto& aSet = inv_map.at(a);
                            const auto& bSet = inv_map.at(b);
                            std::pair<size_t, size_t> ratio = yaucl::iterators::ratio(aSet.begin(), aSet.end(), bSet.begin(), bSet.end());
                            double local_support = ((double)(ratio.first)) / ((double)log_size);
                            if (ratio.first >= minimum_support_threshold) {
                                // I can consider this pattern, again, only if it is within the expected
                                // support rate which, in this case, is given by the amount of traces
                                // globally setting up this pattern
                                DeclareDataAware clause;
                                clause.left_act = event_label_mapper.get(a);
                                clause.right_act = event_label_mapper.get(b);
                                clause.n = 1;
                                clause.casusu = "Choice";
                                declarative_clauses.emplace_back(local_support, clause);
                                if ((!naif) && ratio.second == 0) {
                                    // If there is no intersection, I can also be more strict if I want,
                                    // and provide an exclusive choice pattern if I am confident that
                                    // the two events will never appear in the same trace (according to
                                    // the "training" data
                                    clause.casusu = "ExclChoice";
                                    declarative_clauses.emplace_back(local_support, clause);
                                }
                            }
                        }
                    }
                }
            }
        }
        map.clear();
        inv_map.clear();

        DataMiningMetrics<act_t> counter{binary_patterns};
        // For each frequent itemset, generate a set of possible relevant rules
        for (const Pattern& pattern : binary_patterns) {
            if (pattern.first.size() <= 1) continue;                            // I cannot extract any significant rule from a singleton!
            RulesFromFrequentItemset<act_t> rffi{counter};                         // Generate the top rule
            for (const auto& result: rffi.generate_hypotheses(pattern)) {            // Generate the hypotheses containing a lift greater than one
                if (result.tail.empty()) {
                    // CoExistence pattern
                    DeclareDataAware clause;
                    clause.left_act = event_label_mapper.get(result.head.at(0));
                    clause.right_act = event_label_mapper.get(result.head.at(1));
                    clause.n = 1;
                    clause.casusu = "CoExistence";
                    declarative_clauses.emplace_back(support, clause);
                } else if (result.tail.size() == 1) {
                    // Doing some further finicky processing to refine which kind of implication
                    bool canBeRefined = false;
                    if (canBeRefined) {
                        // the result that was computed before
                    } else {
                        DeclareDataAware clause;
                        clause.left_act = event_label_mapper.get(result.head.at(0));
                        clause.right_act = event_label_mapper.get(result.tail.at(0));
                        clause.n = 1;
                        clause.casusu = "RespExistence";
                        declarative_clauses.emplace_back(support, clause);
                    }
                }
            }
        }


        if (init_end) {
            // This is not directly mined via the frequent mining algorithm but,
            // still, if needed, this can be obtained via an easy linear scan of the
            // secondary index of the knowledge base
            std::vector<size_t> first(max_act_id, 0), last(max_act_id, 0);
            for (size_t trace_id = 0; trace_id < log_size; trace_id++) {
                auto first_last = act_table_by_act_id.secondary_index.at(trace_id);
                first[first_last.first->entry.id.parts.act]++;
                last[first_last.second->entry.id.parts.act]++;
            }
            for (size_t act_id = 0; act_id < max_act_id; act_id++) {
                DeclareDataAware clause;
                clause.n = 1;
                auto first_occ = first.at(act_id), last_occ = last.at(act_id);
                if (first_occ > minimum_support_threshold) {
                    clause.casusu = "Init";
                    clause.left_act = event_label_mapper.get(act_id);
                    declarative_clauses.emplace_back(((double)(first_occ)) / ((double)log_size), clause);
                }
                if (last_occ > minimum_support_threshold) {
                    clause.casusu = "End";
                    clause.left_act = event_label_mapper.get(act_id);
                    declarative_clauses.emplace_back(((double)(last_occ)) / ((double)log_size), clause);
                }
            }

        }
        return declarative_clauses;
    }



    /******************************
     * First part of the pipeline *
     ******************************/

    const Result getLastElements(LeafType leafType = NoneLeaf) const;
    const Result getNotFirstElements(LeafType leafType);

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
