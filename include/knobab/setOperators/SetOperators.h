//
// Created by Sam on 16/02/2022.
//

#ifndef KNOBAB_SETOPERATORS_H
#define KNOBAB_SETOPERATORS_H

#include "knobab/predicates/PredicateManager.h"

#include <map>
#include <functional>

struct example{
    std::vector<std::pair<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>> elems;
};

template<typename Iterator, typename Key, typename Value>
std::map<Key, std::vector<Value>>
GroupByKeyExtractor(Iterator begin, Iterator end, std::function<Key(const Value &)> keyExtractor) {
    assert(std::is_sorted(begin, end));
    std::map<Key, std::vector<Value>> groups;
    decltype(end) upper;

    for (auto lower = begin; lower != end; lower = upper) {
        Key k = keyExtractor(*lower);

        // get the upper position of all elements with the same ID
        upper = std::upper_bound(begin, end, *lower, [keyExtractor](const Value &x, const Value &y) {
            return keyExtractor(x) < keyExtractor(y);
        });

        // add those elements as a group to the output vector
        groups[k] = {lower, upper};
    }

    return groups;
}


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

            for (const auto &elem: first1->second.second) {
                pair.second = elem;

                for (const auto &elem1: first2->second.second) {
                    pair1.second = elem1;

                    if (manager) {
                        e1 = manager->GetPayloadDataFromEvent(pair);
                        e2 = manager->GetPayloadDataFromEvent(pair1);

                        if (manager->checkValidity(e1, e2)) {
                            *d_first = std::make_pair(pair, std::pair<double, std::vector<uint16_t>>{
                                    aggr(first1->second.first, first2->second.first), {elem, elem1}});
                        }
                    } else {
                        *d_first = std::make_pair(pair, std::pair<double, std::vector<uint16_t>>{
                                aggr(first1->second.first, first2->second.first), {elem, elem1}});
                    }
                }
            }
            first1++;
            first2++;
        }
    }
    return std::copy(first2, last2, d_first);
}

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt setUnionUntimed(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Aggregation aggr,
                const PredicateManager *manager = nullptr) {
    std::map<uint32_t, std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>> group1 =
            GroupByKeyExtractor<InputIt1,uint32_t, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>(
                    first1, last1,
                    [](const std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> &p) {
                        return p.first.first;
                    });

    std::map<uint32_t, std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>> group2 =
            GroupByKeyExtractor<InputIt1,uint32_t, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>(
                    first2, last2,
                    [](const std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> &p) {
                        return p.first.first;
                    });

    env e1, e2;
    std::pair<uint32_t, uint16_t> pair, pair1;
    auto start1 = group1.begin(), end1 = group1.end();
    auto start2 = group2.begin(), end2 = group2.end();

    for (; start1 != end1; ++d_first) {
        if (start2 == end2){
            while(start1 != end1) {
                for (const auto &cont1: start1->second) {
                    *d_first = cont1;
                }
                start1++;
            }
            return d_first;
        }
        else if (start1->first > start2->first) {
            for(const auto& cont1 : start2->second){
                *d_first = cont1;
            }
            start2++;
        } else if (start1->first < start2->first) {
            for(const auto& cont1 : start1->second){
                *d_first = cont1;
            }
            start1++;
        } else {
            pair.first = start1->first;
            pair1.first = start2->first;
            for(const auto& cont1 : start1->second){
                for(const auto& cont2 : start2->second){
                    for (const auto &elem: cont1.second.second) {
                        pair.second = elem;

                        for (const auto &elem1: cont2.second.second) {
                            pair1.second = elem1;

                            if (manager) {
                                e1 = manager->GetPayloadDataFromEvent(pair);
                                e2 = manager->GetPayloadDataFromEvent(pair1);


                                if (manager->checkValidity(e1, e2)) {
                                    *d_first = std::make_pair(pair, std::pair<double, std::vector<uint16_t>>{
                                            aggr(first1->second.first, first2->second.first), {elem, elem1}});
                                }
                            } else {
                                *d_first = std::make_pair(pair, std::pair<double, std::vector<uint16_t>>{
                                        aggr(first1->second.first, first2->second.first), {elem, elem1}});
                            }
                        }
                    }
                }
            }

            start1++;
            start2++;
        }
    }

    while(start2 != end2) {
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

            for (const auto &elem: first1->second.second) {
                pair.second = elem;

                for (const auto &elem1: first2->second.second) {
                    pair1.second = elem1;

                    if (manager) {
                        e1 = manager->GetPayloadDataFromEvent(pair);
                        e2 = manager->GetPayloadDataFromEvent(pair1);

                        if (manager->checkValidity(e1, e2)) {
                            *d_first = std::make_pair(pair, std::pair<double, std::vector<uint16_t>>{
                                    aggr(first1->second.first, first2->second.first), {elem, elem1}});
                        }
                    } else {
                        *d_first = std::make_pair(pair, std::pair<double, std::vector<uint16_t>>{
                                aggr(first1->second.first, first2->second.first), {elem, elem1}});
                    }
                }
            }

            first1++;
            first2++;
        }
    }
    return d_first;
}

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt setIntersectionUntimed(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Aggregation aggr,
                         const PredicateManager *manager = nullptr) {
    std::map<uint32_t, std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>> group1 =
            GroupByKeyExtractor<InputIt1,uint32_t, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>(
                    first1, last1,
                    [](const std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> &p) {
                        return p.first.first;
                    });

    std::map<uint32_t, std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>> group2 =
            GroupByKeyExtractor<InputIt1,uint32_t, std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>(
                    first2, last2,
                    [](const std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> &p) {
                        return p.first.first;
                    });

    env e1, e2;
    std::pair<uint32_t, uint16_t> pair, pair1;
    auto start1 = group1.begin(), end1 = group1.end();
    auto start2 = group2.begin(), end2 = group2.end();

    for (; start1 != end1; ++d_first) {
        if (start2 == end2){
            return d_first;
        }
        else if (start1->first > start2->first) {
            start2++;
        } else if (start1->first < start2->first) {
            start1++;
        } else {
            pair.first = start1->first;
            pair1.first = start2->first;
            for(const auto& cont1 : start1->second){
                for(const auto& cont2 : start2->second){
                    for (const auto &elem: cont1.second.second) {
                        pair.second = elem;

                        for (const auto &elem1: cont2.second.second) {
                            pair1.second = elem1;

                            if (manager) {
                                e1 = manager->GetPayloadDataFromEvent(pair);
                                e2 = manager->GetPayloadDataFromEvent(pair1);


                                if (manager->checkValidity(e1, e2)) {
                                    *d_first = std::make_pair(pair, std::pair<double, std::vector<uint16_t>>{
                                            aggr(first1->second.first, first2->second.first), {elem, elem1}});
                                }
                            } else {
                                *d_first = std::make_pair(pair, std::pair<double, std::vector<uint16_t>>{
                                        aggr(first1->second.first, first2->second.first), {elem, elem1}});
                            }
                        }
                    }
                }
            }

            start1++;
            start2++;
        }
    }

    return d_first;
}

example Choice(const example& a, const example& b, const PredicateManager* manager){
    auto cont1 = future(a);
    auto cont2 = future(b);
    example res;
    setUnionUntimed(cont1.begin(), cont1.end(), cont2.begin(), cont2.end(), std::back_inserter(res.elems), Aggregators::maxSimilarity<double, double, double>, manager);
    return res;
}

#endif //KNOBAB_SETOPERATORS_H
