//
// Created by giacomo on 27/12/24.
//

#ifndef DTMINING_FINEGRAINEDSTATS_H
#define DTMINING_FINEGRAINEDSTATS_H

#include <nlohmann/json.hpp>

namespace DTMining {

    struct FineGrainedStats {
        size_t experiment_number;
        std::string env_name;
        size_t event_id;
        size_t nConstituents;
        double event_wise_mine_time;
        double event_wise_serial_time;
        size_t unique_event_label;

        nlohmann::json asJson() const;
    };

} // DTMining

#endif //DTMINING_FINEGRAINEDSTATS_H
