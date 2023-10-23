#include <args.hxx>
#include "yaucl/bpm/structures/log/data_loader.h"
#include "ServerQueryManager.h"

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

int main(int argc, char** argv) {

    const std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
    args::ArgumentParser parser("knobab2-benchmarking", "This file provides a minimum benchmarking utility: loading data and then specification checking");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Positional<std::string> log(parser, "log", "The log file for loaidng the knowledge base");
    args::Positional<std::string> benchmark(parser, "benchmark", "The benchmarking csv file");
    args::Positional<size_t> N_block(parser, "block", "The maximum number of clauses to run per time");
    args::Flag  dataless(parser, "dataless", "Disable payload loading", {'d', "dataless"});
    args::PositionalList<std::string> specification(parser, "spec", "The specification file in powerdecl file");

    std::unordered_map<std::string, log_data_format> metric_map{
            {"XES", XES1},
            {"HRF", HUMAN_READABLE_YAUCL},
            {"TAB", TAB_SEPARATED_EVENTS}};
    args::MapFlag<std::string, log_data_format> logformat(parser, "f", "Format for the log", {'f', "format"}, metric_map);


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

    ServerQueryManager mgr;
    log_data_format format = TAB_SEPARATED_EVENTS;
    std::string log_file, bench_file;
    std::vector<std::string> spec_file;
    bool hasData = true;
    size_t N = 10;
    if (logformat) {
        format = args::get(logformat);
    }
    if (log) {
        log_file = args::get(log);
    }
    if (!std::filesystem::exists(log_file))
        throw std::runtime_error("ERROR: log_file does not exists");
    if (dataless) {
        hasData = false;
    }
    if (specification) {
        spec_file = args::get(specification);
    }
    if (benchmark) {
        bench_file = args::get(benchmark);
    }
    if (N_block) {
        N = args::get(N_block);
    }
    ServerQueryManager sqm;
    std::stringstream ss;

    std::cout << "loading log..." << std::endl;
    // Loading the data
    ss << "load "
       << log_parse_format_type.at((size_t)format)
       << " "
       << std::quoted(log_file);
    if (hasData)
        ss << " with data ";
    ss  <<  " no stats as \"log\"";
    auto tmp = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    sqm.runQuery(query_plan);

    // Dumping the benchmark file
    std::cout << "dumping csv..." << std::endl;
    ss << "benchmarking-log " << std::quoted(bench_file);
    sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();

    for (const auto& s : spec_file) {
        if (!std::filesystem::exists(s))
            throw std::runtime_error("ERROR: spec_file does not exists");
        std::cout << "loading model file..." << std::endl;
        std::vector<DeclareDataAware> binary_clauses;
        {
            std::ifstream t(s);
            std::stringstream buffer;
            buffer << t.rdbuf();
            auto sss = buffer.str();
            if ((!sss.starts_with("ff")) && (!sss.starts_with("tt"))) {
                std::ifstream model{s};
                binary_clauses = sqm.loadModel(model);
            }
        }

        std::cout << "model checking " << std::quoted(s) << ": " << std::endl;
        if (binary_clauses.empty()) {
            ss << "model-check declare " << std::endl;
            ss << " using \"Nothing\" over \"log\" "  << std::endl;
            ss << " plan \"nfmcp23\" "  << std::endl;
            ss << " with operators \"Hybrid\" ";
            std::string a,b;
            std::tie(a,b) = sqm.runQuery(ss.str());
            ss.str(std::string());
            ss.clear();
            sqm.infos.back().model_filename = s;

            // Dumping the benchmark file
            std::cout << "dumping csv(empty)..." << std::endl;
            ss << "benchmarking-log " << std::quoted(bench_file);
            sqm.runQuery(ss.str());
            ss.str(std::string());
            ss.clear();
        } else {
            for (const std::vector<DeclareDataAware>& x : chunker(binary_clauses, N)) {
                ss << "model-check declare " << std::endl;
                for (const auto& d : x) {
                    ss << "\t" << d << std::endl;
                }
                ss << " using \"Nothing\" over \"log\" "  << std::endl;
                ss << " plan \"nfmcp23\" "  << std::endl;
                ss << " with operators \"Hybrid\" ";
                std::string a,b;
                std::tie(a,b) = sqm.runQuery(ss.str());
                ss.str(std::string());
                ss.clear();
                sqm.infos.back().model_filename = s;

                // Dumping the benchmark file
                std::cout << "dumping csv..." << std::endl;
                ss << "benchmarking-log " << std::quoted(bench_file);
                sqm.runQuery(ss.str());
                ss.str(std::string());
                ss.clear();
            }
        }

    }



}