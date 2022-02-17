//
// Created by Sam on 16/02/2022.
//

#ifndef KNOBAB_OPERATORS_H
#define KNOBAB_OPERATORS_H


#include "knobab/predicates/PredicateManager.h"
template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt setUnion(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
                  OutputIt d_first, Aggregation aggr, const PredicateManager* manager = nullptr)
{
    env e1, e2;
    std::pair<uint32_t , uint16_t > pair, pair1;

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

            for(const auto& elem : first1->second.second){
                pair.second = elem;

                for(const auto& elem1 : first2->second.second){
                    pair1.second = elem1;
                    e1 = manager->GetPayloadDataFromEvent(pair);
                    e2 = manager->GetPayloadDataFromEvent(pair1);

                    if(manager->checkValidity(e1, e2)){
                        *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{aggr(first1->second.first, first2->second.first), {elem, elem1}});
                    }
                }
            }
            *first1++;
            *first2++;
        }
    }
    return std::copy(first2, last2, d_first);
}

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt setIntersection(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
                         OutputIt d_first, Aggregation aggr, const PredicateManager* manager)
{
    env e1, e2;
    std::pair<uint32_t , uint16_t > pair, pair1;

    for (; first1 != last1; ++d_first) {
        if (first2 == last2)
            return  d_first;
        if (first1->first > first2->first) {
            first2++;
        } else if (first1->first < first2->first) {
            first1++;
        } else {
            pair.first = first1->first.first;
            pair1.first = first2->first.first;

            for(const auto& elem : first1->second.second){
                pair.second = elem;

                for(const auto& elem1 : first2->second.second){
                    pair1.second = elem1;
                    e1 = manager->GetPayloadDataFromEvent(pair);
                    e2 = manager->GetPayloadDataFromEvent(pair1);

                    if(manager->checkValidity(e1, e2)){
                        *d_first = std::make_pair(first1->first, std::pair<double, std::vector<uint16_t>>{aggr(first1->second.first, first2->second.first), {elem, elem1}});
                    }
                }
            }

            *first1++;
            *first2++;
        }
    }
    return d_first;
}

#endif //KNOBAB_OPERATORS_H
