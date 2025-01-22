//
// Created by giacomo on 20/02/24.
//

#include "knobab/mining/polyadic/polyadic_bolt.h"

bool QM_DECLARE::operator()(const simple_declare& key, const struct declare_lattice_node& x ) const {
    bool basic =  (x.isVisited) && (x.log_support >= log_theta) && (std::abs(x.rconf) > std::numeric_limits<double>::epsilon());
    if (!basic) return false;
    for (const simple_declare& parent : ptr->getGeneralisations(key)) {
        if (parent.first == "Choice") continue;
        if (ptr->get(parent).log_support > x.log_support)
            return false;
    }
    return true;
}


//std::vector<size_t> sat_or_vac;
//std::vector<size_t> sat_vac;

void declare_lattice_node::set(const std::vector<size_t>& SAT, const std::vector<size_t>& VAC, const std::vector<size_t>& UNSAT) {
//    this->log_support = log_support;
//    this->rconf = rconf;
    isVisited = true;
    std::vector<size_t> nosat_or_vac;
    sat = SAT;
    no_sat = UNSAT;
    vac = VAC;
#ifdef DEBUG
    set_case = true;
#endif
}