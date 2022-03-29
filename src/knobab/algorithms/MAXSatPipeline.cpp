//
// Created by giacomo on 20/01/2022.
//

#include <magic_enum.hpp>
<<<<<<< HEAD
#include <knobab/predicates/PredicateManager.h>
#include <knobab/utilities/SetOperators.h>
#include <knobab/operators/simple_ltlf_operators.h>
#include <yaucl/functional/assert.h>
#include <knobab/operators/fast_ltlf_operators.h>
#include "knobab/algorithms/MAXSatPipeline.h"


MAXSatPipeline::MAXSatPipeline(const std::string& plan_file, const std::string& plan, size_t nThreads)
=======
#include <knobab/predicates/SimpleDataPredicate.h>
#include <knobab/predicates/PredicateManager.h>
#include <knobab/utilities/SetOperators.h>
#include <knobab/operators/simple_ltlf_operators.h>
#include "knobab/algorithms/MAXSatPipeline.h"



std::string MAXSatPipeline::LEFT_ATOM{"a"};
std::string MAXSatPipeline::RIGHT_ATOM{"b"};



MAXSatPipeline::MAXSatPipeline(size_t nThreads)
>>>>>>> safe_for_benchmark
#ifdef MAXSatPipeline_PARALLEL
    : pool(nThreads)
#endif
{
<<<<<<< HEAD
    // Equivalent to the old expansion of the declare formula into LTLF, and then into the negated normal form.
    std::ifstream file{plan_file};
    dqlp.parse(file);
    auto it = dqlp.planname_to_declare_to_ltlf.find(plan);
    if (it == dqlp.planname_to_declare_to_ltlf.end()) {
        throw std::runtime_error(plan+": the plan is missing from the script file!");
    }
    ptr = &it->second;
}

void MAXSatPipeline::clear() {
    declare_to_query.clear();
=======
    // TODO: with different specifications
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
}

void MAXSatPipeline::clear() {
>>>>>>> safe_for_benchmark
    data_accessing.clear();
    declare_atomization.clear();
    atomToResultOffset.clear();
    toUseAtoms.clear();
    atomicPartIntersectionResult.clear();
    result.clear();
<<<<<<< HEAD
    support_per_declare.clear();
}


static inline void local_logic_intersection(const LTLfQuery* q, Result& last_union, bool isTimed = true) {
    size_t N = q->args.size();
    if ((!q) || (N == 0)) {
        last_union.clear();
        return;
    } else if (N == 1) {
        last_union = q->args.at(0)->result;
    } else if (N == 2) {
        auto &arg1 = q->args.at(0);
        auto &arg2 = q->args.at(1);
        last_union.clear();
        if (isTimed)
            and_logic_timed(arg1->result,
                            arg2->result,
                            last_union, q->joinCondition);
        else
            and_logic_untimed(arg1->result,
                                   arg2->result,
                                   last_union, q->joinCondition);
    } else {
        DEBUG_ASSERT(q->joinCondition);
        auto it = q->args.begin();
        last_union = (*it)->result;
        Result curr_union;
        for (std::size_t i = 1; i < N; ++i) {
            it++;
            auto ref = (*it)->result;
            if (isTimed)
                and_logic_timed(last_union, ref, curr_union);
            else
                and_logic_untimed(last_union, ref, curr_union);
            std::swap(last_union, curr_union);
            curr_union.clear();
        }
    }
}

static inline void local_fast_intersection(const LTLfQuery* q, Result& last_union, bool isTimed = true) {
    size_t N = q->args.size();
    if ((!q) || (N == 0)) {
        last_union.clear();
        return;
    } else if (N == 1) {
        last_union = q->args.at(0)->result;
    } else if (N == 2) {
        auto &arg1 = q->args.at(0);
        auto &arg2 = q->args.at(1);
        last_union.clear();
        if (isTimed)
            and_fast_timed(arg1->result,
                            arg2->result,
                            last_union, q->joinCondition);
        else
            and_fast_untimed(arg1->result,
                              arg2->result,
                              last_union, q->joinCondition);
    } else {
        DEBUG_ASSERT(q->joinCondition);
        auto it = q->args.begin();
        last_union = (*it)->result;
        Result curr_union;
        for (std::size_t i = 1; i < N; ++i) {
            it++;
            auto ref = (*it)->result;
            if (isTimed)
                and_fast_timed(last_union, ref, curr_union);
            else
                and_fast_untimed(last_union, ref, curr_union);
            std::swap(last_union, curr_union);
            curr_union.clear();
        }
    }
=======
>>>>>>> safe_for_benchmark
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
<<<<<<< HEAD
        std::vector<PartialResult> pr = subqueriesRunning(kb);
        switch (operators) {
            case AbidingLogic:
                abidinglogic_query_running(pr, kb);
                break;

            case FastOperator_v1:
                fast_v1_query_running(pr, kb);
                break;

            default:
                break;
        }

        if (!qm.Q.empty()) {
            switch (final_ensemble) {
                case PerDeclareSupport: {
                    std::unordered_map<LTLfQuery*, double> visited;
                    for (size_t i = 0, N = declare_to_query.size(); i<N; i++) { // each declare i
                        const auto &declare = declare_to_query.at(i);
                        auto &localActivations = qm.activations.at(i)->result;
                        if (localActivations.empty()) {
                            DEBUG_ASSERT(declare->result.empty());
                            support_per_declare.emplace_back(0);
                        } else {
                            auto it2 = visited.emplace(declare, 0);
                            if (it2.second) {
                                auto it = localActivations.begin(), en = localActivations.end();
                                auto val = it->first.first;
                                double numerator = 0.0;
                                double denominator = 1.0; // count == 1
                                it++;
                                for (; it != en; it++) {
                                    if (it->first.first != val) {
                                        val = it->first.first;
                                        denominator += 1.0;
                                    }
                                }
                                for (const auto& trace : declare->result) {
                                    if ((!trace.second.second.empty()) && IS_MARKED_EVENT_ACTIVATION(trace.second.second.at(0)))
                                        numerator += 1.0;
                                }
                                it2.first->second = numerator / denominator;
                            }
                            support_per_declare.emplace_back(it2.first->second);
                        }
                    }
                } break;

                case TraceMaximumSatisfiability: {
                    // Working under the assumption that all of the final Declare clauses are, at the root level, untimed operations
                    max_sat_per_trace.resize(kb.noTraces, 0.0);
                    for (const auto& q : qm.Q.begin()->second) {
                        for (const auto& rx: q->result) {
                            // Counting the trace if and only if it has a near-1 value
                            if (std::abs(rx.second.first - 1.0) <= std::numeric_limits<double>::epsilon())
                                max_sat_per_trace[rx.first.first]++;
                        }
                    }
                    for (auto& ref : max_sat_per_trace) {
                        assert(ref <= declare_to_query.size());
                        ref = ref / ((double)declare_to_query.size());
                    }
                } break;

                case TraceIntersection: {
                    LTLfQuery conjunction;
                    conjunction.args = qm.Q.begin()->second;
                    local_fast_intersection(&conjunction, this->result, false);
                } break;

                default: break;
            }
        }

        auto end = std::chrono::system_clock::now();
        pr.clear();
=======
        actual_query_running(kb);
        auto end = std::chrono::system_clock::now();
>>>>>>> safe_for_benchmark
        auto elapsed =
                std::chrono::duration<double, std::milli>(end - start);
        ltlf_query_time = elapsed.count();
    }

}

void MAXSatPipeline::data_chunk(CNFDeclareDataAware *model,
                                const AtomizingPipeline& atomization,
                                const KnowledgeBase& kb) {

    if (!model) return;
<<<<<<< HEAD
    qm.current_query_id = 0;
    static std::vector<std::pair<std::pair<trace_t, event_t>, double>> empty_result{};
    declare_model = model;
    std::vector<LTLfQuery*> W;
    //label_set_t visitedAtoms;
    size_t declareId = 0;
    for (auto& it : declare_model->singleElementOfConjunction) {
        for (auto& item : it.elementsInDisjunction) {
            // Skipping already-met definitions: those will only duplicate the code to be run!
            auto cp = declare_atomization.emplace(item, declareId++);
            if (!cp.second) {
                declare_to_query.emplace_back(declare_to_query.at(cp.first->second));
                continue;
            }

            // Setting the knowledge base, so to exploit it for join queries
            item.kb = &kb;

            // Getting the definition of Declare from the Query rewriter
            auto it2 = ptr->find(item.casusu);
            if (it2 == ptr->end()) {
                throw std::runtime_error(item.casusu+": missing from the loaded query decomposition");
            }

            // Caching the query, so to generate a pointer to an experssion that was already computed.
            // The query plan manager will identfy the common expressions, and will let represent those only ones
            // via caching and mapping.
            LTLfQuery* formula = qm.simplify(atomization.act_atoms,
                                             maxFormulaId,
                                             it2->second,
                                             item.isTruth() ? nullptr : (const DeclareDataAware *) &item,
                                             atomization.atom_universe,
                                             item.left_decomposed_atoms,
                                             item.right_decomposed_atoms,
                                             toUseAtoms,
                                             atomToFormulaId);

            // Setting specific untimed atom queries, that can be run directly and separatedly

            formula = pushAtomicQueries(atomization, formula, true);
            W.emplace_back(formula);
            // Storing the expression that we analysed.
            fomulaidToFormula.emplace_back(formula);
            maxFormulaId++;
            declare_to_query.emplace_back(formula);
            qm.current_query_id++;
=======
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
            } else if (item.casusu == End) {
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
            } else if (item.casusu == Existence) {
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
            } else if (item.casusu == Absence) {
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
            }


            else {
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
>>>>>>> safe_for_benchmark
        }
    }

    for (auto& ref : atomToFormulaId)
        remove_duplicates(ref.second);
<<<<<<< HEAD
    qm.finalize_unions(atomization, W, (KnowledgeBase*)&kb); // Time Computational Complexity: Squared on the size of the atoms
//
//#ifdef DEBUG
//    for (const auto& ref : W) {
//        std::cout << *ref << std::endl;
//    }
//#endif
=======
    qm.finalize_unions(W, (KnowledgeBase*)&kb); // Time Computational Complexity: Squared on the size of the atoms

    /////////////////////////////// DEBUGGING
    for (const auto& ref : W) {
        human_readable_ltlf_printing(std::cout, ref) << std::endl;
    }
    /////////////////////////////////////////

>>>>>>> safe_for_benchmark
    remove_duplicates(toUseAtoms);

    // TODO: AtomQuery is not required, as range queries already have the atom information!
    // Just the Act iteration
    auto toUseAtomsEnd = std::stable_partition(toUseAtoms.begin(), toUseAtoms.end(), [&](const auto& x) { return atomization.act_atoms.contains(x); });
    for (auto it = toUseAtoms.begin(); it != toUseAtomsEnd; it++) {
<<<<<<< HEAD
        pushAtomDataQuery(DataQuery::AtomQuery(*it), false);
=======
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
>>>>>>> safe_for_benchmark
    }
    barrier_to_range_queries = data_accessing.size();
    barriers_to_atfo = atomToResultOffset.size();

    // Iterating other the remaining non-act atoms, that is, the data predicate+label ones
    for (auto it = toUseAtomsEnd, en = toUseAtoms.end(); it != en; it++) {
        auto interval_to_interval_queries_to_intersect = atomization.atom_to_conjunctedPredicates.find(*it);
<<<<<<< HEAD
        if (interval_to_interval_queries_to_intersect == atomization.atom_to_conjunctedPredicates.end())
            throw std::runtime_error(*it + ": element not in map");
=======
        assert(interval_to_interval_queries_to_intersect != atomization.atom_to_conjunctedPredicates.end());
>>>>>>> safe_for_benchmark

        std::vector<size_t> tmpRanges;
        for (const auto& interval_data_query : interval_to_interval_queries_to_intersect->second) {
            // Sanity Checks
<<<<<<< HEAD
            DEBUG_ASSERT(interval_data_query.casusu == INTERVAL);
            DEBUG_ASSERT(interval_data_query.BiVariableConditions.empty());
=======
            assert(interval_data_query.casusu == INTERVAL);
            assert(interval_data_query.BiVariableConditions.empty());
>>>>>>> safe_for_benchmark

            //       splits the RangeQuery by clause.var (as they are going to be on different tables),
            //       clause.label (as the ranges are ordered by act and then by value) and then sort
            //       the remainder by interval. By doing so, we are going to pay the access to clause.label
            //       only once, and we are going to scan the whole range at most linearly instead of performing
            //       at most linear scans for each predicate.
<<<<<<< HEAD
            //       Then, the access canfomulaidToFormula be parallelized by variable name, as they are going to query separated tables
=======
            //       Then, the access can be parallelized by variable name, as they are going to query separated tables
>>>>>>> safe_for_benchmark
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
<<<<<<< HEAD
//#ifdef DEBUG
//        std::cout << atomToResultOffset.size() << "<=>" << interval_to_interval_queries_to_intersect->first << "-->" << tmpRanges << std::endl;
//#endif
=======
        std::cout << atomToResultOffset.size() << "<=>" << interval_to_interval_queries_to_intersect->first << "-->" << tmpRanges << std::endl;
>>>>>>> safe_for_benchmark
        atomToResultOffset.emplace_back(tmpRanges.begin(), tmpRanges.end());
    }
}

<<<<<<< HEAD
LTLfQuery *MAXSatPipeline::pushAtomicQueries(const AtomizingPipeline &atomization, LTLfQuery *formula, bool isRoot) {
    if (!formula) return formula;
    for (const auto ptr : formula->args)
        pushAtomicQueries(atomization, ptr,  false);
    if ((!formula->fields.id.parts.is_timed) && (formula->isLeaf != NotALeaf) && (atomization.act_atoms.contains(*formula->atom.begin()))) {
        switch (formula->t) {
            case LTLfQuery::INIT_QP:{
                formula->result_id = pushAtomDataQuery(DataQuery::InitQuery(*formula->atom.begin()), true);
                formula->fields.id.parts.directly_from_cache = true;
                //W.emplace_back(formula);
                break;
            }
            case LTLfQuery::END_QP: {
                formula->result_id = pushAtomDataQuery(DataQuery::EndsQuery(*formula->atom.begin()), true);
                formula->fields.id.parts.directly_from_cache = true;
                //W.emplace_back(formula);
                break;
            }
            case LTLfQuery::EXISTS_QP: {
                formula->result_id = pushAtomDataQuery(DataQuery::ExistsQuery(*formula->atom.begin(), formula->n), true);
                formula->fields.id.parts.directly_from_cache = true;
                //W.emplace_back(formula);
                break;
            }
            case LTLfQuery::ABSENCE_QP: {
                formula->result_id = pushAtomDataQuery(DataQuery::AbsenceQuery(*formula->atom.begin(), formula->n), true);
                formula->fields.id.parts.directly_from_cache = true;
                //W.emplace_back(formula);
                break;
            }

            default: break;
        }
    } else if (((formula->fields.id.parts.is_timed) && (formula->t == LTLfQuery::LAST_QP || formula->t == LTLfQuery::FIRST_QP))) {
        switch (formula->t) {
            case LTLfQuery::FIRST_QP: {
                formula->result_id = pushAtomDataQuery(DataQuery::FirstQuery(), true);
                formula->fields.id.parts.directly_from_cache = true;
                //W.emplace_back(formula);
                break;
            }
            case LTLfQuery::LAST_QP: {
                formula->result_id = pushAtomDataQuery(DataQuery::LastQuery(), true);
                formula->fields.id.parts.directly_from_cache = true;
                //W.emplace_back(formula);
                break;
            }

            default: break;
        }
    } else {
        //W.emplace_back(formula);
        if (formula->args.empty()) {
            qm.atomsToDecomposeInUnion.emplace_back(formula);
        }
        formula->fields.id.parts.directly_from_cache = false;
        toUseAtoms.insert(toUseAtoms.end(), formula->atom.begin(), formula->atom.end());
    }
    return formula;
}

size_t MAXSatPipeline::pushAtomDataQuery(const DataQuery &q, bool directlyFromCache) {
    static std::vector<std::pair<std::pair<trace_t, event_t>, double>> empty_result{};
    size_t offset = data_accessing.size();
    auto find = data_offset.emplace(q, offset);
    if (find.second){
        offset = data_accessing.size();
        data_accessing.emplace_back(q, empty_result);
    } else
        offset = (find.first->second);
    maxPartialResultId = std::max(maxPartialResultId, (ssize_t)offset);
    if (!directlyFromCache) {
        atomToResultOffset.emplace_back(std::set<size_t>{offset});
    }
    return offset;
}


static inline void partialResultUnion(const PartialResult& lhs,
                                      const PartialResult& rhs,
                                      std::back_insert_iterator<PartialResult> d_first) {
=======
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
    assert(!decomposition.empty());
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
>>>>>>> safe_for_benchmark
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

<<<<<<< HEAD
static inline void partialResultIntersection(const PartialResult& lhs,
                                             const PartialResult& rhs,
                                             std::back_insert_iterator<PartialResult> d_first) {
=======
static inline void setIntersection(const PartialResult& lhs,
                                   const PartialResult& rhs,
                                    std::back_insert_iterator<PartialResult> d_first) {
>>>>>>> safe_for_benchmark
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
<<<<<<< HEAD
std::vector<std::pair<std::pair<trace_t, event_t>, double>> partialResultIntersection(const std::set<size_t> &vecs,
=======
std::vector<std::pair<std::pair<trace_t, event_t>, double>> local_intersection(const std::set<size_t> &vecs,
>>>>>>> safe_for_benchmark
                                                                               const std::vector<std::pair<DataQuery, std::vector<std::pair<std::pair<trace_t, event_t>, double>>>>& results) {
    if (vecs.empty()) return {};
    auto it = vecs.begin();
    auto last_intersection = results.at(*it).second;
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> curr_intersection;
    for (std::size_t i = 1; i < vecs.size(); ++i) {
        it++;
        auto ref = results.at(*it).second;
<<<<<<< HEAD
        partialResultIntersection(last_intersection,
                                  ref,
                                  std::back_inserter(curr_intersection));
=======
        setIntersection(last_intersection,
                              ref,
                              std::back_inserter(curr_intersection));
>>>>>>> safe_for_benchmark
        std::swap(last_intersection, curr_intersection);
        curr_intersection.clear();
    }
    return last_intersection;
}

<<<<<<< HEAD
static inline PartialResult partialResultIntersection(const std::set<size_t> &vecs,
=======
static inline PartialResult local_intersection(const std::set<size_t> &vecs,
>>>>>>> safe_for_benchmark
                                               const std::vector<PartialResult>& results) {
    if (vecs.empty()) return {};
    auto it = vecs.begin();
    auto last_intersection = results.at(*it);
    PartialResult curr_intersection;
    for (std::size_t i = 1; i < vecs.size(); ++i) {
        it++;
        auto& ref = results.at(*it);
<<<<<<< HEAD
        partialResultIntersection(last_intersection,
                                  ref,
                                  std::back_inserter(curr_intersection));
=======
        setIntersection(last_intersection,
                        ref,
                        std::back_inserter(curr_intersection));
>>>>>>> safe_for_benchmark
        std::swap(last_intersection, curr_intersection);
        curr_intersection.clear();
    }

    return last_intersection;
}

static inline Result local_intersection(const std::set<size_t> &vecs,
                                        const std::vector<PartialResult>& results,
                                        LeafType isLeaf) {
    if (vecs.empty()) return {};
<<<<<<< HEAD
    PartialResult last_intersection = partialResultIntersection(vecs, results);

    // TODO: better done through views!
    Result  finalResult;
    for (auto it = last_intersection.begin(); it != last_intersection.end(); it++) {
=======
    PartialResult last_intersection = local_intersection(vecs, results);

    // TODO: better done through views!
    Result  finalResult;
    for (auto it = last_intersection.begin(); it != last_intersection.end(); it = last_intersection.erase(it)) {
>>>>>>> safe_for_benchmark
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
<<<<<<< HEAD
        partialResultUnion(last_intersection,
                           ref,
                           std::back_inserter(curr_intersection));
=======
        setUnion(last_intersection,
                        ref,
                        std::back_inserter(curr_intersection));
>>>>>>> safe_for_benchmark
        std::swap(last_intersection, curr_intersection);
        curr_intersection.clear();
    }

<<<<<<< HEAD
    ResultRecord rcx;
    if (isLeaf == ActivationLeaf)
        rcx.second.second.emplace_back(marked_event::activation(0));
    else if (isLeaf == TargetLeaf)
        rcx.second.second.emplace_back(marked_event::target(0));

    // TODO: better done through views!
    for (auto it = last_intersection.begin(); it != last_intersection.end(); it++) {
        rcx.first = it->first;
        rcx.second.first = it->second;
        if (isLeaf == ActivationLeaf)
            rcx.second.second.at(0).id.parts.left = it->first.second;
        else if (isLeaf == TargetLeaf)
            rcx.second.second.at(0).id.parts.right = it->first.second;
        result.emplace_back(rcx);
    }
}

static inline void local_logic_union(const LTLfQuery* q, Result& last_union, bool isTimed = true) {
=======
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
>>>>>>> safe_for_benchmark
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
<<<<<<< HEAD
            or_logic_timed(arg1->result,
                            arg2->result,
                           last_union,
                     q->joinCondition);
        else
            or_logic_untimed(arg1->result,
                             arg2->result,
                             last_union,
                             q->joinCondition);
=======
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
>>>>>>> safe_for_benchmark
    } else {
        auto it = q->args.begin();
        last_union = (*it)->result;
        Result curr_union;
        for (std::size_t i = 1; i < q->args.size(); ++i) {
            it++;
            auto ref = (*it)->result;
            if (isTimed)
<<<<<<< HEAD
                or_logic_timed(last_union,
                         ref,
                         curr_union);
            else
                or_logic_untimed(last_union,
                               ref,
                               curr_union);
=======
                setUnion(last_union.begin(), last_union.end(),
                         ref.begin(), ref.end(),
                         std::back_inserter(curr_union),
                         Aggregators::maxSimilarity<double, double, double>);
            else
                setUnionUntimed(last_union.begin(), last_union.end(),
                                ref.begin(), ref.end(),
                                std::back_inserter(curr_union),
                                Aggregators::maxSimilarity<double, double, double>);
>>>>>>> safe_for_benchmark
            std::swap(last_union, curr_union);
            curr_union.clear();
        }
    }
}

<<<<<<< HEAD
static inline void local_fast_union(const LTLfQuery* q, Result& last_union, bool isTimed = true) {
=======
static inline void local_intersection(const ltlf_query* q, Result& last_union, bool isTimed = true) {
>>>>>>> safe_for_benchmark
    size_t N = q->args.size();
    if ((!q) || (N == 0)) {
        last_union.clear();
        return;
    } else if (N == 1) {
<<<<<<< HEAD
        last_union = q->args.at(1)->result;
    } else if (N == 2) {
        last_union.clear();
        auto& arg1 = q->args.at(0);
        auto& arg2 = q->args.at(1);
        if (isTimed)
            or_fast_timed(arg1->result,
                           arg2->result,
                           last_union,
                           q->joinCondition);
        else
            or_fast_untimed(arg1->result,
                             arg2->result,
                             last_union,
                             q->joinCondition);
    } else {
        auto it = q->args.begin();
        last_union = (*it)->result;
        Result curr_union;
        for (std::size_t i = 1; i < q->args.size(); ++i) {
            it++;
            auto ref = (*it)->result;
            if (isTimed)
                or_fast_timed(last_union,
                               ref,
                               curr_union);
            else
                or_fast_untimed(last_union,
                                 ref,
                                 curr_union);
=======
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
        assert(q->joinCondition.isTruth());
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
>>>>>>> safe_for_benchmark
            std::swap(last_union, curr_union);
            curr_union.clear();
        }
    }
<<<<<<< HEAD
}

static inline void absence_or_exists(LTLfQuery* formula, const std::vector<PartialResult>& results_cache) {
=======

}

static inline void absence_or_exists(ltlf_query* formula, const std::vector<PartialResult>& results_cache) {
>>>>>>> safe_for_benchmark
    bool isFirstIteration = true;
    uint32_t traceId = 0;
    uint16_t eventCount = 0;
    Result tmp_result;
    data_merge(formula->partial_results, results_cache, tmp_result, formula->isLeaf);
    ResultRecord cp{{0,0}, {1.0, {}}};
<<<<<<< HEAD
    for (auto ref = tmp_result.begin(); ref != tmp_result.end(); ref++) {
=======
    for (auto ref = tmp_result.begin(); ref != tmp_result.end(); ) {
>>>>>>> safe_for_benchmark
        if (isFirstIteration) {
            traceId = ref->first.first;
            eventCount = 1;
            isFirstIteration = false;
        } else {
            if ((traceId != ref->first.first)) {
<<<<<<< HEAD
                if ((eventCount >= formula->n)) {
=======
                if ((eventCount >= formula->numeric_arg)) {
>>>>>>> safe_for_benchmark
                    cp.first.first = traceId;
                    formula->result.emplace_back(cp);
                }
                traceId = ref->first.first;
                eventCount = 1;
            } else eventCount++;
        }
<<<<<<< HEAD
    }
    tmp_result.clear();
    if ((eventCount >= formula->n)) {
=======
        ref = tmp_result.erase(ref);
    }
    if ((eventCount >= formula->numeric_arg)) {
>>>>>>> safe_for_benchmark
        cp.first.first = traceId;
        formula->result.emplace_back(cp);
    }
}


<<<<<<< HEAD
void MAXSatPipeline::abidinglogic_query_running(const std::vector<PartialResult>& results_cache, const KnowledgeBase& kb) {
    /// Scanning the query plan starting from the leaves (rbegin) towards the actual declare formulae (rend)
    auto it = qm.Q.rbegin(), en = qm.Q.rend();
    size_t idx = qm.Q.size()-1;
    for (; it != en; it++) {
        Result tmp_result;
        PARALLELIZE_LOOP_BEGIN(pool, 0, it->second.size(), lb, ub)
            for (size_t j = lb; j < ub; j++) {
                auto formula = it->second.at(j); // TODO: run this query
                if (!formula) continue;

                if (formula->fields.id.parts.directly_from_cache) {
                    ResultRecordSemantics R{1.0, {}};
                    if (formula->isLeaf == ActivationLeaf) {
                        R.second.emplace_back(marked_event::left(0));
                    } else if (formula->isLeaf == TargetLeaf) {
                        R.second.emplace_back(marked_event::right(0));
                    }
                    for (const auto& ref : data_accessing.at(formula->result_id).second) {
                        if (formula->isLeaf == ActivationLeaf)
                            R.second.at(0).id.parts.left = ref.first.second;
                        else if (formula->isLeaf == TargetLeaf)
                            R.second.at(0).id.parts.right = ref.first.second;
                        formula->result.emplace_back(ref.first, R);
                    }
                } else {
                    // Combine the results from the results_cache
                    switch (formula->t) {
                        case LTLfQuery::INIT_QP:
                            data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                            formula->result.erase(std::remove_if(formula->result.begin(),
                                                                 formula->result.end(),
                                                                 [](const auto&  x){return x.first.second > 0;}),
                                                  formula->result.end());
                            break;

                        case LTLfQuery::END_QP:
                            // This never has a theta condition to consider
                            // This will only work when data conditions are also considered
                            data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                            formula->result.erase(std::remove_if(formula->result.begin(),
                                                                 formula->result.end(),
                                                                 [kb](const auto&  x){return x.first.second < kb.act_table_by_act_id.trace_length.at(x.first.first)-1;}),
                                                  formula->result.end());
                            break;

                        case LTLfQuery::EXISTS_QP:
                            if (formula->fields.id.parts.is_timed)
                                data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                            else
                                absence_or_exists(formula, results_cache);
                            break;

                        case LTLfQuery::ABSENCE_QP:
                            // The difference with absence is that, if it is absent, then it shall not be there with the same number
                            if (formula->fields.id.parts.is_timed)
                                data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                            else
                                absence_or_exists(formula, results_cache);
                            if (formula->fields.id.parts.preserve && (!formula->fields.id.parts.is_timed)) {
                                formula->result = negateUntimed(formula->result, kb.act_table_by_act_id.trace_length, true);
                            } else if (formula->fields.id.parts.is_timed) {
                                if (formula->fields.id.parts.preserve)
                                    throw std::runtime_error("At this stage, cannot preserve data for timed");
                                negated_logic_timed(formula->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                std::swap(formula->result, tmp_result);
                            } else {
                                negated_logic_untimed(formula->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                std::swap(formula->result, tmp_result);
                            }
                            break;

                        case LTLfQuery::NEXT_QP:
                            formula->result = next(formula->args.at(0)->result);
                            break;

                        case LTLfQuery::OR_QP:
                            local_fast_union(formula, formula->result, formula->fields.id.parts.is_timed);
                            break;

                        case LTLfQuery::AND_QP:
                            local_logic_intersection(formula, formula->result, formula->fields.id.parts.is_timed);
                            break;

                        case LTLfQuery::IMPL_QP:
                            if (formula->fields.id.parts.is_timed) {
                                negated_logic_timed(formula->args[0]->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                implies_logic_timed(formula->args.at(0)->result,
                                                    formula->args.at(1)->result,
                                                    tmp_result,
                                                    formula->result,
                                                    formula->joinCondition,
                                                    kb.act_table_by_act_id.trace_length);
                            } else {
                                negated_logic_untimed(formula->args[0]->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                implies_logic_untimed(formula->args.at(0)->result,
                                                      formula->args.at(1)->result,
                                                      tmp_result,
                                                      formula->result,
                                                      formula->joinCondition,
                                                      kb.act_table_by_act_id.trace_length);
                            }
                            break;

                        case LTLfQuery::IFTE_QP:
                            if (formula->fields.id.parts.is_timed)
                                implies_logic_timed(formula->args.at(0)->result,
                                                  formula->args.at(1)->result,
                                                    formula->args.at(2)->result,
                                                  formula->result,
                                                  formula->joinCondition,
                                                  kb.act_table_by_act_id.trace_length);
                            else
                                implies_logic_untimed(formula->args.at(0)->result,
                                                    formula->args.at(1)->result,
                                                      formula->args.at(3)->result,
                                                    formula->result,
                                                    formula->joinCondition,
                                                    kb.act_table_by_act_id.trace_length);
                            break;

                        case LTLfQuery::U_QP:
                            if (formula->fields.id.parts.is_timed)
                                until_logic_timed(formula->args.at(0)->result,
                                                  formula->args.at(1)->result,
                                                  formula->result,
                                                  formula->joinCondition,
                                                  kb.act_table_by_act_id.trace_length);
                            else
                                until_logic_untimed(formula->args.at(0)->result,
                                                    formula->args.at(1)->result,
                                                    formula->result,
                                                    formula->joinCondition,
                                                    kb.act_table_by_act_id.trace_length);
                            break;

                        case LTLfQuery::G_QP:
                            if (formula->fields.id.parts.is_timed)
                                global_logic_timed(formula->args.at(0)->result, formula->result, kb.act_table_by_act_id.trace_length);
                            else
                                global_logic_untimed(formula->args.at(0)->result, formula->result, kb.act_table_by_act_id.trace_length);
                            break;

                        case LTLfQuery::F_QP:
                            if (formula->fields.id.parts.is_timed)
                                future_logic_timed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                            else
                                future_logic_untimed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                            break;

                        case LTLfQuery::NOT_QP:
                            if (formula->fields.id.parts.preserve && (!formula->fields.id.parts.is_timed)) {
                                formula->result = negateUntimed(formula->args[0]->result, kb.act_table_by_act_id.trace_length, true);
                            } else if (formula->fields.id.parts.is_timed) {
                                if (formula->fields.id.parts.preserve)
                                    throw std::runtime_error("At this stage, cannot preserve data for timed");
                                negated_logic_timed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                            } else {
                                negated_logic_untimed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                            }
                            break;

                        case LTLfQuery::AF_QPT:
                            if (formula->fields.id.parts.is_timed) {
                                future_logic_timed(formula->args.at(1)->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                and_logic_timed(formula->args.at(0)->result, tmp_result, formula->result, formula->joinCondition, kb.act_table_by_act_id.trace_length);
                                tmp_result.clear();
                            } else {
                                future_logic_timed(formula->args.at(1)->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                and_logic_untimed(formula->args.at(0)->result, tmp_result, formula->result, formula->joinCondition, kb.act_table_by_act_id.trace_length);
                                tmp_result.clear();
                            }
                            break;

                        case LTLfQuery::AXG_QPT:
                            if (formula->fields.id.parts.is_timed) {
                                Result local;
                                global_logic_timed(formula->args.at(1)->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                next_logical(tmp_result, local);
                                and_logic_timed(formula->args.at(0)->result, local, formula->result, formula->joinCondition, kb.act_table_by_act_id.trace_length);
                                tmp_result.clear();
                            } else {
                                Result local;
                                global_logic_timed(formula->args.at(1)->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                next_logical(tmp_result, local);
                                and_logic_untimed(formula->args.at(0)->result, local, formula->result, formula->joinCondition, kb.act_table_by_act_id.trace_length);
                                tmp_result.clear();
                            }
                            break;

                        case LTLfQuery::AG_QPT:
                            if (formula->fields.id.parts.is_timed) {
                                global_logic_timed(formula->args.at(1)->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                and_logic_timed(formula->args.at(0)->result, tmp_result, formula->result, formula->joinCondition, kb.act_table_by_act_id.trace_length);
                                tmp_result.clear();
                            } else {
                                global_logic_timed(formula->args.at(1)->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                and_logic_untimed(formula->args.at(0)->result, tmp_result, formula->result, formula->joinCondition, kb.act_table_by_act_id.trace_length);
                                tmp_result.clear();
                            }
                            break;

                        case LTLfQuery::FALSEHOOD_QP:
                            formula->result.clear();
                            break;
                    }
                }
            }
        PARALLELIZE_LOOP_END

        // Clearing the caches, so to free potentially unrequired memory for the next computational steps
        // This might help save some memory in big-data scenarios
        PARALLELIZE_LOOP_BEGIN(pool, 0, it->second.size(), lb, ub)
        for (size_t j = lb; j < ub; j++) {
            auto formula = it->second.at(j);
            for (auto ptr : formula->args) {
                // Preserving the cache only if I need it for computing the Support
                if (ptr->parentMin == idx && (((final_ensemble != PerDeclareSupport) || (ptr->isLeaf != ActivationLeaf))))
                    ptr->result.clear();
            }
        }
        PARALLELIZE_LOOP_END
        idx--;
    }
}

std::vector<PartialResult> MAXSatPipeline::subqueriesRunning(const KnowledgeBase &kb) {// 1. Performing the query over each single predicate that we have extracted, so not to duplicate the data access
=======
void MAXSatPipeline::actual_query_running(const KnowledgeBase& kb) {

    // 1. Performing the query over each single predicate that we have extracted, so not to duplicate the data access
>>>>>>> safe_for_benchmark
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
<<<<<<< HEAD
                    case FirstQuery:
                        ref.second = kb.getFirstOrLastElements(true);
                        break;
                    case LastQuery:
                        ref.second = kb.getFirstOrLastElements(false);
                        break;

                    default:
                        DEBUG_ASSERT(false); // This should be dealt in (B)
=======
                    //
                    //    // TODO: ref.second = kb.exists<std::pair<uint32_t, uint16_t>, double>(ref.first.label).traceApproximations;
                    //    break;
                    default:
                        assert(false); // This should be dealt in (B)
>>>>>>> safe_for_benchmark
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
<<<<<<< HEAD
// The computational complexity should be squared on the size of the atoms
    auto set_decomposition_result = partition_sets(atomToResultOffset);
    ///std::cout << set_decomposition_result << std::endl;
    size_t isFromFurtherDecomposition = set_decomposition_result.minimal_common_subsets.size();


    std::vector<PartialResult> resultOfS(set_decomposition_result.minimal_common_subsets.size() + set_decomposition_result.minimal_common_subsets_composition.size());
    PARALLELIZE_LOOP_BEGIN(pool, 0, set_decomposition_result.minimal_common_subsets.size(), lb, ub)
=======
    // The computational complexity should be squared on the size of the atoms
    auto set_decomposition_result = partition_sets(atomToResultOffset);
    std::cout << set_decomposition_result << std::endl;
    size_t isFromFurtherDecomposition = set_decomposition_result.minimal_common_subsets.size();

    std::vector<PartialResult> resultOfS(set_decomposition_result.minimal_common_subsets.size() + set_decomposition_result.minimal_common_subsets_composition.size());
    PARALLELIZE_LOOP_BEGIN(pool, 0,set_decomposition_result.minimal_common_subsets.size(), lb, ub)
>>>>>>> safe_for_benchmark
        for (size_t i = lb; i < ub; i++) {
            auto& S = set_decomposition_result.minimal_common_subsets.at(i);
//            std::cout << i << " is " << S << std::endl;
            // resultOfS for collecting the intermediate computations
<<<<<<< HEAD
            resultOfS[i] = partialResultIntersection(S, data_accessing);
//            for (const auto x : S) {
//                edges.emplace("data_accessing:"+std::to_string(x), "resultOfS:"+std::to_string(i));
//            }
        }
    PARALLELIZE_LOOP_END

    PARALLELIZE_LOOP_BEGIN(pool, 0, set_decomposition_result.minimal_common_subsets_composition.size(), lb, ub)
=======
            resultOfS[i] = local_intersection(S, data_accessing);
        }
    PARALLELIZE_LOOP_END

    PARALLELIZE_LOOP_BEGIN(pool, 0,set_decomposition_result.minimal_common_subsets_composition.size(), lb, ub)
>>>>>>> safe_for_benchmark
        for (size_t i = lb; i < ub; i++) {
            auto& S = set_decomposition_result.minimal_common_subsets_composition.at(i);
            // Perform the intersection among all of the elements in S,
            // using the intermediate results from resultOfSSecond
<<<<<<< HEAD
            resultOfS[isFromFurtherDecomposition + i] = partialResultIntersection(S, resultOfS);
//            for (const auto x : S) {
//                edges.emplace("resultOfS:"+std::to_string(x), "resultOfS:"+std::to_string(i));
//            }
=======
            resultOfS[isFromFurtherDecomposition + i] = local_intersection(S, resultOfS);
>>>>>>> safe_for_benchmark
        }
    PARALLELIZE_LOOP_END

//    for (size_t i = 0; i<resultOfS.size(); i++) {
//        std::cout << i << "->>->" << resultOfS.at(i) << std::endl;
//    }

    ///results_cache.resize(toUseAtoms.size());
    std::vector<PartialResult> results_cache(std::max(toUseAtoms.size(), resultOfS.size()), PartialResult{});
<<<<<<< HEAD
    PARALLELIZE_LOOP_BEGIN(pool, 0, set_decomposition_result.decomposedIndexedSubsets.size(), lb, ub)
=======
    PARALLELIZE_LOOP_BEGIN(pool, 0,set_decomposition_result.decomposedIndexedSubsets.size(), lb, ub)
>>>>>>> safe_for_benchmark
        for (size_t j = lb; j < ub; j++) {
            auto& ref = set_decomposition_result.decomposedIndexedSubsets.at(j);
//            std::cout << ref.first << "-@->" << *ref.second << std::endl;
            // put the global intersection into a map representation.
            // ref->first will correspond to the atom in that same position in toUseAtoms.
<<<<<<< HEAD
            results_cache[ref.first] = partialResultIntersection(*ref.second, resultOfS);
//            for (const auto x : *ref.second) {
//                edges.emplace("resultOfS:"+std::to_string(x), "results_cache:"+std::to_string(ref.first));
//            }
=======
            results_cache[ref.first] = local_intersection(*ref.second, resultOfS);
>>>>>>> safe_for_benchmark
        }
    PARALLELIZE_LOOP_END

    resultOfS.clear();
//    for (size_t i = 0; i<results_cache.size(); i++) {
//        std::cout << i << "-->" << results_cache.at(i) << std::endl;
//    }

    // Preparing the second phase of the pipeline, where the extracted data is going to be combined.
<<<<<<< HEAD
    for (size_t i = 0, N = toUseAtoms.size(); i < N; i++) {
        auto& atom = toUseAtoms.at(i);
        for (size_t formulaId : atomToFormulaId.at(atom)) {
            fomulaidToFormula.at(formulaId)->associateDataQueryIdsToFormulaByAtom(atom, i);// setting the partial results for the data pipeline
        }
    }

    return results_cache;
}

void MAXSatPipeline::fast_v1_query_running(const std::vector<PartialResult>& results_cache, const KnowledgeBase& kb) {
/// Scanning the query plan starting from the leaves (rbegin) towards the actual declare formulae (rend)
    auto it = qm.Q.rbegin(), en = qm.Q.rend();
    size_t idx = qm.Q.size()-1;
    for (; it != en; it++) {
        Result tmp_result;
        PARALLELIZE_LOOP_BEGIN(pool, 0, it->second.size(), lb, ub)
            for (size_t j = lb; j < ub; j++) {
                auto formula = it->second.at(j); // TODO: run this query
                if (!formula) continue;

                if (formula->fields.id.parts.directly_from_cache) {
                    ResultRecordSemantics R{1.0, {}};
                    if (formula->isLeaf == ActivationLeaf) {
                        R.second.emplace_back(marked_event::left(0));
                    } else if (formula->isLeaf == TargetLeaf) {
                        R.second.emplace_back(marked_event::right(0));
                    }
                    for (const auto& ref : data_accessing.at(formula->result_id).second) {
                        if (formula->isLeaf == ActivationLeaf)
                            R.second.at(0).id.parts.left = ref.first.second;
                        else if (formula->isLeaf == TargetLeaf)
                            R.second.at(0).id.parts.right = ref.first.second;
                        formula->result.emplace_back(ref.first, R);
                    }
                } else {
                    // Combine the results from the results_cache
                    switch (formula->t) {
                        case LTLfQuery::INIT_QP:
                            data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                            formula->result.erase(std::remove_if(formula->result.begin(),
                                                                 formula->result.end(),
                                                                 [](const auto&  x){return x.first.second > 0;}),
                                                  formula->result.end());
                            break;

                        case LTLfQuery::END_QP:
                            // This never has a theta condition to consider
                            // This will only work when data conditions are also considered
                            data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                            formula->result.erase(std::remove_if(formula->result.begin(),
                                                                 formula->result.end(),
                                                                 [kb](const auto&  x){return x.first.second < kb.act_table_by_act_id.trace_length.at(x.first.first)-1;}),
                                                  formula->result.end());
                            break;

                        case LTLfQuery::EXISTS_QP:
                            if (formula->fields.id.parts.is_timed)
                                data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                            else
                                absence_or_exists(formula, results_cache);
                            break;

                        case LTLfQuery::ABSENCE_QP:
                            // The difference with absence is that, if it is absent, then it shall not be there with the same number
                            if (formula->fields.id.parts.is_timed)
                                data_merge(formula->partial_results, results_cache, formula->result, formula->isLeaf);
                            else
                                absence_or_exists(formula, results_cache);
                            if (formula->fields.id.parts.preserve && (!formula->fields.id.parts.is_timed)) {
                                formula->result = negateUntimed(formula->result, kb.act_table_by_act_id.trace_length, true);
                            } else if (formula->fields.id.parts.is_timed) {
                                if (formula->fields.id.parts.preserve)
                                    throw std::runtime_error("At this stage, cannot preserve data for timed");
                                negated_fast_timed(formula->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                std::swap(formula->result, tmp_result);
                            } else {
                                negated_fast_untimed(formula->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                std::swap(formula->result, tmp_result);
                            }
                            break;

                        case LTLfQuery::NEXT_QP:
                            formula->result = next(formula->args.at(0)->result);
                            break;

                        case LTLfQuery::OR_QP:
                            local_fast_union(formula, formula->result, formula->fields.id.parts.is_timed);
                            break;

                        case LTLfQuery::AND_QP:
                            local_fast_intersection(formula, formula->result, formula->fields.id.parts.is_timed);
                            break;

                        case LTLfQuery::IMPL_QP:
                            if (formula->fields.id.parts.is_timed) {
                                negated_fast_timed(formula->args[0]->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                implies_fast_timed(formula->args.at(0)->result,
                                                    formula->args.at(1)->result,
                                                    tmp_result,
                                                    formula->result,
                                                    formula->joinCondition,
                                                    kb.act_table_by_act_id.trace_length);
                            } else {
                                negated_fast_untimed(formula->args[0]->result, tmp_result, kb.act_table_by_act_id.trace_length);
                                implies_fast_untimed(formula->args.at(0)->result,
                                                      formula->args.at(1)->result,
                                                      tmp_result,
                                                      formula->result,
                                                      formula->joinCondition,
                                                      kb.act_table_by_act_id.trace_length);
                            }
                            break;

                        case LTLfQuery::IFTE_QP:
                            if (formula->fields.id.parts.is_timed)
                                implies_fast_timed(formula->args.at(0)->result,
                                                    formula->args.at(1)->result,
                                                    formula->args.at(2)->result,
                                                    formula->result,
                                                    formula->joinCondition,
                                                    kb.act_table_by_act_id.trace_length);
                            else
                                implies_fast_untimed(formula->args.at(0)->result,
                                                      formula->args.at(1)->result,
                                                      formula->args.at(3)->result,
                                                      formula->result,
                                                      formula->joinCondition,
                                                      kb.act_table_by_act_id.trace_length);
                            break;

                        case LTLfQuery::U_QP:
                            if (formula->fields.id.parts.is_timed)
                                until_fast_timed(formula->args.at(0)->result,
                                                  formula->args.at(1)->result,
                                                  formula->result,
                                                  formula->joinCondition,
                                                  kb.act_table_by_act_id.trace_length);
                            else
                                until_fast_untimed(formula->args.at(0)->result,
                                                    formula->args.at(1)->result,
                                                    formula->result,
                                                    formula->joinCondition,
                                                    kb.act_table_by_act_id.trace_length);
                            break;

                        case LTLfQuery::G_QP:
                            if (formula->fields.id.parts.is_timed)
                                global_fast_timed(formula->args.at(0)->result, formula->result, kb.act_table_by_act_id.trace_length);
                            else
                                global_fast_untimed(formula->args.at(0)->result, formula->result, kb.act_table_by_act_id.trace_length);
                            break;

                        case LTLfQuery::F_QP:
                            if (formula->fields.id.parts.is_timed)
                                future_fast_timed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                            else
                                future_fast_untimed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                            break;

                        case LTLfQuery::NOT_QP:
                            if (formula->fields.id.parts.preserve && (!formula->fields.id.parts.is_timed)) {
                                formula->result = negateUntimed(formula->args[0]->result, kb.act_table_by_act_id.trace_length, true);
                            } else if (formula->fields.id.parts.is_timed) {
                                if (formula->fields.id.parts.preserve)
                                    throw std::runtime_error("At this stage, cannot preserve data for timed");
                                negated_fast_timed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                            } else {
                                negated_fast_untimed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                            }
                            break;

                        case LTLfQuery::AF_QPT:
                            if (formula->fields.id.parts.is_timed)
                                aAndFutureB_timed(formula->args.at(0)->result,
                                                  formula->args.at(1)->result,
                                                  formula->result,
                                                  formula->joinCondition,
                                                  kb.act_table_by_act_id.trace_length);
                            else
                                throw std::runtime_error("AndFuture is untimed: unexpected implementation!");
                            break;

                        case LTLfQuery::AXG_QPT:
                            if (formula->fields.id.parts.is_timed)
                                aAndNextGloballyB_timed(formula->args.at(0)->result,
                                                        formula->args.at(1)->result,
                                                        formula->result,
                                                        formula->joinCondition,
                                                        kb.act_table_by_act_id.trace_length);
                            else
                                throw std::runtime_error("AndNextGlobally is untimed: unexpected implementation!");
                            break;

                        case LTLfQuery::AG_QPT:
                            if (formula->fields.id.parts.is_timed)
                                aAndGloballyB_timed(formula->args.at(0)->result,
                                                        formula->args.at(1)->result,
                                                        formula->result,
                                                        formula->joinCondition,
                                                        kb.act_table_by_act_id.trace_length);
                            else
                                throw std::runtime_error("AndNextGlobally is untimed: unexpected implementation!");
                            break;

                        case LTLfQuery::FALSEHOOD_QP:
                            formula->result.clear();
                            break;
                    }
                }
            }
        PARALLELIZE_LOOP_END

        // Clearing the caches, so to free potentially unrequired memory for the next computational steps
        // This might help save some memory in big-data scenarios
//        PARALLELIZE_LOOP_BEGIN(pool, 0, it->second.size(), lb, ub)
//            for (size_t j = lb; j < ub; j++) {
//                auto formula = it->second.at(j);
//                for (auto ptr : formula->args) {
//                    // Preserving the cache only if I need it for computing the Support
//                    if (ptr->parentMin == idx && (((final_ensemble != PerDeclareSupport) || (ptr->isLeaf != ActivationLeaf))))
//                        ptr->result.clear();
//                }
//            }
//        PARALLELIZE_LOOP_END
        idx--;
    }
}
=======
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
                        assert(false);
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
                        assert(formula->args.size() == 2);
                        local_intersection(formula, formula->result, formula->isTimed);
                        break;

                    case Q_OR:
                    case Q_XOR:
                        assert(formula->args.size() == 2);
                        local_union(formula, formula->result, formula->isTimed);
                        break;


                    case Q_BOX:
                        assert(formula->args.size() == 1);
                        if (formula->isTimed) {
                            global_logic_timed(formula->args.at(0)->result, formula->result, kb.act_table_by_act_id.trace_length);
                        } else {
                            global_logic_untimed(formula->args.at(0)->result, formula->result, kb.act_table_by_act_id.trace_length);
                        }
                        break;

                    case Q_DIAMOND:
                        assert(formula->args.size() == 1);
                        if (formula->isTimed)
                             future_logic_timed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                        else {
                             future_logic_untimed(formula->args[0]->result, formula->result, kb.act_table_by_act_id.trace_length);
                        }
                        break;


                    case Q_UNTIL:
                        assert(formula->args.size() == 2);
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
                        assert(false);
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

>>>>>>> safe_for_benchmark
