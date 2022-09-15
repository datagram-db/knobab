//
// Created by giacomo on 09/03/2022.
//

#include <iostream>
#include "yaucl/strings/serializers.h"
#include "knobab/dataStructures/TraceData.h"
#include "knobab/Environment.h"
#include "yaucl/graphs/algorithms/minimizeDFA.h"
#include <random>
#include "knobab/flloat_deps/ParseFFLOATDot.h"

struct GenerativePath {
    size_t maxN;
    const std::set<std::vector<std::string>>& toAvoid;
    const std::unordered_set<std::string>& sigma;
    std::set<std::vector<std::string>> result;
    std::vector<std::string> V;

    GenerativePath(size_t maxN, const std::set<std::vector<std::string>> &toAvoid,
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



void generatePatternLog(std::map<std::vector<std::string>, std::vector<std::string>>& trace_to_patterns,
                        const std::filesystem::path& cache_path,
                        const std::filesystem::path& log_path,
                        const std::string& pattern_name,
                        size_t limit_size = 5,
                        const std::unordered_set<std::string>& sigma = {"a", "b", "c", "d", "e", "f", "g", "h", "i"},
                        const std::string& left = "a",
                        const std::string& right = "b") {

    std::cout << "Generating pos/neg log for: " << pattern_name << std::endl;
    ParseFFLOATDot graph_loader;
    std::ifstream strm{cache_path / pattern_name};
    auto patternGraphToInstantiate = graph_loader.parse(strm);

    std::unordered_map<std::string, semantic_atom_set> bogus_act_to_set;
    bogus_act_to_set["a"] = {left};
    bogus_act_to_set["b"] = {right};

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
            trace_to_patterns[trace].emplace_back(pattern_name);
            for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
                tab << trace[j];
                if (j != N) tab  << "\t";
            }
            tab << std::endl;
        }
        for (const auto& trace : gen.result) {
            trace_to_patterns[trace];
            for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
                tab << trace[j];
                if (j != N) tab  << "\t";
            }
            tab << std::endl;
        }
    }
}

void generateMultiplePatternLog(const std::filesystem::path& cache_path,
                                const std::filesystem::path& log_path,
                                const std::string& pattern_name,
                                size_t limit_size = 5,
                                const std::unordered_set<std::string>& sigma = {"a", "b", "c", "d", "e", "f", "g", "h", "i"}) {
    std::map<std::vector<std::string>, std::vector<std::string>> trace_to_patterns;
    std::set<std::vector<std::string>> unionE;
    for (const auto& left : sigma) {
        for (const auto& right : sigma) {
            if (left != right) {
                auto local =  left + ","+ right;
                std::cout << "Generating pos/neg log for: " << pattern_name <<"(" << local << ")"<<  std::endl;
                ParseFFLOATDot graph_loader;
                std::ifstream strm{cache_path / pattern_name};
                auto patternGraphToInstantiate = graph_loader.parse(strm);

                std::unordered_map<std::string, semantic_atom_set> bogus_act_to_set;
                bogus_act_to_set["a"] = {left};
                bogus_act_to_set["b"] = {right};

                {
                    FlexibleFA<size_t, std::string> result;
                    std::unordered_map<size_t, size_t> idConv;
                    for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId < N; nodeId++) {
                        size_t src = result.addNewNodeWithLabel(nodeId);
                        idConv[nodeId] = src;
                    }
                    for (size_t finalNodes: patternGraphToInstantiate.fini()) {
                        result.addToFinalNodesFromId(idConv.at(finalNodes));
                    }
                    for (size_t initialNodes: patternGraphToInstantiate.init()) {
                        result.addToInitialNodesFromId(idConv.at(initialNodes));
                    }
                    for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId < N; nodeId++) {
                        size_t src = idConv.at(nodeId);
                        for (const auto &edge: patternGraphToInstantiate.outgoingEdges(nodeId)) {
                            size_t dst = idConv.at(edge.second);
                            for (const std::string &act: evaluate_easy_prop_to_atoms(edge.first, bogus_act_to_set,
                                                                                     sigma)) {
                                result.addNewEdgeFromId(src, dst, act);
                            }
                        }
                    }
                    auto g = result.makeDFAAsInTheory(sigma);
                    auto DFA = minimizeDFA(g);
                    DFA.removeStatesNotLeadingToAcceptance();

                    auto l = DFA.generative(limit_size);
                    for (const auto& x : l ){
                        trace_to_patterns[x].emplace_back(local);
                    }
                    unionE.insert(l.begin(), l.end());
                }
            }
        }
    }


    GenerativePath gen{limit_size, unionE, sigma};
    gen.run();
    {
        std::cout << "Writing ensemble testing file [clause all events]" << std::endl;
        std::ofstream tab{log_path / (pattern_name + "_all_events.tab")};
        std::ofstream clazz{log_path / (pattern_name +"_all_events_classes.tab")};
        for (const auto& ref : trace_to_patterns) {
            {
                auto& trace = ref.first;
                for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
                    tab << trace.at(j);
                    if (j != N) tab  << "\t";
                }
                tab << std::endl;
            }
            {
                auto& clazzez = ref.second;
                for (size_t j = 0, N = clazzez.size()-1; j<=N; j++) {
                    clazz << clazzez.at(j);
                    if (j != N) clazz  << "\t";
                }
                clazz << std::endl;
            }
        }
        for (const auto& ref : gen.result) {
            {
                auto& trace = ref;
                for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
                    tab << trace.at(j);
                    if (j != N) tab  << "\t";
                }
                tab << std::endl;
            }
            clazz << std::endl;
        }
    }
}

int main() {
    std::map<std::vector<std::string>, std::vector<std::string>> map;
    std::filesystem::path data = "data";
    std::filesystem::path cache_path = data / "cache" / "graph_pattern_cache";
    std::filesystem::path log_path = data / "testing" / "ltlf";
    std::filesystem::path dec_path = data / "testing" / "declare";

    generatePatternLog(map, cache_path, log_path, "Until", 5);
    generatePatternLog(map, cache_path, log_path, "WeakUntil", 5);
    generatePatternLog(map, cache_path, log_path, "Release", 5);
    generatePatternLog(map, cache_path, log_path, "Box", 5);
    generatePatternLog(map, cache_path, log_path, "Diamond", 5);
    map.clear(); // Ignoring pure ltlf formulae from the tests

    generatePatternLog(map, cache_path, dec_path, "AltPrecedence", 5);
    generatePatternLog(map, cache_path, dec_path, "AltResponse", 5);
    generatePatternLog(map, cache_path, dec_path, "AltSuccession", 5);
    generatePatternLog(map, cache_path, dec_path, "ChainPrecedence", 5);
    generatePatternLog(map, cache_path, dec_path, "ChainResponse", 5);
    generatePatternLog(map, cache_path, dec_path, "ChainSuccession", 5);
    generatePatternLog(map, cache_path, dec_path, "Choice", 5);
    generatePatternLog(map, cache_path, dec_path, "CoExistence", 5);
    generatePatternLog(map, cache_path, dec_path, "ExlChoice", 5);
    generatePatternLog(map, cache_path, dec_path, "NegChainSuccession", 5);
    generatePatternLog(map, cache_path, dec_path, "NegSuccession", 5);
    generatePatternLog(map, cache_path, dec_path, "NotCoExistence", 5);
    generatePatternLog(map, cache_path, dec_path, "Precedence", 5);
    generatePatternLog(map, cache_path, dec_path, "RespExistence", 5);
    generatePatternLog(map, cache_path, dec_path, "Response", 5);
    generatePatternLog(map, cache_path, dec_path, "Succession", 5);

    ////////////////////////////////////////////////////////////////
    {
        std::cout << "Writing ensemble testing file [all clauses traces]" << std::endl;
        std::ofstream tab{dec_path / "all_clauses_traces.tab"};
        std::ofstream clazz{dec_path / "all_clauses_traces_class.tab"};
        for (const auto& ref : map) {
            {
                auto& trace = ref.first;
                for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
                    tab << trace.at(j);
                    if (j != N) tab  << "\t";
                }
                tab << std::endl;
            }
            {
                auto& clazzez = ref.second;
                for (size_t j = 0, N = clazzez.size()-1; j<=N; j++) {
                    clazz << clazzez.at(j);
                    if (j != N) clazz  << "\t";
                }
                clazz << std::endl;
            }

        }
        std::cout << std::endl << std::endl;
    }

    ////////////////////////////////////////////////////////////////
    generateMultiplePatternLog( cache_path, dec_path, "Response", 5);

    return 0;
}