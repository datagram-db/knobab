//
// Created by Sam on 24/01/2022.
//

#ifndef KNOBAB_TRACEDATA_H
#define KNOBAB_TRACEDATA_H

#include <knobab/dataStructures/oid.h>

using ResultIndex = std::pair<trace_t, event_t>;
using ResultRecordSemantics = std::pair<double, std::vector<event_t>>;
using ResultRecord = std::pair<ResultIndex, ResultRecordSemantics>;
using Result = std::vector<ResultRecord>;
using PartialResultRecord = std::pair<ResultIndex, double>;
using PartialResult = std::vector<PartialResultRecord>;

#endif //KNOBAB_TRACEDATA_H
