//
// Created by giacomo on 05/10/22.
//

#include <knobab/algorithms/mining/pattern_mining.h>
#include <knobab/mining/apriori.h>

int main() {
    FeedQueryLoadFromFile log;
    log.env_name = "env";
    log.file = "/home/giacomo/projects/knobab2/data/benchmarking/completed/1000_1000_log.tab";
    log.format = TAB_SEPARATED_EVENTS;
    log.no_stats = true;
    std::vector<std::string> templates{"Choice", "ExlChoice", "CoExistence", "RespExistence", "ChainPrecedence", "ChainResponse", "Precedence", "Response"};
    std::string logger_file = "/home/giacomo/logger.text";
    bool use_bolt = false;
    double support = 0.01;
    if (use_bolt)
        bolt_algorithm(logger_file, log, support);       // Using directly the relational database representation without querying!
    else
        apriori(logger_file, log, support, templates);   // Using A-Priori + Querying for checking satisfiability
    return 0;
}