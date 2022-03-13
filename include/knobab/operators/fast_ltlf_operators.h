//
// Created by giacomo on 11/03/2022.
//
#include <knobab/operators/simple_ltlf_operators.h>

#ifndef KNOBAB_FAST_LTLF_OPERATORS_H
#define KNOBAB_FAST_LTLF_OPERATORS_H

/**
 * @author Samuel 'Sam' Appleby, Giacomo Bergami
 *
 * @param lhs
 * @param rhs
 * @param out
 * @param manager
 */
inline void or_fast_timed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    or_logic_timed(lhs, rhs, out, manager, lengths);
}

/**
 * This implementation is faster than the one provided in the logical implementation, as we don't group by trace id,
 * but we directly do the loop on all the events of the trace
 *
 * @author Giacomo Bergami
 *
 * @param lhs
 * @param rhs
 * @param out
 * @param manager
 */
inline void or_fast_untimed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    auto first1 = lhs.begin(), first2 = rhs.begin(),
            last1 = lhs.end(), last2 = rhs.end();
    env e1, e2;
    ResultRecord result{{0, 0}, {0.0, {}}};
    ResultRecord idx{{0, 0}, {0.0, {}}};
    ResultIndex pair, pair1;
    bool hasMatch, completeInsertionRight;
    auto join = marked_event::join(0, 0);
    while (first1 != last1) {
        if (first2 == last2) {
            std::copy(first1, last1, std::back_inserter(out));
            return;
        }
        if (first1->first.first > first2->first.first) {
            out.emplace_back(*first2++);
        } else if (first1->first.first < first2->first.first) {
            out.emplace_back(*first1++);
        } else {
            auto localTrace = first1->first.first;
            idx.first.first = first1->first.first+1; // pointing towards the next trace
            auto endFirst1 = std::upper_bound(first1, last1, idx);
            pair.first = pair1.first = localTrace;
            result.first = first1->first;
            result.second.first = std::min(first1->second.first, first2->second.first);
            result.second.second.clear();
            hasMatch = completeInsertionRight = false;
            decltype(endFirst1) endFirst2;
            for (; first1 != endFirst1; first1++) {
                auto dx = first2;
                do {

                    if (manager) {
                        for (const marked_event &elem: first1->second.second) {
                            if (!IS_MARKED_EVENT_ACTIVATION(elem)) continue;
                            pair.second = GET_ACTIVATION_EVENT(elem);

                            for (const marked_event &elem1: first2->second.second) {
                                if (!IS_MARKED_EVENT_TARGET(elem1)) continue;
                                pair1.second = GET_TARGET_EVENT(elem1);

                                e1 = manager->GetPayloadDataFromEvent(pair);
                                e2 = manager->GetPayloadDataFromEvent(pair1);
                                if (manager->checkValidity(e1, e2)) {
                                    hasMatch = true;
                                    join.id.parts.left = pair.second;
                                    join.id.parts.right = pair1.second;
                                    result.second.second.emplace_back(join);
                                }
                            }
                        }
                    } else {
                        hasMatch = true;
                        if (!completeInsertionRight) {
                            result.second.second.insert(result.second.second.end(), dx->second.second.begin(), dx->second.second.end());
                        }
                    }

                    dx++;
                } while ((dx != last2) && (dx->first.first == localTrace));
                endFirst2 = dx;

                if (!manager) {
                    result.second.second.insert(result.second.second.end(), first1->second.second.begin(), first1->second.second.end());
                    completeInsertionRight = true;
                }
            }

            if (hasMatch) {
                remove_duplicates(result.second.second);
                out.emplace_back(result);
            }
            first2 = endFirst2;
        }
    }
    std::copy(first2, last2, std::back_inserter(out));
}

/**
 * @author Samuel 'Sam' Appleby, Giacomo Bergami
 *
 * @param lhs
 * @param rhs
 * @param out
 * @param manager
 */
inline void and_fast_timed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    and_logic_timed(lhs, rhs, out, manager, lengths);
}

/**
 * This implementation is faster than the one provided in the logical implementation, as we don't group by trace id,
 * but we directly do the loop on all the events of the trace
 * @author Giacomo Bergami
 *
 * @param lhs
 * @param rhs
 * @param out
 * @param manager
 */
inline void and_fast_untimed(const Result& lhs, const Result& rhs, Result& out, const PredicateManager *manager = nullptr, const std::vector<size_t>& lengths = {}) {
    auto first1 = lhs.begin(), first2 = rhs.begin(),
            last1 = lhs.end(), last2 = rhs.end();
    env e1, e2;
    ResultRecord result{{0, 0}, {0.0, {}}};
    ResultRecord idx{{0, 0}, {0.0, {}}};
    ResultIndex pair, pair1;
    auto join = marked_event::join(0, 0);
    bool hasMatch, completeInsertionRight;
    while (first1 != last1) {
        if (first2 == last2) {
            return;
        }
        if (first1->first.first > first2->first.first) {
            first2++;
        } else if (first1->first.first < first2->first.first) {
            first1++;
        } else {
            auto localTrace = first1->first.first;
            idx.first.first = first1->first.first+1; // pointing towards the next trace
            auto endFirst1 = std::upper_bound(first1, last1, idx);
            decltype(endFirst1) endFirst2;
            pair.first = pair1.first = localTrace;
            result.first = first1->first;
            result.second.first = std::min(first1->second.first, first2->second.first);
            result.second.second.clear();
            hasMatch = completeInsertionRight = false;
            for (; first1 != endFirst1; first1++) {
                auto dx = first2;
                do {

                    if (manager) {
                        for (const marked_event &elem: first1->second.second) {
                            if (!IS_MARKED_EVENT_ACTIVATION(elem)) continue;
                            pair.second = GET_ACTIVATION_EVENT(elem);

                            for (const marked_event &elem1: first2->second.second) {
                                if (!IS_MARKED_EVENT_TARGET(elem1)) continue;
                                pair1.second = GET_TARGET_EVENT(elem1);

                                e1 = manager->GetPayloadDataFromEvent(pair);
                                e2 = manager->GetPayloadDataFromEvent(pair1);
                                if (manager->checkValidity(e1, e2)) {
                                    hasMatch = true;
                                    join.id.parts.left = pair.second;
                                    join.id.parts.right = pair1.second;
                                    result.second.second.emplace_back(join);
                                }
                            }
                        }
                    } else {
                        hasMatch = true;
                        if (!completeInsertionRight) {
                            result.second.second.insert(result.second.second.end(), dx->second.second.begin(), dx->second.second.end());
                        }
                    }

                    dx++;
                } while ((dx != last2) && (dx->first.first == localTrace));

                endFirst2 = dx;
                if (!manager){
                    completeInsertionRight = true;
                    result.second.second.insert(result.second.second.end(), first1->second.second.begin(), first1->second.second.end());
                }
            }

            if (hasMatch) {
                remove_duplicates(result.second.second);
                out.emplace_back(result);
            }

            first2 = endFirst2;
        }
    }
}

inline void next_fast(const Result &section, Result& temp, const std::vector<size_t>& lengths = {}) {
    next_logical(section, temp, lengths);
}

/**
 *
 * @author Samuel 'Sam' Appleby
 *
 * @param section
 * @param lengths
 * @param result
 */
inline void future_fast_timed(const Result &section, Result& result, const std::vector<size_t>& lengths) {
    result = section;
}

/**
 *
 * @author Samuel 'Sam' Appleby
 *
 * @param section
 * @param lengths
 * @param result
 */
inline void future_fast_untimed(const Result &section, Result& result, const std::vector<size_t>& lengths) {
    result = section;
}


inline void global_fast_timed(const Result &section, Result& result, const std::vector<size_t>& lengths) {
    global_logic_timed(section, result, lengths);
}

inline void global_fast_untimed(const Result &section, Result& result, const std::vector<size_t>& lengths) {
    global_logic_untimed(section, result, lengths);
}

inline void negated_fast_untimed(const Result &section, Result& result, const std::vector<size_t>& lengths) {
    size_t first1 = 0, last1 = lengths.size();
    ResultRecord rc{{0, 0}, {1.0, {}}};
    auto first2 = section.begin(), last2 = section.end();
    for (; first1 != last1; ) {
        if (first2 == last2) {
            while (first1 != last1) {
                rc.first.first = first1++;
                result.emplace_back(rc);
            }
        }
        if (first1 > first2->first.first) {
            first2++;
        } else if (first1 < first2->first.first) {
            rc.first.first = first1++;
            result.emplace_back(rc);
        } else {
            first1++;
            first2++;
        }
    }
}

inline void negated_fast_timed(const Result &section, Result& result, const std::vector<size_t>& lengths) {
    ResultIndex last1{lengths.size(), 0};
    ResultRecord rc{{0, 0}, {1.0, {}}};
    auto first2 = section.begin(), last2 = section.end();
    for (; rc.first != last1; ) {
        if (first2 == last2) {
            while (rc.first != last1) {
                result.emplace_back(rc);
                rc.first.second++;
                if (rc.first.second >= lengths.at(rc.first.first)) {
                    rc.first.first++;
                    rc.first.second = 0;
                }
            }
        }
        if (rc.first > first2->first) {
            first2++;
        } else if (rc.first < first2->first) {
            result.emplace_back(rc);
            rc.first.second++;
            if (rc.first.second >= lengths.at(rc.first.first)) {
                rc.first.first++;
                rc.first.second = 0;
            }
        } else {
            rc.first.second++;
            if (rc.first.second >= lengths.at(rc.first.first)) {
                rc.first.first++;
                rc.first.second = 0;
            }
            first2++;
        }
    }
}

#endif //KNOBAB_FAST_LTLF_OPERATORS_H
