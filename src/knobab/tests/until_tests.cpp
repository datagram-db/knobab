//
// Created by giacomo on 27/02/2022.
//

#define CTEST_COLOR_OK
#define CTEST_SEGFAULT

#include <vector>
#include <yaucl/structures/set_operations.h>
#include <knobab/server/query_manager/Environment.h>

//#include <gtest/gtest.h>
#include <knobab/server/operators/simple_ltlf_operators.h>
#include <knobab/server/operators/fast_ltlf_operators.h>
#include <knobab/server/operators/polyadic_dataless.h>

#include <catch2/catch_test_macros.hpp>
#define ASSERT_TRUE(c)  REQUIRE(c)
#define EXPECT_TRUE(c)  REQUIRE(c)
#define EXPECT_EQ(a,b)  REQUIRE((a) == (b))
#define EXPECT_GE(a,b)  REQUIRE((a) >= (b))

TEST_CASE("until_tests, basic") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    auto a = env.db.timed_dataless_exists("A", NoneLeaf);
    auto b = env.db.timed_dataless_exists("B", NoneLeaf);
    std::set<uint32_t> expectedTraces{1,3,5,7,9,10,11,12,13};
    auto result = until(a, b, env.db.act_table_by_act_id.getTraceLengths(), nullptr);
    for (const auto& ref : result)
        ASSERT_TRUE(expectedTraces.contains(ref.first.first));
}

TEST_CASE("until_tests, logic") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    auto a = env.db.timed_dataless_exists("A", NoneLeaf);
    auto b = env.db.timed_dataless_exists("B", NoneLeaf);
    std::set<uint32_t> expectedTraces{1,3,5,7,9,10,11,12,13};

    Result result, result2;

    SECTION("nonpoly") {
        until_logic_untimed(a, b, result, nullptr, env.db.act_table_by_act_id.trace_length);
        for (const auto& ref : result)
            ASSERT_TRUE(expectedTraces.contains(ref.first.first));

        polydl_sameroot_until_logic_untimed(a, b, result2, nullptr, env.db.act_table_by_act_id.trace_length);
        EXPECT_EQ(result, result2);
    }
}

TEST_CASE("until_tests, fast") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    auto a = env.db.timed_dataless_exists("A", NoneLeaf);
    auto b = env.db.timed_dataless_exists("B", NoneLeaf);
    std::set<uint32_t> expectedTraces{1,3,5,7,9,10,11,12,13};
    Result result, result2;
    until_fast_untimed(a, b, result, nullptr, env.db.act_table_by_act_id.trace_length);
    for (const auto& ref : result)
        ASSERT_TRUE(expectedTraces.contains(ref.first.first));

    until_logic_untimed(a, b, result2, nullptr, env.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(result, result2);
}

TEST_CASE("until_tests, predicate_manager") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    auto a = env.db.timed_dataless_exists("A", TargetLeaf);
    auto b = env.db.timed_dataless_exists("B", ActivationLeaf);
    PredicateManager pm{{{{"y", "x", GT}}}, &env.db};
    std::set<uint32_t> expectedTraces{1,3,5,7,13};
    auto result = until(a, b, env.db.act_table_by_act_id.getTraceLengths(), &pm);
    for (const auto& ref : result)
        EXPECT_TRUE(expectedTraces.contains(ref.first.first));
}

TEST_CASE("until_tests, logic_pm") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    auto a = env.db.timed_dataless_exists("A", TargetLeaf);
    auto b = env.db.timed_dataless_exists("B", ActivationLeaf);
    PredicateManager pm{{{{"y","x",  GT}}}, &env.db};
    std::set<uint32_t> expectedTraces{1,3,5,7,13};
    std::set<uint32_t> returnedTraces;
    Result result;
    until_logic_untimed(a, b, result, &pm, env.db.act_table_by_act_id.trace_length);
    for (const auto& ref : result)
        EXPECT_TRUE(expectedTraces.contains(ref.first.first));
    for (const auto& ref : result)
        returnedTraces.insert(ref.first.first);
    EXPECT_EQ(returnedTraces, expectedTraces);
}

TEST_CASE("until_tests, fast_pm") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    auto a = env.db.timed_dataless_exists("A", TargetLeaf);
    auto b = env.db.timed_dataless_exists("B", ActivationLeaf);
    PredicateManager pm{{{{"y", "x", GT}}}, &env.db};
    Result result, result2;
    until_fast_untimed(a, b, result, &pm, env.db.act_table_by_act_id.trace_length);
    until_logic_untimed(a, b, result2, &pm, env.db.act_table_by_act_id.trace_length);
    EXPECT_EQ(result, result2);
}

#define DATA_EMPLACE_BACK(l,trace,event,...)     (l).emplace_back(std::make_pair((trace),(event)), std::make_pair((1.0), MarkedEventsVector{ __VA_ARGS__}));

TEST_CASE("until_tests, logic_timed") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    {
        auto a = env.db.timed_dataless_exists("A", NoneLeaf);
        auto b = env.db.timed_dataless_exists("B", NoneLeaf);
        Result result, expected, result2;

        DATA_EMPLACE_BACK(expected, 1, 0)
        DATA_EMPLACE_BACK(expected, 3, 0)
        DATA_EMPLACE_BACK(expected, 3, 1)
        DATA_EMPLACE_BACK(expected, 5, 0)
        DATA_EMPLACE_BACK(expected, 5, 1)
        DATA_EMPLACE_BACK(expected, 7, 0)
        DATA_EMPLACE_BACK(expected, 7, 1)
        DATA_EMPLACE_BACK(expected, 7, 2)
        DATA_EMPLACE_BACK(expected, 8, 2)
        DATA_EMPLACE_BACK(expected, 9, 0)
        DATA_EMPLACE_BACK(expected, 9, 1)
        DATA_EMPLACE_BACK(expected, 9, 2)
        DATA_EMPLACE_BACK(expected, 9, 3)
        DATA_EMPLACE_BACK(expected, 9, 4)
        DATA_EMPLACE_BACK(expected, 9, 5)
        DATA_EMPLACE_BACK(expected, 10, 0)
        DATA_EMPLACE_BACK(expected, 10, 1)
        DATA_EMPLACE_BACK(expected, 10, 2)
        DATA_EMPLACE_BACK(expected, 10, 3)
        DATA_EMPLACE_BACK(expected, 10, 4)
        DATA_EMPLACE_BACK(expected, 11, 0)
        DATA_EMPLACE_BACK(expected, 11, 1)
        DATA_EMPLACE_BACK(expected, 11, 2)
        DATA_EMPLACE_BACK(expected, 11, 3)
        DATA_EMPLACE_BACK(expected, 11, 4)
        DATA_EMPLACE_BACK(expected, 12, 0)
        DATA_EMPLACE_BACK(expected, 12, 1)
        DATA_EMPLACE_BACK(expected, 12, 2)
        DATA_EMPLACE_BACK(expected, 12, 3)
        DATA_EMPLACE_BACK(expected, 13, 0)
        DATA_EMPLACE_BACK(expected, 13, 1)
        DATA_EMPLACE_BACK(expected, 13, 2)
        DATA_EMPLACE_BACK(expected, 13, 3)
        DATA_EMPLACE_BACK(expected, 13, 4)
        DATA_EMPLACE_BACK(expected, 13, 5)
        until_logic_timed(a, b, result, nullptr, env.db.act_table_by_act_id.trace_length);
        EXPECT_EQ(result, expected);

        polydl_sameroot_until_logic_timed(a, b, result2, nullptr, env.db.act_table_by_act_id.trace_length);
        EXPECT_EQ(result, result2);
    }

    {
        auto a = env.db.timed_dataless_exists("A", NoneLeaf);
        auto b = env.db.timed_dataless_exists("B", TargetLeaf);
        Result result, expected, result2;
        DATA_EMPLACE_BACK(expected, 1, 0, marked_event::target(0))
        DATA_EMPLACE_BACK(expected, 3, 0, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 3, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 5, 0, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 5, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 7, 0, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 7, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 7, 2, marked_event::target(2))
        DATA_EMPLACE_BACK(expected, 8, 2, marked_event::target(2))
        DATA_EMPLACE_BACK(expected, 9, 0, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 9, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 9, 2, marked_event::target(2))
        DATA_EMPLACE_BACK(expected, 9, 3, marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 9, 4, marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 9, 5, marked_event::target(5))
        DATA_EMPLACE_BACK(expected, 10, 0, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 10, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 10, 2, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 10, 3, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 10, 4, marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 11, 0, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 11, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 11, 2, marked_event::target(2))
        DATA_EMPLACE_BACK(expected, 11, 3, marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 11, 4, marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 12, 0, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 12, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 12, 2, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 12, 3, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 0, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 1, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 2, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 3, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 4, marked_event::target(5))
        DATA_EMPLACE_BACK(expected, 13, 5, marked_event::target(5))
        until_logic_timed(a, b, result, nullptr, env.db.act_table_by_act_id.trace_length);
        EXPECT_EQ(result, expected);

        polydl_sameroot_until_logic_timed(a, b, result2, nullptr, env.db.act_table_by_act_id.trace_length);
        EXPECT_EQ(result, result2);
    }

    {
        auto a = env.db.timed_dataless_exists("A", ActivationLeaf);
        auto b = env.db.timed_dataless_exists("B", TargetLeaf);
        Result result, expected, result2;
        DATA_EMPLACE_BACK(expected, 1, 0, marked_event::target(0))
        DATA_EMPLACE_BACK(expected, 3, 0, marked_event::activation(0), marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 3, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 5, 0, marked_event::activation(0), marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 5, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 7, 0, marked_event::activation(0), marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 7, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 7, 2, marked_event::target(2))
        DATA_EMPLACE_BACK(expected, 8, 2, marked_event::target(2))
        DATA_EMPLACE_BACK(expected, 9, 0, marked_event::activation(0), marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 9, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 9, 2, marked_event::target(2))
        DATA_EMPLACE_BACK(expected, 9, 3, marked_event::activation(3), marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 9, 4, marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 9, 5, marked_event::target(5))
        DATA_EMPLACE_BACK(expected, 10, 0, marked_event::activation(0), marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 10, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 10, 2, marked_event::activation(2), marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 10, 3, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 10, 4, marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 11, 0, marked_event::activation(0), marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 11, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 11, 2, marked_event::target(2))
        DATA_EMPLACE_BACK(expected, 11, 3, marked_event::activation(3), marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 11, 4, marked_event::target(4))
        DATA_EMPLACE_BACK(expected, 12, 0, marked_event::activation(0), marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 12, 1, marked_event::target(1))
        DATA_EMPLACE_BACK(expected, 12, 2, marked_event::activation(2), marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 12, 3, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 0, marked_event::activation(0),marked_event::activation(1),marked_event::activation(2), marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 1, marked_event::activation(1),marked_event::activation(2), marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 2, marked_event::activation(2), marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 3, marked_event::target(3))
        DATA_EMPLACE_BACK(expected, 13, 4, marked_event::activation(4), marked_event::target(5))
        DATA_EMPLACE_BACK(expected, 13, 5, marked_event::target(5))
        for (auto& ref : expected)
            std::sort(ref.second.second.begin(), ref.second.second.end());
        until_logic_timed(a, b, result, nullptr, env.db.act_table_by_act_id.trace_length);
        EXPECT_EQ(result, expected);

        polydl_sameroot_until_logic_timed(a, b, result2, nullptr, env.db.act_table_by_act_id.trace_length);
        EXPECT_EQ(result, result2);
    }

}


TEST_CASE("until_tests, aAndFutureBTimed") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    auto a = env.db.timed_dataless_exists("A", ActivationLeaf);
    auto b = env.db.timed_dataless_exists("B", TargetLeaf);

    Result futureB, aAndFutureB1, aAndFutureB2, fast;


    long long t1,t2,t3,t4;
    {
        auto start = std::chrono::high_resolution_clock::now();
        future_logic_timed(b, futureB, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t1 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        and_logic_timed(a, futureB, aAndFutureB1, nullptr, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t2 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        and_fast_timed(a, futureB, aAndFutureB2, nullptr, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t3 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    futureB.clear();
    EXPECT_EQ(aAndFutureB1, aAndFutureB2);

    {
        auto start = std::chrono::high_resolution_clock::now();
        aAndFutureB_timed_variant_1(a, b, fast, nullptr, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t4 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    EXPECT_EQ(fast, aAndFutureB2);
    EXPECT_GE((t1+t2), t4); // benchmarking assumption, old it works better on small data
    EXPECT_GE((t1+t3), t4); // benchmarking assumption, old it works better on small data
}

TEST_CASE("until_tests, aAndNextGloballyBTimed") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    auto a = env.db.timed_dataless_exists("A", ActivationLeaf);
    auto b = env.db.timed_dataless_exists("B", TargetLeaf);

    Result nextGlobalB, aAndXGB1, aAndXGB2, fast;


    long long t1,t2,t3,t4;
    {
        Result globalB;
        auto start = std::chrono::high_resolution_clock::now();
        global_logic_timed(b, globalB, env.db.act_table_by_act_id.trace_length);
        next_fast(globalB, nextGlobalB, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t1 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        and_logic_timed(a, nextGlobalB, aAndXGB1, nullptr, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t2 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        and_fast_timed(a, nextGlobalB, aAndXGB2, nullptr, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t3 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    nextGlobalB.clear();
    EXPECT_EQ(aAndXGB1, aAndXGB2);

    {
        auto start = std::chrono::high_resolution_clock::now();
        aAndNextGloballyB_timed(a, b, fast, nullptr, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t4 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    EXPECT_EQ(fast, aAndXGB2);
    EXPECT_GE((t1+t2), t4); // benchmarking assumption
    EXPECT_GE((t1+t3), t4); // benchmarking assumption
}



TEST_CASE("until_tests, aAndGloballyBTimed") {
    Environment env;
    auto file = std::filesystem::current_path().parent_path() / "data" / "testing" / "until_test" / "logUntil.txt";
    {
        std::ifstream if_{file};
        env.load_log(HUMAN_READABLE_YAUCL, true, file.string(), false, if_);
    }
    auto a = env.db.timed_dataless_exists("A", ActivationLeaf);
    auto b = env.db.timed_dataless_exists("B", TargetLeaf);

    Result  aAndXGB1, aAndXGB2, fast;
    Result globalB;

    long long t1,t2,t3,t4;
    {
        auto start = std::chrono::high_resolution_clock::now();
        global_logic_timed(b, globalB, env.db.act_table_by_act_id.trace_length);
        //next_fast(globalB, nextGlobalB, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t1 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        and_logic_timed(a, globalB, aAndXGB1, nullptr, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t2 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        and_fast_timed(a, globalB, aAndXGB2, nullptr, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t3 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    globalB.clear();
    EXPECT_EQ(aAndXGB1, aAndXGB2);

    {
        auto start = std::chrono::high_resolution_clock::now();
        aAndGloballyB_timed_variant_2(a, b, fast, nullptr, env.db.act_table_by_act_id.trace_length);
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        t4 = std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }

    EXPECT_EQ(fast, aAndXGB2);
    EXPECT_GE((t1+t2), t4); // benchmarking assumption
    EXPECT_GE((t1+t3), t4); // benchmarking assumption
}