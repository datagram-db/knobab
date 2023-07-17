//
// Created by giacomo on 17/09/22.
//

#include "knobab/algorithms/mining/pattern_mining.h"
#include "yaucl/learning/dt_predicate.h"
#include "yaucl/learning/DecisionTree.h"
#include <chrono>
#include <knobab/server/query_manager/Environment.h>
#include "roaring64map.hh"
#include "knobab/mining/refinery.h"
#include "knobab/server/dataStructures/marked_event.h"

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
//    std::cout << "Starting from now!" << std::endl;
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
//#ifdef DEBUG
//        std::cout << result << std::endl;
//#endif
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

#define RESPONSE_AB_ID  (0b00000001)
#define PRECEDENCE_AB_ID  (0b00000010)
#define SUCCESSION_AB_ID  (0b00000011)
#define CHAIN_RESPONSE_AB_ID  (0b00000100)
#define CHAIN_PRECEDENCE_AB_ID  (0b00001000)
#define SURROUND_AB_ID  (0b00001100)

#define RESPONSE_BA_ID  (0b00010000)
#define PRECEDENCE_BA_ID  (0b00100000)
#define SUCCESSION_BA_ID  (0b00110000)
#define CHAIN_RESPONSE_BA_ID  (0b01000000)
#define CHAIN_PRECEDENCE_BA_ID  (0b10000000)
#define SURROUND_BA_ID  (0b11000000)

#define CHAIN_SUCCESSION_AB_ID  (0b10000100)
#define CHAIN_SUCCESSION_BA_ID  (0b01001000)

struct GetAwareData {
    GetAwareData() {
        flags = 0;
        r_lb_violations = {};
        p_lb_violations = {};
        r_lb_sup_conf = { -1, -1 };
        p_lb_sup_conf = { -1, -1 };
        r_activation_traces = { {}, {} };
        p_activation_traces = { {}, {} };

        cr_lb_violations = {};
        cp_lb_violations = {};
        cr_lb_sup_conf = { -1, -1 };
        cp_lb_sup_conf = { -1, -1 };
        cr_activation_traces = { {}, {} };
        cp_activation_traces = { {}, {} };
    };

    uint8_t flags;

    roaring::Roaring r_lb_violations;
    roaring::Roaring p_lb_violations;
    std::pair<double,double> r_lb_sup_conf;
    std::pair<double,double> p_lb_sup_conf;
    std::pair<roaring::Roaring, roaring::Roaring> r_activation_traces;
    std::pair<roaring::Roaring, roaring::Roaring> p_activation_traces;

    roaring::Roaring cr_lb_violations;
    roaring::Roaring cp_lb_violations;
    std::pair<double,double> cr_lb_sup_conf;
    std::pair<double,double> cp_lb_sup_conf;
    std::pair<roaring::Roaring, roaring::Roaring> cr_activation_traces;
    std::pair<roaring::Roaring, roaring::Roaring> cp_activation_traces;
};

static inline void
getAware(const KnowledgeBase &kb, bool only_precise_temporal_patterns,
         const CountTemplate &count_table, uint64_t minimum_support_threshold,
         const pattern_mining_result<Rule<act_t>>& candidate, const act_t A, const act_t B, DeclareDataAware &clause,
         std::vector<pattern_mining_result<DeclareDataAware>>& clauses,
         const bool &heur_cs_ab, const bool &heur_cs_ba, const bool &heur_s,
         GetAwareData& data,
         bool lb = false, bool rb = false) {
    auto ntraces = kb.nTraces();
    std::vector<trace_t> allTraces;
    for (trace_t sigma = 0; sigma<ntraces; sigma++) allTraces.emplace_back(sigma);

    size_t expected_support = only_precise_temporal_patterns ?
                              ntraces :
                              minimum_support_threshold;

    bool alles_response = true, alles_precedence = true, alles_altresponse = true, alles_succession_ab = true, alles_succession_ba = true;
    size_t alles_not_precedence = 0, alles_not_response = 0, alles_not_altresponse = 0, alles_not_succession_ab = data.r_lb_violations.cardinality(), alles_not_succession_ba = data.p_lb_violations.cardinality();
//    std::vector<trace_t> removed_traces_from_response;

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
    uint32_t activation_count = 0, not_activated = 0, a_trace_id = a_beginend.first->entry.id.parts.trace_id, a_prev_trace_id = a_trace_id,
    b_trace_id = b_beginend.first->entry.id.parts.trace_id;

    if(a_trace_id != 0) {
        not_activated += a_trace_id;
    }

    while (a_beginend.first != a_beginend.second) {
        if ((!alles_precedence) && (!alles_response) && (!alles_altresponse) && (!alles_succession_ab) && (!alles_succession_ba)) {
            break;
        }

        a_trace_id = a_beginend.first->entry.id.parts.trace_id;
        b_trace_id = b_beginend.first->entry.id.parts.trace_id;

        bool alreadyVisited = (a_trace_id == a_prev_trace_id);
        if (!alreadyVisited) {
            not_activated += (a_trace_id - a_prev_trace_id - 1);
            a_prev_trace_id = a_trace_id;
        }

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

        /* We have As on their own */
        if ((b_beginend.first == b_beginend.second) || (a_trace_id < b_trace_id)) {
            activation_count++;
            // Problem 1)
            // This might be a valid precedence, as nothing is stated
            // to what should happen after the A, but I cannot exploit
            // for a response: therefore, I'm counting it
            decrease_support_X(kb, expected_support, alles_precedence, alles_not_precedence);
            decrease_support_X(kb, expected_support, alles_response, alles_not_response);

            if (lb) {
                data.r_activation_traces.first.add(a_trace_id);
                data.p_activation_traces.second.add(a_trace_id);

                data.r_lb_violations.add(a_trace_id);
                data.p_lb_violations.add(a_trace_id);
            }
            else if (rb) {
                data.r_activation_traces.second.add(a_trace_id);
                data.p_activation_traces.first.add(a_trace_id);

                if (!data.p_lb_violations.contains(a_trace_id)) {
                    decrease_support_X(kb, expected_support, alles_succession_ba, alles_not_succession_ba);
                }
            }

            // Now, skipping to the next trace, as there is no more information for as
            fast_forward_equals(a_trace_id, a_beginend.first, a_beginend.second);
            continue;
        }
        /* We have B's on their own */
        if ((a_beginend.first == a_beginend.second) || a_trace_id > b_trace_id) {
            if(lb) {
                data.r_activation_traces.second.add(a_trace_id);
                data.p_activation_traces.first.add(a_trace_id);
            }
            else if (rb) {
                data.r_activation_traces.first.add(a_trace_id);
                data.p_activation_traces.second.add(a_trace_id);

                if (!data.r_lb_violations.contains(a_trace_id)) {
                    decrease_support_X(kb, expected_support, alles_succession_ab, alles_not_succession_ab);
                }
            }

            // Moving b until I find something related to b. A is kept fixed and not incremented
            fast_forward_lower(a_trace_id, b_beginend.first, b_beginend.second);
            // Not setting the current trace to be visited, as we need to fast-forward B first
            continue;
        }
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

        activation_count++;

        if (lb) {
            data.r_activation_traces.first.add(a_trace_id);
            data.r_activation_traces.second.add(a_trace_id);
            data.p_activation_traces.first.add(a_trace_id);
            data.p_activation_traces.second.add(a_trace_id);
        }

        if ((b_beginend.first != b_beginend.second) && (a_beginend.first != a_beginend.second) && (b_beginend.first->entry.id.parts.event_id >
            a_beginend.first->entry.id.parts.event_id)) {
            decrease_support_X(kb, expected_support, alles_precedence, alles_not_precedence);

            if (lb) {
                data.p_lb_violations.add(a_trace_id);
            }
            if (rb) {
                /* Found a case for Succession(B,A) where Precedence(B,A) is violated (e.g. B) */
                if (!data.r_lb_violations.contains(a_trace_id)) {
                    decrease_support_X(kb, expected_support, alles_succession_ba, alles_not_succession_ba);
                }
            }
        }

        // While I'm scanning the A events within the same trace
//        all_altresponse_in_trace = true;
        while ((a_beginend.first != a_beginend.second) &&
               (a_beginend.first->entry.id.parts.trace_id == a_trace_id)) {
            // ignoring all of the B events that are not relevant for the task!
            while ((b_beginend.first != b_beginend.second) &&
                   (b_beginend.first->entry.id.parts.trace_id == a_trace_id) &&
                   (b_beginend.first->entry.id.parts.event_id <
                    a_beginend.first->entry.id.parts.event_id)) {
                b_beginend.first++;
            }
            if ((b_beginend.first != b_beginend.second) &&
                (b_beginend.first->entry.id.parts.trace_id == a_trace_id) &&
                (b_beginend.first->entry.id.parts.event_id >
                 a_beginend.first->entry.id.parts.event_id)) {
                // Ok, I have a match!
            }
            else {
                // If there is no match for the B event, then I'm setting this to false
                // and quitting the iteration
//                    removed_traces_from_response.emplace_back(trace_id);
                decrease_support_X(kb, expected_support, alles_response, alles_not_response);

                if (lb) {
                    data.r_lb_violations.add(a_trace_id);
                }
                if (rb && !data.p_lb_violations.contains(a_trace_id)) {
                    /* Found a case for Succession(B,A) where Response(B,A) is violated (e.g. B) */
                    decrease_support_X(kb, expected_support, alles_succession_ba, alles_not_succession_ba);
                }

                break;
            }
//                        {
//                            if ((tmp != a_beginend.second) &&
//                                (tmp->entry.id.parts.event_id <
//                                 b_beginend.first->entry.id.parts.event_id)) {
//                                all_altresponse_in_trace = false;
//                            }
//                        }

//                                a_beginend.first++;
            a_beginend.first++;
        }
//                    if (!all_altresponse_in_trace) {
//                        decrease_support_X(kb, expected_support, alles_altresponse, alles_not_altresponse);
//                    }

        fast_forward_equals(a_trace_id, a_beginend.first, a_beginend.second);
        fast_forward_equals(a_trace_id, b_beginend.first, b_beginend.second);
    }

    if((ntraces - 1) != a_prev_trace_id) {
        not_activated += ((ntraces - 1) - a_prev_trace_id);
    }

    // This is still computed, as it is required for both 1) and 2)
    bool alles_next = true, alles_prev = true, alles_cs_ab = heur_cs_ab, alles_cs_ba = heur_cs_ba, alles_surround = heur_s;
    size_t tolerated_errors = ntraces - expected_support;
    size_t alles_not_next = 0, alles_not_prev = 0, alles_not_cs_ab = data.cr_lb_violations.cardinality(), alles_not_cs_ba = data.cp_lb_violations.cardinality(), alles_not_surround = 0;
    size_t conf_prev_counting = 0, conf_next_counting = 0;

    if(rb) {
        if (alles_cs_ab && ((kb.nTraces() - alles_not_cs_ab) < expected_support)) {
            alles_cs_ab = false;
        }
        if (alles_cs_ba && ((kb.nTraces() - alles_not_cs_ba) < expected_support)) {
            alles_cs_ba = false;
        }
    }

    a_beginend = kb.timed_dataless_exists(A);
    auto start = a_beginend.first;
    a_prev_trace_id = a_beginend.first != a_beginend.second ? a_beginend.first->entry.id.parts.trace_id : -1;
    bool forward_prec = false, forward_resp = false;
    double conf_prev_not_counting = 0;

    while (a_beginend.first != a_beginend.second) {
        if ((!alles_next) && (!alles_prev) && !(alles_cs_ab) && !(alles_cs_ba) && !(alles_surround)) {
            break; // Breaking only if all conditions are never met
        }

        auto trace_id = a_beginend.first->entry.id.parts.trace_id;
        if (trace_id != a_prev_trace_id) {
            forward_prec = forward_resp = false;
            a_prev_trace_id = trace_id;
        }

        if (!forward_prec && ((a_beginend.first->next == nullptr) || (a_beginend.first->next->entry.id.parts.act != B))) {
            decrease_support_X(kb, expected_support, alles_next, alles_not_next);
            if (lb) {
                data.cr_lb_violations.add(a_beginend.first->entry.id.parts.trace_id);
            }
            if (rb && !data.cp_lb_violations.contains(a_beginend.first->entry.id.parts.trace_id)) {
                /* We've found a case where ChainPrecedence(A,B) wasn't violated but ChainResponse(B,A) is (e.g. {B,B}),
                 * so reduce support of ChainSuccession(B,A) */
                decrease_support_X(kb, expected_support, alles_cs_ba, alles_not_cs_ba);
            }
            if (alles_surround) {
                decrease_support_X(kb, expected_support, alles_surround, alles_not_surround);
            }
            forward_prec = true;
        }

        if (!forward_resp && ((a_beginend.first->prev != nullptr) && (a_beginend.first->prev->entry.id.parts.act != B))) {
            decrease_support_X(kb, expected_support, alles_prev, alles_not_prev);
            if (lb) {
                data.cp_lb_violations.add(a_beginend.first->entry.id.parts.trace_id);
            }
            if (rb && !data.cr_lb_violations.contains(a_beginend.first->entry.id.parts.trace_id)) {
                /* We've found a case where ChainResponse(A,B) wasn't violated but ChainPrecedence(B,A) is (e.g. {B,B}),
                 * so reduce support of ChainSuccession(A,B) */
                decrease_support_X(kb, expected_support, alles_cs_ab, alles_not_cs_ab);
            }
            if (alles_surround) {
                decrease_support_X(kb, expected_support, alles_surround, alles_not_surround);
            }
            forward_resp = true;
        }

        if ((a_beginend.first == start) || (a_beginend.first - 1)->entry.id.parts.trace_id != trace_id) {
            conf_next_counting++;
            lb ? data.cr_activation_traces.first.add(trace_id) : data.cr_activation_traces.second.add(trace_id);
            if ((a_beginend.first->prev != nullptr) || (kb.getCountTable().resolve_length(A, trace_id) > 1)) {
                conf_prev_counting++;
                lb ? data.cp_activation_traces.first.add(trace_id) : data.cp_activation_traces.second.add(trace_id);
            }
            else if ((a_beginend.first->prev == nullptr) && (kb.getCountTable().resolve_length(A, trace_id) == 1)) {
                conf_prev_not_counting++;
            }
        }

        if (forward_prec && forward_resp) {
            fast_forward_equals(trace_id, a_beginend.first, a_beginend.second);
        }
        else {
            a_beginend.first++;
        }
    }

    /* We may be on the right branch but there are no suitable patterns here, so just add the cached ones */
    if (rb && !alles_response && !alles_precedence && !alles_next && !alles_prev) {
        std::swap(clause.left_act, clause.right_act);
        std::swap(clause.left_act_id, clause.right_act_id);
        if(data.p_lb_sup_conf.second != -1) {
            clause.casusu = "Precedence";
            clauses.emplace_back(clause,
                                 candidate.support_generating_original_pattern,
                                 data.p_lb_sup_conf.first,
                                 data.p_lb_sup_conf.second);
        }
        if(data.r_lb_sup_conf.second != -1) {
            clause.casusu = "Response";
            clauses.emplace_back(clause,
                                 candidate.support_generating_original_pattern,
                                 data.r_lb_sup_conf.first,
                                 data.r_lb_sup_conf.second);
        }
        if(data.cp_lb_sup_conf.second != -1) {
            clause.casusu = "ChainPrecedence";
            clauses.emplace_back(clause,
                                 candidate.support_generating_original_pattern,
                                 data.cp_lb_sup_conf.first,
                                 data.cp_lb_sup_conf.second);
        }
        if(data.cr_lb_sup_conf.second != -1) {
            clause.casusu = "ChainResponse";
            clauses.emplace_back(clause,
                                 candidate.support_generating_original_pattern,
                                 data.cr_lb_sup_conf.first,
                                 data.cr_lb_sup_conf.second);
        }

        return;
    }

    /* Declaring outside for now, need global for the surround */
    double cr_sup = -1;

    if (alles_response || alles_precedence) {
        const uint32_t p_satisfied = ntraces - alles_not_precedence;
        const uint32_t p_satisfied_not_vacuous = p_satisfied - not_activated;
        const double p_sup = ((double) p_satisfied) / ((double) ntraces);

        DEBUG_ASSERT(!(alles_next && !alles_response));
        if (alles_response) {
            const uint32_t r_satisfied = (ntraces - alles_not_response);
            const uint32_t r_satisfied_not_vacuous = r_satisfied - not_activated;
            const double r_sup = ((double) r_satisfied) / ((double) ntraces);

            const uint32_t cr_satisfied = (ntraces - alles_not_next);
            const uint32_t cr_satisfied_not_vacuous = cr_satisfied - not_activated;
            cr_sup = ((double) cr_satisfied) / ((double) ntraces);

            if (alles_next && (cr_sup >= r_sup) && (cr_satisfied_not_vacuous > 0) && (conf_next_counting > 0)) {
                data.flags |= rb ? CHAIN_RESPONSE_BA_ID : CHAIN_RESPONSE_AB_ID;
                clause.casusu = "ChainResponse";
                const double cr_conf = ((double) cr_satisfied_not_vacuous) / ((double) conf_next_counting);
                if(!rb && !lb) {
                    clauses.emplace_back(clause,
                                         candidate.support_generating_original_pattern,
                                         cr_sup,
                                         cr_conf);
                }
                else if (lb) {
                    /* If we are on the left branch, there may be a ChainSucession only detectable on the other
                     * so cache the confidence for now */
                    data.cr_lb_sup_conf.first = cr_sup;
                    data.cr_lb_sup_conf.second = cr_conf;
                }
                else {
                    /* We know that in the first branch ChainResponse(A,B) was added and here have ChainPrecedence(B,A) */
                    const uint32_t cs_ba_activated = (data.cr_activation_traces.second | data.cp_activation_traces.first).cardinality();
                    const uint32_t cs_ba_satisfied = (ntraces - alles_not_cs_ba);
                    const uint32_t cs_ba_satisfied_not_vacuous = cs_ba_satisfied - (ntraces - cs_ba_activated);
                    const double cs_ba_sup = ((double) cs_ba_satisfied) / ((double) ntraces);

                    if (alles_cs_ba && (cs_ba_sup >= cr_sup) && (cs_ba_sup >= data.cp_lb_sup_conf.first) && (cs_ba_satisfied_not_vacuous > 0) && (cs_ba_activated > 0)) {
                        clause.casusu = "ChainSuccession";
                        const double cs_ba_conf = ((double) cs_ba_satisfied_not_vacuous) / ((double) cs_ba_activated);
                        clauses.emplace_back(clause,
                                             candidate.support_generating_original_pattern,
                                             cs_ba_sup,
                                             cs_ba_conf);
                    }
                    /* If ChainPrecedence(A,B) and ChainResponse(A,B) from left branch were enough to form a Surround, then don't re-add */
                    else {
                        /* ChainSuccession(A,B) not good enough, add the current ChainResponse(B,A) and ChainPrecedence(A,B) from the left branch */
                        clauses.emplace_back(clause,
                                             candidate.support_generating_original_pattern,
                                             cr_sup,
                                             cr_conf);

                        /* ChainPrecedence(A,B) has already been consumed by Surround(A,B), so don't add */
                        if (((data.flags & SURROUND_AB_ID) != SURROUND_AB_ID) && ((data.flags & CHAIN_PRECEDENCE_AB_ID) == CHAIN_PRECEDENCE_AB_ID) && (data.cp_lb_sup_conf.second != -1)) {
                            clause.casusu = "ChainPrecedence";
                            pattern_mining_result<DeclareDataAware>& ref = clauses.emplace_back(clause,
                                                                                                candidate.support_generating_original_pattern,
                                                                                                data.cp_lb_sup_conf.first,
                                                                                                data.cp_lb_sup_conf.second);
                            std::swap(ref.clause.left_act, ref.clause.right_act);
                            std::swap(ref.clause.left_act_id, ref.clause.right_act_id);
                        }
                    }
                }
                DEBUG_ASSERT(!((data.flags & CHAIN_RESPONSE_AB_ID) && (data.flags & RESPONSE_AB_ID)));
            }
            else if ((r_satisfied_not_vacuous > 0) && (activation_count > 0)) {
                clause.casusu = "Response";
                data.flags |= rb ? RESPONSE_BA_ID : RESPONSE_AB_ID;

                DEBUG_ASSERT(!((data.flags & RESPONSE_AB_ID) && (data.flags & CHAIN_RESPONSE_AB_ID)));
                DEBUG_ASSERT(!((data.flags & RESPONSE_BA_ID) && (data.flags & CHAIN_RESPONSE_BA_ID)));

                double r_conf = ((double) r_satisfied_not_vacuous) / ((double) activation_count);
                if(!rb && !lb) {
                    clauses.emplace_back(clause,
                                         candidate.support_generating_original_pattern,
                                         r_sup,
                                         r_conf);
                }
                else if (lb) {
                    /* If we are on the left branch, there may be a Succession only detectable on the other
                        * so cache the confidence for now */
                    data.r_lb_sup_conf.first = r_sup;
                    data.r_lb_sup_conf.second = r_conf;
                }
                else {
                    const uint32_t s_ba_activated = (data.p_activation_traces.first | data.r_activation_traces.second).cardinality();
                    const uint32_t s_ba_satisfied = (ntraces - alles_not_succession_ba);
                    const uint32_t s_ba_satisfied_not_vacuous = s_ba_satisfied - (ntraces - s_ba_activated);
                    const double s_ba_sup = ((double) s_ba_satisfied) / ((double) ntraces);

                    if (alles_succession_ba && (s_ba_sup >= data.p_lb_sup_conf.first) && (s_ba_sup >= r_sup) && (s_ba_satisfied_not_vacuous > 0) && (s_ba_activated > 0)) {
                        clause.casusu = "Succession";
                        const double s_ba_conf =  ((double) s_ba_satisfied_not_vacuous) / ((double) s_ba_activated);
                        clauses.emplace_back(clause,
                                             candidate.support_generating_original_pattern,
                                             s_ba_sup,
                                             s_ba_conf);
                    }
                    else {
                        /* Succession(B,A) not good enough, add the current Response(B,A) and Precedence(B,A) from the left branch */
                        clauses.emplace_back(clause,
                                             candidate.support_generating_original_pattern,
                                             r_sup,
                                             r_conf);

                        if(data.p_lb_sup_conf.second != -1) {
                            clause.casusu = "Precedence";
                            clauses.emplace_back(clause,
                                                 candidate.support_generating_original_pattern,
                                                 data.p_lb_sup_conf.first,
                                                 data.p_lb_sup_conf.second);
                        }
                    }
                }
            }
        }
        if (alles_precedence && (!rb || ((data.flags & SUCCESSION_BA_ID) != SUCCESSION_BA_ID)) && (p_satisfied_not_vacuous > 0) && (activation_count > 0)) {
            clause.casusu = "Precedence";
            std::swap(clause.left_act, clause.right_act);
            std::swap(clause.left_act_id, clause.right_act_id);
            data.flags |= rb ? PRECEDENCE_AB_ID : PRECEDENCE_BA_ID;
            const double p_conf = ((double) p_satisfied_not_vacuous) / ((double) activation_count);

            if(!rb && !lb) {
                clauses.emplace_back(clause,
                                     candidate.support_generating_original_pattern,
                                     p_sup,
                                     p_conf);
            }
            else if (lb) {
                /* If we are on the left branch, there may be a Succession only detectable on the other
                    * so cache the confidence for now */
                data.p_lb_sup_conf.first = p_sup;
                data.p_lb_sup_conf.second = p_conf;
            }
            else {
                const uint32_t s_ab_activated = (data.r_activation_traces.first | data.p_activation_traces.second).cardinality();
                const uint32_t s_ab_satisfied = (ntraces - alles_not_succession_ab);
                const uint32_t s_ab_satisfied_not_vacuous = s_ab_satisfied - (ntraces - s_ab_activated);
                const double s_ab_sup = ((double) s_ab_satisfied) / ((double) ntraces);


                if (alles_succession_ab && (s_ab_sup >= data.r_lb_sup_conf.first) && (s_ab_sup >= p_sup) && (s_ab_satisfied_not_vacuous > 0) && (s_ab_activated > 0)) {
                    clause.casusu = "Succession";
                    const double s_ab_conf =  ((double) s_ab_satisfied_not_vacuous) / ((double) s_ab_activated);
                    clauses.emplace_back(clause,
                                         candidate.support_generating_original_pattern,
                                         s_ab_sup,
                                         s_ab_conf);
                }
                else {
                    /* Succession(A,B) not good enough, add the current Precedence(A,B) and Response(A,B) from the left branch */
                    clauses.emplace_back(clause,
                                         candidate.support_generating_original_pattern,
                                         p_sup,
                                         p_conf);

                    if (data.r_lb_sup_conf.second != -1) {
                        clause.casusu = "Response";
                        clauses.emplace_back(clause,
                                             candidate.support_generating_original_pattern,
                                             data.r_lb_sup_conf.first,
                                             data.r_lb_sup_conf.second);
                    }
                }
            }

            std::swap(clause.left_act_id, clause.right_act_id);
            std::swap(clause.left_act, clause.right_act);
        }
    }

    /* [[[IMPORTANT]]] Precedence(A,B) vs ChainPrecedence(B,A) have different semantics, so we have to check separately,
     * i.e. we can return Precedence(A,B) AND ChainPrecedence(B,A) as the latter DOES NOT consume the former
     *
     * Precedence(A,B) + Precedence(B,A) -> INVALID (with non-zero confidence)
     * Precedence(A,B) + ChainPrecedence(A,B) ->  VALID e.g. {ABA} due to the X(A) on the first element
     * Precedence(A,B) + ChainPrecedence(B,A) -> VALID but NOT consuming e.g. {B} satisfies the latter but not the former
     * ChainPrecedence(A,B) + ChainPrecedence(B,A) -> VALID e.g. {A}
     *
     * Therefore, it is entirely possible to have a model with ChainSuccession(A,B), Surround(A,B) and Precedence(A,B) */
    const uint32_t cp_satisfied = (ntraces - alles_not_prev);
    const uint32_t cp_satisfied_not_vacuous = cp_satisfied - (not_activated + conf_prev_not_counting);

    if (alles_prev && (cp_satisfied_not_vacuous > 0) && (conf_prev_counting > 0)) {
        data.flags |= rb ? CHAIN_PRECEDENCE_BA_ID : CHAIN_PRECEDENCE_AB_ID;
        const double cp_sup = ((double) cp_satisfied) / ((double) ntraces);
        const double cp_conf = ((double) cp_satisfied_not_vacuous) / ((double) conf_prev_counting);
        clause.casusu = "ChainPrecedence";

        if (alles_surround) {
            const uint32_t sr_satisfied = (ntraces - alles_not_surround);
            const double sr_sup = ((double) sr_satisfied) / ((double) ntraces);

            const uint32_t sr_activated = !rb ? (data.cr_activation_traces.first | data.cp_activation_traces.second).cardinality() : (data.cr_activation_traces.second | data.cp_activation_traces.first).cardinality();
            const uint32_t sr_satisfied_not_vacuous = sr_satisfied - (ntraces - sr_activated);

            if((sr_sup >= cr_sup) && (sr_sup >= cp_sup) && (sr_satisfied_not_vacuous > 0) && (sr_activated > 0)) {
                clause.casusu = "Surround";
                const double sr_conf =  ((double) sr_satisfied_not_vacuous) / ((double) sr_activated);
                clauses.emplace_back(clause,
                                     candidate.support_generating_original_pattern,
                                     sr_sup,
                                     sr_conf);
            }
        }
        if(!rb && !lb) {
            clauses.emplace_back(clause,
                                 candidate.support_generating_original_pattern,
                                 cp_sup,
                                 cp_conf);
        }
        else if (lb) {
                /* If we are on the left branch, there may be a ChainSuccession only detectable on the other
                    * so cache the confidence for now */
                data.cp_lb_sup_conf.first = cp_sup;
                data.cp_lb_sup_conf.second = cp_conf;
        }
        else {
            /* We know that in the first branch ChainResponse(A,B) was added and here have ChainPrecedence(B,A) */
            const uint32_t cs_ab_satisfied = (ntraces - alles_not_cs_ab);
            const double cs_ab_sup = ((double) cs_ab_satisfied) / ((double) ntraces);

            const uint32_t cs_ab_activated = (data.cr_activation_traces.first | data.cp_activation_traces.second).cardinality();
            const uint32_t cs_ab_satisfied_not_vacuous = cs_ab_satisfied - (ntraces - cs_ab_activated);
            if (alles_cs_ab && (cs_ab_sup >= data.cr_lb_sup_conf.first) && (cs_ab_sup >= cp_sup) && (cs_ab_satisfied_not_vacuous > 0) && (cs_ab_activated > 0)) {
                clause.casusu = "ChainSuccession";
                const double cs_ab_conf = ((double) cs_ab_satisfied_not_vacuous) / ((double) cs_ab_activated);
                pattern_mining_result<DeclareDataAware>& ref = clauses.emplace_back(clause,
                                                                                    candidate.support_generating_original_pattern,
                                                                                    cs_ab_sup,
                                                                                    cs_ab_conf);
                std::swap(ref.clause.left_act, ref.clause.right_act);
                std::swap(ref.clause.left_act_id, ref.clause.right_act_id);
            }
            else {
                /* ChainSuccession(A,B) not good enough, add the current ChainPrecedence(B,A) and ChainResponse(A,B) from the left branch */
                clauses.emplace_back(clause,
                                     candidate.support_generating_original_pattern,
                                     cp_sup,
                                     cp_conf);

                /* ChainResponse(A,B) has already been consumed by Surround(A,B), so don't add */
                if (((data.flags & SURROUND_AB_ID) != SURROUND_AB_ID) && ((data.flags & CHAIN_RESPONSE_AB_ID) == CHAIN_RESPONSE_AB_ID) && (data.cr_lb_sup_conf.second != -1)) {
                    clause.casusu = "ChainResponse";
                    pattern_mining_result<DeclareDataAware>& ref = clauses.emplace_back(clause,
                                                                                        candidate.support_generating_original_pattern,
                                         data.cr_lb_sup_conf.first,
                                         data.cr_lb_sup_conf.second);
                    std::swap(ref.clause.left_act, ref.clause.right_act);
                    std::swap(ref.clause.left_act_id, ref.clause.right_act_id);
                }
            }
        }
    }

    return;

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
#include <roaring.hh>

struct retain_choice {
    roaring::Roaring map;
    std::map<double, std::vector<pattern_mining_result<DeclareDataAware>>> maps;
};


static inline bool
choice_exclchoice(act_t a, act_t b,
                                     size_t log_size,
                                     uint64_t minimum_support_threshold,
                                     std::pair<act_t, act_t>& curr_pair,
                                     std::pair<act_t, act_t>& inv_pair,
                                     const KnowledgeBase& kb,
                                     std::unordered_set<std::pair<act_t, act_t>>& visited_pairs,
                                     std::vector<std::vector<trace_t>>& inv_map,
                                     std::unordered_map<act_t, retain_choice>& map_for_retain,
                                     std::unordered_map<std::unordered_set<act_t>, uint64_t>& mapper,
                                     bool unary = true) {
    const std::unordered_set<act_t> lS{a,b};
    curr_pair.second = inv_pair.first = b;
    if (unary && ((!visited_pairs.emplace(curr_pair).second) ||
        (!visited_pairs.emplace(inv_pair).second))) return false;
    const auto& aSet = inv_map.at(a);
    const auto& bSet = inv_map.at(b);
    std::pair<size_t, size_t> ratio = yaucl::iterators::ratio(aSet.begin(), aSet.end(), bSet.begin(), bSet.end());
    double local_support = ((double)(ratio.first)) / ((double)log_size);
    DeclareDataAware clause;
    clause.left_act = kb.event_label_mapper.get(a);
    clause.left_act_id = a;
    clause.right_act = kb.event_label_mapper.get(b);
    clause.right_act_id = b;
    if (a>b) {
        std::swap(clause.left_act,clause.right_act);
        std::swap(clause.left_act_id, clause.right_act_id);
    }
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
        auto& refA = map_for_retain[a];
        auto& refB = map_for_retain[b];
        refA.map.add(b);
        refB.map.add(a);
        pattern_mining_result<DeclareDataAware> c(clause, (it != mapper.end()) ? ((double)it->second)/((double)log_size) : 0.0, local_support, local_support);
        refA.maps[local_support].emplace_back(c);
        refB.maps[local_support].emplace_back(c);
        return true;
    }

    return false;
}

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
    std::vector<size_t> count_beginnings(max_act_id, 0);
    size_t curr_trace_id = 0;
    for (const auto& cp : kb.act_table_by_act_id.secondary_index) {
        if (kb.getCountTable().resolve_length(cp.first->entry.id.parts.act, curr_trace_id) == 1) {
            count_beginnings[cp.first->entry.id.parts.act]++;
        }
        curr_trace_id++;
    }
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
        for (const auto& y : x.second) absent_acts.erase(y);
        if (x.second.size() == 1) {
            auto it = *x.second.begin();
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
                    clause.left_act_id = it;
                    clause.right_act_id = -1;
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
                    declarative_clauses.emplace_back(clause,
                                                     ((double)x.first)/((double)kb.nTraces()),
                                                     ((double)x.first)/((double)kb.nTraces()),
                                                     ((double)x.first)/((double)kb.nTraces()));
                    clause.n = N+1;
                    clause.casusu = "Absence";
                    declarative_clauses.emplace_back(clause,
                                                     ((double)x.first)/((double)kb.nTraces()),
                                                     ((double)x.first)/((double)kb.nTraces()),
                                                     ((double)x.first)/((double)kb.nTraces()));
                }
            } else {
                // If the support is less than one, then we cannot state that
                // all of these events actually exist in all of the possible traces, and
                // we need to relax such conditions into a choice between the patterns,
                // as this will maximise the score, precision-wise.
                // We are postponing such discussion into point A)

                unary_patterns_for_non_exact_support.insert(it);
                doInitA = true;
            }
        } else {
            DEBUG_ASSERT(x.second.size() == 2);
//#ifdef DEBUG
//            std::cout <<  kb.event_label_mapper.get(*x.second.begin()) << "." << kb.event_label_mapper.get(*(++x.second.begin())) << std::endl;
//#endif
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
        for (act_t act_id = 0; act_id<max_act_id; act_id++) {
            for (size_t trace_id = 0; trace_id < log_size; trace_id++) {
                event_t count = count_table.resolve_length(act_id, trace_id);
                if (count > 0) {
                    map[trace_id].emplace_back(act_id);
                    inv_map[act_id].emplace_back(trace_id);
                }
            }
        }
    }
//    unary_patterns_for_non_exact_support.clear();
    std::unordered_map<act_t, retain_choice> map_for_retain;

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
//                        std::unordered_set<act_t> lS{a,b};
                        choice_exclchoice( a,  b,
                                 log_size,
                                 minimum_support_threshold,
                                 curr_pair,
                                 inv_pair,
                                 kb,
                                 visited_pairs,
                                 inv_map,
                                 map_for_retain,
                                 mapper);
//                        curr_pair.second = inv_pair.first = b;
//                        if ((!visited_pairs.emplace(curr_pair).second) ||
//                            (!visited_pairs.emplace(inv_pair).second)) continue;
//                        const auto& aSet = inv_map.at(a);
//                        const auto& bSet = inv_map.at(b);
//                        std::pair<size_t, size_t> ratio = yaucl::iterators::ratio(aSet.begin(), aSet.end(), bSet.begin(), bSet.end());
//                        double local_support = ((double)(ratio.first)) / ((double)log_size);
//                        DeclareDataAware clause;
//                        clause.left_act = kb.event_label_mapper.get(a);
//                        clause.right_act = kb.event_label_mapper.get(b);
//                        if (a>b)
//                            std::swap(clause.left_act,clause.right_act);
//                        clause.n = 1;
//                        if (ratio.first >= minimum_support_threshold) {
//                            // I can consider this pattern, again, only if it is within the expected
//                            // support rate which, in this case, is given by the amount of traces
//                            // globally setting up this pattern
//                            auto it = mapper.find(lS);
//                            if (ratio.second == 0) {
//                                // If there is no intersection, I can also be more strict if I want,
//                                // and provide an exclusive choice pattern if I am confident that
//                                // the two events will never appear in the same trace (according to
//                                // the "training" data
//                                clause.casusu = "ExclChoice";
//                            } else {
//                                clause.casusu = "Choice";
//                            }
//                            auto& refA = map_for_retain[a];
//                            auto& refB = map_for_retain[b];
//                            refA.map.add(b);
//                            refB.map.add(a);
//                            pattern_mining_result<DeclareDataAware> c(clause, (it != mapper.end()) ? ((double)it->second)/((double)log_size) : 0.0, local_support, -1);
//                            refA.maps[((double)it->second)/((double)log_size)].emplace_back(c);
//                            refB.maps[((double)it->second)/((double)log_size)].emplace_back(c);
//                        }

                    }
                }
            }
        }
    }
    map.clear();
//    inv_map.clear();
    for (auto& [act_id, ref_act] : map_for_retain) {
        auto it = ref_act.maps.find(1.0);
        if (it == ref_act.maps.end()) {
            for (auto i = 0; i<max_act_id; i++) {
                if ((act_id != i) && !unary_patterns_for_non_exact_support.contains(i)) {
                    // Inserting the new patterns only if the support is 100%
                    choice_exclchoice( act_id,  i,
                                       log_size,
                                       log_size,
                                       curr_pair,
                                       inv_pair,
                                       kb,
                                       visited_pairs,
                                       inv_map,
                                       map_for_retain,
                                       mapper);
                }
            }
        }
    }
//    map_for_retain.clear();

    DataMiningMetrics counter{count_table};
//    std::cout << "Pattern generation: " << std::endl;
    for (const Pattern& pattern : binary_patterns) {
//#ifdef DEBUG
//        std::unordered_set<std::string> SSSS;
//        auto cp = it;
//        SSSS.insert(kb.event_label_mapper.get(*it));
//        SSSS.insert(kb.event_label_mapper.get(*(++cp)));
//        if (SSSS.contains("g") && SSSS.contains("f")) {
//            std::cout <<"HERE"<< std::endl;
//        }
//#endif
//        std::cout << " - Pattern: " << kb.event_label_mapper.get(*it) << ",";
//        std::cout <<kb.event_label_mapper.get(*it) << std::endl << std::endl;

        DEBUG_ASSERT(pattern.first.size() == 2);
        Rule<act_t> lr, rl;     // A -> B, B -> A
        auto it = pattern.first.begin();

        lr.head.emplace_back(*it);
        rl.tail.emplace_back(*it);
        it++;
        lr.tail.emplace_back(*it);
        rl.head.emplace_back(*it);

        double lr_conf = counter.confidence(lr);
        double rl_conf = counter.confidence(rl);
//        std::cout << "   conf: " << counter.lift(lr) << " conf: " << counter.lift(rl) << std::endl;
//        std::cout << "   lift: " << counter.lift(lr) << " lift: " << counter.lift(rl) << std::endl;
        pattern_mining_result<Rule<act_t>> candidate_rule;

        if ((lr_conf == rl_conf) && (lr_conf >= support)) {
            candidate_rule = pattern_mining_result(Rule<act_t>{pattern.first},
                                                   ((double) pattern.second) / ((double) log_size), counter.decl_support(lr), lr_conf
                                                   );
        } else if (lr_conf >= rl_conf && lr_conf >= support) {
            candidate_rule = pattern_mining_result(lr, ((double) pattern.second) / ((double) log_size),
                                                   counter.decl_support(lr), lr_conf);
        } else if (rl_conf >= support) {
            candidate_rule = pattern_mining_result(rl, ((double) pattern.second) / ((double) log_size),
                                                   counter.decl_support(rl), rl_conf);
        }

        // Generate the hypotheses containing a lift greater than one
        act_t A;
        act_t B;
        DeclareDataAware clause;
        clause.n = 1;
        double dss = candidate_rule.support_declarative_pattern;
        size_t countOk = std::ceil(dss * kb.nTraces());
        if (candidate_rule.clause.tail.empty()) {
            // CoExistence pattern
            A = candidate_rule.clause.head.at(0);
            B = candidate_rule.clause.head.at(1);
            if (A > B) std::swap(A, B);
            clause.casusu = "CoExistence";
            auto cpA = kb.getCountTable().resolve_primary_index2(A);
            auto cpB = kb.getCountTable().resolve_primary_index2(B);
            countOk = pattern.second;
            /*while ((cpA.first != cpA.second) && (cpB.first != cpB.second)) {
                if ((cpA.first->id.parts.event_id > 0) && ) countOk++;
                cpA.first++;
                cpB.first++;
            }*/
            dss = ((double) countOk) / ((double) log_size);
        } else if (candidate_rule.clause.tail.size() == 1) {
            A = candidate_rule.clause.head.at(0);
            B = candidate_rule.clause.tail.at(0);
            // <>A -> <>B
            clause.casusu = "RespExistence";
        };
        clause.left_act = kb.event_label_mapper.get(A);
        clause.left_act_id = A;
        clause.right_act = kb.event_label_mapper.get(B);
        clause.right_act_id = B;

        size_t prev = declarative_clauses.size();
//            if (alsoFlip && ((clause.left_act == "i" || clause.left_act == "h"))
//                         && ((clause.right_act == "i" || clause.right_act == "h"))) {
//                std::cout << counter.support(lr) << " vs. " << counter.support(rl) << std::endl;
//                std::cout << "DEBUG" << std::endl;
//
//            }
        bool alles_chain_succession_ab = true, alles_chain_succession_ba = true, alles_surround_ab = true, alles_surround_ba = true;
        size_t alles_not_chain_succession_ab = 0, alles_not_chain_succession_ba = 0, alles_not_surround_ab = 0, alles_not_surround_ba = 0;

        if (special_temporal_patterns) {
            uint64_t min_sup = std::max(countOk, minimum_support_threshold);
            size_t expected_support = only_precise_temporal_patterns ?
                                      kb.nTraces() :
                                      minimum_support_threshold;
            // 1) First kind of specialized patterns:
            // events happening immediately previously and next:
            // This might leverage the prev/next pointers!
            /* ChainSuccession Heuristic: remove a b for every triplet sharing a b, and remove a b if the first act is a */
            for (auto sigma = 0; sigma < kb.nTraces(); sigma++) {
                auto lA = count_table.resolve_length(A, sigma);
                auto lB = count_table.resolve_length(B, sigma);

                if (!alles_chain_succession_ab && !alles_surround_ab && !alles_chain_succession_ba &&
                    !alles_surround_ba) {
                    break;
                }

                /* We need to check lA is greater than 0 because it is the activation we are interested in */
                if (lA && (alles_chain_succession_ab || alles_surround_ab)) {
                    size_t cs_coarsening =
                            kb.act_table_by_act_id.secondary_index.at(sigma).first->entry.id.parts.act == B ? 1 : 0;
                    if (!(lB <= lA <= (lB + cs_coarsening))) {
                        decrease_support_X(kb, expected_support, alles_chain_succession_ab,
                                           alles_not_chain_succession_ab);
                    }
                    if (!(lA <= lB <= (2 * lA))) {
                        decrease_support_X(kb, expected_support, alles_surround_ab, alles_not_surround_ab);
                    }
                }

                if (lB && (alles_chain_succession_ba || alles_surround_ba)) {
                    size_t coarsening =
                            kb.act_table_by_act_id.secondary_index.at(sigma).first->entry.id.parts.act != A ? 0 : 1;
                    if (!(lA <= lB <= (lA + coarsening))) {
                        decrease_support_X(kb, expected_support, alles_chain_succession_ba,
                                           alles_not_chain_succession_ba);
                    }
                    if (!(lB <= lA <= (2 * lB))) {
                        decrease_support_X(kb, expected_support, alles_surround_ba, alles_not_surround_ba);
                    }
                }
            }

            GetAwareData data;
            size_t min_int_supp_patt = std::ceil(((double)log_size) * (candidate_rule.support_declarative_pattern));

            /* We want to force a branch if the Bs ever occur at the start of the trace and occur only once.
             * This is due to ChainPrecedence, which has an activation of X(A), and we want to mine a potential ChainPrecedence(A,B) */
            bool branch = candidate_rule.clause.tail.empty() || (count_beginnings.at(B) > 0);
            getAware(kb, only_precise_temporal_patterns,
                     count_table, min_int_supp_patt, candidate_rule,
                     A, B, clause, declarative_clauses, alles_chain_succession_ab, alles_chain_succession_ba,
                     alles_surround_ab, data, branch);

            if (branch) {
                clause.right_act = kb.event_label_mapper.get(A);
                clause.right_act_id = A;
                clause.left_act = kb.event_label_mapper.get(B);
                clause.left_act_id = B;
                getAware(kb, only_precise_temporal_patterns,
                         count_table, min_int_supp_patt, candidate_rule,
                         B, A, clause, declarative_clauses, alles_chain_succession_ba, alles_chain_succession_ab,
                         alles_surround_ba, data, false, true);
            }
        }

        /* Okay, so we found no candidate further down the lattice with better support, add RespExistence/CoExistence */
        if (declarative_clauses.size() == prev) {
            // If the choice/ExclChoice is not better than Co/Resp-Existence
            size_t min_int_supp_patt = std::ceil(((double)log_size) * std::nextafter(candidate_rule.support_declarative_pattern, INFINITY));
            if ((min_int_supp_patt > log_size) || (!choice_exclchoice(A, B, log_size, min_int_supp_patt,
                                                                       curr_pair, inv_pair, kb, visited_pairs, inv_map, map_for_retain, mapper, false))) {
                {
                    declarative_clauses.emplace_back(clause,
                                                     candidate_rule.support_generating_original_pattern,
                                                     candidate_rule.support_declarative_pattern,
                                                     candidate_rule.confidence_declarative_pattern);
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
            if (first_occ == log_size) {
                clause.casusu = "Init";
                clause.left_act = kb.event_label_mapper.get(act_id);
                clause.left_act_id = act_id;
                clause.right_act_id = -1;
                declarative_clauses.emplace_back(clause, ((double)(first_occ)) / ((double)log_size),
                                                 ((double)(first_occ)) / ((double)log_size),
                                                 ((double)(first_occ)) / ((double)log_size));
            }
            if (last_occ == log_size) {
                clause.casusu = "End";
                clause.left_act = kb.event_label_mapper.get(act_id);
                clause.left_act_id = act_id;
                clause.right_act_id = -1;
                declarative_clauses.emplace_back(clause, ((double)(last_occ)) / ((double)log_size),
                                                 ((double)(last_occ)) / ((double)log_size),
                                                 ((double)(last_occ)) / ((double)log_size));            }
        }
    }

    std::unordered_set<std::pair<std::string,std::string>> considered;
    for (const auto& ref : declarative_clauses) {
        if (!ref.clause.left_act.empty()) {
            const auto& a = ref.clause.left_act;
            const auto& b = ref.clause.right_act;
            considered.emplace(a,b);
            considered.emplace(b,a);
        }
    }
    std::pair<std::string,std::string> cp;
    for (auto& [act_id, ref_act] : map_for_retain) {
        auto it_1 = ref_act.maps.begin();
        auto it_2 = ref_act.maps.find(1.0);
        if(it_2 != ref_act.maps.end()) {
            it_1 = it_2;
            it_2++;
        }
        else {
            it_2 = ref_act.maps.end();
        }

        while(it_1 != it_2) {
            for (const auto& clause : it_1->second) {
                cp.first = clause.clause.left_act;
                cp.second = clause.clause.right_act;
                if (!considered.contains(cp)) {
                    declarative_clauses.emplace_back(clause);
                }
            }
            it_1++;
        }
    }

    std::sort(declarative_clauses.begin(), declarative_clauses.end(), [](const pattern_mining_result<DeclareDataAware>& l, const pattern_mining_result<DeclareDataAware>& r) {
        return std::tie(l.clause.casusu, l.clause.left_act, l.clause.right_act, l.clause.n, l.confidence_declarative_pattern, l.support_declarative_pattern) > std::tie(r.clause.casusu, r.clause.left_act, r.clause.right_act, r.clause.n, r.confidence_declarative_pattern, r.support_declarative_pattern);
    });
    declarative_clauses.erase(std::unique(declarative_clauses.begin(), declarative_clauses.end(), [](const pattern_mining_result<DeclareDataAware>& l, const pattern_mining_result<DeclareDataAware>& r) {
        return std::tie(l.clause.casusu, l.clause.left_act, l.clause.right_act, l.clause.n) == std::tie(r.clause.casusu, r.clause.left_act, r.clause.right_act, r.clause.n);
    }), declarative_clauses.end());
    auto t2 = high_resolution_clock::now();
    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;

    return {declarative_clauses, ms_double.count()};
}


static inline
void extractPayloads(std::unordered_map<std::string, std::unordered_map<std::string, LTLfQuery>>::iterator it2,
                        std::vector<DeclareDataAware> &v_intersection,
                     const std::string& envName,
                        Environment &tmpEnv,
                        std::vector<bool>& activations,
                        std::vector<bool>& targets,
                        std::vector<std::vector<std::pair<ForResultReference, int>>> &a,
                        std::vector<std::vector<std::pair<ForResultReference, int>>> &t,
                        std::vector<std::vector<std::pair<ForResultReference, int>>> &corr,
                        size_t clazz) {
    tmpEnv.clearModel(); // initializing the model pipeline
    std::unordered_map<std::string, LTLfQuery>* plans = &it2->second;
    tmpEnv.conjunctive_model = v_intersection;
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
    marked_event me;
    me.id.parts.type =MARKED_EVENT_ACTIVATION;
    ref.pipeline(&tmpEnv.grounding, tmpEnv.ap, tmpEnv.db);
    for (size_t i = 0, N = ref.declare_to_query.size(); i<N; i++) {
        const auto& clauseRef = v_intersection.at(i);
        size_t leftAct = tmpEnv.db.event_label_mapper.get(clauseRef.left_act);
        size_t rightAct = -1;
        if (!clauseRef.right_act.empty())
            rightAct = tmpEnv.db.event_label_mapper.get(clauseRef.right_act);
        auto& x = ref.declare_to_query.at(i);
        if ((x->t == LTLfQuery::EXISTS_QP || x->t == LTLfQuery::ABSENCE_QP) && (!x->fields.id.parts.is_timed)) {
            auto cpy = x->result;
            const uint16_t act_id = tmpEnv.db.getMappedValueFromAction(ref.data_accessing.at(x->table_query.at(0)).first.label);
            DEBUG_ASSERT(act_id != -1);      // Sam.A. We should never have a case where exists returned a non-yielding result

            // If the clause is untimed, then I have to get as results all the events appearing in the trace of interest
            for (auto& ref2 : cpy) {
                ref2.second.second.clear();     // Sam.A. Let's extract all redundant marked events
                auto it = tmpEnv.db.act_table_by_act_id.resolve_index(act_id);
                while (it.first != (it.second + 1)) {
                    const auto& event_ref = tmpEnv.db.act_table_by_act_id.table.at(it.first);
                    if (ref2.first.first == event_ref.entry.id.parts.trace_id) {
                        me.id.parts.left = event_ref.entry.id.parts.event_id;
                        ref2.second.second.emplace_back(me);
                    }
                    it.first++;
                }
                DEBUG_ASSERT(!ref2.second.second.empty());      // Sam.A. We should never have a case where exists returned a non-yielding result
            }
            extractPayload(leftAct, rightAct, &tmpEnv, envName, activations.at(i), targets.at(i), a[i],t[i],corr[i],cpy,clazz);
        } else {
            extractPayload(leftAct, rightAct, &tmpEnv, envName, activations.at(i), targets.at(i), a[i],t[i],corr[i],x->result,clazz);
        }
#ifdef DEBUG
        std::cerr << *x << std::endl;
#endif
    }
}

#include <cmath>

template <typename T>
void collectRefinedClause(std::vector<std::vector<DeclareDataAware>> &VVV,
//                          std::unordered_map<int, Environment *> &tree_to_env,
                          DeclareDataAware &clause, const DecisionTree<T> &dtA,
                          const RefineOver &what,
                          bool doNegate) {
    std::unordered_map<int, std::vector<std::vector<dt_predicate>>> world_to_paths;
    dtA.populate_children_predicates(world_to_paths, nullptr, doNegate);
    for(auto& pair : world_to_paths){
//        auto ref = tree_to_env.find(pair.first);
//        DEBUG_ASSERT(ref != tree_to_env.end());
        DeclareDataAware c = actualClauseRefine(clause, what, pair);
//#ifdef DEBUG
//        std::cout << pair.first  << " -- " << c << std::endl;
//#endif
        VVV[pair.first].emplace_back(c);
    }
//#ifdef DEBUG
//    std::cout <<"~~~~~~~" <<  std::endl;
//#endif
}

#include <functional>

std::tuple<std::vector<std::vector<DeclareDataAware>>,double,double> classifier_mining(ServerQueryManager& sqm,
                                                                       const std::vector<std::string>& model_entry_names,
                                                                       double support,
                                                                       double tau,
                                                                       double purity,
                                                                       size_t maxL,
                                                                       size_t minL,
                                                                       bool naif,
                                                                       bool init_end,
                                                                       bool special_temporal_patterns,
                                                                       bool only_precise_temporal_patterns,
                                                                       bool negative_ones) {
    std::function<union_minimal(const ForResultReference&, const std::string&)> elementi = [&sqm](const ForResultReference& x, const std::string& key) -> union_minimal {
        auto& kb = sqm.multiple_logs[x.world].db;
        bool isTarget = key.starts_with("T.");
        auto it = kb.attribute_name_to_table.find((key.starts_with("A.")||isTarget ? key.substr(2) : key));
        if (it != kb.attribute_name_to_table.end()) {
            size_t offsetO = 0;
            if (isTarget) offsetO = 1;
            size_t offset = kb.act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(x.result.at(offsetO).first).at(x.result.at(offsetO).second);
            std::optional<union_minimal> data = it->second.resolve_record_if_exists2(offset);
            if(data.has_value()) {
                return data.value();
            } else {
                switch (it->second.type) {
                    case DoubleAtt:
                    case LongAtt:
                    case SizeTAtt:
                    case BoolAtt:
                        return 0.0;
                    case StringAtt:
                        return "";
                }
            }
        }

        return 0.0;
    };

    std::vector<std::vector<DeclareDataAware>> VVV;
    std::vector<DeclareDataAware> last_VVV_intersection, curr_VVV_insersection;
    // TODO: would have been more efficient with a min-heap, but we don't have time.
    double overall_dataless_mining_time = 0;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();
    for (size_t i = 0; i < model_entry_names.size(); i++) {
        const auto &ref = model_entry_names.at(i);
        auto tmp = pattern_mining(sqm.multiple_logs[ref].db, support, naif, init_end, special_temporal_patterns, only_precise_temporal_patterns, negative_ones);
        overall_dataless_mining_time += tmp.second;

        std::ofstream benchmark_file( "/home/sam/Documents/Repositories/Codebases/knobab/data/testing/mining/mined_clauses_0.1_theta.csv", std::ios_base::trunc);
        benchmark_file << "algorithm,clause,support";

        std::ofstream model_file( "/home/sam/Documents/Repositories/Codebases/knobab/data/testing/mining/models/model.powerdecl", std::ios_base::trunc);
        model_file << "declare\n";
        std::ofstream sup_conf_file( "/home/sam/Documents/Repositories/Codebases/knobab/data/testing/mining/models/sup_conf.tab", std::ios_base::trunc);

        bool first = true;
        for (const pattern_mining_result<DeclareDataAware> clause : tmp.first) {
            std::cout << clause;
            std::string name = clause.clause.casusu + "(" + clause.clause.left_act;
            if(clause.clause.right_act != "") {
                name += "+" + clause.clause.right_act + ")";
            }
            else if (clause.clause.casusu == "Exists" || clause.clause.casusu == "Absence") {
                name += " " + std::to_string(clause.clause.n) + ")";
            }
            else {
                name += ")";
            }
            benchmark_file << std::endl << "DBoltk," << name << "," << clause.support_declarative_pattern;
            model_file << (!first ? "\n" : "") << clause.clause;
            sup_conf_file << (!first ? "\n" : "") << clause.support_declarative_pattern << "\t" << clause.confidence_declarative_pattern;

            first = false;
        }

        benchmark_file.close();
        model_file.close();
        sup_conf_file.close();

        auto& WWW = VVV.emplace_back();
        std::vector<size_t> low_d_supp;
        size_t j = 0;
        for (auto& ref2 : tmp.first) {
            WWW.emplace_back(ref2.clause);
            if (ref2.support_declarative_pattern < 1.0) low_d_supp.emplace_back(j);
            j++;
        }

        //TODO JUST FOR THE JOURNAL, DONT REFINE
        return std::make_tuple(VVV, overall_dataless_mining_time, 0);

        remove_duplicates(WWW);

        if (i == 0)
             last_VVV_intersection = WWW;
        else {
            std::set_intersection(WWW.begin(), WWW.end(),
                                  last_VVV_intersection.begin(), last_VVV_intersection.end(),
                                  std::back_inserter(curr_VVV_insersection));
            std::swap(last_VVV_intersection, curr_VVV_insersection);
            curr_VVV_insersection.clear();
        }
        remove_index(WWW, low_d_supp);
    }

    auto it2 = sqm.planname_to_declare_to_ltlf.find("nfmcp23");

    ////////////////////////////////////////////////////////////////////////////
    /// Removing the clauses having neither activation nor target conditions ///
    ////////////////////////////////////////////////////////////////////////////
    std::vector<size_t> index;
    std::unordered_set<std::string> toExclude{"Absence", "CoExistence", "Choice"}; // TODO: dealing correctly with Absence!
    std::vector<bool> hasActivations(last_VVV_intersection.size(), false), hasTargets(last_VVV_intersection.size(), false);
    for (size_t i = 0, M = last_VVV_intersection.size(); i<M; i++) {
        const auto& ref = last_VVV_intersection.at(i);
        if (toExclude.contains(ref.casusu)) // Excluding a clause that I have already met
            index.emplace_back(i); // Removing from the index
        else {
            auto it3 = it2->second.find(ref.casusu);
            if (it3 == it2->second.end()) {
                toExclude.emplace(ref.casusu); // Adding the clause in the group of to the be excluded
                index.emplace_back(i); // Removing from the index
            } else {
                // Getting whether this has either activations or target conditions
                bool hasActivation = false;
                bool hasTarget = false;
                std::stack<const LTLfQuery*> ofPointers;
                ofPointers.push(&it3->second);
                while (!ofPointers.empty()) {
                    auto tmp = ofPointers.top();
                    ofPointers.pop();
                    if (tmp->isLeaf == ActivationLeaf) {
                        hasActivation = true;
                    } else if (tmp->isLeaf == TargetLeaf) {
                        hasTarget = true;
                    }
                    for (const auto& ref2 : tmp->args_from_script) {
                        ofPointers.push(&ref2);
                    }
                }
                if (hasActivation) hasActivations[i] = true;
                if (hasTarget) hasTargets[i] = true;
                if ((!hasActivation) && (!hasTarget)) {
                    toExclude.emplace(ref.casusu); // Adding the clause in the group of to the be excluded
                    index.emplace_back(i); // Removing from the index
                }
            }
        }
    }
    if (!toExclude.empty()) {
        remove_index(last_VVV_intersection, index);
        remove_index(hasActivations, index);
        remove_index(hasTargets, index);
    }
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    if (last_VVV_intersection.empty()) {
        auto t2 = high_resolution_clock::now();
        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;
#ifdef DEBUG
        std::cout << overall_dataless_mining_time << "+" << ms_double.count() << "=" << overall_dataless_mining_time+ms_double.count() << "ms\n";
#endif
        return std::make_tuple(VVV,overall_dataless_mining_time, ms_double.count());
    }

    tau = std::clamp(tau, 0.5, 1.);

    // Removing the elements at the intersection
    std::vector<DeclareDataAware> v3;
    for (auto& ref : VVV) {
        v3.clear();
        std::set_difference(ref.begin(), ref.end(), last_VVV_intersection.begin(), last_VVV_intersection.end(), std::back_inserter(v3));
        std::swap(v3, ref);
    }


    // Attempting at classifying per activation
    std::vector<std::vector<std::pair<ForResultReference, int>>> per_clause_AV(last_VVV_intersection.size()),
                                                           per_clause_TV(last_VVV_intersection.size()),
                                                           per_clause_CV(last_VVV_intersection.size());
    std::unordered_set<std::string> numeric_keys, categorical_keys;
    std::unordered_set<std::string> corr_numeric_keys, corr_categorical_keys;
    for (size_t i = 0; i < model_entry_names.size(); i++) {
        const auto& ref = model_entry_names.at(i);
        for (const auto& [k,v] : sqm.multiple_logs[ref].db.attribute_name_to_table) {
            if (v.type == StringAtt) {
                corr_categorical_keys.insert("A."+k);
                corr_categorical_keys.insert("T."+k);
                categorical_keys.insert(k);
            } else {
                corr_numeric_keys.insert("A."+k);
                corr_numeric_keys.insert("T."+k);
                numeric_keys.insert(k);
            }
        }
        extractPayloads(it2, last_VVV_intersection, ref, sqm.multiple_logs[ref], hasActivations, hasTargets, per_clause_AV, per_clause_TV, per_clause_CV, i);
    }
    numeric_keys.erase("__time");
    categorical_keys.erase("__time");
    corr_numeric_keys.erase("A.__time");
    corr_numeric_keys.erase("T.__time");
    corr_categorical_keys.erase("A.__time");
    corr_categorical_keys.erase("T.__time");

    for (size_t i = 0; i < per_clause_AV.size(); i++) {
        auto& clause = last_VVV_intersection.at(i);
        bool doNegate = it2->second.at(clause.casusu).t == LTLfQuery::ABSENCE_QP;
        // Refining over the activations first
        if (hasActivations.at(i)) {
            auto& V = per_clause_AV[i];
            if (!V.empty()) {
                auto it = V.begin(), en = V.end();
                DecisionTree<ForResultReference> dtA(it,
                                               en,
                                               1,
                                               elementi,
                                               numeric_keys,
                                               categorical_keys,
                                               ForTheWin::gain_measures::Gini,
                                               purity,
                                               maxL,
                                               V.size(),
                                               minL);
                if (dtA.goodness >= tau) {
                    if ((!dtA.isLeafNode())) {
                        collectRefinedClause(VVV, clause, dtA, RefineOverActivation, doNegate);
                        continue;
                    } else {
                        VVV[dtA.getMajorityClass()].emplace_back(clause);
                    }
                }

            }
        }

        if (hasTargets.at(i)) {
            // Refining by target
            auto& W = per_clause_TV[i];
            if (!W.empty()) {
                auto itT = W.begin(), enT = W.end();
                DecisionTree<ForResultReference> dtT(itT,
                                               enT,
                                               1,
                                               elementi,
                                               numeric_keys,
                                               categorical_keys,
                                               ForTheWin::gain_measures::Gini,
                                               purity,
                                               maxL,
                                               W.size(),
                                               minL);
                if (dtT.goodness >= tau) {
                    if ((!dtT.isLeafNode())) {
                        collectRefinedClause(VVV, clause, dtT, RefineOverTarget, doNegate);
                        continue;
                    } else  {
                        VVV[dtT.getMajorityClass()].emplace_back(clause);
                    }
                }
            }
        }

        if (hasActivations.at(i) && hasTargets.at(i)) {
            auto& C = per_clause_CV[i];
            if (!C.empty()) {

                auto itC = C.begin(), enC = C.end();
                DecisionTree<ForResultReference> dtC(itC,
                                               enC,
                                               1,
                                                     elementi,
                                               corr_numeric_keys,
                                               corr_categorical_keys,
                                               ForTheWin::gain_measures::Gini,
                                               purity,
                                               maxL,
                                               C.size(),
                                               minL);
                if (dtC.goodness >= tau) {
                    if ((!dtC.isLeafNode())) {
                        collectRefinedClause(VVV, clause, dtC, RefineOverMatch, doNegate);
                    } else {
                        VVV[dtC.getMajorityClass()].emplace_back(clause);
                    }
                }
            }
        }
    }
    auto t2 = high_resolution_clock::now();
    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t2 - t1;
//#ifdef DEBUG
    std::cout << overall_dataless_mining_time << "+" << ms_double.count() << "=" << overall_dataless_mining_time+ms_double.count() << "ms\n";
//#endif
    return std::make_tuple(VVV,overall_dataless_mining_time, ms_double.count());
}
