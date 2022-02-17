//
// Created by giacomo on 17/02/2022.
//

#include "data_views/ExistsIterator.h"

ExistsIterator::ExistsIterator(const std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>::iterator &ptr, bool ifj) : ptr(ptr), isForJoin(ifj) {
}

bool ExistsIterator::operator==(const ExistsIterator &rhs) const {
    return ptr == rhs.ptr;
}

bool ExistsIterator::operator!=(const ExistsIterator &rhs) const {
    return !(ptr == rhs.ptr);
}

ExistsIterator ExistsIterator::operator++(int) { return {ptr++, isForJoin};}

ExistsIterator &ExistsIterator::operator++() { ++ptr; return  *this; }

const DataRepresentationEvent &ExistsIterator::operator*()  {
    if (!isSet) {
        data.event_id =  ptr->first.second;
        data.satisfiability = ptr->second;
        data.trace_id = ptr->first.first;
        if (isForJoin) data.activation_or_target = {ptr->first.second};
        isSet = true;
    }
    return data;
}

ExistsIterator ExistsIterator::operator+(size_t v) const { return {ptr + v, isForJoin}; }

const DataRepresentationEvent &ExistsIterator::operator->()  {
    return **this;
}
