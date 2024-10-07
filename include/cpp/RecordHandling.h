//
// Created by giacomo on 05/10/24.
//

#ifndef DTMINING_RECORDHANDLING_H
#define DTMINING_RECORDHANDLING_H

#include <vector>
using Group = std::tuple<size_t, size_t, size_t>;
#include <array>
#include <string>
#include <fish.h>
#include <BasicRecord.h>
#include <ThreadPool.h>
#include <optional>

static inline
std::optional<Group> inIntervalTree(const std::vector<Group>& ls, size_t x) {
    if (ls.empty()) {
        return {};
    }
    auto it = std::lower_bound(ls.begin(), ls.end(), x,
                               [](const Group& interval, size_t value) {
                                   return std::get<0>(interval) < value;
                               });
    if (it == ls.end()) {
        return {};
    }
    if ((std::get<0>(*it) > x)) {
        if (it != ls.begin()) {
            it--;
            if (!((std::get<0>(*it)<= x) && (x <= std::get<1>(*it))))
                return {};
            else
                return {*it};
        } else {
            return {};
        }
    }

    if (!((std::get<0>(*it)<= x) && (x <= std::get<1>(*it))))
        return {*it};
    else {
        return {};
    }
}

#include <set>
#include <span>

struct RecordHandling {
    std::vector<std::pair<unsigned char, size_t>> arrow_of_time;
    std::array<std::vector<Group>, 2> groups;
    std::vector<size_t> linear_time;
    std::array<std::string, 2> straight{"DecreaseRapidly","IncreaseRapidly"};
    std::array<std::string, 2> V1      {"HighVolatility6","HighVolatility1"};
    std::array<std::string, 2> V2      {"HighVolatility4","HighVolatility3"};
    std::array<std::string, 2> Vcmp      {"HighVolatility5","HighVolatility2"};
    std::array<std::string, 2> _1H      {"DecreaseSlowly4","IncreaseSlowly1"};
    std::array<std::string, 2> _2H      {"DecreaseSlowly3","IncreaseSlowly2"};
    std::array<std::string, 2> _1HN      {"DecreaseSlowly2","IncreaseSlowly3"};
    std::array<std::string, 2> _2HN      {"DecreaseSlowly1","IncreaseSlowly4"};
    std::vector<BasicRecord> ls;
    std::span<double> orig;
    std::span<double> time;
    std::string dimension;
    fish f;

    RecordHandling() = default;
    RecordHandling(const RecordHandling&) = default;
    RecordHandling& operator=(const RecordHandling&) = default;
    RecordHandling(const std::string& dimension,
                   const std::span<double>& orig,
                   const std::span<double>& time,
                   std::vector<BasicRecord>&& list);


    void Algorithm2(ThreadPool& pool,
                    std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>& futures) const {
        for (const auto& group :this->groups[0]) {
            Algorithm2(pool, futures, group, 0);
        }
        for (const auto& group :this->groups[1]) {
            Algorithm2(pool, futures, group, 1);
        }
    }

    inline void Algorithm3(ThreadPool& pool,
                           std::vector<std::future<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>>& futures,
                           size_t time) const {
        auto arrow_idx = linear_time.at(time);
        const auto& arrow = arrow_of_time.at(arrow_idx);
        const auto& group_type = arrow.first;
        const auto& group_offset = arrow.second;
        const auto& groupRange = groups.at(group_type).at(group_offset);
        // TODO: some cases are missing
        int n = arrow_of_time.size();
        int flip = (group_type + 1) % 2;
//        std::cout << time << std::endl;

        for (int end_time = time; end_time <= std::get<1>(groupRange); ++end_time) {
            int span = end_time - time + 1;
            if (span > 1) {
                push_task(pool, futures, straight[group_type], time, end_time); //
            }
            if ((arrow_idx + 1) < n && end_time == std::get<1>(groupRange)) {
                const auto& next_ref = arrow_of_time.at(arrow_idx + 1);
                const auto& next = groups.at(flip).at(next_ref.second);
                if (span == 1) {
                    for (auto next_time = std::get<0>(next)+1;
                         next_time<std::get<1>(next)+1;
                         next_time++) {
                        push_task(pool, futures, _1H[flip], time, next_time); // genPureInterval(_1H[flip], time, next_time)
                    }

                    if (arrow_idx + 2 < n) {
                        if (std::get<2>(next) > 1) {
                            push_task(pool, futures, V2[flip], time, std::get<1>(next) + 1); //
                        } else {
                            const auto& nextnext = groups.at(flip).at(next_ref.second);
                            for (auto nextnext_time = std::get<0>(nextnext)+1;
                                 nextnext_time<std::get<1>(nextnext)+1;
                                 nextnext_time++) {
                                push_task(pool, futures, _2H[group_type], time, nextnext_time);
                            }
                        }
                    }
                } else {
                    push_task(pool, futures, _1HN[group_type], time, std::get<1>(groupRange) + 1); //
                    if ((std::get<2>(next) == 1) && (arrow_idx + 2 < n)) {
                        push_task(pool, futures, _2HN[group_type], time, std::get<1>(groupRange) + 2);
                    }
                }
            }
        }

    }

private:
    void Algorithm2(ThreadPool& pool,
                    std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>& futures,
                    const Group& x,
                    int group_type) const {
        /*
        This algorithm is a speedup if compared to the original IDEAS'24 paper, as we are not required to get the data
        @param x:           The group from which we want to generate data
        @param group_type:  The type associated with the group
        @return:            A vector of events, not being sorted by time
        */
        std::unordered_map<int, std::set<int>> begin_match;
//        std::vector<Event> result;

        int flip = (group_type + 1) % 2;
        for (size_t current_span = 1; current_span <= std::get<2>(x); ++current_span) {
//            std::cout << current_span - 1 << std::endl;
            for (size_t start = std::get<0>(x); start<std::get<1>(x)-current_span+2; start++) {
                if (start + current_span - 1 > std::get<1>(x)) {
                    continue;
                }
                push_task(pool, futures, straight[group_type], start, start + current_span - 1);
                std::optional<Group> Inext;
                if (start == std::get<1>(x)) {
                    Inext = inIntervalTree(groups[flip], std::get<1>(x) + 1);
                }
                if (start == std::get<0>(x)) {
                    auto Iprev = inIntervalTree(groups[flip], start - 1);
                    if (Iprev.has_value()) {
                        push_task(pool, futures, _1H[group_type], start - 1, start + current_span - 1);
                        if (Inext.has_value()) {
                            push_task(pool, futures, V2[group_type], start - 1, start + current_span);
                        }
                        if (std::get<2>(Iprev.value())) {
                            auto Iprevprev = inIntervalTree(groups[group_type], start - 2);
                            if (Iprevprev.has_value()) {
                                push_task(pool, futures, _2H[group_type], start - 2, start + current_span - 1);
                            }
                        } else {
                            for (int prev_start = std::get<0>(Iprev.value());
                                 prev_start < std::get<1>(Iprev.value())+1;
                                 prev_start++) {
                                if (prev_start == start - 1) {
                                    continue;
                                }
                                push_task(pool, futures, V1[group_type], prev_start, start + current_span - 1);
                                begin_match[prev_start].insert(start + current_span - 1);
                            }
                        }
                    }
                }
                if ((start + current_span - 1 == std::get<1>(x)) && (Inext.has_value())) {
                    push_task(pool, futures, _1HN[group_type], start, start + current_span - 1);
                    if (std::get<2>(Inext.value()) == 1) {
                        auto Inextnext = inIntervalTree(groups[group_type], std::get<1>(x) + 2);
                        if (Inextnext.has_value()) {
                            push_task(pool, futures, _2HN[group_type], start, start + current_span);
                        }
                    }
                }
            }
        }
        for (const auto& [begin, ends] : begin_match) {
            for (int end : ends) {
                if (begin_match.find(end + 1) != begin_match.end()) {
                    for (int new_end_time : begin_match[end + 1]) {
                        push_task(pool, futures, Vcmp[group_type], begin, new_end_time);
                    }
                }
            }
        }
    }
    std::pair<std::string, std::unordered_map<std::string,double>> genPureInterval(const std::string &action, size_t begin, size_t end) const;
    std::tuple<size_t, std::string, std::unordered_map<std::string,double>> genInterval(const std::string &action, size_t begin, size_t end) const;

    void push_task(ThreadPool& pool, std::vector<std::future<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>>& futures,
                   const std::string &action,
                   size_t begin,
                   size_t end) const;
    void push_task(ThreadPool& pool, std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>& futures,
                   const std::string &action,
                   size_t begin,
                   size_t end) const;
};


static inline
std::tuple<RecordHandling,RecordHandling,RecordHandling,RecordHandling> transform_series(const std::string& dim,
                                                                                         const std::span<double>& ls,
                                                                                         std::span<double>& timespan,
                                                                                         const double epsilon = 0.0001,
                                                                                         const double maxval = 1000000000.0) {
    auto end = ls.end() - 1;
    std::vector<BasicRecord> increase_list, absence_list, stationary_list, variability_list;
    increase_list.reserve(ls.size());
    absence_list.reserve(ls.size());
    stationary_list.reserve(ls.size());
    variability_list.reserve(ls.size());
    size_t idx = 0;
    for (auto currPtr = ls.begin(); currPtr != end; ++currPtr) {
        auto nextPtr = currPtr + 1;
        if (idx == 0) {
            increase_list.emplace_back(true, idx, *currPtr, *currPtr);
            auto absence_cond = std::abs(*currPtr) <= epsilon;
            absence_list.emplace_back(absence_cond, idx, *currPtr, absence_cond ? std::abs(*currPtr) : 0.0);
            stationary_list.emplace_back(false, idx, *currPtr, *currPtr);
            variability_list.emplace_back(false, idx, *currPtr, maxval);
        }

        increase_list.emplace_back(increase_gen(idx+1, *currPtr, *nextPtr));
        absence_list.emplace_back(absence_gen(epsilon, idx+1, *nextPtr));
        stationary_list.emplace_back(stationariety_gen(epsilon, idx, *currPtr, *nextPtr));
        variability_list.emplace_back(variation_gen(epsilon, maxval, idx,*currPtr, *nextPtr));
        idx++;
    }
    return {{dim+"_i", ls,timespan, std::move(increase_list)},
            {dim+"_a", ls,timespan, std::move(absence_list)},
            {dim+"_s",ls,timespan,std::move(stationary_list)},
            {dim+"_v",ls,timespan,std::move(variability_list)}
    };
}


#endif //DTMINING_RECORDHANDLING_H
