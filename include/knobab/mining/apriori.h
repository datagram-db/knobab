//
// Created by giacomo on 17/01/23.
//

#ifndef KNOBAB_SERVER_APRIORI_H
#define KNOBAB_SERVER_APRIORI_H

#include <ostream>
#include "yaucl/bpm/structures/log/data_loader.h"

struct FeedQueryLoadFromFile {
    log_data_format format = TAB_SEPARATED_EVENTS;
    std::string file = "";
    bool with_data = false;
    bool no_stats = false;
    bool with_missing = false;
    std::string env_name = "kb";
    DEFAULT_CONSTRUCTORS(FeedQueryLoadFromFile)
    friend std::ostream &operator<<(std::ostream &os, const FeedQueryLoadFromFile &file);
};

void apriori(const std::string& benchmarking_logger,
             const FeedQueryLoadFromFile& log,
             double support,
             std::vector<std::string>& templates);

#endif //KNOBAB_SERVER_APRIORI_H
