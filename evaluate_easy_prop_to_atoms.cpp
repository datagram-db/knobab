//
// Created by giacomo on 07/01/2022.
//

#include "evaluate_easy_prop_to_atoms.h"

semantic_atom_set evaluate_easy_prop_to_atoms::run(const easy_prop &prop) {
    switch (prop.casusu) {
        case easy_prop::E_P_AND:
            assert(prop.args.size() == 2);
            assert(!prop.isAtomNegated);
            return unordered_intersection(run( prop.args.at(0)),
                                          run( prop.args.at(1)) );
        case easy_prop::E_P_OR: {
            assert(prop.args.size() == 2);
            assert(!prop.isAtomNegated);
            semantic_atom_set S = run( prop.args.at(0));
            auto tmp = run( prop.args.at(1));
            S.insert(tmp.begin(), tmp.end());
            return S;
        }
        case easy_prop::E_P_ATOM:
            assert(prop.args.empty());
            assert(bogus_act_to_atom->contains(prop.single_atom_if_any));
            if (prop.isAtomNegated) {
                semantic_atom_set S = ap.atom_decomposition(bogus_act_to_atom->at(prop.single_atom_if_any));
                return unordered_difference(S, bogus_act_to_set->at(prop.single_atom_if_any));
            } else {
                return bogus_act_to_set->at(prop.single_atom_if_any);
            }
    }
}

semantic_atom_set evaluate_easy_prop_to_atoms::operator()(const easy_prop &prop,
                                                          const std::unordered_map<std::string, std::string> *bogus_act_to_atom,
                                                          const std::unordered_map<std::string, semantic_atom_set> *bogus_act_to_set) {
    this->bogus_act_to_atom = bogus_act_to_atom;
    this->bogus_act_to_set = bogus_act_to_set;
    assert(bogus_act_to_atom);
    assert(bogus_act_to_set);
    return run(prop);
}
