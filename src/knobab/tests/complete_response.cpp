//
// Created by giacomo on 28/08/23.
//

#include <knobab/server/dataStructures/data_loader.h>
#include "knobab/server/query_manager/ServerQueryManager.h"
#include <iostream>
#include <functional>
#include "yaucl/learning/DecisionTree.h"
#include "yaucl/bpm/structures/commons/DeclareDataAware.h"
#include "knobab/server/query_manager/Environment.h"
#include "args.hxx"
#include <catch2/catch_test_macros.hpp>

#define STRING(s) #s
#define TEST(a,b)       TEST_CASE(a ## b)
#define ASSERT_TRUE(c)  REQUIRE(c)


#include <vector>
#include <cassert>
#include <yaucl/structures/set_operations.h>

#include <fstream>
TEST_CASE("complete_response_support"){
//int main(){
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
    auto path =  std::filesystem::current_path().parent_path() / "data"/"testing"/"complete_response"/"log_response.txt";
    auto world_file_to_load = path.string();
    ServerQueryManager sqm;
    std::stringstream ss;
    std::stringstream ss2;

    ss << "load "
       << log_parse_format_type.at((size_t)HUMAN_READABLE_YAUCL)
       << " "
       << std::quoted(world_file_to_load)
       <<  " with data as \"log\"";
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan);

    ss << "model-check declare \"Response\"(\"A\", true, \"A\", true)\n"
          "\"Response\"(\"A\", true, \"B\", true)\n"
          "\"Response\"(\"A\", true, \"C\", true)\n"
          "\"Response\"(\"B\", true, \"A\", true)\n"
          "\"Response\"(\"B\", true, \"B\", true)\n"
          "\"Response\"(\"B\", true, \"C\", true)\n"
          "\"Response\"(\"C\", true, \"A\", true)\n"
          "\"Response\"(\"C\", true, \"B\", true)\n"
          "\"Response\"(\"C\", true, \"C\", true)" << std::endl;
    ss << " using \"PerDeclareSupport\" over \"log\"" << std::endl;
    ss << " plan \"nfmcp23\" "  << std::endl;
    ss << " with operators \"Hybrid\" ";

    std::string a,b;
    std::tie(a,b) = sqm.runQuery(ss.str());
    auto declare_support = nlohmann::json::parse(a)["PerDeclareSupport"].get<std::vector<double>>();
    ss.str(std::string());
    ss.clear();
    ASSERT_TRUE(declare_support.size() == 9);
    ASSERT_TRUE(declare_support ==  (std::vector<double>{1.0,1.0,1.0/9.0,0.0,1.0,0.0,8.0/9.0,1.0,1.0}));
}

