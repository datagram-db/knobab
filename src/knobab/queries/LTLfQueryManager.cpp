//
// Created by giacomo on 16/02/2022.
//

#include "knobab/queries/LTLfQueryManager.h"
#include <stack>

static inline void topological_sort(const std::vector<LTLfQuery*>& W,
                                    std::vector<LTLfQuery*>& vertexOreder) {
    std::unordered_set<LTLfQuery*> toVisit{W.begin(), W.end()};
    std::stack<std::pair<bool, LTLfQuery*>> internalStack;
    std::unordered_set<LTLfQuery*> visited;
    //auto isEnd = adjMap.end();
    while (!toVisit.empty()) {
        auto first = toVisit.begin();
        if (first != toVisit.cend()) {
            internalStack.push(std::make_pair(false, *first));
        }
        while (!internalStack.empty()) {
            std::pair<bool, LTLfQuery*>  cp = internalStack.top();
            internalStack.pop();
            toVisit.erase(cp.second);
            if (cp.first) {
                vertexOreder.emplace_back(cp.second);
            } else {
                if (visited.insert(cp.second).second) {
                    internalStack.push((cp));
                    internalStack.push(std::make_pair(true, cp.second));
                    //auto out = adjMap.find(cp.second);
                    for (auto & i : cp.second->args) {
                        internalStack.push(std::make_pair(false, i));
                    }
                }
            }
        }
    }
    std::reverse(vertexOreder.begin(), vertexOreder.end());
}

#include <magic_enum.hpp>
#include <yaucl/functional/assert.h>

void LTLfQueryManager::generateGraph(std::map<LTLfQuery*, std::vector<LTLfQuery*>>& ref, LTLfQuery*q) const {
    auto it = ref.emplace(q, q->args);
    if (it.second) {
        for (const auto& arg : q->args)
            generateGraph(ref, arg);
    }
}

#include <nlohmann/json.hpp>
std::string LTLfQueryManager::generateGraph() const {
    std::map<LTLfQuery*, std::vector<LTLfQuery*>> ref;
    std::map<LTLfQuery*,size_t> layerId;
    if (Q.empty()) return "{nodes: [], edges: []}";
    auto it = Q.begin();
    ref[nullptr] = it->second;
    layerId[nullptr] = 0;
    for (; it != Q.end(); it++) {
        for (const auto& arg : it->second) {
            layerId[arg] = 1+it->first;
            generateGraph(ref, arg);
        }
    }
    nlohmann::json json;
    json["nodes"] = {};
    json["edges"] = {};
    auto& nodes = json["nodes"];
    auto& edges = json["edges"];
    for (const auto& cp : ref) {
        nlohmann::json node;
        node["id"] = cp.first ? (size_t)cp.first : 0;
        node["group"] = layerId[cp.first];
        if ((cp.first) && (!cp.first->atom.empty())) {
            std::stringstream aa;
            aa << *cp.first;
            node["label"] = aa.str();
        } else {
            node["label"] = cp.first ? ((cp.first->fields.id.parts.is_timed ? "t" : "") + std::string(magic_enum::enum_name(cp.first->t))) : "Ensemble";
        }
        nodes.push_back(node);
        for (const auto& out : cp.second) {
            nlohmann::json edge;
            edge["from"] = cp.first ? (size_t)cp.first : 0;
            edge["to"] = (size_t)out;
            edges.push_back(edge);
        }
    }
    return json.dump(4);
}

void LTLfQueryManager::clear() {
    for (auto it = conversion_map_for_subexpressions.begin(); it != conversion_map_for_subexpressions.end(); it++) {
        delete it->second;
        it = conversion_map_for_subexpressions.erase(it);
    }
    //VSet.clear();
    atomsToDecomposeInUnion.clear();
    counter.clear();
}

void LTLfQueryManager::finalize_unions(const std::vector<LTLfQuery*>& W, KnowledgeBase* ptr) {
    std::vector<std::set<std::string>> unionToDecompose;
    for (const auto& ptr : atomsToDecomposeInUnion)
        unionToDecompose.emplace_back(ptr->atom);
    auto result = partition_sets(unionToDecompose);
    size_t isFromFurtherDecomposition = result.minimal_common_subsets.size();
    for (const auto& ref : result.decomposedIndexedSubsets) {
        auto& f = atomsToDecomposeInUnion.at(ref.first);
        bool just = true;
        LTLfQuery r;
        for (size_t i : *ref.second) {
            if (i < isFromFurtherDecomposition) {
                auto l = LTLfQuery::qEXISTS(1, DECLARE_TYPE_NONE, true, false);
                l.fields.id.parts.is_atom = false;
                l.atom.insert(result.minimal_common_subsets.at(i).begin(), result.minimal_common_subsets.at(i).end());
                if (just) {
                    r = l;
                    just = false;
                } else {
                    r = LTLfQuery::qOR(l, r, false, false);
                }
            } else
                for (size_t further : result.minimal_common_subsets_composition.at(i-isFromFurtherDecomposition)) {
                    auto l = LTLfQuery::qEXISTS(1, DECLARE_TYPE_NONE, true, false);
                    l.fields.id.parts.is_atom = false;
                    l.atom.insert(result.minimal_common_subsets.at(further).begin(), result.minimal_common_subsets.at(further).end());
                    if (just) {
                        r = l;
                        just = false;
                    } else {
                        r = LTLfQuery::qOR(l, r, false, false);
                    }
                }

        }
        LTLfQuery* q = simplify(r);
        auto tmpValue = atomsToDecomposeInUnion[ref.first]->declare_type;
        q->declare_type = tmpValue;
        *atomsToDecomposeInUnion[ref.first] = *q;
        //delete q; //this will not delete the other nodes, recursively. TODO: this should be done in clear() and avoid leaks
        atomsToDecomposeInUnion[ref.first]->declare_type = tmpValue;
    }

    // Making ready for the parallelization of the query execution by setting it into layers
    std::vector<LTLfQuery*> topological_order;
    topological_sort(W, topological_order);
    for (const auto& subFormula: topological_order) {
        for (const auto& adj : subFormula->args) {
            adj->dis = std::max(adj->dis, subFormula->dis +1);
            adj->parentMax = std::max(adj->parentMax, subFormula->dis);
            adj->parentMin = std::min(adj->parentMin, subFormula->dis);
        }
    }

    for (const auto& subFormula: topological_order) {
        Q[subFormula->dis].emplace_back(subFormula);
    }
}

#include <iostream>

LTLfQuery *LTLfQueryManager::simplify(const std::unordered_set<std::string>& atom,
                                      size_t formulaId,
                                      const LTLfQuery &input,
                                      const DeclareDataAware *joinCondition,
                                      const std::unordered_set<std::string> &atom_universe,
                                      const std::unordered_set<std::string> &left,
                                      const std::unordered_set<std::string> &right,
                                      std::vector<std::string> &toUseAtoms,
                                      std::unordered_map<std::string , std::vector<size_t>>& atomToFormulaId) {
    LTLfQuery q;
    q.t = input.t;
    q.n = input.n;
    q.declare_type = input.declare_type;
    q.fields = input.fields;
    q.fields.id.parts.is_negated = false; // after resolution, nothing is negated!


    if ((input.declare_type == DECLARE_TYPE_LEFT) || (input.declare_type == DECLARE_TYPE_NONE)) {
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
    } else if (input.declare_type == DECLARE_TYPE_RIGHT) {
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
        //DEBUG_ASSERT(!q.fields.id.parts.is_atom);
    }

    if (input.fields.id.parts.has_theta) {
        q.joinCondition = joinCondition;
    }
    for (auto& args : input.args_from_script)
        q.args.emplace_back(simplify(atom, formulaId, args, joinCondition, atom_universe, left, right, toUseAtoms, atomToFormulaId));
    q.fields.id.parts.is_queryplan = true;
    return simplify(q);
}

LTLfQuery *LTLfQueryManager::simplify(const LTLfQuery &q) {
    auto it = conversion_map_for_subexpressions.find(q);
    if (it != conversion_map_for_subexpressions.end()) {
        counter[it->second]++;
        return it->second;
    } else {
        auto* ptr = new LTLfQuery();
        *ptr = q;
        counter.emplace(ptr, 1);
        assert(conversion_map_for_subexpressions.emplace(q, ptr).second);
        return ptr;
    }
}