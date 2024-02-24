#include <knobab/mining/polyadic/polyadic_bolt.h>
#include "yaucl/bpm/structures/log/data_loader.h"
#include "knobab/server/query_manager/ServerQueryManager.h"
#include <iostream>
#include <functional>
#include "yaucl/learning/DecisionTree.h"
#include "yaucl/bpm/structures/commons/DeclareDataAware.h"
#include "knobab/server/query_manager/Environment.h"
#include "knobab/mining/bolt2.h"
#include <catch2/catch_test_macros.hpp>


#define STRING(s) #s
#define TEST(a,b)       TEST_CASE(a ## b)
#define ASSERT_TRUE(c)  REQUIRE(c)

double supp = 0.05;
const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};

auto folder = std::filesystem::current_path().parent_path() / "data" / "testing" / "mining";
static inline std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> load_and_return_polyadic(const std::filesystem::path& path) {
    auto world_file_to_load = path.string();
    ServerQueryManager sqm;
    std::stringstream ss;


    ss << "load "
       << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)
       << " "
       << std::quoted(world_file_to_load)
       <<  " with data as "
       << std::quoted(world_file_to_load);
    auto tmp = sqm.runQuery(ss.str());

//    sqm.runQuery(query_plan);

    polyadic_bolt g;

    std::vector<DeclareDataAware> VVV;
    g.run(supp, &sqm.multiple_logs[world_file_to_load].db);

//    auto result = bolt2(
//            sqm.multiple_logs[world_file_to_load].db, supp, false, true, true, false, false);

    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>, std::vector<pattern_mining_result<FastDatalessClause>>> values;
    for (const auto& ref2 : g.Phi) {
        if (ref2.support_declarative_pattern == 1.0) {
            values.first.emplace_back(ref2);
        }
        else {
            values.second.emplace_back(ref2);
        }
    }

    return values;
}

TEST_CASE("mining_tests2: exists_absence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "exists_absence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Exists";
    to_find.clause.left = "a";
    to_find.clause.n = 3;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    pattern_mining_result<FastDatalessClause> to_find_1;
    to_find_1.clause.casusu = "Absence";
    to_find_1.clause.left = "a";
    to_find_1.clause.n = 4;
    to_find_1.support_generating_original_pattern = 1.0;
    to_find_1.support_declarative_pattern = 1.0;
    to_find_1.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find_1) != values.first.end());
}


TEST_CASE("mining_tests2: init") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "init.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Init";
    to_find.clause.left = "a";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests2: end") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "end.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "End";
    to_find.clause.left = "a";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests2: precedence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "precedence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Precedence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}