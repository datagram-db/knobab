//
// Created by giacomo on 05/10/22.
//

#include <knobab/algorithms/mining/pattern_mining.h>
#include <knobab/mining/apriori.h>

enum Algorithm {
    BOLT,
    APRIORI,
    PREVIOUS_MINING
};

int main() {
    FeedQueryLoadFromFile log;
    log.env_name = "env";
    log.file = "/home/giacomo/projects/knobab2/data/benchmarking/completed/1000_1000_log.tab";
    log.format = TAB_SEPARATED_EVENTS;
    log.no_stats = true;
    std::vector<std::string> templates{"Choice", "ExlChoice", "CoExistence", "RespExistence", "ChainPrecedence", "ChainResponse", "Precedence", "Response"};
    std::string logger_file = "/home/giacomo/logger.text";
    Algorithm use_bolt = BOLT;
    double support = 0.00001;
    switch (use_bolt) {
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