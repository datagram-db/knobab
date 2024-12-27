//
// Created by giacomo on 27/12/24.
//

#ifndef DTMINING_TRACESTATS_H
#define DTMINING_TRACESTATS_H

#include <dt_mining/benchmarking/SegmentStats.h>

namespace DTMining {
    struct TraceStats { // per_trace_stats
        size_t experiment_number;
        std::string env_name;                       // Environment name
        size_t nOfSegments;                         // Number of segments composing the trace
        size_t dimensions;                          // Number of dimensions within the trace
        std::vector<SegmentStats> stats_vector;
        size_t unique_event_label;                  // Number of total distinct event labels per trace trace
        size_t avgSegmentSize;                      // Average size for segments
        size_t totalConstituents;                   // Total number of constituents appearing across dimensions and segments within the trace
        double event_wise_mine_time;
        double event_wise_serial_time;

        nlohmann::json asJson() const ;
    };
};

#endif //DTMINING_TRACESTATS_H
