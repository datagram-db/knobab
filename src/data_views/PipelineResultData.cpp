//
// Created by giacomo on 17/02/2022.
//

#include <data_views/PipelineResultData.h>

PipelineResultData::PipelineResultData(uint32_t traceId, uint16_t eventId, double satisfiability,
                                       const std::vector<size_t> &activationOrTarget) : trace_id(traceId),
                                                                                                    event_id(eventId),
                                                                                                    satisfiability(
                                                                                                            satisfiability),
                                                                                                    activation_or_target(
                                                                                                            activationOrTarget) {}

bool PipelineResultData::operator==(const PipelineResultData &rhs) const {
    return trace_id == rhs.trace_id &&
           event_id == rhs.event_id &&
           satisfiability == rhs.satisfiability &&
           activation_or_target == rhs.activation_or_target;
}

bool PipelineResultData::operator!=(const PipelineResultData &rhs) const {
    return !(rhs == *this);
}

bool PipelineResultData::operator<(const PipelineResultData &rhs) const {
    if (trace_id < rhs.trace_id)
        return true;
    if (rhs.trace_id < trace_id)
        return false;
    if (event_id < rhs.event_id)
        return true;
    if (rhs.event_id < event_id)
        return false;
    if (satisfiability < rhs.satisfiability)
        return true;
    if (rhs.satisfiability < satisfiability)
        return false;
    return activation_or_target < rhs.activation_or_target;
}

bool PipelineResultData::operator>(const PipelineResultData &rhs) const {
    return rhs < *this;
}

bool PipelineResultData::operator<=(const PipelineResultData &rhs) const {
    return !(rhs < *this);
}

bool PipelineResultData::operator>=(const PipelineResultData &rhs) const {
    return !(*this < rhs);
}

std::ostream& operator<<(std::ostream& os, const PipelineResultData &event) {
    os << "trace_id: " << event.trace_id << " event_id: " << event.event_id << " satisfiability: "
       << event.satisfiability << " activation_or_target: {";
    for (size_t i = 0, N = event.activation_or_target.size(); i<N; i++) {
        os << event.activation_or_target.at(i);
        if (i != (N-1))
            os << ", ";
    }
    return os <<"}";
}
