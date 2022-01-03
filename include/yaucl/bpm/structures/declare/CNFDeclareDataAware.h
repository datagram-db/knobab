//
// Created by giacomo on 02/01/2022.
//

#ifndef KNOBAB_CNFDECLAREDATAAWARE_H
#define KNOBAB_CNFDECLAREDATAAWARE_H

#include <yaucl/bpm/structures/declare/DisjunctiveDeclareDataAware.h>

struct CNFDeclareDataAware {
    std::vector<DisjunctiveDeclareDataAware> singleElementOfConjunction;
    DEFAULT_CONSTRUCTORS(CNFDeclareDataAware)

    CNFDeclareDataAware(const DeclareDataAware& singleton);
    CNFDeclareDataAware(const std::vector<DeclareDataAware>& orig);
};


#endif //KNOBAB_CNFDECLAREDATAAWARE_H
