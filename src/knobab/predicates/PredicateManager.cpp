//
// Created by Sam on 15/02/2022.
//

#include "knobab/predicates/PredicateManager.h"

bool PredicateManager::checkValidity(const env &e1, const env &e2) const {
    if (predicates.empty()) return true;
    for(const auto& pred_withConj : predicates){
        bool result = true;
        for (const auto& pred : pred_withConj) {
            if(!pred(e1, e2)){
                result = false;
                break;
            }
        }
        if (result) return true;
    }
    return false;
}

bool PredicateManager::operator==(const PredicateManager &rhs) const {
    return predicates == rhs.predicates;
}

bool PredicateManager::operator!=(const PredicateManager &rhs) const {
    return !(rhs == *this);
}
