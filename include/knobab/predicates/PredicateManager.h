//
// Created by Sam on 15/02/2022.
//

#ifndef KNOBAB_PREDICATEMANAGER_H
#define KNOBAB_PREDICATEMANAGER_H


#include <vector>
#include "SimpleDataPredicate.h"
class KnowledgeBase;

struct PredicateManager {
    DEFAULT_CONSTRUCTORS(PredicateManager);
    PredicateManager(const std::vector<std::vector<SimpleDataPredicate>>& pred, const KnowledgeBase* k): predicates(pred) , kb(k){}
    bool checkValidity(const env &e1, const env &e2) const;

    bool operator==(const PredicateManager &rhs) const;
    bool operator!=(const PredicateManager &rhs) const;

    bool isTruth() const {
        return predicates.empty();
    }

    const KnowledgeBase* getKnowledgeBase() const{
        return kb;
    }
    

    env GetPayloadDataFromEvent(const std::pair<uint32_t , uint16_t>& pair) const;


    std::vector<std::vector<SimpleDataPredicate>> predicates;
    const KnowledgeBase* kb;
};


namespace std {
    template <> struct hash<PredicateManager> {
        size_t operator()(const PredicateManager& x) const {
            size_t y = 17;
            for (const auto& z : x.predicates) {
                for (const auto& t : z) {
                    y += std::hash<SimpleDataPredicate>{}(t) * 7;
                }
            }
            return y;
        }
    };
}


#endif //KNOBAB_PREDICATEMANAGER_H
