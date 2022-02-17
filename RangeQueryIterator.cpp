//
// Created by giacomo on 17/02/2022.
//

#include <streambuf>
#include <memory>

#include "RangeQueryIterator.h"

bool RangeQueryIterator::hasValue() {
    if ((!range_data_ptr) || (range_data_ptr->first == 0))
        return false;
    else if (range_data_ptr->first == 1) {
        return (local_data_representation.trace_id < no_trace_ids) && (local_data_representation.event_id < no_event_ids.at(local_data_representation.trace_id));
    } else
        return current_range_data_ptr_pos < range_data_ptr->second.size();
}

RangeQueryIterator RangeQueryIterator::operator++(int) /* postfix */         {
    if (!range_data_ptr) return *this;
    else if (range_data_ptr->first == 1) {
        do {
            local_data_representation.event_id++;
            if (local_data_representation.event_id < no_event_ids.at(local_data_representation.trace_id)) break;
            else do {
                local_data_representation.trace_id++;
                local_data_representation.event_id = 0;
            } while ((local_data_representation.trace_id < no_trace_ids) && (local_data_representation.event_id >= this->no_event_ids.at(local_data_representation.trace_id)));
            if ((local_data_representation.trace_id >= no_trace_ids) || (local_data_representation.event_id < no_event_ids.at(local_data_representation.trace_id))) break;
        } while ((local_data_representation.trace_id < no_trace_ids) && (local_data_representation.trace_id < no_trace_ids));
    } else if (range_data_ptr->first == 2) {
        current_range_data_ptr_pos++;
        if (current_range_data_ptr_pos >= range_data_ptr->second.size()) {
            local_data_representation.event_id = 0;
            local_data_representation.trace_id = range_data_ptr->second.rbegin()->first.first+1;
        }
    }
    local_data_representation.activation_or_target = isEventJoinCondition ? std::vector<size_t>{local_data_representation.event_id} : std::vector<size_t>{};
    isCurrentSet = false;
    return *this;
}

RangeQueryIterator &RangeQueryIterator::operator++()    /* prefix */ { (*this)++; return *this; }

const DataRepresentationEvent& RangeQueryIterator::operator*()  {
    assert(!((!range_data_ptr) || (range_data_ptr->first == 0)));
    if (!isCurrentSet) {
       if (range_data_ptr->first == 2) {
            assert(current_range_data_ptr_pos < range_data_ptr->second.size());
            auto& ref = range_data_ptr->second.at(current_range_data_ptr_pos);
            std::tie(local_data_representation.trace_id, local_data_representation.event_id) = ref.first;
            local_data_representation.satisfiability = ref.second;
        }
    }
    return local_data_representation;
}

const DataRepresentationEvent &RangeQueryIterator::operator->() { return **this; }

RangeQueryIterator RangeQueryIterator::operator+(size_t v) const {
    if ((!range_data_ptr) || (range_data_ptr->first == 0))
        return *this;
    else if (range_data_ptr->first == 2) {
        assert(current_range_data_ptr_pos + v < range_data_ptr->second.size());
        auto& ref = range_data_ptr->second.at(current_range_data_ptr_pos+v);
        return {no_trace_ids, ref.first.first, ref.first.second, current_range_data_ptr_pos+v, no_event_ids, isEventJoinCondition, range_data_ptr};
    } else if (range_data_ptr->first == 1) {
        RangeQueryIterator it = *this;
        for (size_t i = 0; i<v; i++) it++;
        return it;
    }
}

RangeQueryIterator::RangeQueryIterator(uint32_t noTraceIds,
                                       uint32_t currentTraceId,
                                       uint16_t currentEventId,
                                       uint64_t currentRangeDataPtrPos,
                                       const std::vector<size_t> &noEventIds,
                                       bool isEventJoinCondition,
                                       std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> *rangeDataPtr)
        : no_trace_ids(noTraceIds),
          current_range_data_ptr_pos(currentRangeDataPtrPos), no_event_ids(noEventIds),
          isEventJoinCondition(isEventJoinCondition),
          isCurrentSet(false), range_data_ptr(rangeDataPtr) {
    local_data_representation.trace_id = currentTraceId;
    local_data_representation.event_id = currentEventId;
    if (rangeDataPtr->first == 1) {
        local_data_representation.satisfiability = 1.0;
    }
    local_data_representation.activation_or_target = isEventJoinCondition ? std::vector<size_t>{local_data_representation.event_id} : std::vector<size_t>{};
}

RangeQueryIterator::RangeQueryIterator(std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> *v,
                                       bool isBegin, uint32_t no_trace_ids, const std::vector<size_t> &no_event_ids,
                                       bool isEventJoinCondition) : range_data_ptr(v), no_event_ids{no_event_ids}, no_trace_ids{no_trace_ids}, isEventJoinCondition{isEventJoinCondition} {
    assert(v != nullptr);
    isCurrentSet = true;
    if (v == nullptr) {
        local_data_representation.trace_id = std::numeric_limits<uint32_t>::max();
        local_data_representation.event_id = std::numeric_limits<uint16_t>::max();
        current_range_data_ptr_pos = std::numeric_limits<uint64_t>::max();
    } else {
        local_data_representation.trace_id = 0;
        local_data_representation.event_id  = 0;
        current_range_data_ptr_pos = 0;
    }
    if (!isBegin) {
        if (v->first == 1) {
            local_data_representation.satisfiability = 1.0;
            local_data_representation.trace_id  = no_trace_ids;
            local_data_representation.event_id  = 0;
        } else if (v->first == 2) {
            current_range_data_ptr_pos = v->second.size();
            local_data_representation.trace_id  = v->second.rbegin()->first.first+1;
            local_data_representation.event_id  = 0;
        }
    } else {
        local_data_representation.satisfiability = 1.0;
    }
    local_data_representation.activation_or_target = isEventJoinCondition ? std::vector<size_t>{local_data_representation.event_id} : std::vector<size_t>{};
}

bool RangeQueryIterator::operator==(const RangeQueryIterator &rhs) const {
    return local_data_representation == rhs.local_data_representation;
}

bool RangeQueryIterator::operator!=(const RangeQueryIterator &rhs) const {
    return !(rhs == *this);
}
