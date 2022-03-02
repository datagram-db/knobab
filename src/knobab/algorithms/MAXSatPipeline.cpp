//
// Created by giacomo on 20/01/2022.
//

#include <magic_enum.hpp>
#include <knobab/predicates/SimpleDataPredicate.h>
#include <knobab/predicates/PredicateManager.h>
#include <knobab/utilities/SetOperators.h>
#include "knobab/algorithms/MAXSatPipeline.h"



std::string MAXSatPipeline::LEFT_ATOM{"a"};
std::string MAXSatPipeline::RIGHT_ATOM{"b"};



MAXSatPipeline::MAXSatPipeline(size_t nThreads)
#ifdef MAXSatPipeline_PARALLEL
    : pool(nThreads)
#endif
{
    // TODO: with different specifications
    for (declare_templates t : magic_enum::enum_values<declare_templates>()) {
        if (isUnaryPredicate(t)) continue; // discarding unary predicates
        ltlf_semantics.emplace(t, DeclareDataAware::binary(t, LEFT_ATOM, RIGHT_ATOM)
                .toFiniteSemantics(false)
                .nnf(false)).first->second.mark_join_condition(LEFT_ATOM, RIGHT_ATOM);
    }
}

void MAXSatPipeline::clear() {
    data_accessing.clear();
    declare_atomization.clear();
    atomToFormulaOffset.clear();
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
        data_chunk(model, atomization);
        auto end = std::chrono::system_clock::now();
        auto elapsed =
                std::chrono::duration<double, std::milli>(end - start);
        declare_to_ltlf_time = elapsed.count();
    }

    {
        auto start = std::chrono::system_clock::now();
        data_pipeline_first(kb);
        auto end = std::chrono::system_clock::now();
        auto elapsed =
                std::chrono::duration<double, std::milli>(end - start);
        ltlf_query_time = elapsed.count();
    }

}

void MAXSatPipeline::data_chunk(CNFDeclareDataAware *model,
                                const AtomizingPipeline& atomization) {

    if (!model) return;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> empty_result{};
    declare_model = model;
    std::vector<ltlf_query *> W;
    //label_set_t visitedAtoms;
    for (auto& it : declare_model->singleElementOfConjunction) {
        for (auto& item : it.elementsInDisjunction) {
            auto& ref = declare_atomization[item];
            if (!ref.empty()) continue; // Ignoring already visited patterns

            ltlf_query *formula;
            if (item.casusu == Init) {
                formula = qm.init1(item.left_act, item.left_decomposed_atoms);
                generateAtomQuery(toUseAtoms, empty_result, item, formula, InitQuery);
            } else if (item.casusu == End) {
                formula = qm.end1(item.left_act, item.left_decomposed_atoms);
                generateAtomQuery(toUseAtoms, empty_result, item, formula, EndsQuery);
            } else if (item.casusu == Existence) {
                formula = qm.exists1(item.left_act, item.left_decomposed_atoms);
                generateAtomQuery(toUseAtoms, empty_result, item, formula, ExistsQuery);
            }


            else {
                auto& ltlf_sem = ltlf_semantics[item.casusu];
                //std::vector<size_t> requirements;
                std::unordered_map<std::string, std::unordered_set<bool>> collection;
                ltlf_sem.collectElements(collection);

                // Collecting all of the atoms, for then transferring those to the analysis per atom
                localExtract(atomization, toUseAtoms, ref, collection, item.left_decomposed_atoms, LEFT_ATOM);
                localExtract(atomization, toUseAtoms, ref, collection, item.right_decomposed_atoms, RIGHT_ATOM);

                // Reconstructing the join
                auto tmp = ltlf_sem.replace_with(ref);
                tmp.instantiateJoinCondition(item.conjunctive_map);

                // Creating the formula
                formula = qm.simplify(tmp);
                //std::swap(formula->partial_results, requirements);
            }

            fomulaidToFormula.emplace_back(formula);
            maxFormulaId++;
            W.emplace_back(formula);
            human_readable_ltlf_printing(std::cout, formula) << std::endl; //todo: debugging
        }
    }

    for (auto& ref : atomToFormulaId)
        remove_duplicates(ref.second);

    qm.finalize_unions(W); // Squared on the size of the atoms

    for (const auto& ref : W) {
        human_readable_ltlf_printing(std::cout, ref) << std::endl; //todo: debugging
    }

    remove_duplicates(toUseAtoms);

    // Just the atom iteration
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
        atomToFormulaOffset.emplace_back(std::set<size_t>{offset});
    }
    barrier_to_range_queries = data_accessing.size();
    barriers_to_atfo = atomToFormulaOffset.size();

    for (auto it = toUseAtomsEnd, en = toUseAtoms.end(); it != en; it++) {
        auto it2 = atomization.atom_to_conjunctedPredicates.find(*it);
        assert(it2 != atomization.atom_to_conjunctedPredicates.end());

        std::set<size_t> ranges;
        for (const auto& clause : it2->second) {
            // Sanity Checks
            assert(clause.casusu == INTERVAL);
            assert(clause.BiVariableConditions.empty());

            // TODO: split the RangeQuery by clause.var (as they are going to be on different tables),
            //       clause.label (as the ranges are ordered by act and then by value) and then sort
            //       the remainder by interval. By doing so, we are going to pay the access to clause.label
            //       only once, and we are going to scan the whole range at most linearly instead of performing
            //       at most linear scans for each predicate.
            //       Then, the access can be parallelized by variable name, as they are going to query separated tables
            auto q = DataQuery::RangeQuery(clause.label, clause.var, clause.value, clause.value_upper_bound);
            auto find = data_offset.emplace(q, data_accessing.size());
            if (find.second){
                maxPartialResultId = std::max(maxPartialResultId, (ssize_t)data_accessing.size());
                ranges.insert(data_accessing.size());
                data_accessing_range_query_to_offsets[clause.var][clause.label].emplace_back(data_accessing.size());
                data_accessing.emplace_back(q, empty_result);
            } else {
                maxPartialResultId = std::max(maxPartialResultId, (ssize_t)find.first->second);
                ranges.insert(find.first->second);
            }
        }
        atomToFormulaOffset.emplace_back(ranges);
    }
}

void
MAXSatPipeline::generateAtomQuery(std::vector<std::string> &toUseAtoms,
                                  std::vector<std::pair<std::pair<trace_t, event_t>, double>> &empty_result,
                                  DeclareDataAware &item, ltlf_query *formula, DataQueryType r) {
    size_t offset;
    auto q = DataQuery::AtomQueries(r, item.left_act);
    auto find = data_offset.emplace(q, data_accessing.size());
    if (find.second){
        formula->partial_results.emplace(data_accessing.size()); // TODO: removable
        offset = data_accessing.size();
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
    atomToFormulaOffset.emplace_back(std::set<size_t>{offset});
    maxPartialResultId = std::max(maxPartialResultId, (ssize_t)offset);
}

void
MAXSatPipeline::localExtract(const AtomizingPipeline &atomization,
                             std::vector<std::string> &toUseAtoms,
                             std::unordered_map<std::pair<bool, std::string>, label_set_t> &ref,
                             std::unordered_map<std::string, std::unordered_set<bool>> &collection,
                             const std::unordered_set<std::string> &decomposition,
                             const std::string &collectionMapKey) {
    assert(!decomposition.empty());
    auto it = collection.find(collectionMapKey);
    if (it != collection.end()) {
        for (bool isNegated : it->second) {
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
    }
}




static inline void setUnion(const MAXSatPipeline::partial_result& lhs,
                            const MAXSatPipeline::partial_result& rhs,
                            std::back_insert_iterator<MAXSatPipeline::partial_result> d_first) {
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

static inline void setIntersection(const MAXSatPipeline::partial_result& lhs,
                                   const MAXSatPipeline::partial_result& rhs,
                                    std::back_insert_iterator<MAXSatPipeline::partial_result> d_first) {
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
    std::copy(first2, last2, d_first);
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

static inline MAXSatPipeline::partial_result local_intersection(const std::set<size_t> &vecs,
                                               const std::vector<MAXSatPipeline::partial_result>& results) {
    if (vecs.empty()) return {};
    auto it = vecs.begin();
    auto last_intersection = results.at(*it);
    MAXSatPipeline::partial_result curr_intersection;
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

static inline dataContainer local_intersection(const std::set<size_t> &vecs,
                                               const std::vector<MAXSatPipeline::partial_result>& results,
                                               LeafType isLeaf) {
    if (vecs.empty()) return {};
    MAXSatPipeline::partial_result last_intersection = local_intersection(vecs, results);

    // TODO: better done through views!
    dataContainer  finalResult;
    for (auto it = last_intersection.begin(); it != last_intersection.end(); it = last_intersection.erase(it)) {
        switch (isLeaf) {
            case ActivationLeaf:
            case TargetLeaf:
                finalResult.emplace_back(it->first, std::make_pair(it->second, std::vector<uint16_t>{it->first.second}));
                break;
            default:
                finalResult.emplace_back(it->first, std::make_pair(it->second, std::vector<uint16_t>{}));
                break;
        }
    }

    return finalResult;
}

static inline dataContainer local_union(const std::set<size_t> &vecs,
                                        const std::vector<MAXSatPipeline::partial_result>& results,
                                        LeafType isLeaf) {
    if (vecs.empty()) return {};
    auto it = vecs.begin();
    auto last_intersection = results.at(*it);
    MAXSatPipeline::partial_result curr_intersection;
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
    dataContainer  finalResult;
    for (auto it = last_intersection.begin(); it != last_intersection.end(); it = last_intersection.erase(it)) {
        switch (isLeaf) {
            case ActivationLeaf:
            case TargetLeaf:
                finalResult.emplace_back(it->first, std::make_pair(it->second, std::vector<uint16_t>{it->first.second}));
                break;
            default:
                finalResult.emplace_back(it->first, std::make_pair(it->second, std::vector<uint16_t>{}));
                break;
        }
    }

    return finalResult;
}

static inline dataContainer local_union(const ltlf_query* q, bool isTimed = true) {
    if ((!q) || (q->args.empty())) return {};
    auto it = q->args.begin();
    auto last_union = (*it)->result;
    dataContainer curr_union;
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
    return last_union;
}

static inline dataContainer local_intersection(const ltlf_query* q, bool isTimed = true) {
    if ((!q) || (q->args.empty())) return {};
    auto it = q->args.begin();
    auto last_union = (*it)->result;
    dataContainer curr_union;
    for (std::size_t i = 1; i < q->args.size(); ++i) {
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
    return last_union;
}




void MAXSatPipeline::data_pipeline_first(const KnowledgeBase& kb) {

    // 1. Performing the query over each single predicate that we have extracted, so not to duplicate the data access
    if (barrier_to_range_queries > 0) {
        PARALLELIZE_LOOP_BEGIN(pool, 0, barrier_to_range_queries, a, b)
            for (size_t i = a; i < b; i++) {
                auto& ref = data_accessing.at(i);
                // TODO: Given the query in ref.first, put the result in ref.second
                switch (ref.first.type) {
                    case ExistsQuery:
                        ref.second = kb.exists(kb.resolveCountingData(ref.first.label));
                        break;
                    case InitQuery:
                        ref.second = kb.initOrig<std::pair<uint32_t, uint16_t>, double>(ref.first.label).traceApproximations;
                        break;
                    case EndsQuery:
                        ref.second = kb.endsOrig<std::pair<uint32_t, uint16_t>, double>(ref.first.label).traceApproximations;
                        break;
                    //case AtomQuery:
                    //    // TODO: ref.second = kb.exists<std::pair<uint32_t, uint16_t>, double>(ref.first.label).traceApproximations;
                    //    break;
                    default:
                        assert(false); // This should be dealt in (B)
                }
            }
        PARALLELIZE_LOOP_END
    }


    // 2. Performing the queries over the range queries
    if (!data_accessing_range_query_to_offsets.empty()) {
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

    }

    auto result = partition_sets(atomToFormulaOffset); // O: squared on the size of the atoms
    size_t isFromFurtherDecomposition = result.minimal_common_subsets.size();

    std::vector<MAXSatPipeline::partial_result> resultOfS(result.minimal_common_subsets.size()+result.minimal_common_subsets_composition.size());
    PARALLELIZE_LOOP_BEGIN(pool, 0,result.minimal_common_subsets.size(), lb, ub)
        for (size_t i = lb; i < ub; i++) {
            auto& S = result.minimal_common_subsets.at(i);
            // resultOfS for collecting the intermediate computations
            resultOfS[i] = local_intersection(S, data_accessing);
        }
    PARALLELIZE_LOOP_END

    PARALLELIZE_LOOP_BEGIN(pool, 0,result.minimal_common_subsets_composition.size(), lb, ub)
        for (size_t i = lb; i < ub; i++) {
            auto& S = result.minimal_common_subsets_composition.at(i);
            // Perform the intersection among all of the elements in S,
            // using the intermediate results from resultOfSSecond
            resultOfS[isFromFurtherDecomposition + i] = local_intersection(S, resultOfS);
        }
    PARALLELIZE_LOOP_END


    ///results_cache.resize(toUseAtoms.size());
    std::vector<MAXSatPipeline::partial_result> results_cache(std::max(toUseAtoms.size(), resultOfS.size()), MAXSatPipeline::partial_result{});
    PARALLELIZE_LOOP_BEGIN(pool, 0,result.decomposedIndexedSubsets.size(), lb, ub)
        for (size_t j = lb; j < ub; j++) {
            auto& ref = result.decomposedIndexedSubsets.at(j);
            // put the global intersection into a map representation.
            // ref->first will correspond to the atom in that same position in toUseAtoms.
            results_cache[ref.first] = local_intersection(*ref.second, resultOfS);
        }
    PARALLELIZE_LOOP_END

    resultOfS.clear();

    // Preparing the second phase of the pipeline, where the extracted data is going to be combined.
    for (size_t i = 0, N = toUseAtoms.size(); i<N; i++) {
        auto& atom = toUseAtoms.at(i);
        for (size_t formulaId : atomToFormulaId.at(atom)) {
            fomulaidToFormula.at(formulaId)->associateDataQueryIdsToFormulaByAtom(atom, i);
        }
    }

    auto it = qm.Q.rbegin(), en = qm.Q.rend();
    for (; it != en; it++) {
        PARALLELIZE_LOOP_BEGIN(pool, 0, it->second.size(), lb, ub)
            for (size_t j = lb; j < ub; j++) {
                auto formula = it->second.at(j); // TODO: run this query
                switch (formula->casusu) {
                    case Q_TRUE:
                        break;
                    case Q_NEXT:
                        break;

                    case Q_FALSE:
                        // Empty result by default
                        formula->result.clear();
                        break;

                    case Q_ACT:
                    case Q_INIT:
                    case Q_END:
                        formula->result = local_union(formula->partial_results, results_cache, formula->isLeaf);
                        break;

                    case Q_AND:
                        // TODO: theta
                        formula->result = local_intersection(formula, formula->isTimed);
                        break;

                    case Q_OR:
                        // TODO: theta
                        formula->result = local_union(formula, formula->isTimed);
                        break;

                    case Q_XOR:
                        formula->result = local_union(formula, formula->isTimed);
                        break;

                    case Q_BOX:
                        break;
                    case Q_DIAMOND:
                        break;
                    case Q_UNTIL:
                        break;
                    case Q_RELEASE:
                        break;
                    case Q_LAST:
                        break;

                    case Q_EXISTS:
                        break;
                }
            }
        PARALLELIZE_LOOP_END
        // TODO: parallelized run over the it->second, and store the results
    }

    if (!qm.Q.empty()) {
        ltlf_query conjunction;
        conjunction.args = qm.Q.begin()->second;
        this->result = local_intersection(&conjunction, false);
    } else {
    }


}

