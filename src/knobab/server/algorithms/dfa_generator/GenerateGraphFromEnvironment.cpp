//
// Created by giacomo on 23/01/23.
//

#include "knobab/server/algorithms/dfa_generator/GenerateGraphFromEnvironment.h"
#include "yaucl/graphs/algorithms/minimizeDFA.h"
#include <random>


//    std::set<std::vector<EdgeLabel>>


FlexibleFA<size_t, std::string> generateGraphFromEnvironment(Environment& env,
                                  const std::filesystem::path& cache_path) {
    auto sigma = env.getSigmaAll();
    std::vector<FlexibleFA<std::string, size_t>> result(env.grounding.singleElementOfConjunction.size());
    for (const auto& ref : env.grounding.singleElementOfConjunction) {
        std::vector<FlexibleFA<std::string, size_t>> localResult(ref.elementsInDisjunction.size());
        size_t local_counting = 0;
        for (const auto& clause : ref.elementsInDisjunction) {
            FlexibleFA<size_t, std::string> curr;
            generateGraphFromPattern(cache_path, clause,  sigma, curr);
            result[local_counting++] = curr.shiftLabelsToNodes();
        }
        if (!localResult.empty()) {
            if (localResult.size() == 1) {
                std::swap(result.emplace_back(), localResult[0]);
            } else {
                FlexibleFA<std::string, size_t> unionG;
                std::unordered_map<size_t, std::unordered_map<size_t, size_t>> mmap;
                for (size_t i = 0; i<localResult.size(); i++) {
                    auto& refForCurrentGraph = mmap[i];
                    const auto& currG = localResult.at(i);
                    for (size_t nid = 0; nid< currG.vSize(); nid++) {
                        refForCurrentGraph[nid] = unionG.addNewNodeWithLabel(currG.getNodeLabel(nid));
                    }
                    for (size_t eid : currG.getEdgeIds()) {
                        const auto& src_dst = currG.g.edge_ids.at(eid);
                        unionG.addNewEdgeFromId(refForCurrentGraph.at(src_dst.first),
                                                refForCurrentGraph.at(src_dst.second),
                                                currG.getEdgeLabel(eid));
                    }
                }
                minimizeDFA(unionG).ignoreNodeLabels(result.emplace_back());
                result.back().removeStatesNotLeadingToAcceptance();
            }
        } else {
            return {};
        }
    }
    for (size_t i = 1; i<result.size(); i++) {
        std::cout << "join" << i << std::endl;
        result[0] = FlexibleFA<std::string,size_t>::crossProductWithNodeLabels(result[0], result[i], true);
    }
    {
        auto g = result[0].shiftLabelsToEdges();
        result.clear();
        g.makeDFAAsInTheory(sigma);
        g.removeStatesNotLeadingToAcceptance();
        return g;
//        g.dot(std::cout); std::cout << std::endl;
//        std::vector<std::vector<std::string>> tab;
//        generative(g, tab, min_len, max_len, max_per_limit, doSample, sampleRate);
//        return tab;
    }
}