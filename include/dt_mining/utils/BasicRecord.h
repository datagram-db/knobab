//
// Created by giacomo on 05/10/24.
//

#ifndef DTMINING_BASICRECORD_H
#define DTMINING_BASICRECORD_H


struct BasicRecord {
    bool type;          // The former Y/N
    int time_spot;      // The pointwise event's position in the trace
    double raw_payload; // The payload associated to the position name
    double additional;  // Whether it has additional payload from a different type of event capturing

    BasicRecord(bool t, int ts, double rp, double a);
    BasicRecord() = default;
    BasicRecord(const BasicRecord&) = default;
    BasicRecord(BasicRecord&&) = default;
    BasicRecord& operator=(const BasicRecord&) = default;
    BasicRecord& operator=(BasicRecord&&) = default;
};

#include <cmath>

static inline BasicRecord absence_gen(double epsilon, int idx, double value) {
    double abs_val = std::abs(value);
    bool absence_cond = abs_val <= epsilon;
    return {absence_cond, idx, value, absence_cond ? 0.0 : abs_val};
}

static inline BasicRecord increase_gen(int idx, double curr, double next) {
    double diff = next - curr;
    bool absence_cond = diff > 0;
    return {absence_cond, idx, next, diff};
}

static inline BasicRecord stationariety_gen(double epsilon, int idx, double curr, double next) {
    double diff = std::abs(next - curr);
    bool stationariety_cond = diff <= epsilon;
    return {stationariety_cond, idx, next, stationariety_cond ? 0.0 : diff};
}

static inline BasicRecord variation_gen(double epsilon, double maxval, int idx, double curr, double next) {
    double diff = std::abs(next - curr);
    bool stationariety_cond = diff <= epsilon;
    if (stationariety_cond) {
        return {stationariety_cond, idx, next, 0.0};
    } else if (std::abs(curr) <= epsilon) {
        return {!stationariety_cond, idx, next, maxval};
    } else {
        return {!stationariety_cond, idx, next, (next - curr) / curr};
    }
}

#endif //DTMINING_BASICRECORD_H
