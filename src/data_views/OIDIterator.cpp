//
// Created by giacomo on 17/02/2022.
//

#include "data_views/OIDIterator.h"

OIDIterator::OIDIterator(const std::vector<oid>::iterator &ptr, bool isForJoin) : ptr(ptr), isForJoin(isForJoin) {
}
OIDIterator OIDIterator::operator++(int) /* postfix */         { return {ptr++, isForJoin}; }
OIDIterator &OIDIterator::operator++()    /* prefix */          { ++ptr; return *this; }

const DataRepresentationEvent &OIDIterator::operator*() {
    if (!isSet) {
        data.event_id =  ptr->id.parts.event_id;
        data.satisfiability = 1.0;
        data.trace_id = ptr->id.parts.trace_id;
        if (isForJoin) data.activation_or_target = {ptr->id.parts.trace_id};
        isSet = true;
    }
    return data; }

const DataRepresentationEvent &OIDIterator::operator->() { return **this; }
OIDIterator OIDIterator::operator+(size_t v) const { return {ptr + v, isForJoin}; }
bool OIDIterator::operator==(const OIDIterator &rhs) const { return ptr == rhs.ptr; }
bool OIDIterator::operator!=(const OIDIterator &rhs) const { return ptr != rhs.ptr; }
