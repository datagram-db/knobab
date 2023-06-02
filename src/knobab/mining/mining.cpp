//
// Created by giacomo on 05/10/22.
//
#include <args.hxx>
#include <knobab/algorithms/mining/pattern_mining.h>
#include <knobab/mining/apriori.h>
#include <fstream>
#include "knobab/server/query_manager/ServerQueryManager.h"

enum Algorithm {
    BOLT,
    APRIORI,
    PREVIOUS_MINING
};

int main(int argc, char **argv) {
    log_data_format format = XES1;
    std::string log_file = "/home/giacomo/Scaricati/1000_30_10.xes";
    std::string unary_model_file = "/home/sam/Documents/Repositories/CodeBases/knobab/data/benchmarking/mining/models/unary.txt";
    std::string binary_model_file = "/home/sam/Documents/Repositories/CodeBases/knobab/data/benchmarking/mining/models/binary.txt";
    std::string logger_file = "/home/sam/Documents/Repositories/CodeBases/knobab/data/benchmarking/mining/results.csv";

    double support = 0.1;
    Algorithm algorithm = BOLT;
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
    args::Flag boltAlgorithm(algorithm_group, "Bolt Algorithm", "Bolt algorithm for the mining", {'b', "bolt"});
    args::Flag aprioriAlgorithm(algorithm_group, "Apriori Algorithm", "Apriori algorithm for the mining", {'a', "apriori"});
    args::Flag previousAlgorithm(algorithm_group, "Previous Algorithm", "Previous algorithm for the mining", {'p', "previous"});

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

    if (boltAlgorithm) {
        algorithm = BOLT;
    }
    if (aprioriAlgorithm) {
        algorithm = APRIORI;
    }
    if (previousAlgorithm) {
        algorithm = PREVIOUS_MINING;
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
            case BOLT:
                bolt_algorithm(logger_file, log, support, i, no_stats);       // Using directly the relational database representation without querying
                break;
            case APRIORI:
                apriori(logger_file, log, support, unary_templates, binary_templates, i, no_stats);   // Using A-Priori + Querying for checking satisfiability
                break;
            case PREVIOUS_MINING:
                binary_templates.insert(binary_templates.end(), unary_templates.begin(), unary_templates.end());
                previous_mining(logger_file, log, support, binary_templates, i, no_stats);   // Using A-Priori + Querying for checking satisfiability
                break;
        }
    }

    return 0;
}