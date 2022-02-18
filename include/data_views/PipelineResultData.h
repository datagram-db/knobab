//
// Created by giacomo on 17/02/2022.
//

#ifndef DATAREP_PIPELINERESULTDATA_H
#define DATAREP_PIPELINERESULTDATA_H

#include <stdint.h>
#include <vector>
#include <cmath>
#include <ostream>


struct PipelineResultData {
    uint32_t              trace_id;
    uint16_t              event_id;
    double                satisfiability;
    std::vector<size_t> activation_or_target;

    PipelineResultData(uint32_t traceId,
                       uint16_t eventId,
                       double satisfiability= 1.0,
                       const std::vector<size_t> &activationOrTarget = {});
    PipelineResultData() = default;
    PipelineResultData(const PipelineResultData& ) = default;
    PipelineResultData(PipelineResultData&& ) = default;
    PipelineResultData& operator=(const PipelineResultData& ) = default;
    PipelineResultData& operator=(PipelineResultData&& ) = default;

    bool operator==(const PipelineResultData &rhs) const;
    bool operator!=(const PipelineResultData &rhs) const;
    bool operator<(const PipelineResultData &rhs) const;
    bool operator>(const PipelineResultData &rhs) const;
    bool operator<=(const PipelineResultData &rhs) const;
    bool operator>=(const PipelineResultData &rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const PipelineResultData &event);
};

namespace std {
    template <> struct hash<PipelineResultData> {
        size_t operator()(const PipelineResultData& x) const {
            return (((size_t)x.trace_id) << 32) + (((size_t)x.event_id) << 16) + (x.activation_or_target.empty() ? 0 : x.activation_or_target.at(0)+1);
        }
    };
}



#endif //DATAREP_PIPELINERESULTDATA_H
