//
// Created by giacomo on 27/12/24.
//

#include <dt_mining/benchmarking/SegmentStats.h>

nlohmann::json DTMining::SegmentStats::asJson() const {
    nlohmann::json dom = nlohmann::json::object();
    dom["experiment_number"] = experiment_number;
    dom["env_name"] = env_name;
    dom["clazzSegmentSize"] = clazzSegmentSize;
    dom["nConstituents"] = nConstituents;
    dom["event_wise_mine_time"] = event_wise_mine_time;
    dom["event_wise_serial_time"] = event_wise_serial_time;
    dom["unique_event_label"] = unique_event_label;
    auto fs = nlohmann::json::array();
    for (const auto& item : fineStats) {
        fs.emplace_back(item.asJson());
    }
    dom["fineStats"] = fs;
    return dom;
};