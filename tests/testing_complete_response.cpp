//
// Created by giacomo on 23/03/2022.
//
#define CTEST_COLOR_OK
#define CTEST_SEGFAULT
#include <vector>
#include <cassert>
#include <yaucl/structures/set_operations.h>
#include <knobab/Environment.h>
#include <fstream>

#include <gtest/gtest.h>
#include <knobab/operators/simple_ltlf_operators.h>
#include <knobab/operators/fast_ltlf_operators.h>

class CompleteResponse : public testing::Test {
protected:
    void SetUp() override {
        env.doStats = false;
        env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
        env.set_atomization_parameters("p", 10);
        auto file = std::filesystem::current_path().parent_path().parent_path() / "data" / "testing" / "declare" / "Response_all_events.tab";
        {
            std::ifstream if_{file};
            env.load_log(TAB_SEPARATED_EVENTS, false, file.string(), false, if_);
        }
        auto scripts = std::filesystem::current_path().parent_path().parent_path();
        auto classes = scripts / "data" / "testing" / "declare" / "Response_all_events_classes.tab";
        {
            std::string line;
            std::ifstream stream{classes};
            while (std::getline(stream, line)) {
                auto& set = traceId_toClasses.emplace_back();
                std::istringstream iss(line);
                std::string token;
                while(std::getline(iss, token, '\t')) {
                    auto it = token.find(',');
                    assert(it != std::string::npos);
                    set.emplace(token.substr(0, it), token.substr(it+1));
                }
                allOfPossibleSets.emplace(set);
            }
        }

//        for (size_t i = 0, N = traceId_toClasses.size(); i<N; i++) {
//            const auto& ref = traceId_toClasses.at(i);
//            std::cout << "#" << i << ": " << ref.size() << std::endl;
//        }
//        for (const auto& ref : allOfPossibleSets)
//            std::cout << ref << std::endl;
        std::cout << allOfPossibleSets.size() << std::endl;
        std::string declare_file_path, maxsat;
        maxsat = (scripts / "scripts" / ("maxsat_pipeline_singledecl_testing.yaml")).string();
        std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
        env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
        env.set_atomization_parameters("p", 10);
        env.set_maxsat_parameters(std::filesystem::path(maxsat));
    }

    std::vector<std::set<std::pair<std::string,std::string>>> traceId_toClasses;
    std::set<std::set<std::pair<std::string,std::string>>> allOfPossibleSets;
    Environment env;
};

//TEST_F(CompleteResponse, InSetting) {
//    // I cannot possibly enumerate all of the possible sets composed of all of the possible pairs, which are 72.
//    // As this would be 2^|72|, generating this is too costly and exeeds the memory limits
//    std::cout << "Loading ok" << std::endl;
////    std::set<std::set<std::pair<std::string,std::string>>> allOfPossibleSets{{{"a", "b"}},
////                                                                             {{"a", "b"}, {"c", "d"}},
////                                                                             {{"a", "b"}, {"c", "d"}, {"e","f"}},
////                                                                             {{"a", "b"}, {"c", "d"}, {"e","f"}, {"g", "h"}},
////                                                                             {{"a", "b"}, {"c", "d"}, {"e","f"}, {"g", "h"}, {"i","a"}},
////                                                                             {{"a", "b"}, {"c", "d"}, {"e","f"}, {"g", "h"}, {"i","a"}, {"b", "c"}},
////                                                                             {{"a", "b"}, {"c", "d"}, {"e","f"}, {"g", "h"}, {"i","a"}, {"b", "c"}, {"d", "e"}},
////                                                                             {{"a", "b"}, {"c", "d"}, {"e","f"}, {"g", "h"}, {"i","a"}, {"b", "c"}, {"d", "e"}, {"f", "g"}},
////                                                                             {{"a", "b"}, {"c", "d"}, {"e","f"}, {"g", "h"}, {"i","a"}, {"b", "c"}, {"d", "e"}, {"f", "g"}, {"h", "i"}}};
//    for (const auto& set : allOfPossibleSets) {
//        // Clearing the previous model specification
//        env.clearModel();
//
//        std::vector<DeclareDataAware> model;
//        for (const auto ref: set) {
//            model.emplace_back(DeclareDataAware::binary_for_testing("Response", ref.first, ref.second));
//        }
//        env.load_model(model.begin(), model.end());
//
//        // Dummy model computation
//        env.doGrounding();
//        env.init_atomize_tables();
//        env.first_atomize_model();
//        auto ref = env.query_model();
//        std::cout << ref.ltlf_query_time << "ms for ";
//        std::cout << set.size() << "... done!" <<  std::endl <<  std::endl;
//    }
//}