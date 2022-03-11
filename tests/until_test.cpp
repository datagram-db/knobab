//
// Created by giacomo on 27/02/2022.
//

#define CTEST_COLOR_OK
#define CTEST_SEGFAULT

#include <vector>
#include <yaucl/structures/set_operations.h>
#include <knobab/Environment.h>

#include <gtest/gtest.h>
#include <knobab/operators/simple_ltlf_operators.h>
#include "log_for_tests.h"

class until_tests : public testing::Test {
protected:
    void SetUp() override {
        auto file = std::filesystem::current_path().parent_path().parent_path() / "data" / "testing" / "logUntil.txt";
        {
            std::ifstream if_{file};
            env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
        }
    }
    Environment env;
};

TEST_F(until_tests, basic) {
    auto a = env.db.exists("A", true);
    auto b = env.db.exists("B", true);
    std::set<uint32_t> expectedTraces{1,3,5,7,9,10,11,12,13};
    auto result = until(a, b, env.db.act_table_by_act_id.getTraceLengths(), nullptr);
    for (const auto& ref : result)
        EXPECT_TRUE(expectedTraces.contains(ref.first.first));
}

TEST_F(until_tests, logic) {
    auto a = env.db.exists("A", true);
    auto b = env.db.exists("B", true);
    std::set<uint32_t> expectedTraces{1,3,5,7,9,10,11,12,13};
    Result result;
    until_logic_untimed(a, b, env.db.act_table_by_act_id.getTraceLengths(), result, nullptr);
    for (const auto& ref : result)
        EXPECT_TRUE(expectedTraces.contains(ref.first.first));
}

TEST_F(until_tests, predicate_manager) {
    auto a = env.db.exists("A", true);
    auto b = env.db.exists("B", true);
    PredicateManager pm{{{{"x", "y", LT}}}, &env.db};
    std::set<uint32_t> expectedTraces{1,3,5,7,13};
    auto result = until(a, b, env.db.act_table_by_act_id.getTraceLengths(), &pm);
    for (const auto& ref : result)
        EXPECT_TRUE(expectedTraces.contains(ref.first.first));
}

TEST_F(until_tests, logic_pm) {
    auto a = env.db.exists("A", true);
    auto b = env.db.exists("B", true);
    PredicateManager pm{{{{"x", "y", LT}}}, &env.db};
    std::set<uint32_t> expectedTraces{1,3,5,7,13};
    Result result;
    until_logic_untimed(a, b, env.db.act_table_by_act_id.getTraceLengths(), result, &pm);
    for (const auto& ref : result)
        EXPECT_TRUE(expectedTraces.contains(ref.first.first));
}

#define DATA_EMPLACE_BACK(l,trace,event,...)     (l).emplace_back(std::make_pair((trace),(event)), std::make_pair((1.0),std::vector<uint16_t>{ __VA_ARGS__}));

TEST_F(until_tests, logic_timed) {
    auto a = env.db.exists("A", true);
    auto b = env.db.exists("B", true);
    Result result, expected;
    DATA_EMPLACE_BACK(expected, 1, 0, 0)
    DATA_EMPLACE_BACK(expected, 3, 0, 0, 1)
    DATA_EMPLACE_BACK(expected, 3, 1, 1)
    DATA_EMPLACE_BACK(expected, 5, 0, 0, 1)
    DATA_EMPLACE_BACK(expected, 5, 1, 1)
    DATA_EMPLACE_BACK(expected, 7, 0, 0, 1)
    DATA_EMPLACE_BACK(expected, 7, 1, 1)
    DATA_EMPLACE_BACK(expected, 9, 0, 0, 1)
    DATA_EMPLACE_BACK(expected, 9, 1, 1)
    DATA_EMPLACE_BACK(expected, 9, 3, 3, 4)
    DATA_EMPLACE_BACK(expected, 9, 4, 4)
    DATA_EMPLACE_BACK(expected, 10, 0, 0, 1)
    DATA_EMPLACE_BACK(expected, 10, 1, 1)
    DATA_EMPLACE_BACK(expected, 10, 2, 2, 3)
    DATA_EMPLACE_BACK(expected, 10, 3, 3)
    DATA_EMPLACE_BACK(expected, 11, 0, 0, 1)
    DATA_EMPLACE_BACK(expected, 11, 1, 1)
    DATA_EMPLACE_BACK(expected, 11, 3, 3, 4)
    DATA_EMPLACE_BACK(expected, 11, 4, 4)
    DATA_EMPLACE_BACK(expected, 12, 0, 0, 1)
    DATA_EMPLACE_BACK(expected, 12, 1, 1)
    DATA_EMPLACE_BACK(expected, 12, 2, 2, 3)
    DATA_EMPLACE_BACK(expected, 12, 3, 3)
    DATA_EMPLACE_BACK(expected, 13, 0, 0, 1, 2, 3)
    DATA_EMPLACE_BACK(expected, 13, 1, 1, 2, 3)
    DATA_EMPLACE_BACK(expected, 13, 2, 2, 3)
    DATA_EMPLACE_BACK(expected, 13, 3, 3)
    DATA_EMPLACE_BACK(expected, 13, 4, 4, 5)
    DATA_EMPLACE_BACK(expected, 13, 5, 5)
    until_logic_timed(a, b, env.db.act_table_by_act_id.getTraceLengths(), result, nullptr);
    EXPECT_TRUE(result == expected);
}