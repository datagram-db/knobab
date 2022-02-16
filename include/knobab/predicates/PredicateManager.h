//
// Created by Sam on 15/02/2022.
//

#ifndef KNOBAB_PREDICATEMANAGER_H
#define KNOBAB_PREDICATEMANAGER_H


#include <vector>
#include "SimpleDataPredicate.h"

class PredicateManager {
public:
    DEFAULT_CONSTRUCTORS(PredicateManager);
    PredicateManager(const std::vector<SimpleDataPredicate>& pred): predicates(pred){}
    bool checkValidity(const env &e1, const env &e2) const;

private:
    std::vector<SimpleDataPredicate> predicates;
};


#endif //KNOBAB_PREDICATEMANAGER_H
