//
// Created by Sam on 08/02/2022.
//

#ifndef KNOBAB_LTLFOPERATORS_H
#define KNOBAB_LTLFOPERATORS_H

#include <optional>

const uint16_t max = std::numeric_limits<uint16_t>::max();
static const std::vector<uint16_t> maxVec(max,max);

#include <knobab/predicates/PredicateManager.h>
#include <knobab/utilities/Aggregators.h>

#include <map>
#include <functional>

using dataContainer = std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>;

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt setUnion(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
                  OutputIt d_first, Aggregation aggr, const PredicateManager *manager = nullptr) {
    env e1, e2;
    std::pair<uint32_t, uint16_t> pair, pair1;

    for (; first1 != last1; ++d_first) {
        if (first2 == last2)
            return std::copy(first1, last1, d_first);
        if (first1->first > first2->first) {
            *d_first = *first2++;
        } else if (first1->first < first2->first) {
            *d_first = *first1++;
        } else {
            pair.first = first1->first.first;
            pair1.first = first2->first.first;

            if (manager) {
                e1 = manager->GetPayloadDataFromEvent(pair);
                e2 = manager->GetPayloadDataFromEvent(pair1);

                for (const auto &elem: first1->second.second) {
                    pair.second = elem;

                    for (const auto &elem1: first2->second.second) {
                        pair1.second = elem1;

                        if (manager->checkValidity(e1, e2)) {
                            *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                                    aggr(first1->second.first, first2->second.first), {elem, elem1}});
                        }
                    }
                }
            } else {
                *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                        aggr(first1->second.first, first2->second.first), {}});
            }

            first1++;
            first2++;
        }
    }
    return std::copy(first2, last2, d_first);
}

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt
setUnionUntimed(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Aggregation aggr,
                const PredicateManager *manager = nullptr) {
    std::map<uint32_t, dataContainer> group1 = GroupByKeyExtractor<InputIt1, uint32_t, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>(
            first1, last1,
            [](const std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> &p) {
                return p.first.first;
            });

    std::map<uint32_t, dataContainer> group2 = GroupByKeyExtractor<InputIt2, uint32_t, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>(
            first2, last2,
            [](const std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> &p) {
                return p.first.first;
            });

    env e1, e2;
    std::pair<uint32_t, uint16_t> pair, pair1;
    auto start1 = group1.begin(), end1 = group1.end();
    auto start2 = group2.begin(), end2 = group2.end();

    for (; start1 != end1; ++d_first) {
        if (start2 == end2) {
            while (start1 != end1) {
                for (const auto &cont1: start1->second) {
                    *d_first = cont1;
                }
                start1++;
            }
            return d_first;
        } else if (start1->first > start2->first) {
            for (const auto &cont1: start2->second) {
                *d_first = cont1;
            }
            start2++;
        } else if (start1->first < start2->first) {
            for (const auto &cont1: start1->second) {
                *d_first = cont1;
            }
            start1++;
        } else {
            pair.first = start1->first;
            pair1.first = start2->first;
            for (const auto &cont1: start1->second) {
                for (const auto &cont2: start2->second) {
                    if (manager) {
                        for (const auto &elem: cont1.second.second) {
                            pair.second = elem;
                            for (const auto &elem1: cont2.second.second) {
                                pair1.second = elem1;
                                e1 = manager->GetPayloadDataFromEvent(pair);
                                e2 = manager->GetPayloadDataFromEvent(pair1);

                                if (manager->checkValidity(e1, e2)) {
                                    *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                              std::pair<double, std::vector<uint16_t>>{
                                                                      aggr(cont1.second.first, cont2.second.first),
                                                                      {pair.second, pair1.second}});
                                }
                            }
                        }
                    } else {
                        // NOTE: that will discard potential activation and target conditions, and just put the two events where the situation holds.
                        *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                  std::pair<double, std::vector<uint16_t>>{
                                                          aggr(cont1.second.first, cont2.second.first), {}});
                    }
                }
            }

            start1++;
            start2++;
        }
    }

    while (start2 != end2) {
        for (const auto &cont1: start2->second) {
            *d_first = cont1;
        }
        start2++;
    }
    return d_first;
}

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt setIntersection(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
                         OutputIt d_first, Aggregation aggr, const PredicateManager *manager = nullptr) {
    env e1, e2;
    std::pair<uint32_t, uint16_t> pair, pair1;

    for (; first1 != last1; ++d_first) {
        if (first2 == last2)
            return d_first;
        if (first1->first > first2->first) {
            first2++;
        } else if (first1->first < first2->first) {
            first1++;
        } else {
            pair.first = first1->first.first;
            pair1.first = first2->first.first;

            if (manager) {
                for (const auto &elem: first1->second.second) {
                    pair.second = elem;

                    for (const auto &elem1: first2->second.second) {
                        pair1.second = elem1;

                        e1 = manager->GetPayloadDataFromEvent(pair);
                        e2 = manager->GetPayloadDataFromEvent(pair1);

                        if (manager->checkValidity(e1, e2)) {
                            *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                                    aggr(first1->second.first, first2->second.first), {pair.second, pair1.second}});
                        }
                    }
                }
            } else {
                *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                        aggr(first1->second.first, first2->second.first), {}});
            }

            first1++;
            first2++;
        }
    }
    return d_first;
}

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt setIntersectionUntimed(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first,
                                Aggregation aggr,
                                const PredicateManager *manager = nullptr) {
    std::map<uint32_t, dataContainer> group1 = GroupByKeyExtractor<InputIt1, uint32_t, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>(
            first1, last1,
            [](const std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> &p) {
                return p.first.first;
            });

    std::map<uint32_t, dataContainer> group2 = GroupByKeyExtractor<InputIt2, uint32_t, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>(
            first2, last2,
            [](const std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> &p) {
                return p.first.first;
            });

    env e1, e2;
    std::pair<uint32_t, uint16_t> pair, pair1;
    auto start1 = group1.begin(), end1 = group1.end();
    auto start2 = group2.begin(), end2 = group2.end();

    for (; start1 != end1; ++d_first) {
        if (start2 == end2) {
            return d_first;
        } else if (start1->first > start2->first) {
            start2++;
        } else if (start1->first < start2->first) {
            start1++;
        } else {
            pair.first = start1->first;
            pair1.first = start2->first;
            for (const auto &cont1: start1->second) {
                for (const auto &cont2: start2->second) {
                    if (manager) {
                        for (const auto &elem: cont1.second.second) {
                            pair.second = elem;

                            for (const auto &elem1: cont2.second.second) {
                                pair1.second = elem1;
                                e1 = manager->GetPayloadDataFromEvent(pair);
                                e2 = manager->GetPayloadDataFromEvent(pair1);

                                if (manager->checkValidity(e1, e2)) {
                                    *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                              std::pair<double, std::vector<uint16_t>>{
                                                                      aggr(cont1.second.first, cont2.second.first),
                                                                      {pair.second, pair1.second}});
                                }
                            }
                        }
                    } else {
                        // NOTE: that will discard potential activation and target conditions, and just put the two events where the situation holds.
                        *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                  std::pair<double, std::vector<uint16_t>>{
                                                          aggr(cont1.second.first, cont2.second.first),
                                                          {pair.second, pair1.second}});
                    }


                }
            }

            start1++;
            start2++;
        }
    }

    return d_first;
}

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
dataContainer negateUntimed(TableSection &data_untimed, const std::vector<size_t> &lengths, bool preserveNegatedFacts = true) {
    dataContainer result;
    size_t first1 = 0, last1 = lengths.size();
    auto first2 = data_untimed.begin(), last2 = data_untimed.end();
    for (; first1 != last1; ) {
        if (first2 == last2) {
            do {
                result.emplace_back(std::make_pair(first1, 0), std::make_pair(1.0, std::vector<uint16_t>{}));
            } while (first1 != last1);
        }
        if (first1 > first2->first.first) {
            first2++;
        } else if (first1 < first2->first.first) {
            result.emplace_back(std::make_pair(first1, 0), std::make_pair(1.0, std::vector<uint16_t>{}));
            first1++;
        } else {
            // MEMO: if you want to preserve the condition where it didn't hold for repairs or givin advices, then you should return a result having 0, and containing the result of the match
            if (preserveNegatedFacts || (first2->second.first <= std::numeric_limits<double>::epsilon())) {
                auto tmp = *first2;
                tmp.second.first = 1.0 - tmp.second.first;
                result.push_back(tmp);
            }
            first1++;
            first2++;
        }
    }
    return result;
}

template<typename TableSection>
dataContainer until(const uint32_t &traceId,
                    const uint16_t &startEventId,
                    const uint16_t& endEventId,

                    const TableSection &aSection,
                    const TableSection &bSection,

                    const PredicateManager* manager = nullptr) {

    auto lower = std::lower_bound(bSection.begin(), bSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId}, {0, {}}});
    auto localUpper = lower;
    auto upper = std::upper_bound(lower, bSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId},  {1, maxVec}});
    if(upper == bSection.end()){
        return {};
    }

    auto aIt = std::lower_bound(aSection.begin(), aSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId}, {0, {}}});
    auto aEn = std::upper_bound(aIt, aSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId}, {1, maxVec}});

    dataContainer temp {};

    for( ; aIt != aEn; aIt++) {
        if (aIt->first.second == startEventId) {
            temp.emplace_back(*aIt);
        } else {
            localUpper = std::upper_bound(localUpper, upper, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, aIt->first.second-1},  {1, maxVec}});
            if(lower == localUpper){
                // Rationale: (1)
                // if the condition does not hold for a time [startEventId, aIt->first.second-1], it is because one event makes it not hold.
                // Therefore, it should never hold even if you are extending the data that you have.
                return temp;
            } else {
                const uint32_t dist = std::distance(lower, upper - 1);
                if(dist == ((aIt->first.second) - startEventId)){
                    std::vector<uint16_t> vec = populateAndReturnEvents(lower, aIt);
                    temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, startEventId}, std::pair<double, std::vector<uint16_t>>{1,vec});
                } else {
                    // For (1)
                    return temp;
                }
            }
        }

     }

    return temp;
}

template<typename TableSection>
dataContainer until(const TableSection &aSection, const TableSection &bSection, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr) {
    typename TableSection::iterator lower = bSection.begin();
    typename TableSection::iterator localUpper = lower;
    typename TableSection::iterator upper = bSection.end();

    typename TableSection::iterator aIt = aSection.begin();
    typename TableSection::iterator aEn = aSection.begin();
    typename TableSection::iterator upperA = aSection.begin();

    dataContainer temp {};

    for (uint32_t traceId = 0, N = (uint32_t)lengths.size(); traceId < N; traceId++) {
        lower = std::lower_bound(lower, bSection.end(),
                                 std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, 0},
                                                                                                                    {0,       {}}});
        localUpper = std::upper_bound(lower, upper, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, lengths.at(traceId)-1},  {1, maxVec}});;

        if(upper == bSection.end()){
            continue;
        }

        aIt = std::lower_bound(aEn, upperA, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, 0}, {0, {}}});
        aEn = std::upper_bound(aIt, upperA, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, lengths.at(traceId)-1}, {1, maxVec}});


        for( ; aIt != aEn; aIt++) {
            if (aIt->first.second == 0) {
                temp.emplace_back(*aIt);
            } else {
                localUpper = std::upper_bound(localUpper, upper, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, aIt->first.second-1},  {1, maxVec}});
                if(lower == localUpper){
                    // Rationale: (1)
                    // if the condition does not hold for a time [startEventId, aIt->first.second-1], it is because one event makes it not hold.
                    // Therefore, it should never hold even if you are extending the data that you have.
                    break;
                } else {
                    const uint32_t dist = std::distance(lower, upper - 1);
                    if(dist == ((aIt->first.second))){
                        std::vector<uint16_t> vec = populateAndReturnEvents(lower, aIt);
                        temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, 0}, std::pair<double, std::vector<uint16_t>>{1,vec});
                    } else {
                        // For (1)
                        break;
                    }
                }
            }

        }
    }

    return temp;
}

#endif //KNOBAB_LTLFOPERATORS_H
