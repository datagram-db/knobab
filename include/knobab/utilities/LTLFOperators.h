//
// Created by Sam on 08/02/2022.
//

#ifndef KNOBAB_LTLFOPERATORS_H
#define KNOBAB_LTLFOPERATORS_H

#include <optional>

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> next(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), (std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(traceId, startEventId), 0))) + 1;
    auto upper = std::upper_bound(lower, section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(traceId, endEventId), 1));

    while (lower != upper) {
        temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, lower->first.second - 1}, 1);
        lower++;
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> next(const TableSection &section) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    auto itr = section.elems.begin();
    while (itr != section.elems.end()) {
        if(itr->first.second != 1){
            temp.emplace_back(std::pair<uint32_t, uint16_t>{itr->first.first, itr->first.second - 1}, 1);
        }
        itr++;
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> global(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(traceId, startEventId), 0));
    auto upper = std::upper_bound(lower, section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(traceId, endEventId), 1));

    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    if(lower == upper){
        return temp;
    }

    const uint32_t dist = std::distance(lower, upper - 1);

    if(dist == (endEventId - startEventId)){
        temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, startEventId}, 1);
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> global(const TableSection &section, const std::vector<size_t>& lengths) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;
    uint32_t currentTraceId = section.elems.begin()->first.first;
    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(currentTraceId, 0), 0));
    auto upper = std::upper_bound(lower, section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(currentTraceId, lengths[currentTraceId]), 1));

    while(lower != section.elems.end()){
        if(std::abs(upper - lower) == lengths[section.elems.begin()->first.first]){
            temp.emplace_back(std::pair<uint32_t, uint16_t>{currentTraceId, 1}, 1);
        }

        currentTraceId = (upper + 1)->first.first;
        lower = std::lower_bound(upper, section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(currentTraceId, 1), 0));
        upper = std::upper_bound(lower, section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(currentTraceId, lengths[currentTraceId]), 1));
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> future(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(traceId, startEventId), 0));
    auto upper = std::upper_bound(section.elems.begin(), section.elems.end(), std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(traceId, endEventId), 1));

    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    if(lower == upper){
        return temp;
    }

    while (lower != upper) {
        temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, lower->first.second}, 1);
        lower++;
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> future(const TableSection &section) {
    return section.elems;
}

#endif //KNOBAB_LTLFOPERATORS_H
