//
// Created by Sam on 22/02/2022.
//

#ifndef KNOBAB_DECLARECLAUSES_H
#define KNOBAB_DECLARECLAUSES_H

#include "knobab/utilities/LTLFOperators.h"
#include "Aggregators.h"

dataContainer NotCoexistence(const dataContainer& notA, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr);


dataContainer DChoice(const dataContainer& a, const dataContainer& b, const PredicateManager* manager = nullptr){
    dataContainer futureA = future(a);
    dataContainer futureB = future(b);

    dataContainer futureAOrFutureB {};
    setUnionUntimed( futureA.begin(), futureA.end(), futureB.begin(), futureB.end(), std::back_inserter(futureAOrFutureB), Aggregators::maxSimilarity<double, double, double>, manager);
    return futureAOrFutureB;
}

dataContainer ExclusiveChoice(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    dataContainer choiceAB = DChoice(a, b, manager);
    dataContainer globalNotAOrGlobalNotB = NotCoexistence(notA, notB, lengths, manager);

    dataContainer choiceABAndGlobalNotAOrGlobalNotB {};
    setIntersectionUntimed( choiceAB.begin(), choiceAB.end(), globalNotAOrGlobalNotB.begin(), globalNotAOrGlobalNotB.end(), std::back_inserter(choiceABAndGlobalNotAOrGlobalNotB), Aggregators::maxSimilarity<double, double, double>, manager);
    return choiceABAndGlobalNotAOrGlobalNotB;
}

dataContainer ResponseExistence(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    dataContainer futureA = future(a);
    dataContainer futureB = future(b);

    dataContainer globalNotA = global(notA, lengths);

    dataContainer aAndB{};
    setIntersectionUntimed( futureA.begin(), futureA.end(), futureB.begin(), futureB.end(), std::back_inserter(aAndB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer globalNotAOrAndB;
    setUnionUntimed( globalNotA.begin(), globalNotA.end(), aAndB.begin(), aAndB.end(), std::back_inserter(globalNotAOrAndB), Aggregators::maxSimilarity<double, double, double>, manager);
    return globalNotAOrAndB;
}

dataContainer Coexistence(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    dataContainer respExAB = ResponseExistence(a, notA, b, lengths, manager);
    dataContainer respExBA = ResponseExistence(b, notB,a, lengths, manager);

    dataContainer respExABAndRespExBA {};
    setIntersectionUntimed( respExAB.begin(), respExAB.end(), respExBA.begin(), respExBA.end(), std::back_inserter(respExABAndRespExBA), Aggregators::maxSimilarity<double, double, double>, manager);
    return respExABAndRespExBA;
}

dataContainer NotCoexistence(const dataContainer& notA, const dataContainer& notB, const std::vector<size_t>& lengths, const PredicateManager* manager){
    dataContainer globalNotA = global(notA, lengths);
    dataContainer globalNotB = global(notB, lengths);

    dataContainer globalNotAOrGlobalNotB {};
    setUnionUntimed( globalNotA.begin(), globalNotA.end(), globalNotB.begin(), globalNotB.end(), std::back_inserter(globalNotAOrGlobalNotB), Aggregators::maxSimilarity<double, double, double>, manager);
    return globalNotAOrGlobalNotB;
}

dataContainer DChainResponse(const dataContainer& a, const dataContainer& notA, const dataContainer& b, const std::vector<size_t>& lengths, const PredicateManager* manager){
    dataContainer nextB = next(b);
    dataContainer aAndNextB {};
    setIntersection( a.begin(), a.end(), nextB.begin(), nextB.end(), std::back_inserter(aAndNextB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer notAOrAAndNextB{};
    setUnion( aAndNextB.begin(), aAndNextB.end(), notA.begin(), notA.end(), std::back_inserter(notAOrAAndNextB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer result = global(notAOrAAndNextB, lengths);
    return result;
}


dataContainer DChainPrecedence(const dataContainer& a, const dataContainer& b, const dataContainer& notB, const dataContainer& notFirstElems, const dataContainer& lastElems, const std::vector<size_t>& lengths, const PredicateManager* manager){
    dataContainer aORB {};
    setUnion( a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(aORB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer filteredNotFirstElems {};
    setIntersection( notFirstElems.begin(), notFirstElems.end(), aORB.begin(), aORB.end(), std::back_inserter(filteredNotFirstElems), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer filteredLastElems {};
    setIntersection( lastElems.begin(), lastElems.end(), aORB.begin(), aORB.end(), std::back_inserter(filteredLastElems), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer nextNotB = next(notB);
    dataContainer nextNotBOrLast;
    setUnion( nextNotB.begin(), nextNotB.end(), filteredLastElems.begin(), filteredLastElems.end(), std::back_inserter(nextNotBOrLast), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer notFirstOrNotB;
    setUnion( notB.begin(), notB.end(), filteredNotFirstElems.begin(), filteredNotFirstElems.end(), std::back_inserter(notFirstOrNotB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer nextNotBOrLastAndNotFirstOrNotB;
    setIntersection( nextNotBOrLast.begin(), nextNotBOrLast.end(), notFirstOrNotB.begin(), notFirstOrNotB.end(), std::back_inserter(nextNotBOrLastAndNotFirstOrNotB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer nextB = next(b);
    dataContainer nextBAndA {};
    setIntersection( nextB.begin(), nextB.end(), a.begin(), a.end(), std::back_inserter(nextBAndA), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer nextNotBOrLastAndNotFirstOrNotBOrNextBAndA{};
    setUnion( nextNotBOrLastAndNotFirstOrNotB.begin(), nextNotBOrLastAndNotFirstOrNotB.end(), nextBAndA.begin(), nextBAndA.end(), std::back_inserter(nextNotBOrLastAndNotFirstOrNotBOrNextBAndA), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer result = global(nextNotBOrLastAndNotFirstOrNotBOrNextBAndA, lengths);
    return result;
}

dataContainer DChainSuccession(const dataContainer& a,  const dataContainer& notA, const dataContainer& b, const dataContainer& notB, const dataContainer& notLastElems, const dataContainer& lastElems, const std::vector<size_t>& lengths, const PredicateManager* manager){
    dataContainer chainResponse = DChainResponse(a, notA, b, lengths, manager);
    dataContainer chainPrecedence = DChainPrecedence(a, b, notB, notLastElems, lastElems, lengths,manager);

    dataContainer chainResponseAndChainPrecedence {};
    setIntersection( chainResponse.begin(), chainResponse.end(), chainPrecedence.begin(), chainPrecedence.end(), std::back_inserter(chainResponseAndChainPrecedence), Aggregators::maxSimilarity<double, double, double>, manager);
    return chainResponseAndChainPrecedence;
}

dataContainer NegationChainSuccession(const dataContainer& a,  const dataContainer& notA, const dataContainer& b, const dataContainer& notB, const dataContainer& lastElems, const std::vector<size_t>& lengths, const PredicateManager* manager){
    dataContainer aORB {};
    setUnion( a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(aORB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer filteredLastElems {};
    setIntersection( lastElems.begin(), lastElems.end(), aORB.begin(), aORB.end(), std::back_inserter(filteredLastElems), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer nextNotB = next(notB);
    dataContainer nextNotBOrLast;
    setUnion( nextNotB.begin(), nextNotB.end(), filteredLastElems.begin(), filteredLastElems.end(), std::back_inserter(nextNotBOrLast), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer aAndNextNotBOrLast;
    setIntersection( nextNotBOrLast.begin(), nextNotBOrLast.end(), a.begin(), a.end(), std::back_inserter(aAndNextNotBOrLast), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer notAOrAAndNextNotB{};
    setUnion( aAndNextNotBOrLast.begin(), aAndNextNotBOrLast.end(), notA.begin(), notA.end(), std::back_inserter(notAOrAAndNextNotB), Aggregators::maxSimilarity<double, double, double>, manager);

    dataContainer result = global(notAOrAAndNextNotB, lengths);
    return result;
}

#endif //KNOBAB_DECLARECLAUSES_H
