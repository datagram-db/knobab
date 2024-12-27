//
// Created by giacomo on 27/12/24.
//

#ifndef DTMINING_SEGMENTSTATS_H
#define DTMINING_SEGMENTSTATS_H

#include <dt_mining/benchmarking/FineGrainedStats.h>

namespace DTMining {
    struct SegmentStats {
        size_t experiment_number;
        std::string env_name;
        size_t unique_event_label;
        size_t clazzSegmentSize;
        size_t nConstituents;
        double event_wise_mine_time;
        double event_wise_serial_time;
        std::vector<FineGrainedStats> fineStats;

        nlohmann::json asJson() const;
    };
};

#endif //DTMINING_SEGMENTSTATS_H
