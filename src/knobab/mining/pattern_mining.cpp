//
// Created by giacomo on 17/09/22.
//

#include "knobab/algorithms/mining/pattern_mining.h"
#include "yaucl/learning/dt_predicate.h"
#include "yaucl/learning/DecisionTree.h"
#include <chrono>
#include <knobab/server/query_manager/Environment.h>

void bolt_algorithm(const std::string& logger_file,
                    const FeedQueryLoadFromFile& conf,
                    double support,
                    uint16_t iter_num,
                    bool no_stats) {
    Environment env;
    env.doStats = true;
    env.experiment_logger.min_support = support;
    env.experiment_logger.mining_algorithm = "BOLT";
    env.experiment_logger.iteration_num = iter_num;
    auto scripts = std::filesystem::current_path();
    std::filesystem::path file{conf.file};
    {
        std::ifstream if_{file};
        env.load_log(conf.format, conf.with_data, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    std::cout << "Starting from now!" << std::endl;
    std::pair<std::vector<pattern_mining_result<DeclareDataAware>>, double> list = pattern_mining(env.db, support, false, true, true, false, false);

//    bool filePreexists1 = std::filesystem::exists("/home/sam/Documents/Repositories/CodeBases/knobab/data/benchmarking/mining/mined_clauses.csv");
//    std::ofstream log1("/home/sam/Documents/Repositories/CodeBases/knobab/data/benchmarking/mining/mined_clauses.csv", std::ios_base::app | std::ios_base::out);
//    if (!filePreexists1) {
//        log1 << "algorithm" << ","
//                << "clause" << ","
//                << "support" << std::endl;
//    }
//
    for (const pattern_mining_result<DeclareDataAware>& result : list.first) {
        std::cout << result << std::endl;
//        if(result.clause.right_act != "") {
//            std::cout << "BOLT" << ","
//                 << result.clause.casusu + "(" + result.clause.left_act + "+" + result.clause.right_act <<  + ")" << ","
//                 << result.support_declarative_pattern << std::endl;
//        }
//        else {
//            std::cout << "BOLT" << ","
//                 << result.clause.casusu + "(" + result.clause.left_act <<  + ")" << ","
//                 << result.support_declarative_pattern << std::endl;
//        }
    }

    if(!no_stats){
        bool filePreexists = std::filesystem::exists(logger_file);
        std::ofstream log(logger_file, std::ios_base::app | std::ios_base::out);
        if (!filePreexists) {
            LoggerInformation::log_csv_file_header(log);
        }

        env.experiment_logger.model_data_decomposition_time = 0;
        env.experiment_logger.model_atomization_time = 0;
        env.experiment_logger.model_declare_to_ltlf = 0;
        env.experiment_logger.model_ltlf_query_time = list.second;

        env.experiment_logger.log_csv_file(log);
    }
}

static inline void decrease_support_X(const KnowledgeBase &kb,
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

static inline void fast_forward_equals(trace_t trace_id,
                                       ActTable::record*& to_increment,
                                       ActTable::record *&end) {
    while ((to_increment != end) &&
           (to_increment->entry.id.parts.trace_id == trace_id)) {
        to_increment++;
    }
}

static inline void fast_forward_lower(trace_t trace_id,
                                      ActTable::record*& to_increment,
                                      ActTable::record *&end) {
    while ((to_increment != end) &&
           (to_increment->entry.id.parts.trace_id < trace_id)) {
        to_increment++;
    }
}

struct forNegation {
    act_t act;
    std::vector<trace_t> witnesses;
    double local_support;

    DEFAULT_CONSTRUCTORS(forNegation)
    forNegation(act_t act, const std::vector<trace_t> &witnesses, double localSupport) : act(act),
                                                                                         local_support(localSupport),
                                                                                         witnesses(witnesses) {}
};

static inline DeclareDataAware &
getAware(const KnowledgeBase &kb, bool special_temporal_patterns, bool only_precise_temporal_patterns,
         const CountTemplate &count_table, uint64_t minimum_support_threshold,
         std::vector<pattern_mining_result<DeclareDataAware>> &declarative_clauses,
         const pattern_mining_result<Rule<act_t>> &result, act_t A, act_t B, DeclareDataAware &clause) {
    auto ntraces = kb.nTraces();
//    auto nacts = kb.nAct();

    std::vector<trace_t> allTraces;
    for (trace_t sigma = 0; sigma<ntraces; sigma++) allTraces.emplace_back(sigma);
    if (special_temporal_patterns) {
        size_t expected_support = only_precise_temporal_patterns ?
                                  ntraces :
                                  minimum_support_threshold;
        size_t tolerated_errors = ntraces - expected_support;


        size_t alles_not_nexte = 0;
        bool alles_nexte = true;
        bool hasClausesMined = false;
        size_t alles_not_prev = 0,
                alles_not_next = 0;
        bool alles_prev = true, alles_next = true;
        std::vector<trace_t> removed_traces_from_response;
        auto alles_precedence = true;
        size_t alles_not_precedence = 0;
        auto alles_response = true;
        size_t alles_not_response = 0;
        auto alles_altresponse = true;
        size_t alles_not_altresponse = 0;
        std::vector<bool> isTraceVisitedU(ntraces, false);

// 1) First kind of specialized patterns:
// events happening immediately previously and next:
// This might leverage the prev/next pointers!
        for (auto sigma = 0; sigma < ntraces; sigma++) {
            auto lA = count_table.resolve_length(A, sigma);
            if (lA > 0) {
                if (lA != count_table.resolve_length(B, sigma)) {
                    alles_not_nexte++;
                    if ((ntraces - alles_not_nexte) < expected_support) {
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
//        std::vector<bool> isTraceVisitedU(ntraces, false);


// 2) Second kind of patters, always starting scanning from the
// activation condition (that is also the premise of the rule).
// This is still computed, as it is required for both 1) and 2)
        auto a_beginend = kb.timed_dataless_exists(A);
        DEBUG_ASSERT(a_beginend.first != a_beginend.second);

        auto b_beginend = kb.timed_dataless_exists(B);
// As I obtained the rule, there should be some data pertaining to it!
        DEBUG_ASSERT(b_beginend.first != b_beginend.second);



        while (a_beginend.first < a_beginend.second) {
            if ((!alles_precedence) && (!alles_response) && (!alles_altresponse))
                break;
            auto trace_id = a_beginend.first->entry.id.parts.trace_id;
//            if ((clause.left_act == "c") && (clause.right_act == "b") && (trace_id == 7)) {
//                auto cp = kb.act_table_by_act_id.secondary_index.at(8);
//                std::cout << kb.act_table_by_act_id << std::endl;
//                while (cp.first) {
//                    std::cout <<
//                              kb.event_label_mapper.get(
//                                      cp.first->entry.id.parts.act) << " ";
//                    cp.first = cp.first->next;
//                }
//                std::cout << std::endl;
//                std::cout << ": HERE" << std::endl;
//            }
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
                    bool all_response_in_trace = true;
//                            all_altresponse_in_trace = true;

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

//                        {
                            auto tmp = a_beginend.first++;
//                            if ((tmp != a_beginend.second) &&
//                                (tmp->entry.id.parts.event_id <
//                                 b_beginend.first->entry.id.parts.event_id)) {
//                                all_altresponse_in_trace = false;
//                            }
//                        }

//                                a_beginend.first++;
                    }
                    if (!all_response_in_trace) {
                        removed_traces_from_response.emplace_back(trace_id);
                        decrease_support_X(kb, expected_support, alles_response, alles_not_response);
                    }
//                    if (!all_altresponse_in_trace) {
//                        decrease_support_X(kb, expected_support, alles_altresponse, alles_not_altresponse);
//                    }
                    fast_forward_equals(trace_id, a_beginend.first, a_beginend.second);
                }
            }

            isTraceVisitedU[trace_id] = true;
        }

        if  (((!alles_nexte) || (alles_not_nexte > 0))) {
// This is still computed, as it is required for both 1) and 2)
            auto a_beginend = kb.timed_dataless_exists(A);
            DEBUG_ASSERT(a_beginend.first != a_beginend.second);
            while (a_beginend.first != a_beginend.second) {
//                if (!isTraceVisitedU.at(a_beginend.first->entry.id.parts.trace_id))
                {
                    if (alles_prev && (a_beginend.first->prev == nullptr ||
                                       (a_beginend.first->prev->entry.id.parts.act != B))) {
                        alles_not_prev++;
                        if ((ntraces - alles_not_prev) < expected_support) {
                            alles_prev = false;
                        }
                    }
                    if (alles_next && (a_beginend.first->next == nullptr ||
                                       (a_beginend.first->next->entry.id.parts.act != B))) {
                        alles_not_next++;
                        if ((ntraces - alles_not_next) < expected_support) {
                            alles_next = false;
                        }
                    }
//                    isTraceVisitedU[a_beginend.first->entry.id.parts.trace_id] = true;
                }
                if ((!alles_next) && (!alles_prev)) {
                    break; // Breaking only if both conditions are never met
                }
                a_beginend.first++;
            }


// re-setting the vector to all falses!
//            std::fill(isTraceVisitedU.begin(), isTraceVisitedU.end(), false);
        }



        if (alles_prev && (alles_not_prev <= alles_not_next) && (alles_not_prev <= tolerated_errors)) {
            hasClausesMined = true;
            clause.casusu = "ChainPrecedence";
            declarative_clauses.emplace_back(clause,
                                             result.support_generating_original_pattern,
                                             (((double) (ntraces - alles_not_prev)) /
                                              ((double) ntraces)),
                                             -1);
        }
        if (alles_next && (alles_not_prev >= alles_not_next) && (alles_not_next <= tolerated_errors)) {
            hasClausesMined = true;
            clause.casusu = "ChainResponse";
            declarative_clauses.emplace_back(clause,
                                             result.support_generating_original_pattern,
                                             (((double) (ntraces - alles_not_next)) /
                                              ((double) ntraces)),
                                             -1);
        }
        if ((!hasClausesMined) && alles_nexte && (alles_not_nexte<= tolerated_errors)) {
            clause.casusu = "ChainSuccession";
            declarative_clauses.emplace_back(clause,
                                             result.support_generating_original_pattern,
                                             (((double) (ntraces - alles_not_nexte)) /
                                              ((double) ntraces)),
                                             -1);
        }
        if (alles_precedence && (alles_not_precedence<= tolerated_errors)) {
            clause.casusu = "Precedence";
            declarative_clauses.emplace_back(clause,
                                             result.support_generating_original_pattern,
                                             (((double) (ntraces - alles_not_precedence)) /
                                              ((double) ntraces)),
                                             -1);
        }
        if (alles_response && (alles_not_response<= tolerated_errors)) {
            clause.casusu = "Response";
            declarative_clauses.emplace_back(clause,
                                             result.support_generating_original_pattern,
                                             (((double) (ntraces - alles_not_response)) /
                                              ((double) ntraces)),
                                             -1);
//            if (ptn) {
//                std::vector<trace_t> responseSupp;
//                std::set_difference(allTraces.begin(), allTraces.end(),
//                                    removed_traces_from_response.begin(), removed_traces_from_response.end(),
//                                    std::back_inserter(responseSupp));
//                auto it_1 = ptn->find("Response");
//                if (it_1 == ptn->end()) {
//                    it_1 = ptn->emplace("Response", std::unordered_map < act_t, std::vector<forNegation> > {}).first;
//                }
//                auto it_2 = it_1->second.find(A);
//                if (it_2 == it_1->second.end()) {
//                    it_2 = it_1->second.emplace(A, std::vector<forNegation>{}).first;
//                }
//                it_2->second.emplace_back(B, responseSupp, (((double) (ntraces - alles_not_response)) /
//                                                            ((double) ntraces)));
//            }
        }
    }
    return clause;
}


//// Todo:
//inline void globallyA_And_FutureB(const std::pair<ActTable::record*, ActTable::record*>& left,
//                                  const std::pair<ActTable::record*, ActTable::record*>& right,
//                                  std::vector<ResultIndex>& result) {
////    if (right.first == right.second) {
////        result.clear();
////        return;
////    }
//    auto bCurrent = right.first, bEnd = right.second;
//    ResultIndex rcx;
//    bool hasMatch;
//
//    for (auto aCurrent = left.first, aEnd = left.second; aCurrent != aEnd; ) {
//        if ((aCurrent->entry.id.parts.trace_id > bCurrent->entry.id.parts.trace_id) ||
//            ((aCurrent->entry.id.parts.trace_id == bCurrent->entry.id.parts.trace_id) &&
//             (aCurrent->entry.id.parts.event_id > bCurrent->entry.id.parts.event_id))) {
//            bCurrent++;
//            if (bCurrent == bEnd) break;
//        } else {
//            auto newItr = bCurrent;
//            rcx.first = aCurrent->entry.id.parts.trace_id;
//            rcx.second = aCurrent->entry.id.parts.event_id;
//            hasMatch = false;
//
//            while (newItr != bEnd) {
//                if(newItr->entry.id.parts.trace_id != aCurrent->entry.id.parts.trace_id){
//                    break;
//                }
//                if (!hasMatch) {
//                    result.emplace_back(rcx);
//                    hasMatch = true;
//                }
//                newItr++;
//            }
//
//            if ((aCurrent->entry.id.parts.trace_id == bCurrent->entry.id.parts.trace_id) &&
//                (aCurrent->entry.id.parts.event_id == bCurrent->entry.id.parts.event_id)) {
//                bCurrent++;
//                if (bCurrent == bEnd) break;
//            }
//
//            aCurrent++;
//        }
//    }
//}

#include <yaucl/strings/serializers.h>
#include <unordered_map>
#include <yaucl/hashing/pair_hash.h>
#include <yaucl/hashing/uset_hash.h>


/** Pattern mining **/
std::pair<std::vector<pattern_mining_result<DeclareDataAware>>, double> pattern_mining(const KnowledgeBase& kb,
                                                                    double support,
                                                                    bool naif,
                                                                    bool init_end,
                                                                    bool special_temporal_patterns,
                                                                    bool only_precise_temporal_patterns,
                                                                    bool negative_patterns) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();
    std::unordered_map<std::string, std::unordered_map<act_t, std::vector<forNegation>>> patterns_to_negate;
    support = std::max(std::min(support, 1.0), 0.0); // forcing the value to be between 0 and 1.
    size_t log_size = kb.nTraces();
    std::unordered_set<act_t> absent_acts;
    const auto& count_table = kb.getCountTable();
    uint64_t minimum_support_threshold = std::min((uint64_t)std::ceil((double)log_size * support), log_size);
    uint64_t max_act_id = kb.nAct();
    for (auto i = 0; i<max_act_id; i++) absent_acts.insert(i);
//    FPTree t{count_table, minimum_support_threshold, max_act_id};
    std::vector<std::pair<act_t, size_t>> final_element_for_scan;
    std::vector<pattern_mining_result<DeclareDataAware>> declarative_clauses;
    bool doInitA = false;
//    std::cout << max_act_id << std::endl;
    auto fpt_result = fpgrowth(count_table, max_act_id, final_element_for_scan, minimum_support_threshold, 2);
//    std::cout << fpt_result << std::endl;
    std::unordered_set<Pattern> binary_patterns;
    std::unordered_map<std::unordered_set<act_t>, uint64_t> mapper;
    std::unordered_set<act_t> unary_patterns_for_non_exact_support;
    for (const auto& x : fpt_result) {
        if (x.second.size() == 1) {
            auto it = *x.second.begin();
            absent_acts.erase(it);
//            std::cout <<
//                      kb.event_label_mapper.get(it)  <<"=" << it << "!" << x.second << std::endl;
            if (x.first == kb.nTraces()) {
                // If the support is actually one, then we can infer that if an event
                // exists in all of the events, that should always happen
//                if (naif) {
//                    // The naif algorithm will just state that, if the pattern exists,
//                    // then it exists at least once
//                    DeclareDataAware clause;
//                    clause.left_act = kb.event_label_mapper.get(it);
//                    clause.n = 1;
//                    clause.casusu = "Exists1";
//                    declarative_clauses.emplace_back(((double)x.first)/((double)kb.nTraces()), clause);
//                } else
                {
                    // The non-naif version is exploiting the couting information from
                    // the counting table, and also providing an expected number of times
                    // the event should happen. You can also apply the same reasoning
                    // for mining the absence pattern
                    DeclareDataAware clause;
                    clause.left_act = kb.event_label_mapper.get(it);
                    event_t n = std::numeric_limits<event_t>::max(),
                            N = 0;
                    auto cp = count_table.resolve_primary_index2(it);
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
                    declarative_clauses.emplace_back(((double)x.first)/((double)kb.nTraces()), clause);
                    clause.n = N+1;
                    clause.casusu = "Absence";
                    declarative_clauses.emplace_back(((double)x.first)/((double)kb.nTraces()), clause);
                }
            } else {
                // If the support is less than one, then we cannot state that
                // all of these events actually exist in all of the possible traces, and
                // we need to relax such conditions into a choice between the patterns,
                // as this will maximise the score, precision-wise.
                // We are postponing such discussion into point A)

                absent_acts.erase(it);
                unary_patterns_for_non_exact_support.insert(it);
                doInitA = true;
            }
        } else {
            DEBUG_ASSERT(x.second.size() == 2);
#ifdef DEBUG
            std::cout <<  kb.event_label_mapper.get(*x.second.begin()) << "." << kb.event_label_mapper.get(*(++x.second.begin())) << std::endl;
#endif
            for (const auto& y : x.second) absent_acts.erase(y);
//            std::set<act_t> S{x.second.begin(), x.second.end()};
            binary_patterns.emplace(x.second, x.first);
            mapper[x.second] = x.first;
        }
    }
//    if (negative_patterns) {
//        DeclareDataAware clause;
//        clause.casusu = "Absence";
//        for (const auto& y : absent_acts) {
//            clause.left_act = kb.event_label_mapper.get(y);
//            auto cp = count_table.resolve_primary_index2(y);
//            size_t absence1_not_supp = 0;
//            event_t N = 0;
//            while (cp.first != cp.second) {
//                if (cp.first->id.parts.event_id > 0) absence1_not_supp++;
//                if (cp.first->id.parts.event_id > N) {
//                    N = cp.first->id.parts.event_id;
//                }
//                cp.first++;
//            }
//            if ((log_size-absence1_not_supp) > minimum_support_threshold) {
//                clause.n = 1;
//                declarative_clauses.emplace_back(((double)(log_size-absence1_not_supp))/((double)kb.nTraces()), clause);
//            }
//            clause.n = N+1;
//            declarative_clauses.emplace_back(1.0, clause);
//        }
//        // NotCoexistence
//
//    }
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
                        std::unordered_set<act_t> lS{a,b};
                        curr_pair.second = inv_pair.first = b;
                        if ((!visited_pairs.emplace(curr_pair).second) ||
                            (!visited_pairs.emplace(inv_pair).second)) continue;
                        const auto& aSet = inv_map.at(a);
                        const auto& bSet = inv_map.at(b);
                        std::pair<size_t, size_t> ratio = yaucl::iterators::ratio(aSet.begin(), aSet.end(), bSet.begin(), bSet.end());
                        double local_support = ((double)(ratio.first)) / ((double)log_size);
                        DeclareDataAware clause;
                        clause.left_act = kb.event_label_mapper.get(a);
                        clause.right_act = kb.event_label_mapper.get(b);
                        clause.n = 1;
                        if (ratio.first >= minimum_support_threshold) {
                            // I can consider this pattern, again, only if it is within the expected
                            // support rate which, in this case, is given by the amount of traces
                            // globally setting up this pattern
                            auto it = mapper.find(lS);
                            if (ratio.second == 0) {
                                // If there is no intersection, I can also be more strict if I want,
                                // and provide an exclusive choice pattern if I am confident that
                                // the two events will never appear in the same trace (according to
                                // the "training" data
                                clause.casusu = "ExclChoice";
                            } else {
                                clause.casusu = "Choice";
                            }
                            declarative_clauses.emplace_back(clause, (it != mapper.end()) ? ((double)it->second)/((double)log_size) : 0.0, local_support, -1);
                        }

                    }
                }
            }
        }
    }
    map.clear();
    inv_map.clear();

    DataMiningMetrics counter{count_table};
//    std::cout << "Pattern generation: " << std::endl;
    for (const Pattern& pattern : binary_patterns) {
        std::vector<pattern_mining_result<Rule<act_t>>> candidate_rule;

        DEBUG_ASSERT(pattern.first.size() == 2);
        Rule<act_t> lr, rl;
        auto it = pattern.first.begin();
        lr.head.emplace_back(*it); rl.tail.emplace_back(*it);
#ifdef DEBUG
        std::unordered_set<std::string> SSSS;
        auto cp = it;
        SSSS.insert(kb.event_label_mapper.get(*it));
        SSSS.insert(kb.event_label_mapper.get(*(++cp)));
        if (SSSS.contains("g") && SSSS.contains("f")) {
            std::cout <<"HERE"<< std::endl;
        }
#endif
//        std::cout << " - Pattern: " << kb.event_label_mapper.get(*it) << ",";
        it++;
//        std::cout <<kb.event_label_mapper.get(*it) << std::endl << std::endl;
        lr.tail.emplace_back(*it); rl.head.emplace_back(*it);
        double lr_conf = counter.confidence(lr);
        double rl_conf = counter.confidence(rl);
//        std::cout << "   conf: " << counter.lift(lr) << " conf: " << counter.lift(rl) << std::endl;
//        std::cout << "   lift: " << counter.lift(lr) << " lift: " << counter.lift(rl) << std::endl;
        if ((lr_conf == rl_conf) && (rl_conf >= support)) {
            Rule<act_t> lrBoth;
            candidate_rule.emplace_back(Rule<act_t>{pattern.first}, ((double)pattern.second)/((double)log_size), -1, -1);
        } else {
            if (lr_conf >= rl_conf) {
                if (lr_conf >= support)
                    candidate_rule.emplace_back(lr, ((double)pattern.second)/((double)log_size), counter.support(lr), lr_conf);
            } else if (rl_conf >= support) {
                if (rl_conf >= support)
                    candidate_rule.emplace_back(rl, ((double)pattern.second)/((double)log_size), counter.support(rl), rl_conf);
            }
        }

        for (const auto& result: candidate_rule) {            // Generate the hypotheses containing a lift greater than one
            act_t A;
            act_t B;
            DeclareDataAware clause;
            clause.n = 1;
            bool alsoFlip = false;
            double dss = result.support_declarative_pattern;
            size_t countOk = std::ceil(dss * kb.nTraces());
            if (result.clause.tail.empty()) {
                // CoExistence pattern
                A = result.clause.head.at(0);
                B = result.clause.head.at(1);
                clause.casusu = "CoExistence";
                alsoFlip = true;
                auto cpA = kb.getCountTable().resolve_primary_index2(A);
                auto cpB = kb.getCountTable().resolve_primary_index2(B);
                countOk = 0;
                while ((cpA.first != cpA.second) && (cpB.first != cpB.second)) {
                    if ((cpA.first->id.parts.event_id > 0) && (cpB.first->id.parts.event_id > 0))
                        countOk++;
                    else if ((cpA.first->id.parts.event_id == 0) && (cpB.first->id.parts.event_id == 0))
                        countOk++;
                    cpA.first++;
                    cpB.first++;
                }
                dss = ((double)countOk)/((double)log_size);
            } else if (result.clause.tail.size() == 1) {
                A = result.clause.head.at(0);
                B = result.clause.tail.at(0);
                // <>A -> <>B
                clause.casusu = "RespExistence";
            };
            clause.left_act = kb.event_label_mapper.get(A);
            clause.right_act = kb.event_label_mapper.get(B);
            size_t prev = declarative_clauses.size();
//            if (alsoFlip && ((clause.left_act == "i" || clause.left_act == "h"))
//                         && ((clause.right_act == "i" || clause.right_act == "h"))) {
//                std::cout << counter.support(lr) << " vs. " << counter.support(rl) << std::endl;
//                std::cout << "DEBUG" << std::endl;
//
//            }
            getAware(kb, special_temporal_patterns, only_precise_temporal_patterns, count_table,
                     std::max(countOk, minimum_support_threshold),
                     declarative_clauses, result, A, B, clause);
            if (alsoFlip) {
                clause.right_act = kb.event_label_mapper.get(A);
                clause.left_act = kb.event_label_mapper.get(B);
                getAware(kb, special_temporal_patterns, only_precise_temporal_patterns, count_table,
                         std::max(countOk, minimum_support_threshold),
                         declarative_clauses, result, B, A, clause);
            }
            if (declarative_clauses.size() == prev) {
                declarative_clauses.emplace_back(clause,
                                                 result.support_generating_original_pattern,
                                                 dss,
                                                 result.confidence_declarative_pattern);
            }
        }
    }


    // TODO: debug (might get stuck in a loop!)
//    if (negative_patterns) {
//        std::vector<bool> isTraceVisitedU(log_size, false);
//        std::vector<pattern_mining_result<DeclareDataAware>> negative_to_test;
//        DeclareDataAware clause;
//        clause.n = 1;
//        clause.casusu = "NotSuccession";
//        auto it = patterns_to_negate.find("Response");
//        if (it != patterns_to_negate.end()) {
//            for (const auto& cp : it->second) {
//                act_t A = cp.first;
//                clause.left_act = kb.event_label_mapper.get(A);
//                std::vector<bool> contributingToNegation(max_act_id, false);
//                std::unordered_set<trace_t> tracesForNegation;
//                double prod = 1.0;
//                for (const auto& cp2 : cp.second) {
//                    contributingToNegation[cp2.act] = true;
//                    tracesForNegation.insert(cp2.witnesses.begin(), cp2.witnesses.end());
//                    prod *= cp2.local_support;
//                }
//                size_t support = tracesForNegation.size();
//                for (act_t BNeg = 0; BNeg < max_act_id; BNeg++) {
//                    if (contributingToNegation.at(BNeg) || (BNeg == A)) continue;
//                    if (support >= minimum_support_threshold) {
//                        std::fill(isTraceVisitedU.begin(), isTraceVisitedU.end(), false);
//                        clause.right_act = kb.event_label_mapper.get(BNeg);
//
//                        auto alles_response = true;
//                        size_t alles_not_response = 0;
//                        auto a_beginend = kb.timed_dataless_exists(A);
//                        auto b_beginend = kb.timed_dataless_exists(BNeg);
//
//                        while (a_beginend.first < a_beginend.second) {
//                            if (!alles_response) break;
//                            auto trace_id = a_beginend.first->entry.id.parts.trace_id;
//                            DEBUG_ASSERT(!isTraceVisitedU.at(trace_id));
//                            auto trace_id_visited = isTraceVisitedU.at(trace_id);
//
//                            if (b_beginend.first == b_beginend.second) {
//                                // Problem 1)
//                                // This might be a valid precedence, as nothing is stated
//                                // to what should happen after the A, but I cannot exploit
//                                // for a response: therefore, I'm counting it
////                                decrease_support_X(kb, minimum_support_threshold, alles_response, alles_not_response);
//
//                                // Now, skipping to the next trace, as there is no more information for as
//                                fast_forward_equals(trace_id, a_beginend.first, a_beginend.second);
//                                isTraceVisitedU[trace_id] = true;
//                                continue;
//                            } else {
//                                // Otherwise, I have something to check related to B!
//                                if (a_beginend.first->entry.id.parts.trace_id >
//                                    b_beginend.first->entry.id.parts.trace_id) {
//
//                                    // Moving b until I find something related to b. A is kept fixed and not incremented
//                                    fast_forward_lower(trace_id, b_beginend.first, b_beginend.second);
//                                    // Not setting the current trace to be visited, as we need to fast-forward B first
//                                    continue;
//                                } else if (a_beginend.first->entry.id.parts.trace_id <
//                                           b_beginend.first->entry.id.parts.trace_id) {
//
//                                    // If I am not able to find a B, then this is detrimental to A's response
//                                    // (Problem 1)
////                                    decrease_support_X(kb, minimum_support_threshold, alles_response, alles_not_response);
//
//                                    // Now, skipping to the next trace, as there is no more information for as
//                                    fast_forward_equals(trace_id, a_beginend.first, a_beginend.second);
//                                    continue;
//                                } else {
//                                    // Please remember, we are not visiting traces, rather than
//                                    // events associated to traces. Therefore, it is of the
//                                    // uttermost importance to remember conditions related to
//                                    // the trace level, and to remember whether this was the
//                                    // first time the trace was visited or not.
//
//                                    // Problem 2)
//                                    // If B happens before the event A, this cannot be referred
//                                    // to the precedence, and therefore this should be decreased
//                                    // Still, this consideration should be performed only up until
//                                    // the first event is visited
////                                    if (b_beginend.first->entry.id.parts.event_id <
////                                        a_beginend.first->entry.id.parts.event_id) {
////                                        decrease_support_X(kb, expected_support, alles_precedence, alles_not_precedence);
////                                    }
//
//                                    // While I'm scanning the A events within the same trace
//                                    bool all_response_in_trace = true;
//
//                                    while ((a_beginend.first != a_beginend.second) &&
//                                           (a_beginend.first->entry.id.parts.trace_id == trace_id)) {
//                                        // ignoring all of the B events that are not relevant for the task!
//                                        while ((b_beginend.first != b_beginend.second) &&
//                                               (b_beginend.first->entry.id.parts.trace_id == trace_id) &&
//                                               (b_beginend.first->entry.id.parts.event_id <
//                                                a_beginend.first->entry.id.parts.event_id)) {
//                                            b_beginend.first++;
//                                        }
//                                        if ((b_beginend.first != b_beginend.second) &&
//                                            (b_beginend.first->entry.id.parts.trace_id == trace_id) &&
//                                            (b_beginend.first->entry.id.parts.event_id >=
//                                             a_beginend.first->entry.id.parts.event_id)) {
//                                            // Problematic, I have a match!
//                                            all_response_in_trace = false;
//                                            break;
//                                        } else {
//                                            // If there is no match for the B event, then I'm setting this to false
//                                            // and quitting the iteration
//                                        }
//                                        a_beginend.first++;
//                                    }
//                                    if (!all_response_in_trace) {
//                                        decrease_support_X(kb, minimum_support_threshold, alles_response, alles_not_response);
//                                    }
//                                    fast_forward_equals(trace_id, a_beginend.first, a_beginend.second);
//                                }
//                            }
//
//                            isTraceVisitedU[trace_id] = true;
//                        }
//
//                        if (alles_response) {
//                            clause.casusu = "NotSuccession";
//                            declarative_clauses.emplace_back(clause,
//                                                             ((double)support) / ((double)log_size),
//                                                             (((double) (log_size - alles_not_response)) /
//                                                              ((double) log_size)),
//                                                             -1);
//                        }
//                    }
//                }
//            }
//
//
//        }
//
//
//
//    }

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
    auto t2 = high_resolution_clock::now();
    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms\n";
//    exit(1);
    return std::pair<std::vector<pattern_mining_result<DeclareDataAware>>, double>(declarative_clauses, ms_double.count());
}



#include "knobab/mining/refinery.h"


void extractPayloads(std::unordered_map<std::string, std::unordered_map<std::string, LTLfQuery>>::iterator it2,
                        std::vector<pattern_mining_result<DeclareDataAware>> &v_intersection,
                        Environment &tmpEnv,
                        std::vector<std::vector<std::pair<payload_data, int>>> &a,
                     std::vector<std::vector<std::pair<payload_data, int>>> &t,
                     std::vector<std::vector<std::pair<payload_data, int>>> &corr,
//                        RefineOver what,
                        int clazz) {
    tmpEnv.clearModel(); // initializing the model pipeline
    std::unordered_map<std::string, LTLfQuery>* plans = &it2->second;
    tmpEnv.conjunctive_model.clear();
    for (const auto& ref : v_intersection) {
        tmpEnv.conjunctive_model.emplace_back(ref.clause);
    }
    tmpEnv.experiment_logger.model_parsing_ms = 0;
    tmpEnv.experiment_logger.model_size = v_intersection.size();
    tmpEnv.experiment_logger.model_filename = "Intersection";
    bool doPreliminaryFill = true;
    bool ignoreActForAttributes = false;
    bool creamOffSingleValues = true;
    GroundingStrategyConf::pruning_strategy grounding_strategy = GroundingStrategyConf::NO_EXPANSION;
    tmpEnv.set_grounding_parameters(doPreliminaryFill, ignoreActForAttributes, creamOffSingleValues, grounding_strategy);
    tmpEnv.doGrounding();
    std::string atomj{"p"};
    AtomizationStrategy atom_strategy = AtomizeOnlyOnDataPredicates;
    size_t n = 3;
    tmpEnv.set_atomization_parameters(atomj, n , atom_strategy);
    tmpEnv.init_atomize_tables();
    tmpEnv.first_atomize_model();
    size_t nThreads = 1;
    auto& ref2 = tmpEnv.experiment_logger;
    EnsembleMethods em = Nothing;
    OperatorQueryPlan op = FastOperator_v1;
    tmpEnv.set_maxsat_parameters(nThreads, em, op);
    MAXSatPipeline ref(plans, nThreads, BLOCK_STATIC_SCHEDULE, 3);
    ref.final_ensemble = em;
    ref.operators = op;
    ref.pipeline(&tmpEnv.grounding, tmpEnv.ap, tmpEnv.db);
    for (auto & i : ref.declare_to_query) {
        auto& clause_result = i->result;
        extractPayload(&tmpEnv,a.emplace_back(),t.emplace_back(),corr.emplace_back(),clause_result,
//         what,
         clazz);
    }
}

#include <cmath>

void collectRefinedClause(std::pair<std::vector<pattern_mining_result<DeclareDataAware>>, double> &P,
                          std::pair<std::vector<pattern_mining_result<DeclareDataAware>>, double> &N,
                          std::unordered_map<int, Environment *> &tree_to_env,
                          std::unordered_map<int, std::vector<std::vector<dt_predicate>>> &world_to_paths,
                          pattern_mining_result<DeclareDataAware> &clause, const DecisionTree<payload_data> &dtA,
                          const RefineOver &what) {
    world_to_paths.clear();
    dtA.populate_children_predicates(world_to_paths);
    for(auto& pair : world_to_paths){
        auto ref = tree_to_env.find(pair.first);
        DEBUG_ASSERT(ref != tree_to_env.end());
        DeclareDataAware c = actualClauseRefine(clause.clause, what, pair);
        if (pair.first == 1) {
            P.first.emplace_back(clause, c);
        } else if (pair.first == 0) {
            N.first.emplace_back(clause, c);
        }
    }
}

std::pair<std::vector<pattern_mining_result<DeclareDataAware>>,
          std::vector<pattern_mining_result<DeclareDataAware>>> classifier_mining(ServerQueryManager sqm,
                                                                       const std::string& pos,
                                                                       const std::string& neg,
                                                                       double support,
                                                                       double tau,
                                                                       bool naif,
                                                                       bool init_end,
                                                                       bool special_temporal_patterns,
                                                                       bool only_precise_temporal_patterns,
                                                                       bool negative_ones) {
    std::string query_plan = "queryplan \"nfmcp23\" {\n"
                             "     template \"Init\"                   := INIT  activation\n"
                             "     template \"End\"                    := END activation\n"
                             "     template \"Exists1\"                := (EXISTS 1 activation)\n"
                             "     template \"Absence1\"               := ABSENCE 1 activation\n"
                             "     template \"Absence2\"               := ABSENCE 2 activation\n"
                             "     template \"Precedence\" args 2      := ((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)\n"
                             "     template \"ChainPrecedence\" args 2 := G(((LAST OR t (NEXT EXISTS ~ 1 t #1))) OR t ((NEXT EXISTS 1 t #1 activation) AND t THETA INV (EXISTS 1 t #2 target) ))\n"
                             "     template \"Choice\" args 2          := (EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)\n"
                             "     template \"Response\" args 2        := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )\n"
                             "     template \"ChainResponse\" args 2   := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                             "     template \"RespExistence\" args 2   := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target)))\n"
                             "     template \"ExlChoice\" args 2       := ((EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)) AND ((ABSENCE 1 #1) OR (ABSENCE 1 #2))\n"
                             "     template \"CoExistence\" args 2     := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target))) AND ( ((ABSENCE 1 #2)) OR ((EXISTS 1 #2 activation) AND THETA INV (EXISTS 1 #1 target)))\n"
                             "     template \"NotCoExistence\" args 2  := ~ ((EXISTS 1 t #1 activation) AND THETA (EXISTS 1 t #2 target)) PRESERVE\n"
                             "\n"
                             "     template \"Succession\" args 2      := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )) AND (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 target)) OR (ABSENCE 1 #2))\n"
                             "     template \"NegSuccession\" args 2   := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Gt  (EXISTS ~ 1 t #2)) ))\n"
                             "     template \"ChainSuccession\" args 2 := G( (((LAST OR t (NEXT EXISTS ~ 1 t #2))) OR t ((NEXT EXISTS 1 t #2 activation) AND t THETA INV (EXISTS 1 t #1 target))) AND t\n"
                             "                                             ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                             "                                           )\n"
                             "     template \"AltResponse\" args 2     := G ( (EXISTS ~ 1 t #1) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT ((EXISTS ~ 1 t #1) U t (EXISTS 1 t #2 target)) )))\n"
                             "     template \"AltPrecedence\" args 2   := (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)) AND\n"
                             "                                           (G(((EXISTS ~ 1 t #1)) OR t (((EXISTS 1 t #1 activation)) AND t THETA (NEXT (((EXISTS  ~ 1 t #1) U t (EXISTS 1 t #2 target)) OR t (G t (EXISTS  ~ 1 t #1))))  )))\n"
                             "}";

    sqm.runQuery(query_plan);
    auto it2 = sqm.planname_to_declare_to_ltlf.find("nfmcp23");
    auto P = pattern_mining(sqm.multiple_logs[pos].db, support, naif, init_end, special_temporal_patterns, only_precise_temporal_patterns, negative_ones);
    auto N = pattern_mining(sqm.multiple_logs[neg].db, support, naif, init_end, special_temporal_patterns, only_precise_temporal_patterns, negative_ones);
    auto f = [](const pattern_mining_result<DeclareDataAware>& l, const pattern_mining_result<DeclareDataAware>& r) {
        return std::tie(l.clause.casusu, l.clause.left_act, l.clause.right_act, l.clause.n) < std::tie(r.clause.casusu, r.clause.left_act, r.clause.right_act, r.clause.n);
    };
    auto e = [](const pattern_mining_result<DeclareDataAware>& l, const pattern_mining_result<DeclareDataAware>& r) {
        return std::tie(l.clause.casusu, l.clause.left_act, l.clause.right_act, l.clause.n) == std::tie(r.clause.casusu, r.clause.left_act, r.clause.right_act, r.clause.n);
    };
    std::sort(P.first.begin(), P.first.end(), f);
    P.first.erase( std::unique( P.first.begin(), P.first.end(), e ), P.first.end() );
    std::sort(N.first.begin(), N.first.end(), f);
    N.first.erase( std::unique( N.first.begin(), N.first.end(), e ), N.first.end() );

    std::vector<pattern_mining_result<DeclareDataAware>> v_intersection;
    std::set_intersection(P.first.begin(), P.first.end(), N.first.begin(), N.first.end(),
                          std::back_inserter(v_intersection),
                          [](const pattern_mining_result<DeclareDataAware>& l, const pattern_mining_result<DeclareDataAware>& r) {
                              return std::tie(l.clause.casusu, l.clause.left_act, l.clause.right_act, l.clause.n) == std::tie(r.clause.casusu, r.clause.left_act, r.clause.right_act, r.clause.n);
                          });
    if (v_intersection.empty()) {
        return {P.first, N.first};
    }
    tau = std::clamp(tau, 0.5, 1.);

    // Removing the elements at the intersection
    P.first.erase(std::remove_if(P.first.begin(), P.first.end(), [&v_intersection,&e](const pattern_mining_result<DeclareDataAware>& l){
        return std::find_if(v_intersection.begin(), v_intersection.end(), [&l,&e](const auto &x) {
            return e(x,l);
        }) != v_intersection.end();
    }), P.first.end());
    N.first.erase(std::remove_if(N.first.begin(), N.first.end(), [&v_intersection,&e](const pattern_mining_result<DeclareDataAware>& l){
        return std::find_if(v_intersection.begin(), v_intersection.end(), [&l,&e](const auto &x) {
            return e(x,l);
        }) != v_intersection.end();
    }), N.first.end());


    // Attempting at classifying per activation
    std::vector<std::vector<std::pair<payload_data, int>>> per_clause_AV, per_clause_TV, per_clause_CV;
    extractPayloads(it2, v_intersection, sqm.multiple_logs[pos], per_clause_AV, per_clause_TV, per_clause_CV,  1);
    extractPayloads(it2, v_intersection, sqm.multiple_logs[neg], per_clause_AV, per_clause_TV, per_clause_CV,   0);



//    // Extraction of activation conditions per clause
//    std::vector<std::vector<std::pair<ResultRecord,int>>> activations;
//    extractActivations(it2, v_intersection, sqm.multiple_logs[pos], activations, true);
//    extractActivations(it2, v_intersection, sqm.multiple_logs[pos], activations, false);

//    std::function<simple_data(const ResultRecord&, const std::string&)> fpos = [&sqm,&pos,&neg](const ResultRecord& x, const std::string& key) -> simple_data {
//            auto& kb = x.second.first == 1.0 ? sqm.multiple_logs[pos].db : sqm.multiple_logs[neg].db;
//            auto it = kb.attribute_name_to_table.find(key);
//            if (it != kb.attribute_name_to_table.end()) {
//                size_t offset = kb.act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(x.first.first).at(x.first.second);
//                std::optional<union_minimal> data = it->second.resolve_record_if_exists2(offset);
//                if(data.has_value()) {
//                    return data.value();
//                } else {
//                    switch (it->second.type) {
//                        case DoubleAtt:
//                        case LongAtt:
//                        case SizeTAtt:
//                        case BoolAtt:
//                            return 0.0;
//                        case StringAtt:
//                            return "";
//                    }
//                }
//            } else
//                return 0.0;
//    };

    std::unordered_set<std::string> numeric_keys, categorical_keys;
    for (const auto& [k,v] : sqm.multiple_logs[pos].db.attribute_name_to_table) {
        if (v.type == StringAtt)
            categorical_keys.insert(k);
        else
            numeric_keys.insert(k);
    }
    for (const auto& [k,v] : sqm.multiple_logs[neg].db.attribute_name_to_table) {
        if (v.type == StringAtt)
            categorical_keys.insert(k);
        else
            numeric_keys.insert(k);
    }

    worlds_activations w_activations;
    std::unordered_map<int, Environment*> tree_to_env;
    std::unordered_map<int, std::vector<std::vector<dt_predicate>>> world_to_paths;
    for (size_t i = 0, M = per_clause_AV.size(); i<M; i++) {
        auto& clause = v_intersection.at(i);

        // Refining over the activations first
        auto& V = per_clause_AV[i];
        auto it = V.begin(), en = V.end();
        DecisionTree<payload_data> dtA(it,
                                      en,
                                      1,
                                      selector,
                                      numeric_keys,
                                      categorical_keys,
                                      ForTheWin::gain_measures::Entropy,
                                      0.9,
                                      1,
                                      V.size(),
                                      1);
        if (dtA.goodness >= tau) {
            collectRefinedClause(P, N, tree_to_env, world_to_paths, clause, dtA, RefineOverActivation);
            // Do refine!
        } else {
            // Refining by target
            auto& W = per_clause_TV[i];
            auto itT = W.begin(), enT = W.end();
            DecisionTree<payload_data> dtT(itT,
                                          enT,
                                          1,
                                          selector,
                                          numeric_keys,
                                          categorical_keys,
                                          ForTheWin::gain_measures::Entropy,
                                          0.9,
                                          1,
                                          W.size(),
                                          1);
            if (dtT.goodness >= tau) {
                collectRefinedClause(P, N, tree_to_env, world_to_paths, clause, dtT, RefineOverTarget);
                // Do refine!
            } else {
                auto& C = per_clause_TV[i];
                auto itC = C.begin(), enC = C.end();
                DecisionTree<payload_data> dtC(itC,
                                               enC,
                                               1,
                                               selector,
                                               numeric_keys,
                                               categorical_keys,
                                               ForTheWin::gain_measures::Entropy,
                                               0.9,
                                               1,
                                               C.size(),
                                               1);
                if (dtC.goodness >= tau) {
                    collectRefinedClause(P, N, tree_to_env, world_to_paths, clause, dtC, RefineOverMatch);
                }
            }
        }
    }

    return {P.first, N.first};
}
