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

#include <gtest/gtest.h>
#include "log_for_tests.h"

class basic_operators : public testing::Test {
protected:
    void SetUp() override {
        ss << log1;
        env.load_log(HUMAN_READABLE_YAUCL, true, "log1.txt", false, ss);
    }

    Environment env;
    std::stringstream ss;
};
#define DATA_EMPLACE_BACK(l,trace,event,isMatch)    do { (l).emplace_back(std::make_pair((trace),(event)), std::make_pair((1.0),std::vector<uint16_t>{})); if (isMatch) (l).back().second.second.emplace_back(event);} while (false)
#define DATA_DECREMENT_EMPLACE_BACK(l,trace,event,isMatch)    do { (l).emplace_back(std::make_pair((trace),(event)-1), std::make_pair((1.0),std::vector<uint16_t>{})); if (isMatch) (l).back().second.second.emplace_back((event));} while (false)

TEST_F(basic_operators, A) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        auto resultA = env.db.exists("A", value);

        Result A;
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

        EXPECT_EQ(resultA, A);
    }
}

TEST_F(basic_operators, B) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        auto resultA = env.db.exists("B", value);

        Result A;
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

        EXPECT_EQ(resultA, A);
    }
}

TEST_F(basic_operators, C) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        auto resultA = env.db.exists("C", value);

        Result A;
        DATA_EMPLACE_BACK(A, 2, 0, value);
        DATA_EMPLACE_BACK(A, 4, 1, value);
        DATA_EMPLACE_BACK(A, 6, 1, value);
        DATA_EMPLACE_BACK(A, 8, 1, value);

        EXPECT_TRUE(resultA == A);
    }
}

TEST_F(basic_operators, init_act) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        {
            auto resultA = env.db.init("A", value);
            Result A;
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
            EXPECT_EQ(resultA, A);
        }

        {
            auto resultA = env.db.init("B", value);
            Result A;
            DATA_EMPLACE_BACK(A, 1, 0, value);
            EXPECT_EQ(resultA, A);
        }

        {
            auto resultA = env.db.init("C", value);
            Result A;
            DATA_EMPLACE_BACK(A, 2, 0, value);
            EXPECT_EQ(resultA, A);
        }
    }
}

TEST_F(basic_operators, end_act) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        {
            auto resultA = env.db.ends("A", value);
            Result A;
            DATA_EMPLACE_BACK(A, 0, 0, value);
            DATA_EMPLACE_BACK(A, 7, 3, value);
            DATA_EMPLACE_BACK(A, 8, 3, value);
            EXPECT_EQ(resultA, A);
        }

        {
            auto resultA =  env.db.ends("B", value);
            Result A;
            DATA_EMPLACE_BACK(A, 1, 0, value);
            DATA_EMPLACE_BACK(A, 3, 1, value);
            DATA_EMPLACE_BACK(A, 5, 1, value);
            DATA_EMPLACE_BACK(A, 9, 5, value);
            DATA_EMPLACE_BACK(A, 10, 4, value);
            DATA_EMPLACE_BACK(A, 11, 4, value);
            DATA_EMPLACE_BACK(A, 12, 3, value);
            EXPECT_EQ(resultA, A);
        }

        {
            auto resultA =  env.db.ends("C", value);
            Result A;
            DATA_EMPLACE_BACK(A, 2, 0, value);
            DATA_EMPLACE_BACK(A, 4, 1, value);
            DATA_EMPLACE_BACK(A, 6, 1, value);
            EXPECT_EQ(resultA, A);
        }
    }
}

TEST_F(basic_operators, next_act) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        {
            auto resultA = next(env.db.exists("A", value));
            Result A;
            DATA_DECREMENT_EMPLACE_BACK(A, 7, 3, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 8, 3, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 9, 3, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 10, 2, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 11, 3, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 12, 2, value);
            EXPECT_EQ(resultA, A);
        }

        {
            auto resultA = next(env.db.exists("B", value));
            Result A;
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
            EXPECT_EQ(resultA, A);
        }

        {
            auto resultA = next(env.db.exists("C", value));
            Result A;
            DATA_DECREMENT_EMPLACE_BACK(A, 4, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 6, 1, value);
            DATA_DECREMENT_EMPLACE_BACK(A, 8, 1, value);
            EXPECT_EQ(resultA, A);
        }
    }
}

TEST_F(basic_operators, no_multiple_labels) {
    {
        // A and B shall never appear at the same time
        Result result;
        auto resultA = (env.db.exists("A", true));
        auto resultB = (env.db.exists("B", true));
        setIntersection(resultA.begin(), resultA.end(), resultB.begin(), resultB.end(), std::back_inserter(result), Aggregators::maxSimilarity<double,double,double>, nullptr);
        EXPECT_TRUE(result.empty());
    }

    {
        // A and B shall never appear in the next time
        Result result;
        auto resultA = next(env.db.exists("A", true));
        auto resultB = next(env.db.exists("B", true));
        setIntersection(resultA.begin(), resultA.end(), resultB.begin(), resultB.end(), std::back_inserter(result), Aggregators::maxSimilarity<double,double,double>, nullptr);
        EXPECT_TRUE(result.empty());
    }

    {
        // On the other hand, predicates should have an intersection
        auto higherRange = env.db.exact_range_query({"A", "x", -10, 2000});
        auto narrowRange = env.db.exact_range_query({"A", "x", 0, 2000});
        std::vector<std::pair<trace_t, event_t>> result;
        std::set_intersection(higherRange.begin(), higherRange.end(), narrowRange.begin(), narrowRange.end(), std::back_inserter(result));
        EXPECT_EQ(narrowRange, result);
    }

    {
        // A and_{where x<y} next B
        Result result, expected;
        PredicateManager pm{{{{"x", "y", LT}}}, &env.db};
        auto resultA = (env.db.exists("A", true));
        auto resultB = next(env.db.exists("B", true));
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

        EXPECT_EQ(result, expected);
    }
}

TEST_F(basic_operators, globally_untimed) {
    {
        auto result = global(env.db.exists("A", true), env.db.act_table_by_act_id.getTraceLengths());
        EXPECT_EQ(result, Result {std::make_pair(std::make_pair(0, 0), std::make_pair(1.0, std::vector<uint16_t>{0}))});
    }
    {
        auto result = global(env.db.exists("B", true), env.db.act_table_by_act_id.getTraceLengths());
        EXPECT_EQ(result, Result {std::make_pair(std::make_pair(1, 0), std::make_pair(1.0, std::vector<uint16_t>{0}))});
    }
    {
        auto result = global(env.db.exists("C", true), env.db.act_table_by_act_id.getTraceLengths());
        EXPECT_EQ(result, Result {std::make_pair(std::make_pair(2, 0), std::make_pair(1.0, std::vector<uint16_t>{0}))});
    }
}

