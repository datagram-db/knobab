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
        auto scripts = std::filesystem::current_path().parent_path().parent_path();
        auto file = scripts / "data" / "testing" / "logs" / "log_response.txt";
        {
            std::ifstream if_{file};
            env.load_log(HUMAN_READABLE_YAUCL, false, file.string(), false, if_);
        }
        std::filesystem::path declare_file_path, maxsat;
        maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
        declare_file_path = (scripts / "data" /"testing"/ "declare" / ("response.powerdecl"));
        std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
        env.load_model(declare_file_path);
        env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
        env.set_atomization_parameters("p", 10);
        env.set_maxsat_parameters(std::filesystem::path(maxsat));
        env.doGrounding();
        env.init_atomize_tables();
        env.first_atomize_model();
    }

    Environment env;
};

TEST_F(CompleteResponse, Support) {
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
//        //env.server(ref);
//    }
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 9);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0,1.0,1.0/9.0,0.0,1.0,0.0,0.0,1.0/9.0,1.0/9.0}));
}