//
// Created by giacomo on 26/12/20.
//

#include "knobab/AttributeTable.h"
#include "SimplifiedFuzzyStringMatching.h"
#include <cassert>

static inline union_type cast_unions(AttributeTableType type, const union_minimal &x) {
    switch (type) {
        case DoubleAtt:
            return std::get<double>(x);
        case LongAtt:
            return (long long)std::get<double>(x);
        case StringAtt:
            return  std::get<std::string>(x);
        case BoolAtt:
            return std::abs(std::get<double>(x)) > std::numeric_limits<double>::epsilon();
            //case SizeTAtt:
        default:
            // TODO: hierarchical types!, https://dl.acm.org/doi/10.1145/3410566.3410583
            return (size_t)std::get<double>(x);
    }
}

static inline double similarityFunction(const union_type& lhs, const union_type& rhs, double c = 2.0) {
    if (lhs.index() != rhs.index())
        return std::numeric_limits<double>::max(); // If the elements are not of the same type, then they are associated to the maximum distance.
    if (std::holds_alternative<double>(lhs)) {
        return 1.0 / ( (std::abs(std::get<double>(lhs)- std::get<double>(rhs)))/c + 1.0);
    } else if (std::holds_alternative<size_t>(lhs)) {
        auto l = std::get<size_t>(lhs);
        auto r = std::get<size_t>(rhs);
        return 1.0 / ( ((double)(((l > r) ? (l-r) : (r-l))))/c + 1.0);
    } else if (std::holds_alternative<long long>(lhs)) {
        return 1.0 / ( (std::abs((double)std::get<long long>(lhs)- (double)std::get<long long>(rhs)))/c + 1.0);
    }

}

union_type AttributeTable::resolve(const AttributeTable::record &x) const {
    switch (type) {
        case DoubleAtt:
            return *(double*)(&x.value);
        case LongAtt:
            return *(long long*)(&x.value);
        case StringAtt:
            assert(ptr);
            return ptr->get(x.value);
        case BoolAtt:
            return x.value != 0;
        //case SizeTAtt:
        default:
            // TODO: hierarchical types!, https://dl.acm.org/doi/10.1145/3410566.3410583
            return x.value;
    }
}

bool AttributeTable::assertVariant(const std::variant<double, size_t, long long int, std::string, bool> &val) {
    switch (type) {
        case DoubleAtt:
            return std::holds_alternative<double>(val);
        case SizeTAtt:
            return std::holds_alternative<size_t>(val);
        case LongAtt:
            return std::holds_alternative<long long>(val);
        case StringAtt:
            return std::holds_alternative<std::string>(val);
        case BoolAtt:
            return std::holds_alternative<bool>(val);
        default:
            return false;
    }
}

void
AttributeTable::record_load(act_t act_id, const std::variant<double, size_t, long long int, std::string, bool> &val,
                            trace_t tid, event_t eid) {
    assert(assertVariant(val));
    while (elements.size() <= act_id)
        elements.emplace_back();
    elements[act_id][val].emplace_back(tid, eid);
}

size_t AttributeTable::storeLoad(const std::variant<double, size_t, long long int, std::string, bool> &x) {
    switch (type) {
        case DoubleAtt: {
            double tmp = std::get<double>(x);
            return *(size_t*)(&tmp);
        }

        case LongAtt: {
            long long tmp = std::get<long long>(x);
            return *(size_t*)(&tmp);
        }

        case StringAtt: {
            std::string tmp = std::get<std::string>(x);
            return ptr->put(tmp).first;
        }
        case BoolAtt:
            return std::get<bool>(x) ? 1 : 0;
            //case SizeTAtt:
        default:
            // TODO: hierarchical types!, https://dl.acm.org/doi/10.1145/3410566.3410583
            return std::get<size_t>(x);
    }
}

void AttributeTable::index(const std::vector<std::vector<size_t>> &trace_id_to_event_id_to_offset) {
    for (size_t act_id = 0, N = elements.size(); act_id < N; act_id++) {
        auto& ref = elements[act_id];
        size_t begin = table.size();
        if (!ref.empty()) {
            std::map<union_type, std::vector<size_t>> valueToOffsetInTable;
            for (const auto& val_offset : ref) {
                for (const auto& traceid_eventid : val_offset.second) {
                    size_t offset =
                            trace_id_to_event_id_to_offset.at(traceid_eventid.first).at(traceid_eventid.second);
                    secondary_index[offset] = table.size();
                    valueToOffsetInTable[val_offset.first].emplace_back(offset);
                }
            }
            for (auto it = valueToOffsetInTable.begin(); it != valueToOffsetInTable.end(); ) {
                std::sort(it->second.begin(), it->second.end());
                size_t val = storeLoad(it->first);
                std::string current_string;
                if (type == StringAtt)
                    current_string = std::get<std::string>(it->first);
                for (const auto& refx : it->second) {
                    if (type == StringAtt)
                        string_offset_mapping[current_string].emplace_back(table.size());
                    table.emplace_back(act_id, val, refx);
                }
                it = valueToOffsetInTable.erase(it);
            }
            ref.clear();
        }
        size_t end = table.size();
        primary_index.emplace_back(begin, end);
    }
    elements.clear();
}

const AttributeTable::record * AttributeTable::resolve_record_if_exists(size_t actTableOffset) const {
    auto it = secondary_index.find(actTableOffset);
    if (it == secondary_index.end())
        return nullptr;
    else
        return (table.data()) + it->second;
}

std::ostream &AttributeTable::resolve_and_print(std::ostream &os, const AttributeTable::record &x) const {
    switch (type) {
        case DoubleAtt:
            return os << *(double*)(&x.value);
        case LongAtt:
            return os << *(long long*)(&x.value);
        case StringAtt:
            assert(ptr);
            return os << ptr->get(x.value);
        case BoolAtt:
            return os << ((x.value != 0) ? "true" : "false");
            //case SizeTAtt:
        default:
            // TODO: hierarchical types!, https://dl.acm.org/doi/10.1145/3410566.3410583
            return os << x.value;
    }
}

#include <sstream>

AttributeTable::range_query_result AttributeTable::range_query(DataPredicate prop, ssize_t act, double min_threshold, const double c) const {
    constexpr size_t max_int = std::numeric_limits<size_t>::max();
    assert(prop.label == this->attr_name); // Just for consistency checking, I need to evaluate the predicate over the specific table. This is also for efficiency reasons
    assert(min_threshold >= 0.0); // Cannot have a negative approximation, as the approximation is just a distance
    if (!prop.labelRHS.empty()) {
        std::stringstream sstr;
        sstr << "Predicate " << prop << ": cannot have a predicate over two distinct variables! ";
        throw std::runtime_error(sstr.str());
    }
    if (!prop.exceptions.empty()) {
        std::stringstream sstr;
        sstr << "Predicate " << prop << ": cannot have excepted values at this stage: this should be an already decomposed interval ";
        throw std::runtime_error(sstr.str());
    }
    if (!prop.BiVariableConditions.empty()) {
        std::stringstream sstr;
        sstr << "Predicate " << prop << ": cannot variable conditions that are proper to join conditions: either the decomposition is faulty, or the data clearing is not effective, or the interval decomposition isn't, or the interval is used inappropriately ";
        throw std::runtime_error(sstr.str());
    }
    bool isNotExactMatch = min_threshold > std::numeric_limits<double>::epsilon();
    bool doWeHaveStrings = this->type == StringAtt;
    if (doWeHaveStrings)
        assert(ptr); // we need ptr for approximate string matching!
    SimplifiedFuzzyStringMatching matchIfAttrIsString;
    if ((prop.casusu != TTRUE) && (prop.casusu != INTERVAL)) {
        prop.asInterval();// If it is not an interval, make that as so, so we need to deal with only one case!
    }

    // At this stage, we know for sure that this element should be an interval at most.
    AttributeTable::range_query_result result;
    if (act == -1) {
        for (size_t actId = 0, N = primary_index.size(); actId < N; actId++) {
            range_query(actId, prop, result, isNotExactMatch, min_threshold);
        }
    } else {
        AttributeTable::range_query_result result;
        range_query(act, prop, result, isNotExactMatch, min_threshold);
    }
    return result;
}

bool AttributeTable::range_query(size_t actId,
                                 const DataPredicate &prop,
                                 AttributeTable::range_query_result &result,
                                 bool isNotExactMatch,
                                 double min_threshold) const {
    if (actId > primary_index.size()) return false; // missing act
    auto it = primary_index.at(actId);
    if (it.first == it.second)
        return false; // missing attribute ~ for the meantime, we are not approximating the match on the attribute name, but we should in the future

    switch (prop.casusu) {
        case TTRUE:

            // TTRUE -> RETURN everything
            // As the interval is true, return all of the elements that are in there
            result.emplace_back(&table[it.first], &table[it.second]);
            return true;
            break;

        case INTERVAL: {
            auto table_leftValue = resolve(table[it.first]);
            auto table_rightValue = resolve(table[it.second]);

            const union_type prop_leftValue = cast_unions(type, prop.value);
            const union_type prop_rightValue = cast_unions(type, prop.value_upper_bound);

            auto begin = table.begin() + it.first;
            auto end = table.begin() + it.second;

            auto lb = std::lower_bound(begin, end, prop_leftValue, [&](const record &r, const union_type &value) {
                return resolve(r) < value;
            });

            bool hasSolution = false;
            if (lb != end) {
                hasSolution = true;
                auto ub = std::upper_bound(begin, end, prop_rightValue, [&](const union_type &value, const record &r) {
                    return resolve(r) < value;
                });

                auto &thisResult = result.emplace_back();
                thisResult.exact_solution.first = (const AttributeTable::record *) &lb;
                thisResult.exact_solution.second = (const AttributeTable::record *) &ub;

                if (isNotExactMatch) {
                    // this computation shall be performed only if we also need to provide the approximated match
                    if (type == StringAtt) {
                        SimplifiedFuzzyStringMatching sfzm;
                        for (auto i = lb; i != ub; ++i) {
                            // Filling the approximate match element with the items of interest
                            sfzm.put(get<std::string>(resolve(*i)));
                        }

                        // Retrieving the best candidate for each element of the interval: we cannot do better than this...
                        for (auto i = begin; i != lb; ++i) {
                            getIterator(min_threshold, thisResult, sfzm, i);
                        }
                        for (auto i = ub; i != end; ++i) {
                            getIterator(min_threshold, thisResult, sfzm, i);
                        }
                    } else {
                        for (auto i = lb - 1; i <= begin; i--) {
                            double thisValue = similarityFunction(resolve(*i), prop_leftValue);
                            if (thisValue < min_threshold)
                                break; // Stop the iteration if we reached the maximum part
                            thisResult.approx_solution.emplace_back((const AttributeTable::record *) &i, thisValue);
                        }
                        for (auto i = end - 1; i <= ub; i--) {
                            double thisValue = similarityFunction(resolve(*i), prop_rightValue);
                            if (thisValue < min_threshold)
                                break; // Stop the iteration if we reached the maximum part
                            thisResult.approx_solution.emplace_back((const AttributeTable::record *) &i, thisValue);
                        }
                    }
                    std::sort(thisResult.approx_solution.begin(), thisResult.approx_solution.end());
                }
            } else {
                // No solution found! only approximations are admissable.
                if (type == StringAtt) {
                    // Instead of scanning the whole records, just use ptr, and then perform the intersection!

                    std::unordered_map<std::string, double> aggr;
                    {
                        std::multimap<double, std::string> result;
                        ptr->fuzzyMatch(min_threshold, 1, std::get<std::string>(prop_leftValue), result);
                        for (const auto &cp: result) {
                            auto it = aggr.emplace(cp.second, cp.first);
                            if (!it.second) {
                                it.first->second = std::max(it.first->second, cp.first);
                            }
                        }
                    }
                    {
                        std::multimap<double, std::string> result;
                        ptr->fuzzyMatch(min_threshold, 1, std::get<std::string>(prop_rightValue), result);
                        for (const auto &cp: result) {
                            auto it = aggr.emplace(cp.second, cp.first);
                            if (!it.second) {
                                it.first->second = std::max(it.first->second, cp.first);
                            }
                        }
                    }

                    if (!aggr.empty()) {
                        auto &thisResult = result.emplace_back();
                        for (const auto &ref: aggr) {
                            auto it2 = string_offset_mapping.find(ref.first);
                            if (it2 != string_offset_mapping.end()) {
                                for (const size_t offset: it2->second) {
                                    thisResult.approx_solution.emplace_back((const record *) (&this->table[offset]),
                                                                            ref.second);
                                }
                            }
                        }
                    }
                    return !aggr.empty();
                } else {
                    disjunctive_range_query_result *current_result = nullptr;
                    for (auto i = end - 1; i <= begin; i--) {
                        double thisValue = similarityFunction(resolve(*i), prop_leftValue);
                        if (thisValue < min_threshold)
                            break; // Stop the iteration if we reached the maximum part
                        if (!current_result) {
                            result.emplace_back();
                            current_result = result.data() + (result.size() - 1);
                        }
                        current_result->approx_solution.emplace_back((const AttributeTable::record *) &i, thisValue);
                    }
                    for (auto i = begin; i < end; i++) {
                        double thisValue = similarityFunction(resolve(*i), prop_rightValue);
                        if (thisValue < min_threshold)
                            break; // Stop the iteration if we reached the maximum part
                        if (!current_result) {
                            result.emplace_back();
                            current_result = result.data() + (result.size() - 1);
                        }
                        current_result->approx_solution.emplace_back((const AttributeTable::record *) &i, thisValue);
                    }
                    if (current_result)
                        std::sort(current_result->approx_solution.begin(), current_result->approx_solution.end());
                    return current_result != nullptr;
                }
            }
        }
            break;

        default:
            throw std::runtime_error("UNEXPECTED CASE!");
    }
    return true;
}






AttributeTable::record::record(act_t act, size_t value, size_t actTableOffset) : act(act), value(value),
                                                                                 act_table_offset(actTableOffset) {}
