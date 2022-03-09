//
// Created by giacomo on 09/03/2022.
//


#include <vector>
#include <yaucl/structures/set_operations.h>
#include <knobab/Environment.h>
#include <gtest/gtest.h>
#include <knobab/operators/simple_ltlf_operators.h>

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
env.doStats = false;                                            \
env.load_log(TAB_SEPARATED_EVENTS, true, filename, false, ss);\
}\
\
Environment env;\
std::ifstream ss;                                               \
size_t pos, neg;                                                \
};\


TEST(Box)

TEST_F(Box_tests, basic) {
    auto a = env.db.exists("a", true);

    dataContainer result;
    global_logic_untimed(a, env.db.act_table_by_act_id.trace_length, result);
    EXPECT_EQ(pos, result.size());
}

TEST(Diamond)
TEST_F(Diamond_tests, basic) {
    auto a = env.db.exists("a", true);

    dataContainer result;
    future_logic_untimed(a, env.db.act_table_by_act_id.trace_length, result);
    EXPECT_EQ(pos, result.size());
}


TEST(Until)
TEST_F(Until_tests, basic) {
    auto a = env.db.exists("a", true);
    auto b = env.db.exists("b", true);

    dataContainer result;
    until_logic_untimed(a, b, env.db.act_table_by_act_id.trace_length, result);
    EXPECT_EQ(pos, result.size());
}