//
// Created by Sam on 08/02/2022.
//

#ifndef KNOBAB_LTLFOPERATORS_H
#define KNOBAB_LTLFOPERATORS_H

#include <knobab/operators/semantics.h>

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation> inline
OutputIt setUnion(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
                  OutputIt d_first, Aggregation aggr, const PredicateManager *manager = nullptr, bool dropMatches = false) {
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
                            double aggrV = aggr(first1->second.first, first2->second.first);
                            if (dropMatches) {
                                *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                                        aggr(first1->second.first, first2->second.first), {elem, elem1}});
                            } else {
                                *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                                        aggr(first1->second.first, first2->second.first), {}});
                            }
                        }
                    }
                }
            } else {
                std::vector<uint16_t> V;
                if (!dropMatches) {
                    first1->second.second;
                    V.insert(V.end(), first2->second.second.begin(), first2->second.second.end());
                    remove_duplicates(V);
                }
                *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                        aggr(first1->second.first, first2->second.first), V});
            }

            first1++;
            first2++;
        }
    }
    return std::copy(first2, last2, d_first);
}

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation> inline
OutputIt
setUnionUntimed(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Aggregation aggr,
                const PredicateManager *manager = nullptr, bool dropMatches = false) {
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
                                    if (dropMatches) {
                                        *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                                  std::pair<double, std::vector<uint16_t>>{
                                                                          aggr(cont1.second.first, cont2.second.first),
                                                                          {}});
                                    } else {

                                        *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                                  std::pair<double, std::vector<uint16_t>>{
                                                                          aggr(cont1.second.first, cont2.second.first),
                                                                          {pair.second, pair1.second}});
                                    }
                                }
                            }
                        }
                    } else {
                        // NOTE: that will discard potential activation and target conditions, and just put the two events where the situation holds.
                        std::vector<uint16_t> V;
                        if (!dropMatches) {
                            first1->second.second;
                            V.insert(V.end(), first2->second.second.begin(), first2->second.second.end());
                            remove_duplicates(V);
                        }
                        *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                  std::pair<double, std::vector<uint16_t>>{
                                                          aggr(cont1.second.first, cont2.second.first), V});
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

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation> inline
OutputIt setIntersection(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
                         OutputIt d_first, Aggregation aggr, const PredicateManager *manager = nullptr, bool dropMatches = false) {
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
                            if (dropMatches) {
                                *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                                        aggr(first1->second.first, first2->second.first), {}});
                            } else {
                                *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                                        aggr(first1->second.first, first2->second.first), {pair.second, pair1.second}});
                            }
                        }
                    }
                }
            } else {
                std::vector<uint16_t> V;
                if (!dropMatches) {
                    V = first1->second.second;
                    V.insert(V.end(), first2->second.second.begin(), first2->second.second.end());
                    remove_duplicates(V);
                }
                *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{
                        aggr(first1->second.first, first2->second.first), V});
            }

            first1++;
            first2++;
        }
    }
    return d_first;
}

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation> inline
OutputIt setIntersectionUntimed(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first,
                                Aggregation aggr,
                                const PredicateManager *manager = nullptr,
                                bool dropMatches = false) {
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
                                    if (dropMatches) {
                                        *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                                  std::pair<double, std::vector<uint16_t>>{
                                                                          aggr(cont1.second.first, cont2.second.first),
                                                                          {}});
                                    } else {
                                        *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                                  std::pair<double, std::vector<uint16_t>>{
                                                                          aggr(cont1.second.first, cont2.second.first),
                                                                          {pair.second, pair1.second}});
                                    }
                                }
                            }
                        }
                    } else {
                        // NOTE: that will discard potential activation and target conditions, and just put the two events where the situation holds.
                        std::vector<uint16_t> V;
                        if (!dropMatches) {
                            V = first1->second.second;
                            V.insert(V.end(), first2->second.second.begin(), first2->second.second.end());
                            remove_duplicates(V);
                        }
                        *d_first = std::make_pair(std::pair<uint32_t, uint16_t>{pair.first, 0},
                                                  std::pair<double, std::vector<uint16_t>>{
                                                          aggr(cont1.second.first, cont2.second.first),
                                                          V});
                    }


                }
            }

            start1++;
            start2++;
        }
    }

    return d_first;
}

template<typename TableSection> inline
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

template<typename TableSection> inline
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

template<typename TableSection> inline
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

template<typename TableSection> inline
dataContainer global(const TableSection &section, const std::vector<size_t>& lengths) {
    dataContainer temp {};
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;

        lower = upper;//std::lower_bound(upper, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, 0}, {0, {}}});
        upper = std::upper_bound(lower, section.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, lengths[currentTraceId] - 1}, {1, maxVec}});

        const uint32_t dist = std::distance(lower, upper - 1);

        if(dist == lengths[currentTraceId] - 1){
            std::vector<uint16_t> vec = populateAndReturnEvents(lower, upper);
            temp.emplace_back(std::pair<uint32_t, uint16_t>{currentTraceId, 0}, std::pair<double, std::vector<uint16_t>>{1, vec});
        }
    }

    return temp;
}

#include <iostream>


template<typename TableSection> inline
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

template<typename TableSection> inline
dataContainer future(const TableSection &section) {
    return section;
}

template<typename TableSection> inline
dataContainer negateUntimed(TableSection &data_untimed, const std::vector<size_t> &lengths, bool preserveNegatedFacts = true) {
    dataContainer result;
    size_t first1 = 0, last1 = lengths.size();
    auto first2 = data_untimed.begin(), last2 = data_untimed.end();
    for (; first1 != last1; ) {
        if (first2 == last2) {
            do {
                result.emplace_back(std::make_pair(first1++, 0), std::make_pair(1.0, std::vector<uint16_t>{}));
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

template<typename TableSection> inline
dataContainer until(const uint32_t &traceId,
                    const uint16_t &startEventId,
                    const uint16_t& endEventId,

                    const TableSection &aSection,
                    const TableSection &bSection,

                    const PredicateManager* manager = nullptr) {

    auto lower = std::lower_bound(bSection.begin(), bSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId}, {0, {}}});
    //auto localUpper = lower;
    auto upper = std::upper_bound(lower, bSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId},  {1, maxVec}});
    if(lower == upper){
        return {};
    }

    auto aIt = std::lower_bound(aSection.begin(), aSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, startEventId}, {0, {}}});
    auto aEn = aIt;//std::upper_bound(aIt, aSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId}, {1, maxVec}});
    auto aEnd = aSection.end();

    dataContainer temp {};
    env e1, e2;
    std::pair<uint32_t, uint16_t> Fut, Prev;

    for( ; lower != upper; lower++) {
        Fut.first = lower->first.first;
        if (lower->first.second == startEventId) {
            temp.emplace_back(*lower);
        } else {
            aEn = std::upper_bound(aEn, aEnd, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, lower->first.second-1},  {1, maxVec}});
            if(aIt == aEn){
                // Rationale: (1)
                // if the condition does not hold for a time [startEventId, lower->first.second-1], it is because one event makes it not hold.
                // Therefore, it should never hold even if you are extending the data that you have.
                return temp;
            } else {
                const uint32_t dist = std::distance(aIt, aEn - 1);
                std::vector<uint16_t> V;
                if(dist == ((lower->first.second) - startEventId)-1){
                    if (manager) {
                        for (uint16_t activationEvent : lower->second.second) {
                            Fut.second = activationEvent;
                            e1 = manager->GetPayloadDataFromEvent(Fut);
                            for (auto curr = aIt; curr != aEn; curr++) {
                                Prev.first = curr->first.first;
                                for (uint16_t targetEvent : curr->second.second) {
                                    Prev.second = targetEvent;
                                    e2 = manager->GetPayloadDataFromEvent(Prev);
                                    if (!manager->checkValidity(e2, e1)) {
                                        return temp;
                                    } else {
                                        V.emplace_back(targetEvent);
                                    }
                                }
                            }
                        }
                        V.insert(V.begin(), lower->second.second.begin(), lower->second.second.end());
                    } else {
                        V = populateAndReturnEvents(aIt, aEn);
                        V.insert(V.begin(), lower->second.second.begin(), lower->second.second.end());
                    }
                    temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, startEventId}, std::pair<double, std::vector<uint16_t>>{1,V});
                } else {
                    // For (1)
                    return temp;
                }
            }
        }

     }

    return temp;
}

template<typename TableSection> inline
dataContainer weakUntil(const uint32_t &traceId,
                    const uint16_t &startEventId,
                    const uint16_t& endEventId,

                    const TableSection &aSection,
                    const TableSection &bSection,

                    const PredicateManager* manager = nullptr) {
    dataContainer result;
    dataContainer untilR = until(traceId, startEventId, endEventId, aSection, bSection, manager);
    dataContainer globally = global(traceId, startEventId, endEventId, aSection);
    setUnion(untilR.begin(), untilR.end(), globally.begin(), globally.end(), std::back_inserter(result), Aggregators::joinSimilarity<double, double, double>, nullptr);
    return result;
}

template<typename TableSection> inline
dataContainer release(const uint32_t &traceId,
                        const uint16_t &startEventId,
                        const uint16_t& endEventId,

                        const TableSection &aSection,
                        const TableSection &bSection,

                        const PredicateManager* manager = nullptr) {

    dataContainer newB;
    dataContainer result;
    setIntersection(aSection.begin(), aSection.end(),
                    bSection.begin(), bSection.end(),
                    std::back_inserter(newB),
                    Aggregators::joinSimilarity<double,double,double>,
                    manager);

    if (manager) {
        auto flipped = manager->flip();
        weakUntil(traceId, startEventId, endEventId, bSection, newB, flipped);
    } else {
        weakUntil(traceId, startEventId, endEventId, bSection, newB, nullptr);
    }
    return result;
}

template<typename TableSection> inline
dataContainer until(const TableSection &aSection, const TableSection &bSection, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr) {
    auto lower = bSection.begin();
    auto localUpper = lower;
    auto upper = bSection.end();

    auto aIt = aSection.begin();
    auto aEn = aSection.begin();
    auto upperA = aSection.end();

    dataContainer temp {};
    env e1, e2;
    std::pair<uint32_t, uint16_t> Fut, Prev;

    while (lower != upper) {
        uint32_t currentTraceId = localUpper->first.first;

        //std::lower_bound(localUpper, upper, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, 0}, {0, {}}});
        localUpper = std::upper_bound(lower, upper, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, lengths[currentTraceId] - 1}, {1, maxVec}});

        aIt = std::lower_bound(aIt, upperA, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, 0}, {0, {}}});
        aEn = aIt;//std::upper_bound(aIt, aSection.end(), std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{traceId, endEventId}, {1, maxVec}});

        for( ; lower != localUpper; lower++) {
            Fut.first = lower->first.first;
            if (lower->first.second == 0) {
                temp.emplace_back(*lower);
            } else {
                aEn = std::upper_bound(aEn, upperA, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, lower->first.second-1},  {1, maxVec}});
                if(aIt == aEn){
                    // Rationale: (1)
                    // if the condition does not hold for a time [startEventId, lower->first.second-1], it is because one event makes it not hold.
                    // Therefore, it should never hold even if you are extending the data that you have.
                    break;
                } else {
                    const uint32_t dist = std::distance(aIt, aEn - 1);
                    std::vector<uint16_t> V;
                    if(dist == ((lower->first.second))-1){
                        if (manager) {
                            bool hasFail = false;
                            for (uint16_t activationEvent : lower->second.second) {
                                if (hasFail) break;
                                Fut.second = activationEvent;
                                e1 = manager->GetPayloadDataFromEvent(Fut);
                                for (auto curr = aIt; curr != aEn; curr++) {
                                    if (hasFail) break;
                                    Prev.first = curr->first.first;
                                    for (uint16_t targetEvent : curr->second.second) {
                                        Prev.second = targetEvent;
                                        e2 = manager->GetPayloadDataFromEvent(Prev);
                                        if (!manager->checkValidity(e2, e1)) {
                                            hasFail = true;
                                            break;
                                        } else {
                                            V.emplace_back(targetEvent);
                                        }
                                    }
                                }
                            }
                            if (hasFail) break;
                            std::sort(V.begin(), V.end());
                            V.erase(std::unique(V.begin(), V.end()), V.end());
                            V.insert(V.begin(), lower->second.second.begin(), lower->second.second.end());
                        } else {
                            V = populateAndReturnEvents(aIt, aEn);
                            V.insert(V.begin(), lower->second.second.begin(), lower->second.second.end());
                        }
                        temp.emplace_back(std::pair<uint32_t, uint16_t>{currentTraceId, 0}, std::pair<double, std::vector<uint16_t>>{1,V});
                    } else {
                        // For (1)
                        break;
                    }
                }
            }

        }


#if 0


        aIt = std::lower_bound(aEn, upperA, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, 0}, {0, {}}});
        aEn = std::upper_bound(aIt, upperA, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, lengths.at(currentTraceId)-1}, {1, maxVec}});



        for( ; aIt != aEn; aIt++) {
            if (aIt->first.second == 0) {
                temp.emplace_back(*aIt);
            } else {
                localUpper = std::upper_bound(localUpper, upper, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, aIt->first.second-1},  {1, maxVec}});
                if(lower == localUpper){
                    // Rationale: (1)
                    // if the condition does not hold for a time [startEventId, aIt->first.second-1], it is because one event makes it not hold.
                    // Therefore, it should never hold even if you are extending the data that you have.
                    break;
                } else {
                    const uint32_t dist = std::distance(lower, localUpper - 1);
                    std::vector<uint16_t> V;
                    if(dist == ((aIt->first.second)-1)){
                        if (manager) {
                            for (uint16_t activationEvent : aIt->second.second) {
                                Fut.second = activationEvent;
                                e1 = manager->GetPayloadDataFromEvent(Fut);
                                for (auto curr = lower; curr != localUpper; curr++) {
                                    Prev.first = curr->first.first;
                                    for (uint16_t targetEvent : curr->second.second) {
                                        Prev.second = targetEvent;
                                        e2 = manager->GetPayloadDataFromEvent(Prev);
                                        if (!manager->checkValidity(e1, e2)) {
                                            break;
                                        } else {
                                            V.emplace_back(targetEvent);
                                        }
                                    }
                                }
                            }
                            V.insert(V.begin(), aIt->second.second.begin(), aIt->second.second.end());
                        } else {
                            V = populateAndReturnEvents(lower, localUpper);
                            V.insert(V.begin(), aIt->second.second.begin(), aIt->second.second.end());
                        }
                        temp.emplace_back(std::pair<uint32_t, uint16_t>{currentTraceId, 0}, std::pair<double, std::vector<uint16_t>>{1,V});
                    } else {
                        // For (1)
                        break;
                    }
                }
            }

        }
#endif
        lower = localUpper;
    }

    return temp;
}

template<typename TableSection> inline
dataContainer weakUntil(const TableSection &aSection,
                               const TableSection &bSection,
                               const std::vector<size_t>& lengths,
                               const PredicateManager* manager = nullptr) {
    dataContainer result;
    dataContainer untilR = until(aSection, bSection, manager);
    dataContainer globally = global(aSection, lengths);
    setUnionUntimed(untilR.begin(), untilR.end(), globally.begin(), globally.end(), std::back_inserter(result), Aggregators::joinSimilarity<double, double, double>, nullptr);
    return result;
}

template<typename TableSection> inline
dataContainer release(const TableSection &psi,
                      const TableSection &phi,
                      const std::vector<size_t>& lengths,
                      const PredicateManager* manager = nullptr) {

    dataContainer intersection;
    dataContainer result;
    setIntersectionUntimed(psi.begin(), psi.end(),
                           phi.begin(), phi.end(),
                           std::back_inserter(intersection),
                           Aggregators::joinSimilarity<double,double,double>,
                           manager);

    if (manager) {
        auto flipped = manager->flip();
        weakUntil(phi, intersection, lengths, flipped);
    } else {
        weakUntil(phi, intersection, lengths, nullptr);
    }

    return result;
}


#endif //KNOBAB_LTLFOPERATORS_H
