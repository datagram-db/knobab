//
// Created by Sam on 15/02/2022.
//

#include "knobab/predicates/PredicateManager.h"

bool PredicateManager::checkValidity(const env &e1, const env &e2) const {
    for(const auto& pred : predicates){
        if(!pred(e1, e2)){
            return false;
        }
    }
    return true;
}
