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
    dataContainer result;
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
    dataContainer result;
    until_logic_untimed(a, b, env.db.act_table_by_act_id.getTraceLengths(), result, &pm);
    for (const auto& ref : result)
        EXPECT_TRUE(expectedTraces.contains(ref.first.first));
}