//
// Created by giacomo on 24/02/2022.
//

#define CTEST_COLOR_OK
#define CTEST_SEGFAULT

#include <SimplifiedFuzzyStringMatching.h>
#include "../submodules/ctest/ctest.h"

#define GLOBAL_TEST(name)     CTEST_DATA(name) {  SimplifiedFuzzyStringMatching matcher; std::multimap<double, std::string> result; std::unordered_map<std::string, double> values; };

GLOBAL_TEST(fsm)

GLOBAL_TEST(fsm_exact_match)

CTEST_SETUP(fsm) {
    data->matcher.put("hello");
    data->matcher.put("bello");
    data->matcher.put("bel");
    data->matcher.put("hell");
    data->matcher.put("call");
    data->matcher.put("fall");
    data->matcher.put("tall");
    data->matcher.put("all");
    data->matcher.put("elementary");
    data->matcher.fuzzyMatch(0.0, 100, "fall", data->result);
    for (const auto& cp : data->result) {
        data->values.emplace(cp.second, cp.first);
    }
}

CTEST_SETUP(fsm_exact_match) {
    data->matcher.put("hello");
    data->matcher.put("bello");
    data->matcher.put("bel");
    data->matcher.put("hell");
    data->matcher.put("call");
    data->matcher.put("fall");
    data->matcher.put("tall");
    data->matcher.put("all");
    data->matcher.put("elementary");
    data->matcher.fuzzyMatch(1.0, 1, "fall", data->result);
    for (const auto& cp : data->result) {
        data->values.emplace(cp.second, cp.first);
    }
}

CTEST2(fsm_exact_match, only_one_match) {
    ASSERT_EQUAL(data->values.size(), 1);
}

CTEST2(fsm_exact_match, has_fall) {
    ASSERT_TRUE(data->values.contains("fall"));
}


CTEST2(fsm_exact_match, has_fall_top) {
    ASSERT_DBL_NEAR(data->values.find("fall")->second, 1.0);
}

CTEST2(fsm, data_not_matched) {
    ASSERT_TRUE(!(data->values.contains("elementary")));
}

CTEST2(fsm, match_order) {
    ASSERT_TRUE((data->values.at("fall")) > (data->values.at("all")));
    ASSERT_TRUE((data->values.at("all")) > (data->values.at("tall")));
    ASSERT_TRUE((data->values.at("all")) > (data->values.at("call")));
    ASSERT_DBL_NEAR((data->values.at("call")), (data->values.at("tall")));
    ASSERT_TRUE((data->values.at("call")) > (data->values.at("hell")));
    ASSERT_TRUE((data->values.at("hell")) > (data->values.at("hello")));
    ASSERT_DBL_NEAR((data->values.at("hello")), (data->values.at("bello")));
}