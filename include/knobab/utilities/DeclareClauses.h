//
// Created by Sam on 22/02/2022.
//

#ifndef KNOBAB_DECLARECLAUSES_H
#define KNOBAB_DECLARECLAUSES_H

#include "SetOperators.h"
#include "knobab/utilities/LTLFOperators.h"
#include "Aggregators.h"
dataContainer NotCoexistence(const dataContainer& notA, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr);

dataContainer Choice(const dataContainer& a, const dataContainer& b, const PredicateManager* manager = nullptr){
    dataContainer futureA = future(a);
    dataContainer futureB = future(b);

    dataContainer futureAOrFutureB {};
    setUnion(false, futureA.begin(), futureA.end(), futureB.begin(), futureB.end(), std::back_inserter(futureAOrFutureB), Aggregators::maxSimilarity<double, double, double>, manager);
    return futureAOrFutureB;
}

dataContainer ExclusiveChoice(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    dataContainer choiceAB = Choice(a, b, manager);
    dataContainer globalNotAOrGlobalNotB = NotCoexistence(notA, notB, lengths, manager);

    dataContainer choiceABAndGlobalNotAOrGlobalNotB {};
    setIntersection(false, choiceAB.begin(), choiceAB.end(), globalNotAOrGlobalNotB.begin(), globalNotAOrGlobalNotB.end(), std::back_inserter(choiceABAndGlobalNotAOrGlobalNotB), Aggregators::maxSimilarity<double, double, double>, manager);
    return choiceABAndGlobalNotAOrGlobalNotB;
}

dataContainer ResponseExistence(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    dataContainer futureA = future(a);
    dataContainer futureB = future(b);

    dataContainer globalNotA = global(notA, lengths);

    dataContainer aAndB{};
    setIntersection(false, futureA.begin(), futureA.end(), futureB.begin(), futureB.end(), std::back_inserter(aAndB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer globalNotAOrAndB;
    setUnion(false, globalNotA.begin(), globalNotA.end(), aAndB.begin(), aAndB.end(), std::back_inserter(globalNotAOrAndB), Aggregators::maxSimilarity<double, double, double>, manager);
    return globalNotAOrAndB;
}

dataContainer Coexistence(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    dataContainer respExAB = ResponseExistence(a, notA, b, lengths, manager);
    dataContainer respExBA = ResponseExistence(b, notB,a, lengths, manager);

    dataContainer respExABAndRespExBA {};
    setIntersection(false, respExAB.begin(), respExAB.end(), respExBA.begin(), respExBA.end(), std::back_inserter(respExABAndRespExBA), Aggregators::maxSimilarity<double, double, double>, manager);
    return respExABAndRespExBA;
}

dataContainer NotCoexistence(const dataContainer& notA, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager){
    dataContainer globalNotA = global(notA, lengths);
    dataContainer globalNotB = global(notB, lengths);

    dataContainer globalNotAOrGlobalNotB {};
    setUnion(false, globalNotA.begin(), globalNotA.end(), globalNotB.begin(), globalNotB.end(), std::back_inserter(globalNotAOrGlobalNotB), Aggregators::maxSimilarity<double, double, double>, manager);
    return globalNotAOrGlobalNotB;
}

dataContainer ChainResponse(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const std::vector<size_t>& lengths, const PredicateManager* manager){
    dataContainer nextB = next(b);
    dataContainer aAndNextB {};
    setIntersection(true, a.begin(), a.end(), nextB.begin(), nextB.end(), std::back_inserter(aAndNextB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer notAOrAAndNextB{};
    setUnion(true, aAndNextB.begin(), aAndNextB.end(), notA.begin(), notA.end(), std::back_inserter(notAOrAAndNextB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer result = global(notAOrAAndNextB, lengths);
    return result;
}


dataContainer ChainPrecedence(const dataContainer& a, const dataContainer& b, const dataContainer& notB, const dataContainer& lastElems, const std::vector<size_t>& lengths, const PredicateManager* manager){
    dataContainer nextNotB = next(notB);
    dataContainer nextNotBOrLast;
    setUnion(true, nextNotB.begin(), nextNotB.end(), lastElems.begin(), lastElems.end(), std::back_inserter(nextNotBOrLast), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer nextB = next(b);
    dataContainer nextBAndA {};
    setIntersection(true, nextB.begin(), nextB.end(), a.begin(), a.end(), std::back_inserter(nextBAndA), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer nextNotBOrLastOrNextBAndA{};
    setUnion(true, nextNotBOrLast.begin(), nextNotBOrLast.end(), nextBAndA.begin(), nextBAndA.end(), std::back_inserter(nextNotBOrLastOrNextBAndA), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer result = global(nextNotBOrLastOrNextBAndA, lengths);
    return result;
}

#endif //KNOBAB_DECLARECLAUSES_H
