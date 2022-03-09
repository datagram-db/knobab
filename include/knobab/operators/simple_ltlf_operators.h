//
// Created by giacomo on 09/03/2022.
//

#ifndef KNOBAB_SIMPLE_LTLF_OPERATORS_H
#define KNOBAB_SIMPLE_LTLF_OPERATORS_H

#include <knobab/operators/semantics.h>

/**
 *
 * @author Giacomo Bergami
 *
 * @param section
 * @param lengths
 * @param result
 */
inline void future_logic_timed(const dataContainer &section, const std::vector<size_t>& lengths, dataContainer& result) {
    //std::cout << section << std::endl;
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    constexpr auto max16 = std::numeric_limits<uint16_t>::max();

    std::pair<uint32_t, uint16_t> first;
    std::pair<double, std::vector<uint16_t>> second{0.0, 0.0};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cp{{0,0}, {1.0, {}}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = cp.first.first = currentTraceId;
        cp.first.second = lengths.at(currentTraceId);
        first.second = 0;

        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp);

        dataContainer toBeReversed;
        auto it = lower+std::distance(lower,upper)-1;
        bool isBegin = true;
        for (int64_t i = (upper-1)->first.second; i >= 0; i--) {
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
inline void future_logic_untimed(const dataContainer &section, const std::vector<size_t>& lengths, dataContainer& result) {
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    constexpr auto max16 = std::numeric_limits<uint16_t>::max();

    std::pair<uint32_t, uint16_t> first{0, 0};
    std::pair<double, std::vector<uint16_t>> second{0.0, {}};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cp{{0,0}, {1.0, {}}};

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
inline void global_logic_timed(const dataContainer &section, const std::vector<size_t>& lengths, dataContainer& result) {
    //std::cout << section << std::endl;
    //dataContainer temp {};
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    constexpr auto max16 = std::numeric_limits<uint16_t>::max();

    std::pair<uint32_t, uint16_t> first;
    std::pair<double, std::vector<uint16_t>> second{0.0, 0.0};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cp{{0,0}, {1.0, {}}};

    while(upper != end){
        uint32_t currentTraceId = upper->first.first;
        first.first = cp.first.first = currentTraceId;
        cp.first.second = lengths.at(currentTraceId);
        first.second = 0;

        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp);

        dataContainer toBeReversed;
        auto it = lower+std::distance(lower,upper)-1;
        bool isBegin = true;
        for (int64_t i = (upper-1)->first.second; i >= 0; i--) {
            first.second = i;
            const uint32_t dist = std::distance(it, upper - 1);

            if(dist == (cp.first.second - it->first.second)){
                second.first = std::min(it->second.first, second.first);
                second.second.insert(second.second.begin(), it->second.second.begin(), it->second.second.end());
                remove_duplicates(second.second);
                //temp.emplace_back(std::pair<uint32_t, uint16_t>{traceId, startEventId}, std::pair<double, std::vector<uint16_t>>{1,vec});
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
inline void global_logic_untimed(const dataContainer &section, const std::vector<size_t>& lengths, dataContainer& result) {
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();
    result.clear();

    std::pair<uint32_t, uint16_t> first{0, 0}, firstLast;
    std::pair<double, std::vector<uint16_t>> second{1.0, {}};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cp{{0,0}, {1.0, {}}};

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



inline void until_logic_untimed(const dataContainer &aSection, const dataContainer &bSection, const std::vector<size_t>& lengths, dataContainer& temp, const PredicateManager* manager = nullptr) {
    auto lower = bSection.begin();
    auto localUpper = lower;
    auto upper = bSection.end();

    auto aIt = aSection.begin();
    auto aEn = aSection.begin();
    auto upperA = aSection.end();

    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cpAIt{{0, 0}, {0, {}}};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cpLocalUpper{{0, 0}, {1.0, {}}};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cpAEn{{0, 0}, {1.0, maxVec}};
    std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> cpResult{{0, 0}, {1.0, {}}};

    ///dataContainer temp {};
    env e1, e2;
    std::pair<uint32_t, uint16_t> Fut, Prev;

    while (lower != upper) {
        uint32_t currentTraceId = localUpper->first.first;
        cpAIt.first.first = cpLocalUpper.first.first = cpAEn.first.first = cpResult.first.first = currentTraceId;
        cpLocalUpper.first.second = lengths.at(currentTraceId);

        localUpper = std::upper_bound(lower, upper, cpLocalUpper);
        aIt = std::lower_bound(aIt, upperA, cpAIt);
        aEn = aIt;

        for( ; lower != localUpper; lower++) {
            Fut.first = lower->first.first;
            if (lower->first.second == 0) {
                temp.emplace_back(*lower);
            } else {
                cpAEn.first.second = lower->first.second-1;
                aEn = std::upper_bound(aEn, upperA, cpAEn/*std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>{{currentTraceId, lower->first.second-1},  {1, maxVec}}*/);
                if(aIt == aEn){
                    // Rationale: (1)
                    // if the condition does not hold for a time [startEventId, lower->first.second-1], it is because one event makes it not hold.
                    // Therefore, it should never hold even if you are extending the data that you have.
                    break;
                } else {
                    const uint32_t dist = std::distance(aIt, aEn - 1);
                    cpResult.second.second.clear();

                    if(dist == ((lower->first.second))-1){
                        if (manager) {
                            bool hasFail = false;
                            for (uint16_t activationEvent : lower->second.second) {
                                if (hasFail) break;
                                Fut.second = activationEvent;
                                e1 = manager->GetPayloadDataFromEvent(Fut);
                                for (auto curr = aIt; curr != aEn; curr++) {
                                    if (hasFail) break;
                                    Prev.first = curr->first.first;
                                    for (uint16_t targetEvent : curr->second.second) {
                                        Prev.second = targetEvent;
                                        e2 = manager->GetPayloadDataFromEvent(Prev);
                                        if (!manager->checkValidity(e2, e1)) {
                                            hasFail = true;
                                            break;
                                        } else {
                                            cpResult.second.second.emplace_back(targetEvent);
                                        }
                                    }
                                }
                            }
                            if (hasFail) break;
                            std::sort(cpResult.second.second.begin(), cpResult.second.second.end());
                            cpResult.second.second.erase(std::unique(cpResult.second.second.begin(), cpResult.second.second.end()), cpResult.second.second.end());
                            cpResult.second.second.insert(cpResult.second.second.begin(), lower->second.second.begin(), lower->second.second.end());
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

#endif //KNOBAB_SIMPLE_LTLF_OPERATORS_H
