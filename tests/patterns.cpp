//
// Created by giacomo on 16/09/22.
//
#define CTEST_COLOR_OK
#define CTEST_SEGFAULT
#include <vector>
#include <knobab/Environment.h>
#include <fstream>
#include <gtest/gtest.h>

TEST(patterns, init) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "declare" / "choice_test";
    {
        std::ifstream if_{file};
        env.load_log(TAB_SEPARATED_EVENTS, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
//    for (const auto& result : env.db.pattern_mining(0.5, false, true)) {
//        std::cout << "Score: " << result.first << " Pattern: "<< result.second << std::endl;
//    }
//    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
//    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitDataless.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
//    env.load_model(declare_file_path);
//    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
//    env.set_atomization_parameters("p", 20);
//    env.set_maxsat_parameters(std::filesystem::path(maxsat));
//    env.doGrounding();
//    env.init_atomize_tables();
//    env.first_atomize_model();
//    auto ref = env.query_model();
//    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
//    EXPECT_EQ(ref.support_per_declare.size(), 1);
//    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0}));
}