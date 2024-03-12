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
//    sat.clear();
//    no_sat.clear();
//    vac.clear();
//    nosat_or_vac.clear();
//    std::set_union(VAC.begin(), VAC.end(), UNSAT.begin(), UNSAT.end(), std::back_inserter(nosat_or_vac));
//    std::set_difference(SAT.begin(), SAT.end(), nosat_or_vac.begin(), nosat_or_vac.end(), std::back_inserter(sat));
//
//    nosat_or_vac.clear();
//    std::set_union(SAT.begin(), SAT.end(), VAC.begin(), VAC.end(), std::back_inserter(nosat_or_vac));
//    std::set_difference(UNSAT.begin(), UNSAT.end(), nosat_or_vac.begin(), nosat_or_vac.end(), std::back_inserter(no_sat));
//
//    nosat_or_vac.clear();
//    std::set_difference(VAC.begin(), VAC.end(), UNSAT.begin(), UNSAT.end(), std::back_inserter(nosat_or_vac));
//    std::set_difference(nosat_or_vac.begin(), nosat_or_vac.end(), sat.begin(), sat.end(), std::back_inserter(vac));
}