//
// Created by giacomo on 02/01/2022.
//

#ifndef KNOBAB_DISJUNCTIVEDECLAREDATAAWARE_H
#define KNOBAB_DISJUNCTIVEDECLAREDATAAWARE_H

#include <vector>
#include <yaucl/bpm/structures/declare/DeclareDataAware.h>

class DisjunctiveDeclareDataAware {
public:
    std::vector<DeclareDataAware> elementsInDisjunction;

    DEFAULT_CONSTRUCTORS(DisjunctiveDeclareDataAware)
    DisjunctiveDeclareDataAware(const DeclareDataAware& singleton);
};


#endif //KNOBAB_DISJUNCTIVEDECLAREDATAAWARE_H
