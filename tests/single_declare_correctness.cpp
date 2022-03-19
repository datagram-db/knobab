//
// Created by giacomo on 27/02/2022.
//

#define CTEST_COLOR_OK
#define CTEST_SEGFAULT

#include <vector>
#include <yaucl/structures/set_operations.h>
#include <knobab/Environment.h>
#include <gtest/gtest.h>

#define TEST_MULTI(name) class name ## _tests : public testing::Test {\
protected:\
void SetUp() override {\
        env.clear();\
        env.doStats = false;\
        std::string declare_file_path, maxsat;\
        std::string base{#name}; \
        std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();\
        std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;\
        auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");\
        declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();\
        maxsat = (root_folder / "scripts" / ("maxsat_pipeline_singledecl_testing.yaml")).string();\
        std::ifstream file{curr};\
        env.load_log(TAB_SEPARATED_EVENTS, true, curr.filename(), false, file);\
        std::ifstream fs{sizes};\
        fs >> pos >> neg;\
        env.load_model(declare_file_path);\
        env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);\
        env.doGrounding();\
        env.set_atomization_parameters("p", 10);\
        env.init_atomize_tables();\
        env.first_atomize_model();\
        env.set_maxsat_parameters(std::filesystem::path(maxsat));\
}\
Environment env;\
size_t pos, neg;                                                \
};\

TEST_MULTI(Response)
TEST_F(Response_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(Choice)
TEST_F(Choice_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(ChainResponse)
TEST_F(ChainResponse_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(RespExistence)
TEST_F(RespExistence_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(ExlChoice)
TEST_F(ExlChoice_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(CoExistence)
TEST_F(CoExistence_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(NotCoExistence)
TEST_F(NotCoExistence_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(Precedence)
TEST_F(Precedence_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(Succession)
TEST_F(Succession_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

// NegSuccession

TEST_MULTI(NegSuccession)
TEST_F(NegSuccession_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(ChainPrecedence)
TEST_F(ChainPrecedence_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}


TEST_MULTI(ChainSuccession)
TEST_F(ChainSuccession_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}

TEST_MULTI(AltResponse)
TEST_F(AltResponse_tests, test_single_declare_clause) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}