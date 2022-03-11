//
// Created by giacomo on 09/03/2022.
//


#include <vector>
#include <yaucl/structures/set_operations.h>
#include <knobab/Environment.h>
#include <gtest/gtest.h>
#include <knobab/operators/simple_ltlf_operators.h>

constexpr char tabChar = '\t';

#define TEST(name) class name ## _tests : public testing::Test {\
protected:\
void SetUp() override {\
std::filesystem::path curr = std::filesystem::current_path();   \
std::string base{#name};                                        \
auto filename = curr.parent_path().parent_path() / "data" /"testing"/"ltlf" / base;\
auto sizes = curr.parent_path().parent_path() / "data" /"testing"/"ltlf" / (base+"_pos_neg.txt");\
ss.open(filename);\
std::ifstream fs{sizes};                                               \
char c;                                                         \
fs >> pos >> c >> neg;                                          \
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
    auto a = env.db.exists("a", ActivationLeaf);
    Result result;
    global_logic_untimed(a, env.db.act_table_by_act_id.trace_length, result);
    EXPECT_EQ(pos, result.size());
}

TEST_F(Box_tests, timed) {
    Environment env2;
    env2.doStats = false;
    auto a = env.db.exists("a", ActivationLeaf);
    Result result;
    global_logic_timed(a, env.db.act_table_by_act_id.trace_length, result);
    EXPECT_TRUE(!result.empty());
    size_t resultSize = result.size();
    env2.db.enterLog("box_test_timed", "box_test_timed");
    size_t trace_count = 0;
    size_t timestamp = 0;
    for (auto it = result.begin(); it != result.end(); it++) {
        auto &trace = traces.at(it->first.first);
        auto beg = trace.begin() + it->first.second;
        auto end = trace.end();
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
    a = env2.db.exists("a", ActivationLeaf);
    global_logic_untimed(a, env2.db.act_table_by_act_id.trace_length, result);
    EXPECT_EQ(trace_count, result.size());
    EXPECT_EQ(resultSize, result.size());
}

TEST(Diamond)
TEST_F(Diamond_tests, basic) {
    auto a = env.db.exists("a", ActivationLeaf);

    Result result;
    future_logic_untimed(a, env.db.act_table_by_act_id.trace_length, result);
    EXPECT_EQ(pos, result.size());
}
TEST_F(Diamond_tests, timed) {
    Environment env2;
    env2.doStats = false;
    auto a = env.db.exists("a", ActivationLeaf);
    Result result;
    future_logic_timed(a, env.db.act_table_by_act_id.trace_length, result);
    EXPECT_TRUE(!result.empty());
    size_t resultSize = result.size();
    env2.db.enterLog("diamond_test_timed", "diamond_test_timed");
    size_t trace_count = 0;
    size_t timestamp = 0;
    for (auto it = result.begin(); it != result.end(); it++) {
        auto &trace = traces.at(it->first.first);
        auto beg = trace.begin() + it->first.second;
        auto end = trace.end();
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
    a = env2.db.exists("a", ActivationLeaf);
    future_logic_untimed(a, env2.db.act_table_by_act_id.trace_length, result);
    EXPECT_EQ(trace_count, result.size());
    EXPECT_EQ(resultSize, result.size());
}

TEST(Until)
TEST_F(Until_tests, basic) {
    auto a = env.db.exists("a", ActivationLeaf);
    auto b = env.db.exists("b", TargetLeaf);
    Result result;
    until_logic_untimed(a, b, env.db.act_table_by_act_id.trace_length, result);
    EXPECT_EQ(pos, result.size());
}


TEST_F(Until_tests, timed) {
    Environment env2;
    env2.doStats = false;
    auto a = env.db.exists("a", ActivationLeaf);
    auto b = env.db.exists("b", TargetLeaf);
    Result result;
    until_logic_timed(a, b, env.db.act_table_by_act_id.trace_length, result);
    EXPECT_TRUE(!result.empty());
    size_t resultSize = result.size();
    env2.db.enterLog("until_test_timed", "until_test_timed");
    size_t trace_count = 0;
    size_t timestamp = 0;
    for (auto it = result.begin(); it != result.end(); it++) {
        auto &trace = traces.at(it->first.first);
        auto beg = trace.begin() + it->first.second;
        auto end = trace.end();
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
    a = env2.db.exists("a", ActivationLeaf);
    b = env2.db.exists("b", TargetLeaf);
    until_logic_untimed(a, b, env2.db.act_table_by_act_id.trace_length, result);
    EXPECT_EQ(trace_count, result.size());
    EXPECT_EQ(resultSize, result.size());
}