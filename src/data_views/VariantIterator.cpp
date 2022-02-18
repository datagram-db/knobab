//
// Created by giacomo on 17/02/2022.
//

#include "data_views/VariantIterator.h"
#include <cassert>
#include <limits>
#include <memory>

VariantIterator::VariantIterator(const VariantIterator &begin, const VariantIterator &end,
                                 std::function<bool(const DataRepresentationEvent &)> f) :
        iterators{begin, end}, f{f}, T{T}, casusu{FilterIt} {
    while (iterators[0] != iterators[1] && !f(*iterators[0])) {
        iterators[0].pop();
    }
}

VariantIterator::VariantIterator(const VariantIterator &begin, const VariantIterator &end,
                                 std::function<bool(const DataRepresentationEvent &)> f,
                                 std::function<DataRepresentationEvent(const DataRepresentationEvent &)> T,
                                 std::function<DataRepresentationEvent(const DataRepresentationEvent &)> Tinv) :
        iterators{begin, end}, f{f}, T{T}, Tinv{Tinv}, casusu{FilterAndTransformIt} {
    while (iterators[0] != iterators[1] && !f(*iterators[0])) {
        iterators[0].pop();
    }
}

VariantIterator::VariantIterator(uint32_t noTraceIds,
        uint32_t currentTraceId,
uint16_t currentEventId,
        uint64_t currentRangeDataPtrPos,
const std::vector<size_t> *noEventIds,
bool isEventJoinCondition,
        std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> *rangeDataPtr)
: no_trace_ids(noTraceIds),
  current_range_data_ptr_pos(currentRangeDataPtrPos), no_event_ids(noEventIds),
  isForJoin(isEventJoinCondition), casusu{RangeQueryIt},
  isCurrentSet(false), range_data_ptr(rangeDataPtr) {
    data.trace_id = currentTraceId;
    data.event_id = currentEventId;
    if (rangeDataPtr->first == 1) {
        data.satisfiability = 1.0;
    }
    data.activation_or_target = isEventJoinCondition ? std::vector<size_t>{data.event_id} : std::vector<size_t>{};
}


VariantIterator::VariantIterator(const std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>::iterator& ptr, bool ifj) : ex_ptr(ptr), casusu{ExistsIt}, isForJoin(ifj) {
}


VariantIterator::VariantIterator(const std::vector<oid>::iterator& ptr, bool isForJoin) : oid_ptr(ptr),casusu{OIDIt}, isForJoin(isForJoin) {
}

VariantIterator::VariantIterator(std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> *v,
                                       bool isBegin, uint32_t no_trace_ids, const std::vector<size_t> *no_event_ids,
                                       bool isEventJoinCondition) : range_data_ptr(v), no_event_ids{no_event_ids}, no_trace_ids{no_trace_ids}, casusu{RangeQueryIt}, isForJoin{isEventJoinCondition} {
    assert(v != nullptr);
    isCurrentSet = true;
    if (v == nullptr) {
        data.trace_id = std::numeric_limits<uint32_t>::max();
        data.event_id = std::numeric_limits<uint16_t>::max();
        current_range_data_ptr_pos = std::numeric_limits<uint64_t>::max();
    } else {
        data.trace_id = 0;
        data.event_id  = 0;
        current_range_data_ptr_pos = 0;
    }
    if (!isBegin) {
        if (v->first == 1) {
            data.satisfiability = 1.0;
            data.trace_id  = no_trace_ids;
            data.event_id  = 0;
        } else if (v->first == 2) {
            current_range_data_ptr_pos = v->second.size();
            data.trace_id  = v->second.rbegin()->first.first + 1;
            data.event_id  = 0;
        }
    } else {
        data.satisfiability = 1.0;
    }
    data.activation_or_target = isEventJoinCondition ? std::vector<size_t>{data.event_id} : std::vector<size_t>{};
}


VariantIterator VariantIterator::operator++(int) /* postfix */         {
    switch(casusu ){
        case RangeQueryIt:
        {
            if (!range_data_ptr) return *this;
            else if (range_data_ptr->first == 1) {
                do {
                    data.event_id++;
                    if (data.event_id < no_event_ids->at(data.trace_id)) break;
                    else do {
                            data.trace_id++;
                            data.event_id = 0;
                        } while ((data.trace_id < no_trace_ids) && (data.event_id >= this->no_event_ids->at(data.trace_id)));
                    if ((data.trace_id >= no_trace_ids) || (data.event_id < no_event_ids->at(data.trace_id))) break;
                } while ((data.trace_id < no_trace_ids) && (data.trace_id < no_trace_ids));
            } else if (range_data_ptr->first == 2) {
                current_range_data_ptr_pos++;
                if (current_range_data_ptr_pos < range_data_ptr->second.size()) {
                    auto& ref = this->range_data_ptr->second.at(current_range_data_ptr_pos);
                    std::tie(data.trace_id, data.event_id) = ref.first;
                    data.satisfiability = ref.second;
                } else {
                    current_range_data_ptr_pos = range_data_ptr->second.size();
                    data.trace_id  = range_data_ptr->second.rbegin()->first.first + 1;
                    data.event_id  = 0;
                    data.satisfiability = 1.0;
                }
            }
            data.activation_or_target = isForJoin ? std::vector<size_t>{data.event_id} : std::vector<size_t>{};
            isCurrentSet = false;
            return *this;
        }
        case ExistsIt:
            return {ex_ptr++, isForJoin};
        case OIDIt:
            return {oid_ptr++, isForJoin};
        case FilterIt:
        case FilterAndTransformIt:
        {
            VariantIterator copy = *this;
            iterators[0].pop();
            return copy;
        }
        case NoneIt:
            break;
    }
}

VariantIterator VariantIterator::operator++()    /* prefix */ {
    switch (casusu) {
        case RangeQueryIt:
            (*this)++; return *this;
            break;
        case ExistsIt:
            ++ex_ptr; return *this;
        case OIDIt:
            ++oid_ptr; return *this;
        case FilterIt:
        case FilterAndTransformIt:
            do {
                iterators[0].pop();
            } while (iterators[0] != iterators[1] && !f(*iterators[0]));
            return *this;
        case NoneIt:
            break;
    }
}

const DataRepresentationEvent& VariantIterator::operator*()  {
    switch(casusu ){
        case RangeQueryIt: {
            assert(!((!range_data_ptr) || (range_data_ptr->first == 0)));
            if (!isCurrentSet) {
                if (range_data_ptr->first == 2) {
                    assert(current_range_data_ptr_pos < range_data_ptr->second.size());
                    auto& ref = range_data_ptr->second.at(current_range_data_ptr_pos);
                    std::tie(data.trace_id, data.event_id) = ref.first;
                    data.satisfiability = ref.second;
                    data.activation_or_target = isForJoin ? std::vector<size_t>{data.event_id} : std::vector<size_t>{};
                }
            }
            return data;
        }

        case ExistsIt:
        {
            if (!isSet) {
                data.event_id =  ex_ptr->first.second;
                data.satisfiability = ex_ptr->second;
                data.trace_id = ex_ptr->first.first;
                if (isForJoin) data.activation_or_target = {ex_ptr->first.second};
                isSet = true;
            }
            return data;
        }
        case OIDIt: {
            if (!isSet) {
                data.event_id =  oid_ptr->id.parts.event_id;
                data.satisfiability = 1.0;
                data.trace_id = oid_ptr->id.parts.trace_id;
                if (isForJoin) data.activation_or_target = {oid_ptr->id.parts.trace_id};
                isSet = true;
            }
            return data;
        }
        case FilterIt:
            return *iterators[0];

        case FilterAndTransformIt:
            data = T(*iterators[0]);
            return data;

        case NoneIt:
            break;
    }

}

const DataRepresentationEvent& VariantIterator::operator->() {
    return **this;
}

VariantIterator VariantIterator::operator+(size_t v) const {
    switch(casusu) {
        case RangeQueryIt: {
            if ((!range_data_ptr) || (range_data_ptr->first == 0))
                return *this;
            else if (range_data_ptr->first == 2) {
                assert(current_range_data_ptr_pos + v < range_data_ptr->second.size());
                auto& ref = range_data_ptr->second.at(current_range_data_ptr_pos+v);
                return {no_trace_ids, ref.first.first, ref.first.second, current_range_data_ptr_pos+v, no_event_ids, isForJoin, range_data_ptr};
            } else if (range_data_ptr->first == 1) {
                VariantIterator it = *this;
                for (size_t i = 0; i<v; i++) it++;
                return it;
            }
        }
            break;
        case ExistsIt:
            return {ex_ptr + v, isForJoin};
        case OIDIt:
            return {oid_ptr + v, isForJoin};
        case FilterIt:
        case FilterAndTransformIt:
            return iterators[0] + v;
        case NoneIt:
            break;
    }

}

bool VariantIterator::operator==(const VariantIterator &rhs) const {
    if (casusu != rhs.casusu) return false;
    switch(casusu) {
        case RangeQueryIt:
            return data.trace_id == rhs.data.trace_id && data.event_id == rhs.data.event_id;
        case ExistsIt:
            return ex_ptr == rhs.ex_ptr;
        case OIDIt:
            return oid_ptr == rhs.oid_ptr;
        case FilterIt:
        case FilterAndTransformIt:
            return iterators == rhs.iterators;
        case NoneIt:
            return false;
    }
}

bool VariantIterator::operator!=(const VariantIterator &rhs) const {
    return !(rhs == *this);
}

void VariantIterator::pop() {
    switch (casusu) {
        case RangeQueryIt:
            (*this)++;
            break;
        case ExistsIt:
            ++ex_ptr;
        case OIDIt:
            ++oid_ptr;
        case FilterIt:
        case FilterAndTransformIt:
            do {
                iterators[0].pop();
            } while (iterators[0] != iterators[1] && !f(*iterators[0]));
            break;
        case NoneIt:
            break;
    }
}

int64_t VariantIterator::operator-(const VariantIterator &begin) const {
    assert(casusu == begin.casusu);
    switch (casusu) {
        case RangeQueryIt:
            assert(range_data_ptr == range_data_ptr);
            if (range_data_ptr->first == 2) {
                return ((int64_t)current_range_data_ptr_pos) - ((int64_t)begin.current_range_data_ptr_pos);
            } else if (range_data_ptr->first == 1) {
                if ((data.trace_id < begin.data.trace_id) || ((data.trace_id == begin.data.trace_id) && (data.event_id < begin.data.event_id)))
                    return -(begin - *this);
                else if (data.trace_id == begin.data.trace_id)
                    return (data.event_id - begin.data.event_id);
                else {
                    int64_t currTraceId = begin.data.trace_id;
                    int64_t distancePos = (int64_t)(no_event_ids->at(currTraceId)-begin.data.event_id) + data.event_id;
                    currTraceId++;
                    for (; currTraceId<data.trace_id; currTraceId++)
                        distancePos += (int64_t)no_event_ids->at(currTraceId);
                    return distancePos;
                }
            }
        case ExistsIt:
            return std::distance(ex_ptr, begin.ex_ptr);
        case OIDIt:
            return std::distance(oid_ptr, begin.oid_ptr);
        case FilterIt:
        case FilterAndTransformIt:
            // In this case, the best I can do is to return the maximum distance
            return iterators.at(0) - begin.iterators.at(0);
        case VectorIt:
            return std::distance(data_ptr, begin.data_ptr);
        case NoneIt:
            return 0;
    }
}

#include <iostream>

VariantIterator
VariantIterator::lower_bound(VariantIterator first, VariantIterator last, const DataRepresentationEvent &value) {
    std::optional<VariantIterator> vi = reset_pointers_recursively(first, last, value, true);
    if (vi.has_value())
        return vi.value();

    VariantIterator it = first;
    auto L = (int64_t)first.currentIteratorPosition(), R = (int64_t)last.currentIteratorPosition();
    int64_t count, step;
    count = R - L;
    while (count > 0) {
        it = first;
        step = count / 2;
        it = it + step;
        std::cout << *it << " vs " << value << std::endl;
        if (*it < value) {
            it.pop();
            first = it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}


VariantIterator
VariantIterator::upper_bound(VariantIterator first, VariantIterator last, const DataRepresentationEvent &value) {
    std::optional<VariantIterator> vi = reset_pointers_recursively(first, last, value, false);
    if (vi.has_value())
        return vi.value();

    VariantIterator it = first;
    auto L = (int64_t)first.currentIteratorPosition(), R = (int64_t)last.currentIteratorPosition();
    int64_t count, step;
    count = R - L;
    while (count > 0) {
        it = first;
        step = count / 2;
        it = it + step;
        std::cout << *it << " vs " << value << std::endl;
        if (!(*it < value)) {
            it.pop();
            first = it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}


VariantIterator_t VariantIterator::getConcreteOperatorType() const {
    switch (casusu) {
        case RangeQueryIt:
        case ExistsIt:
        case OIDIt:
        case NoneIt:
        case VectorIt:
            return casusu;

        case FilterIt:
        case FilterAndTransformIt:
            auto t = iterators.at(0).getConcreteOperatorType();
            assert(t == iterators.at(1).getConcreteOperatorType());
            return t;
    }
}

void VariantIterator::setPointerAtStep(const VariantIterator &begin) {
    if (casusu == begin.casusu) {
        settingRestrained(begin, casusu);
    } else {
        assert(getConcreteOperatorType() == begin.getConcreteOperatorType());
        settingRestrained(begin, getConcreteOperatorType());
    }
}

void VariantIterator::settingRestrained(const VariantIterator &begin, VariantIterator_t kase) {
    no_trace_ids = begin.no_trace_ids;
    no_event_ids = begin.no_event_ids;
    isForJoin = begin.isForJoin;
    isCurrentSet = false;
    data.trace_id = begin.data.trace_id;
    data.event_id = begin.data.event_id;
    data.activation_or_target = isForJoin ? std::vector<size_t>{data.event_id}
                                          : std::vector<size_t>{};

    switch (kase) {
        case RangeQueryIt:
        {
            assert(range_data_ptr == begin.range_data_ptr);
            current_range_data_ptr_pos = begin.current_range_data_ptr_pos;
            if (range_data_ptr->first == 1) {
                data.satisfiability = 1.0;
            }
        }

        case ExistsIt:
            ex_ptr = begin.ex_ptr;
            break;

        case OIDIt:
            oid_ptr = begin.oid_ptr;
            break;

        case FilterIt:
        case FilterAndTransformIt:// Bug Here
            iterators[0].setPointerAtStep(begin);
            iterators[1].setPointerAtStep(begin.iterators[1]);
            while (iterators[0] != iterators[1] && !f(*iterators[0])) {
                iterators[0].pop();
            }
            break;
        case VectorIt:

            break;
        case NoneIt:
            return;
    }
}

VariantIterator VariantIterator::copy()  {
    switch (casusu) {
        case RangeQueryIt:
            return {no_trace_ids,
                    data.trace_id,
                    data.event_id,
                    current_range_data_ptr_pos,
                    this->no_event_ids,
                    isForJoin,
                    range_data_ptr};

        case ExistsIt:
            return {ex_ptr, isForJoin};

        case OIDIt:
            return {oid_ptr, isForJoin};

        case FilterIt:
            return {iterators[0].copy(), iterators[1].copy(), f};

        case FilterAndTransformIt:
            return {iterators[0].copy(), iterators[1].copy(), f, T, Tinv};

        case VectorIt:
            break;
        case NoneIt:
            return *this;
    }
}

size_t VariantIterator::currentIteratorPosition() const {
    switch (casusu) {
        case RangeQueryIt: {
            if (this->range_data_ptr->first == 1) {
                size_t totalCount = 0;
                for (size_t traceId = 0; traceId < data.trace_id; traceId++)
                    totalCount+=no_event_ids->at(traceId);
                return totalCount + data.event_id;
            } else if (this->range_data_ptr->first == 2) {
                return current_range_data_ptr_pos;
            } else // including 0
                return 0;
        }
        case ExistsIt:
            return ((size_t)ex_ptr.base()) / sizeof(std::pair<std::pair<uint32_t, uint16_t>, double>);

        case OIDIt:
            return ((size_t)oid_ptr.base()) / sizeof(oid);

        case FilterIt:
        case FilterAndTransformIt:
            return iterators.at(0).currentIteratorPosition();
            break;

        case VectorIt:
            // TODO
            break;
        case NoneIt:
            return 0;
    }
}

std::optional<VariantIterator> VariantIterator::reset_pointers_recursively(VariantIterator first, VariantIterator last,
                                                                           const DataRepresentationEvent &value,
                                                                           bool isLower) {
    assert(first.casusu == last.casusu);
    switch (first.casusu) {
        case FilterIt:
        {
            VariantIterator self = first;
            auto it = isLower ?
                      VariantIterator::lower_bound(first.iterators[0], last.iterators[0], value) :
                      VariantIterator::upper_bound(first.iterators[0], last.iterators[0], value);
            self.setPointerAtStep(it);
            return {self};
        }
        case FilterAndTransformIt:
        {
            VariantIterator self = first;
            auto it = isLower ?
                      VariantIterator::lower_bound(first.iterators[0], last.iterators[0], value) :
                      VariantIterator::upper_bound(first.iterators[0], last.iterators[0], value);
            self.setPointerAtStep(it);
            return {self};
        }

            break;
        case NoneIt:
            return {first};
        default:
            return {};
    }
}


