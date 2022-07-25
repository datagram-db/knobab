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
        auto file = scripts / "data" / "testing" / "complete_response" / "log_response.txt";
        {
            std::ifstream if_{file};
            env.load_log(HUMAN_READABLE_YAUCL, false, file.string(), false, if_);
        }
        std::filesystem::path declare_file_path, maxsat;
        maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
        declare_file_path = (scripts / "data" /"testing"/ "complete_response" /("response.powerdecl"));
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
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 9);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0,1.0,1.0/9.0,0.0,1.0,0.0,0.0,1.0/9.0,1.0/9.0}));
}