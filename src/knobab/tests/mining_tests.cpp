//
// Created by sam on 12/07/23.
//


#include <knobab/server/dataStructures/data_loader.h>
#include "knobab/server/query_manager/ServerQueryManager.h"
#include <iostream>
#include <functional>
#include "yaucl/learning/DecisionTree.h"
#include "yaucl/bpm/structures/commons/DeclareDataAware.h"
#include "knobab/server/query_manager/Environment.h"
#include "args.hxx"
#include "knobab/mining/bolt2.h"
#include <catch2/catch_test_macros.hpp>

#define STRING(s) #s
#define TEST(a,b)       TEST_CASE(a ## b)
#define ASSERT_TRUE(c)  REQUIRE(c)

double supp = 0.05;
const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
const std::string query_plan = "queryplan \"nfmcp23\" {\n"
                                 "     template \"Init\"                   := INIT  activation\n"
                                 "     template \"End\"                    := END activation\n"
                                 "     template \"Exists\"                := (EXISTS $ activation)\n"
                                 "     template \"Exists1\"                := (EXISTS 1 activation)\n"
                                 "     template \"Absence\"               := ABSENCE $ activation\n"
                                 "     template \"Absence1\"               := ABSENCE 1 activation\n"
                                 "     template \"Absence2\"               := ABSENCE 2 activation\n"
                                 "     template \"Precedence\" args 2      := ((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)\n"
                                 "     template \"ChainPrecedence\" args 2 := G(((LAST OR t (NEXT EXISTS ~ 1 t #1))) OR t ((NEXT EXISTS 1 t #1 activation) AND t THETA INV (EXISTS 1 t #2 target) ))\n"
                                 "     template \"Choice\" args 2          := (EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)\n"
                                 "     template \"Response\" args 2        := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )\n"
                                 "     template \"ChainResponse\" args 2   := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                                 "     template \"RespExistence\" args 2   := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target)))\n"
                                 "     template \"ExclChoice\" args 2       := ((EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)) AND ((ABSENCE 1 #1) OR (ABSENCE 1 #2))\n"
                                 "     template \"CoExistence\" args 2     := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target))) AND ( ((ABSENCE 1 #2)) OR ((EXISTS 1 #2 activation) AND THETA INV (EXISTS 1 #1 target)))\n"
                                 "     template \"NotCoExistence\" args 2  := ~ ((EXISTS 1 t #1 activation) AND THETA (EXISTS 1 t #2 target)) PRESERVE\n"
                                 "\n"
                                 "     template \"Succession\" args 2      := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )) AND (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 target)) OR (ABSENCE 1 #2))\n"
                                 "     template \"NegSuccession\" args 2   := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Gt  (EXISTS ~ 1 t #2)) ))\n"
                                 "     template \"ChainSuccession\" args 2 := G( (((LAST OR t (NEXT EXISTS ~ 1 t #2))) OR t ((NEXT EXISTS 1 t #2 activation) AND t THETA INV (EXISTS 1 t #1 target))) AND t\n"
                                 "                                             ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                                 "                                           )\n"
                                 "     template \"AltResponse\" args 2     := G ( (EXISTS ~ 1 t #1) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT ((EXISTS ~ 1 t #1) U t (EXISTS 1 t #2 target)) )))\n"
                                 "     template \"AltPrecedence\" args 2   := (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)) AND\n"
                                 "                                           (G(((EXISTS ~ 1 t #1)) OR t (((EXISTS 1 t #1 activation)) AND t THETA (NEXT (((EXISTS  ~ 1 t #1) U t (EXISTS 1 t #2 target)) OR t (G t (EXISTS  ~ 1 t #1))))  )))\n"
                                 "}";

#include <filesystem>
auto folder = std::filesystem::current_path().parent_path() / "data" / "testing" / "mining";

static inline std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> load_and_return(const std::filesystem::path& path) {
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

    sqm.runQuery(query_plan);

    std::vector<DeclareDataAware> VVV;
    auto result = bolt2(
            sqm.multiple_logs[world_file_to_load].db, supp, false, true, true, false, false);

    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>, std::vector<pattern_mining_result<FastDatalessClause>>> values;
    for (const auto& ref2 : result.first) {
        if (ref2.support_declarative_pattern == 1.0) {
            values.first.emplace_back(ref2);
        }
        else {
            values.second.emplace_back(ref2);
        }
    }

    return values;
}

TEST_CASE("correct_mining") {
    auto path =  std::filesystem::current_path().parent_path() / "data"/"benchmarking"/"completed"/"1000_10_10(1).xes";
    auto world_file_to_load = path.string();
    ServerQueryManager sqm;
    std::stringstream ss;
    std::stringstream ss2;

    ss << "load "
       << log_parse_format_type.at((size_t)XES1)
       << " "
       << std::quoted(world_file_to_load)
       <<  " with data as "
       << std::quoted(world_file_to_load);
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan);

    std::vector<DeclareDataAware> VVV;
    auto result = bolt2(
            sqm.multiple_logs[world_file_to_load].db, 0.1, false, true, true, false, false);

    ss << "model-check declare " << std::endl;
    std::vector<pattern_mining_result<FastDatalessClause>> values;
    for (const auto& ref2 : result.first) {
        if (ref2.support_declarative_pattern == 1.0) {
            values.emplace_back(ref2);
            ss << "\t" << ref2.clause << std::endl;
        }
    }

    ss << " using \"PerDeclareSupport\" over " << std::quoted(world_file_to_load) << std::endl;
    ss << " plan \"nfmcp23\" "  << std::endl;
    ss << " with operators \"Hybrid\" ";

    std::string a,b;
    std::tie(a,b) = sqm.runQuery(ss.str());
    auto declare_support = nlohmann::json::parse(a)["PerDeclareSupport"].get<std::vector<double>>();
    ss.str(std::string());
    ss.clear();
    ASSERT_TRUE(declare_support.size() == values.size());
    for (double val : declare_support)
        ASSERT_TRUE((val-1.0)<= std::numeric_limits<double>::epsilon());
}

TEST_CASE("mining_tests: exists_absence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "exists_absence.tab");

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
}

TEST_CASE("mining_tests: init") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "init.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Init";
    to_find.clause.left = "a";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: end") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "end.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "End";
    to_find.clause.left = "a";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: precedence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "precedence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Precedence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.5;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: precedence restrictive") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "precedence_restrict_supp.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Precedence";
    to_find.clause.left = "A";
    to_find.clause.right = "B";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 4.0/18.0;
    to_find.support_declarative_pattern = 16.0/18.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 9.0 / 11.0;

    std::cout << values.second << std::endl;
    auto it = std::find(values.second.begin(), values.second.end(), to_find);
    ASSERT_TRUE(it != values.second.end());
    ASSERT_TRUE(it->restrictive_support_declarative_pattern == (0.5));
}

TEST_CASE("mining_tests: precedence count_OK") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "precedence_restrict_supp_countok.tab");
// Basically, this should not throw the assertion in the Bolt2Branch
}

TEST_CASE("mining_tests: chain_precedence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "chain_precedence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainPrecedence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.75;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: chain_precedence_force_right_branch") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "chain_precedence_force_right_branch.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainPrecedence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.8;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: choice") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "choice.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Choice";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.25;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

////TODO Below is a case where a Choice(A,B) would be better than any more refined clauses, but we aren't sure we want to add it
////TEST_CASE("mining_tests: choice_edge_case") {
////    std::vector<DeclareDataAware> abs_supp = load_and_return("/home/sam/Documents/Repositories/Codebases/knobab/data/testing/mining/choice_2.tab");
////
////    DeclareDataAware to_find;
////    to_find.casusu = "Choice";
////    to_find.left = "a";
////    to_find.right = "b";
////    to_find.n = 1;
////
////    ASSERT_TRUE(std::find(abs_supp.begin(), abs_supp.end(), to_find) != abs_supp.end());

TEST_CASE("mining_tests: response") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "response.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Response";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.75;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: chain_response") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "chain_response.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainResponse";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.8;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: responded_existence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "responded_existence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "RespExistence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.4;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: excl_choice") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "excl_choice.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ExclChoice";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    std::cout << values.first << std::endl;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: coexistence") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "coexistence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "CoExistence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.4;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    pattern_mining_result<FastDatalessClause> to_find_mirror = to_find;
    std::swap(to_find_mirror.clause.left, to_find_mirror.clause.right);

    ASSERT_TRUE(((std::find(values.first.begin(), values.first.end(), to_find) != values.first.end()) || (std::find(values.first.begin(), values.first.end(), to_find_mirror) != values.first.end())) );
}

TEST_CASE("mining_tests: not succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "not_coexistence.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Succession";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 3.0/5.0;
    to_find.support_declarative_pattern = 3.0/5.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 3.0/5.0;
    ASSERT_TRUE(((std::find(values.second.begin(), values.second.end(), to_find) == values.second.end())));

    to_find.clause.casusu = "Precedence";
    to_find.support_generating_original_pattern = 3.0/5.0;
    to_find.support_declarative_pattern = 4.0/5.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 3.0/4.0;
    ASSERT_TRUE(((std::find(values.second.begin(), values.second.end(), to_find) != values.second.end())));

    to_find.clause.casusu = "Response";
    ASSERT_TRUE(((std::find(values.second.begin(), values.second.end(), to_find) != values.second.end())));
}


TEST_CASE("mining_tests: surround") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "surround.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Surround";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: surround_not_chain_succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "surround_not_chain_succession.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Surround";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());

    pattern_mining_result<FastDatalessClause> to_find_1 = to_find;
    to_find_1.clause.casusu = "ChainSuccession";

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find_1) == values.first.end());
}

TEST_CASE("mining_tests: surround_chain_succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "surround_chain_succession.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Surround";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());

    pattern_mining_result<FastDatalessClause> to_find_1 = to_find;
    to_find_1.clause.casusu = "ChainSuccession";

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find_1) != values.first.end());
}

TEST_CASE("mining_tests: not_chain_succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "not_chain_succession.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainSuccession";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = (3.0/7.0);
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 0.6;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) == values.first.end());
}

TEST_CASE("mining_tests: succession") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "succession.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Succession";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}

TEST_CASE("mining_tests: no duplicate surround") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "no_duplicate_surround.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "Surround";
    to_find.clause.left = "A";
    to_find.clause.right = "B";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 1.0;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;
    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
    to_find.clause.casusu = "ChainPrecedence";
    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) == values.first.end());
}

TEST_CASE("mining_tests: chain_precedence_mirror") {
    std::pair<std::vector<pattern_mining_result<FastDatalessClause>>,std::vector<pattern_mining_result<FastDatalessClause>>> values =
            load_and_return(folder / "chain_precedence_mirror.tab");

    pattern_mining_result<FastDatalessClause> to_find;
    to_find.clause.casusu = "ChainPrecedence";
    to_find.clause.left = "a";
    to_find.clause.right = "b";
    to_find.clause.n = 1;
    to_find.support_generating_original_pattern = 0.5;
    to_find.support_declarative_pattern = 1.0;
    to_find.restrictive_confidence_plus_declarative_pattern = 1.0;

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());

    std::swap(to_find.clause.left, to_find.clause.right);

    ASSERT_TRUE(std::find(values.first.begin(), values.first.end(), to_find) != values.first.end());
}