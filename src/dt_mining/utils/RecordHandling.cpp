//
// Created by giacomo on 05/10/24.
//

#include "dt_mining/utils/RecordHandling.h"

RecordHandling::RecordHandling(bool isDataless,
                               const std::string& dimension,
               const std::span<double>& orig,
                               const std::span<double>& time,
               std::vector<BasicRecord>&& list) : dimension{dimension}, ls{std::move(list)}, orig{orig}, time{time} {
    linear_time.resize(ls.size());
    size_t init_curr_value = 0;
    bool latest_value;
    size_t N = ls.size();
    for (size_t timestamp = 0; timestamp<N; timestamp++) {
        const auto& x = ls.at(timestamp);
        if (timestamp == 0) {
            init_curr_value = timestamp;
            latest_value = x.type;
        } else if (x.type != latest_value) {
            unsigned char idx = latest_value ? 1 : 0;
            auto aotoff = arrow_of_time.size();
            arrow_of_time.emplace_back(idx, groups[idx].size());
            groups[idx].emplace_back(init_curr_value, timestamp-1, timestamp-init_curr_value);
            for (size_t i = init_curr_value; i<=timestamp-1; i++)
                linear_time[i] = aotoff;
            init_curr_value = timestamp;
            latest_value = x.type;
        }
    }
    if (!ls.empty()) {
        unsigned char idx = latest_value ? 1 : 0;
        auto aotoff = arrow_of_time.size();
        arrow_of_time.emplace_back(idx, groups[idx].size());
        groups[idx].emplace_back(init_curr_value, N-1, N-init_curr_value);
        for (size_t idx = init_curr_value; idx<=N-1; idx++)
            linear_time[idx] = aotoff;
    }
    if (isDataless) {
        setDataless();
    }
}

std::pair<std::string, std::unordered_map<std::string,double>> RecordHandling::genPureInterval(const std::string &action, size_t begin, size_t end) const {
    return {dimension+"("+action+")", f.eval(orig, time, begin, end)};
}

std::tuple<size_t, std::string, std::unordered_map<std::string,double>> RecordHandling::genInterval(const std::string &action, size_t begin, size_t end) const {
    return {begin, dimension+"("+action+")", f.eval(orig, time, begin, end)};
}

