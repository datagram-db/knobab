//
// Created by giacomo on 22/07/22.
//
#define CTEST_COLOR_OK
#define CTEST_SEGFAULT
#include <vector>
#include <knobab/Environment.h>
#include <fstream>
#include <gtest/gtest.h>

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
TEST(ideas22_uce, InitData) { //AtomizeEverythingIfAnyDataPredicate
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

TEST(ideas22_uce, InitDataAE) { //
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20, AtomizeEverythingIfAnyDataPredicate);
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
    env.set_atomization_parameters("p", 20, AtomizeEverythingIfAnyDataPredicate);
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

TEST(ideas22_uce, InitDataMixAE) {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20, AtomizeEverythingIfAnyDataPredicate);
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
    env.set_atomization_parameters("p", 20, AtomizeEverythingIfAnyDataPredicate);
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

TEST(ideas22_uce, InitEndsMix2) {
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
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitEndsMix2.powerdecl"));
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
    EXPECT_EQ(ref.support_per_declare.size(), 11);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{0.0, 1.0, 1.0, 0.0, 2.0/3.0, 1.0/3.0, 1.0/3.0, 1.0,  1.0, 1.0/3.0, 1.0}));
}


TEST(ideas22_uce, InitEndsMix2AE) {
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
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("InitEndsMix2.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20, AtomizeEverythingIfAnyDataPredicate);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareSupport);
    EXPECT_EQ(ref.support_per_declare.size(), 11);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{0.0, 1.0, 1.0, 0.0, 2.0/3.0, 1.0/3.0, 1.0/3.0, 1.0,  1.0, 1.0/3.0, 1.0}));
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


TEST(ideas22_uce, ActualExampleSupp) {
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
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("ActualExample.powerdecl"));
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
    EXPECT_EQ(ref.support_per_declare.size(), 3);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{1.0/3.0,1.0/3.0,2.0/3.0}));
}

TEST(ideas22_uce, ActualExampleConf) {
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
    maxsat = (scripts / "scripts" / ("confidence_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("ActualExample.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, PerDeclareConfidence);
    EXPECT_EQ(ref.support_per_declare.size(), 3);
    EXPECT_EQ(ref.support_per_declare, (std::vector<double>{0.5, 0.5, 1.0}));
}

TEST(ideas22_uce, ActualExampleMAXSat) {
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
    maxsat = (scripts / "scripts" / ("maxsat_pipeline_singledecl_testing.yaml")).string();
    declare_file_path = (scripts / "data" /"testing"/ "ideas22" /  ("ActualExample.powerdecl"));
    std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
    env.load_model(declare_file_path);
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    env.set_maxsat_parameters(std::filesystem::path(maxsat));
    env.doGrounding();
    env.init_atomize_tables();
    env.first_atomize_model();
    auto ref = env.query_model();
    EXPECT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    EXPECT_EQ(ref.max_sat_per_trace.size(), 3);
    EXPECT_EQ(ref.max_sat_per_trace, (std::vector<double>{1.0,2.0/3.0,1.0/3.0}));
}