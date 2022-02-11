//
// Created by giacomo on 20/01/2022.
//

#include "knobab/trace_repairs/MarkedEvent.h"

bool MarkedEvent::operator==(const MarkedEvent &rhs) const {
    return trace_id == rhs.trace_id &&
           event_id == rhs.event_id &&
           atomic_match == rhs.atomic_match;
}

bool MarkedEvent::operator!=(const MarkedEvent &rhs) const {
    return !(rhs == *this);
}

bool MarkedEvent::operator<(const MarkedEvent &rhs) const {
    if (trace_id < rhs.trace_id)
        return true;
    if (rhs.trace_id < trace_id)
        return false;
    if (event_id < rhs.event_id)
        return true;
    if (rhs.event_id < event_id)
        return false;
    return atomic_match < rhs.atomic_match;
}

bool MarkedEvent::operator>(const MarkedEvent &rhs) const {
    return rhs < *this;
}

bool MarkedEvent::operator<=(const MarkedEvent &rhs) const {
    return !(rhs < *this);
}

bool MarkedEvent::operator>=(const MarkedEvent &rhs) const {
    return !(*this < rhs);
}
