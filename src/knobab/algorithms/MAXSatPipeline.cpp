//
// Created by giacomo on 20/01/2022.
//

#include <magic_enum.hpp>
#include <knobab/predicates/SimpleDataPredicate.h>
#include <knobab/predicates/PredicateManager.h>
#include "knobab/algorithms/MAXSatPipeline.h"


std::string MAXSatPipeline::LEFT_ATOM{"a"};
std::string MAXSatPipeline::RIGHT_ATOM{"b"};

MAXSatPipeline::MAXSatPipeline() {
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
                formula = ltlf_query_manager::init1(item.left_act, item.left_decomposed_atoms);
                generateConcreteQuery(toUseAtoms, empty_result, item, formula, InitQuery);
            } else if (item.casusu == End) {
                formula = ltlf_query_manager::end1(item.left_act, item.left_decomposed_atoms);
                generateConcreteQuery(toUseAtoms, empty_result, item, formula, EndsQuery);
            } else if (item.casusu == Existence) {
                formula = ltlf_query_manager::exists1(item.left_act, item.left_decomposed_atoms);
                generateConcreteQuery(toUseAtoms, empty_result, item, formula, ExistsQuery);
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
                formula = ltlf_query_manager::simplify(tmp);
                //std::swap(formula->partial_results, requirements);
                fomulaidToFormula.emplace_back(formula);
            }

            maxFormulaId++;
            W.emplace_back(formula);
            human_readable_ltlf_printing(std::cout, formula) << std::endl; //todo: debugging
        }
    }

    for (auto& ref : atomToFormulaId)
        remove_duplicates(ref.second);

    ltlf_query_manager::finalize_unions();
    for (const auto& ref : W)
        human_readable_ltlf_printing(std::cout, ref) << std::endl; //todo: debugging

    remove_duplicates(toUseAtoms);

    assert(!toUseAtoms.empty());
    for (const auto& atom : toUseAtoms) { // So not to do the data visiting twice!
        if (atomization.act_atoms.contains(atom)) {
            auto q = DataQuery::AtomQuery(atom);
            size_t offset = data_accessing.size();
            auto find = data_offset.emplace(q, offset);
            if (find.second){
                offset = data_accessing.size();
                data_accessing.emplace_back(q, empty_result);
            } else
                offset = (find.first->second);
            atomToFormulaOffset.emplace_back(std::set<size_t>{offset});
        } else {
            auto it = atomization.atom_to_conjunctedPredicates.find(atom);
            assert(it != atomization.atom_to_conjunctedPredicates.end());

            std::set<size_t> ranges;
            for (const auto& clause : it->second) {
                // Sanity Checks
                assert(clause.casusu == INTERVAL);
                assert(clause.BiVariableConditions.empty());

                auto q = DataQuery::RangeQuery(clause.label, clause.var, clause.value, clause.value_upper_bound);
                auto find = data_offset.emplace(q, data_accessing.size());
                if (find.second){
                    ranges.insert(data_accessing.size());
                    data_accessing.emplace_back(q, empty_result);
                } else
                    ranges.insert(find.first->second);
            }
            atomToFormulaOffset.emplace_back(ranges);
        }
    }

}

void
MAXSatPipeline::generateConcreteQuery(std::vector<std::string> &toUseAtoms,
                                      std::vector<std::pair<std::pair<trace_t, event_t>, double>> &empty_result,
                                      DeclareDataAware &item, ltlf_query *formula, DataQueryType r) {
    auto q = DataQuery::AtomQueries(r, item.left_act);
    auto find = data_offset.emplace(q, data_accessing.size());
    if (find.second){
        formula->partial_results.emplace_back(data_accessing.size());
        data_accessing.emplace_back(q, empty_result);
    } else
        formula->partial_results.emplace_back(find.first->second);
    if (item.left_decomposed_atoms.empty())
        toUseAtoms.emplace_back(item.left_act);
    else
        toUseAtoms.insert(toUseAtoms.end(), item.left_decomposed_atoms.begin(), item.left_decomposed_atoms.end());
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

void MAXSatPipeline::data_pipeline_first() {

    // 1. Performing the query over each single predicate that we have extracted, so not to duplicate the data access
    for (const auto& ref : data_accessing) {
        // TODO: multithreaded
        // TODO: Given the query in ref.first, put the result in ref.second
    }

    auto result = partition_sets(atomToFormulaOffset);
    size_t isFromFurtherDecomposition = result.minimal_common_subsets.size();

    // TODO: resultOfS for collecting the intermediate computations
    for (const auto& S : result.minimal_common_subsets) {
        // TODO: multithreaded
        // TODO: perform the intersection among all of the elements in S, and put the result in resultOfS
    }

    // TODO: resultOfSSecond
    for (const auto& S : result.minimal_common_subsets_composition) {
        // TODO: multithreaded
        // TODO: perform the intersection among all of the elements in S, using the intermediate results from resultOfSSecond
    }

    results_cache.resize(toUseAtoms.size());
    for (const auto& ref : result.decomposedIndexedSubsets) {
        // TODO: multithreaded
        bool just = true;
        for (size_t i : *ref.second) {
            // TODO: perform the intersection
            if (i < isFromFurtherDecomposition) {
                // TODO: Obtain the (i)-th result from resultOfS
                if (just) {
                    just = false;
                }
            } else {
                // TODO: Obtain the (i-isFromFurtherDecomposition)-th result from resultOfSSecond
                if (just) {
                    just = false;
                }
            }
        }
        // TODO: put the global intersection into a map representation.
        // TODO: ref->first will correspond to the atom in that same position in toUseAtoms.

    }

    // TODO: clear resultOfS
    // TODO: clear resultOfSSecond

    // Preparing the second phase of the pipeline, where the extracted data is going to be combined.
    for (size_t i = 0, N = toUseAtoms.size(); i<N; i++) {
        auto& atom = toUseAtoms.at(i);
        for (size_t formulaId : atomToFormulaId.at(atom)) {
            fomulaidToFormula.at(formulaId)->associateDataQueryIdsToFormulaByAtom(atom, i);
        }
    }

}

