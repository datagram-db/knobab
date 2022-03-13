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
    bool checkValidity(const env &e1, uint32_t t2, uint16_t e2) const;
    bool checkValidity(uint32_t t1, uint16_t ea, const env &e2) const;


    bool operator==(const PredicateManager &rhs) const;
    bool operator!=(const PredicateManager &rhs) const;

    bool isTruth() const {
        return predicates.empty();
    }

    const KnowledgeBase* getKnowledgeBase() const{
        return kb;
    }

    PredicateManager flip() const {
        PredicateManager result;
        for (const auto& ref : predicates) {
            auto inner = result.predicates.emplace_back();
            for (const auto& ref2 : ref) {
                inner.emplace_back(ref2.flip());
            }
        }
        result.kb = this->kb;
        return result;
    }

    env GetPayloadDataFromEvent(const std::pair<uint32_t , uint16_t>& pair) const;
    env GetPayloadDataFromEvent(uint32_t first, uint16_t second, bool isLeft, std::unordered_set<std::string>& leftArgs) const;


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
