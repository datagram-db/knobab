//
// Created by giacomo on 27/12/24.
//

#include "dt_mining/benchmarking/FineGrainedStats.h"

namespace DTMining {
    nlohmann::json FineGrainedStats::asJson() const {
        nlohmann::json dom = nlohmann::json::object();
        dom["experiment_number"] = experiment_number;
        dom["env_name"] = env_name;
        dom["event_id"] = event_id;
        dom["nConstituents"] = nConstituents;
        dom["event_wise_mine_time"] = event_wise_mine_time;
        dom["event_wise_serial_time"] = event_wise_serial_time;
        dom["unique_event_label"] = unique_event_label;
        return dom;
    };

} // DTMining