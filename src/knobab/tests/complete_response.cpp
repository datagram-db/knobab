//
// Created by giacomo on 28/08/23.
//

#include "yaucl/bpm/structures/log/data_loader.h"
#include "knobab/server/query_manager/ServerQueryManager.h"
#include <iostream>
#include <functional>
#include "yaucl/learning/DecisionTree.h"
#include "args.hxx"
#include <catch2/catch_test_macros.hpp>

#define STRING(s) #s
#define TEST(a,b)       TEST_CASE(a ## b)
#define ASSERT_TRUE(c)  REQUIRE(c)

#include <vector>
#include <fstream>

//const std::string query_plan = "queryplan \"nfmcp23\" {\n"
//                               "     template \"Init\"                   := INIT  activation\n"
//                               "     template \"End\"                    := END activation\n"
//                               "     template \"Exists\"                := (EXISTS $ activation)\n"
//                               "     template \"Exists1\"                := (EXISTS 1 activation)\n"
//                               "     template \"Absence\"               := ABSENCE $ activation\n"
//                               "     template \"Absence1\"               := ABSENCE 1 activation\n"
//                               "     template \"Absence2\"               := ABSENCE 2 activation\n"
//                               "     template \"Precedence\" args 2      := ((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)\n"
//                               "     template \"ChainPrecedence\" args 2 := G(((LAST OR t (NEXT EXISTS ~ 1 t #1))) OR t ((NEXT EXISTS 1 t #1 activation) AND t THETA INV (EXISTS 1 t #2 target) ))\n"
//                               "     template \"Choice\" args 2          := (EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)\n"
//                               "     template \"Response\" args 2        := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )\n"
//                               "     template \"ChainResponse\" args 2   := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
//                               "     template \"RespExistence\" args 2   := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target)))\n"
//                               "     template \"ExclChoice\" args 2       := ((EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)) AND ((ABSENCE 1 #1) OR (ABSENCE 1 #2))\n"
//                               "     template \"CoExistence\" args 2     := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target))) AND ( ((ABSENCE 1 #2)) OR ((EXISTS 1 #2 activation) AND THETA INV (EXISTS 1 #1 target)))\n"
//                               "     template \"NotCoExistence\" args 2  := ~ ((EXISTS 1 t #1 activation) AND THETA (EXISTS 1 t #2 target)) PRESERVE\n"
//                               "\n"
//                               "     template \"Succession\" args 2      := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )) AND (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 target)) OR (ABSENCE 1 #2))\n"
//                               "     template \"NegSuccession\" args 2   := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Gt  (EXISTS ~ 1 t #2)) ))\n"
//                               "     template \"ChainSuccession\" args 2 := G( (((LAST OR t (NEXT EXISTS ~ 1 t #2))) OR t ((NEXT EXISTS 1 t #2 activation) AND t THETA INV (EXISTS 1 t #1 target))) AND t\n"
//                               "                                             ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
//                               "                                           )\n"
//                               "     template \"AltResponse\" args 2     := G ( (EXISTS ~ 1 t #1) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT ((EXISTS ~ 1 t #1) U t (EXISTS 1 t #2 target)) )))\n"
//                               "     template \"AltPrecedence\" args 2   := (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)) AND\n"
//                               "                                           (G(((EXISTS ~ 1 t #2)) OR t (((EXISTS 1 t #2 target)) AND t THETA INV (NEXT (((EXISTS  ~ 1 t #2) U t (EXISTS 1 t #1 activation)) OR t (G t (EXISTS  ~ 1 t #2))))  )))\n"
//                               "}";


const std::string query_plan_novel = "queryplan \"edbt24\" {\n"
                               "     template \"Init\"                   := INIT  activation\n"
                               "     template \"End\"                    := END activation\n"
                               "     template \"Exists\"                := (EXISTS $ activation)\n"
                               "     template \"Exists1\"                := (EXISTS 1 activation)\n"
                               "     template \"Absence\"               := ABSENCE $ activation\n"
                               "     template \"Absence1\"               := ABSENCE 1 activation\n"
                               "     template \"Absence2\"               := ABSENCE 2 activation\n"
                               "     template \"Precedence\" args 2      := ((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)\n"
                               "     template \"ChainPrecedence\" args 2 := G (((LAST OR t (NEXT EXISTS ~ 1 t #1))) OR t ((EXISTS 1 t #1 activation) XB THETA INV #2 target ))\n"
                               "     template \"Choice\" args 2          := (EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)\n"
                               "     template \"Response\" args 2        := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )\n"
                               "     template \"ChainResponse\" args 2   := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) BX THETA  #2 target))\n"
                               "     template \"RespExistence\" args 2   := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target)))\n"
                               "     template \"ExclChoice\" args 2       := ((EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)) AND ((ABSENCE 1 #1) OR (ABSENCE 1 #2))\n"
                               "     template \"CoExistence\" args 2     := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target))) AND ( ((ABSENCE 1 #2)) OR ((EXISTS 1 #2 activation) AND THETA INV (EXISTS 1 #1 target)))\n"
                               "     template \"NotCoExistence\" args 2  := ~ ((EXISTS 1 t #1 activation) AND THETA (EXISTS 1 t #2 target)) PRESERVE\n"
                               "\n"
                               "     template \"Succession\" args 2      := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )) AND (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 target)) OR (ABSENCE 1 #2))\n"
                               "     template \"NegSuccession\" args 2   := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Gt  (EXISTS ~ 1 t #2)) ))\n"
                               "     template \"ChainSuccession\" args 2 := G ( (((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) BX THETA  #2 target)) AND t (((LAST OR t (NEXT EXISTS ~ 1 t #2))) OR t ((EXISTS 1 t #2 activation) XB THETA #1 target )) )\n"
                               "     template \"AltResponse\" args 2     := G ( (EXISTS ~ 1 t #1) OR t ((EXISTS 1 t #1 activation) &Ft!X THETA (((EXISTS 1 t #2 target)))))\n"
                               "     template \"AltPrecedence\" args 2   := (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)) AND\n"
                               "                                           (G(((EXISTS ~ 1 t #2)) OR t ((EXISTS 1 t #2 target) &Ft!WX THETA INV (((EXISTS 1 t #1 activation))))))\n"
                               "}";

TEST_CASE("complete_response_support"){
//int main(){
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
    auto path =  std::filesystem::current_path().parent_path() / "data"/"testing"/"complete_response"/"log_response.txt";
    auto world_file_to_load = path.string();
    ServerQueryManager sqm;
    std::stringstream ss;

    ss << "load "
       << log_parse_format_type.at((size_t)HUMAN_READABLE_YAUCL)
       << " "
       << std::quoted(world_file_to_load)
       <<  " with data as \"log\"";
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan_novel);

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
    ss << " plan \"edbt24\" "  << std::endl;
    ss << " with operators \"Hybrid\" ";

    std::string a,b;
    std::tie(a,b) = sqm.runQuery(ss.str());
    auto declare_support = nlohmann::json::parse(a)["PerDeclareSupport"].get<std::vector<double>>();
    ss.str(std::string());
    ss.clear();
    ASSERT_TRUE(declare_support.size() == 9);
    ASSERT_TRUE(declare_support ==  (std::vector<double>{1.0,1.0,1.0/9.0,0.0,1.0,0.0,8.0/9.0,1.0,1.0}));
}

/*
 * Turn A into a string literal without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define STRINGIZE_NX(A) #A

#define TEST_CASE_PER_OPERATOR(name, op) TEST_CASE(STRINGIZE_NX(base ## name ## op)) {\
    size_t pos, neg;\
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};\
\
    auto root_folder =  std::filesystem::current_path().parent_path();\
    std::string base{#name}; \
    std::string operators{#op}; \
    std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;\
    auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");\
    ServerQueryManager sqm;\
    std::stringstream ss;\
    std::ifstream fs{sizes};\
        fs >> pos >> neg;\
    auto declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();\
    std::ifstream t(declare_file_path);\
    std::stringstream buffer;\
    buffer << t.rdbuf();\
\
    ss << "load "\
       << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)\
       << " "\
       << std::quoted(curr.string())\
       <<  " no stats as "\
       << std::quoted(base);\
    auto tmp = sqm.runQuery(ss.str());\
    ss.str(std::string());\
    ss.clear();\
\
    sqm.runQuery(query_plan_novel);\
    ss << "model-check declare " << buffer.str() << std::endl;\
    ss << " using \"TraceMaximumSatisfiability\" over " << std::quoted(base) << std::endl;\
    ss << " plan \"edbt24\" "  << std::endl;\
    ss << " with operators  " << std::quoted(operators);\
    std::string a,b;\
    std::tie(a,b) = sqm.runQuery(ss.str());\
    auto js = nlohmann::json::parse(a);\
    std::vector<double> max_sat_per_trace = js["TraceMaximumSatisfiability"].get<std::vector<double>>();\
    for (size_t i = 0; i<max_sat_per_trace.size(); i++) {\
        if (i<pos)\
            ASSERT_TRUE(max_sat_per_trace.at(i)== 1.0);\
        else\
            ASSERT_TRUE(max_sat_per_trace.at(i)== 0.0);\
    }\
};

#define TEST_CASE_PER_NOVEL_OPERATOR(name, op) TEST_CASE(STRINGIZE_NX(novel ## name ## op)) {\
    size_t pos, neg;\
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};\
    auto root_folder =  std::filesystem::current_path().parent_path();\
    std::string base{#name}; \
    std::string operators{#op}; \
    std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;\
    auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");\
    ServerQueryManager sqm;\
    std::stringstream ss;\
    std::ifstream fs{sizes};\
        fs >> pos >> neg;\
    auto declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();\
    std::ifstream t(declare_file_path);\
    std::stringstream buffer;\
    buffer << t.rdbuf();\
\
    ss << "load "\
       << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)\
       << " "\
       << std::quoted(curr.string())\
       <<  " as "\
       << std::quoted(base);\
    auto tmp = sqm.runQuery(ss.str());\
    ss.str(std::string());\
    ss.clear();\
\
    sqm.runQuery(query_plan_novel);\
    ss << "model-check declare " << buffer.str() << std::endl;\
    ss << " using \"TraceMaximumSatisfiability\" over " << std::quoted(base) << std::endl;\
    ss << " plan \"edbt24\" "  << std::endl;\
    ss << " with operators  " << std::quoted(operators);\
    std::string a,b;\
    std::tie(a,b) = sqm.runQuery(ss.str());                                                  \
    auto js = nlohmann::json::parse(a);\
    std::vector<double> max_sat_per_trace = js["TraceMaximumSatisfiability"].get<std::vector<double>>();\                                                                                    \
    std::vector<double> max_sat_per_trace = nlohmann::json::parse(a)["TraceMaximumSatisfiability"].get<std::vector<double>>();\
    for (size_t i = 0; i<max_sat_per_trace.size(); i++) {\
        if (i<pos)\
            ASSERT_TRUE(max_sat_per_trace.at(i)== 1.0);\
        else\
            ASSERT_TRUE(max_sat_per_trace.at(i)== 0.0);\
    }\
};

TEST_CASE_PER_OPERATOR(AltPrecedence, Hybrid)
TEST_CASE_PER_OPERATOR(AltResponse, Hybrid)
/// @author: Giacomo Bergami
TEST_CASE("novelAltResponseHybrid") {
    size_t pos, neg;
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
//    std::ofstream f{"/home/giacomo/b.txt", std::ios_base::app};

    auto root_folder =  std::filesystem::current_path().parent_path();
    std::string base{"AltResponse"};
    std::string operators{"Hybrid"};
    std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;
    auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");

    ServerQueryManager sqm;
    std::stringstream ss;
    std::ifstream fs{sizes};
        fs >> pos >> neg;
    auto declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();
    std::ifstream t(declare_file_path);
    std::stringstream buffer;
    buffer << t.rdbuf();

    ss << "load "
       << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)
       << " "
       << std::quoted(curr.string())
       <<  " no stats as "
       << std::quoted(base);
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan_novel);
    ss << "model-check declare " << buffer.str() << std::endl;
    ss << " using \"TraceMaximumSatisfiability\" over " << std::quoted(base) << std::endl;
    ss << " plan \"edbt24\" "  << std::endl;
    ss << " with operators  " << std::quoted(operators);
    std::string a,b;
    std::tie(a,b) = sqm.runQuery(ss.str());
    auto js = nlohmann::json::parse(a);
    std::vector<double> max_sat_per_trace = js["TraceMaximumSatisfiability"].get<std::vector<double>>();
    for (size_t i = 0; i<max_sat_per_trace.size(); i++) {
//        std::cout << i << " for "<< max_sat_per_trace.size() << std::endl;
        if (i<pos)
            ASSERT_TRUE(max_sat_per_trace.at(i)== 1.0);
        else
            ASSERT_TRUE(max_sat_per_trace.at(i)== 0.0);
    }
}

/// @author: Giacomo Bergami
TEST_CASE("novelChainResponseHybrid") {
    size_t pos, neg;
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
//    std::ofstream f{"/home/giacomo/b.txt", std::ios_base::app};

    auto root_folder =  std::filesystem::current_path().parent_path();
    std::string base{"ChainResponse"};
    std::string operators{"Hybrid"};
    std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;
    auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");

    ServerQueryManager sqm;
    std::stringstream ss;
    std::ifstream fs{sizes};
    fs >> pos >> neg;
    auto declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();
    std::ifstream t(declare_file_path);
    std::stringstream buffer;
    buffer << t.rdbuf();

    ss << "load "
       << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)
       << " "
       << std::quoted(curr.string())
       <<  " no stats as "
       << std::quoted(base);
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan_novel);
    ss << "model-check declare " << buffer.str() << std::endl;
    ss << " using \"TraceMaximumSatisfiability\" over " << std::quoted(base) << std::endl;
    ss << " plan \"edbt24\" "  << std::endl;
    ss << " with operators  " << std::quoted(operators);
    std::string a,b;
    std::tie(a,b) = sqm.runQuery(ss.str());
    auto js = nlohmann::json::parse(a);
    std::vector<double> max_sat_per_trace = js["TraceMaximumSatisfiability"].get<std::vector<double>>();

//    f << base<< " "<<operators <<" : " << js["model_ltlf_query_time"].get<double>() << std::endl;
    for (size_t i = 0; i<max_sat_per_trace.size(); i++) {
//        std::cout << i << " for "<< max_sat_per_trace.size() << std::endl;
        if (i<pos)
            ASSERT_TRUE(max_sat_per_trace.at(i)== 1.0);
        else
            ASSERT_TRUE(max_sat_per_trace.at(i)== 0.0);
    }
}

/// @author: Giacomo Bergami
TEST_CASE("novelChainPrecedenceHybrid") {
    size_t pos, neg;
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
//    std::ofstream f{"/home/giacomo/b.txt", std::ios_base::app};

    auto root_folder =  std::filesystem::current_path().parent_path();
    std::string base{"ChainPrecedence"};
    std::string operators{"Hybrid"};
    std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;
    auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");

    ServerQueryManager sqm;
    std::stringstream ss;
    std::ifstream fs{sizes};
    fs >> pos >> neg;
    auto declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();
    std::ifstream t(declare_file_path);
    std::stringstream buffer;
    buffer << t.rdbuf();

    ss << "load "
       << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)
       << " "
       << std::quoted(curr.string())
       <<  " no stats as "
       << std::quoted(base);
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan_novel);
    ss << "model-check declare " << buffer.str() << std::endl;
    ss << " using \"TraceMaximumSatisfiability\" over " << std::quoted(base) << std::endl;
    ss << " plan \"edbt24\" "  << std::endl;
    ss << " with operators  " << std::quoted(operators);
    std::string a,b;
    std::tie(a,b) = sqm.runQuery(ss.str());
    auto js = nlohmann::json::parse(a);
    std::vector<double> max_sat_per_trace = js["TraceMaximumSatisfiability"].get<std::vector<double>>();

//    f << base<< " "<<operators <<" : " << js["model_ltlf_query_time"].get<double>() << std::endl;
    for (size_t i = 0; i<max_sat_per_trace.size(); i++) {
        if (i<pos) {
            if (max_sat_per_trace.at(i)!= 1.0)
                std::cerr << "HERE" << std::endl;
            ASSERT_TRUE(max_sat_per_trace.at(i)== 1.0);
        }
        else {
            ASSERT_TRUE(max_sat_per_trace.at(i) == 0.0);
        }
    }
}

/// @author: Giacomo Bergami
TEST_CASE("novelAltPrecedenceHybrid") {
    size_t pos, neg;
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
//    std::ofstream f{"/home/giacomo/b.txt", std::ios_base::app};

    auto root_folder =  std::filesystem::current_path().parent_path();
    std::string base{"AltPrecedence"};
    std::string operators{"Hybrid"};
    std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;
    auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");

    ServerQueryManager sqm;
    std::stringstream ss;
    std::ifstream fs{sizes};
    fs >> pos >> neg;
    auto declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();
    std::ifstream t(declare_file_path);
    std::stringstream buffer;
    buffer << t.rdbuf();

    ss << "load "
       << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)
       << " "
       << std::quoted(curr.string())
       <<  " no stats as "
       << std::quoted(base);
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan_novel);
    ss << "model-check declare " << buffer.str() << std::endl;
    ss << " using \"TraceMaximumSatisfiability\" over " << std::quoted(base) << std::endl;
    ss << " plan \"edbt24\" "  << std::endl;
    ss << " with operators  " << std::quoted(operators);
    std::string a,b;
    std::tie(a,b) = sqm.runQuery(ss.str());
    auto js = nlohmann::json::parse(a);
    std::vector<double> max_sat_per_trace = js["TraceMaximumSatisfiability"].get<std::vector<double>>();
    for (size_t i = 0; i<max_sat_per_trace.size(); i++) {
//        std::cout << i << " for "<< max_sat_per_trace.size() << std::endl;
        if (i<pos)
            ASSERT_TRUE(max_sat_per_trace.at(i)== 1.0);
        else
            ASSERT_TRUE(max_sat_per_trace.at(i)== 0.0);
    }
}

//TEST_CASE("novelAltPrecedenceHybrid2") {
//    size_t pos, neg;
//    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
////    std::ofstream f{"/home/giacomo/b.txt", std::ios_base::app};
//
//    auto root_folder =  std::filesystem::current_path().parent_path();
//    std::string base{"AltPrecedence"};
//    std::string operators{"Hybrid"};
//    std::filesystem::path curr = "/home/giacomo/basic.txt";
////    auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");
//
//    ServerQueryManager sqm;
//    std::stringstream ss;
////    std::ifstream fs{sizes};
////    fs >> pos >> neg;
//    auto declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();
//    std::ifstream t(declare_file_path);
//    std::stringstream buffer;
//    buffer << t.rdbuf();
//
//    ss << "load "
//       << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)
//       << " "
//       << std::quoted(curr.string())
//       <<  " no stats as "
//       << std::quoted(base);
//    auto tmp = sqm.runQuery(ss.str());
//    ss.str(std::string());
//    ss.clear();
//
//    sqm.runQuery(query_plan_novel);
//    ss << "model-check declare " << buffer.str() << std::endl;
//    ss << " using \"TraceMaximumSatisfiability\" over " << std::quoted(base) << std::endl;
//    ss << " plan \"edbt24\" "  << std::endl;
//    ss << " with operators  " << std::quoted(operators);
//    std::string a,b;
//    std::tie(a,b) = sqm.runQuery(ss.str());
//    auto js = nlohmann::json::parse(a);
//    std::vector<double> max_sat_per_trace = js["TraceMaximumSatisfiability"].get<std::vector<double>>();
//
//    std::cout << "j" << std::endl;
//
////    for (size_t i = 0; i<max_sat_per_trace.size(); i++) {
//////        std::cout << i << " for "<< max_sat_per_trace.size() << std::endl;
////        if (i<pos)
////            ASSERT_TRUE(max_sat_per_trace.at(i)== 1.0);
////        else
////            ASSERT_TRUE(max_sat_per_trace.at(i)== 0.0);
////    }
//}

TEST_CASE_PER_OPERATOR(ChainPrecedence, Hybrid)
TEST_CASE_PER_OPERATOR(ChainResponse, Hybrid)
TEST_CASE_PER_OPERATOR(ChainSuccession, Hybrid)
TEST_CASE_PER_OPERATOR(Choice, Hybrid)
TEST_CASE_PER_OPERATOR(CoExistence, Hybrid)
TEST_CASE_PER_OPERATOR(Precedence, Hybrid)
TEST_CASE_PER_OPERATOR(RespExistence, Hybrid)
TEST_CASE_PER_OPERATOR(Response, Hybrid)
TEST_CASE_PER_OPERATOR(Succession, Hybrid)

#if 0
/// Bugged test case: it misses the data, which stayed on SamuelAppleby's PC and was not pushed in the repo
TEST_CASE("benchmarking") {
    const uint32_t iter_count = 1;
    const double support = 0.02;
    auto root_folder = std::filesystem::current_path().parent_path();
    std::vector<std::string> base{"AltResponse"};
    std::string operators{"Hybrid"};
    std::string ensemble{"TraceMaximumSatisfiability"};
//    std::string ensemble{"PerDeclareSupport"};
//    std::string ensemble{"Nothing"};
    std::filesystem::path curr = root_folder / "data" / "testing" / "cyber_security" / "all_classes_sampled";
    std::unordered_map<std::string, std::string> plans{
            {query_plan, "nfmcp23"},
            {query_plan_novel, "edbt24"}
    };

    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};

    for(uint32_t i = 0; i < iter_count; ++i) {
        for (const auto& dirEntry : recursive_directory_iterator(curr)) {
            for(const std::string& s : base) {
                ServerQueryManager sqm;
                std::stringstream ss;

                ss << "load "
                   << log_parse_format_type.at((size_t)TAB_SEPARATED_EVENTS)
                   << " "
                   << std::quoted(dirEntry.path().string())
                   <<  " no stats as "
                   << std::quoted(dirEntry.path().string());

                auto tmp = sqm.runQuery(ss.str());
                ss.str(std::string());
                ss.clear();

                Environment& env = sqm.multiple_logs[dirEntry.path().string()];
//                size_t support_int = (size_t)std::ceil((support) * (double)env.db.nAct());
                size_t support_int = 1;

                for(const std::pair<std::string, std::string>& str : plans) {
                    sqm.runQuery(str.first);

                    auto declare_file_path = (root_folder / "data" / "testing" / "declare" / (s+".powerdecl")).string();
                    std::ifstream t(declare_file_path);
                    std::stringstream buffer;
                    buffer << t.rdbuf();

                    ss << "model-check declare " << buffer.str() << std::endl;
//                    ss << "model-check template " << std::quoted(s) << " logtop " << support_int << std::endl;
                    ss << " using \"" << ensemble << "\" over " << std::quoted(dirEntry.path().string()) << std::endl;
                    ss << " plan \"" << str.second << "\" "  << std::endl;
                    ss << " with operators  " << std::quoted(operators);

                    std::string a,b;
                    std::tie(a,b) = sqm.runQuery(ss.str());
                    ss.str(std::string());
                    ss.clear();

                    auto js = nlohmann::json::parse(a);

                    if (env.strategy != Nothing) {
                        std::vector<double> result = js[ensemble].get<std::vector<double>>();

                        for(LoggerInformation& log : sqm.infos) {
                            log.mining_algorithm = str.second;
                            log.iteration_num = i;
                        }
                    }

                    ss << "benchmarking-log " << std::quoted((root_folder / "data" / "testing" / "cyber_security" / "output.csv").string());
                    sqm.runQuery(ss.str());
                    ss.str(std::string());
                    ss.clear();
                }
            }
        }
    }
}
#endif
