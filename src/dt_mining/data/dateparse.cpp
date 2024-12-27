//
// Created by giacomo on 05/10/24.
//

#include "dt_mining/data/dateparse.h"

#include <date/date.h>
#include <chrono>
#include <iomanip>

long long int parse8601(const std::string &save) {
    try {
        return std::stoull(save);
    } catch (...) {
        std::istringstream in{save};
        date::sys_time<std::chrono::milliseconds> tp;
        in >> date::parse("%FT%TZ", tp);
        if (in.fail()) {
            bool fail = false;
            std::istringstream in2{save};
            try {
                in2 >> date::parse("%FT%T%Ez", tp);
            } catch (...) {
                fail = true;
            }
            if (fail || (in2.fail())) {
                try {
                    std::tm tm = {};
                    std::stringstream ss(save);
                    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%OS6");
                    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(mktime(&tm));
                    return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
                } catch (...) {
                    return std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch()
                    ).count();
                }
            }
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    }
}

struct time_series_dataset {
    std::vector<std::string> dimName;
};
