//
// Created by giacomo on 16/02/2022.
//

#ifndef KNOBAB_LTLFQUERYMANAGER_H
#define KNOBAB_LTLFQUERYMANAGER_H

#include <yaucl/numeric/ssize_t.h>
#include <knobab/algorithms/atomization_pipeline.h>
#include <knobab/queries/LTLfQuery.h>
#include <unordered_map>

struct LTLfQueryManager {
    std::unordered_map<LTLfQuery, LTLfQuery*> conversion_map_for_subexpressions;
    std::map<size_t, std::vector<LTLfQuery*>> Q;
    std::unordered_map<LTLfQuery*, size_t> counter;
//    std::vector<LTLfQuery*> atomsToDecomposeInUnion;
    size_t current_query_id = 0;
    std::vector<std::unordered_set<LTLfQuery*>> activations;

    void generateGraph(std::map<LTLfQuery*, std::vector<LTLfQuery*>>& ref, LTLfQuery*q) const;
    std::string generateGraph() const;

    /**
     * Clears the manager, so to get ready with another query plan to be analysed
     */
    void clear();

    std::vector<std::set<std::string>> untimed_atomsets;
    std::vector<std::set<std::string>> timed_atomsets;

    void finalizeUnions() {
        remove_duplicates(untimed_atomsets);
        remove_duplicates(timed_atomsets);

        bool isTimed = true;
        auto result = partition_sets(timed_atomsets);
        size_t isFromFurtherDecomposition = result.minimal_common_subsets.size();

        for (const auto& min_set : result.minimal_common_subsets) {
            bool just = true;
            LTLfQuery element_disjunction;
            for ()
        }


//
//        for (const auto& ref : result.decomposedIndexedSubsets) {
////        auto& f = atomsToDecomposeInUnion.at(ref.first);
//        bool just = true;
//        LTLfQuery element_disjunction;
//        for (size_t i : *ref.second) {
//            if (i < isFromFurtherDecomposition) {
//                auto l = LTLfQuery::qEXISTS(1, DECLARE_TYPE_NONE, NoneLeaf, true, false);
//                l.fields.id.parts.is_atom = false;
//                l.atom.insert(result.minimal_common_subsets.at(i).begin(), result.minimal_common_subsets.at(i).end());
//                if (just) {
//                    element_disjunction = l;
//                    element_disjunction.fields.id.parts.is_timed = isTimed;
//                    element_disjunction.fields.id.parts.is_queryplan = true;
//                    just = false;
//                } else {
//                    element_disjunction = LTLfQuery::qOR(l, element_disjunction, true, false);
//                }
//            } else
//                for (size_t further : result.minimal_common_subsets_composition.at(i-isFromFurtherDecomposition)) {
//                    auto l = LTLfQuery::qEXISTS(1, DECLARE_TYPE_NONE, NoneLeaf, true, false);
//                    l.fields.id.parts.is_atom = false;
//                    l.atom.insert(result.minimal_common_subsets.at(further).begin(), result.minimal_common_subsets.at(further).end());
//                    if (just) {
//                        element_disjunction = l;
//                        element_disjunction.fields.id.parts.is_timed = isTimed;
//                        element_disjunction.fields.id.parts.is_queryplan = true;
//                        just = false;
//                    } else {
//                        element_disjunction = LTLfQuery::qOR(l, element_disjunction, true, false);
//                        element_disjunction.fields.id.parts.is_timed = isTimed;
//                        element_disjunction.fields.id.parts.is_queryplan = true;
//                    }
//                }
//
//        }
//        element_disjunction.fields.id.parts.is_timed = isTimed;
//        element_disjunction.fields.id.parts.is_queryplan = true;
//
////        LTLfQuery *q = simplifyRecursively(element_disjunction);
////        auto tmpValue = atomsToDecomposeInUnion[ref.first]->isLeaf;
////        q->isLeaf = tmpValue;
////
////        if (atomsToDecomposeInUnion[ref.first]->isDisjunctionOfExistentials()) {
////            // All of this code is correct, just because we always had this assumption to work with!
////            *atomsToDecomposeInUnion[ref.first] = *q;
////            //delete q; //this will not delete the other nodes, recursively. TODO: this should be done in clear() and avoid leaks
////            atomsToDecomposeInUnion[ref.first]->isLeaf = tmpValue;
////        } else {
////            LTLfQuery tmp = *atomsToDecomposeInUnion[ref.first];
////            tmp.args.emplace_back(q);
////            tmp.isLeaf = tmpValue;
////            LTLfQuery *q2 = simplifyRecursively(tmp);
////            q2->isLeaf = tmpValue;
////            *atomsToDecomposeInUnion[ref.first] = *q2;
////            //delete q; //this will not delete the other nodes, recursively. TODO: this should be done in clear() and avoid leaks
////            atomsToDecomposeInUnion[ref.first]->isLeaf = tmpValue;
////        }
//    }

    }

    LTLfQuery instantiate(const std::unordered_set<std::string>& atom,
                                          size_t formulaId,
                                          const LTLfQuery &input,
                                          const DeclareDataAware *joinCondition,
                                        const std::unordered_set<std::string> &data_atom,
                                          const std::unordered_set<std::string> &atom_universe,
                                          const std::unordered_set<std::string> &left,
                                          const std::unordered_set<std::string> &right) {
        LTLfQuery q;
        q.t = input.t;
        q.n = input.n;
        q.isLeaf = input.isLeaf;
        q.fields = input.fields;
        q.fields.id.parts.is_negated = false; // after resolution, nothing is negated!
        q.isLeaf = input.isLeaf;


        bool hasAtLeastOneDataAtom = false;
        if ((input.declare_arg == DECLARE_TYPE_LEFT)) {
            if (input.fields.id.parts.is_negated) {
                for (const auto& x : atom_universe) {
                    if (!left.contains(x)) {
                        hasAtLeastOneDataAtom = hasAtLeastOneDataAtom || data_atom.contains(x);
                        q.atom.insert(x);
//                        atomToFormulaId[x].emplace_back(formulaId);
                    }
                }
            } else {
                for (const auto& x : left) {
                    hasAtLeastOneDataAtom = hasAtLeastOneDataAtom || data_atom.contains(x);
                    q.atom.insert(x);
//                    atomToFormulaId[x].emplace_back(formulaId);
                }
            }
            if (input.fields.id.parts.is_timed) {
                timed_atomsets.emplace_back(q.atom);
            } else {
                untimed_atomsets.emplace_back(q.atom);
            }
        } else if (input.declare_arg == DECLARE_TYPE_RIGHT) {
            if (input.fields.id.parts.is_negated) {
                for (const auto& x : atom_universe) {
                    if (!right.contains(x)) {
                        hasAtLeastOneDataAtom = hasAtLeastOneDataAtom || data_atom.contains(x);
                        q.atom.insert(x);
//                        atomToFormulaId[x].emplace_back(formulaId);
                    }
                }
            } else {
                for (const auto& x : right) {
                    hasAtLeastOneDataAtom = hasAtLeastOneDataAtom || data_atom.contains(x);
                    q.atom.insert(x);
//                    atomToFormulaId[x].emplace_back(formulaId);
                }
            }
            if (input.fields.id.parts.is_timed) {
                timed_atomsets.emplace_back(q.atom);
            } else {
                untimed_atomsets.emplace_back(q.atom);
            }
        } else {
//            //To be done at a future step: supporting three argument clauses
//            if (q.fields.id.parts.is_atom) {
//                if(!((q.t == LTLfQuery::FIRST_QP) || (q.t == LTLfQuery::LAST_QP))) {
//                    if (input.fields.id.parts.is_negated) {
//                        for (const auto& x : atom_universe) {
//                            if (!left.contains(x)) {
//                                hasAtLeastOneDataAtom = hasAtLeastOneDataAtom || data_atom.contains(x);
//                                q.atom.insert(x);
////                                atomToFormulaId[x].emplace_back(formulaId);
//                            }
//                        }
//                    } else {
//                        for (const auto& x : left) {
//                            hasAtLeastOneDataAtom = hasAtLeastOneDataAtom || data_atom.contains(x);
//                            q.atom.insert(x);
////                            atomToFormulaId[x].emplace_back(formulaId);
//                        }
//                    }
//                }
//            }
        }
        if (input.fields.id.parts.has_theta) {
            q.joinCondition = joinCondition;
        }
        for (auto& args : input.args_from_script)
            q.args_from_script.emplace_back(instantiate(atom, formulaId, args, joinCondition, data_atom, atom_universe, left, right));
        q.fields.id.parts.is_queryplan = true;
        q.fields.id.parts.is_negated = false;
        q.declare_arg = input.declare_arg; //DECLARE_TYPE_NONE;
        if ((q.t == LTLfQuery::ABSENCE_QP) && hasAtLeastOneDataAtom) {
            LTLfQuery trueAbsence;
            trueAbsence.t = input.t;
            trueAbsence.n = input.n;
            trueAbsence.isLeaf = NotALeaf;
            trueAbsence.fields = input.fields;
            trueAbsence.fields.id.parts.is_atom = false;
            trueAbsence.fields.id.parts.is_negated = false; // after resolution, nothing is negated!
            trueAbsence.isLeaf = input.isLeaf;
            trueAbsence.args_from_script.emplace_back(q);
            return trueAbsence;
        } else
            return q;
//        auto tmp = simplify(q);
//        if (q.isLeaf == ActivationLeaf) {
//            if (current_query_id == activations.size()) { // Query Id counting from zero, so, if that happens, then it means that I need to add the activation in here!
//                activations.emplace_back().emplace(tmp);
//            } else {
//                activations.back().emplace(tmp);
//                //DEBUG_ASSERT( (activations.back() == tmp)); // By default, the activations should always refer to the same atom! That will make the assumption in the pipeline correct
//            }
//        }
//        return tmp;
    }

    LTLfQuery *simplify(const std::unordered_set<std::string>& atom,
                        size_t formulaId,
                        const LTLfQuery &input,
                        const DeclareDataAware *joinCondition,
                        const std::unordered_set<std::string> &atom_universe,
                        const std::unordered_set<std::string> &left,
                        const std::unordered_set<std::string> &right,
                        std::vector<std::string> &toUseAtoms,
                        std::unordered_map<std::string , std::vector<size_t>>& atomToFormulaId);

    void finalize_unions(const AtomizingPipeline& ap, std::vector<LTLfQuery*>& W, KnowledgeBase* ptr);

private:

    LTLfQuery* simplify(const LTLfQuery& q);
    LTLfQuery* simplifyRecursively(LTLfQuery &element_disjunction);
};


#endif //KNOBAB_LTLFQUERYMANAGER_H
