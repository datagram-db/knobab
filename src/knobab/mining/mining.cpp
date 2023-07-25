//
// Created by giacomo on 05/10/22.
//
#include <args.hxx>
#include <knobab/mining/bolt1.h>
#include <knobab/mining/apriori.h>
#include <knobab/mining/bolt2.h>
#include <fstream>
#include "knobab/server/query_manager/ServerQueryManager.h"

enum Algorithm {
    BOLT1,
    BOLT2,
    APRIORI,
    TOP_K_MINING
};

void questo() {
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
//    ss << "\"RespExistence\"( \"d\", true, \"g\", true)   " << std::endl;
//    ss << "\"Absence\"( \"f\", true, 6)  \"Absence\"( \"c\", true, 7)  \"Absence\"( \"b\", true, 3)  " << std::endl;
            ss << "\t" << ref2.clause << std::endl;
        }
    }
//    ss << values.at(11).clause << std::endl;
//    ss << values.at(14).clause << std::endl;
//    ss << values.at(36).clause << std::endl;
//    ss << values.at(38).clause << std::endl;
//    ss << values.at(39).clause << std::endl;
//    auto sstr = ss2.str();
//    exit(1);

    ss << " using \"ReturnTraces\" over " << std::quoted(world_file_to_load) << std::endl;
//    ss << " using \"TraceMaximumSatisfiability\" over " << std::quoted(world_file_to_load) << std::endl;
    ss << " plan \"nfmcp23\" "  << std::endl;
    ss << " with operators \"Hybrid\" ";

    std::string a,b;
    std::tie(a,b) = sqm.runQuery(ss.str());
    ss.str(std::string());
    ss.clear();
}

int main(int argc, char **argv) {
    questo();
    exit(1);

    log_data_format format = XES1;
    std::string log_file = "1000_30_10.xes";
    std::string unary_model_file = "data/benchmarking/mining/models/unary.txt";
    std::string binary_model_file = "data/benchmarking/mining/models/binary.txt";
    std::string logger_file = "benchmarking/mining/results.csv";

    double support = 0.1;
    Algorithm algorithm = BOLT1;
    uint16_t iters = 1;
    bool no_stats = false;

    std::vector<std::string> unary_templates{};
    std::vector<std::string> binary_templates{};

    args::ArgumentParser parser("Title", "Description");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Group file_format(parser, "This group is all exclusive:", args::Group::Validators::AtMostOne);
    args::ValueFlag<std::string> logFile(file_format, "Log", "The Log, in human readable format, to load into the knowledgebase", {'l', "log"});
    args::ValueFlag<std::string> xesFile(file_format, "XES", "The Log in xes format to load into the knowledgebase", {'x', "xes"});
    args::ValueFlag<std::string> tabFile(file_format, "TAB", "The Log in a tab separated format, with no event payload, to load into the knowledgebase", {'t', "tab"});

    args::Group algorithm_group(parser, "This group is all exclusive:", args::Group::Validators::AtMostOne);
    std::unordered_map<std::string, Algorithm> metric_map{
            {"bolt1", BOLT1},
            {"bolt2", BOLT2},
            {"apriori", APRIORI},
            {"topk", TOP_K_MINING}};
    args::MapFlag<std::string, Algorithm> use_confidence_for_clustering(parser, "a", "Setting up the mining algorithm of choice", {'a', "algorithm"}, metric_map);

    args::Group group(parser, "You can use the following parameters", args::Group::Validators::DontCare, args::Options::Global);
    args::ValueFlag<double>  supportVal(group, "Support Value", "If present, specifies the support value", {'s', "support"});
    args::ValueFlag<std::string> unaryModelFile(group, "Unary clauses to mine", "The model containing the unary clauses templates to mine", {'u', "unary"});
    args::ValueFlag<std::string> binaryModelFile(group, "Binary Clauses to mine", "The model containing the binary clause templates to mine", {'n', "binary"});
    args::ValueFlag<uint16_t>  iterNum(group, "Number of Iterations", "If present, specifies the number of times the pipeline will be run (for benchmarking)", {'q', "queryCount"});
    args::ValueFlag<std::string> loggerFile(group, "Logger File Location", "The file to logs output (times etc) to", {'o', "output"});
    args::Flag noStats(group, "No Stats", "Whether or not to generate statistical data", {"noStats"});

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

    if (logFile) {
        log_file = args::get(logFile);
        format = HUMAN_READABLE_YAUCL;
    }
    if (xesFile) {
        log_file = args::get(xesFile);
        format = XES1;
    }
    if (tabFile) {
        log_file = args::get(tabFile);
        format = TAB_SEPARATED_EVENTS;
    }
    if (use_confidence_for_clustering) {
        algorithm = args::get(use_confidence_for_clustering);
    }

    if(supportVal){
        support = args::get(supportVal);
    }
    if(unaryModelFile){
        unary_model_file = args::get(unaryModelFile);
    }
    if(binaryModelFile){
        binary_model_file = args::get(binaryModelFile);
    }
    if(loggerFile){
        logger_file = args::get(loggerFile);
    }
    if(noStats){
        no_stats = true;
    }

    FeedQueryLoadFromFile log;
    log.env_name = "env";
    log.file = log_file;
    log.format = format;
    log.no_stats = false;

    std::ifstream u_input_stream(unary_model_file);

    unary_templates.clear();
    std::string line;
    while (getline(u_input_stream, line)) {
        unary_templates.push_back(line);
    }

    std::ifstream b_input_stream(binary_model_file);

    binary_templates.clear();
    line.clear();
    while (getline(b_input_stream, line)) {
        binary_templates.push_back(line);
    }

    if(iterNum){
        iters = args::get(iterNum);
    }

    for(uint16_t i = 0; i < iters; ++i){
        switch (algorithm) {
            case BOLT1:
                bolt_algorithm(logger_file, log, support, i, no_stats);       // Using directly the relational database representation without querying
                break;
            case BOLT2:
                bolt_algorithm2(logger_file, log, support, i, no_stats);       // Using directly the relational database representation without querying
                break;
            case APRIORI:
                apriori(logger_file, log, support, unary_templates, binary_templates, i, no_stats);   // Using A-Priori + Querying for checking satisfiability
                break;
            case TOP_K_MINING:
                binary_templates.insert(binary_templates.end(), unary_templates.begin(), unary_templates.end());
                top_k_mining(logger_file, log, support, binary_templates, i, no_stats);   // Using A-Priori + Querying for checking satisfiability
                break;
        }
    }

    return 0;
}