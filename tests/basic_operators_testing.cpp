//
// Created by giacomo on 25/02/2022.
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

CTEST_DATA(basic_operators) {
    Environment env;
};

CTEST_DATA(until_test) {
    Environment env;
};

#define DATA_EMPLACE_BACK(l,trace,event,isMatch)    do { (l).emplace_back(std::make_pair((trace),(event)), std::make_pair((1.0),std::vector<uint16_t>{})); if (isMatch) (l).back().second.second.emplace_back(event);} while (false)
#define DATA_DECREMENT_EMPLACE_BACK(l,trace,event,isMatch)    do { (l).emplace_back(std::make_pair((trace),(event)-1), std::make_pair((1.0),std::vector<uint16_t>{})); if (isMatch) (l).back().second.second.emplace_back((event));} while (false)


CTEST_SETUP(basic_operators) {
    Environment& env = data->env;
    env.clear();
    std::stringstream ss;
    ss << log1;
    // log_data_format format, bool loadData, std::istream &stream, KnowledgeBase &output,
    //                              std::string &filename
    env.load_log(HUMAN_READABLE_YAUCL, true, "log1", false, ss);
};


CTEST_SETUP(until_test) {
    Environment& env = data->env;
    env.clear();
    std::stringstream ss;
    ss << logUntil;
    env.load_log(HUMAN_READABLE_YAUCL, true, "logUntil.txt", false, ss);
};


CTEST2(basic_operators, A) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        auto resultA = data->env.db.exists("A", value);

        dataContainer A;
        DATA_EMPLACE_BACK(A, 0, 0, value);
        DATA_EMPLACE_BACK(A, 3, 0, value);
        DATA_EMPLACE_BACK(A, 4, 0, value);
        DATA_EMPLACE_BACK(A, 5, 0, value);
        DATA_EMPLACE_BACK(A, 6, 0, value);
        DATA_EMPLACE_BACK(A, 7, 0, value);
        DATA_EMPLACE_BACK(A, 7, 3, value);
        DATA_EMPLACE_BACK(A, 8, 0, value);
        DATA_EMPLACE_BACK(A, 8, 3, value);
        DATA_EMPLACE_BACK(A, 9, 0, value);
        DATA_EMPLACE_BACK(A, 9, 3, value);
        DATA_EMPLACE_BACK(A, 10, 0, value);
        DATA_EMPLACE_BACK(A, 10, 2, value);
        DATA_EMPLACE_BACK(A, 11, 0, value);
        DATA_EMPLACE_BACK(A, 11, 3, value);
        DATA_EMPLACE_BACK(A, 12, 0, value);
        DATA_EMPLACE_BACK(A, 12, 2, value);

        ASSERT_TRUE(resultA == A);
    }
}

CTEST2(basic_operators, B) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        auto resultA = data->env.db.exists("B", value);

        dataContainer A;
        DATA_EMPLACE_BACK(A, 1, 0, value);
        DATA_EMPLACE_BACK(A, 3, 1, value);
        DATA_EMPLACE_BACK(A, 5, 1, value);
        DATA_EMPLACE_BACK(A, 7, 1, value);
        DATA_EMPLACE_BACK(A, 7, 2, value);
        DATA_EMPLACE_BACK(A, 8, 2, value);
        DATA_EMPLACE_BACK(A, 9, 1, value);
        DATA_EMPLACE_BACK(A, 9, 2, value);
        DATA_EMPLACE_BACK(A, 9, 4, value);
        DATA_EMPLACE_BACK(A, 9, 5, value);
        DATA_EMPLACE_BACK(A, 10, 1, value);
        DATA_EMPLACE_BACK(A, 10, 3, value);
        DATA_EMPLACE_BACK(A, 10, 4, value);
        DATA_EMPLACE_BACK(A, 11, 1, value);
        DATA_EMPLACE_BACK(A, 11, 2, value);
        DATA_EMPLACE_BACK(A, 11, 4, value);
        DATA_EMPLACE_BACK(A, 12, 1, value);
        DATA_EMPLACE_BACK(A, 12, 3, value);

        ASSERT_TRUE(resultA == A);
    }
}

CTEST2(basic_operators, C) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        auto resultA = data->env.db.exists("C", value);

        dataContainer A;
        DATA_EMPLACE_BACK(A, 2, 0, value);
        DATA_EMPLACE_BACK(A, 4, 1, value);
        DATA_EMPLACE_BACK(A, 6, 1, value);
        DATA_EMPLACE_BACK(A, 8, 1, value);

        ASSERT_TRUE(resultA == A);
    }
}

CTEST2(basic_operators, init_act) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        {
            auto resultA = data->env.db.init("A", value);
            dataContainer A;
            DATA_EMPLACE_BACK(A, 0, 0, value);
            DATA_EMPLACE_BACK(A, 3, 0, value);
            DATA_EMPLACE_BACK(A, 4, 0, value);
            DATA_EMPLACE_BACK(A, 5, 0, value);
            DATA_EMPLACE_BACK(A, 6, 0, value);
            DATA_EMPLACE_BACK(A, 7, 0, value);
            DATA_EMPLACE_BACK(A, 8, 0, value);
            DATA_EMPLACE_BACK(A, 9, 0, value);
            DATA_EMPLACE_BACK(A, 10, 0, value);
            DATA_EMPLACE_BACK(A, 11, 0, value);
            DATA_EMPLACE_BACK(A, 12, 0, value);
            ASSERT_TRUE(resultA == A);
        }

        {
            auto resultA = data->env.db.init("B", value);
            dataContainer A;
            DATA_EMPLACE_BACK(A, 1, 0, value);
            ASSERT_TRUE(resultA == A);
        }

        {
            auto resultA = data->env.db.init("C", value);
            dataContainer A;
            DATA_EMPLACE_BACK(A, 2, 0, value);
            ASSERT_TRUE(resultA == A);
        }
    }
}

CTEST2(basic_operators, end_act) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        {
            auto resultA = data->env.db.ends("A", value);
            dataContainer A;
            DATA_EMPLACE_BACK(A, 0, 0, value);
            DATA_EMPLACE_BACK(A, 7, 3, value);
            DATA_EMPLACE_BACK(A, 8, 3, value);
            ASSERT_TRUE(resultA == A);
        }

        {
            auto resultA = data->env.db.ends("B", value);
            dataContainer A;
            DATA_EMPLACE_BACK(A, 1, 0, value);
            DATA_EMPLACE_BACK(A, 3, 1, value);
            DATA_EMPLACE_BACK(A, 5, 1, value);
            DATA_EMPLACE_BACK(A, 9, 5, value);
            DATA_EMPLACE_BACK(A, 10, 4, value);
            DATA_EMPLACE_BACK(A, 11, 4, value);
            DATA_EMPLACE_BACK(A, 12, 3, value);
            ASSERT_TRUE(resultA == A);
        }

        {
            auto resultA = data->env.db.ends("C", value);
            dataContainer A;
            DATA_EMPLACE_BACK(A, 2, 0, value);
            DATA_EMPLACE_BACK(A, 4, 1, value);
            DATA_EMPLACE_BACK(A, 6, 1, value);
            ASSERT_TRUE(resultA == A);
        }
    }
}

CTEST2(basic_operators, next_act) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        {
            auto resultA = next(data->env.db.exists("A", value));
            dataContainer A;
            DATA_DECREMENT_EMPLACE_BACK(A, 7, 3, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 8, 3, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 9, 3, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 10, 2, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 11, 3, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 12, 2, value);
            ASSERT_TRUE(resultA == A);
        }

        {
            auto resultA = next(data->env.db.exists("B", value));
            dataContainer A;
            DATA_DECREMENT_EMPLACE_BACK(A, 3, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 5, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 7, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 7, 2, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 8, 2, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 9, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 9, 2, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 9, 4, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 9, 5, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 10, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 10, 3, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 10, 4, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 11, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 11, 2, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 11, 4, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 12, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 12, 3, value);
            ASSERT_TRUE(resultA == A);
        }

        {
            auto resultA = next(data->env.db.exists("C", value));
            dataContainer A;
            DATA_DECREMENT_EMPLACE_BACK(A, 4, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 6, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 8, 1, value);
            ASSERT_TRUE(resultA == A);
        }
    }
}

CTEST2(basic_operators, no_multiple_labels) {
    {
        // A and B shall never appear at the same time
        dataContainer result;
        auto resultA = (data->env.db.exists("A", true));
        auto resultB = (data->env.db.exists("B", true));
        setIntersection(resultA.begin(), resultA.end(), resultB.begin(), resultB.end(), std::back_inserter(result), Aggregators::maxSimilarity<double,double,double>, nullptr);
        ASSERT_TRUE(result.empty());
    }

    {
        // A and B shall never appear in the next time
        dataContainer result;
        auto resultA = next(data->env.db.exists("A", true));
        auto resultB = next(data->env.db.exists("B", true));
        setIntersection(resultA.begin(), resultA.end(), resultB.begin(), resultB.end(), std::back_inserter(result), Aggregators::maxSimilarity<double,double,double>, nullptr);
        ASSERT_TRUE(result.empty());
    }

    {
        // On the other hand, predicates should have an intersection
        auto higherRange = data->env.db.exact_range_query({"A", "x", -10, 2000});
        auto narrowRange = data->env.db.exact_range_query({"A", "x", 0, 2000});
        std::vector<std::pair<trace_t, event_t>> result;
        std::set_intersection(higherRange.begin(), higherRange.end(), narrowRange.begin(), narrowRange.end(), std::back_inserter(result));
        ASSERT_TRUE(narrowRange == result);
    }

    {
        // A and_{where x<y} next B
        dataContainer result, expected;
        PredicateManager pm{{{{"x", "y", LT}}}, &data->env.db};
        auto resultA = (data->env.db.exists("A", true));
        auto resultB = next(data->env.db.exists("B", true));
        setIntersection(resultA.begin(), resultA.end(), resultB.begin(), resultB.end(), std::back_inserter(result), Aggregators::maxSimilarity<double,double,double>, &pm);

        DATA_EMPLACE_BACK(expected, 3, 0, false);
        expected.back().second.second.emplace_back(0);
        expected.back().second.second.emplace_back(1);
        DATA_EMPLACE_BACK(expected, 5, 0, false);
        expected.back().second.second.emplace_back(0);
        expected.back().second.second.emplace_back(1);
        DATA_EMPLACE_BACK(expected, 7, 0, false);
        expected.back().second.second.emplace_back(0);
        expected.back().second.second.emplace_back(1);

        ASSERT_TRUE(result == expected);
    }
}

CTEST2(basic_operators, globally_untimed) {
    {
        auto result = global(data->env.db.exists("A", true), data->env.db.act_table_by_act_id.getTraceLengths());
        ASSERT_TRUE(result == dataContainer {std::make_pair(std::make_pair(0,0), std::make_pair(1.0, std::vector<uint16_t>{0}))});
    }
    {
        auto result = global(data->env.db.exists("B", true), data->env.db.act_table_by_act_id.getTraceLengths());
        ASSERT_TRUE(result == dataContainer {std::make_pair(std::make_pair(1,0), std::make_pair(1.0, std::vector<uint16_t>{0}))});
    }
    {
        auto result = global(data->env.db.exists("C", true), data->env.db.act_table_by_act_id.getTraceLengths());
        ASSERT_TRUE(result == dataContainer {std::make_pair(std::make_pair(2,0), std::make_pair(1.0, std::vector<uint16_t>{0}))});
    }
}

CTEST2(until_test, until_basic) {
    auto a = data->env.db.exists("A", true);
    auto b = data->env.db.exists("B", true);
    {
        std::set<uint32_t> expectedTraces{1,3,5,7,9,10,11,12,13};
        auto result = until(a, b, data->env.db.act_table_by_act_id.getTraceLengths(), nullptr);
        for (const auto& ref : result)
            ASSERT_TRUE(expectedTraces.contains(ref.first.first));
    }

    PredicateManager pm{{{{"x", "y", LT}}}, &data->env.db};
    {
        std::set<uint32_t> expectedTraces{1,3,5,7,13};
        auto result = until(a, b, data->env.db.act_table_by_act_id.getTraceLengths(), &pm);
        for (const auto& ref : result)
            ASSERT_TRUE(expectedTraces.contains(ref.first.first));
    }
}