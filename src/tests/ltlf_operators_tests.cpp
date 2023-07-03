//
// Created by giacomo on 09/03/2022.
//


#include <vector>
#include <yaucl/structures/set_operations.h>
#include <knobab/server/query_manager/Environment.h>
#include <gtest/gtest.h>
#include <knobab/server/operators/simple_ltlf_operators.h>
#include <knobab/server/operators/fast_ltlf_operators.h>

constexpr char tabChar = '\t';

#define TEST(name) class name ## _tests : public testing::Test {\
protected:\
void SetUp() override {\
std::filesystem::path curr = std::filesystem::current_path();   \
std::string base{#name};                                        \
auto filename = curr.parent_path() / "data" /"testing"/"ltlf" / base;\
auto sizes = curr.parent_path() / "data" /"testing"/"ltlf" / (base+"_pos_neg.txt");\
ss.open(filename);\
std::ifstream fs{sizes};                                               \
char c;                                                         \
fs >> pos >> neg;                                          \
env.doStats = false;  env.index_missing_data = false;           \
env.load_log(TAB_SEPARATED_EVENTS, true, filename, false, ss);  \
ss.close(); ss.open(filename);                                  \
    std::string line;\
while (std::getline(ss, line)) {\
auto& v = traces.emplace_back();\
\
std::istringstream iss(line);\
std::string token;\
while(std::getline(iss, token, '\t')) v.emplace_back(token); \
}\
}\
\
Environment env;\
std::ifstream ss;                                               \
size_t pos, neg;                                                \
std::vector<std::vector<std::string>> traces;\
};\

TEST(Box)
TEST_F(Box_tests, untimed) {
    auto a = env.db.timed_dataless_exists("a", ActivationLeaf);
    Result result;
    global_logic_untimed(a, result, env.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(pos, result.size());
}

TEST_F(Box_tests, timed) {
    Environment env2;
    env2.doStats = false;
    auto a = env.db.timed_dataless_exists("a", ActivationLeaf);
    Result result, result2;
    global_logic_timed(a, result, env.db.act_table_by_act_id.trace_length);
    EXPECT_TRUE(!result.empty());
    size_t resultSize = result.size();
    env2.db.enterLog("box_test_timed", "box_test_timed");
    size_t trace_count = 0;
    size_t timestamp = 0;
    for (auto it = result.begin(); it != result.end(); it++) {
        auto &trace = traces.at(it->first.first);
        size_t traceId = env2.db.enterTrace(std::to_string(trace_count));
        for (auto beg = trace.begin() + it->first.second, end = trace.end(); beg != end; beg++) {
            env2.db.enterEvent(timestamp++, *beg);
        }
        env2.db.exitTrace(traceId);
        trace_count++;
    }
    result.clear();
    env2.db.exitLog("box_test_timed", "box_test_timed");
    env2.db.index_data_structures(false);
    assert(result.empty());
    a = env2.db.timed_dataless_exists("a", ActivationLeaf);
    global_logic_untimed(a, result, env2.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(trace_count, result.size());
    EXPECT_EQ(resultSize, result.size());

    global_fast_untimed(a, result2, env2.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(result, result2);
}

TEST(Diamond)
TEST_F(Diamond_tests, basic) {
    auto a = env.db.timed_dataless_exists("a", ActivationLeaf);

    Result result;
    future_logic_untimed(a, result, env.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(pos, result.size());
}
TEST_F(Diamond_tests, timed) {
    Environment env2;
    env2.doStats = false;
    auto a = env.db.timed_dataless_exists("a", ActivationLeaf);
    Result result;
    future_logic_timed(a, result, env.db.act_table_by_act_id.trace_length);
    EXPECT_TRUE(!result.empty());
    size_t resultSize = result.size();
    env2.db.enterLog("diamond_test_timed", "diamond_test_timed");
    size_t trace_count = 0;
    size_t timestamp = 0;
    for (auto it = result.begin(); it != result.end(); it++) {
        auto &trace = traces.at(it->first.first);
        size_t traceId = env2.db.enterTrace(std::to_string(trace_count));
        for (auto beg = trace.begin() + it->first.second, end = trace.end(); beg != end; beg++) {
            env2.db.enterEvent(timestamp++, *beg);
        }
        env2.db.exitTrace(traceId);
        trace_count++;
    }
    result.clear();
    env2.db.exitLog("diamond_test_timed", "diamond_test_timed");
    env2.db.index_data_structures(false);
    assert(result.empty());
    a = env2.db.timed_dataless_exists("a", ActivationLeaf);
    future_logic_untimed(a, result, env2.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(trace_count, result.size());
    EXPECT_EQ(resultSize, result.size());
}

TEST(Until)
TEST_F(Until_tests, basic) {
    auto a = env.db.timed_dataless_exists("a", ActivationLeaf);
    auto b = env.db.timed_dataless_exists("b", TargetLeaf);
    Result result, result2;
    until_logic_untimed(a, b, result, nullptr, env.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(pos, result.size());

    until_fast_untimed(a, b, result2, nullptr, env.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(result, result2);
}


TEST_F(Until_tests, timed) {
    Environment env2;
    env2.doStats = false;
    auto a = env.db.timed_dataless_exists("a", ActivationLeaf);
    auto b = env.db.timed_dataless_exists("b", TargetLeaf);
    Result result;
    until_logic_timed(a, b, result, nullptr, env.db.act_table_by_act_id.trace_length);
    EXPECT_TRUE(!result.empty());
    size_t resultSize = result.size();
    env2.db.enterLog("until_test_timed", "until_test_timed");
    size_t trace_count = 0;
    size_t timestamp = 0;
    for (auto it = result.begin(); it != result.end(); it++) {
        auto &trace = traces.at(it->first.first);
        size_t traceId = env2.db.enterTrace(std::to_string(trace_count));
        for (auto beg = trace.begin() + it->first.second, end = trace.end(); beg != end; beg++) {
            env2.db.enterEvent(timestamp++, *beg);
        }
        env2.db.exitTrace(traceId);
        trace_count++;
    }
    result.clear();
    env2.db.exitLog("until_test_timed", "until_test_timed");
    env2.db.index_data_structures(false);
    assert(result.empty());
    a = env2.db.timed_dataless_exists("a", ActivationLeaf);
    b = env2.db.timed_dataless_exists("b", TargetLeaf);
    until_logic_untimed(a, b, result, nullptr, env2.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(trace_count, result.size());
    EXPECT_EQ(resultSize, result.size());
}

TEST_F(Until_tests, negated_untimed) {
    Environment env2;
    env2.doStats = false;
    auto a = env.db.timed_dataless_exists("a", ActivationLeaf);
    auto b = env.db.timed_dataless_exists("b", TargetLeaf);
    Result until, result, resultF;
    until_logic_untimed(a, b, until, nullptr, env.db.act_table_by_act_id.trace_length);
    negated_logic_untimed(until, result, env.db.act_table_by_act_id.trace_length);
    std::unordered_set<size_t> S;
    for (const auto& ref : result) S.insert(ref.first.first);
    EXPECT_EQ(neg, S.size());

    S.clear();
    negated_fast_untimed(until, resultF, env.db.act_table_by_act_id.trace_length);
    for (const auto& ref : resultF) S.insert(ref.first.first);
    EXPECT_EQ(neg, S.size());
    EXPECT_EQ(result, resultF);
}

TEST_F(Until_tests, negated_timed) {
    Environment env2;
    env2.doStats = false;
    auto a = env.db.timed_dataless_exists("a", ActivationLeaf);
    auto b = env.db.timed_dataless_exists("b", TargetLeaf);

    Result until, result, resultF;
    until_logic_timed(a, b, until, nullptr, env.db.act_table_by_act_id.trace_length);
    negated_logic_timed(until, result, env.db.act_table_by_act_id.trace_length);

    size_t tot = 0;
    for (auto& ref : env.db.act_table_by_act_id.trace_length) tot += ref;

    EXPECT_EQ(until.size() + result.size(), tot);

    negated_fast_timed(until, resultF, env.db.act_table_by_act_id.trace_length);

    EXPECT_EQ(until.size() + resultF.size(), tot);
    EXPECT_EQ(result, resultF);
}


