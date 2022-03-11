//
// Created by giacomo on 20/01/2022.
//

#ifndef KNOBAB_MARKEDEVENT_H
#define KNOBAB_MARKEDEVENT_H


#include <knobab/dataStructures/oid.h>
#include <knobab/trace_repairs/DataQuery.h>

struct MarkedEvent {
    trace_t trace_id;
    event_t event_id;
    std::string atomic_match;

    /**
     * Checking if two MarkedEvents are compatible
     * @param rhs
     * @return
     */
    bool comp(const MarkedEvent &rhs) const {
        return (trace_id == rhs.trace_id) && ((event_id != rhs.event_id) || (atomic_match == rhs.atomic_match));
    }

    bool operator==(const MarkedEvent &rhs) const;
    bool operator!=(const MarkedEvent &rhs) const;
    bool operator<(const MarkedEvent &rhs) const;
    bool operator>(const MarkedEvent &rhs) const;
    bool operator<=(const MarkedEvent &rhs) const;
    bool operator>=(const MarkedEvent &rhs) const;
};


namespace std {
    template<>
    struct hash<struct MarkedEvent> {
        std::size_t operator()(const struct MarkedEvent &k) const {
            return yaucl::hashing::hash_combine(31, k.atomic_match);
        }
    };

}


#endif //KNOBAB_MARKEDEVENT_H
