//
// Created by giacomo on 07/01/2022.
//

#ifndef KNOBAB_EASY_PROP_H
#define KNOBAB_EASY_PROP_H

#include <string>
#include <vector>
#include <yaucl/structures/default_constructors.h>

struct easy_prop {
    enum t {
        E_P_AND,
        E_P_OR,
        E_P_ATOM
    };
    t casusu;
    bool isAtomNegated;
    std::vector<easy_prop> args;
    std::string single_atom_if_any;

    DEFAULT_CONSTRUCTORS(easy_prop)
};


#endif //KNOBAB_EASY_PROP_H
