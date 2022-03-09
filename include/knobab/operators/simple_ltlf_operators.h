//
// Created by giacomo on 09/03/2022.
//

#ifndef KNOBAB_SIMPLE_LTLF_OPERATORS_H
#define KNOBAB_SIMPLE_LTLF_OPERATORS_H

#include <knobab/operators/semantics.h>

inline void future_logic_timed(const dataContainer &section, dataContainer& result) {
    //std::cout << section << std::endl;
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    constexpr auto max16 = std::numeric_limits<uint16_t>::max();

    std::pair<uint32_t, uint16_t> first;
    std::pair<double, std::vector<uint16_t>> second{0.0, 0.0};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;

        lower = upper;//std::lower_bound(upper, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, 0}, {0, {}}});
        upper = std::upper_bound(lower, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, max16}, {1, maxVec}});

        dataContainer toBeReversed;
        first.first = currentTraceId;
        first.second = 0;

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

    //std::cout << temp << std::endl;
    //return result;
}

inline void future_logic_untimed(const dataContainer &section, dataContainer& result) {
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    constexpr auto max16 = std::numeric_limits<uint16_t>::max();

    std::pair<uint32_t, uint16_t> first{0, 0};
    std::pair<double, std::vector<uint16_t>> second{0.0, {}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = currentTraceId;
        second.second.clear();
        second.first = 0.0;

        lower = upper;//std::lower_bound(upper, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, stauto it = upper-1; it != lower-1; it--d::vector<uint16_t>>>{{currentTraceId, 0}, {0, {}}});
        upper = std::upper_bound(lower, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, max16}, {1, maxVec}});

        for (auto it = lower+std::distance(lower,upper)-1; it >= lower; it--) {
            second.first = std::max(second.first, it->second.first);
            second.second.insert(second.second.end(), it->second.second.begin(), it->second.second.end());
        }
        remove_duplicates(second.second);
        result.emplace_back(first, second);
    }

    //return result;
}

#endif //KNOBAB_SIMPLE_LTLF_OPERATORS_H
