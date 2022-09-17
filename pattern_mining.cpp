//
// Created by giacomo on 17/09/22.
//

#include "pattern_mining.h"

/** Pattern mining **/
std::vector<pattern_mining_result<DeclareDataAware>> pattern_mining(const KnowledgeBase& kb,
                                                                    double support,
                                                                    bool naif,
                                                                    bool init_end) {
    support = std::max(std::min(support, 1.0), 0.0); // forcing the value to be between 0 and 1.
    size_t log_size = kb.nTraces();
    const auto& count_table = kb.getCountTable();
    uint64_t minimum_support_threshold = std::min((uint64_t)std::ceil((double)log_size * support), log_size);
    uint64_t max_act_id = kb.nAct();
    FPTree t{count_table, minimum_support_threshold, max_act_id};
    std::vector<pattern_mining_result<DeclareDataAware>> declarative_clauses;
    bool doInitA = false;
    auto result = fptree_growth(t, 2);
    std::set<Pattern> binary_patterns;
    std::unordered_set<act_t> unary_patterns_for_non_exact_support;
    for (const auto& x : result) {
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
    result.clear();

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
    for (const Pattern& pattern : binary_patterns) {
        std::vector<pattern_mining_result<Rule<act_t>>> candidate_rule;
        DEBUG_ASSERT(pattern.first.size() == 2);
        Rule<act_t> lr, rl;
        auto it = pattern.first.begin();
        lr.head.emplace_back(*it); rl.tail.emplace_back(*it);
        it++;
        lr.tail.emplace_back(*it); rl.head.emplace_back(*it);
        double lr_conf = counter.confidence(lr);
        double rl_conf = counter.confidence(rl);
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
            if (result.clause.tail.empty()) {
                // CoExistence pattern
                DeclareDataAware clause;
                clause.left_act = kb.event_label_mapper.get(result.clause.head.at(0));
                clause.right_act = kb.event_label_mapper.get(result.clause.head.at(1));
                clause.n = 1;
                clause.casusu = "CoExistence";
                declarative_clauses.emplace_back(clause,
                                                 result.support_generating_original_pattern,
                                                 result.support_declarative_pattern,
                                                 result.confidence_declarative_pattern);
            } else if (result.clause.tail.size() == 1) {
                // Doing some further finicky processing to refine which kind of implication
                bool canBeRefined = false;
                if (canBeRefined) {
                    // the result that was computed before
                } else {
                    DeclareDataAware clause;
                    clause.left_act = kb.event_label_mapper.get(result.clause.head.at(0));
                    clause.right_act = kb.event_label_mapper.get(result.clause.tail.at(0));
                    clause.n = 1;
                    clause.casusu = "RespExistence";
                    declarative_clauses.emplace_back(clause,
                                                     result.support_generating_original_pattern,
                                                     result.support_declarative_pattern,
                                                     result.confidence_declarative_pattern);
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

