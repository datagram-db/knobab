//
// Created by giacomo on 17/09/22.
//

#include "pattern_mining.h"

// Todo:
inline void globallyA_And_FutureB(const std::pair<ActTable::record*, ActTable::record*>& left,
                              const std::pair<ActTable::record*, ActTable::record*>& right,
                              std::vector<ResultIndex>& result) {
//    if (right.first == right.second) {
//        result.clear();
//        return;
//    }
    auto bCurrent = right.first, bEnd = right.second;
    ResultIndex rcx;
    bool hasMatch;

    for (auto aCurrent = left.first, aEnd = left.second; aCurrent != aEnd; ) {
        if ((aCurrent->entry.id.parts.trace_id > bCurrent->entry.id.parts.trace_id) ||
                ((aCurrent->entry.id.parts.trace_id == bCurrent->entry.id.parts.trace_id) &&
                        (aCurrent->entry.id.parts.event_id > bCurrent->entry.id.parts.event_id))) {
            bCurrent++;
            if (bCurrent == bEnd) break;
        } else {
            auto newItr = bCurrent;
            rcx.first = aCurrent->entry.id.parts.trace_id;
            rcx.second = aCurrent->entry.id.parts.event_id;
            hasMatch = false;

            while (newItr != bEnd) {
                if(newItr->entry.id.parts.trace_id != aCurrent->entry.id.parts.trace_id){
                    break;
                }
                if (!hasMatch) {
                    result.emplace_back(rcx);
                    hasMatch = true;
                }
                newItr++;
            }

            if ((aCurrent->entry.id.parts.trace_id == bCurrent->entry.id.parts.trace_id) &&
                (aCurrent->entry.id.parts.event_id == bCurrent->entry.id.parts.event_id)) {
                bCurrent++;
                if (bCurrent == bEnd) break;
            }

            aCurrent++;
        }
    }
}

void decrease_support_X(const KnowledgeBase &kb,
                        size_t expected_support,
                        bool& alles_X,
                        size_t& alles_not_X) {
    if (alles_X) {
        alles_not_X++;
        if ((kb.nTraces() - alles_not_X) < expected_support) {
            alles_X = false;
        }
    }
}

void fast_forward_equals(trace_t trace_id,
                         ActTable::record*& to_increment,
                         ActTable::record *&end) {
    do {
        to_increment++;
    } while ((to_increment != end) &&
           (to_increment->entry.id.parts.trace_id == trace_id));
}

void fast_forward_lower(trace_t trace_id,
                         ActTable::record*& to_increment,
                         ActTable::record *&end) {
    do {
        to_increment++;
    } while ((to_increment != end) &&
             (to_increment->entry.id.parts.trace_id < trace_id));
}

/** Pattern mining **/
std::vector<pattern_mining_result<DeclareDataAware>> pattern_mining(const KnowledgeBase& kb,
                                                                    double support,
                                                                    bool naif,
                                                                    bool init_end,
                                                                    bool special_temporal_patterns,
                                                                    bool only_precise_temporal_patterns) {

    std::unordered_map<std::string, std::unordered_map<act_t, std::vector<pattern_mining_result<std::string>>>> patterns_to_negate;
    support = std::max(std::min(support, 1.0), 0.0); // forcing the value to be between 0 and 1.
    size_t log_size = kb.nTraces();
    const auto& count_table = kb.getCountTable();
    uint64_t minimum_support_threshold = std::min((uint64_t)std::ceil((double)log_size * support), log_size);
    uint64_t max_act_id = kb.nAct();
    FPTree t{count_table, minimum_support_threshold, max_act_id};
    std::vector<pattern_mining_result<DeclareDataAware>> declarative_clauses;
    bool doInitA = false;
    auto fpt_result = fptree_growth(t, 2);
    std::set<Pattern> binary_patterns;
    std::unordered_set<act_t> unary_patterns_for_non_exact_support;
    for (const auto& x : fpt_result) {
        if (x.first.size() == 1) {
            if (support == 1.0) {
                // If the support is actually one, then we can infer that if an event
                // exists in all of the events, that should always happen
                if (naif) {
                    // The naif algorithm will just state that, if the pattern exists,
                    // then it exists at least once
                    DeclareDataAware clause;
                    clause.left_act = kb.event_label_mapper.get(*x.first.begin());
                    clause.n = 1;
                    clause.casusu = "Exists1";
                    declarative_clauses.emplace_back(support, clause);
                } else {
                    // The non-naif version is exploiting the couting information from
                    // the counting table, and also providing an expected number of times
                    // the event should happen. You can also apply the same reasoning
                    // for mining the absence pattern
                    DeclareDataAware clause;
                    clause.left_act = kb.event_label_mapper.get(*x.first.begin());
                    event_t n = std::numeric_limits<event_t>::max(),
                            N = 0;
                    auto cp = count_table.resolve_primary_index2(*x.first.begin());
                    while (cp.first != cp.second) {
                        if (cp.first->id.parts.event_id > 0) {
                            if (cp.first->id.parts.event_id < n) {
                                n = cp.first->id.parts.event_id;
                            }
                            if (cp.first->id.parts.event_id > N) {
                                N = cp.first->id.parts.event_id;
                            }
                        }
                        cp.first++;
                    }
                    clause.n = n;
                    clause.casusu = "Exists";
                    declarative_clauses.emplace_back(support, clause);
                    clause.n = N+1;
                    clause.casusu = "Absence";
                    declarative_clauses.emplace_back(support, clause);
                }
            } else {
                // If the support is less than one, then we cannot state that
                // all of these events actually exist in all of the possible traces, and
                // we need to relax such conditions into a choice between the patterns,
                // as this will maximise the score, precision-wise.
                // We are postponing such discussion into point A)
                unary_patterns_for_non_exact_support.insert(*x.first.begin());
                doInitA = true;
            }
        } else {
            binary_patterns.insert(x);
        }
    }
    fpt_result.clear();

    // Point A): initialisation.
    // Please observe that, given the support definition for traditional
    // event mining, I can extract a Choice pattern only when the support is
    // less than one, otherwise this choice can be rewritten simply as an exists
    std::vector<std::vector<act_t>> map;
    std::vector<std::vector<trace_t>> inv_map;
    std::set<std::vector<act_t>> S;
    if (doInitA) {
        map.insert(map.begin(), (log_size), std::vector<act_t>{});
        inv_map.insert(inv_map.begin(), max_act_id, std::vector<trace_t>{});
        for (const act_t& act_id : unary_patterns_for_non_exact_support) {
            for (size_t trace_id = 0; trace_id < log_size; trace_id++) {
                event_t count = count_table.resolve_length(act_id, trace_id);
                if (count > 0) {
                    map[trace_id].emplace_back(act_id);
                    inv_map[act_id].emplace_back(trace_id);
                }
            }
        }
    }
    unary_patterns_for_non_exact_support.clear();

    for (auto& v : map) {
        std::sort(v.begin(), v.end());
    }
    for (auto& v : inv_map) {
        std::sort(v.begin(), v.end());
    }
    std::pair<act_t, act_t> curr_pair, inv_pair;
    std::unordered_set<std::pair<act_t, act_t>> visited_pairs;
    // Point A)
    for (const auto& v : map)
        S.insert(v);
    for (const auto& x : S) {
        for (const auto& y : S) {
            if (x != y) {
                for (const auto& a : x) {
                    curr_pair.first = inv_pair.second = a;
                    for (const auto& b : y) {
                        if (b == a) continue;
                        curr_pair.second = inv_pair.first = b;
                        if ((!visited_pairs.emplace(curr_pair).second) ||
                            (!visited_pairs.emplace(inv_pair).second)) continue;
                        const auto& aSet = inv_map.at(a);
                        const auto& bSet = inv_map.at(b);
                        std::pair<size_t, size_t> ratio = yaucl::iterators::ratio(aSet.begin(), aSet.end(), bSet.begin(), bSet.end());
                        double local_support = ((double)(ratio.first)) / ((double)log_size);
                        if (ratio.first >= minimum_support_threshold) {
                            // I can consider this pattern, again, only if it is within the expected
                            // support rate which, in this case, is given by the amount of traces
                            // globally setting up this pattern
                            DeclareDataAware clause;
                            clause.left_act = kb.event_label_mapper.get(a);
                            clause.right_act = kb.event_label_mapper.get(b);
                            clause.n = 1;
                            clause.casusu = "Choice";
                            declarative_clauses.emplace_back(clause, support, local_support, -1);
                            if ((!naif) && ratio.second == 0) {
                                // If there is no intersection, I can also be more strict if I want,
                                // and provide an exclusive choice pattern if I am confident that
                                // the two events will never appear in the same trace (according to
                                // the "training" data
                                clause.casusu = "ExclChoice";
                                declarative_clauses.emplace_back(clause, support, local_support, -1);
                            }
                        }
                    }
                }
            }
        }
    }
    map.clear();
    inv_map.clear();

    DataMiningMetrics counter{count_table};
    std::cout << "Pattern generation: " << std::endl;
    for (const Pattern& pattern : binary_patterns) {
        std::vector<pattern_mining_result<Rule<act_t>>> candidate_rule;

        DEBUG_ASSERT(pattern.first.size() == 2);
        Rule<act_t> lr, rl;
        auto it = pattern.first.begin();
        lr.head.emplace_back(*it); rl.tail.emplace_back(*it);
        std::cout << " - Pattern: " << kb.event_label_mapper.get(*it) << ",";
        it++;
        std::cout <<kb.event_label_mapper.get(*it) << std::endl << std::endl;
        lr.tail.emplace_back(*it); rl.head.emplace_back(*it);
        double lr_conf = counter.confidence(lr);
        double rl_conf = counter.confidence(rl);
        std::cout << "   conf: " << counter.lift(lr) << " conf: " << counter.lift(rl) << std::endl;
        std::cout << "   lift: " << counter.lift(lr) << " lift: " << counter.lift(rl) << std::endl;
        if ((lr_conf == rl_conf) && (rl_conf >= support)) {
            candidate_rule.emplace_back(Rule<act_t>{pattern.first}, ((double)pattern.second)/((double)log_size), -1, -1);
        } else {
            if (lr_conf >= rl_conf) {
                if (lr_conf >= support)
                    candidate_rule.emplace_back(lr, ((double)pattern.second)/((double)log_size), counter.support(lr), lr_conf);
            } else if (rl_conf >= support) {
                if (rl_conf >= support)
                    candidate_rule.emplace_back(rl, ((double)pattern.second)/((double)log_size), counter.support(lr), rl_conf);
            }
        }

        for (const auto& result: candidate_rule) {            // Generate the hypotheses containing a lift greater than one
            act_t A;
            act_t B;
            DeclareDataAware clause;
            clause.n = 1;
            if (result.clause.tail.empty()) {
                // CoExistence pattern
                A = result.clause.head.at(0);
                B = result.clause.head.at(1);
                clause.casusu = "CoExistence";
            } else if (result.clause.tail.size() == 1) {
                A = result.clause.head.at(0);
                B = result.clause.tail.at(0);
                // <>A -> <>B
                clause.casusu = "RespExistence";
            };
            clause.left_act = kb.event_label_mapper.get(A);
            clause.right_act = kb.event_label_mapper.get(B);
            declarative_clauses.emplace_back(clause,
                                             result.support_generating_original_pattern,
                                             result.support_declarative_pattern,
                                             result.confidence_declarative_pattern);

            if (special_temporal_patterns) {
                size_t expected_support = only_precise_temporal_patterns ?
                                          kb.nTraces() :
                                          minimum_support_threshold;



                // 1) First kind of specialized patterns:
                // events happening immediately previously and next:
                // This might leverage the prev/next pointers!
                size_t alles_not_nexte = 0;
                bool alles_nexte = true;
                for (auto sigma = 0; sigma < kb.nTraces(); sigma++) {
                    auto lA = count_table.resolve_length(A, sigma);
                    if (lA > 0) {
                        if (lA != count_table.resolve_length(B, sigma)) {
                            alles_not_nexte++;
                            if ((kb.nTraces() - alles_not_nexte) < expected_support) {
                                alles_nexte = false;
                                break; // skipping if this is going out of hand
                            }
                        }
                    }
                }
                // Only if the heuristic was activated (for next-based conditions
                // to happen I need to have As and Bs in the same number. Otherwise,
                // this is unlikely to happen, and I do not ever start performing the
                // search.
                auto ll = kb.nTraces();
                std::vector<bool> isTraceVisitedU(kb.nTraces(), false);
                if (alles_nexte) {
                    size_t alles_not_prev = 0,
                            alles_not_next = 0;
                    bool alles_prev = true, alles_next = true;
                    // This is still computed, as it is required for both 1) and 2)
                    auto a_beginend = kb.timed_dataless_exists(A);
                    DEBUG_ASSERT(a_beginend.first != a_beginend.second);
                    while (a_beginend.first != a_beginend.second) {
                        if (!isTraceVisitedU.at(a_beginend.first->entry.id.parts.trace_id)) {
                            if (alles_prev && (a_beginend.first->prev == nullptr ||
                                               (a_beginend.first->prev->entry.id.parts.act != B))) {
                                alles_not_prev++;
                                if ((kb.nTraces() - alles_not_prev) < expected_support) {
                                    alles_prev = false;
                                }
                            }
                            if (alles_next && (a_beginend.first->next == nullptr ||
                                               (a_beginend.first->next->entry.id.parts.act != B))) {
                                alles_not_next++;
                                if ((kb.nTraces() - alles_not_next) < expected_support) {
                                    alles_next = false;
                                }
                            }
                            // TODO: chainsuccession is just attempting to generate B from As'next, and then
                            //       comparing this attempted reconstruction to the actual results in B
                            isTraceVisitedU[a_beginend.first->entry.id.parts.trace_id] = true;
                        }
                        if ((!alles_next) && (!alles_prev)) {
                            break; // Breaking only if both conditions are never met
                        }
                        a_beginend.first++;
                    }
                    if (alles_prev) {
                        clause.casusu = "ChainPrecedence";
                        declarative_clauses.emplace_back(clause,
                                                         result.support_generating_original_pattern,
                                                         (((double) (kb.nTraces() - alles_not_prev)) /
                                                          ((double) kb.nTraces())),
                                                         -1);
                    }
                    if (alles_next) {
                        clause.casusu = "ChainResponse";
                        declarative_clauses.emplace_back(clause,
                                                         result.support_generating_original_pattern,
                                                         (((double) (kb.nTraces() - alles_not_next)) /
                                                          ((double) kb.nTraces())),
                                                         -1);
                    }

                    // re-setting the vector to all falses!
                    std::fill(isTraceVisitedU.begin(), isTraceVisitedU.end(), false);
                }

                // 2) Second kind of patters, always starting scanning from the
                // activation condition (that is also the premise of the rule).
                // This is still computed, as it is required for both 1) and 2)
                auto a_beginend = kb.timed_dataless_exists(A);
                DEBUG_ASSERT(a_beginend.first != a_beginend.second);

                auto b_beginend = kb.timed_dataless_exists(B);
                // As I obtained the rule, there should be some data pertaining to it!
                DEBUG_ASSERT(b_beginend.first != b_beginend.second);

                auto alles_precedence = true;
                size_t alles_not_precedence = 0;
                auto alles_response = true;
                size_t alles_not_response = 0;
                auto alles_altresponse = true;
                size_t alles_not_altresponse = 0;

                while (a_beginend.first != a_beginend.second) {
                    if ((!alles_precedence) && (!alles_response) && (!alles_altresponse))
                        break;
                    auto trace_id = a_beginend.first->entry.id.parts.trace_id;
                    DEBUG_ASSERT(!isTraceVisitedU.at(trace_id));
                    auto trace_id_visited = isTraceVisitedU.at(trace_id);

                    if (b_beginend.first == b_beginend.second) {
                        // Problem 1)
                        // This might be a valid precedence, as nothing is stated
                        // to what should happen after the A, but I cannot exploit
                        // for a response: therefore, I'm counting it
                        decrease_support_X(kb, expected_support, alles_response, alles_not_response);

                        // Now, skipping to the next trace, as there is no more information for as
                        fast_forward_equals(trace_id, a_beginend.first, a_beginend.second);
                        isTraceVisitedU[trace_id] = true;
                        continue;
                    } else {
                        // Otherwise, I have something to check related to B!
                        if (a_beginend.first->entry.id.parts.trace_id >
                            b_beginend.first->entry.id.parts.trace_id) {

                            // Moving b until I find something related to b. A is kept fixed and not incremented
                            fast_forward_lower(trace_id, b_beginend.first, b_beginend.second);
                            // Not setting the current trace to be visited, as we need to fast-forward B first
                            continue;
                        } else if (a_beginend.first->entry.id.parts.trace_id <
                                   b_beginend.first->entry.id.parts.trace_id) {

                            // If I am not able to find a B, then this is detrimental to A's response
                            // (Problem 1)
                            decrease_support_X(kb, expected_support, alles_response, alles_not_response);

                            // Now, skipping to the next trace, as there is no more information for as
                            fast_forward_equals(trace_id, a_beginend.first, a_beginend.second);
                            continue;
                        } else {
                            // Please remember, we are not visiting traces, rather than
                            // events associated to traces. Therefore, it is of the
                            // uttermost importance to remember conditions related to
                            // the trace level, and to remember whether this was the
                            // first time the trace was visited or not.

                            // Problem 2)
                            // If B happens before the event A, this cannot be referred
                            // to the precedence, and therefore this should be decreased
                            // Still, this consideration should be performed only up until
                            // the first event is visited
                            if (b_beginend.first->entry.id.parts.event_id <
                                a_beginend.first->entry.id.parts.event_id) {
                                decrease_support_X(kb, expected_support, alles_precedence, alles_not_precedence);
                            }

                            // While I'm scanning the A events within the same trace
                            bool all_response_in_trace = true,
                                    all_altresponse_in_trace = true;

                            while ((a_beginend.first != a_beginend.second) &&
                                   (a_beginend.first->entry.id.parts.trace_id == trace_id)) {
                                // ignoring all of the B events that are not relevant for the task!
                                while ((b_beginend.first != b_beginend.second) &&
                                       (b_beginend.first->entry.id.parts.trace_id == trace_id) &&
                                       (b_beginend.first->entry.id.parts.event_id <
                                        a_beginend.first->entry.id.parts.event_id)) {
                                    b_beginend.first++;
                                }
                                if ((b_beginend.first != b_beginend.second) &&
                                    (b_beginend.first->entry.id.parts.trace_id == trace_id) &&
                                    (b_beginend.first->entry.id.parts.event_id >=
                                     a_beginend.first->entry.id.parts.event_id)) {
                                    // Ok, I have a match!
                                } else {
                                    // If there is no match for the B event, then I'm setting this to false
                                    // and quitting the iteration
                                    all_response_in_trace = false;
                                    break;
                                }

                                {
                                    auto tmp = a_beginend.first++;
                                    if ((tmp != a_beginend.second) &&
                                        (tmp->entry.id.parts.event_id <
                                         b_beginend.first->entry.id.parts.event_id)) {
                                        all_altresponse_in_trace = false;
                                    }
                                }

//                                a_beginend.first++;
                            }
                            if (!all_response_in_trace) {
                                decrease_support_X(kb, expected_support, alles_response, alles_not_response);
                            }
                            if (!all_altresponse_in_trace) {
                                decrease_support_X(kb, expected_support, alles_altresponse, alles_not_altresponse);
                            }
                        }
                    }

                    isTraceVisitedU[trace_id] = true;
                }

                if (alles_precedence) {
                    clause.casusu = "Precedence";
                    declarative_clauses.emplace_back(clause,
                                                     result.support_generating_original_pattern,
                                                     (((double) (kb.nTraces() - alles_not_precedence)) /
                                                      ((double) kb.nTraces())),
                                                     -1);
                }
                if (alles_response) {
                    clause.casusu = "Response";
                    declarative_clauses.emplace_back(clause,
                                                     result.support_generating_original_pattern,
                                                     (((double) (kb.nTraces() - alles_not_response)) /
                                                      ((double) kb.nTraces())),
                                                     -1);
                }
            }
        }
    }


    if (init_end) {
        // This is not directly mined via the frequent mining algorithm but,
        // still, if needed, this can be obtained via an easy linear scan of the
        // secondary index of the knowledge base
        std::vector<size_t> first(max_act_id, 0), last(max_act_id, 0);
        for (size_t trace_id = 0; trace_id < log_size; trace_id++) {
            auto first_last = kb.act_table_by_act_id.secondary_index.at(trace_id);
            first[first_last.first->entry.id.parts.act]++;
            last[first_last.second->entry.id.parts.act]++;
        }
        for (size_t act_id = 0; act_id < max_act_id; act_id++) {
            DeclareDataAware clause;
            clause.n = 1;
            auto first_occ = first.at(act_id), last_occ = last.at(act_id);
            if (first_occ > minimum_support_threshold) {
                clause.casusu = "Init";
                clause.left_act = kb.event_label_mapper.get(act_id);
                declarative_clauses.emplace_back(((double)(first_occ)) / ((double)log_size), clause);
            }
            if (last_occ > minimum_support_threshold) {
                clause.casusu = "End";
                clause.left_act = kb.event_label_mapper.get(act_id);
                declarative_clauses.emplace_back(((double)(last_occ)) / ((double)log_size), clause);
            }
        }

    }
    return declarative_clauses;
}


std::vector<pattern_mining_result<DeclareDataAware>> classifier_mining(const KnowledgeBase& pos,
                                                                       const KnowledgeBase& neg,
                                                                       double support,
                                                                       bool naif,
                                                                       bool init_end,
                                                                       bool special_temporal_patterns,
                                                                       bool only_precise_temporal_patterns) {
//    auto P = pattern_mining(pos, support, naif, init_end, special_temporal_patterns, only_precise_temporal_patterns);
//    auto N = pattern_mining(neg, support, naif, init_end, special_temporal_patterns, only_precise_temporal_patterns);
}