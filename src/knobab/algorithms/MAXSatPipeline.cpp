//
// Created by giacomo on 20/01/2022.
//

#include <magic_enum.hpp>
#include "knobab/algorithms/MAXSatPipeline.h"


std::string MAXSatPipeline::LEFT_ATOM{"a"};
std::string MAXSatPipeline::RIGHT_ATOM{"b"};

MAXSatPipeline::MAXSatPipeline() {
    for (declare_templates t : magic_enum::enum_values<declare_templates>()) {
        if (isUnaryPredicate(t)) continue; // discarding unary predicates
        ltlf_semantics[t] = DeclareDataAware::binary(t, LEFT_ATOM, RIGHT_ATOM)
                                 .toFiniteSemantics(false)
                                 .nnf(false);
    }
}

void MAXSatPipeline::clear() {
    data_accessing.clear();
    declare_atomization.clear();
}

void MAXSatPipeline::data_chunk(CNFDeclareDataAware *model,
                                const AtomizingPipeline& atomization) {
    if (!model) return;
    declare_model = model;
    label_set_t visitedAtoms;
    for (auto& it : declare_model->singleElementOfConjunction) {
        for (auto& item : it.elementsInDisjunction) {
            assert(!item.left_act.empty());
            assert(isUnaryPredicate(item.casusu) || (!item.right_act.empty()));

            auto& ref = declare_atomization[item];
            if (!ref.empty()) continue; // Ignoring already visited patterns

            if (item.casusu == Init) {

            }

            auto& ltlf_sem = ltlf_semantics[item.casusu];
            std::unordered_map<std::string, std::unordered_set<bool>> collection;
            ltlf_sem.collectElements(collection);
            std::unordered_set<std::string> toUseAtoms; // This is to ensure the insertion of unique elements to the map!

            bool isJoinQuery = false;
            auto& refMap = item.dnf_left_map;
            const auto& decomposition = item.left_decomposed_atoms;
            std::string atomOfReference = item.left_act;
            std::string collectionMapKey = LEFT_ATOM;
            std::unordered_map<std::string, std::unordered_set<std::string>> atom_to_associated_labels;

            // just a map Sanity Check for pipeline debugging purposes: testing the assumptions
            for (const auto& coll : item.dnf_left_map) {
                for (const auto& pred: coll) {
                    if (isJoinQuery) {
                        assert(pred.second.BiVariableConditions.empty());
                        assert(pred.second.casusu == TTRUE);
                    }
                }
            }

            // Also, the decomposition map should not be empty, as it must mean that there
            // at least one predicate, which is the atom!

            if (visitedAtoms.insert(atomOfReference).second) { //(A)
                data_accessing.emplace_back(DataQuery::ExistsQuery(atomOfReference),
                                            std::vector<std::pair<std::pair<trace_t, event_t>, double>>{});
            }
            assert(!decomposition.empty());
            auto it = collection.find(collectionMapKey);
            if (it != collection.end()) {
                for (bool isNegated : it->second) {
                    const auto& local_decomposition = ref.emplace(std::pair<bool, std::string>{isNegated, collectionMapKey},
                                                                  isNegated ?
                                                                  unordered_difference(atomization.atom_universe, decomposition)
                                                                            : decomposition).first->second;
                    toUseAtoms.insert(local_decomposition.begin(), local_decomposition.end());
                    for (const auto& refX : local_decomposition) {
                        atom_to_associated_labels[refX].insert(atomOfReference); // Todo: just as a map, if the assertion is never broken. Furthermore, just for sanity check
                        assert(atom_to_associated_labels[refX].size() == 1);
                    }
                }
            }


            assert(!toUseAtoms.empty());
            for (const auto& atom : toUseAtoms) {
                if (visitedAtoms.insert(atom).second) { // Only considering the firstly met atoms!
                    if (atomization.act_atoms.contains(atom)) {
                        assert(false); // Should have been already inserted in (A)!
                    } else {
                        auto it = atomization.atom_to_conjunctedPredicates.find(atom);
                        assert(it != atomization.atom_to_conjunctedPredicates.end());
                        for (const auto& clause : it->second) {
                            // Sanity Checks
                            assert(clause.casusu == INTERVAL);
                            assert(clause.BiVariableConditions.empty());
                            for (const auto& localAtomOfReference : atom_to_associated_labels[atom]) {
                                assert(!clause.label.empty());
                                assert(localAtomOfReference == clause.label);
                            }

                            data_accessing.emplace_back(DataQuery::RangeQuery(clause.label, clause.var, clause.value, clause.value_upper_bound),
                                                        std::vector<std::pair<std::pair<trace_t, event_t>, double>>{});
                        }
                    }
                }
            }
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

