//
// Created by giacomo on 11/03/2022.
//
#include <knobab/server/operators/simple_ltlf_operators.h>
#include <yaucl/numeric/ssize_t.h>

#ifndef KNOBAB_NOVEL_LTLF_OPERATORS_H
#define KNOBAB_NOVEL_LTLF_OPERATORS_H

#include <knobab/server/tables/KnowledgeBase.h>

inline void and_next(const Result &lhsOperand,
                                 Result& result,
                                 const KnowledgeBase& kb,
                                 act_t right_activity,
                                 const PredicateManager* right_predicate = nullptr,
                                 const PredicateManager* correlation = nullptr) {
    std::unordered_set<std::string> cache;
    if (right_predicate)
        DEBUG_ASSERT(right_predicate->kb == &kb);
    for (const auto& x : lhsOperand) {
        if ((kb.act_table_by_act_id.getTraceLength(x.first.first)-1) == (x.first.second))
            continue;
        size_t right_offset = kb.act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(x.first.first).at(x.first.second+1);
        if ((kb.act_table_by_act_id.table.at(right_offset).entry.id.parts.act != right_activity))
            continue;
        bool rightMatch = !right_predicate ? true : right_predicate->checkValidity(false, x.first.first, x.first.second);
        if (rightMatch) {
            if (correlation) {
                env e1 = correlation->GetPayloadDataFromEvent(x.first.first, x.first.second, true, cache);
                if (correlation->checkValidity(e1, x.first.first, x.first.second+1)) {
                    result.emplace_back(x).second.second.emplace_back(marked_event::right(x.first.second+1));
                }
            } else {
                result.emplace_back(x).second.second.emplace_back(marked_event::right(x.first.second+1));
            }
        }
    }
}

inline void next_and(const Result &lhsOperand,
                     Result& result,
                     const KnowledgeBase& kb,
                     act_t right_activity,
                     const PredicateManager* right_predicate = nullptr,
                     const PredicateManager* correlation = nullptr) {
    std::unordered_set<std::string> cache;
    if (right_predicate)
        DEBUG_ASSERT(right_predicate->kb == &kb);
    for (const auto& x : lhsOperand) {
        if (x.first.first == 0) continue;
        size_t right_offset = kb.act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(x.first.first).at(x.first.second-1);
        if ((kb.act_table_by_act_id.table.at(right_offset).entry.id.parts.act != right_activity))
            continue;
        bool rightMatch = !right_predicate ? true : right_predicate->checkValidity(false, x.first.first, x.first.second);
        if (rightMatch) {
            if (correlation) {
                env e1 = correlation->GetPayloadDataFromEvent(x.first.first, x.first.second, true, cache);
                if (correlation->checkValidity(e1, x.first.first, x.first.second-1)) {
                    result.emplace_back(x).second.second.emplace_back(marked_event::right(x.first.second-1));
                }
            } else {
                result.emplace_back(x).second.second.emplace_back(marked_event::right(x.first.second-1));
            }
        }
    }
}

/**
 *
 * @author Giacomo Bergami
 *
 * @param aResult
 * @param bResult
 * @param result
 * @param manager
 * @param lengths
 */
inline void aAlternateAndFutureB(const Result& aResult, const Result& bResult, Result& result, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    if (bResult.empty()) {
        result.clear();
        return;
    }
    auto bCurrent = bResult.begin(), bEnd = bResult.end();
    ResultRecord rcx;
    marked_event join = marked_event::join(0,0);
    bool hasMatch;
    std::unordered_set<std::string> cache;

    for (auto aCurrent = aResult.begin(), aEnd = aResult.end(); aCurrent != aEnd; /*++d_first*/) {
        /* Our iterators should never be pointing at the same event */
        // assert(aCurrent != bCurrent);

        if (aCurrent->first > bCurrent->first) {
            bCurrent++;
            if (bCurrent == bEnd) break;
        } else {
            auto newItr = bCurrent;
            rcx.first = aCurrent->first;
            rcx.second.second.clear();
            rcx.second.first = 1.0;
            hasMatch = false;
            auto aNext = aCurrent+1;
            if ((aNext != aEnd) && (aNext->first.first == rcx.first.first) && (aNext->first < bCurrent->first)) {
                aCurrent++;
                continue;
            }

            while (newItr != bEnd) {
                if(newItr->first.first != aCurrent->first.first){
                    break;
                }
                if (manager && (!aCurrent->second.second.empty()) && (!newItr->second.second.empty())) {
                    for (const auto &elem: aCurrent->second.second) {
                        if (!IS_MARKED_EVENT_ACTIVATION(elem)) continue;
                        join.id.parts.left = GET_ACTIVATION_EVENT(elem);
                        env e1 = manager->GetPayloadDataFromEvent(aCurrent->first.first, join.id.parts.left, true, cache);
                        for (const auto &elem1: newItr->second.second) {
                            if (!IS_MARKED_EVENT_TARGET(elem1)) continue;
                            join.id.parts.right = GET_TARGET_EVENT(elem1);

                            if (manager->checkValidity(e1, newItr->first.first, join.id.parts.right)) {
                                hasMatch = true;
                                rcx.second.second.push_back(join);
                                rcx.second.first *= (1.0 - std::min(aCurrent->second.first, newItr->second.first));
                            }
                        }
                    }
                } else {
                    hasMatch = true;
                    rcx.second.second.insert(rcx.second.second.end(), newItr->second.second.begin(), newItr->second.second.end());
                    if (manager)
                        rcx.second.second.insert(rcx.second.second.end(), aCurrent->second.second.begin(), aCurrent->second.second.end());
                }
                newItr++;
            }

            if (hasMatch) {
                if (!manager) rcx.second.second.insert(rcx.second.second.end(), aCurrent->second.second.begin(), aCurrent->second.second.end());
                remove_duplicates(rcx.second.second);
                if (manager) rcx.second.first = 1.0 - rcx.second.first;
                result.emplace_back(rcx);
            }

            if (aCurrent->first == bCurrent->first) {
                bCurrent++;
                if (bCurrent == bEnd) break;
            }

            aCurrent++;
        }
    }
}

/**
 *
 * @author Giacomo Bergami
 *
 * @param aResult
 * @param bResult
 * @param result
 * @param manager
 * @param lengths
 */
inline void aWeakAlternateAndFutureB(const Result& aResult, const Result& bResult, Result& result, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    if (bResult.empty()) {
        result.clear();
        return;
    }
    auto bCurrent = bResult.begin(), bEnd = bResult.end();
    ResultRecord rcx;
    marked_event join = marked_event::join(0,0);
    bool hasMatch;
    std::unordered_set<std::string> cache;

    for (auto aCurrent = aResult.begin(), aEnd = aResult.end(); aCurrent != aEnd; /*++d_first*/) {
        if (aCurrent->first > bCurrent->first) {
            bCurrent++;
            if (bCurrent == bEnd) break;
        } else {
            auto newItr = bCurrent;
            rcx.first = aCurrent->first;
            rcx.second.second.clear();
            rcx.second.first = 1.0;
            hasMatch = false;
            if (lengths.at(aCurrent->first.first)-1==aCurrent->first.second)
                hasMatch = false;
            else if (bCurrent->first.first > aCurrent->first.first) {
                auto aNext = aCurrent+1;
                if ((aNext != aEnd) && (aNext->first.first == rcx.first.first))
                    hasMatch = false;
                else
                    hasMatch = true;
            } else {
                auto aNext = aCurrent+1;
                if ((aNext != aEnd) && (aNext->first.first == rcx.first.first) && (aNext->first < bCurrent->first)) {
                    aCurrent++;
                    continue;
                }

                while (newItr != bEnd) {
                    if(newItr->first.first != aCurrent->first.first){
                        break;
                    }
                    if (manager && (!aCurrent->second.second.empty()) && (!newItr->second.second.empty())) {
                        for (const auto &elem: aCurrent->second.second) {
                            if (!IS_MARKED_EVENT_ACTIVATION(elem)) continue;
                            join.id.parts.left = GET_ACTIVATION_EVENT(elem);
                            env e1 = manager->GetPayloadDataFromEvent(aCurrent->first.first, join.id.parts.left, true, cache);
                            for (const auto &elem1: newItr->second.second) {
                                if (!IS_MARKED_EVENT_TARGET(elem1)) continue;
                                join.id.parts.right = GET_TARGET_EVENT(elem1);

                                if (manager->checkValidity(e1, newItr->first.first, join.id.parts.right)) {
                                    hasMatch = true;
                                    rcx.second.second.push_back(join);
                                    rcx.second.first *= (1.0 - std::min(aCurrent->second.first, newItr->second.first));
                                }
                            }
                        }
                    } else {
                        hasMatch = true;
                        rcx.second.second.insert(rcx.second.second.end(), newItr->second.second.begin(), newItr->second.second.end());
                        if (manager)
                            rcx.second.second.insert(rcx.second.second.end(), aCurrent->second.second.begin(), aCurrent->second.second.end());
                    }
                    newItr++;
                }
            }

            if (hasMatch) {
                if (!manager) rcx.second.second.insert(rcx.second.second.end(), aCurrent->second.second.begin(), aCurrent->second.second.end());
                remove_duplicates(rcx.second.second);
                if (manager) rcx.second.first = 1.0 - rcx.second.first;
                result.emplace_back(rcx);
            }

            if (aCurrent->first == bCurrent->first) {
                bCurrent++;
                if (bCurrent == bEnd) break;
            }

            aCurrent++;
        }
    }
}

// ImplyAndAltUntilOrAbsence

#endif //KNOBAB_FAST_LTLF_OPERATORS_H
