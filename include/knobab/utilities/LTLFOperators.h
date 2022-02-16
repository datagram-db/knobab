//
// Created by Sam on 08/02/2022.
//

#ifndef KNOBAB_LTLFOPERATORS_H
#define KNOBAB_LTLFOPERATORS_H

#include <optional>

const uint16_t max = std::numeric_limits<uint16_t>::max();
static const std::vector<uint16_t> maxVec(max,max);

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> next(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId},  {0, {}}});
    auto upper = std::upper_bound(lower, section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId},  {1, maxVec}});

    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> temp;

    while (lower != upper) {
        if(lower->first.second > 1){
            temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, lower->first.second - 1}, lower->second);
        }
        lower++;
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> next(const TableSection &section) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> temp;

    auto itr = section.elems.begin();
    while (itr != section.elems.end()) {
        if(itr->first.second != 1){
            temp.emplace_back(std::pair<uint32_t, uint16_t>{itr->first.first, itr->first.second - 1}, itr->second);
        }
        itr++;
    }

    return temp;
}

std::vector<uint16_t> populateAndReturnEvents(auto it1, auto it2){
    std::vector<uint16_t> vec;

    for(auto itr = it1; itr != it2; ++itr){
        for(const auto& r2 : itr->second.second){
            vec.emplace_back(r2);
        }
    }

    std::sort( vec.begin(), vec.end() );
    vec.erase( std::unique( vec.begin(), vec.end() ), vec.end());

    return vec;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> global(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId},  {0, {}}});
    auto upper = std::upper_bound(lower, section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId},  {1, maxVec}});

    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> temp;

    if(lower == upper){
        return temp;
    }

    const uint32_t dist = std::distance(lower, upper - 1);

    if(dist == (endEventId - startEventId)){
        std::vector<uint16_t> vec = populateAndReturnEvents(lower, upper);
        temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, startEventId}, std::pair<double, std::vector<uint16_t>>{1,vec});
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> global(const TableSection &section, const std::vector<size_t>& lengths) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> temp;
    auto lower = section.elems.begin(), upper = section.elems.begin();

    while(upper != section.elems.end()){
        uint32_t currentTraceId = upper->first.first;

        lower = std::lower_bound(upper, section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, 1}, {0, {}}});
        upper = std::upper_bound(lower, section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, lengths[currentTraceId]}, {1, maxVec}});

        const uint32_t dist = std::distance(lower, upper - 1);

        if(dist == lengths[currentTraceId] - 1){
            std::vector<uint16_t> vec = populateAndReturnEvents(lower, upper);
            temp.emplace_back(std::pair<uint32_t, uint16_t>{currentTraceId, 1}, std::pair<double, std::vector<uint16_t>>{1, vec});
        }
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>  future(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId}, {0, {}}});
    auto upper = std::upper_bound(section.elems.begin(), section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId}, {1, maxVec}});

    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>  temp;

    if(lower == upper){
        return temp;
    }

    while (lower != upper) {
        temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, lower->first.second}, lower->second);
        lower++;
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>  future(const TableSection &section) {
    return section.elems;
}

#endif //KNOBAB_LTLFOPERATORS_H
