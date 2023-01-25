//
// Created by giacomo on 23/01/23.
//


#include <yaucl/graphs/flloat/ParseFFLOATDot.h>
#include <knobab/server/algorithms/dfa_generator/GenerateGraphFromAtomisedClause.h>
#include <knobab/server/algorithms/atomization/AtomizingPipeline.h>
#include <yaucl/graphs/algorithms/minimizeDFA.h>

inline
semantic_atom_set evaluate_easy_prop_to_atoms(const easy_prop &prop,
                                              const std::unordered_map<std::string, semantic_atom_set> &bogus_act_to_set,
                                              const std::unordered_set<std::string>& sigma) {
    switch (prop.casusu) {
        case easy_prop::E_P_AND:
            DEBUG_ASSERT(prop.args.size() == 2);
            DEBUG_ASSERT(!prop.isAtomNegated);
            return unordered_intersection(evaluate_easy_prop_to_atoms( prop.args.at(0)/*, bogus_act_to_atom*/, bogus_act_to_set, sigma),
                                          evaluate_easy_prop_to_atoms( prop.args.at(1)/*, bogus_act_to_atom*/, bogus_act_to_set, sigma) );
        case easy_prop::E_P_OR: {
            DEBUG_ASSERT(prop.args.size() == 2);
            DEBUG_ASSERT(!prop.isAtomNegated);
            semantic_atom_set S = evaluate_easy_prop_to_atoms( prop.args.at(0)/*, bogus_act_to_atom*/, bogus_act_to_set, sigma);
            auto tmp = evaluate_easy_prop_to_atoms( prop.args.at(1)/*, bogus_act_to_atom*/, bogus_act_to_set, sigma);
            S.insert(tmp.begin(), tmp.end());
            return S;
        }
        case easy_prop::E_P_ATOM:
            DEBUG_ASSERT(prop.args.empty());
            DEBUG_ASSERT(bogus_act_to_set.contains(prop.single_atom_if_any));
            if (prop.isAtomNegated) {
                return unordered_difference(sigma, bogus_act_to_set.at(prop.single_atom_if_any));
            } else {
                return bogus_act_to_set.at(prop.single_atom_if_any);
            }
        case easy_prop::E_P_TRUE:
            return sigma;
        default: //case easy_prop::E_P_FALSE:
            return {};
    }
}

void generateGraphFromPattern(const std::filesystem::path& cache_path,
                              const DeclareDataAware& atomised_clause,
                              const std::unordered_set<std::string>& sigma,
                              FlexibleFA<size_t, std::string>& result) {
    ParseFFLOATDot graph_loader;
    std::string full_name = atomised_clause.casusu;
    if (atomised_clause.right_act.empty())
        full_name = full_name+"_"+std::to_string(atomised_clause.n);
    std::ifstream strm{cache_path / full_name};
    auto patternGraphToInstantiate = graph_loader.parse(strm);
    std::unordered_map<std::string, semantic_atom_set> bogus_act_to_set;
    bogus_act_to_set["a"] = atomised_clause.left_decomposed_atoms;
    if (!atomised_clause.right_act.empty())
        bogus_act_to_set["b"] = atomised_clause.right_decomposed_atoms;
    std::unordered_map<size_t, size_t> idConv;
    for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId<N; nodeId++) {
        size_t src = result.addNewNodeWithLabel(nodeId);
        idConv[nodeId] = src;
    }
    for (size_t finalNodes : patternGraphToInstantiate.fini()) {
        result.addToFinalNodesFromId(idConv.at(finalNodes));
    }
    for (size_t initialNodes : patternGraphToInstantiate.init()) {
        result.addToInitialNodesFromId(idConv.at(initialNodes));
    }
    for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId<N; nodeId++) {
        size_t src = idConv.at(nodeId);
        for (const auto&edge : patternGraphToInstantiate.outgoingEdges(nodeId)) {
            size_t dst = idConv.at(edge.second);
            for (const std::string& act : evaluate_easy_prop_to_atoms(edge.first, bogus_act_to_set, sigma)) {
                result.addNewEdgeFromId(src, dst, act);
            }
        }
    }
    result.makeDFAAsInTheory(sigma);
}