//
// Created by giacomo on 20/01/2022.
//

#ifndef KNOBAB_MARKEDEVENT_H
#define KNOBAB_MARKEDEVENT_H


#include <knobab/oid.h>
#include <knobab/trace_repairs/DataQuery.h>

struct MarkedEvent {
    trace_t trace_id;
    event_t event_id;
    std::vector<DataQuery> correction_list;

    /**
     * Checking if two MarkedEvents are compatible
     * @param rhs
     * @return
     */
    bool comp(const MarkedEvent &rhs) const {
        return (trace_id == rhs.trace_id) && ((event_id != rhs.event_id) || (correction_list == rhs.correction_list));
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
            return k.trace_id ^ k.event_id;
        }
    };

}


#endif //KNOBAB_MARKEDEVENT_H
