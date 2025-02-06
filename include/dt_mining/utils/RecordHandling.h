//
// Created by giacomo on 05/10/24.
//

#ifndef DTMINING_RECORDHANDLING_H
#define DTMINING_RECORDHANDLING_H

#include <unordered_set>
#include <dt_mining/utils/utils.h>
#include <vector>
using Group = std::tuple<size_t, size_t, size_t>;

#define GRP_START(x)                    (std::get<0>(x))
#define GRP_STARTS_AT(x,time)           (GRP_START(x) == time)
#define GRP_FINISH(x)                   (std::get<1>(x))
#define GRP_FINISHES_AT(x,time)         (GRP_FINISH(x) == time)
#define GRP_INT_DURATION(x)             (std::get<2>(x))
#define GRP_IS_SINGLET(x)               (std::get<2>(x)==1)
#define HAS_PREV_ARROW(x)               (((x))>0)
#define ARROW_NEXT(x)                   (((x)) + 1)
#define ARROW_NEXT_NEXT(x)              (((x)) + 2)
#define ARROW_NEXT_NEXT_NEXT(x)         (((x)) + 3)
#define HAS_ARROW_NEXT(x,n)             (ARROW_NEXT(x) < (n))
#define HAS_ARROW_NEXT_NEXT(x,n)        (ARROW_NEXT_NEXT(x) < (n))
#define HAS_ARROW_NEXT_NEXT_NEXT(x,n)   (ARROW_NEXT_NEXT_NEXT(x) < (n))

#include <array>
#include <string>
#include <dt_mining/utils/fish.h>
#include <dt_mining/utils/BasicRecord.h>
#include <ThreadPool.h>
#include <optional>
#include <yaucl/functional/assert.h>

#ifdef DEBUG
#include <iostream>
#include <tabulate/tabulate.hpp>
#include <tabulate/table.hpp>
#endif

// This function is not supported by all major C++ libraries!// https://en.cppreference.com/w/cpp/algorithm/lower_bound
template<class ForwardIt, class T = typename std::iterator_traits<ForwardIt>::value_type,
        class Compare>
ForwardIt lower_bound2(ForwardIt first, ForwardIt last, const T& value, Compare comp)
{
    ForwardIt it;
    typename std::iterator_traits<ForwardIt>::difference_type count, step;
    count = std::distance(first, last);

    while (count > 0)
    {
        it = first;
        step = count / 2;
        std::advance(it, step);

        if (comp(*it, value))
        {
            first = ++it;
            count -= step + 1;
        }
        else
            count = step;
    }

    return first;
}

static inline
std::optional<Group> inIntervalTree(const std::vector<Group>& ls, size_t x) {
    if (ls.empty()) {
        return {};
    }
    auto it = lower_bound2(ls.begin(), ls.end(), x,
                               [](const Group& interval, size_t value) {
                                   return GRP_START(interval) < value;
                               });
    if (it == ls.end()) {
        return {};
    }
    if ((GRP_START(*it) > x)) {
        if (it != ls.begin()) {
            it--;
            if (!((GRP_START(*it)<= x) && (x <= GRP_FINISH(*it))))
                return {};
            else
                return {*it};
        } else {
            return {};
        }
    }

    if (!((GRP_START(*it)<= x) && (x <= GRP_FINISH(*it))))
        return {*it};
    else {
        return {};
    }
}

#include <set>
#include <span>

class conditional_structure {
    bool is_future{false};
    bool is_int_prefix{false};
    void* data{nullptr};

public:
    void set(std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>* obj)  {
        data = (void*)obj;
        is_future = is_int_prefix = true;
    }
    void set(std::vector<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>* obj)  {
        data = (void*)obj;
        is_future = false;
        is_int_prefix = true;
    }
    void set(std::vector<std::future<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>>* obj) {
        data = (void*)obj;
        is_future = true;
        is_int_prefix = false;
    }
    void set(std::vector<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>* obj)  {
        data = (void*)obj;
        is_future = false;
        is_int_prefix = false;
    }



    const bool is_future_based() const { return is_future; }
    const bool has_int_prefix() const { return is_int_prefix; }
    std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>& get_int_future() {
        return *(std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>*)data;
    }
    std::vector<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>& get_int_tradition() {
        return *(std::vector<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>*)data;
    }
    std::vector<std::future<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>>& get_future() {
        return *(std::vector<std::future<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>>*)data;
    }
    std::vector<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>& get_tradition() {
        return *(std::vector<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>*)data;
    }
};

//using conditional_structure = std::variant<std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>,
//                                           std::vector<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>
//                                          >;

#define IS_FUTURE_BASED(x)          (std::holds_alternative<std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>>(x))
#define GET_FUTURE(x)               (std::get<std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>>(x))
#define IS_DATA_BASED(x)            (std::holds_alternative<std::vector<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>(x))
#define GET_TRADITION(x)            (std::get<std::vector<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>>(x))

struct RecordHandling {
    std::vector<std::pair<unsigned char, size_t>> arrow_of_time;    // Determining whether the pattern is positive/negative and at which offset of the group is holden
    std::array<std::vector<Group>, 2> groups;                       // Determining two groups of contiguous information: one, for the satisfied predicates, and the other one,
    // for the non-satisfied ones. These are mimicking the ascending and descending patterns, but also considering just not the numerical value, but also the variation and similar
    std::vector<size_t> linear_time;
    std::array<std::string, 2> straight{"DecreaseRapidly","IncreaseRapidly"};
    std::array<std::string, 2> HV6_1      {"HighVolatility6", "HighVolatility1"};
    std::array<std::string, 2> HV4_3      {"HighVolatility4", "HighVolatility3"};
    std::array<std::string, 2> HV5_2      {"HighVolatility5", "HighVolatility2"};
    std::array<std::string, 2> OneHiccup_S41      {"DecreaseSlowly4", "IncreaseSlowly1"};
    std::array<std::string, 2> TwoHiccups_S32      {"DecreaseSlowly3", "IncreaseSlowly2"};
    std::array<std::string, 2> EndHiccup_S23      {"DecreaseSlowly2","IncreaseSlowly3"};
    std::array<std::string, 2> End2Hiccups_S14      {"DecreaseSlowly1","IncreaseSlowly4"};
    std::vector<BasicRecord> ls;
    std::span<double> orig;
    std::span<double> time;
    std::string dimension;
    fish f;

#ifdef DEBUG
    void print_indices() const {
        std::cout << "Printing the indices:" << std::endl;
        {
            tabulate::Table aot_table;
            tabulate::Table::Row_t R(arrow_of_time.size()+1);
            {
                R[0] = ("Index");
                for (size_t i = 0; i<arrow_of_time.size(); i++) {
                    R[i+1] = std::to_string(i);
                }
                aot_table.add_row(R);
                R[0] = ("ArrowOfTime");
                for (size_t i = 0; i<arrow_of_time.size(); i++) {
                    tabulate::Table cell;
                    cell.add_row({arrow_of_time[i].first ? "true" : "false", std::to_string(arrow_of_time[i].second)});
                    R[i+1] = cell;
                }
                aot_table.add_row(R);
            }
            std::cout << aot_table << std::endl << std::endl ;
        }
        {
            tabulate::Table aot_table;
            tabulate::Table::Row_t R(linear_time.size()+1);
            {
                R[0] = ("Index");
                for (size_t i = 0; i<linear_time.size(); i++) {
                    R[i+1] = std::to_string(i);
                }
                aot_table.add_row(R);
                R[0] = ("linear_time");
                for (size_t i = 0; i<linear_time.size(); i++) {
                    R[i+1] = std::to_string(linear_time[i]);
                }
                aot_table.add_row(R);
            }
            {
                R[0] = ("Offsets");
                for (size_t i = 0; i<linear_time.size(); i++) {
                    R[i+1] = std::to_string(i+1);
                }
                aot_table.add_row(R);
            }
            std::cout << aot_table << std::endl << std::endl ;
        }
        {
            std::cout << "Group[true]" << std::endl;
            tabulate::Table aot_table;
            tabulate::Table::Row_t R(groups[1].size()+1);
            {
                R[0] = ("Index");
                for (size_t i = 0; i<groups[1].size(); i++) {
                    R[i+1] = std::to_string(i);
                }
                aot_table.add_row(R);
            }
            {
                R[0] = ("Begin");
                for (size_t i = 0; i<groups[1].size(); i++) {
                    R[i+1] = std::to_string(GRP_START(groups[1][i]));
                }
                aot_table.add_row(R);
            }
            {
                R[0] = ("End");
                for (size_t i = 0; i<groups[1].size(); i++) {
                    R[i+1] = std::to_string(GRP_FINISH(groups[1][i]));
                }
                aot_table.add_row(R);
            }
            {
                R[0] = ("Span/Duration/Len");
                for (size_t i = 0; i<groups[1].size(); i++) {
                    R[i+1] = std::to_string(GRP_INT_DURATION(groups[1][i]));
                }
                aot_table.add_row(R);
            }
            std::cout << aot_table << std::endl << std::endl ;
        }
        {
            std::cout << "Group[false]" << std::endl;
            tabulate::Table aot_table;
            tabulate::Table::Row_t R(groups[0].size()+1);
            {
                R[0] = ("Index");
                for (size_t i = 0; i<groups[0].size(); i++) {
                    R[i+1] = std::to_string(i);
                }
                aot_table.add_row(R);
            }
            {
                R[0] = ("Begin");
                for (size_t i = 0; i<groups[0].size(); i++) {
                    R[i+1] = std::to_string(GRP_START(groups[0][i]));
                }
                aot_table.add_row(R);
            }
            {
                R[0] = ("End");
                for (size_t i = 0; i<groups[0].size(); i++) {
                    R[i+1] = std::to_string(GRP_FINISH(groups[0][i]));
                }
                aot_table.add_row(R);
            }
            {
                R[0] = ("Span/Duration/Len");
                for (size_t i = 0; i<groups[0].size(); i++) {
                    R[i+1] = std::to_string(GRP_INT_DURATION(groups[0][i]));
                }
                aot_table.add_row(R);
            }
            std::cout << aot_table << std::endl << std::endl ;
        }
    }
#endif

    inline void setDataless(){
        f.setDataless();
    }

    inline const bool dataless() const {
        return f.dataless();
    }

    RecordHandling() = default;
    RecordHandling(const RecordHandling&) = default;
    RecordHandling& operator=(const RecordHandling&) = default;
    RecordHandling(bool isDataless,
                   const std::string& dimension,
                   const std::span<double>& orig,
                   const std::span<double>& time,
                   std::vector<BasicRecord>&& list);

//#ifdef DEBUG
//    std::string algorithm_element;
//#endif

    void Algorithm2(ThreadPool& pool,
                    conditional_structure& futures) const {
        for (const auto& group :this->groups[0]) {
            Algorithm2(pool, futures, group, 0);
        }
        for (const auto& group :this->groups[1]) {
            Algorithm2(pool, futures, group, 1);
        }
    }

    void Algorithm4(ThreadPool& pool,
                    conditional_structure& futures) const {
        for (const auto& group :this->groups[0]) {
            Algorithm4(pool, futures, group, 0);
        }
        for (const auto& group :this->groups[1]) {
            Algorithm4(pool, futures, group, 1);
        }
    }

    // Original version of the algorithm, with some redundancies on the generated events
    inline void Algorithm3(ThreadPool& pool,
                           conditional_structure& futures,
                           size_t time) const {
        auto arrow_idx = VAT(linear_time,time);                  // Pointer to the current positive/negative group is present
        const auto& group_idx = VAT(arrow_of_time,arrow_idx);   // Retrieving the positive/negative and #group information
        const auto& group_type = group_idx.first;
        const auto& group_offset = group_idx.second;
        const auto& groupRange = VAT(VAT(groups,group_type),group_offset); // Retrieving the group

        // TODO: some cases are missing
        int total_groups = arrow_of_time.size();
        int flip = (group_type + 1) % 2;
        //  0: begin, 1: end (inclusive), 2: span

        for (int end_time = time; end_time <= GRP_FINISH(groupRange); ++end_time) { // Iterating over all the possible event lengths starting from here
            int current_span = end_time - time + 1;                                         // Duration of the current event according to the novel ending time
            if (current_span > 1) {
                push_task(pool, futures, straight[group_type], time, end_time);     // Adding a straight increase/decrease event for all possible spans
            }
            if (HAS_ARROW_NEXT(arrow_idx, total_groups) && (GRP_FINISHES_AT(groupRange, end_time))) { // If you have a next event and you reached the end of this group
                const auto& next_ref = VAT(arrow_of_time, ARROW_NEXT(arrow_idx));
                const auto& next = VAT(VAT(groups, flip), next_ref.second);
                if (current_span == 1) {
                    push_task(pool, futures, HV6_1[flip], time, GRP_START(next));
                    if ((GRP_IS_SINGLET(next)) && (HAS_ARROW_NEXT_NEXT(arrow_idx, total_groups)) && (HAS_ARROW_NEXT_NEXT_NEXT(arrow_idx, total_groups))) {
                        const auto& nextnext_ref = VAT(arrow_of_time, ARROW_NEXT_NEXT(arrow_idx));
                        const auto& nextnext = VAT( VAT(groups,group_type), nextnext_ref.second);
                        if (GRP_IS_SINGLET(nextnext)) {
                            push_task(pool, futures, OneHiccup_S41[flip], time, GRP_FINISH(nextnext));
                        }
                    } else {
                        for (auto next_time = GRP_START(next)+1;
                             next_time <=GRP_FINISH(next);
                             next_time++ ) {
                            push_task(pool, futures, OneHiccup_S41[flip], time, next_time);
                        }
                    }

                    if (HAS_ARROW_NEXT_NEXT(arrow_idx, total_groups)) {
                        if (GRP_INT_DURATION(next) > 1) {
                            push_task(pool, futures, HV4_3[flip], time, GRP_FINISH(next) + 1); //
                        } else {
                            const auto& nextnext_ref = VAT(arrow_of_time, ARROW_NEXT_NEXT(arrow_idx));
                            const auto& nextnext = VAT( VAT(groups,group_type), nextnext_ref.second);
                            for (auto nextnext_time = GRP_START(nextnext)+1;
                                 nextnext_time< GRP_FINISH(nextnext)+1;
                                 nextnext_time++) {
                                push_task(pool, futures, TwoHiccups_S32[group_type], time, nextnext_time);
                            }
                        }

                    }
                } else {
                    if (current_span <= GRP_INT_DURATION(next)) {
                        for (auto next_time = GRP_START(next)+current_span-1;
                             next_time <=GRP_FINISH(next);
                             next_time++ ) {
                            push_task(pool, futures, HV6_1[flip], time, next_time);
                            if (HAS_ARROW_NEXT_NEXT(arrow_idx, total_groups) && HAS_ARROW_NEXT_NEXT_NEXT(arrow_idx, total_groups)) {
                                const auto& nextnext_ref = VAT(arrow_of_time, ARROW_NEXT_NEXT(arrow_idx));
                                const auto& nextnext = VAT( VAT(groups,group_type), nextnext_ref.second);
                                if (GRP_INT_DURATION(nextnext) <= current_span) {
                                    const auto& next3_ref = VAT(arrow_of_time, ARROW_NEXT_NEXT_NEXT(arrow_idx));
                                    const auto& next3 = VAT( VAT(groups,flip), next3_ref.second);
                                    for (auto next3_time = GRP_START(next3)+current_span-1;
                                         next3_time <=GRP_FINISH(next3);
                                         next3_time++ ) {
                                        push_task(pool, futures, HV5_2[flip], time, next_time);
                                    }
                                }
                            }
                        }
                    }

                    push_task(pool, futures, End2Hiccups_S14[group_type], time, GRP_FINISH(groupRange) + 1); //
                    if ((GRP_IS_SINGLET(next)) && (HAS_ARROW_NEXT_NEXT(arrow_idx, total_groups))) {
                        push_task(pool, futures, EndHiccup_S23[group_type], time, GRP_FINISH(groupRange) + 2);
                    }
                }
            }
        }

    }

    // A more reduced version of Algorithm 3
    inline void Algorithm5(ThreadPool& pool,
                           conditional_structure& futures,
                           size_t time) const {
        auto arrow_idx = VAT(linear_time,time);                  // Pointer to the current positive/negative group is present
        const auto& group_idx = VAT(arrow_of_time,arrow_idx);   // Retrieving the positive/negative and #group information
        const auto& group_type = group_idx.first;
        const auto& group_offset = group_idx.second;
        const auto& groupRange = VAT(VAT(groups,group_type),group_offset); // Retrieving the group

        // TODO: some cases are missing
        int total_groups = arrow_of_time.size();
        int flip = (group_type + 1) % 2;
        //  0: begin, 1: end (inclusive), 2: span

        // A, optimized
        push_task(pool, futures, straight[group_type], time, GRP_FINISH(groupRange));

        // Iterating over all the possible event lengths starting from here
        for (int end_time = time; end_time <= GRP_FINISH(groupRange); ++end_time) {
            // Duration of the current event according to the novel ending time
            int current_span = end_time - time + 1;

            // If you have a next event and you reached the end of this group
            if (HAS_ARROW_NEXT(arrow_idx, total_groups) && (GRP_FINISHES_AT(groupRange, end_time))) {
                const auto& next_ref = VAT(arrow_of_time, ARROW_NEXT(arrow_idx));
                const auto& next = VAT(VAT(groups, flip), next_ref.second);
                auto has_arrow_next_next = HAS_ARROW_NEXT_NEXT(arrow_idx, total_groups);
                if (current_span == 1) {
                    if (has_arrow_next_next) {
                        if (GRP_IS_SINGLET(next)) {
                            const auto& nextnext_ref = VAT(arrow_of_time, ARROW_NEXT_NEXT(arrow_idx));
                            const auto& nextnext = VAT( VAT(groups,group_type), nextnext_ref.second);
                            if ((HAS_ARROW_NEXT_NEXT_NEXT(arrow_idx, total_groups))) {
                                if (GRP_IS_SINGLET(nextnext)) {
                                    push_task(pool, futures, OneHiccup_S41[flip], time, time+4);
                                } else {
                                    push_task(pool, futures, TwoHiccups_S32[group_type], time, GRP_FINISH(nextnext)); //C, optimized
                                }
                            } else {
                                push_task(pool, futures, OneHiccup_S41[flip], time, GRP_FINISH(next)); //B, optimized
                            }
                        } else {
                            push_task(pool, futures, HV4_3[flip], time, GRP_FINISH(next) + 1); //
                        }
                    } else {
                        push_task(pool, futures, HV6_1[flip], time, GRP_START(next));
                    }
                } else {
                    // E, Original position of (before the if statement, and not in the else branch).
                    if ((GRP_IS_SINGLET(next)) && (has_arrow_next_next)) {
                        push_task(pool, futures, EndHiccup_S23[group_type], time, GRP_FINISH(groupRange) + 2);
                    } else {
                        if (current_span <= GRP_INT_DURATION(next)) {
                            bool inner_vol_found = false;
                            if (has_arrow_next_next && HAS_ARROW_NEXT_NEXT_NEXT(arrow_idx, total_groups)) {
                                const auto& nextnext_ref = VAT(arrow_of_time, ARROW_NEXT_NEXT(arrow_idx));
                                const auto& nextnext = VAT( VAT(groups,group_type), nextnext_ref.second);
                                if (GRP_INT_DURATION(nextnext) <= current_span) {
                                    const auto& next3_ref = VAT(arrow_of_time, ARROW_NEXT_NEXT_NEXT(arrow_idx));
                                    const auto& next3 = VAT( VAT(groups,flip), next3_ref.second);
                                    push_task(pool, futures, HV5_2[flip], time, GRP_FINISH(next3)); //D, optimized
                                    inner_vol_found = true;
                                }
                            }
                            if (!inner_vol_found) {
                                push_task(pool, futures, HV6_1[flip], time, GRP_FINISH(next)); // Optimization, only longest one
                            }
                        } else {
                            push_task(pool, futures, End2Hiccups_S14[group_type], time, GRP_FINISH(groupRange) + 1); // Originally in E
                        }
                    }
                }
            }
        }

    }

private:

    // Algorithm
    void Algorithm2(ThreadPool& pool,
                    conditional_structure& futures,
                    const Group& x,
                    int group_type) const {
        /*
        This algorithm is a speedup if compared to the original IDEAS'24 paper, as we are not required to get the data
        @param x:           The group from which we want to generate data
        @param group_type:  The type associated with the group
        @return:            A vector of events, not being sorted by time
        */
        std::unordered_map<int, std::set<int>> begin_match;

        int flip = (group_type + 1) % 2;
//        DEBUG_ASSERT((GRP_START(x)+GRP_INT_DURATION(x)-1) == (GRP_INT_DURATION(x)));
        push_task(pool, futures, straight[group_type], GRP_START(x), GRP_FINISH(x));
        for (size_t current_span = 1; current_span <= GRP_INT_DURATION(x); ++current_span) {
            for (size_t start = GRP_START(x); start <= (GRP_FINISH(x)-current_span+1); start++) {
                if (start + current_span - 1 > GRP_FINISH(x)) {
                    continue;
                }
                std::optional<Group> Inext;
                if (GRP_FINISHES_AT(x, start)) {
                    Inext = inIntervalTree(groups[flip], GRP_FINISH(x) + 1);
                }
                if (GRP_STARTS_AT(x, start)) {
                    auto Iprev = inIntervalTree(groups[flip], start - 1);
                    if (Iprev.has_value()) {
                        push_task(pool, futures, OneHiccup_S41[group_type], start - 1, start + current_span - 1);
                        if (Inext.has_value()) {
                            push_task(pool, futures, HV4_3[group_type], start - 1, start + current_span);
                        }
                        if (GRP_INT_DURATION(Iprev.value())) {
                            auto Iprevprev = inIntervalTree(groups[group_type], start - 2);
                            if (Iprevprev.has_value()) {
                                push_task(pool, futures, TwoHiccups_S32[group_type], start - 2, start + current_span - 1);
                            }
                        } else {
                            for (int prev_start = GRP_START(Iprev.value());
                                     prev_start <= GRP_FINISH(Iprev.value());
                                     prev_start++) {
                                if (prev_start == start - 1) {
                                    continue;
                                }
                                push_task(pool, futures, HV6_1[group_type], prev_start, start + current_span - 1);
                                begin_match[prev_start].insert(start + current_span - 1);
                            }
                        }
                    }
                }
                if ((GRP_FINISHES_AT(x, start + current_span - 1)) && (Inext.has_value())) {
                    bool found = false;
                    if (GRP_IS_SINGLET(Inext.value())) {
                        auto Inextnext = inIntervalTree(groups[group_type], GRP_FINISH(x) + 2);
                        if (Inextnext.has_value()) {
                            found = true;
                            push_task(pool, futures, EndHiccup_S23[group_type], start, start + current_span);
                        }
                    }
                    if (!found) {
                        push_task(pool, futures, End2Hiccups_S14[group_type], start, start + current_span - 1);
                    }
                }
            }
        }
        for (const auto& [begin, ends] : begin_match) {
            for (int end : ends) {
                if (begin_match.find(end + 1) != begin_match.end()) {
                    for (int new_end_time : begin_match[end + 1]) {
                        push_task(pool, futures, HV5_2[group_type], begin, new_end_time);
                    }
                }
            }
        }
    }

    void Algorithm4(ThreadPool& pool,
                    conditional_structure& futures,
                    const Group& x,
                    int group_type) const {
        /*
        This algorithm is a speedup if compared to the original IDEAS'24 paper, as we are not required to get the data
        @param x:           The group from which we want to generate data
        @param group_type:  The type associated with the group
        @return:            A vector of events, not being sorted by time
        */
        std::unordered_map<int, std::set<int>> begin_match;

        int flip = (group_type + 1) % 2;
        for (size_t current_span = 1; current_span <= GRP_INT_DURATION(x); ++current_span) {
            for (size_t start = GRP_START(x); start <= (GRP_FINISH(x)-current_span+1); start++) {
                if (start + current_span - 1 > GRP_FINISH(x)) {
                    continue;
                }
                push_task(pool, futures, straight[group_type], start, start + current_span - 1);
                std::optional<Group> Inext;
                if (GRP_FINISHES_AT(x, start)) {
                    Inext = inIntervalTree(groups[flip], GRP_FINISH(x) + 1);
                }
                if (GRP_STARTS_AT(x, start)) {
                    auto Iprev = inIntervalTree(groups[flip], start - 1);
                    if (Iprev.has_value()) {
                        push_task(pool, futures, OneHiccup_S41[group_type], start - 1, start + current_span - 1);
                        if (Inext.has_value()) {
                            push_task(pool, futures, HV4_3[group_type], start - 1, start + current_span);
                        }
                        if (GRP_INT_DURATION(Iprev.value())) {
                            auto Iprevprev = inIntervalTree(groups[group_type], start - 2);
                            if (Iprevprev.has_value()) {
                                push_task(pool, futures, TwoHiccups_S32[group_type], start - 2, start + current_span - 1);
                            }
                        } else {
                            for (int prev_start = GRP_START(Iprev.value());
                                 prev_start <= GRP_FINISH(Iprev.value());
                                 prev_start++) {
                                if (prev_start == start - 1) {
                                    continue;
                                }
                                push_task(pool, futures, HV6_1[group_type], prev_start, start + current_span - 1);
                                begin_match[prev_start].insert(start + current_span - 1);
                            }
                        }
                    }
                }
                if ((GRP_FINISHES_AT(x, start + current_span - 1)) && (Inext.has_value())) {
                    push_task(pool, futures, End2Hiccups_S14[group_type], start, start + current_span - 1);
                    if (GRP_IS_SINGLET(Inext.value())) {
                        auto Inextnext = inIntervalTree(groups[group_type], GRP_FINISH(x) + 2);
                        if (Inextnext.has_value()) {
                            push_task(pool, futures, EndHiccup_S23[group_type], start, start + current_span);
                        }
                    }
                }
            }
        }
        for (const auto& [begin, ends] : begin_match) {
            for (int end : ends) {
                if (begin_match.find(end + 1) != begin_match.end()) {
                    for (int new_end_time : begin_match[end + 1]) {
                        push_task(pool, futures, HV5_2[group_type], begin, new_end_time);
                    }
                }
            }
        }
    }

    std::pair<std::string, std::unordered_map<std::string,double>> genPureInterval(const std::string &action, size_t begin, size_t end) const;
    std::tuple<size_t, std::string, std::unordered_map<std::string,double>> genInterval(const std::string &action, size_t begin, size_t end) const;


    inline void push_task(ThreadPool& pool, conditional_structure& fut, const std::string& action, size_t begin, size_t end) const {
//        static std::unordered_set<std::string> S;
//        if (S.insert(action).second) {
//            std::cout << action<< std::endl;
//        }

        if (fut.has_int_prefix()) {
            if (fut.is_future_based()) {
                fut.get_int_future().push_back(pool.enqueue([this](const std::string &action, size_t begin, size_t end){
//#ifdef DEBUG
//                    if ((algorithm_element == "576") && (end-begin+1==18))
//                        std::cerr<< "HERE" <<std::endl;
//#endif
                    return std::tuple<size_t, std::string, std::unordered_map<std::string,double>, size_t>{begin, action+"("+dimension+")", f.eval(orig, time, begin, end), end-begin+1};
                }, action, begin, end));
            } else {
                DEBUG_ASSERT(f.dataless());
                fut.get_int_tradition().emplace_back(begin, action+"("+dimension+")", f.eval(orig, time, begin, end), end-begin+1);
            }
        } else {
            if (fut.is_future_based()) {
                fut.get_future().push_back(pool.enqueue([this](const std::string &action, size_t begin, size_t end){
//#ifdef DEBUG
//                    if ((algorithm_element == "576") && (end-begin+1==18))
//                        std::cerr<< "HERE" <<std::endl;
//#endif
                    return  std::tuple<std::string, std::unordered_map<std::string,double>,size_t>{action+"("+dimension+")", f.eval(orig, time, begin, end), end-begin+1};
                }, action, begin, end));
            } else {
                DEBUG_ASSERT(f.dataless());
                fut.get_tradition().emplace_back( action+"("+dimension+")", f.eval(orig, time, begin, end), end-begin+1);
            }
        }
    }

//    inline void push_task(ThreadPool& pool, std::vector<std::future<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>>& futures,
//                                   const std::string &action,
//                                   size_t begin,
//                                   size_t end) const {
//        futures.push_back(pool.enqueue([this](const std::string &action, size_t begin, size_t end){
//            return  std::tuple<std::string, std::unordered_map<std::string,double>,size_t>{dimension+"("+action+")", f.eval(orig, time, begin, end), end-begin+1};
//        }, action, begin, end));
//    }

//    inline void push_task(ThreadPool& pool, std::vector<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>& futures,
//                          const std::string &action,
//                          size_t begin,
//                          size_t end) const {
//        futures
//    }
//
//    inline void push_task(ThreadPool& pool, std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>, size_t>>>& futures,
//                                   const std::string &action,
//                                   size_t begin,
//                                   size_t end) const {
//
//    }
//
//    inline void push_task(ThreadPool& pool, std::vector<std::tuple<size_t, std::string, std::unordered_map<std::string,double>, size_t>>& futures,
//                          const std::string &action,
//                          size_t begin,
//                          size_t end) const {
//        futures.emplace_back(begin, dimension+"("+action+")", f.eval(orig, time, begin, end), end-begin+1);
//    }
};


static inline
std::tuple<RecordHandling,RecordHandling,RecordHandling,RecordHandling> transform_series(bool isDataless, const std::string& dim,
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
    return {{isDataless, dim+"_i", ls,timespan, std::move(increase_list)},
            {isDataless, dim+"_a", ls,timespan, std::move(absence_list)},
            {isDataless, dim+"_s",ls,timespan,std::move(stationary_list)},
            {isDataless, dim+"_v",ls,timespan,std::move(variability_list)}
    };
}


#endif //DTMINING_RECORDHANDLING_H
