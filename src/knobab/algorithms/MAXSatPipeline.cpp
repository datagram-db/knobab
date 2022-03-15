//
// Created by giacomo on 20/01/2022.
//

#include <magic_enum.hpp>
#include <knobab/predicates/SimpleDataPredicate.h>
#include <knobab/predicates/PredicateManager.h>
#include <knobab/utilities/SetOperators.h>
#include <knobab/operators/simple_ltlf_operators.h>
#include <yaucl/functional/assert.h>
#include "knobab/algorithms/MAXSatPipeline.h"



std::string MAXSatPipeline::LEFT_ATOM{"a"};
std::string MAXSatPipeline::RIGHT_ATOM{"b"};



MAXSatPipeline::MAXSatPipeline(size_t nThreads)
#ifdef MAXSatPipeline_PARALLEL
    : pool(nThreads)
#endif
{
    // TODO: with different specifications
    DEBUG_ASSERT(false);
#if 0
    for (declare_templates t : magic_enum::enum_values<declare_templates>()) {
        if (isUnaryPredicate(t)) continue;/*
        ltlf_semantics.emplace(t, DeclareDataAware::unary(t, LEFT_ATOM, 1)
                .toFiniteSemantics(false)
                .nnf(false)).first->second.mark_join_condition(LEFT_ATOM, RIGHT_ATOM);
        else */
        ltlf_semantics.emplace(t, DeclareDataAware::binary(t, LEFT_ATOM, RIGHT_ATOM)
                .toFiniteSemantics(false)
                .nnf(false)).first->second.mark_join_condition(LEFT_ATOM, RIGHT_ATOM);
    }
#endif
}

void MAXSatPipeline::clear() {
    data_accessing.clear();
    declare_atomization.clear();
    atomToResultOffset.clear();
    toUseAtoms.clear();
    atomicPartIntersectionResult.clear();
    result.clear();
}

void MAXSatPipeline::pipeline(CNFDeclareDataAware* model,
                       const AtomizingPipeline& atomization,
                       const KnowledgeBase& kb) {
    /// Clearing the previous spurious computation values
    clear();

    /// Extracting the predicates from both the LTLf semantics and the data extracted from it
    {
        auto start = std::chrono::system_clock::now();
        data_chunk(model, atomization, kb);
        auto end = std::chrono::system_clock::now();
        auto elapsed =
                std::chrono::duration<double, std::milli>(end - start);
        declare_to_ltlf_time = elapsed.count();
    }

    {
        auto start = std::chrono::system_clock::now();
        actual_query_running(kb);
        auto end = std::chrono::system_clock::now();
        auto elapsed =
                std::chrono::duration<double, std::milli>(end - start);
        ltlf_query_time = elapsed.count();
    }

}

void MAXSatPipeline::data_chunk(CNFDeclareDataAware *model,
                                const AtomizingPipeline& atomization,
                                const KnowledgeBase& kb) {

    if (!model) return;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> empty_result{};
    declare_model = model;
    std::vector<ltlf_query *> W;
    //label_set_t visitedAtoms;
    for (auto& it : declare_model->singleElementOfConjunction) {
        for (auto& item : it.elementsInDisjunction) {
            item.kb = &kb; // Setting the knowledge base, so to exploit it for join queries



            ///// OLD PART OF THE CODE!!!!!!!!!!!!!!
            auto& ref = declare_atomization[item];
            if (!ref.empty()) continue; // Ignoring already visited patterns

            ltlf_query *formula;
            if (item.casusu == "Init") {
                formula = qm.init1(item.left_act, item.left_decomposed_atoms);
                if ((item.left_decomposed_atoms.size() == 1) && (*item.left_decomposed_atoms.begin() == item.left_act)) {
                    generateAtomQuery(toUseAtoms, empty_result, item, formula, InitQuery, 0);
                } else {
                    std::unordered_map<std::string, std::unordered_set<bool>> collection;
                    std::vector<std::string> LDA{item.left_decomposed_atoms.begin(), item.left_decomposed_atoms.end()};
                    collection[LEFT_ATOM] = {false};
                    localExtract(atomization, toUseAtoms, ref, item.left_decomposed_atoms, LEFT_ATOM, false);
                    qm.getQuerySemiInstantiated(LDA, false, formula, true, nullptr, formula->casusu, nullptr,
                                             false, (KnowledgeBase*)&kb);
                }
            } else if (item.casusu == "End") {
                formula = qm.end1(item.left_act, item.left_decomposed_atoms);
                if ((item.left_decomposed_atoms.size() == 1) && (*item.left_decomposed_atoms.begin() == item.left_act)) {
                    generateAtomQuery(toUseAtoms, empty_result, item, formula, EndsQuery, 0);
                } else {
                    std::unordered_map<std::string, std::unordered_set<bool>> collection;
                    std::vector<std::string> LDA{item.left_decomposed_atoms.begin(), item.left_decomposed_atoms.end()};
                    collection[LEFT_ATOM] = {false};
                    localExtract(atomization, toUseAtoms, ref, collection, item.left_decomposed_atoms, LEFT_ATOM);
                    qm.getQuerySemiInstantiated(LDA, false, formula, true, nullptr, formula->casusu, nullptr,
                                                false, (KnowledgeBase*)&kb);
                }
            } else if (item.casusu == "Existence") {
                formula = qm.exists(item.left_act, item.left_decomposed_atoms, item.n);
                if ((item.left_decomposed_atoms.size() == 1) && (*item.left_decomposed_atoms.begin() == item.left_act)) {
                    generateAtomQuery(toUseAtoms, empty_result, item, formula, ExistsQuery, item.n);
                } else {
                    std::unordered_map<std::string, std::unordered_set<bool>> collection;
                    std::vector<std::string> LDA{item.left_decomposed_atoms.begin(), item.left_decomposed_atoms.end()};
                    collection[LEFT_ATOM] = {false};
                    localExtract(atomization, toUseAtoms, ref, collection, item.left_decomposed_atoms, LEFT_ATOM);
                    qm.getQuerySemiInstantiated(LDA, false, formula, true, nullptr, formula->casusu, nullptr,
                                                false, (KnowledgeBase*)&kb);
                }
            } else if (item.casusu == "Absence") {
                formula = qm.absence(item.left_act, item.left_decomposed_atoms, item.n);
                if ((item.left_decomposed_atoms.size() == 1) && (*item.left_decomposed_atoms.begin() == item.left_act)) {
                    generateAtomQuery(toUseAtoms, empty_result, item, formula, AbsenceQuery, item.n);
                } else {
                    std::unordered_map<std::string, std::unordered_set<bool>> collection;
                    std::vector<std::string> LDA{item.left_decomposed_atoms.begin(), item.left_decomposed_atoms.end()};
                    collection[LEFT_ATOM] = {false};
                    localExtract(atomization, toUseAtoms, ref, collection, item.left_decomposed_atoms, LEFT_ATOM);
                    qm.getQuerySemiInstantiated(LDA, false, formula, true, nullptr, formula->casusu, nullptr,
                                                false, (KnowledgeBase*)&kb);
                }
            } else {
                // Parameters
                bool hasRight = true;


                ltlf_query* local_formula;
                auto& ltlf_sem = ltlf_semantics[item.casusu];
                //std::vector<size_t> requirements;
                std::unordered_map<std::string, std::unordered_set<bool>> collection;
                ltlf_sem.collectElements(collection);

                // Collecting all of the atoms, for then transferring those to the analysis per atom
                localExtract(atomization, toUseAtoms, ref, collection, item.left_decomposed_atoms, LEFT_ATOM);
                if (hasRight)
                    localExtract(atomization, toUseAtoms, ref, collection, item.right_decomposed_atoms, RIGHT_ATOM);

                // If it is a binary Declare operand, then I need to exploit the LTLf rewriting.
                if (hasRight) {
                    auto tmp = ltlf_sem.replace_with(ref);
                    // Reconstructing the join
                    tmp.instantiateJoinCondition(item.conjunctive_map);
                    // Creating the formula
                    local_formula = qm.simplify(tmp, false, (KnowledgeBase*)&kb);
                } else {
                    // Otherwise,
                }

                // Returned result
                formula = local_formula;
            }

            fomulaidToFormula.emplace_back(formula);
            maxFormulaId++;
            W.emplace_back(formula);
            human_readable_ltlf_printing(std::cout, formula) << std::endl; //todo: debugging
        }
    }

    for (auto& ref : atomToFormulaId)
        remove_duplicates(ref.second);
    qm.finalize_unions(W, (KnowledgeBase*)&kb); // Time Computational Complexity: Squared on the size of the atoms

    /////////////////////////////// DEBUGGING
    for (const auto& ref : W) {
        human_readable_ltlf_printing(std::cout, ref) << std::endl;
    }
    /////////////////////////////////////////

    remove_duplicates(toUseAtoms);

    // TODO: AtomQuery is not required, as range queries already have the atom information!
    // Just the Act iteration
    auto toUseAtomsEnd = std::stable_partition(toUseAtoms.begin(), toUseAtoms.end(), [&](const auto& x) { return atomization.act_atoms.contains(x); });
    for (auto it = toUseAtoms.begin(); it != toUseAtomsEnd; it++) {
        auto q = DataQuery::AtomQuery(*it);
        size_t offset = data_accessing.size();
        auto find = data_offset.emplace(q, offset);
        if (find.second){
            offset = data_accessing.size();
            data_accessing.emplace_back(q, empty_result);
        } else
            offset = (find.first->second);
        maxPartialResultId = std::max(maxPartialResultId, (ssize_t)offset);
        atomToResultOffset.emplace_back(std::set<size_t>{offset});
    }
    barrier_to_range_queries = data_accessing.size();
    barriers_to_atfo = atomToResultOffset.size();

    // Iterating other the remaining non-act atoms, that is, the data predicate+label ones
    for (auto it = toUseAtomsEnd, en = toUseAtoms.end(); it != en; it++) {
        auto interval_to_interval_queries_to_intersect = atomization.atom_to_conjunctedPredicates.find(*it);
        DEBUG_ASSERT(interval_to_interval_queries_to_intersect != atomization.atom_to_conjunctedPredicates.end());

        std::vector<size_t> tmpRanges;
        for (const auto& interval_data_query : interval_to_interval_queries_to_intersect->second) {
            // Sanity Checks
            DEBUG_ASSERT(interval_data_query.casusu == INTERVAL);
            DEBUG_ASSERT(interval_data_query.BiVariableConditions.empty());

            //       splits the RangeQuery by clause.var (as they are going to be on different tables),
            //       clause.label (as the ranges are ordered by act and then by value) and then sort
            //       the remainder by interval. By doing so, we are going to pay the access to clause.label
            //       only once, and we are going to scan the whole range at most linearly instead of performing
            //       at most linear scans for each predicate.
            //       Then, the access can be parallelized by variable name, as they are going to query separated tables
            auto q = DataQuery::RangeQuery(interval_data_query.label, interval_data_query.var, interval_data_query.value, interval_data_query.value_upper_bound);
            size_t tmpDataAccessingSize =  data_accessing.size();
            auto find = data_offset.emplace(q, tmpDataAccessingSize);
            if (find.second){
                maxPartialResultId = std::max(maxPartialResultId, (ssize_t)tmpDataAccessingSize);
                tmpRanges.emplace_back(tmpDataAccessingSize);
                data_accessing_range_query_to_offsets[interval_data_query.var][interval_data_query.label].emplace_back(tmpDataAccessingSize);
                data_accessing.emplace_back(q, empty_result);
            } else {
                maxPartialResultId = std::max(maxPartialResultId, (ssize_t)find.first->second);
                tmpRanges.emplace_back(find.first->second);
            }
        }
        std::cout << atomToResultOffset.size() << "<=>" << interval_to_interval_queries_to_intersect->first << "-->" << tmpRanges << std::endl;
        atomToResultOffset.emplace_back(tmpRanges.begin(), tmpRanges.end());
    }
}

void
MAXSatPipeline::generateAtomQuery(std::vector<std::string> &toUseAtoms,
                                  std::vector<std::pair<std::pair<trace_t, event_t>, double>> &empty_result,
                                  DeclareDataAware &item, ltlf_query *formula, DataQueryType r,
                                  size_t numeric_argument) {
    size_t offset;
    auto q = DataQuery::AtomQueries(r, item.left_act);
    q.numeric_argument = numeric_argument;
    size_t tmpDataAccessingSize = data_accessing.size();
    auto find = data_offset.emplace(q, tmpDataAccessingSize);
    if (find.second){
        formula->partial_results.emplace(tmpDataAccessingSize); // TODO: removable
        offset = tmpDataAccessingSize;
        data_accessing.emplace_back(q, empty_result);
    } else {
        offset = (find.first->second);
        formula->partial_results.emplace(find.first->second); // TODO: removable
    }

    if (/*(!item.left_decomposed_atoms.empty()) && */((item.left_decomposed_atoms.size() > 1) || (!item.left_decomposed_atoms.contains(item.left_act))))
        /*toUseAtoms.emplace_back(item.left_act);
    else*/
        toUseAtoms.insert(toUseAtoms.end(), item.left_decomposed_atoms.begin(), item.left_decomposed_atoms.end());
    atomToFormulaId[item.left_act].emplace_back(maxFormulaId);
    atomToResultOffset.emplace_back(std::set<size_t>{offset});
    maxPartialResultId = std::max(maxPartialResultId, (ssize_t)offset);
}

void
MAXSatPipeline::localExtract(const AtomizingPipeline &atomization,
                             std::vector<std::string> &toUseAtoms,
                             std::unordered_map<std::pair<bool, std::string>, label_set_t> &ref,
                             std::unordered_map<std::string, std::unordered_set<bool>> &collection,
                             const std::unordered_set<std::string> &decomposition,
                             const std::string &collectionMapKey) {
    DEBUG_ASSERT(!decomposition.empty());
    auto it = collection.find(collectionMapKey);
    if (it != collection.end()) {
        for (bool isNegated : it->second) {
            localExtract(atomization, toUseAtoms, ref, decomposition, collectionMapKey, isNegated);
        }
    }
}

void MAXSatPipeline::localExtract(const AtomizingPipeline &atomization, std::vector<std::string> &toUseAtoms,
                                  std::unordered_map<std::pair<bool, std::string>, label_set_t> &ref,
                                  const std::unordered_set<std::string> &decomposition,
                                  const std::string &collectionMapKey, bool isNegated) {
    const auto& local_decomposition = ref.emplace(std::pair<bool, std::string>{isNegated, collectionMapKey},
                                                  isNegated ?
                                                  unordered_difference(atomization.atom_universe, decomposition)
                                                            : decomposition).first->second;
    for (const auto& x : local_decomposition) {
        //toUseAtoms.insert(x);
        atomToFormulaId[x].emplace_back(maxFormulaId);
    }
    toUseAtoms.insert(toUseAtoms.end(), local_decomposition.begin(), local_decomposition.end());
}


static inline void setUnion(const PartialResult& lhs,
                            const PartialResult& rhs,
                            std::back_insert_iterator<PartialResult> d_first) {
    env e1, e2;
    auto first1 = lhs.begin(), last1 = lhs.end(), first2 = rhs.begin(), last2 = rhs.end();
    std::pair<uint32_t, uint16_t> pair, pair1;

    for (; first1 != last1; ++d_first) {
        if (first2 == last2) {
            std::copy(first1, last1, d_first);
            return;
        }
        if (first1->first > first2->first) {
            *d_first = *first2++;
        } else if (first1->first < first2->first) {
            *d_first = *first1++;
        } else {
            pair.first = first1->first.first;
            pair1.first = first2->first.first;
            *d_first = std::make_pair(first1->first, std::max(first1->second, first2->second));
            first1++;
            first2++;
        }
    }
    std::copy(first2, last2, d_first);
}

static inline void setIntersection(const PartialResult& lhs,
                                   const PartialResult& rhs,
                                    std::back_insert_iterator<PartialResult> d_first) {
    env e1, e2;
    auto first1 = lhs.begin(), last1 = lhs.end(), first2 = rhs.begin(), last2 = rhs.end();
    std::pair<uint32_t, uint16_t> pair, pair1;
    for (; first1 != last1; ++d_first) {
        if (first2 == last2) {
            return;
        }
        if (first1->first > first2->first) {
            *first2++;
        } else if (first1->first < first2->first) {
            *first1++;
        } else {
            pair.first = first1->first.first;
            pair1.first = first2->first.first;
            *d_first = std::make_pair(first1->first, std::max(first1->second, first2->second));
            first1++;
            first2++;
        }
    }
}

static inline
std::vector<std::pair<std::pair<trace_t, event_t>, double>> local_intersection(const std::set<size_t> &vecs,
                                                                               const std::vector<std::pair<DataQuery, std::vector<std::pair<std::pair<trace_t, event_t>, double>>>>& results) {
    if (vecs.empty()) return {};
    auto it = vecs.begin();
    auto last_intersection = results.at(*it).second;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> curr_intersection;
    for (std::size_t i = 1; i < vecs.size(); ++i) {
        it++;
        auto ref = results.at(*it).second;
        setIntersection(last_intersection,
                              ref,
                              std::back_inserter(curr_intersection));
        std::swap(last_intersection, curr_intersection);
        curr_intersection.clear();
    }
    return last_intersection;
}

static inline PartialResult local_intersection(const std::set<size_t> &vecs,
                                               const std::vector<PartialResult>& results) {
    if (vecs.empty()) return {};
    auto it = vecs.begin();
    auto last_intersection = results.at(*it);
    PartialResult curr_intersection;
    for (std::size_t i = 1; i < vecs.size(); ++i) {
        it++;
        auto& ref = results.at(*it);
        setIntersection(last_intersection,
                        ref,
                        std::back_inserter(curr_intersection));
        std::swap(last_intersection, curr_intersection);
        curr_intersection.clear();
    }

    return last_intersection;
}

static inline Result local_intersection(const std::set<size_t> &vecs,
                                        const std::vector<PartialResult>& results,
                                        LeafType isLeaf) {
    if (vecs.empty()) return {};
    PartialResult last_intersection = local_intersection(vecs, results);

    // TODO: better done through views!
    Result  finalResult;
    for (auto it = last_intersection.begin(); it != last_intersection.end(); it = last_intersection.erase(it)) {
        switch (isLeaf) {
            case ActivationLeaf:
                finalResult.emplace_back(it->first, std::make_pair(it->second, MarkedEventsVector{marked_event::activation(it->first.second)}));
            case TargetLeaf:
                finalResult.emplace_back(it->first, std::make_pair(it->second, MarkedEventsVector{marked_event::target(it->first.second)}));
                break;
            default:
                finalResult.emplace_back(it->first, std::make_pair(it->second, MarkedEventsVector{}));
                break;
        }
    }

    return finalResult;
}

static inline void data_merge(const std::set<size_t> &vecs,
                              const std::vector<PartialResult>& results,
                              Result& result,
                              LeafType isLeaf) {
    if (vecs.empty()) {
        result.clear();
        return;
    }
    auto it = vecs.begin();
    auto last_intersection = results.at(*it);
    PartialResult curr_intersection;
    for (std::size_t i = 1; i < vecs.size(); ++i) {
        it++;
        auto ref = results.at(*it);
        setUnion(last_intersection,
                        ref,
                        std::back_inserter(curr_intersection));
        std::swap(last_intersection, curr_intersection);
        curr_intersection.clear();
    }

    // TODO: better done through views!
    for (auto it = last_intersection.begin(); it != last_intersection.end(); it = last_intersection.erase(it)) {
        switch (isLeaf) {
            case ActivationLeaf:
                result.emplace_back(it->first, std::make_pair(it->second, MarkedEventsVector{marked_event::activation(it->first.second)}));
            case TargetLeaf:
                result.emplace_back(it->first, std::make_pair(it->second, MarkedEventsVector{marked_event::target(it->first.second)}));
                break;
            default:
                result.emplace_back(it->first, std::make_pair(it->second, MarkedEventsVector{}));
                break;
        }
    }

}

static inline void local_union(const ltlf_query* q, Result& last_union, bool isTimed = true) {
    size_t N = q->args.size();
    if ((!q) || (N == 0)) {
        last_union.clear();
        return;
    } else if (N == 1) {
        last_union = q->args.at(1)->result;
    } else if (N == 2) {
        last_union.clear();
        auto& arg1 = q->args.at(0);
        auto& arg2 = q->args.at(1);
        if (isTimed)
            setUnion(arg1->result.begin(), arg1->result.end(),
                            arg2->result.begin(), arg2->result.end(),
                            std::back_inserter(last_union),
                            Aggregators::maxSimilarity<double, double, double>,
                     q->joinCondition.isTruth() ? nullptr : &q->joinCondition);
        else
            setUnionUntimed(arg1->result.begin(), arg1->result.end(),
                                   arg2->result.begin(), arg2->result.end(),
                                   std::back_inserter(last_union),
                                   Aggregators::maxSimilarity<double, double, double>,
                            q->joinCondition.isTruth() ? nullptr : &q->joinCondition);
    } else {
        auto it = q->args.begin();
        last_union = (*it)->result;
        Result curr_union;
        for (std::size_t i = 1; i < q->args.size(); ++i) {
            it++;
            auto ref = (*it)->result;
            if (isTimed)
                setUnion(last_union.begin(), last_union.end(),
                         ref.begin(), ref.end(),
                         std::back_inserter(curr_union),
                         Aggregators::maxSimilarity<double, double, double>);
            else
                setUnionUntimed(last_union.begin(), last_union.end(),
                                ref.begin(), ref.end(),
                                std::back_inserter(curr_union),
                                Aggregators::maxSimilarity<double, double, double>);
            std::swap(last_union, curr_union);
            curr_union.clear();
        }
    }
}

static inline void local_intersection(const ltlf_query* q, Result& last_union, bool isTimed = true) {
    size_t N = q->args.size();
    if ((!q) || (N == 0)) {
        last_union.clear();
        return;
    } else if (N == 1) {
        last_union = q->args.at(0)->result;
    } else if (N == 2) {
        auto& arg1 = q->args.at(0);
        auto& arg2 = q->args.at(1);
        last_union.clear();
        if (isTimed)
            setIntersection(arg1->result.begin(), arg1->result.end(),
                            arg2->result.begin(), arg2->result.end(),
                            std::back_inserter(last_union),
                            Aggregators::maxSimilarity<double, double, double>,
                                    q->joinCondition.isTruth() ? nullptr : &q->joinCondition);
        else
            setIntersectionUntimed(arg1->result.begin(), arg1->result.end(),
                                   arg2->result.begin(), arg2->result.end(),
                                   std::back_inserter(last_union),
                                   Aggregators::maxSimilarity<double, double, double>,
                                   q->joinCondition.isTruth() ? nullptr : &q->joinCondition);
    } else {
        DEBUG_ASSERT(q->joinCondition.isTruth());
        auto it = q->args.begin();
        last_union = (*it)->result;
        Result curr_union;
        for (std::size_t i = 1; i < N; ++i) {
            it++;
            auto ref = (*it)->result;
            if (isTimed)
                setIntersection(last_union.begin(), last_union.end(),
                                ref.begin(), ref.end(),
                                std::back_inserter(curr_union),
                                Aggregators::maxSimilarity<double, double, double>);
            else
                setIntersectionUntimed(last_union.begin(), last_union.end(),
                                       ref.begin(), ref.end(),
                                       std::back_inserter(curr_union),
                                       Aggregators::maxSimilarity<double, double, double>);
            std::swap(last_union, curr_union);
            curr_union.clear();
        }
    }

}

static inline void absence_or_exists(ltlf_query* formula, const std::vector<PartialResult>& results_cache) {
    bool isFirstIteration = true;
    uint32_t traceId = 0;
    uint16_t eventCount = 0;
    Result tmp_result;
    data_merge(formula->partial_results, results_cache, tmp_result, formula->isLeaf);
    ResultRecord cp{{0,0}, {1.0, {}}};
    for (auto ref = tmp_result.begin(); ref != tmp_result.end(); ) {
        if (isFirstIteration) {
            traceId = ref->first.first;
            eventCount = 1;
            isFirstIteration = false;
        } else {
            if ((traceId != ref->first.first)) {
                if ((eventCount >= formula->numeric_arg)) {
                    cp.first.first = traceId;
                    formula->result.emplace_back(cp);
                }
                traceId = ref->first.first;
                eventCount = 1;
            } else eventCount++;
        }
        ref = tmp_result.erase(ref);
    }
    if ((eventCount >= formula->numeric_arg)) {
        cp.first.first = traceId;
        formula->result.emplace_back(cp);
    }
}


void MAXSatPipeline::actual_query_running(const KnowledgeBase& kb) {

    // 1. Performing the query over each single predicate that we have extracted, so not to duplicate the data access
    if (barrier_to_range_queries > 0) {
        PARALLELIZE_LOOP_BEGIN(pool, 0, barrier_to_range_queries, a, b)
            for (size_t i = a; i < b; i++) {
                auto& ref = data_accessing.at(i);
                // TODO: Given the query in ref.first, put the Result in ref.second
                switch (ref.first.type) {
                    case ExistsQuery:
                        ref.second = kb.exists(kb.resolveCountingData(ref.first.label), ref.first.numeric_argument);
                        break;
                    case AtomQuery:
                        ref.second = kb.exists(ref.first.label);
                        break;
                    case AbsenceQuery:
                        ref.second = kb.absence(kb.resolveCountingData(ref.first.label), ref.first.numeric_argument);
                        break;
                    case InitQuery:
                        ref.second = kb.initOrig(ref.first.label);
                        break;
                    case EndsQuery:
                        ref.second = kb.endsOrig(ref.first.label);
                        break;
                    //
                    //    // TODO: ref.second = kb.exists<std::pair<uint32_t, uint16_t>, double>(ref.first.label).traceApproximations;
                    //    break;
                    default:
                        DEBUG_ASSERT(false); // This should be dealt in (B)
                }
            }
        PARALLELIZE_LOOP_END
    }


    // 2. Performing the queries over the range queries
    if (!data_accessing_range_query_to_offsets.empty()) {
#ifdef MAXSatPipeline_PARALLEL
        // If this is a parallel execution, I can exploit the current library only if I have a vector. Otherwise,
        // the map data structures are not safe for iteration
        std::vector<std::pair<std::string, std::unordered_map<std::string,std::vector<size_t>>>> someVector;
        std::transform(std::move_iterator(data_accessing_range_query_to_offsets.begin()),
                       std::move_iterator(data_accessing_range_query_to_offsets.end()),
                       std::back_inserter(someVector),
                       [](auto&& entry){ return std::forward<decltype(entry)>(entry); });

        PARALLELIZE_LOOP_BEGIN(pool, 0,someVector.size(), lb, ub)
            for (size_t i = lb; i < ub; i++) {
                auto& rangeQueryRefs = someVector.at(i);
                kb.exact_range_query(rangeQueryRefs.first, rangeQueryRefs.second, data_accessing);
            }
        PARALLELIZE_LOOP_END

        std::transform(std::move_iterator(someVector.begin()),
                       std::move_iterator(someVector.end()),
                       std::inserter(data_accessing_range_query_to_offsets, data_accessing_range_query_to_offsets.begin()),
                       [](auto&& entry){ return std::forward<decltype(entry)>(entry); });
#else
        // If this is not in parallel mode, then there is no purpose to create a temporary vector for the parallelization:
        // therefore, I can directly iterate over the map.
        for (auto& rangeQueryRefs : data_accessing_range_query_to_offsets) {
            kb.exact_range_query(rangeQueryRefs.first, rangeQueryRefs.second, data_accessing);
        }
#endif
    }
//
//    for (const auto& da : data_accessing) {
//        std::cout << da.first.label << '.' << da.first.var << " \\in [" << std::get<1>(da.first.lower_bound) << ',' << get<1>(da.first.upper_bound) << "] --> " << da.second << std::endl;
//    }

    // After accessing the data, we perform the interval intersection between these
    // The computational complexity should be squared on the size of the atoms
    auto set_decomposition_result = partition_sets(atomToResultOffset);
    std::cout << set_decomposition_result << std::endl;
    size_t isFromFurtherDecomposition = set_decomposition_result.minimal_common_subsets.size();

    std::vector<PartialResult> resultOfS(set_decomposition_result.minimal_common_subsets.size() + set_decomposition_result.minimal_common_subsets_composition.size());
    PARALLELIZE_LOOP_BEGIN(pool, 0,set_decomposition_result.minimal_common_subsets.size(), lb, ub)
        for (size_t i = lb; i < ub; i++) {
            auto& S = set_decomposition_result.minimal_common_subsets.at(i);
//            std::cout << i << " is " << S << std::endl;
            // resultOfS for collecting the intermediate computations
            resultOfS[i] = local_intersection(S, data_accessing);
        }
    PARALLELIZE_LOOP_END

    PARALLELIZE_LOOP_BEGIN(pool, 0,set_decomposition_result.minimal_common_subsets_composition.size(), lb, ub)
        for (size_t i = lb; i < ub; i++) {
            auto& S = set_decomposition_result.minimal_common_subsets_composition.at(i);
            // Perform the intersection among all of the elements in S,
            // using the intermediate results from resultOfSSecond
            resultOfS[isFromFurtherDecomposition + i] = local_intersection(S, resultOfS);
        }
    PARALLELIZE_LOOP_END

//    for (size_t i = 0; i<resultOfS.size(); i++) {
//        std::cout << i << "->>->" << resultOfS.at(i) << std::endl;
//    }

    ///results_cache.resize(toUseAtoms.size());
    std::vector<PartialResult> results_cache(std::max(toUseAtoms.size(), resultOfS.size()), PartialResult{});
    PARALLELIZE_LOOP_BEGIN(pool, 0,set_decomposition_result.decomposedIndexedSubsets.size(), lb, ub)
        for (size_t j = lb; j < ub; j++) {
            auto& ref = set_decomposition_result.decomposedIndexedSubsets.at(j);
//            std::cout << ref.first << "-@->" << *ref.second << std::endl;
            // put the global intersection into a map representation.
            // ref->first will correspond to the atom in that same position in toUseAtoms.
            results_cache[ref.first] = local_intersection(*ref.second, resultOfS);
        }
    PARALLELIZE_LOOP_END

    resultOfS.clear();
//    for (size_t i = 0; i<results_cache.size(); i++) {
//        std::cout << i << "-->" << results_cache.at(i) << std::endl;
//    }

    // Preparing the second phase of the pipeline, where the extracted data is going to be combined.
    for (size_t i = 0, N = toUseAtoms.size(); i<N; i++) {
        auto& atom = toUseAtoms.at(i);
        for (size_t formulaId : atomToFormulaId.at(atom)) {
            fomulaidToFormula.at(formulaId)->associateDataQueryIdsToFormulaByAtom(atom, i);
        }
    }

    auto it = qm.Q.rbegin(), en = qm.Q.rend();
    for (; it != en; it++) {
        Result tmp_result;

        PARALLELIZE_LOOP_BEGIN(pool, 0, it->second.size(), lb, ub)
            for (size_t j = lb; j < ub; j++) {
                auto formula = it->second.at(j); // TODO: run this query
                switch (formula->casusu) {
                    case Q_TRUE:
                        DEBUG_ASSERT(false);
                        break;

                    case Q_NEXT:
                        formula->result = next(formula->args.at(0)->result);
                        break;

                    case Q_FALSE:
                        formula->result.clear();
                        break;

                    case Q_ACT:
                        // This shall collect the temporary results from the previous data computation
                        // This never has a theta condition to consider
                        data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                        break;

                    case Q_INIT:
                        // This never has a theta condition to consider
                        // This will only work when data conditions are also considered
                        data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                        formula->result.erase(std::remove_if(formula->result.begin(),
                                                             formula->result.end(),
                                                  [](const auto&  x){return x.first.second > 0;}),
                                              formula->result.end());
                        break;

                    case Q_END:
                        // This never has a theta condition to consider
                        // This will only work when data conditions are also considered
                        data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                        formula->result.erase(std::remove_if(formula->result.begin(),
                                                             formula->result.end(),
                                                             [kb](const auto&  x){return x.first.second < kb.act_table_by_act_id.trace_length.at(x.first.first)-1;}),
                                              formula->result.end());
                        break;

                    case Q_AND:
                        DEBUG_ASSERT(formula->args.size() == 2);
                        local_intersection(formula, formula->result, formula->isTimed);
                        break;

                    case Q_OR:
                    case Q_XOR:
                        DEBUG_ASSERT(formula->args.size() == 2);
                        local_union(formula, formula->result, formula->isTimed);
                        break;


                    case Q_BOX:
                        DEBUG_ASSERT(formula->args.size() == 1);
                        if (formula->isTimed) {
                            global_logic_timed(formula->args.at(0)->result, formula->result, kb.act_table_by_act_id.trace_length);
                        } else {
                            global_logic_untimed(formula->args.at(0)->result, formula->result, kb.act_table_by_act_id.trace_length);
                        }
                        break;

                    case Q_DIAMOND:
                        DEBUG_ASSERT(formula->args.size() == 1);
                        if (formula->isTimed)
                             future_logic_timed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                        else {
                             future_logic_untimed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                        }
                        break;


                    case Q_UNTIL:
                        DEBUG_ASSERT(formula->args.size() == 2);
                        if (formula->isTimed) {
                            until_logic_timed(formula->args.at(0)->result,
                                                formula->args.at(1)->result,
                                                formula->result,
                                                formula->joinCondition.isTruth() ? nullptr : &formula->joinCondition,
                                              kb.act_table_by_act_id.trace_length);
                        } else {
                            until_logic_untimed(formula->args.at(0)->result,
                                                    formula->args.at(1)->result,
                                                    formula->result,
                                                    formula->joinCondition.isTruth() ? nullptr : &formula->joinCondition,
                                                kb.act_table_by_act_id.trace_length);
                        }
                        break;

                    case Q_RELEASE:
                        DEBUG_ASSERT(false);
                        break;

                    case Q_LAST:
                        // TODO
                        break;

                        break;
                    case Q_EXISTS: {
                        absence_or_exists(formula, results_cache);
                    } break;

                    case Q_ABSENCE: {
                        // The difference with absence is that, if it is absent, then it shall not be there with the same number
                        absence_or_exists(formula, results_cache);
                        formula->result = negateUntimed(formula->result, kb.act_table_by_act_id.trace_length, false);
                    } break;
                }
            }
        PARALLELIZE_LOOP_END
        // TODO: parallelized run over the it->second, and store the results
    }

    if (!qm.Q.empty()) {
        // Only one possibility, for MaxSAT
        ltlf_query conjunction;
        conjunction.args = qm.Q.begin()->second;
        local_intersection(&conjunction, this->result, false);

        // Others should follow, like the one for maximum satisfiability, for computing
    } else {
    }


}

