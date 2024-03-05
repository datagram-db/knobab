//
// Created by Giacomo Bergami on 11/03/2022.
//
#include <knobab/server/operators/simple_ltlf_operators.h>
#include <yaucl/numeric/ssize_t.h>

#ifndef KNOBAB_NOVEL_LTLF_OPERATORS_H
#define KNOBAB_NOVEL_LTLF_OPERATORS_H

#include <knobab/server/algorithms/atomization/AtomizingPipeline.h>
#include <knobab/server/tables/KnowledgeBase.h>

inline void and_next(const Result &lhsOperand,
                                 Result& result,
                                 const KnowledgeBase& kb,
                                 const AtomizingPipeline* ap,
                                 const std::set<std::string>& rhs_atoms,
                                 const PredicateManager* correlation = nullptr,bool polyadic = true) {
    std::unordered_set<std::string> cache;
    act_t right_activity;
    bool right_predicate;
    if ((!ap) ||((rhs_atoms.size() == 1) && ap->act_atoms.contains(*rhs_atoms.begin()))) {
        right_predicate = false;
        ssize_t tmp = kb.event_label_mapper.signed_get(*rhs_atoms.begin());
        if (tmp == -1) return;
        right_activity = tmp;
    } else {
        right_predicate = true;
        ssize_t tmp = kb.event_label_mapper.signed_get(ap->atom_to_conjunctedPredicates.at(*rhs_atoms.begin()).at(0).label);
        if (tmp == -1) return;
        right_activity = tmp;
    }
    event_t count = 0;
    auto cp = kb.getCountTable().resolve_primary_index2(right_activity);
    auto iter = cp.first;
    while (iter != cp.second) {
        count += iter->id.parts.event_id;
        if (count > 0) break;
        iter++;
    }
    if (count == 0) return;
    for (const auto& x : lhsOperand) {
        if (cp.first[x.first.first].id.parts.event_id == 0) continue;
        auto L =(kb.act_table_by_act_id.getTraceLength(x.first.first));
        if (((L-1) == (x.first.second)) || (x.first.second+(polyadic?x.second.first:1)>=L))
            continue;
        const auto& right_offsets = kb.act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(x.first.first).at(x.first.second+x.second.first);
        bool found = right_offsets.find(right_activity) != right_offsets.end();
        if (!found) continue;
        bool rightMatch = true;
        if (right_predicate) {
            const auto& table_offsets_map =
                    kb.act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(x.first.first).at(x.first.second+(polyadic?x.second.first:1));
            for(const auto& pred_withConj : rhs_atoms){
                bool result = true;
                for (const auto& predDummy : ap->atom_to_conjunctedPredicates.at(pred_withConj)) {
                    bool test = false;
                    auto temp2_a = kb.attribute_name_to_table.find(predDummy.var);
                    if (temp2_a != kb.attribute_name_to_table.end()) {
                        for (const auto& [act,table_offsets] : table_offsets_map) {
                            for (const auto table_offset : table_offsets) {
                                std::optional<union_minimal> data = temp2_a->second.resolve_record_if_exists2(table_offset);
                                if (data.has_value()) {
                                    if (predDummy.testOverSingleVariable(data.value())) {
                                        test = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if (!test) {
                        result = false;
                        break;
                    }
                }
                if (result) {
                    rightMatch= true;
                    break;
                }
            }
            rightMatch=  false;
        }
        if (rightMatch) {
            if (correlation) {
                for (const auto& e1 : correlation->GetPayloadDataFromEvent(x.first.first, x.first.second, true, cache)) {
                    if (correlation->checkValidity(e1, x.first.first, x.first.second+1)) {
                        result.emplace_back(x).second.second.emplace_back(marked_event::right(x.first.second+1));
                        break;
                    }
                }
//                env e1 = correlation->GetPayloadDataFromEvent(x.first.first, x.first.second, true, cache);
//                if (correlation->checkValidity(e1, x.first.first, x.first.second+1)) {
//                    result.emplace_back(x).second.second.emplace_back(marked_event::right(x.first.second+1));
//                }
            } else {
                result.emplace_back(x).second.second.emplace_back(marked_event::right(x.first.second+1));
            }
        }
    }
}

inline void next_and(const Result &lhsOperand,
                     Result& result,
                     const KnowledgeBase& kb,
                     const AtomizingPipeline* ap,
                     const std::set<std::string>& rhs_atoms,
                     const PredicateManager* correlation = nullptr) {
    std::unordered_set<std::string> cache;
    act_t right_activity;
    bool right_predicate;
    if ((!ap) ||((rhs_atoms.size() == 1) && ap->act_atoms.contains(*rhs_atoms.begin()))) {
        right_predicate = false;
        ssize_t tmp = kb.event_label_mapper.signed_get(*rhs_atoms.begin());
        if (tmp == -1) return;
        right_activity = tmp;
    } else {
        right_predicate = true;
        ssize_t tmp = kb.event_label_mapper.signed_get(ap->atom_to_conjunctedPredicates.at(*rhs_atoms.begin()).at(0).label);
        if (tmp == -1) return;
        right_activity = tmp;
    }
//    if ((!ap) ||((rhs_atoms.size() == 1) && ap->act_atoms.contains(*rhs_atoms.begin()))) {
//        right_predicate = false;
//        right_activity = kb.event_label_mapper.signed_get(*rhs_atoms.begin());
//    } else {
//        right_predicate = true;
//        right_activity = kb.event_label_mapper.signed_get(ap->atom_to_conjunctedPredicates.at(*rhs_atoms.begin()).at(0).label);
//    }
    auto cp = kb.getCountTable().resolve_primary_index2(right_activity);
    auto iter = cp.first;
    size_t count = 0;
    while (iter != cp.second) {
        count += iter->id.parts.event_id;
        if (count > 0) break;
        iter++;
    }
    if (count == 0) return;
    for (const auto& x : lhsOperand) {
        if (x.first.second == 0) continue;
        if (cp.first[x.first.first].id.parts.event_id == 0) continue;
        const auto& right_offsets = kb.act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(x.first.first).at(x.first.second-1);
        bool found = right_offsets.find(right_activity) != right_offsets.end();
//        for (const auto right_offset : right_offsets ){
//            if ((kb.act_table_by_act_id.table.at(right_offset).entry.id.parts.act == right_activity)) {
//                found = true;
//                break;
//            }
//        }
        if (!found) continue;
        bool rightMatch = true;
        if (right_predicate) {
            const auto& table_offsets_map =
                    kb.act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(x.first.first).at(x.first.second-1);
            for(const auto& pred_withConj : rhs_atoms){
                bool result = true;
                for (const auto& predDummy : ap->atom_to_conjunctedPredicates.at(pred_withConj)) {
                    bool test = false;
                    auto temp2_a = kb.attribute_name_to_table.find(predDummy.var);
                    if (temp2_a != kb.attribute_name_to_table.end()) {
                        for (const auto& [act, table_offsets] : table_offsets_map) {
                            for (size_t table_offset : table_offsets ) {
                                std::optional<union_minimal> data = temp2_a->second.resolve_record_if_exists2(table_offset);
                                if (data.has_value()) {
                                    if (predDummy.testOverSingleVariable(data.value())) {
                                        test = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if (!test) {
                        result = false;
                        break;
                    }
                }
                if (result) {
                    rightMatch= true;
                    break;
                }
            }
            rightMatch=  false;
        }
        if (rightMatch) {
            if (correlation) {
                for (const auto& e1 : correlation->GetPayloadDataFromEvent(x.first.first, x.first.second, true, cache)) {
                    if (correlation->checkValidity(e1, x.first.first, x.first.second-1)) {
                        auto& ref = result.emplace_back(x);
                        ref.first.second--;
                        ref.second.second.emplace_back(marked_event::right(x.first.second-1));
                        break;
                    }
                }
//                env e1 = correlation->GetPayloadDataFromEvent(x.first.first, x.first.second, true, cache);
            } else {
                auto& ref = result.emplace_back(x);
                ref.first.second--;
                ref.second.second.emplace_back(marked_event::right(x.first.second-1));
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
            bool first= true;
            hasMatch = false;
            auto aNext = aCurrent+1;
            if ((aNext != aEnd) && (aNext->first.first == rcx.first.first) && (aNext->first < bCurrent->first)) {
                aCurrent++;
                continue;
            }

            while (((first) || hasMatch) && (newItr != bEnd)) {
                if(newItr->first.first != aCurrent->first.first){
                    break;
                }
                if (manager && (!aCurrent->second.second.empty()) && (!newItr->second.second.empty())) {
                    for (const auto &elem: aCurrent->second.second) {
                        if (!IS_MARKED_EVENT_ACTIVATION(elem)) continue;
                        join.id.parts.left = GET_ACTIVATION_EVENT(elem);
                        const auto e1V = manager->GetPayloadDataFromEvent(aCurrent->first.first, join.id.parts.left, true, cache);
                        for (const auto &elem1: newItr->second.second) {
                            if (!IS_MARKED_EVENT_TARGET(elem1)) continue;
                            join.id.parts.right = GET_TARGET_EVENT(elem1);
                            for (const auto& e1 : e1V) {
                                if (manager->checkValidity(e1, newItr->first.first, join.id.parts.right)) {
                                    hasMatch = true;
                                    rcx.second.second.push_back(join);
//                                    rcx.second.first *= (1.0 - std::min(aCurrent->second.first, newItr->second.first));
                                    break;
                                }
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

            first = false;
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
            if (bCurrent == bEnd) {
                rcx.first = aCurrent->first;
                rcx.second.second.clear();
                rcx.second.first = 1.0;
                hasMatch = false;
                if (lengths.at(aCurrent->first.first)-1==aCurrent->first.second)
                    hasMatch = false;
                else  {
                    auto aNext = aCurrent + 1;
                    if ((aNext != aEnd) && (aNext->first.first == rcx.first.first))
                        hasMatch = false;
                    else
                        hasMatch = true;
                }
                if (hasMatch) {
                    rcx.second.second.insert(rcx.second.second.end(), aCurrent->second.second.begin(), aCurrent->second.second.end());
                    remove_duplicates(rcx.second.second);
                    result.emplace_back(rcx);
                }
                break;
            }
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
                bool first = true;

                while ((first || hasMatch) && (newItr != bEnd)) {
                    if(newItr->first.first != aCurrent->first.first){
                        break;
                    }
                    if (manager && (!aCurrent->second.second.empty()) && (!newItr->second.second.empty())) {
                        for (const auto &elem: aCurrent->second.second) {
                            if (!IS_MARKED_EVENT_ACTIVATION(elem)) continue;
                            join.id.parts.left = GET_ACTIVATION_EVENT(elem);
                            const auto e1V = manager->GetPayloadDataFromEvent(aCurrent->first.first, join.id.parts.left, true, cache);
                            for (const auto &elem1: newItr->second.second) {
                                if (!IS_MARKED_EVENT_TARGET(elem1)) continue;
                                join.id.parts.right = GET_TARGET_EVENT(elem1);
                                for (const auto& e1 : e1V) {
                                    if (manager->checkValidity(e1, newItr->first.first, join.id.parts.right)) {
                                        hasMatch = true;
                                        rcx.second.second.push_back(join);
//                                        rcx.second.first *= (1.0 - std::min(aCurrent->second.first, newItr->second.first));
                                        break;
                                    }
                                }
                            }
                        }
                    } else {
                        hasMatch = true;
                        rcx.second.second.insert(rcx.second.second.end(), newItr->second.second.begin(), newItr->second.second.end());
                        if (manager)
                            rcx.second.second.insert(rcx.second.second.end(), aCurrent->second.second.begin(), aCurrent->second.second.end());
                    }
                    first = false;
                    newItr++;
                }
            }

            if (hasMatch) {
                if ((!manager))
                    rcx.second.second.insert(rcx.second.second.end(), aCurrent->second.second.begin(), aCurrent->second.second.end());
                remove_duplicates(rcx.second.second);
                if (manager) rcx.second.first = 1.0 - rcx.second.first;
                result.emplace_back(rcx);
            }

            if (aCurrent->first == bCurrent->first) {
                bCurrent++;
                if (bCurrent == bEnd) {
                    break;
                }
            }

            aCurrent++;
        }
    }
}

// ImplyAndAltUntilOrAbsence

#endif //KNOBAB_FAST_LTLF_OPERATORS_H
