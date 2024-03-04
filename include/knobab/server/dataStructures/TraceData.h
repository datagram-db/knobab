//
// Created by giacomo on 16/04/2022.
//

#ifndef KNOBAB_SERVER_TRACEDATA_H
#define KNOBAB_SERVER_TRACEDATA_H


#include <knobab/server/dataStructures/marked_event.h>
#include <vector>
#include <utility>

using MarkedEventsVector = std::vector<marked_event>;
using ResultIndex = std::pair<trace_t, event_t>;
using ResultRecordSemantics = std::pair<event_t, MarkedEventsVector>;
using ResultRecord = std::pair<ResultIndex, ResultRecordSemantics>;
using Result = std::vector<ResultRecord>;
using PartialResultRecord = std::pair<ResultIndex, event_t>;
using PartialResult = std::vector<PartialResultRecord>;

#define RESULT_RECORD_MIN(name, tid, eid)         ResultRecord name{{tid, eid}, {0.0, {}}}
#define RESULT_RECORD_MAX(name, tid, eid)         ResultRecord name{{tid, eid}, {1.0, maxVec}}

#endif //KNOBAB_SERVER_TRACEDATA_H
