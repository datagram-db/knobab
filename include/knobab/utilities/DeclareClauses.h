//
// Created by Sam on 22/02/2022.
//

#ifndef KNOBAB_DECLARECLAUSES_H
#define KNOBAB_DECLARECLAUSES_H

#include "SetOperators.h"
#include "knobab/utilities/LTLFOperators.h"
#include "Aggregators.h"
dataContainer NotCoexistence(const dataContainer& notA, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr);

dataContainer DChoice(const dataContainer& a, const dataContainer& b, const PredicateManager* manager = nullptr){
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> futureA = future(a);
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> futureB = future(b);

    dataContainer futureAOrFutureB {};
    setUnionUntimed(futureA.begin(), futureA.end(), futureB.begin(), futureB.end(), std::back_inserter(futureAOrFutureB), Aggregators::maxSimilarity<double, double, double>, manager);
    return futureAOrFutureB;
}

dataContainer ExclusiveChoice(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    dataContainer choiceAB = DChoice(a, b, manager);
    dataContainer globalNotAOrGlobalNotB = NotCoexistence(notA, notB, lengths, manager);

    dataContainer choiceABAndGlobalNotAOrGlobalNotB {};
    setIntersectionUntimed(choiceAB.begin(), choiceAB.end(), globalNotAOrGlobalNotB.begin(), globalNotAOrGlobalNotB.end(), std::back_inserter(choiceABAndGlobalNotAOrGlobalNotB), Aggregators::maxSimilarity<double, double, double>, manager);
    return choiceABAndGlobalNotAOrGlobalNotB;
}

dataContainer ResponseExistence(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> futureA = future(a);
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> futureB = future(b);

    dataContainer aAndB;
    setIntersectionUntimed(futureA.begin(), futureA.end(), futureB.begin(), futureB.end(), std::back_inserter(aAndB), Aggregators::maxSimilarity<double, double, double>, manager);

    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> globalNotA = global(notA, lengths);
    dataContainer globalNotAOrAndB;
    setUnionUntimed(globalNotA.begin(), globalNotA.end(), aAndB.begin(), aAndB.end(), std::back_inserter(globalNotAOrAndB), Aggregators::maxSimilarity<double, double, double>, manager);
    return globalNotAOrAndB;
}

dataContainer Coexistence(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    dataContainer respExAB = ResponseExistence(a, notA, b, lengths, manager);
    dataContainer respExBA = ResponseExistence(b, notB,a, lengths, manager);

    dataContainer respExABAndRespExBA {};
    setIntersectionUntimed(respExAB.begin(), respExAB.end(), respExBA.begin(), respExBA.end(), std::back_inserter(respExABAndRespExBA), Aggregators::maxSimilarity<double, double, double>, manager);
    return respExABAndRespExBA;
}

dataContainer NotCoexistence(const dataContainer& notA, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager){
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> globalNotA = global(notA, lengths);
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>> globalNotB = global(notB, lengths);

    dataContainer globalNotAOrGlobalNotB {};
    setUnionUntimed(globalNotA.begin(), globalNotA.end(), globalNotB.begin(), globalNotB.end(), std::back_inserter(globalNotAOrGlobalNotB), Aggregators::maxSimilarity<double, double, double>, manager);
    return globalNotAOrGlobalNotB;
}

#endif //KNOBAB_DECLARECLAUSES_H
