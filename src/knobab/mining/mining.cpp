//
// Created by giacomo on 05/10/22.
//
#include <args.hxx>
#include <knobab/algorithms/mining/pattern_mining.h>
#include <knobab/mining/apriori.h>
#include <fstream>

enum Algorithm {
    BOLT,
    APRIORI,
    PREVIOUS_MINING
};

int main() {
    log_data_format format = XES1;
    std::string log_file = "/home/giacomo/Scaricati/10.xes";
    std::string model_file = "data/benchmarking/mining/models/model.txt";
//    std::string log_file = "data/benchmarking/mining/bpic_2019/logs/10.xes";
//    std::string model_file = "data/benchmarking/mining/models/model.txt";
    double support = 0.00001;
    Algorithm algorithm = BOLT;

    std::vector<std::string> templates{};

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
    args::ValueFlag<uint16_t>  supportVal(group, "Support Value", "If present, specifies the support value", {'s', "support"});
    args::ValueFlag<std::string> modelFile(group, "Clauses to mine", "The model containing the clause templates to mine", {'m', "model"});

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
    if(modelFile){
        model_file = args::get(modelFile);
    }

    FeedQueryLoadFromFile log;
    log.env_name = "env";
    log.file = log_file;
    log.format = format;
    log.no_stats = true;

    std::ifstream input_stream(model_file);
//    if (!input_stream) {
//        std::cerr << "Can't open model file!";
//        return 1;
//    }

    templates.clear();
    std::string line;
    while (getline(input_stream, line)) {
        templates.push_back(line);
    }

    std::string logger_file = "data/benchmarking/mining/output.text";

    switch (algorithm) {
        case BOLT:
            bolt_algorithm(logger_file, log, support);       // Using directly the relational database representation without querying
            break;
        case APRIORI:
            apriori(logger_file, log, support, templates);   // Using A-Priori + Querying for checking satisfiability
            break;
        case PREVIOUS_MINING:
            previous_mining(logger_file, log, support, templates);   // Using A-Priori + Querying for checking satisfiability
            break;
    }
    return 0;
}