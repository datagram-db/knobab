//
// Created by giacomo on 17/02/2022.
//

#ifndef DATAREP_RANGEQUERYITERATOR_H
#define DATAREP_RANGEQUERYITERATOR_H

#include <stdint.h>
#include <vector>
#include <limits>
#include <cassert>
#include "DataRepresentation.h"


class RangeQueryIterator {
    uint32_t              no_trace_ids;
    uint64_t              current_range_data_ptr_pos;
    const std::vector<size_t>& no_event_ids;
    bool                  isEventJoinCondition;
    DataRepresentationEvent local_data_representation;
    bool isCurrentSet;

    /**
     * If the first element of the pair is zero, then the result represents an empty set
     * If the first element of the pair is one, then the result represent the universal relationship,
     * Otherwise, the first element of the pair shall be two, and the remainder shall contain actual data.
     */
    std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>>* range_data_ptr;

    RangeQueryIterator(uint32_t noTraceIds,
                       uint32_t currentTraceId,
                       uint16_t currentEventId,
                       uint64_t currentRangeDataPtrPos,
                       const std::vector<size_t> &noEventIds,
                       bool isEventJoinCondition,
                       std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> *rangeDataPtr);

public:
    RangeQueryIterator(std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>>* v,
                       bool isBegin,
                       uint32_t  no_trace_ids,
                       const std::vector<size_t>& no_event_ids,
                       bool isEventJoinCondition);

    ~RangeQueryIterator() {}
    bool hasValue();
    RangeQueryIterator  operator++(int);
    RangeQueryIterator& operator++();
    const DataRepresentationEvent& operator* () ;
    const DataRepresentationEvent&   operator->();
    RangeQueryIterator  operator+ (size_t v)   const;

    bool operator==(const RangeQueryIterator &rhs) const;

    bool operator!=(const RangeQueryIterator &rhs) const;

};


#endif //DATAREP_RANGEQUERYITERATOR_H
