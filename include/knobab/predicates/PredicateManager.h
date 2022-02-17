//
// Created by Sam on 15/02/2022.
//

#ifndef KNOBAB_PREDICATEMANAGER_H
#define KNOBAB_PREDICATEMANAGER_H


#include <vector>
#include "SimpleDataPredicate.h"

struct PredicateManager {
    DEFAULT_CONSTRUCTORS(PredicateManager);
    PredicateManager(const std::vector<std::vector<SimpleDataPredicate>>& pred): predicates(pred){}
    bool checkValidity(const env &e1, const env &e2) const;

    bool operator==(const PredicateManager &rhs) const;
    bool operator!=(const PredicateManager &rhs) const;

    bool isTruth() const {
        return predicates.empty();
    }


    std::vector<std::vector<SimpleDataPredicate>> predicates;
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
