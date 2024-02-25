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


const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};

auto folder = std::filesystem::current_path().parent_path() / "data" / "testing" / "mining";
static inline std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> load_and_return_polyadic(const std::filesystem::path& path, double supp = 0.05) {
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

TEST_CASE("mining_tests2: precedence restrictive") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "precedence_restrict_supp.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Precedence";
    to_find.clause.left = "A";
    to_find.clause.right = "B";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern =-1;
    to_find.support_declarative_pattern = 16.0/18.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 9.0 / 11.0;

    auto it = std::find(values.second.begin(), values.second.end(), to_find);
    ASSERT_TRUE(it != values.second.end());
//    ASSERT_TRUE(it->restrictive_support_declarative_pattern == (0.5));
}

TEST_CASE("mining_tests2: chain_precedence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "chain_precedence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainPrecedence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests2: chain_precedence_force_right_branch") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "chain_precedence_force_right_branch.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainPrecedence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests2: choice") {
    {
        std::pair<std::vector<pattern_mining_result<FastDatalessClause>>, std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "choice.tab", 0.9);

        pattern_mining_result<FastDatalessClause> to_find;
        to_find.clause.casusu = "Choice";
        to_find.clause.left = "a";
        to_find.clause.right = "b";
        to_find.clause.n = 1;
        to_find.support_generating_original_pattern = 0;
        to_find.support_declarative_pattern = 1.0;
        to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

        pattern_mining_result<FastDatalessClause> to_find2;
        to_find2.clause.casusu = "Choice";
        to_find2.clause.left = "b";
        to_find2.clause.right = "a";
        to_find2.clause.n = 1;
        to_find2.support_generating_original_pattern = 0;
        to_find2.support_declarative_pattern = 1.0;
        to_find2.restrictive_confidence_plus_declarative_pattern = 1.0;

        ASSERT_TRUE((std::find(values.first.begin(), values.first.end(), to_find) != values.first.end()) !=
                    (std::find(values.first.begin(), values.first.end(), to_find2) != values.first.end()));
    }
    {
        std::pair<std::vector<pattern_mining_result<FastDatalessClause>>, std::vector<pattern_mining_result<FastDatalessClause>>> values =
                load_and_return_polyadic(folder / "choice.tab");

        pattern_mining_result<FastDatalessClause> to_find;
        to_find.clause.casusu = "Choice";
        to_find.clause.left = "a";
        to_find.clause.right = "b";
        to_find.clause.n = 1;
        to_find.support_generating_original_pattern = -1;
        to_find.support_declarative_pattern = 1.0;
        to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

        pattern_mining_result<FastDatalessClause> to_find2;
        to_find2.clause.casusu = "Choice";
        to_find2.clause.left = "b";
        to_find2.clause.right = "a";
        to_find2.clause.n = 1;
        to_find2.support_generating_original_pattern = -1;
        to_find2.support_declarative_pattern = 1.0;
        to_find2.restrictive_confidence_plus_declarative_pattern = 1.0;

        ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) == values.first.end());
        ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find2) == values.first.end());
    }
}

TEST_CASE("mining_tests2: response") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "response.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Response";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests2: chain_response") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "chain_response.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainResponse";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests2: responded_existence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "responded_existence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "RespExistence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests2: excl_choice") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "excl_choice.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ExclChoice";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    std::cout << values.first << std::endl;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}
TEST_CASE("mining_tests2: coexistence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "coexistence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "CoExistence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    pattern_mining_result<FastDatalessClause> to_find_mirror = to_find;
    std::swap(to_find_mirror.clause.left, to_find_mirror.clause.right);

    ASSERT_TRUE(((std::find(values.first.begin(), values.first.end(), to_find) != values.first.end()) || (std::find(values.first.begin(), values.first.end(), to_find_mirror) != values.first.end())) );
}

TEST_CASE("mining_tests2: not succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "not_coexistence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Succession";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 3.0/5.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 3.0/5.0;
    ASSERT_TRUE(((std::find(values.second.begin(), values.second.end(), to_find) == values.second.end())));

    to_find.clause.casusu = "Precedence";
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 4.0/5.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 4.0/5.0;
    ASSERT_TRUE(((std::find(values.second.begin(), values.second.end(), to_find) != values.second.end())));

    to_find.clause.casusu = "Response";
    to_find.restrictive_confidence_plus_declarative_pattern = 3.0/4.0;
    ASSERT_TRUE(((std::find(values.second.begin(), values.second.end(), to_find) != values.second.end())));
}


//TEST_CASE("mining_tests: surround") {
//    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
//            load_and_return(folder / "surround.tab");
//
//    pattern_mining_result<FastDatalessClause> to_find;
//    to_find.clause.casusu = "Surround";
//    to_find.clause.left = "a";
//    to_find.clause.right = "b";
//    to_find.clause.n = 1;
//    to_find.support_generating_original_pattern = 1.0;
//    to_find.support_declarative_pattern = 1.0;
//    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;
//
//    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
//}

TEST_CASE("mining_tests2: surround_not_chain_succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "surround_not_chain_succession.tab");

    pattern_mining_result<FastDatalessClause> to_find;
//    to_find.clause.casusu = "Surround";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

//    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());

    pattern_mining_result<FastDatalessClause> to_find_1 = to_find;
    to_find_1.clause.casusu = "ChainSuccession";

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find_1) == values.first.end());
}

TEST_CASE("mining_tests2: surround_chain_succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "surround_chain_succession.tab");

    pattern_mining_result<FastDatalessClause> to_find;
//    to_find.clause.casusu = "Surround";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

//    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());

    pattern_mining_result<FastDatalessClause> to_find_1 = to_find;
    to_find_1.clause.casusu = "ChainSuccession";

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find_1) != values.first.end());
}

TEST_CASE("mining_tests2: not_chain_succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "not_chain_succession.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainSuccession";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 0.6;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) == values.first.end());
}

TEST_CASE("mining_tests2: succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "succession.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Succession";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());

    to_find.clause.casusu = "Response";
    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) == values.first.end());

    to_find.clause.casusu = "Precednce";
    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) == values.first.end());

    to_find.clause.casusu = "RespExistence";
    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) == values.first.end());
}

//TEST_CASE("mining_tests2: no duplicate surround") {
//    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
//            load_and_return(folder / "no_duplicate_surround.tab");
//
//    pattern_mining_result<FastDatalessClause> to_find;
//    to_find.clause.casusu = "Surround";
//    to_find.clause.left = "A";
//    to_find.clause.right = "B";
//    to_find.clause.n = 1;
//    to_find.support_generating_original_pattern = 1.0;
//    to_find.support_declarative_pattern = 1.0;
//    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;
//    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
//    to_find.clause.casusu = "ChainPrecedence";
//    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) == values.first.end());
//}

TEST_CASE("mining_tests2: chain_precedence_mirror") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return_polyadic(folder / "chain_precedence_mirror.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainPrecedence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 2;
    to_find.support_generating_original_pattern = -1;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
    std::swap(to_find.clause.left, to_find.clause.right);
    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}