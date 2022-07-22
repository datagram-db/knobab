//
// Created by giacomo on 22/07/22.
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

// Testing dataless queries, untimed, just one
TEST(ideas22_uce, InitDataless) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitDataless.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 1);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0}));
}

// Testing dataless queries, untimed, two
TEST(ideas22_uce, InitDataless2) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitDataless2.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 2);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0, 0.0}));
}

// Testing data queries, untimed, just one
TEST(ideas22_uce, InitData) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitData.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 1);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{0.0}));
}

// Testing data queries, untimed, just one, with an inequality predicate
TEST(ideas22_uce, InitDataNeq) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitDataNeq.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 1);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0}));
}

// Testing data queries, untimed, two
TEST(ideas22_uce, InitData2) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitData2.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 2);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{0.0, 1.0}));
}


TEST(ideas22_uce, InitDataMix) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitDataMix.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 4);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{0.0, 1.0, 1.0, 0.0}));
}

TEST(ideas22_uce, InitEndsMix) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitEndsMix.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 8);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{0.0, 1.0, 1.0, 0.0, 2.0/3.0, 1.0/3.0, 1.0/3.0, 1.0}));
}

TEST(ideas22_uce, ExistsData) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("ExistsData.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 1);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0/3.0}));
}

TEST(ideas22_uce, ResponseData) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("ResponseData.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 2);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0/3.0,1.0/3.0}));
}

TEST(ideas22_uce, ResponseDataTheta) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("ResponseDataTheta.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 1);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{0.0}));
}

TEST(ideas22_uce, ResponseDataTheta2) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path().parent_path().parent_path();
    auto file = scripts / "data" / "testing" / "ideas22" / "anything.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    maxsat = (scripts / "scripts" / ("support_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("ResponseDataTheta2.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 2);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0/3.0,0.0}));
}