//
// Created by giacomo on 27/12/24.
//

#include <dt_mining/benchmarking/TraceStats.h>

nlohmann::json DTMining::TraceStats::asJson() const {
    nlohmann::json dom = nlohmann::json::object();
    dom["experiment_number"] = experiment_number;
    dom["env_name"] = env_name;
    dom["nOfSegments"] = nOfSegments;
    dom["dimensions"] = dimensions;
    dom["event_wise_mine_time"] = event_wise_mine_time;
    dom["event_wise_serial_time"] = event_wise_serial_time;
    dom["unique_event_label"] = unique_event_label;
    dom["avgSegmentSize"] = avgSegmentSize;
    dom["totalConstituents"] = totalConstituents;
    auto fs = nlohmann::json::array();
    for (const auto& item : stats_vector) {
        fs.emplace_back(item.asJson());
    }
    dom["stats_vector"] = fs;
    return dom;
};