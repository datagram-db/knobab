//
// Created by Sam on 24/01/2022.
//

#ifndef KNOBAB_TRACEDATA_H
#define KNOBAB_TRACEDATA_H

#include <vector>
<<<<<<< HEAD
=======
#include <iostream>
#include "knobab/predicates/PredicateManager.h"
>>>>>>> sam

template<typename traceIdentifier, typename traceValue>
class TraceData {
public:
    TraceData(){}

    TraceData(traceIdentifier id, traceValue v) : traceApproximations(id, v) {
    }

<<<<<<< HEAD
    template<typename InputIt2, typename OutputIt, typename Aggregation>
    OutputIt setUnion(InputIt2 first2, InputIt2 last2,
                      OutputIt d_first, Aggregation aggr)
    {
        auto first1 = traceApproximations.begin(), last1 = traceApproximations.end();
        for (; first1 != last1; ++d_first) {
            if (first2 == last2)
                return std::copy(first1, last1, d_first);
            if (first1->first > first2->first) {
                *d_first = *first2++;
            } else if (first1->first < first2->first) {
                *d_first = *first1++;
            } else {
                *d_first = std::make_pair(first1->first, aggr(first1->second, first2->second));
                *first1++;
                *first2++;
            }
        }
        return std::copy(first2, last2, d_first);
    }

    template<typename InputIt2, typename OutputIt, typename Aggregation>
    OutputIt setIntersection(InputIt2 first2, InputIt2 last2,
                             OutputIt d_first, Aggregation aggr)
    {
        auto first1 = traceApproximations.begin(), last1 = traceApproximations.end();
        for (; first1 != last1; ++d_first) {
            if (first2 == last2)
                return  d_first;
            if (first1->first > first2->first) {
                first2++;
            } else if (first1->first < first2->first) {
                first1++;
            } else {
                *d_first = std::make_pair(first1->first, aggr(first1->second, first2->second));
                *first1++;
                *first2++;
            }
        }
        return d_first;
    }
=======

>>>>>>> sam

    std::vector<std::pair<traceIdentifier, traceValue>>& getTraceApproximations() {
        return traceApproximations;
    }

<<<<<<< HEAD
    std::vector<std::pair<traceIdentifier, traceValue>> traceApproximations;
private:
};

template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt setIntersection(InputIt1 first1, InputIt1 last1,
                        InputIt2 first2, InputIt2 last2,
                         OutputIt d_first, Aggregation aggr)
{
    for (; first1 != last1; ++d_first) {
        if (first2 == last2)
            return  d_first;
        if (first1->first > first2->first) {
            first2++;
        } else if (first1->first < first2->first) {
            first1++;
        } else {
            *d_first = std::make_pair(first1->first, aggr(first1->second, first2->second));
            *first1++;
            *first2++;
        }
    }
    return d_first;
}


template<typename InputIt1, typename InputIt2, typename OutputIt, typename Aggregation>
OutputIt setUnion(InputIt1 first1, InputIt1 last1,
                  InputIt2 first2, InputIt2 last2,
                  OutputIt d_first, Aggregation aggr)
{
    for (; first1 != last1; ++d_first) {
        if (first2 == last2)
            return std::copy(first1, last1, d_first);
        if (first1->first > first2->first) {
            *d_first = *first2++;
        } else if (first1->first < first2->first) {
            *d_first = *first1++;
        } else {
            *d_first = std::make_pair(first1->first, aggr(first1->second, first2->second));
            *first1++;
            *first2++;
        }
    }
    return std::copy(first2, last2, d_first);
}
=======
private:
    std::vector<std::pair<traceIdentifier, traceValue>> traceApproximations;
};

>>>>>>> sam

#endif //KNOBAB_TRACEDATA_H
