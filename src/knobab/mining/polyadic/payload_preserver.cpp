//
// Created by giacomo on 29/12/24.
//

#include <knobab/mining/polyadic/payload_preserver.h>


std::vector<std::pair<std::string,union_minimal>>* constituent_idx_key::get() const {
    if (payload)
        return payload;
    else
        return &ptr->redundantX.at(offset);
}

bool constituent_idx_key::operator<(const constituent_idx_key& rhs) const {
    return   *get() < *rhs.get();
}