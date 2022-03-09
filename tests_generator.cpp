//
// Created by giacomo on 09/03/2022.
//

#include <iostream>
#include "yaucl/strings/serializers.h"
#include "knobab/dataStructures/TraceData.h"
#include "knobab/Environment.h"
#include <yaucl/graphs/algorithms/minimizeDFA.h>
#include <random>
#include <knobab/flloat_deps/ParseFFLOATDot.h>

struct GenerativePath {
    size_t maxN;
    const std::unordered_set<std::vector<std::string>>& toAvoid;
    const std::unordered_set<std::string>& sigma;
    std::unordered_set<std::vector<std::string>> result;
    std::vector<std::string> V;

    GenerativePath(size_t maxN, const std::unordered_set<std::vector<std::string>> &toAvoid,
                   const std::unordered_set<std::string> &sigma) : maxN(maxN), toAvoid(toAvoid), sigma(sigma), V(maxN, "") {}

    void run(size_t i=0) {
        if (maxN == 0) {
            result.emplace();
            return;
        }
        if ((i>=maxN)) {
            std::vector<std::string> W{V.begin(), V.begin()+i};
            if (!toAvoid.contains(W))
                result.insert(W);
        } else {
            for (const std::string& symbol : sigma) {
                V[i] = symbol;
                run(i+1);
            }
        }
    }

};

void generatePatternLog(const std::filesystem::path& cache_path,
                        const std::filesystem::path& log_path,
                        const std::string& pattern_name,
                        size_t limit_size = 5,
                        const std::unordered_set<std::string>& sigma = {"a", "b", "c", "d", "e", "f", "g", "h", "i"}) {

    std::cout << "Generating pos/neg log for: " << pattern_name << std::endl;
    ParseFFLOATDot graph_loader;
    std::ifstream strm{cache_path / pattern_name};
    auto patternGraphToInstantiate = graph_loader.parse(strm);

    std::unordered_map<std::string, semantic_atom_set> bogus_act_to_set;
    bogus_act_to_set["a"] = {"a"};
    bogus_act_to_set["b"] = {"b"};

    {
        FlexibleFA<size_t, std::string> result;
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
        auto g = result.makeDFAAsInTheory(sigma);
        auto DFA = minimizeDFA(g);
        DFA.removeStatesNotLeadingToAcceptance();


        std::ofstream tab{log_path / pattern_name};
        auto l = DFA.generative(limit_size);
        GenerativePath gen{limit_size, l, sigma};
        gen.run();

        {
            std::ofstream pos_neg{log_path / (pattern_name + "_pos_neg.txt")};
            pos_neg << l.size() << "\t" << gen.result.size() << std::endl;
        }

        for (const auto& trace : l) {
            for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
                tab << trace[j];
                if (j != N) tab  << "\t";
            }
            tab << std::endl;
        }
        for (const auto& trace : gen.result) {
            for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
                tab << trace[j];
                if (j != N) tab  << "\t";
            }
            tab << std::endl;
        }
    }
}

int main() {
    std::filesystem::path data = "data";
    std::filesystem::path cache_path = data / "cache" / "graph_pattern_cache";
    std::filesystem::path log_path = data / "testing" / "ltlf";

    generatePatternLog(cache_path, log_path, "Until", 5);
    generatePatternLog(cache_path, log_path, "WeakUntil", 5);
    generatePatternLog(cache_path, log_path, "Release", 5);
    generatePatternLog(cache_path, log_path, "Box", 5);
    generatePatternLog(cache_path, log_path, "Diamond", 5);

    return 0;
}