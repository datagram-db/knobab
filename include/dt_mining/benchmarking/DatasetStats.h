//
// Created by giacomo on 27/12/24.
//

#ifndef DTMINING_DATASETSTATS_H
#define DTMINING_DATASETSTATS_H

#include <dt_mining/benchmarking/TraceStats.h>

namespace DTMining {
    struct DatasetStats {
        size_t experiment_number;
        std::filesystem::path folder;
        double precision;
        double max_val;
        double loading;
        double serial;
        double total_time_mining;
        double total_event_time_serial;
        double total_time;
        bool   isAlgo3;
        bool   isDataless;
        size_t nProcessors;
        std::vector<TraceStats> all_stats;

        nlohmann::json asJson() const ;
    };

}

#endif //DTMINING_DATASETSTATS_H
