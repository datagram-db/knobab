//
// Created by giacomo on 17/02/2022.
//

#ifndef DATAREP_DATAREPRESENTATION_H
#define DATAREP_DATAREPRESENTATION_H

#include <stdint.h>
#include <vector>
#include <cmath>
#include <ostream>

struct DataRepresentationEvent {
    uint32_t              trace_id;
    uint16_t              event_id;
    double                satisfiability;
    std::vector<size_t> activation_or_target;

    DataRepresentationEvent(uint32_t traceId,
                            uint16_t eventId,
                            double satisfiability= 1.0,
                            const std::vector<size_t> &activationOrTarget = {});
    DataRepresentationEvent() = default;
    DataRepresentationEvent(const DataRepresentationEvent& ) = default;
    DataRepresentationEvent(DataRepresentationEvent&& ) = default;
    DataRepresentationEvent& operator=(const DataRepresentationEvent& ) = default;
    DataRepresentationEvent& operator=(DataRepresentationEvent&& ) = default;

    bool operator==(const DataRepresentationEvent &rhs) const;
    bool operator!=(const DataRepresentationEvent &rhs) const;
    bool operator<(const DataRepresentationEvent &rhs) const;
    bool operator>(const DataRepresentationEvent &rhs) const;
    bool operator<=(const DataRepresentationEvent &rhs) const;
    bool operator>=(const DataRepresentationEvent &rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const DataRepresentationEvent &event);
};

namespace std {
    template <> struct hash<DataRepresentationEvent> {
        size_t operator()(const DataRepresentationEvent& x) const {
            return (((size_t)x.trace_id) << 32) + (((size_t)x.event_id) << 16) + (x.activation_or_target.empty() ? 0 : x.activation_or_target.at(0)+1);
        }
    };
}



#endif //DATAREP_DATAREPRESENTATION_H
