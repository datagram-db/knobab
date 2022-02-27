//
// Created by giacomo on 27/02/2022.
//

#define CTEST_COLOR_OK
#define CTEST_SEGFAULT

#include <vector>
#include <yaucl/structures/set_operations.h>
#include <knobab/Environment.h>
#include <fstream>
#include <filesystem>

#include "../submodules/ctest/ctest.h"
#include "log_for_tests.h"

CTEST_DATA(until_tests) {
    Environment env;
    std::stringstream ss;
};

CTEST_SETUP(until_tests) {
    Environment& env = data->env;
    env.clear();
    data->ss << logUntil;
    env.load_log(HUMAN_READABLE_YAUCL, true, "logUntil.txt", false, data->ss);
};

CTEST2(until_tests, basic) {
    auto a = data->env.db.exists("A", true);
    auto b = data->env.db.exists("B", true);
    std::set<uint32_t> expectedTraces{1,3,5,7,9,10,11,12,13};
    auto result = until(a, b, data->env.db.act_table_by_act_id.getTraceLengths(), nullptr);
    for (const auto& ref : result)
        ASSERT_TRUE(expectedTraces.contains(ref.first.first));
}

CTEST2(until_tests, predicate_manager) {
    auto a = data->env.db.exists("A", true);
    auto b = data->env.db.exists("B", true);
    PredicateManager pm{{{{"x", "y", LT}}}, &data->env.db};
    std::set<uint32_t> expectedTraces{1,3,5,7,13};
    auto result = until(a, b, data->env.db.act_table_by_act_id.getTraceLengths(), &pm);
    for (const auto& ref : result)
        ASSERT_TRUE(expectedTraces.contains(ref.first.first));
}