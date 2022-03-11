//
// Created by giacomo on 26/12/20.
//

#include <knobab/dataStructures/oid.h>

oid::oid() : oid(0) {}

oid::oid(const monotone_hash_t &x) {
    id.monotone_hash = x;
}

oid::oid(act_t fact, trace_t trace_id, event_t event_pos) {
    //id.parts.future = 0;
    id.parts.act = fact;
    id.parts.trace_id = trace_id;
    id.parts.event_id = event_pos;
}

oid &oid::operator=(const oid &x) { id.monotone_hash = x.id.monotone_hash; return *this; }

oid::oid(const oid &x) { id.monotone_hash = x.id.monotone_hash; }

bool oid::operator<(const oid &rhs) const { return id.monotone_hash < rhs.id.monotone_hash;  }

bool oid::operator>(const oid &rhs) const { return rhs < *this;     }

bool oid::operator<=(const oid &rhs) const { return !(rhs < *this);  }

bool oid::operator>=(const oid &rhs) const { return !(*this < rhs);  }

bool oid::operator==(const oid &rhs) const { return id.monotone_hash == rhs.id.monotone_hash; }

bool oid::operator!=(const oid &rhs) const { return !(rhs == *this); }
