//
// Created by Sam on 15/02/2022.
//

#ifndef KNOBAB_PREDICATEMANAGER_H
#define KNOBAB_PREDICATEMANAGER_H


#include <vector>
#include "SimpleDataPredicate.h"

class KnowledgeBase;

class PredicateManager {
public:
    DEFAULT_CONSTRUCTORS(PredicateManager);
    PredicateManager(const std::vector<SimpleDataPredicate>& pred, const KnowledgeBase* k): predicates(pred), kb(k){}
    bool checkValidity(const env &e1, const env &e2) const;
    const KnowledgeBase* getKnowledgeBase() const{
        return kb;
    }

    env GetPayloadDataFromEvent(const std::pair<uint32_t , uint16_t>& pair) const;

private:
    std::vector<SimpleDataPredicate> predicates;
    const KnowledgeBase* kb;
};


#endif //KNOBAB_PREDICATEMANAGER_H
