//
// Created by giacomo on 20/02/24.
//

#include "knobab/mining/polyadic/polyadic_bolt.h"

bool QM_DECLARE::operator()(const simple_declare& key, const struct declare_lattice_node& x ) const {
    bool basic =  (x.isVisited) && (x.log_support >= log_theta) && (std::abs(x.rconf) > std::numeric_limits<double>::epsilon());
    if (!basic) return false;
    for (const simple_declare& parent : ptr->getGeneralisations(key)) {
        if (ptr->get(parent).log_support > x.log_support)
            return false;
    }
    return true;
}