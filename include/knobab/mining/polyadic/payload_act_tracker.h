//
// Created by giacomo on 29/12/24.
//

#include <vector>

#ifndef KNOBAB_SERVER_PAYLOAD_ACT_TRACKER_H
#define KNOBAB_SERVER_PAYLOAD_ACT_TRACKER_H

struct payload_act_tracker {
    std::vector<size_t> A_and_B, A_or_B, neither_ACT;
    std::vector<size_t> A_not_B, B_not_A, all_VIOL, excl_OCC;

    inline void clear() {
        A_and_B.clear();
        A_or_B.clear();
        neither_ACT.clear();
        A_not_B.clear();
        B_not_A.clear();
        all_VIOL.clear();
        excl_OCC.clear();
    }
};

#endif //KNOBAB_SERVER_PAYLOAD_ACT_TRACKER_H
