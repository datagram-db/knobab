//
// Created by Sam on 08/02/2022.
//

#ifndef KNOBAB_LTLFOPERATORS_H
#define KNOBAB_LTLFOPERATORS_H

#include <optional>

const uint16_t max = std::numeric_limits<uint16_t>::max();
static const std::vector<uint16_t> maxVec(max,max);

template<typename TableSection>
dataContainer next(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    auto lower = std::lower_bound(section.begin(), section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId},  {0, {}}});
    auto upper = std::upper_bound(lower, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId},  {1, maxVec}});

    dataContainer temp {};

    while (lower != upper) {
        if(lower->first.second > 0){
            temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, lower->first.second - 1}, lower->second);
        }
        lower++;
    }

    return temp;
}

template<typename TableSection>
dataContainer next(const TableSection &section) {
    dataContainer temp;

    auto itr = section.begin();
    while (itr != section.end()) {
        if(itr->first.second > 0){
            temp.emplace_back(std::pair<uint32_t, uint16_t>{itr->first.first, itr->first.second - 1}, itr->second);
        }
        itr++;
    }

    return temp;
}

std::vector<uint16_t> populateAndReturnEvents(auto it1, auto it2, const PredicateManager* manager = nullptr){
    std::vector<uint16_t> vec;

    for(auto itr = it1; itr != it2; ++itr){
        for(const auto& r2 : itr->second.second){
                vec.push_back(r2);
        }
    }

    std::sort( vec.begin(), vec.end() );
    vec.erase( std::unique( vec.begin(), vec.end() ), vec.end());

    return vec;
}

template<typename TableSection>
dataContainer global(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    auto lower = std::lower_bound(section.begin(), section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId},  {0, {}}});
    auto upper = std::upper_bound(lower, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId},  {1, maxVec}});

    dataContainer temp {};

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
dataContainer global(const TableSection &section, const std::vector<size_t>& lengths) {
    dataContainer temp {};
    auto lower = section.begin(), upper = section.begin();

    while(upper != section.end()){
        uint32_t currentTraceId = upper->first.first;

        lower = std::lower_bound(upper, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, 0}, {0, {}}});
        upper = std::upper_bound(lower, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, lengths[currentTraceId] - 1}, {1, maxVec}});

        const uint32_t dist = std::distance(lower, upper - 1);

        if(dist == lengths[currentTraceId] - 1){
            std::vector<uint16_t> vec = populateAndReturnEvents(lower, upper);
            temp.emplace_back(std::pair<uint32_t, uint16_t>{currentTraceId, 0}, std::pair<double, std::vector<uint16_t>>{1, vec});
        }
    }

    return temp;
}

template<typename TableSection>
dataContainer future(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &section) {
    auto lower = std::lower_bound(section.begin(), section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId}, {0, {}}});
    auto upper = std::upper_bound(section.begin(), section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId}, {1, maxVec}});

    dataContainer  temp {};

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
dataContainer future(const TableSection &section) {
    return section;
}

template<typename TableSection>
dataContainer until(const uint32_t &traceId, const uint16_t &startEventId, const uint16_t& endEventId, const TableSection &aSection, const TableSection &bSection, const PredicateManager* manager = nullptr) {
    dataContainer aBSection {};
    setUnion(true, aSection.begin(), aSection.end(), bSection.begin(), bSection.end(), std::back_inserter(aBSection), Aggregators::maxSimilarity<double, double, double>, manager);

    auto bLower = std::lower_bound(bSection.begin(), bSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId}, {0, {}}});

    if(bLower == bSection.end()){
        return {};
    }

    auto aBLower = std::lower_bound(aBSection.begin(), aBSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId}, {0, {}}});
    auto aBLowerIncr = aBLower;
    auto aBUpper = std::upper_bound(aBSection.begin(), aBSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId}, {1, maxVec}});

    auto aLower = std::lower_bound(aSection.begin(), aSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId}, {0, {}}});

    dataContainer temp {};

    while (aBLowerIncr != aBUpper) {
        if(*aBLowerIncr != *aLower){
            if(*aBLowerIncr == *bLower){
                std::vector<uint16_t> vec = populateAndReturnEvents(aBLower, aBUpper);
                temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, 0}, std::pair<double, std::vector<uint16_t>>{1, vec});
                return temp;
            }
            else{
                break;
            }
        }
        aLower++;
        aBLowerIncr++;
    }

    return {};
}

template<typename TableSection>
dataContainer until(const TableSection &aSection, const TableSection &bSection, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr) {
    dataContainer aBSection {};
    setUnion(true, aSection.begin(), aSection.end(), bSection.begin(), bSection.end(), std::back_inserter(aBSection), Aggregators::maxSimilarity<double, double, double>, manager);

    auto aBLower = aBSection.begin(), aBUpper = aBSection.begin();

    dataContainer temp {};

    while(aBUpper != aBSection.end()){
        uint32_t currentTraceId = aBUpper->first.first;

        aBLower = std::lower_bound(aBUpper, aBSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, 0}, {0, {}}});
        auto aBLowerIncr = aBLower;
        aBUpper = std::upper_bound(aBLower, aBSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, lengths[currentTraceId]}, {1, maxVec}});

        auto aLower = std::lower_bound(aSection.begin(), aSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, aBLower->first.second}, {0, {}}});
        auto bLower = std::lower_bound(bSection.begin(), bSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, aBLower->first.second}, {0, {}}});

        while (aBLowerIncr != aBUpper) {
            if(*aBLowerIncr != *aLower){
                if(*aBLowerIncr == *bLower){
                    std::vector<uint16_t> vec = populateAndReturnEvents(aBLower, aBUpper);
                    temp.emplace_back(std::pair<uint32_t, uint16_t>{currentTraceId, 0}, std::pair<double, std::vector<uint16_t>>{1, vec});
                    return temp;
                }
                else{
                    break;
                }
            }
            aLower++;
            aBLowerIncr++;
        }
    }

    return temp;
}

#endif //KNOBAB_LTLFOPERATORS_H
