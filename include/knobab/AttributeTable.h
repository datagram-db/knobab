//
// Created by giacomo on 26/12/20.
//

#ifndef BZDB_ATTRIBUTETABLE_H
#define BZDB_ATTRIBUTETABLE_H

#include <string>
#include <knobab/oid.h>
#include <variant>
#include <yaucl/bpm/structures/commons/DataPredicate.h>
class SimplifiedFuzzyStringMatching;

enum AttributeTableType {
    DoubleAtt,
    SizeTAtt,
    LongAtt,
    StringAtt,
    BoolAtt
    // TODO: hierarchical types!, https://dl.acm.org/doi/10.1145/3410566.3410583
};

using union_type = std::variant<double, size_t, long long, std::string, bool>;

#include <vector>
#include <map>
#include <unordered_map>



struct AttributeTable {
    std::string attr_name;
    SimplifiedFuzzyStringMatching *ptr;
    AttributeTableType type;
    std::unordered_map<std::string, std::vector<size_t>> string_offset_mapping;

    struct record {
        act_t act;
        size_t value;
        size_t act_table_offset;

        record(act_t act, size_t value, size_t actTableOffset);
        DEFAULT_COPY_ASSGN(record);
    };

    std::vector<record> table;


    struct disjunctive_range_query_result {
        /**
         * Constructor to be used when an exact solution is always the case
         */
        disjunctive_range_query_result(const record *begin, const record *end) : exact_solution(begin, end) {}
        DEFAULT_CONSTRUCTORS(disjunctive_range_query_result);

        /**
         * If the query is an approximated query, I cannot efficiently just provide a range of values where the values
         * are known for sure/certain that are going to be returned.
         */
        std::vector<std::pair<const record *, double>> approx_solution;

        /**
         * If the query requires the exact solution, the range of the records is return, without the need of copying the
         * data, inefficiencly, with some data duplication issues. This can be still returned if the query if the query
         * is approximated, and there is still some data that we want to return, and therefore, some exact matches are
         * found
         */
        std::pair<const record *, const record *> exact_solution;
    };

    using range_query_result = std::vector<disjunctive_range_query_result>;

    /**
     * Mapping the act id to the offset of where it appears within the record
     */
    std::vector<std::pair<size_t, size_t>> primary_index;

    /**
     * This data structure maps an offset in the ActTable, identifying a specific event with a specific act/event-label
     * A within a given trace/run, to an offset within the current table, thus stating that there exists an attribute
     * A.attr_name in the payload
     */
    std::unordered_map<size_t, size_t> secondary_index;

    AttributeTable() : attr_name(""), ptr{nullptr}, type{BoolAtt} {}

    AttributeTable(const std::string &attr, AttributeTableType type, SimplifiedFuzzyStringMatching *ptr = nullptr)
            : attr_name(attr), ptr{ptr}, type{type} {}

    AttributeTable(const AttributeTable &) = default;

    AttributeTable(AttributeTable &&) = default;

    AttributeTable &operator=(const AttributeTable &) = default;

    AttributeTable &operator=(AttributeTable &&) = default;

    const record *resolve_record_if_exists(size_t actTableOffset) const;

    std::ostream &resolve_and_print(std::ostream &os, const AttributeTable::record &x) const;

    void record_load(act_t act_id, const union_type &val, trace_t tid, event_t eid);

    void index(const std::vector<std::vector<size_t>> &trace_id_to_event_id_to_offset);

    union_type resolve(const record &x) const;

    /**
     *
     * @param prop              Predicate that we want to test
     * @param act               Act over which we want to perform the query. If -1, then it means that we want to retrieve them all
     * @param maximumApprox     If zero, it means that we only want to have exact queries. Otherwise, if a positive value is given,
     *                          this provides the maximum degree of approximation that is expected
     * @param c                 For distance functions over non-strings, it provides the distance among all of the elements that have
     *                          less distance than the given one
     * @return
     */
    range_query_result
    range_query(DataPredicate prop, ssize_t act = -1, double maximumApprox = 0.0, const double c = 2.0) const;

private:
    std::vector<std::map<union_type, std::vector<std::pair<trace_t, event_t>>>> elements;

    size_t storeLoad(const union_type &x);

    bool assertVariant(const union_type &val);

    bool range_query(size_t actId, const DataPredicate &prop, AttributeTable::range_query_result &result,
                     bool isNotExactMatch, double min_threshold) const ;

    template <typename T>
    void
    getIterator(double min_threshold, AttributeTable::disjunctive_range_query_result &thisResult,
                SimplifiedFuzzyStringMatching &sfzm,
                const T& i) const {
        std::multimap<double, std::string> result;
        sfzm.fuzzyMatch(min_threshold, 1, get<std::string>(resolve(*i)), result); // IDE has a problem, but this is going to be expanded in the Cpp!
        if (!result.empty()) {
            for (const auto& ref : result) {
                auto it2 = string_offset_mapping.find(ref.second);
                if (it2 != string_offset_mapping.end()) {
                    for (const size_t offset : it2->second) {
                        thisResult.approx_solution.emplace_back((const record*)(&this->table[offset]), ref.first);
                    }
                }
            }
        }
    }

};

#endif //BZDB_ATTRIBUTETABLE_H
