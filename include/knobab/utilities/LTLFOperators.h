//
// Created by Sam on 08/02/2022.
//

#ifndef KNOBAB_LTLFOPERATORS_H
#define KNOBAB_LTLFOPERATORS_H

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> next(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), ((traceId, startEventId), 0));
    auto upper = std::lower_bound(section.elems.begin(), section.elems.end(), ((traceId, endEventId), 1));

    while (lower != upper) {
        assert(lower->first.second <= 1);       // If we are trying to decrement the first element of the trace
        temp.emplace_back(std::pair<uint32_t, uint16_t>{lower->first.first, lower->first.second - 1}, 1);
        lower++;
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> next(const TableSection &section) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    auto itr = section.begin();
    while (itr != section.end()) {
        if(itr->first.second != 1){
            temp.emplace_back(std::pair<uint32_t, uint16_t>{itr->first.first, itr->first.second - 1}, 1);
        }
        itr++;
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> global(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, TableSection &section) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), ((traceId, startEventId), 0));
    auto upper = std::lower_bound(section.elems.begin(), section.elems.end(), ((traceId, endEventId), 1));

    const uint32_t dist = std::distance(lower, upper);

    if(dist && std::distance(lower, upper) == (endEventId - startEventId)){
        temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, startEventId}, 1);
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> global(TableSection &section) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    auto itr = section.begin();
    uint32_t currentTraceId = itr->first.first;
    bool patternHolds = true;
    while (itr != section.end()) {
        if(itr->first.first != currentTraceId){
            if(patternHolds) {
                temp.emplace_back(std::pair<uint32_t, uint16_t>{currentTraceId, (itr--)->first.second}, 1);
            }
            currentTraceId = itr->first.first;
            patternHolds = true;
        }
        else if(itr->first.second - (itr--)->first.second > 1){
            patternHolds = false;
        }
        itr++;
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> future(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, TableSection &section) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    auto lower = std::lower_bound(section.elems.begin(), section.elems.end(), ((traceId, startEventId), 0));
    auto upper = std::lower_bound(section.elems.begin(), section.elems.end(), ((traceId, endEventId), 1));

    while (lower != upper) {
        temp.emplace_back(std::pair<uint32_t, uint16_t>{lower->first.first, lower->first.second}, 1);
        lower++;
    }

    return temp;
}

template<typename TableSection>
std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> future(TableSection &section) {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> temp;

    auto itr = section.begin();
    while (itr != section.end()) {
        temp.emplace_back(std::pair<uint32_t, uint16_t>{itr->first.first, itr->first.second}, 1);
        itr++;
    }

    return temp;
}



#endif //KNOBAB_LTLFOPERATORS_H
