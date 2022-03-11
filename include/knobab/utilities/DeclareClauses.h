//
// Created by Sam on 22/02/2022.
//

#ifndef KNOBAB_DECLARECLAUSES_H
#define KNOBAB_DECLARECLAUSES_H

#include "knobab/operators/LTLFOperators.h"
#include "Aggregators.h"



Result DChoice(const Result& a, const Result& b, const PredicateManager* manager = nullptr){
    Result futureA = future(a);
    Result futureB = future(b);

    Result futureAOrFutureB {};
    setUnionUntimed( futureA.begin(), futureA.end(), futureB.begin(), futureB.end(), std::back_inserter(futureAOrFutureB), Aggregators::maxSimilarity<double, double, double>, manager);
    return futureAOrFutureB;
}



Result ResponseExistence(const Result& a, const Result& notA, const Result& b, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    Result futureA = future(a);
    Result futureB = future(b);

    Result globalNotA = global(notA, lengths);

    Result aAndB{};
    setIntersectionUntimed( futureA.begin(), futureA.end(), futureB.begin(), futureB.end(), std::back_inserter(aAndB), Aggregators::maxSimilarity<double, double, double>, manager);

    Result globalNotAOrAndB;
    setUnionUntimed( globalNotA.begin(), globalNotA.end(), aAndB.begin(), aAndB.end(), std::back_inserter(globalNotAOrAndB), Aggregators::maxSimilarity<double, double, double>, manager);
    return globalNotAOrAndB;
}

Result Coexistence(const Result& a, const Result& notA, const Result& b, const Result& notB, const std::vector<size_t>& lengths, const PredicateManager* manager = nullptr){
    Result respExAB = ResponseExistence(a, notA, b, lengths, manager);
    Result respExBA = ResponseExistence(b, notB, a, lengths, manager);

    Result respExABAndRespExBA {};
    setIntersectionUntimed( respExAB.begin(), respExAB.end(), respExBA.begin(), respExBA.end(), std::back_inserter(respExABAndRespExBA), Aggregators::maxSimilarity<double, double, double>, nullptr);
    return respExABAndRespExBA;
}

Result NotCoexistence(const Result& notA, const Result& notB, const std::vector<size_t>& lengths){
    Result globalNotA = global(notA, lengths);
    Result globalNotB = global(notB, lengths);

    Result globalNotAOrGlobalNotB {};
    setUnionUntimed( globalNotA.begin(), globalNotA.end(), globalNotB.begin(), globalNotB.end(), std::back_inserter(globalNotAOrGlobalNotB), Aggregators::maxSimilarity<double, double, double>, nullptr);
    return globalNotAOrGlobalNotB;
}



Result NotCoexistence(const Result& A, const Result& B, const std::vector<size_t>& lengths, const PredicateManager* predMan, bool preserveNegatedFacts = true) {
    Result Fa = future(A);
    Result Fb = future(B);

    Result FaAbdFb {};
    setIntersectionUntimed( Fa.begin(), Fa.end(), Fb.begin(), Fb.end(), std::back_inserter(FaAbdFb), Aggregators::maxSimilarity<double, double, double>, predMan);

    //dataContainer Result = negateUntimed(FaAbdFb, lengths);
    return negateUntimed(FaAbdFb, lengths, preserveNegatedFacts);
}

Result ExclusiveChoice(const Result& a, const Result& notA, const Result& b, const Result& notB, const std::vector<size_t>& lengths){
    Result choiceAB = DChoice(a, b, nullptr);
    Result globalNotAOrGlobalNotB = NotCoexistence(notA, notB, lengths);

    Result choiceABAndGlobalNotAOrGlobalNotB {};
    setIntersectionUntimed( choiceAB.begin(), choiceAB.end(), globalNotAOrGlobalNotB.begin(), globalNotAOrGlobalNotB.end(), std::back_inserter(choiceABAndGlobalNotAOrGlobalNotB), Aggregators::maxSimilarity<double, double, double>, nullptr);
    return choiceABAndGlobalNotAOrGlobalNotB;
}

Result ExclusiveChoice(const Result& a, const Result& notA, const Result& b, const Result& notB, const std::vector<size_t>& lengths, const PredicateManager* manager, bool preserveNegatedFacts = true){
    Result choiceAB = DChoice(a, b, manager);
    Result globalNotAOrGlobalNotB = NotCoexistence(a, b, lengths, manager, preserveNegatedFacts);

    Result choiceABAndGlobalNotAOrGlobalNotB {};
    setIntersectionUntimed( choiceAB.begin(), choiceAB.end(), globalNotAOrGlobalNotB.begin(), globalNotAOrGlobalNotB.end(), std::back_inserter(choiceABAndGlobalNotAOrGlobalNotB), Aggregators::maxSimilarity<double, double, double>, nullptr);
    return choiceABAndGlobalNotAOrGlobalNotB;
}

Result DChainResponse(const Result& a, const Result& notA, const Result& b, const std::vector<size_t>& lengths, const PredicateManager* manager){
    Result nextB = next(b);
    Result aAndNextB {};
    setIntersection( a.begin(), a.end(), nextB.begin(), nextB.end(), std::back_inserter(aAndNextB), Aggregators::maxSimilarity<double, double, double>, manager);

    Result notAOrAAndNextB{};
    setUnion( aAndNextB.begin(), aAndNextB.end(), notA.begin(), notA.end(), std::back_inserter(notAOrAAndNextB), Aggregators::maxSimilarity<double, double, double>, manager);

    Result result = global(notAOrAAndNextB, lengths);
    return result;
}

Result DChainPrecedence(const Result& a, const Result& b, const Result& notB, const Result& lastElems, const std::vector<size_t>& lengths, const PredicateManager* manager){
    Result aORB {};
    setUnion( a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(aORB), Aggregators::maxSimilarity<double, double, double>, manager);

    Result filteredLastElems {};
    setIntersection( lastElems.begin(), lastElems.end(), aORB.begin(), aORB.end(), std::back_inserter(filteredLastElems), Aggregators::maxSimilarity<double, double, double>, manager);

    Result nextNotB = next(notB);
    Result nextNotBOrLast;
    setUnion( nextNotB.begin(), nextNotB.end(), filteredLastElems.begin(), filteredLastElems.end(), std::back_inserter(nextNotBOrLast), Aggregators::maxSimilarity<double, double, double>, manager);

    Result nextB = next(b);
    Result nextBAndA {};
    setIntersection( nextB.begin(), nextB.end(), a.begin(), a.end(), std::back_inserter(nextBAndA), Aggregators::maxSimilarity<double, double, double>, manager);

    Result nextNotBOrLastOrNextBAndA{};
    setUnion( nextNotBOrLast.begin(), nextNotBOrLast.end(), nextBAndA.begin(), nextBAndA.end(), std::back_inserter(nextNotBOrLastOrNextBAndA), Aggregators::maxSimilarity<double, double, double>, manager);

    Result result = global(nextNotBOrLastOrNextBAndA, lengths);
    return result;
}


Result DChainPrecedence_with_initial_constraint(const Result& a, const Result& b, const Result& notB, const Result& notFirstElems, const Result& lastElems, const std::vector<size_t>& lengths, const PredicateManager* manager){
    Result aORB {};
    setUnion( a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(aORB), Aggregators::maxSimilarity<double, double, double>, manager);

    Result filteredNotFirstElems {};
    setIntersection( notFirstElems.begin(), notFirstElems.end(), aORB.begin(), aORB.end(), std::back_inserter(filteredNotFirstElems), Aggregators::maxSimilarity<double, double, double>, manager);

    Result filteredLastElems {};
    setIntersection( lastElems.begin(), lastElems.end(), aORB.begin(), aORB.end(), std::back_inserter(filteredLastElems), Aggregators::maxSimilarity<double, double, double>, manager);

    Result nextNotB = next(notB);
    Result nextNotBOrLast;
    setUnion( nextNotB.begin(), nextNotB.end(), filteredLastElems.begin(), filteredLastElems.end(), std::back_inserter(nextNotBOrLast), Aggregators::maxSimilarity<double, double, double>, manager);

    Result notFirstOrNotB;
    setUnion( notB.begin(), notB.end(), filteredNotFirstElems.begin(), filteredNotFirstElems.end(), std::back_inserter(notFirstOrNotB), Aggregators::maxSimilarity<double, double, double>, manager);

    Result nextNotBOrLastAndNotFirstOrNotB;
    setIntersection( nextNotBOrLast.begin(), nextNotBOrLast.end(), notFirstOrNotB.begin(), notFirstOrNotB.end(), std::back_inserter(nextNotBOrLastAndNotFirstOrNotB), Aggregators::maxSimilarity<double, double, double>, manager);

    Result nextB = next(b);
    Result nextBAndA {};
    setIntersection( nextB.begin(), nextB.end(), a.begin(), a.end(), std::back_inserter(nextBAndA), Aggregators::maxSimilarity<double, double, double>, manager);

    Result nextNotBOrLastAndNotFirstOrNotBOrNextBAndA{};
    setUnion( nextNotBOrLastAndNotFirstOrNotB.begin(), nextNotBOrLastAndNotFirstOrNotB.end(), nextBAndA.begin(), nextBAndA.end(), std::back_inserter(nextNotBOrLastAndNotFirstOrNotBOrNextBAndA), Aggregators::maxSimilarity<double, double, double>, manager);

    Result result = global(nextNotBOrLastAndNotFirstOrNotBOrNextBAndA, lengths);
    return result;
}

Result DChainSuccession(const Result& a, const Result& notA, const Result& b, const Result& notB, const Result& lastElems, const std::vector<size_t>& lengths, const PredicateManager* manager){
    Result chainResponse = DChainResponse(a, notA, b, lengths, manager);
    Result chainPrecedence = DChainPrecedence(a, b, notB, lastElems, lengths, manager);

    Result chainResponseAndChainPrecedence {};
    setIntersection( chainResponse.begin(), chainResponse.end(), chainPrecedence.begin(), chainPrecedence.end(), std::back_inserter(chainResponseAndChainPrecedence), Aggregators::maxSimilarity<double, double, double>, manager);
    return chainResponseAndChainPrecedence;
}

Result DChainSuccession_with_initial_constraint(const Result& a, const Result& notA, const Result& b, const Result& notB, const Result& notLastElems, const Result& lastElems, const std::vector<size_t>& lengths, const PredicateManager* manager){
    Result chainResponse = DChainResponse(a, notA, b, lengths, manager);
    Result chainPrecedence = DChainPrecedence_with_initial_constraint(a, b, notB, notLastElems, lastElems, lengths, manager);

    Result chainResponseAndChainPrecedence {};
    setIntersection( chainResponse.begin(), chainResponse.end(), chainPrecedence.begin(), chainPrecedence.end(), std::back_inserter(chainResponseAndChainPrecedence), Aggregators::maxSimilarity<double, double, double>, manager);
    return chainResponseAndChainPrecedence;
}

Result NegationChainSuccession(const Result& a, const Result& notA, const Result& b, const Result& notB, const Result& lastElems, const std::vector<size_t>& lengths, const PredicateManager* manager){
    Result aORB {};
    setUnion( a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(aORB), Aggregators::maxSimilarity<double, double, double>, manager);

    Result filteredLastElems {};
    setIntersection( lastElems.begin(), lastElems.end(), aORB.begin(), aORB.end(), std::back_inserter(filteredLastElems), Aggregators::maxSimilarity<double, double, double>, manager);

    Result nextNotB = next(notB);
    Result nextNotBOrLast;
    setUnion( nextNotB.begin(), nextNotB.end(), filteredLastElems.begin(), filteredLastElems.end(), std::back_inserter(nextNotBOrLast), Aggregators::maxSimilarity<double, double, double>, manager);

    Result aAndNextNotBOrLast;
    setIntersection( nextNotBOrLast.begin(), nextNotBOrLast.end(), a.begin(), a.end(), std::back_inserter(aAndNextNotBOrLast), Aggregators::maxSimilarity<double, double, double>, manager);

    Result notAOrAAndNextNotB{};
    setUnion( aAndNextNotBOrLast.begin(), aAndNextNotBOrLast.end(), notA.begin(), notA.end(), std::back_inserter(notAOrAAndNextNotB), Aggregators::maxSimilarity<double, double, double>, manager);

    Result result = global(notAOrAAndNextNotB, lengths);
    return result;
}

#endif //KNOBAB_DECLARECLAUSES_H
