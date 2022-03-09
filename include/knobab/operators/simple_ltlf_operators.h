//
// Created by giacomo on 09/03/2022.
//

#ifndef KNOBAB_SIMPLE_LTLF_OPERATORS_H
#define KNOBAB_SIMPLE_LTLF_OPERATORS_H

#include <knobab/operators/semantics.h>

inline void future_logic_timed(const dataContainer &section, const std::vector<size_t>& lengths, dataContainer& result) {
    //std::cout << section << std::endl;
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    constexpr auto max16 = std::numeric_limits<uint16_t>::max();

    std::pair<uint32_t, uint16_t> first;
    std::pair<double, std::vector<uint16_t>> second{0.0, 0.0};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cp{{0,0}, {1.0, {}}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = cp.first.first = currentTraceId;
        cp.first.second = lengths.at(currentTraceId);
        first.second = 0;

        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp);

        dataContainer toBeReversed;
        auto it = lower+std::distance(lower,upper)-1;
        bool isBegin = true;
        for (int64_t i = (upper-1)->first.second; i >= 0; i--) {
            first.second = i;
            if (it->first.second == i) {
                second.first = std::max(it->second.first, second.first);
                second.second.insert(second.second.begin(), it->second.second.begin(), it->second.second.end());
                remove_duplicates(second.second);
                if (isBegin) isBegin = false;
                it--;
            } else {
                assert(!isBegin);
            }
            toBeReversed.emplace_back(first, second);
        }

        // Inserting the elements in reversed order
        result.insert(result.end(), std::make_move_iterator(toBeReversed.rbegin()),
                    std::make_move_iterator(toBeReversed.rend()));

        second.second.clear();
    }
}

inline void future_logic_untimed(const dataContainer &section, const std::vector<size_t>& lengths, dataContainer& result) {
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    constexpr auto max16 = std::numeric_limits<uint16_t>::max();

    std::pair<uint32_t, uint16_t> first{0, 0};
    std::pair<double, std::vector<uint16_t>> second{0.0, {}};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cp{{0,0}, {1.0, {}}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = cp.first.first = currentTraceId;
        cp.first.second = lengths.at(currentTraceId);
        second.second.clear();
        second.first = 0.0;

        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp);

        for (auto it = lower+std::distance(lower,upper)-1; it >= lower; it--) {
            second.first = std::max(second.first, it->second.first);
            second.second.insert(second.second.end(), it->second.second.begin(), it->second.second.end());
        }
        remove_duplicates(second.second);
        result.emplace_back(first, second);
    }
}

inline void global_logic_untimed(const dataContainer &section, const std::vector<size_t>& lengths, dataContainer& result) {
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    result.clear();

    std::pair<uint32_t, uint16_t> first{0, 0}, firstLast;
    std::pair<double, std::vector<uint16_t>> second{1.0, {}};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cp{{0,0}, {1.0, {}}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = cp.first.first = currentTraceId;
        cp.first.second = lengths.at(currentTraceId);
        cp.second.second.clear();

        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp);

        const uint32_t dist = std::distance(lower, upper - 1);

        if(dist == cp.first.second - 1){
            populateAndReturnEvents(lower, upper, second.second);
            result.emplace_back(first, second);
        }
    }
}

#endif //KNOBAB_SIMPLE_LTLF_OPERATORS_H
