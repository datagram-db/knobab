//
// Created by giacomo on 27/12/24.
//

#include <dt_mining/benchmarking/DatasetStats.h>

nlohmann::json DTMining::DatasetStats::asJson() const {
    nlohmann::json dom = nlohmann::json::object();
    dom["experiment_number"] = experiment_number;
    dom["folder"] = folder.string();
    dom["precision"] = precision;
    dom["max_val"] = max_val;
    dom["loading"] = loading;
    dom["serial"] = serial;
    dom["Algo"] = isAlgo3 ? "Algorithm3" : "Algorithm2";
    dom["total_time_mining"] = total_time_mining;
    dom["total_event_time_serial"] = (total_event_time_serial);
    dom["total_time"] = total_time;
    dom["isDataless"] = isDataless;
    auto fs = nlohmann::json::array();
    for (const auto& item : all_stats) {
        fs.emplace_back(item.asJson());
    }
    dom["all_stats"] = fs;
    return dom;
};