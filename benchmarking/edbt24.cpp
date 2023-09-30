/// Giacomo Bergami

#include "knobab/server/query_manager/ServerQueryManager.h"
#include <iostream>
#include <functional>
#include "args.hxx"

#define ASSERT_TRUE(c)  REQUIRE(c)

#include <vector>
#include <fstream>

/// Corresponds to the "Original" set-up in the most recent paper
const std::string query_plan = "queryplan \"mdpi23\" {\n"
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
                               "                                           (G(((EXISTS ~ 1 t #2)) OR t (((EXISTS 1 t #2 target)) AND t THETA INV (NEXT (((EXISTS  ~ 1 t #2) U t (EXISTS 1 t #1 activation)) OR t (G t (EXISTS  ~ 1 t #2))))  )))\n"
                               "}";

/// Corresponds to the "Proposed" set-up in the most recent paper
const std::string query_plan_novel = "queryplan \"edbt24\" {\n"
                                     "     template \"Init\"                   := INIT  activation\n"
                                     "     template \"End\"                    := END activation\n"
                                     "     template \"Exists\"                := (EXISTS $ activation)\n"
                                     "     template \"Exists1\"                := (EXISTS 1 activation)\n"
                                     "     template \"Absence\"               := ABSENCE $ activation\n"
                                     "     template \"Absence1\"               := ABSENCE 1 activation\n"
                                     "     template \"Absence2\"               := ABSENCE 2 activation\n"
                                     "     template \"Precedence\" args 2      := ((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)\n"
                                     "     template \"ChainPrecedence\" args 2 := G(((LAST OR t (NEXT EXISTS ~ 1 t #1))) OR t ((EXISTS 1 t #1 activation) XB THETA INV #2 target ))\n"
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
                                     "     template \"ChainSuccession\" args 2 := G( (((LAST OR t (NEXT EXISTS ~ 1 t #2))) OR t ((NEXT EXISTS 1 t #2 activation) AND t THETA INV (EXISTS 1 t #1 target))) AND t\n"
                                     "                                             ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                                     "                                           )\n"
                                     "     template \"AltResponse\" args 2     := G ( (EXISTS ~ 1 t #1) OR t ((EXISTS 1 t #1 activation) &Ft!X THETA (((EXISTS 1 t #2 target)))))\n"
                                     "     template \"AltPrecedence\" args 2   := (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)) AND\n"
                                     "                                           (G(((EXISTS ~ 1 t #2)) OR t ((EXISTS 1 t #2 target) &Ft!WX THETA INV (((EXISTS 1 t #1 activation))))))\n"
                                     "}";


#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

//std::vector<std::string> query_plans{"nfmcp23"};

void
runModelFile(ServerQueryManager &sqm, std::stringstream &ss, const std::string &qp, const std::filesystem::path &p, size_t N = 10) {
    auto s = absolute(p);
    std::cout << "loading model file: " << p.string() << "... " << std::endl;
    std::vector<DeclareDataAware> binary_clauses;
    {
        std::ifstream t(s);
        std::stringstream buffer;
        buffer << "declare ";
        buffer << t.rdbuf();
        auto sss = buffer.str();
        if ((!sss.starts_with("ff")) && (!sss.starts_with("tt"))) {
            binary_clauses = sqm.loadModel(buffer);
        }
    }

    for (size_t i = 0; i<N; i++) {
        ss << "model-check declare " << std::endl;
        for (const auto& d : binary_clauses) {
            ss << "\t" << d << std::endl;
        }
        ss << " using \"Nothing\" over \"log\" "  << std::endl;
        ss << " plan " << std::quoted(qp) << " " << std::endl;
        ss << " with operators \"Hybrid\" ";
        std::string a,b;
        std::tie(a,b) = sqm.runQuery(ss.str());
        ss.str(std::string());
        ss.clear();
        sqm.infos.back().model_filename = p.stem();
        sqm.infos.back().queries_plan = qp;
    }

    // Dumping the benchmark file
    std::cout << "dumping csv..." << std::endl;
    ss << "benchmarking-log " << std::quoted("benchmark.csv");
    sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();
}

int main(int argc, char* argv[]) {
    args::ArgumentParser parser("knobab2-edbt24-benchmarking", "This file provides a minimum benchmarking utility: loading data and then specification checking");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Positional<std::string> log(parser, "log", "The log file for loaidng the knowledge base");
    args::Positional<std::string> models(parser, "spec", "The specification in DECLARE to be benchmarked");
    args::Positional<std::string> query_plan_choice(parser, "models", "The query plan to be used (mdpi23|edbt24)");
    args::Flag isModelFolder(parser, "isfolder", "Whether the path specified for the model is a folder or not (i.e., a file)", {'f', "isfolder"});
    args::ValueFlag<size_t> nIterations(parser, "niterations", "The number of times to repeat the formal verification task (default=10)", {'n', "N"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    if (!(log)) {
        std::cerr << "ERROR: you need to provide a log file! Run the command with the help format to know more" << std::endl;
        exit(1);
    }
    auto log_file = std::filesystem::path(args::get(log));
    if ((!std::filesystem::exists(log_file)) || (!std::filesystem::is_regular_file(log_file))) {
        std::cerr << "ERROR: the log file that you provided does not exist: Second Time's the Charm.";
        exit(1);
    }
    if (!(models)) {
        std::cerr << "ERROR: you need to provide a specification file! Run the command with the help format to know more" << std::endl;
        exit(1);
    }

    bool isFolder = (isModelFolder);
    auto models_folder = std::filesystem::path(args::get(models));
    if ((!std::filesystem::exists(models_folder)) || (isFolder ? (!std::filesystem::is_directory(models_folder)): (!std::filesystem::is_regular_file(models_folder)))) {
        std::cerr << "ERROR: the model folder that you provided does not exist: Second Time's the Charm.";
        exit(1);
    }
    if (!(query_plan_choice)) {
        std::cerr << "You must specify a query plan (edbt24|mdpi23)";
        exit(1);
    }
    size_t nIter = 10;
    if (nIterations) {
        nIter = args::get(nIterations);
    }
    ServerQueryManager sqm;
    std::stringstream ss;


    ss << "load TAB "
       << std::quoted(args::get(log))
       <<  " no stats as \"log\"";
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan_novel);
    sqm.runQuery(query_plan);

    auto qp = args::get(query_plan_choice);
    {
        if (isFolder) {
            std::vector<std::filesystem::path> paths;
            for(auto& p: fs::directory_iterator(models_folder)) {
                paths.emplace_back(p);
            }
            std::sort(paths.begin(), paths.end());
            for (auto& p : paths) {
                runModelFile(sqm, ss, qp, p, nIter);
            }
        } else {
            runModelFile(sqm, ss, qp, models_folder, nIter);
        }
    }
}