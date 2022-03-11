//
// Created by giacomo on 09/03/2022.
//

#ifndef KNOBAB_SIMPLE_LTLF_OPERATORS_H
#define KNOBAB_SIMPLE_LTLF_OPERATORS_H

#include <knobab/operators/semantics.h>

#if 0
/**
 *
 * @author Samuel 'Sam' Appleby, Giacomo Bergami
 *
 * @param section
 * @param lengths
 * @param result
 */
void inline union_logic_untimed(const Result& lhs, const Result& rhs, Result& result, std::function<double(double, double)> aggr,
                                const PredicateManager *manager = nullptr, bool dropMatches = false) {
    auto first1 = lhs.begin(), last1 = lhs.end(), first2 = rhs.begin(), last2 = rhs.end();
    std::map<uint32_t, Result> group1 = GroupByKeyExtractor<decltype(first1), uint32_t, ResultRecord>(
            first1, last1, [](const auto &p) { return p.first.first; });

    std::map<uint32_t, Result> group2 = GroupByKeyExtractor<decltype(first2), uint32_t, ResultRecord>(
            first2, last2, [](const auto &p) { return p.first.first; });

    env e1, e2;
    ResultIndex pair, pair1;
    auto start1 = group1.begin(), end1 = group1.end();
    auto start2 = group2.begin(), end2 = group2.end();
    ResultRecord val;

    for (; start1 != end1; /*++d_first*/) {
        if (start2 == end2) {
            while (start1 != end1) {
                for (const auto &cont1: start1->second) {
                    result.emplace_back(cont1);
                }
                start1++;
            }
            return;
        } else if (start1->first > start2->first) {
            for (const auto &cont1: start2->second) {
                result.emplace_back(cont1);
            }
            start2++;
        } else if (start1->first < start2->first) {
            for (const auto &cont1: start1->second) {
                result.emplace_back(cont1);
            }
            start1++;
        } else {
            pair.first = val.first.first = start1->first;
            pair1.first = start2->first;
            for (const auto &cont1: start1->second) {
                for (const auto &cont2: start2->second) {
                    if (manager) {
                        for (const auto &elem: cont1.second.second) {
                            if (!IS_MARKED_EVENT_ACTIVATION(elem)) continue;
                            pair.second = GET_ACTIVATION_EVENT(elem);
                            for (const auto &elem1: cont2.second.second) {
                                if (!IS_MARKED_EVENT_TARGET(elem)) continue;
                                pair1.second = GET_TARGET_EVENT(elem1);
                                e1 = manager->GetPayloadDataFromEvent(pair);
                                e2 = manager->GetPayloadDataFromEvent(pair1);

                                if (manager->checkValidity(e1, e2)) {
                                    val.second.first = aggr(cont1.second.first, cont2.second.first);
                                    if (!dropMatches) {
                                        val.second.second = {pair.second, pair1.second};
                                    } else {
                                        val.second.second.clear();
                                    }
                                    result.emplace_back(val);
                                }
                            }
                        }
                    } else {
                        // NOTE: that will discard potential activation and target conditions, and just put the two events where the situation holds.
                        val.second.second.clear();
                        if (!dropMatches) {
                            first1->second.second;
                            val.second.second.insert(val.second.second.end(), first2->second.second.begin(), first2->second.second.end());
                            remove_duplicates(val.second.second);
                        }
                        val.second.first = aggr(cont1.second.first, cont2.second.first);
                        result.emplace_back(val);
                    }
                }
            }
            start1++;
            start2++;
        }
    }

    while (start2 != end2) {
        for (const auto &cont1: start2->second) {

            result.emplace_back(cont1);
        }
        start2++;
    }
    //return d_first;
}
#endif

/**
 *
 * @author Giacomo Bergami
 *
 * @param section
 * @param lengths
 * @param result
 */
inline void future_logic_timed(const Result &section, const std::vector<size_t>& lengths, Result& result) {
    //std::cout << section << std::endl;
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    constexpr auto max16 = std::numeric_limits<uint16_t>::max();

    ResultIndex first;
    ResultRecordSemantics second{0.0, 0.0};
    ResultRecord cp{{0,0}, {1.0, {}}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = cp.first.first = currentTraceId;
        cp.first.second = lengths.at(currentTraceId);
        first.second = 0;

        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp);

        Result toBeReversed;
        auto it = lower+std::distance(lower,upper)-1;
        bool isBegin = true;
        for (int64_t i = (upper-1)->first.second; (i >= 0) && (it >= lower); i--) {
            first.second = i;
            if (it->first.second == i) {
                second.first = std::max(it->second.first, second.first);
                second.second.insert(second.second.begin(), it->second.second.begin(), it->second.second.end());
                remove_duplicates(second.second);
                if (isBegin) isBegin = false;
                it--;
            } else {
                assert(!isBegin);
            }
            toBeReversed.emplace_back(first, second);
        }

        // Inserting the elements in reversed order
        result.insert(result.end(), std::make_move_iterator(toBeReversed.rbegin()),
                    std::make_move_iterator(toBeReversed.rend()));

        second.second.clear();
    }
}

/**
 *
 * @author Samuel 'Sam' Appleby, Giacomo Bergami
 *
 * @param section
 * @param lengths
 * @param result
 */
inline void future_logic_untimed(const Result &section, const std::vector<size_t>& lengths, Result& result) {
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();

    ResultIndex first{0, 0};
    ResultRecordSemantics second{0.0, {}};
    ResultRecord cp{{0,0}, {1.0, {}}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = cp.first.first = currentTraceId;
        cp.first.second = lengths.at(currentTraceId);
        second.second.clear();
        second.first = 0.0;

        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp);

        for (auto it = lower+std::distance(lower,upper)-1; it >= lower; it--) {
            second.first = std::max(second.first, it->second.first);
            second.second.insert(second.second.end(), it->second.second.begin(), it->second.second.end());
        }
        remove_duplicates(second.second);
        result.emplace_back(first, second);
    }
}

/**
 *
 * @author Giacomo Bergami
 *
 * @param section
 * @param lengths
 * @param result
 */
inline void global_logic_timed(const Result &section, const std::vector<size_t>& lengths, Result& result) {
    //std::cout << section << std::endl;
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    constexpr auto max16 = std::numeric_limits<uint16_t>::max();

    ResultIndex first;
    ResultRecordSemantics second{1.0, 0.0};
    ResultRecord cp{{0,0}, {1.0, {}}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = cp.first.first = currentTraceId;
        cp.first.second = lengths.at(currentTraceId);
        first.second = 0;

        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp);

        Result toBeReversed;
        auto it = lower+std::distance(lower,upper)-1;
        bool isBegin = true;
        for (int64_t i = (upper-1)->first.second; i >= 0; i--) {
            first.second = i;
            const uint32_t dist = std::distance(it, upper);

            if((cp.first.first == it->first.first) && (dist == (cp.first.second - it->first.second))){
                second.first = std::min(it->second.first, second.first);
                second.second.insert(second.second.begin(), it->second.second.begin(), it->second.second.end());
                remove_duplicates(second.second);
                it--;
            } else {
                break; // If after this the condition does not hold, then it means that in the remainder I will have
                // events that are not matching the condition
            }
            toBeReversed.emplace_back(first, second);
        }

        // Inserting the elements in reversed order
        result.insert(result.end(), std::make_move_iterator(toBeReversed.rbegin()),
                      std::make_move_iterator(toBeReversed.rend()));

        second.second.clear();
    }
}

/**
 *
 * @author Samuel 'Sam' Appleby, Giacomo Bergami
 *
 * @param section
 * @param lengths
 * @param result
 */
inline void global_logic_untimed(const Result &section, const std::vector<size_t>& lengths, Result& result) {
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    result.clear();

    ResultIndex first{0, 0}, firstLast;
    ResultRecordSemantics second{1.0, {}};
    ResultRecord cp{{0,0}, {1.0, {}}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = cp.first.first = currentTraceId;
        cp.first.second = lengths.at(currentTraceId);
        cp.second.second.clear();

        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp);

        const uint32_t dist = std::distance(lower, upper - 1);

        if(dist == cp.first.second - 1){
            populateAndReturnEvents(lower, upper, second.second);
            result.emplace_back(first, second);
        }
    }
}

#include <iostream>
#include <queue>

/**
 *
 * @author Giacomo Bergami
 *
 * @param section
 * @param lengths
 * @param result
 */
inline void until_logic_timed(const Result &aSection, const Result &bSection, const std::vector<size_t>& lengths, Result& temp, const PredicateManager* manager = nullptr) {
    auto lower = bSection.begin();
    auto localUpper = lower;
    auto upper = bSection.end();

    auto aIt = aSection.begin();
    auto aEn = aSection.begin();
    auto upperA = aSection.end();
    bool setUntilHolds;
    uint16_t untilHolds;

    ResultRecord cpAIt{{0, 0}, {0, {}}};
    ResultRecord cpLocalUpper{{0, 0}, {1.0, {}}};
    ResultRecord cpAEn{{0, 0}, {1.0, maxVec}};
    ResultRecord cpResult{{0, 0}, {1.0, {}}};

    ///dataContainer temp {};
    env e1, e2;
    std::pair<uint32_t, uint16_t> Fut, Prev;
    temp.clear();

    while (lower != upper) {
        setUntilHolds = true;
        uint32_t currentTraceId = localUpper->first.first;
        cpAIt.first.first = cpLocalUpper.first.first = cpAEn.first.first = cpResult.first.first = currentTraceId;
        cpLocalUpper.first.second = lengths.at(currentTraceId);
        cpAIt.first.second = 0;

        localUpper = std::upper_bound(lower, upper, cpLocalUpper);
        aIt = std::lower_bound(aIt, upperA, cpAIt);
        aEn = aIt;
        std::queue<decltype(aEn)> nextCandidateQueue;

        while (lower != localUpper) {
            Fut.first = lower->first.first;
            if (lower->first.second == 0) {
                temp.emplace_back(*lower);
            } else {
                cpAEn.first.second = lower->first.second;
                aEn = std::upper_bound(aEn, upperA, cpAEn);
                if (aEn->first.first == currentTraceId)
                    nextCandidateQueue.emplace(aEn);
                if(aIt == aEn){
                    // Rationale: (1)
                    // if the condition does not hold for a time [startEventId, lower->first.second-1], it is because
                    // one event makes it not hold. Therefore, it should never hold even if you are extending the data
                    // that you have.
                    setUntilHolds = true;
                    if (nextCandidateQueue.empty()) break;
                    aIt = nextCandidateQueue.front();
                    nextCandidateQueue.pop();
                    cpAEn.first.second = lower->first.second;
                    aEn = std::upper_bound(aEn, upperA, cpAEn);
                    if (aEn->first.first == currentTraceId)
                        nextCandidateQueue.emplace(aEn);
                    lower++;
                    continue;
                } else {
                    const uint32_t dist = std::distance(aIt, aEn);
                    cpResult.second.second.clear();
                    if (setUntilHolds) {
                        untilHolds = aIt->first.second;
                        setUntilHolds = false;
                    }

                    if(dist == ((lower->first.second)-aIt->first.second)){
                        if (manager) {
                            bool hasFail = false;
                            for (auto& activationEvent : lower->second.second) {
                                if (hasFail) break;
                                if (!IS_MARKED_EVENT_TARGET(activationEvent)) continue;
                                Fut.second = GET_TARGET_EVENT(activationEvent);
                                e1 = manager->GetPayloadDataFromEvent(Fut);
                                for (auto curr = aIt; curr != aEn; curr++) {
                                    if (hasFail) break;
                                    Prev.first = curr->first.first;
                                    for (auto& targetEvent : curr->second.second) {
                                        if (!IS_MARKED_EVENT_ACTIVATION(targetEvent)) continue;
                                        Prev.second = GET_ACTIVATION_EVENT(targetEvent);
                                        e2 = manager->GetPayloadDataFromEvent(Prev);
                                        if (!manager->checkValidity(e2, e1)) {
                                            hasFail = true;
                                            break;
                                        } else {
                                            cpResult.second.second.emplace_back(marked_event::join(Fut.second, Prev.second));
                                        }
                                    }
                                }
                            }
                            if (hasFail) {
                                setUntilHolds = true;
                                if (nextCandidateQueue.empty()) break;
                                aIt = nextCandidateQueue.front();
                                nextCandidateQueue.pop();
                                ///aIt = aEn;
                                ///aIt--;
                                cpAEn.first.second = lower->first.second;
                                aEn = std::upper_bound(aEn, upperA, cpAEn);
                                if (aEn->first.first == currentTraceId)
                                    nextCandidateQueue.emplace(aEn);
                                lower++;
                                continue;
                            }
//                            std::sort(cpResult.second.second.begin(), cpResult.second.second.end());
//                            cpResult.second.second.erase(std::unique(cpResult.second.second.begin(), cpResult.second.second.end()), cpResult.second.second.end());
//                            cpResult.second.second.insert(cpResult.second.second.begin(), lower->second.second.begin(), lower->second.second.end());
                        } else {
                            populateAndReturnEvents(aIt, aEn, cpResult.second.second);
                            cpResult.second.second.insert(cpResult.second.second.begin(), lower->second.second.begin(), lower->second.second.end());
                        }
                        remove_duplicates(cpResult.second.second);
                        auto itAllEnd = cpResult.second.second.end();
                        auto it = std::lower_bound(cpResult.second.second.begin(), itAllEnd, manager ? marked_event::join(untilHolds, lower->first.second) : marked_event::activation(untilHolds));
                        auto itEnd = std::upper_bound(it, itAllEnd, manager ? marked_event::join(lower->first.second-1, lower->first.second) : marked_event::activation(lower->first.second-1));
                        bool hasMatch = it != itEnd;
                        for (uint16_t i = untilHolds; i<=lower->first.second; i++) {
                            cpResult.first.second = i;
                            temp.emplace_back(cpResult);
                            if (hasMatch && (i < lower->first.second)) {
//                                assert(IS_MARKED_EVENT_ACTIVATION(*it) || IS_MARKED_EVENT_MATCH(*it));
//                                assert(GET_ACTIVATION_EVENT(*it) == i);
                                it = cpResult.second.second.erase(it);
//                                itEnd = std::upper_bound(it, itAllEnd, manager ? marked_event::join(lower->first.second-1, lower->first.second) : marked_event::activation(lower->first.second-1));
                            }
                        }
                    } else {
                        // For (1)
                        setUntilHolds = true;
                        if (nextCandidateQueue.empty()) break;
                        aIt = nextCandidateQueue.front();
                        nextCandidateQueue.pop();
                        ///aIt = aEn;
                        ///aIt--;
                        cpAEn.first.second = lower->first.second;
                        aEn = std::upper_bound(aEn, upperA, cpAEn);
                        if (aEn->first.first == currentTraceId)
                            nextCandidateQueue.emplace(aEn);
                        continue;
                    }
                }
            }
            lower++;
        }

        lower = localUpper;
    }
}


inline void until_logic_untimed(const Result &aSection, const Result &bSection, const std::vector<size_t>& lengths, Result& temp, const PredicateManager* manager = nullptr) {
    auto lower = bSection.begin();
    auto localUpper = lower;
    auto upper = bSection.end();

    auto aIt = aSection.begin();
    auto aEn = aSection.begin();
    auto upperA = aSection.end();

    ResultRecord cpAIt{{0, 0}, {0, {}}};
    ResultRecord cpLocalUpper{{0, 0}, {1.0, {}}};
    ResultRecord cpAEn{{0, 0}, {1.0, maxVec}};
    ResultRecord cpResult{{0, 0}, {1.0, {}}};

    ///dataContainer temp {};
    env e1, e2;
    std::pair<uint32_t, uint16_t> Fut, Prev;
    temp.clear();

    while (lower != upper) {
        uint32_t currentTraceId = localUpper->first.first;
        cpAIt.first.first = cpLocalUpper.first.first = cpAEn.first.first = cpResult.first.first = currentTraceId;
        cpLocalUpper.first.second = lengths.at(currentTraceId);
        cpAIt.first.second = 0;

        localUpper = std::upper_bound(lower, upper, cpLocalUpper);
        aIt = std::lower_bound(aIt, upperA, cpAIt);
        aEn = aIt;

        for( ; lower != localUpper; lower++) {
            Fut.first = lower->first.first;
            if (lower->first.second == 0) {
                temp.emplace_back(*lower);
            } else {
                cpAEn.first.second = lower->first.second-1;
                aEn = std::upper_bound(aEn, upperA, cpAEn);
                if(aIt == aEn){
                    // Rationale: (1)
                    // if the condition does not hold for a time [startEventId, lower->first.second-1], it is because
                    // one event makes it not hold. Therefore, it should never hold even if you are extending the data
                    // that you have.
                    break;
                } else {
                    const uint32_t dist = std::distance(aIt, aEn - 1);
                    cpResult.second.second.clear();

                    if(dist == ((lower->first.second))-1) {
                        if (manager) {
                            bool hasFail = false;
                            for (auto& activationEvent : lower->second.second) {
                                if (hasFail) break;
                                if (!IS_MARKED_EVENT_TARGET(activationEvent)) continue;
                                Fut.second = GET_TARGET_EVENT(activationEvent);
                                e1 = manager->GetPayloadDataFromEvent(Fut);
                                for (auto curr = aIt; curr != aEn; curr++) {
                                    if (hasFail) break;
                                    Prev.first = curr->first.first;
                                    for (auto& targetEvent : curr->second.second) {
                                        if (!IS_MARKED_EVENT_ACTIVATION(targetEvent)) continue;
                                        Prev.second = GET_ACTIVATION_EVENT(targetEvent);
                                        e2 = manager->GetPayloadDataFromEvent(Prev);
                                        if (!manager->checkValidity(e2, e1)) {
                                            hasFail = true;
                                            break;
                                        } else {
                                            cpResult.second.second.emplace_back(marked_event::join(Fut.second, Prev.second));
                                        }
                                    }
                                }
                            }
                            if (hasFail) break;
//                            std::sort(cpResult.second.second.begin(), cpResult.second.second.end());
//                            cpResult.second.second.erase(std::unique(cpResult.second.second.begin(), cpResult.second.second.end()), cpResult.second.second.end());
//                            cpResult.second.second.insert(cpResult.second.second.begin(), lower->second.second.begin(), lower->second.second.end());
                        } else {
                            populateAndReturnEvents(aIt, aEn, cpResult.second.second);
                            cpResult.second.second.insert(cpResult.second.second.begin(), lower->second.second.begin(), lower->second.second.end());
                        }
                        temp.emplace_back(cpResult);
                    } else {
                        // For (1)
                        break;
                    }
                }
            }

        }

        lower = localUpper;
    }
}


inline void weakUntil_logic_untimed(const Result &aSection,
                                    const Result &bSection,
                                    const std::vector<size_t>& lengths,
                                    Result& result,
                                    const PredicateManager* manager = nullptr) {
    Result untilR, globally;
    until_logic_untimed(aSection, bSection, lengths, untilR, manager);
    global_logic_untimed(aSection, lengths, globally);
    //TODO: setUnionUntimed(untilR.begin(), untilR.end(), globally.begin(), globally.end(), std::back_inserter(result), Aggregators::joinSimilarity<double, double, double>, nullptr);
    //TODO: return result;
}


#endif //KNOBAB_SIMPLE_LTLF_OPERATORS_H
