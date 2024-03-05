
#pragma once

/// THESE OPERATORS NEED to be used only when elements belong to the same root!

inline void polydl_sameroot_or_fast_timed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    DEBUG_ASSERT(!manager);
    or_fast_timed(lhs, rhs, out, manager, lengths);
}

inline void polydl_sameroot_or_fast_untimed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    DEBUG_ASSERT(!manager);
    or_fast_untimed(lhs, rhs, out, manager, lengths);
}

inline void polydl_sameroot_and_fast_timed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    DEBUG_ASSERT(!manager);
    auto first1 = lhs.begin(), first2 = rhs.begin(),
            last1 = lhs.end(), last2 = rhs.end();
//    env e1, e2;
    ResultIndex pair, pair1;
    ResultRecord result{{0, 0}, {1.0, {}}};
    bool hasMatch;
    marked_event join = marked_event::join(0,0);
    std::unordered_set<std::string> cache;

    while (first1 != last1) {
        if (first2 == last2)
            return;
        if (first1->first > first2->first) {
            first2++;
        } else if (first1->first < first2->first) {
            first1++;
        } else {
            pair.first = first1->first.first;
            result.first = first1->first;
            result.second.first = std::min(first1->second.first, first2->second.first);
            result.second.second.clear();
            pair1.first = first2->first.first;
            hasMatch = false;
            {
                hasMatch |= (for_each_if(first1->second.second.begin(), first1->second.second.end(),
                                         [hasMatch](const marked_event& elem) {
                                             return (!hasMatch) && (!IS_MARKED_EVENT_ACTIVATION(elem));
                                         },
                                         [first2,&hasMatch](const marked_event& elem) {
                                             size_t c = for_each_if(first2->second.second.begin(), first2->second.second.end(),
                                                                    [hasMatch](const marked_event& elem) {
                                                                        return (!hasMatch) && (!IS_MARKED_EVENT_TARGET(elem));
                                                                    },[&hasMatch,elem](const marked_event& elem1) {
                                                         hasMatch = true;
                                                     });
                                             if (c==0) hasMatch= true;
                                         }) == 0);
                if (hasMatch) {
                    result.second.second.insert(result.second.second.end(), first1->second.second.begin(), first1->second.second.end());
                    result.second.second.insert(result.second.second.end(), first2->second.second.begin(), first2->second.second.end());
                }
            }

            if (hasMatch) {
                remove_duplicates(result.second.second);
                out.emplace_back(result);
            }

            first1++;
            first2++;
        }
    }
}

inline void polydl_sameroot_and_fast_untimed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    DEBUG_ASSERT(!manager);
    and_fast_untimed(lhs, rhs, out, nullptr, lengths);
}

inline void polydl_sameroot_future_timed(const Result &section, Result &result, const std::vector<size_t> &lengths) {
    future_logic_timed(section, result, lengths);
}

inline void polydl_sameroot_future_untimed(const Result &section, Result &result, const std::vector<size_t> &lengths) {
    future_logic_untimed(section, result, lengths);
}

inline void polydl_sameroot_global_untimed(const Result &section, Result &result, const std::vector<size_t> &lengths) {
    auto upper = section.begin();
    auto end = section.end();
    result.clear();

    ResultIndex first_g{0, 0};
    ResultRecordSemantics second_g{1.0, {}};
    ResultRecord cp_g{{0,   0},
                      {1.0, {}}};
    std::vector<size_t> S;
    size_t count;
    while (upper != end) {
        uint32_t currentTraceId = upper->first.first;
        first_g.first = cp_g.first.first = currentTraceId;
        size_t len = lengths.at(currentTraceId);
        cp_g.second.second.clear();
        second_g.second.clear();
        auto it = upper, lower = upper;
        S.clear();
        while ((it != end) && (it->first.first == currentTraceId)) {
            for (size_t i = 0; i<it->second.first; i++) {
                S.emplace_back(it->first.second+i);
            }
            it++;
        }
        remove_duplicates(S);
        count = 0;
        for (size_t id : S) {
            if ((id < len)) {
                count++;
            }
        }
        if (count == len) {
            populateAndReturnEvents(lower, it, second_g.second);
            result.emplace_back(first_g, second_g);
        }
        upper = it;
    }
}

inline void polydl_sameroot_global_timed(const Result &section, Result &result, const std::vector<size_t> &lengths) {
    result.clear();
    auto lower = section.begin(), upper = section.begin();
    auto end = section.end();

    ResultIndex first_g;
    ResultRecordSemantics second_g{1.0, 0.0};
    ResultRecord cp_g{{0,   0},
                      {1.0, {}}};
    std::unordered_set<event_t> S;
    event_t count, dist;

    while (upper != end) {
        uint32_t currentTraceId = upper->first.first;
        auto firstTraceEvent = upper;
        first_g.first = cp_g.first.first = currentTraceId;
        second_g.first = 1.0;
        cp_g.first.second = lengths.at(currentTraceId);
        first_g.second = 0;
        S.clear();
        lower = upper;
        upper = std::upper_bound(lower, section.end(), cp_g);
        count = dist = 0;
        Result toBeReversed;

        auto it = upper-1, itcpy = upper-1;
        event_t prev;
        while (itcpy >= firstTraceEvent) {
            first_g.second = itcpy->first.second;
            prev = cp_g.first.second - itcpy->first.second;
            for (event_t j = 0; (j<itcpy->second.first) && (itcpy->first.second+j<=(upper - 1)->first.second); j++) {
                if (!S.insert(it->first.second+j).second) {
                    break;
                } else {
                    count++;
                    itcpy--;
                }
            }

            if ((count >= prev)) {
                second_g.first = std::min(it->second.first, second_g.first);
                second_g.second.insert(second_g.second.begin(), it->second.second.begin(), it->second.second.end());
                remove_duplicates(second_g.second);
                it--;
                dist++;
            } else {
                break; // If after this the condition does not hold, then it means that in the remainder I will have
                // events that are not matching the condition
            }
            toBeReversed.emplace_back(first_g, second_g);
        }

        // Inserting the elements in reversed order
        result.insert(result.end(), std::make_move_iterator(toBeReversed.rbegin()),
                      std::make_move_iterator(toBeReversed.rend()));

        second_g.second.clear();
    }
}


/// Until: the preceeding times at least cover the time before the activated one

inline void polydl_sameroot_until_logic_untimed(const Result &aSection, const Result &bSection, Result &temp,
                                const PredicateManager *manager = nullptr, const std::vector<size_t> &lengths = {}) {
    DEBUG_ASSERT(!manager);
    auto bCurrent = bSection.begin();
    auto localBUpper = bCurrent;
    auto upper = bSection.end();

    auto aIt = aSection.begin();
    auto aEn = aSection.begin();
    auto upperA = aSection.end();

    ResultRecord cpAIt{{0, 0},
                       {0, {}}};
    ResultRecord cpLocalUpper{{0,   0},
                              {1.0, {}}};
    ResultRecord cpAEn{{0,   0},
                       {1.0, maxVec}};
    ResultRecord cpResult{{0,   0},
                          {1.0, {}}};

    env e1, e2;
    std::pair<uint32_t, uint16_t> Fut, Prev;
    temp.clear();
    std::unordered_set<event_t> S;
    size_t count;

    while (bCurrent != upper) {
        S.clear();
        uint32_t currentTraceId = localBUpper->first.first;
        cpAIt.first.first = cpLocalUpper.first.first = cpAEn.first.first = cpResult.first.first = currentTraceId;
        cpLocalUpper.first.second = lengths.at(currentTraceId);
        cpAIt.first.second = count = 0;

        localBUpper = std::upper_bound(bCurrent, upper, cpLocalUpper);
        while ((localBUpper != upper) && (localBUpper->first.first == currentTraceId))
            localBUpper++;
        aIt = std::lower_bound(aIt, upperA, cpAIt);
        aEn = aIt;

        bool atLeastOneResult = false;
        for (; bCurrent != localBUpper; bCurrent++) {
            Fut.first = bCurrent->first.first;
            if (bCurrent->first.second == 0) {
                atLeastOneResult = true;
                cpResult.second.second.insert(cpResult.second.second.end(), bCurrent->second.second.begin(),
                                              bCurrent->second.second.end());
            } else {
                cpAEn.first.second = bCurrent->first.second - 1;
                aEn = std::upper_bound(aEn, upperA, cpAEn);
                if (aIt == aEn) {
                    // Rationale: (1)
                    // if the condition does not hold for a time [startEventId, lower->first.second-1], it is because
                    // one event makes it not hold. Therefore, it should never hold even if you are extending the data
                    // that you have.
                    break;
                } else {
//                    const uint32_t dist = std::distance(aIt, aEn - 1);
                    auto tmp = aIt;
                    while (tmp != aEn) {
                        for (event_t j = 0; (j<tmp->second.first) && (tmp->first.second+j<=bCurrent->first.second); j++) {
                            if (!S.insert(tmp->first.second+j).second) {
                                break;
                            } else {
                                count++;
                            }
                        }
                        tmp++;
                    }

                    if (S.size() == ((bCurrent->first.second))) {
                        {
                            populateAndReturnEvents(aIt, aEn, cpResult.second.second);
                            cpResult.second.second.insert(cpResult.second.second.end(), bCurrent->second.second.begin(),
                                                          bCurrent->second.second.end());
                            atLeastOneResult = true;
                        }
                    } else {
                        // For (1)
                        break;
                    }
                }
            }
        }

        if (atLeastOneResult) {
            remove_duplicates(cpResult.second.second);
            temp.emplace_back(cpResult);
        }
        cpResult.second.second.clear();


        bCurrent = localBUpper;
    }
}


inline void polydl_sameroot_until_logic_timed(const Result &aSection, const Result &bSection, Result &temp,
                              const PredicateManager *manager = nullptr, const std::vector<size_t> &lengths = {}) {
    DEBUG_ASSERT(!manager);
    auto bLower = bSection.begin();
    auto bLocalUpper = bLower;
    auto bUpper = bSection.end();
    auto aLower = aSection.begin();
    auto aLocalUpper = aSection.begin();
    auto aUpper = aSection.end();
    event_t dst = 0;


    ResultRecordSemantics semein{1.0, {}};
    ResultRecordSemantics semeinMax{1.0, maxVec};
    ResultRecord cp_bLocalUpper{{0, 0}, semeinMax};
    ResultRecord cp_aLocalLower{{0, 0}, semein};
    ResultRecord cp_aLocalUpper{{0, 0}, semeinMax};
    ResultIndex first;
    //
//    auto join = marked_event::join(0, 0);
    std::unordered_set<event_t> S;
    size_t count;

    env e1, e2;
    std::pair<uint32_t, uint16_t> Fut, Prev;

    while (bLower != bUpper) {
        trace_t currentTraceId = bLower->first.first;
        cp_aLocalLower.first.first = cp_aLocalUpper.first.first
                = Fut.first
                = Prev.first
                = first.first
                = cp_bLocalUpper.first.first
                = currentTraceId;
        cp_bLocalUpper.first.second = lengths.at(bLower->first.first);

        bLocalUpper = std::upper_bound(bLocalUpper, bUpper, cp_bLocalUpper);
        event_t alpha = count = 0;
        S.clear();
        std::map<event_t, ResultRecordSemantics> globalResultBeforeShift;

        while (bLower != bLocalUpper) {
            event_t t = bLower->first.second;
            if (t == alpha) {
                // TODO: add event in results
                if (globalResultBeforeShift.empty()) {
                    // If there were not previous events, directly insert this into temp
                    temp.emplace_back(*bLower);
                } else {
                    // Otherwise, the last element of the collection should be the current event:
                    // add the current data to it, and then shift it to the result.
                    auto& ref = globalResultBeforeShift.emplace(t, semein).first->second;
                    ref.first = std::max(ref.first, bLower->second.first);
                    ref.second.insert(ref.second.end(), bLower->second.second.begin(), bLower->second.second.end());
                    remove_duplicates(ref.second);
                }
                bLower++; /// Increment Upper
            } else {
                // Scanning the "a"-s
                cp_aLocalLower.first.second = alpha;
                aLower = std::lower_bound(aLower, aUpper, cp_aLocalLower);
                cp_aLocalUpper.first.second = std::max((event_t)(bLower->first.second - 1), (event_t)cp_aLocalLower.first.second);
                aLocalUpper = std::upper_bound(aLower, aUpper, cp_aLocalUpper);
                S.clear();
                dst = S.size();

                // If there is a sequence of As preceeding the Bs
                if ((aLower != aUpper) && (aLower->first.first == currentTraceId)) {
                    auto tmp = aLower;
                    while ((tmp != aUpper) && (tmp->first.first == currentTraceId) && (tmp->first.second< (bLower)->first.second)) {
                        for (event_t j = 0; (j<tmp->second.first) && (tmp->first.second+j<=bLower->first.second); j++) {
                            if (S.insert(tmp->first.second+j).second) {
                                dst++;
                            } else {
                                break;
                            }
                        }
                        tmp++;
                    }
//                    dst = std::distance(aLower, aLocalUpper) +
//                          1; // distance between the beginning of the sequence of the As and the bLower
                    count = ((bLower)->first.second - alpha) ;

                    // if there exists a contiguous sequence sequence A_alpha ... A_{t-1}
                    if (dst == count) {
                        {
                            for (auto curr = aLower; curr != aLocalUpper; curr++) {
                                auto& ref2 = globalResultBeforeShift.emplace(curr->first.second, semein).first->second;
                                ref2.first = std::min(ref2.first, curr->second.first);
                                ref2.second.insert(ref2.second.end(),
                                                   bLower->second.second.begin(),
                                                   bLower->second.second.end());
                                ref2.second.insert(ref2.second.end(),
                                                   curr->second.second.begin(),
                                                   curr->second.second.end());
                                populateAndReturnEvents(curr, aLocalUpper, ref2.second);
                            }
                        }
                        first.second = bLower->first.second;
                        auto &ref = globalResultBeforeShift.emplace(first.second, semein).first->second;
                        ref.first = std::max(ref.first, bLower->second.first);
                        ref.second.insert(ref.second.end(), bLower->second.second.begin(), bLower->second.second.end());
                        bLower++;
                    } else if ((aLower->first.first == currentTraceId) && (aLower->first.second < t)) {
                        if (alpha == aLower->first.second)
                            alpha++;
                        else
                            alpha = aLower->first.second;
                    } else {
                        alpha = t;
                    }
                } else {
                    // Otherwise, if there is no sequence, I will need to add the current event
                    alpha = t;
                }
            }
        }

        for (auto it = globalResultBeforeShift.begin(), en = globalResultBeforeShift.end(); it!= en; it++) {
            first.second = it->first;
            remove_duplicates(it->second.second);
            temp.emplace_back(first, it->second);
        }
        globalResultBeforeShift.clear();
    }
}