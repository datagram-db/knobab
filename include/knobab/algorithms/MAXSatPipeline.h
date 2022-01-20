//
// Created by giacomo on 20/01/2022.
//

#ifndef KNOBAB_MAXSATPIPELINE_H
#define KNOBAB_MAXSATPIPELINE_H

#include <knobab/KnowledgeBase.h>
#include <yaucl/bpm/structures/declare/CNFDeclareDataAware.h>
#include "atomization_pipeline.h"

struct MAXSatPipeline {
    // Input
    const KnowledgeBase& kb;
    CNFDeclareDataAware* declare_model = nullptr;
    const AtomizingPipeline& atomization;
    double c_the_distance_cost = 2.0;
    double min_approximation;

    struct DataQuery {
        std::string label;
        std::string var;
        union_minimal lower_bound, upper_bound;

        static DataQuery ExistsQuery(const std::string& atom) {
            DataQuery returnable;
            assert(!atom.empty());
            returnable.label = atom;
            return returnable;
        }
        static DataQuery RangeQuery(const std::string& atom, const std::string& var, const union_minimal& lb, const union_minimal& ub) {
            DataQuery returnable;
            assert(!atom.empty());
            returnable.label = atom;
            assert(!var.empty());
            returnable.var = var;
            returnable.lower_bound = lb;
            returnable.upper_bound = ub;
            return returnable;
        }

        bool isAtomQuery() const { return !var.empty(); }

        DEFAULT_CONSTRUCTORS(DataQuery);
    };

    std::vector<std::pair<DataQuery, std::vector<std::pair<std::pair<trace_t, event_t>, double>>>> V;

    void data_chunk(CNFDeclareDataAware* model) {
        if (!model) return;
        declare_model = model;
        label_set_t visitedAtoms;
        for (auto& it : declare_model->singleElementOfConjunction) {
            for (auto& item : it.elementsInDisjunction) {
                assert(!item.left_act.empty());
                assert(isUnaryPredicate(item.casusu) || (!item.right_act.empty()));

                bool isJoinQuery = false;
                auto& refMap = item.dnf_left_map;
                auto& decomposition = item.left_decomposed_atoms;
                std::string atomOfReference = item.left_act;
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
                    V.emplace_back(DataQuery::ExistsQuery(atomOfReference),
                                   std::vector<std::pair<std::pair<trace_t, event_t>, double>>{});
                }
                assert(!decomposition.empty());
                for (const auto& atom : decomposition) {
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
                                V.emplace_back(DataQuery::RangeQuery(atomOfReference, clause.var, clause.value, clause.value_upper_bound),
                                               std::vector<std::pair<std::pair<trace_t, event_t>, double>>{});
                            }
                        }
                    }
                }
            }
        }
    }

    void data_pipeline_first() {
        // TODO: SAM's task: fill in the results in V
    }


};


#endif //KNOBAB_MAXSATPIPELINE_H
