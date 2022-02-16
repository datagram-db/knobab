//
// Created by Sam on 24/01/2022.
//

#ifndef KNOBAB_TRACEDATA_H
#define KNOBAB_TRACEDATA_H

#include <vector>
#include "knobab/predicates/PredicateManager.h"

template<typename traceIdentifier, typename traceValue>
class TraceData {
public:
    TraceData(){}

    TraceData(traceIdentifier id, traceValue v) : traceApproximations(id, v) {
    }

    template<typename InputIt2, typename OutputIt, typename Aggregation>
    OutputIt setUnion(InputIt2 first2, InputIt2 last2,
                      OutputIt d_first, Aggregation aggr, const PredicateManager& manager = {})
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

    std::vector<std::pair<traceIdentifier, traceValue>>& getTraceApproximations() {
        return traceApproximations;
    }

private:
    std::vector<std::pair<traceIdentifier, traceValue>> traceApproximations;
};


#endif //KNOBAB_TRACEDATA_H
