//
// Created by giacomo on 20/01/2022.
//

#include <magic_enum.hpp>
#include <knobab/predicates/SimpleDataPredicate.h>
#include <knobab/predicates/PredicateManager.h>
#include "knobab/algorithms/MAXSatPipeline.h"
#include "../../../ltlf_query.h"


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
}

void MAXSatPipeline::data_chunk(CNFDeclareDataAware *model,
                                const AtomizingPipeline& atomization) {
    std::unordered_set<std::string> toUseAtoms; // This is to ensure the insertion of unique elements to the map!

    if (!model) return;
    declare_model = model;
    //label_set_t visitedAtoms;
    for (auto& it : declare_model->singleElementOfConjunction) {
        for (auto& item : it.elementsInDisjunction) {
            auto& ref = declare_atomization[item];
            if (!ref.empty()) continue; // Ignoring already visited patterns

            ltlf_query *formula;
            if (item.casusu == Init) {
                formula = ltlf_query_manager::init1(item.left_act, item.left_decomposed_atoms);
                formula->partial_results.emplace_back(data_accessing.size());
                data_accessing.emplace_back(DataQuery::InitQuery(item.left_act),
                                            std::vector<std::pair<std::pair<trace_t, event_t>, double>>{});
                if (item.left_decomposed_atoms.empty())
                    toUseAtoms.insert(item.left_act);
                else
                    toUseAtoms.insert(item.left_decomposed_atoms.begin(), item.left_decomposed_atoms.end());
            } else if (item.casusu == End) {
                formula = ltlf_query_manager::end1(item.left_act, item.left_decomposed_atoms);
                formula->partial_results.emplace_back(data_accessing.size());
                data_accessing.emplace_back(DataQuery::EndsQuery(item.left_act),
                                            std::vector<std::pair<std::pair<trace_t, event_t>, double>>{});
                if (item.left_decomposed_atoms.empty())
                    toUseAtoms.insert(item.left_act);
                else
                    toUseAtoms.insert(item.left_decomposed_atoms.begin(), item.left_decomposed_atoms.end());
            } else {
                auto& ltlf_sem = ltlf_semantics[item.casusu];
                std::vector<size_t> requirements;
                std::unordered_map<std::string, std::unordered_set<bool>> collection;
                ltlf_sem.collectElements(collection);

                if (item.left_decomposed_atoms.contains(item.left_act)) {
                    requirements.emplace_back(data_accessing.size());
                    data_accessing.emplace_back(DataQuery::ExistsQuery(item.left_act),
                                                std::vector<std::pair<std::pair<trace_t, event_t>, double>>{});
                }
                localExtract(atomization, toUseAtoms, ref, collection, item.left_decomposed_atoms, LEFT_ATOM);

                if (item.right_decomposed_atoms.contains(item.right_act)) {
                    requirements.emplace_back(data_accessing.size());
                    data_accessing.emplace_back(DataQuery::ExistsQuery(item.right_act),
                                                std::vector<std::pair<std::pair<trace_t, event_t>, double>>{});
                }
                localExtract(atomization, toUseAtoms, ref, collection, item.right_decomposed_atoms, RIGHT_ATOM);

                // Reconstructing the join
                auto tmp = ltlf_sem.replace_with(ref);
                tmp.instantiateJoinCondition(item.conjunctive_map);

                // Creating the formula
                formula = ltlf_query_manager::simplify(tmp);
                std::swap(formula->partial_results, requirements);
                fomulaidToFormula.emplace_back(formula);
            }

            maxFormulaId++;
            human_readable_ltlf_printing(std::cout, formula) << std::endl;
        }
    }

    assert(!toUseAtoms.empty());
    for (const auto& atom : toUseAtoms) { // So not to do the data visiting twice!
        //if (visitedAtoms.insert(atom).second) { // Only considering the firstly met atoms!
            if (atomization.act_atoms.contains(atom)) {
                assert(false); // Should have been already inserted in (A)!
            } else {
                auto it = atomization.atom_to_conjunctedPredicates.find(atom);
                assert(it != atomization.atom_to_conjunctedPredicates.end());

                std::vector<size_t> ranges;
                for (const auto& clause : it->second) {
                    // Sanity Checks
                    assert(clause.casusu == INTERVAL);
                    assert(clause.BiVariableConditions.empty());
                    /*for (const auto& localAtomOfReference : atom_to_associated_labels[atom]) {
                        assert(!clause.label.empty());
                        assert(localAtomOfReference == clause.label);
                    }*/
                    ranges.emplace_back(data_accessing.size());
                    data_accessing.emplace_back(DataQuery::RangeQuery(clause.label, clause.var, clause.value, clause.value_upper_bound),
                                                std::vector<std::pair<std::pair<trace_t, event_t>, double>>{});

                }
                for (size_t formulaId : atomToFormulaId.at(atom)) {
                    fomulaidToFormula.at(formulaId)->associateDataQueryIdsToFormulaByAtom(atom, ranges);
                }
            }
        //}
    }

}

void
MAXSatPipeline::localExtract(const AtomizingPipeline &atomization,
                             std::unordered_set<std::string> &toUseAtoms,
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
            toUseAtoms.insert(local_decomposition.begin(), local_decomposition.end());
        }
    }
}

void MAXSatPipeline::data_pipeline_first() {
    // TODO: future task, use a thread pool, in common to all the phases of the pipeline, to feed the tasks
    for (const auto& ref : data_accessing) {
        // TODO: SAM
        // Given the query in ref.first, put the result in ref.second
    }
}

