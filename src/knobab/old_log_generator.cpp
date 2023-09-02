//
// Created by giacomo on 09/03/2022.
//

#include <iostream>
#include "yaucl/strings/serializers.h"
#include "ServerQueryManager.h"
//#include "knobab/dataStructures/TraceData.h"
//#include "knobab/Environment.h"
#include <yaucl/graphs/algorithms/minimizeDFA.h>
#include <random>
#include <yaucl/graphs/flloat/ParseFFLOATDot.h>
#include <knobab/server/query_manager/Environment.h>
#include <knobab/server/declare/DeclareDataAware.h>

struct GenerativePath {
    size_t maxN;
    const std::set<std::vector<std::string>>& toAvoid;
    const std::unordered_set<std::string>& sigma;
    std::set<std::vector<std::string>> result;
    std::vector<std::string> V;

    GenerativePath(size_t maxN,
                   const std::set<std::vector<std::string>> &toAvoid,
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

void generateGraphFromPattern(const std::filesystem::path& cache_path,
                              const std::string& pattern_name,
                              FlexibleFA<size_t, std::string>& result,
                              const std::string& left = "a",
                              const std::string& right = "b",
                              const std::unordered_set<std::string>& sigma = {"a", "b", "c", "d", "e", "f", "g", "h", "i"}) {
    ParseFFLOATDot graph_loader;
    std::ifstream strm{cache_path / pattern_name};
    auto patternGraphToInstantiate = graph_loader.parse(strm);
    std::unordered_map<std::string, std::unordered_set<std::string>> bogus_act_to_set;
    bogus_act_to_set["a"] = {left};
    bogus_act_to_set["b"] = {right};
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
}

void generateGraphFromIntersection(const std::vector<DeclareDataAware>& model,
                                   const std::filesystem::path& cache_path,
                                   const std::filesystem::path& log_path,
                                   size_t min_len,
                                   size_t max_len,
                                   const std::unordered_set<std::string>& sigma = {"a", "b", "c", "d", "e", "f", "g", "h", "i"},
                                   bool doSample = false,
                                   double sampleRate = 0.5,
                                   size_t max_per_limit = std::numeric_limits<size_t>::max()
) {
    std::vector<FlexibleFA<std::string, size_t>> result(model.size());
    size_t counting = 0;
    for (const auto& clause : model) {
        FlexibleFA<size_t, std::string> curr;
        generateGraphFromPattern(cache_path, clause.casusu,  curr, clause.left_act, clause.right_act, sigma);
        result[counting++] = curr.shiftLabelsToNodes();
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
        g.dot(std::cout); std::cout << std::endl;
        std::ofstream tab{log_path };
        g.generative(tab, min_len, max_len, max_per_limit, doSample, sampleRate);
    }
}

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
//
//void generateMultiplePatternLog(const std::filesystem::path& cache_path,
//                                const std::filesystem::path& log_path,
//                                const std::string& pattern_name,
//                                size_t limit_size = 5,
//                                const std::unordered_set<std::string>& sigma = {"a", "b", "c", "d", "e", "f", "g", "h", "i"}) {
//    std::map<std::vector<std::string>, std::vector<std::string>> trace_to_patterns;
//    std::set<std::vector<std::string>> unionE;
//    for (const auto& left : sigma) {
//        for (const auto& right : sigma) {
//            if (left != right) {
//                auto local =  left + ","+ right;
//                std::cout << "Generating pos/neg log for: " << pattern_name <<"(" << local << ")"<<  std::endl;
//                ParseFFLOATDot graph_loader;
//                std::ifstream strm{cache_path / pattern_name};
//                auto patternGraphToInstantiate = graph_loader.parse(strm);
//
//                std::unordered_map<std::string, semantic_atom_set> bogus_act_to_set;
//                bogus_act_to_set["a"] = {left};
//                bogus_act_to_set["b"] = {right};
//
//                {
//                    FlexibleFA<size_t, std::string> result;
//                    std::unordered_map<size_t, size_t> idConv;
//                    for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId < N; nodeId++) {
//                        size_t src = result.addNewNodeWithLabel(nodeId);
//                        idConv[nodeId] = src;
//                    }
//                    for (size_t finalNodes: patternGraphToInstantiate.fini()) {
//                        result.addToFinalNodesFromId(idConv.at(finalNodes));
//                    }
//                    for (size_t initialNodes: patternGraphToInstantiate.init()) {
//                        result.addToInitialNodesFromId(idConv.at(initialNodes));
//                    }
//                    for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId < N; nodeId++) {
//                        size_t src = idConv.at(nodeId);
//                        for (const auto &edge: patternGraphToInstantiate.outgoingEdges(nodeId)) {
//                            size_t dst = idConv.at(edge.second);
//                            for (const std::string &act: evaluate_easy_prop_to_atoms(edge.first, bogus_act_to_set,
//                                                                                     sigma)) {
//                                result.addNewEdgeFromId(src, dst, act);
//                            }
//                        }
//                    }
//                    auto g = result.makeDFAAsInTheory(sigma);
//                    auto DFA = minimizeDFA(g);
//                    DFA.removeStatesNotLeadingToAcceptance();
//
//                    auto l = DFA.generative(limit_size);
//                    for (const auto& x : l ){
//                        trace_to_patterns[x].emplace_back(local);
//                    }
//                    unionE.insert(l.begin(), l.end());
//                }
//            }
//        }
//    }
//
//
//    GenerativePath gen{limit_size, unionE, sigma};
//    gen.run();
//    {
//        std::cout << "Writing ensemble testing file [clause all events]" << std::endl;
//        std::ofstream tab{log_path / (pattern_name + "_all_events.tab")};
//        std::ofstream clazz{log_path / (pattern_name +"_all_events_classes.tab")};
//        for (const auto& ref : trace_to_patterns) {
//            {
//                auto& trace = ref.first;
//                for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
//                    tab << trace.at(j);
//                    if (j != N) tab  << "\t";
//                }
//                tab << std::endl;
//            }
//            {
//                auto& clazzez = ref.second;
//                for (size_t j = 0, N = clazzez.size()-1; j<=N; j++) {
//                    clazz << clazzez.at(j);
//                    if (j != N) clazz  << "\t";
//                }
//                clazz << std::endl;
//            }
//        }
//        for (const auto& ref : gen.result) {
//            {
//                auto& trace = ref;
//                for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
//                    tab << trace.at(j);
//                    if (j != N) tab  << "\t";
//                }
//                tab << std::endl;
//            }
//            clazz << std::endl;
//        }
//    }
//}
const std::string query_plan_novel = "queryplan \"edbt24\" {\n"
                                     "     template \"Init\"                   := INIT  activation\n"
                                     "     template \"End\"                    := END activation\n"
                                     "     template \"Exists\"                := (EXISTS $ activation)\n"
                                     "     template \"Exists1\"                := (EXISTS 1 activation)\n"
                                     "     template \"Absence\"               := ABSENCE $ activation\n"
                                     "     template \"Absence1\"               := ABSENCE 1 activation\n"
                                     "     template \"Absence2\"               := ABSENCE 2 activation\n"
                                     "     template \"Precedence\" args 2      := ((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)\n"
                                     "     template \"ChainPrecedence\" args 2 := G(((LAST OR t (NEXT EXISTS ~ 1 t #1))) OR t ((EXISTS 1 t #1 activation) XB THETA INV #2 target ))\n"
                                     "     template \"Choice\" args 2          := (EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)\n"
                                     "     template \"Response\" args 2        := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )\n"
                                     "     template \"ChainResponse\" args 2   := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) BX THETA  #2 target))\n"
                                     "     template \"RespExistence\" args 2   := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target)))\n"
                                     "     template \"ExclChoice\" args 2       := ((EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)) AND ((ABSENCE 1 #1) OR (ABSENCE 1 #2))\n"
                                     "     template \"CoExistence\" args 2     := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target))) AND ( ((ABSENCE 1 #2)) OR ((EXISTS 1 #2 activation) AND THETA INV (EXISTS 1 #1 target)))\n"
                                     "     template \"NotCoExistence\" args 2  := ~ ((EXISTS 1 t #1 activation) AND THETA (EXISTS 1 t #2 target)) PRESERVE\n"
                                     "\n"
                                     "     template \"Succession\" args 2      := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )) AND (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 target)) OR (ABSENCE 1 #2))\n"
                                     "     template \"NegSuccession\" args 2   := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Gt  (EXISTS ~ 1 t #2)) ))\n"
                                     "     template \"ChainSuccession\" args 2 := G( (((LAST OR t (NEXT EXISTS ~ 1 t #2))) OR t ((NEXT EXISTS 1 t #2 activation) AND t THETA INV (EXISTS 1 t #1 target))) AND t\n"
                                     "                                             ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                                     "                                           )\n"
                                     "     template \"AltResponse\" args 2     := G ( (EXISTS ~ 1 t #1) OR t ((EXISTS 1 t #1 activation) &Ft!X THETA (((EXISTS 1 t #2 target)))))\n"
                                     "     template \"AltPrecedence\" args 2   := (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)) AND\n"
                                     "                                           (G(((EXISTS ~ 1 t #2)) OR t ((EXISTS 1 t #2 target) &Ft!WX THETA INV (((EXISTS 1 t #1 activation))))))\n"
                                     "}";
int main() {
    size_t pos, neg;
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
    std::ofstream f{"/home/giacomo/b.txt", std::ios_base::app};

    auto root_folder =  std::filesystem::current_path();
    std::string base{"ChainPrecedence"};
    std::string operators{"Hybrid"};
    std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;
    auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");

    ServerQueryManager sqm;
    std::stringstream ss;
    std::ifstream fs{sizes};
    fs >> pos >> neg;
    auto declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();
    std::ifstream t(declare_file_path);
    std::stringstream buffer;
    buffer << t.rdbuf();

    ss << "load "
       << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)
    << " "
            << std::quoted(curr.string())
            <<  " no stats as "
            << std::quoted(base);
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan_novel);
    ss << "model-check declare " << buffer.str() << std::endl;
    ss << " using \"TraceMaximumSatisfiability\" over " << std::quoted(base) << std::endl;
    ss << " plan \"edbt24\" "  << std::endl;
    ss << " with operators  " << std::quoted(operators);
    std::string a,b;
    std::tie(a,b) = sqm.runQuery(ss.str());
    auto js = nlohmann::json::parse(a);
    std::vector<double> max_sat_per_trace = js["TraceMaximumSatisfiability"].get<std::vector<double>>();
    f << base<< " "<<operators <<" : " << js["model_ltlf_query_time"].get<double>() << std::endl;
    for (size_t i = 0; i<max_sat_per_trace.size(); i++) {
//        std::cout << i << " for "<< max_sat_per_trace.size() << std::endl;
        if (i<pos)
            DEBUG_ASSERT(max_sat_per_trace.at(i)== 1.0);
        else
            DEBUG_ASSERT(max_sat_per_trace.at(i)== 0.0);
    }
}

int main2() {
    std::vector<DeclareDataAware> model;
//    model.emplace_back(DeclareDataAware::binary("ChainPrecedence", "b", "c"));
//    model.emplace_back(DeclareDataAware::binary("ChainResponse", "d", "e"));
//    model.emplace_back(DeclareDataAware::binary("Choice", "f", "g"));
//    model.emplace_back(DeclareDataAware::binary("ExlChoice", "h", "a"));
//    model.emplace_back(DeclareDataAware::unary("Existence_1", "b", 1));
//    model.emplace_back(DeclareDataAware::unary("Init_1", "e", 1));
//    model.emplace_back(DeclareDataAware::binary("Precedence", "c", "b"));
//    model.emplace_back(DeclareDataAware::binary("RespExistence", "h", "i"));
//    model.emplace_back(DeclareDataAware::binary("RespExistence", "i", "h"));
//    model.emplace_back(DeclareDataAware::binary("RespExistence", "e", "f"));
//    model.emplace_back(DeclareDataAware::binary("Response", "e", "f"));

//    generateGraphFromIntersection(model,
//                                  "/home/giacomo/projects/knobab/data/cache/graph_pattern_cache",
//                                  "/home/giacomo/logs/30_30_1000.tab",
//                                  30,
//                                  30,
//                                  {"a","b","c","d","e","f","g","h","i"},
//                                  true,
//                                  0.23,
//                                  1000);

    std::map<std::vector<std::string>, std::vector<std::string>> map;
    std::filesystem::path data = "data";
    std::filesystem::path cache_path = data / "cache" / "graph_pattern_cache";
//    std::filesystem::path log_path = data / "testing" / "ltlf";
    std::filesystem::path dec_path = data / "testing" / "declare";
//
//    generatePatternLog(map, cache_path, log_path, "Until", 5);
//    generatePatternLog(map, cache_path, log_path, "WeakUntil", 5);
//    generatePatternLog(map, cache_path, log_path, "Release", 5);
//    generatePatternLog(map, cache_path, log_path, "Box", 5);
//    generatePatternLog(map, cache_path, log_path, "Diamond", 5);
//    map.clear(); // Ignoring pure ltlf formulae from the tests
//
    generatePatternLog(map, cache_path, dec_path, "AltPrecedence", 5);
//    generatePatternLog(map, cache_path, dec_path, "AltResponse", 5);
//    generatePatternLog(map, cache_path, dec_path, "AltSuccession", 5);
//    generatePatternLog(map, cache_path, dec_path, "ChainPrecedence", 5);
//    generatePatternLog(map, cache_path, dec_path, "ChainResponse", 5);
//    generatePatternLog(map, cache_path, dec_path, "ChainSuccession", 5);
//    generatePatternLog(map, cache_path, dec_path, "Choice", 5);
//    generatePatternLog(map, cache_path, dec_path, "CoExistence", 5);
//    generatePatternLog(map, cache_path, dec_path, "ExlChoice", 5);
//    generatePatternLog(map, cache_path, dec_path, "NegChainSuccession", 5);
//    generatePatternLog(map, cache_path, dec_path, "NegSuccession", 5);
//    generatePatternLog(map, cache_path, dec_path, "NotCoExistence", 5);
//    generatePatternLog(map, cache_path, dec_path, "Precedence", 5);
//    generatePatternLog(map, cache_path, dec_path, "RespExistence", 5);
//    generatePatternLog(map, cache_path, dec_path, "Response", 5);
//    generatePatternLog(map, cache_path, dec_path, "Succession", 5);
//
//    ////////////////////////////////////////////////////////////////
//    {
//        std::cout << "Writing ensemble testing file [all clauses traces]" << std::endl;
//        std::ofstream tab{dec_path / "all_clauses_traces.tab"};
//        std::ofstream clazz{dec_path / "all_clauses_traces_class.tab"};
//        for (const auto& ref : map) {
//            {
//                auto& trace = ref.first;
//                for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
//                    tab << trace.at(j);
//                    if (j != N) tab  << "\t";
//                }
//                tab << std::endl;
//            }
//            {
//                auto& clazzez = ref.second;
//                for (size_t j = 0, N = clazzez.size()-1; j<=N; j++) {
//                    clazz << clazzez.at(j);
//                    if (j != N) clazz  << "\t";
//                }
//                clazz << std::endl;
//            }
//
//        }
//        std::cout << std::endl << std::endl;
//    }
//
//    ////////////////////////////////////////////////////////////////
//    generateMultiplePatternLog( cache_path, dec_path, "Response", 5);

    return 0;
}