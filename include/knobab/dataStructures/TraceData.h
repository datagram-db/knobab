//
// Created by Sam on 24/01/2022.
//

#ifndef KNOBAB_TRACEDATA_H
#define KNOBAB_TRACEDATA_H

#include <knobab/dataStructures/TaggedMatchedEvents.h>

using MarkedEventsVector = std::vector<marked_event>;
using ResultIndex = std::pair<trace_t, event_t>;
using ResultRecordSemantics = std::pair<double, MarkedEventsVector>;
using ResultRecord = std::pair<ResultIndex, ResultRecordSemantics>;
using Result = std::vector<ResultRecord>;
using PartialResultRecord = std::pair<ResultIndex, double>;
using PartialResult = std::vector<PartialResultRecord>;

#define RESULT_RECORD_MIN(name, tid, eid)         ResultRecord name{{tid, eid}, {0.0, {}}}
#define RESULT_RECORD_MAX(name, tid, eid)         ResultRecord name{{tid, eid}, {1.0, maxVec}}

#endif //KNOBAB_TRACEDATA_H
