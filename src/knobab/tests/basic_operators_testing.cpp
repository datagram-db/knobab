//
// Created by giacomo on 02/11/22.
//
#include <vector>
#include "ServerQueryManager.h"

#include <yaucl/structures/set_operations.h>
#include <knobab/server/query_manager/Environment.h>
#include "knobab/server/operators/polyadic_dataless.h"
//#include <gtest/gtest.h>
#include <knobab/server/operators/simple_ltlf_operators.h>
#include <knobab/server/operators/fast_ltlf_operators.h>
#include <knobab/server/operators/novel_ltlf_operators.h>

#include <catch2/catch_test_macros.hpp>
#define ASSERT_TRUE(c)  REQUIRE(c)
#define EXPECT_TRUE(c)  REQUIRE(c)
#define EXPECT_EQ(a,b)  REQUIRE((a) == (b))
#define EXPECT_GE(a,b)  REQUIRE((a) >= (b))
#define DATA_EMPLACE_BACK(l,trace,event,...)     (l).emplace_back(std::make_pair((trace),(event)), std::make_pair((1.0), MarkedEventsVector{ __VA_ARGS__}))
#define DATA_EMPLACE_ACTORNONE(l,trace,event,b)  if (b) DATA_EMPLACE_BACK(l,trace,event,marked_event::activation(event)); else DATA_EMPLACE_BACK(l,trace,event)
#define DATA_DECREMENT_EMPLACE_BACK(l,trace,event,isMatch)    do { (l).emplace_back(std::make_pair((trace),(event)-1), std::make_pair((1.0),MarkedEventsVector{})); if (isMatch) (l).back().second.second.emplace_back(marked_event::activation(event));} while (false)
#define DATA_DECREMENT_EMPLACE_CORRNEXTBACK(l,trace,event,isMatch)    do { (l).emplace_back(std::make_pair((trace),(event)), std::make_pair((1.0),MarkedEventsVector{})); if (isMatch) { (l).back().second.second.emplace_back(marked_event::activation(event)); (l).back().second.second.emplace_back(marked_event::target(event+1)); } }  while (false)


TEST_CASE("basic_operators") {
    Environment env;
    std::filesystem::path curr = std::filesystem::current_path().parent_path() / "data" / "testing" / "basic_operator_testing" / "log.txt";
    {
        std::ifstream if_{curr};
        env.load_log(HUMAN_READABLE_YAUCL, true, curr.string(), false, if_);
    }
    SECTION("A") {
        for (int i = 0; i<2; i++) {
            bool value = (i == 0);
            auto resultA = env.db.timed_dataless_exists("A", value ? ActivationLeaf : NoneLeaf);

            Result A;
            DATA_EMPLACE_ACTORNONE(A, 0, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 3, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 4, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 5, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 6, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 7, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 7, 3, value);
            DATA_EMPLACE_ACTORNONE(A, 8, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 8, 3, value);
            DATA_EMPLACE_ACTORNONE(A, 9, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 9, 3, value);
            DATA_EMPLACE_ACTORNONE(A, 10, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 10, 2, value);
            DATA_EMPLACE_ACTORNONE(A, 11, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 11, 3, value);
            DATA_EMPLACE_ACTORNONE(A, 12, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 12, 2, value);

            EXPECT_EQ(resultA, A);
        }
    }
    SECTION("B") {
        for (int i = 0; i<2; i++) {
            bool value = (i == 0);
            auto resultA = env.db.timed_dataless_exists("B", (i == 0) ? ActivationLeaf : NoneLeaf);

            Result A;
            DATA_EMPLACE_ACTORNONE(A, 1, 0, value);
            DATA_EMPLACE_ACTORNONE(A, 3, 1, value);
            DATA_EMPLACE_ACTORNONE(A, 5, 1, value);
            DATA_EMPLACE_ACTORNONE(A, 7, 1, value);
            DATA_EMPLACE_ACTORNONE(A, 7, 2, value);
            DATA_EMPLACE_ACTORNONE(A, 8, 2, value);
            DATA_EMPLACE_ACTORNONE(A, 9, 1, value);
            DATA_EMPLACE_ACTORNONE(A, 9, 2, value);
            DATA_EMPLACE_ACTORNONE(A, 9, 4, value);
            DATA_EMPLACE_ACTORNONE(A, 9, 5, value);
            DATA_EMPLACE_ACTORNONE(A, 10, 1, value);
            DATA_EMPLACE_ACTORNONE(A, 10, 3, value);
            DATA_EMPLACE_ACTORNONE(A, 10, 4, value);
            DATA_EMPLACE_ACTORNONE(A, 11, 1, value);
            DATA_EMPLACE_ACTORNONE(A, 11, 2, value);
            DATA_EMPLACE_ACTORNONE(A, 11, 4, value);
            DATA_EMPLACE_ACTORNONE(A, 12, 1, value);
            DATA_EMPLACE_ACTORNONE(A, 12, 3, value);

            EXPECT_EQ(resultA, A);
        }
    }
    SECTION("init") {
        for (int i = 0; i<2; i++) {
            bool value = (i == 0);

            {
                auto resultA = env.db.init("A", (i == 0) ? ActivationLeaf :  NoneLeaf);
                Result A;
                DATA_EMPLACE_ACTORNONE(A, 0, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 3, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 4, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 5, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 6, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 7, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 8, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 9, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 10, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 11, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 12, 0, value);
                EXPECT_EQ(resultA, A);
            }

            {
                auto resultA = env.db.init("B", (i == 0) ? ActivationLeaf :  NoneLeaf);
                Result A;
                DATA_EMPLACE_ACTORNONE(A, 1, 0, value);
                EXPECT_EQ(resultA, A);
            }

            {
                auto resultA = env.db.init("C", (i == 0) ? ActivationLeaf :  NoneLeaf);
                Result A;
                DATA_EMPLACE_ACTORNONE(A, 2, 0, value);
                EXPECT_EQ(resultA, A);
            }
        }
    }
    SECTION("END") {
        for (int i = 0; i<2; i++) {
            bool value = (i == 0);

            {
                auto resultA = env.db.ends("A", (i == 0) ? ActivationLeaf :  NoneLeaf);
                Result A;
                DATA_EMPLACE_ACTORNONE(A, 0, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 7, 3, value);
                DATA_EMPLACE_ACTORNONE(A, 8, 3, value);
                EXPECT_EQ(resultA, A);
            }

            {
                auto resultA =  env.db.ends("B", (i == 0) ? ActivationLeaf :  NoneLeaf);
                Result A;
                DATA_EMPLACE_ACTORNONE(A, 1, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 3, 1, value);
                DATA_EMPLACE_ACTORNONE(A, 5, 1, value);
                DATA_EMPLACE_ACTORNONE(A, 9, 5, value);
                DATA_EMPLACE_ACTORNONE(A, 10, 4, value);
                DATA_EMPLACE_ACTORNONE(A, 11, 4, value);
                DATA_EMPLACE_ACTORNONE(A, 12, 3, value);
                EXPECT_EQ(resultA, A);
            }

            {
                auto resultA =  env.db.ends("C", (i == 0) ? ActivationLeaf :  NoneLeaf);
                Result A;
                DATA_EMPLACE_ACTORNONE(A, 2, 0, value);
                DATA_EMPLACE_ACTORNONE(A, 4, 1, value);
                DATA_EMPLACE_ACTORNONE(A, 6, 1, value);
                EXPECT_EQ(resultA, A);
            }
        }
    }
        /// Anonymous submission
    SECTION("ANDNEXT") {
        auto As = env.db.timed_dataless_exists("A", ActivationLeaf);
        Result out, expected;
        and_next(As, out, env.db, nullptr, {"B"}, nullptr);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 3, 0, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 5, 0, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 7, 0, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 9, 0, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 9, 3, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 10, 0, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 10, 2, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 11, 0, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 11, 3, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 12, 0, true);
        DATA_DECREMENT_EMPLACE_CORRNEXTBACK(expected, 12, 2, true);
        EXPECT_EQ(out, expected);
    }
    SECTION("NEXT") {
        for (int i = 0; i<2; i++) {
            bool value = (i == 0);

            {
                auto resultA = rnext(env.db.timed_dataless_exists("A", (i == 0) ? ActivationLeaf : NoneLeaf));
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
                auto resultA = rnext(env.db.timed_dataless_exists("B", (i == 0) ? ActivationLeaf : NoneLeaf));
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
                auto resultA = rnext(env.db.timed_dataless_exists("C", (i == 0) ? ActivationLeaf : NoneLeaf));
                Result A;
                DATA_DECREMENT_EMPLACE_BACK(A, 4, 1, value);
                DATA_DECREMENT_EMPLACE_BACK(A, 6, 1, value);
                DATA_DECREMENT_EMPLACE_BACK(A, 8, 1, value);
                EXPECT_EQ(resultA, A);
            }
        }
    }
}


TEST_CASE("globalTesting") {
    size_t pos, neg;
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};

    auto root_folder = std::filesystem::current_path().parent_path();
    std::string base{"Box"};
    std::filesystem::path curr = root_folder / "data" / "testing" / "ltlf" / base;
    auto sizes = root_folder / "data" / "testing" / "declare" / (base + "_pos_neg.txt");
    ServerQueryManager sqm;
    std::stringstream ss;
    std::ifstream fs{sizes};
    fs >> pos >> neg;

    ss << "load "
       << log_parse_format_type.at((size_t) TAB_SEPARATED_EVENTS)
       << " "
       << std::quoted(curr.string())
       << " no stats as "
       << std::quoted(base);
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    auto &ref = sqm.multiple_logs[base];
    auto A = ref.db.timed_dataless_exists("a", NoneLeaf);
    const auto &V = ref.db.act_table_by_act_id.getTraceLengths();

    SECTION("untimed") {
        Result globally1, globally2;
        global_fast_untimed(A, globally1, V);
        polydl_global_untimed(A, globally2, V);
        EXPECT_EQ(globally1, globally2);
        EXPECT_EQ(globally2.size(), 5);
    }

    SECTION("timed") {
        Result globally1, globally2;
        global_fast_timed(A, globally1, V);
        polydl_global_timed(A, globally2, V);
        for (size_t i = 0, N = std::min(globally2.size(), globally1.size()); i<N; i++) {
            if (globally1.at(i) != globally2.at(i)) {
                REQUIRE(false);
            }
        }
        EXPECT_EQ(globally1, globally2);
    }
}
