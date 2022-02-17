//
// Created by Sam on 24/01/2022.
//

#ifndef KNOBAB_TRACEDATA_H
#define KNOBAB_TRACEDATA_H

#include <vector>
#include <iostream>
#include "knobab/predicates/PredicateManager.h"

template<typename traceIdentifier, typename traceValue>
class TraceData {
public:
    TraceData(){}

    TraceData(traceIdentifier id, traceValue v) : traceApproximations(id, v) {
    }



    std::vector<std::pair<traceIdentifier, traceValue>>& getTraceApproximations() {
        return traceApproximations;
    }

private:
    std::vector<std::pair<traceIdentifier, traceValue>> traceApproximations;
};


#endif //KNOBAB_TRACEDATA_H
