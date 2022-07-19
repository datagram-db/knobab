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
    std::vector<LTLfQuery*> atomsToDecomposeInUnion;
    size_t current_query_id = 0;
    std::vector<std::unordered_set<LTLfQuery*>> activations;

    void generateGraph(std::map<LTLfQuery*, std::vector<LTLfQuery*>>& ref, LTLfQuery*q) const;
    std::string generateGraph() const;

    /**
     * Clears the manager, so to get ready with another query plan to be analysed
     */
    void clear();

    void instantiate(const std::unordered_set<std::string>& atom,
                                          size_t formulaId,
                                          const LTLfQuery &input,
                                          const DeclareDataAware *joinCondition,
                                          const std::unordered_set<std::string> &atom_universe,
                                          const std::unordered_set<std::string> &left,
                                          const std::unordered_set<std::string> &right,
                                          std::vector<std::string> &toUseAtoms,
                                          std::unordered_map<std::string , std::vector<size_t>>& atomToFormulaId,
                                          std::vector<LTLfQuery>& result_vector) {
        LTLfQuery& q = result_vector.emplace_back();
        q.t = input.t;
        q.n = input.n;
        q.isLeaf = input.isLeaf;
        q.fields = input.fields;
        q.fields.id.parts.is_negated = false; // after resolution, nothing is negated!
        q.isLeaf = input.isLeaf;

        if ((input.declare_arg == DECLARE_TYPE_LEFT)) {
            if (input.fields.id.parts.is_negated) {
                for (const auto& x : atom_universe) {
                    if (!left.contains(x)) {
                        q.atom.insert(x);
                        atomToFormulaId[x].emplace_back(formulaId);
                    }
                }
            } else {
                for (const auto& x : left) {
                    q.atom.insert(x);
                    atomToFormulaId[x].emplace_back(formulaId);
                }
            }
        } else if (input.declare_arg == DECLARE_TYPE_RIGHT) {
            if (input.fields.id.parts.is_negated) {
                for (const auto& x : atom_universe) {
                    if (!right.contains(x)) {
                        q.atom.insert(x);
                        atomToFormulaId[x].emplace_back(formulaId);
                    }
                }
            } else {
                for (const auto& x : right) {
                    q.atom.insert(x);
                    atomToFormulaId[x].emplace_back(formulaId);
                }
            }
        } else {
            //To be done at a future step: supporting three argument clauses
            if (q.fields.id.parts.is_atom) {
                if(!((q.t == LTLfQuery::FIRST_QP) || (q.t == LTLfQuery::LAST_QP))) {
                    if (input.fields.id.parts.is_negated) {
                        for (const auto& x : atom_universe) {
                            if (!left.contains(x)) {
                                q.atom.insert(x);
                                atomToFormulaId[x].emplace_back(formulaId);
                            }
                        }
                    } else {
                        for (const auto& x : left) {
                            q.atom.insert(x);
                            atomToFormulaId[x].emplace_back(formulaId);
                        }
                    }
                }
            }
        }
        if (input.fields.id.parts.has_theta) {
            q.joinCondition = joinCondition;
        }
        for (auto& args : input.args_from_script)
            q.args.emplace_back(simplify(atom, formulaId, args, joinCondition, atom_universe, left, right, toUseAtoms, atomToFormulaId));
        q.fields.id.parts.is_queryplan = true;
        q.fields.id.parts.is_negated = false;
        q.declare_arg = input.declare_arg; //DECLARE_TYPE_NONE;
//        return q;
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
