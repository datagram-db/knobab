//
// Created by giacomo on 05/10/24.
//

#include "BasicRecord.h"

BasicRecord::BasicRecord(bool t, int ts, double rp, double a)
        : type(t), time_spot(ts), raw_payload(rp), additional(a) {}
