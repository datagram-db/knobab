//
// Created by giacomo on 20/02/24.
//

#ifndef KNOBAB_SERVER_POLYADIC_BOLT_H
#define KNOBAB_SERVER_POLYADIC_BOLT_H

#include <algorithm>
#include "knobab/server/tables/KnowledgeBase.h"
#include "knobab/server/declare/DeclareDataAware.h"
#include "knobab/mining/bolt_commons.h"
#include <knobab/mining/polyadic/lattice.h>



using simple_declare = std::pair<std::string, bool>; // true = AB, false = BA
struct declare_lattice_node {
    size_t log_support = 0;
    double rconf = 0;
    bool isVisited = false;
    std::vector<size_t> sat,vac,no_sat;

    declare_lattice_node() {};
    declare_lattice_node(const declare_lattice_node&) = default;
    declare_lattice_node(declare_lattice_node&&) = default;
    declare_lattice_node& operator=(const declare_lattice_node&) = default;
    declare_lattice_node& operator=(declare_lattice_node&&) = default;
    inline void set(size_t log_support, double rconf) {
        this->log_support = log_support;
        this->rconf = rconf;
        isVisited = true;
    }
    void set(const std::vector<size_t>& SAT, const std::vector<size_t>& VAC, const std::vector<size_t>& UNSAT);

    inline void reset() {
        log_support = 0;
        isVisited = false;
        rconf = 0.0;
    }
};

struct QM_DECLARE {
    const lattice<simple_declare, declare_lattice_node>* ptr;
    size_t log_theta;
    explicit QM_DECLARE(const lattice<simple_declare, declare_lattice_node> *ptr, size_t log_theta) : ptr(ptr), log_theta(log_theta) {}

    bool operator()(const simple_declare& key, const struct declare_lattice_node& x ) const;
};

#define TRACE_SET_CONTAINS_INIT(it,m)             auto it = m.begin()
#define TRACE_SET_SIZE(m)                   (m).size()
#define TRACE_SET_ADD(m,a)                  (m).emplace_back(a)
#define TRACE_SET_UNION_CARDINALITY(a,b)        (yaucl::iterators::ratio(a.begin(), a.end(), b.begin(), b.end())).first
#define TRACE_SET_INTERSECTION_CARDINALITY(a,b)        (yaucl::iterators::ratio(a.begin(), a.end(), b.begin(), b.end())).second
using trace_set2 =  std::vector<trace_t>;

struct SimpleDeclare {
    std::string name;
    u_int16_t A, B;
    double support;
    double rconf;

    SimpleDeclare(const std::string &name, u_int16_t a, u_int16_t b, double support, double rconf) : name(name), A(a),
                                                                                                     B(b),
                                                                                                     support(support),
                                                                                                     rconf(rconf) {}
    SimpleDeclare() = default;
    SimpleDeclare(const SimpleDeclare&) = default;
    SimpleDeclare(SimpleDeclare&&) = default;
    SimpleDeclare& operator=(const SimpleDeclare&) = default;
    SimpleDeclare& operator=(SimpleDeclare&&) = default;
};

struct result_container {
    u_int16_t A, B;
    double log_size;
    std::vector<SimpleDeclare> result;

    void operator()(const simple_declare& x, const declare_lattice_node& node) {
        DEBUG_ASSERT((((double)node.log_support)/log_size)<=1);
        if (x.second) {
            result.emplace_back(x.first, A, B, ((double)node.log_support)/log_size, node.rconf);
        } else {
            result.emplace_back(x.first, B, A, ((double)node.log_support)/log_size, node.rconf);
        }
    }
    void complex_operator(const simple_declare& x, const declare_lattice_node& node, const std::string&A, const std::string& B, const size_t total_log, std::unordered_map<std::tuple<std::string,std::string,std::string>, std::vector<char>>& result_map) {
        std::tuple<std::string,std::string,std::string> simple_clause;
        if (x.second) {
            simple_clause = {x.first, A, B};
        } else {
            simple_clause = {x.first, B, A};
        }
        if (result_map.contains(simple_clause))
            exit(3);
        auto& embeddings = result_map[simple_clause];
        embeddings.resize(total_log, -2);

        for (size_t trace : node.sat) {
            embeddings[trace] = 1;
        }
        for (size_t trace : node.no_sat) {
            embeddings[trace] = -1;
        }
        for (size_t trace : node.vac) {
            auto& val = embeddings[trace];
            if ((val == 1) || (val == -2))
                val = 0;
        }
    }
};


struct polyadic_bolt {
    const CountTemplate* count_table{nullptr};
    size_t log_size{0};
    act_t max_act_id{0};
    std::vector<size_t> Beginnings, first, last;
//    FastDatalessClause clause;
    std::vector<pattern_mining_result<FastDatalessClause>> Phi;
    std::vector<act_t> ChoiceFilter;
    std::unordered_map<std::unordered_set<act_t>, uint64_t> map_for_itemset_support_score;
    Rule<act_t> lr, rl;
    DataMiningMetrics counter;
    std::vector<std::vector<trace_t>> inv_map;

    lattice<simple_declare , declare_lattice_node> graph;
    simple_declare choiceAB_BA{"Choice", true};
    simple_declare exclchoiceAB_BA{"ExclChoice", true};
    simple_declare resp_existenceAB{"RespExistence", true};
    simple_declare respAB{"Response", true};
    simple_declare crespAB{"ChainResponse", true};
    simple_declare precBA{"Precedence", false};
    simple_declare cprecAB{"ChainPrecedence", true};
    simple_declare resp_existenceBA{"RespExistence", false};
    simple_declare coexistenceAB_BA{"CoExistence", true};
    simple_declare respBA{"Response", false};
    simple_declare crespBA{"ChainResponse", false};
    simple_declare precAB{"Precedence", true};
    simple_declare succAB{"Succession", true};
    simple_declare succBA{"Succession", false};
    simple_declare cprecBA{"ChainPrecedence", false};
    simple_declare csuccAB{"ChainSuccession", true};
    simple_declare csuccBA{"ChainSuccession", false};

    polyadic_bolt() {
        // ~~ Initialisation of the lattice data structure
        graph.add_node(choiceAB_BA);
        graph.add_node(resp_existenceAB);
        graph.add_edge(choiceAB_BA, resp_existenceAB, true);

        graph.add_node(respAB);
        graph.add_edge(resp_existenceAB, respAB);

        graph.add_node(crespAB);
        graph.add_edge(respAB, crespAB);

        graph.add_node(precBA);
        graph.add_edge(resp_existenceAB, precBA);

        graph.add_node(cprecAB);
        graph.add_edge(choiceAB_BA, cprecAB, true);

        graph.add_node(resp_existenceBA);
        graph.add_edge(choiceAB_BA, resp_existenceBA, true);

        graph.add_node(coexistenceAB_BA);
        graph.add_edge(resp_existenceBA, coexistenceAB_BA, true);
        graph.add_edge(resp_existenceAB, coexistenceAB_BA, true);

        graph.add_node(respBA);
        graph.add_edge(resp_existenceBA, respBA);

        graph.add_node(crespBA);
        graph.add_edge(respBA, crespBA);

        graph.add_node(precAB);
        graph.add_edge(resp_existenceBA, precAB);

        graph.add_node(succAB);
        graph.add_edge(respAB, succAB);
        graph.add_edge(precAB, succAB);

        graph.add_node(succBA);
        graph.add_edge(respBA, succBA);
        graph.add_edge(precBA, succBA);

        graph.add_node(cprecBA);
        graph.add_edge(choiceAB_BA, cprecBA, true);

        graph.add_node(csuccAB);
        graph.add_edge(cprecBA, csuccAB, true);
        graph.add_edge(crespAB, csuccAB, true);

        graph.add_node(csuccBA);
        graph.add_edge(cprecAB, csuccBA, true);
        graph.add_edge(crespBA, csuccBA, true);

        graph.add_node(exclchoiceAB_BA);
    }

    // O: always, *(A,B). 1: always, *(B,A)
    std::array<std::vector<size_t>, 2> act_r, sat_r, viol_r, act_p, sat_p, viol_p, act_cr, sat_cr,  viol_cr, act_cp, sat_cp, viol_cp,  vac_r, vac_p, vac_cr, vac_cp;
    std::vector<size_t> act_cs0, act_cs1, sat_cs0, sat_cs1, viol_cs0, viol_cs1, vac_cs0, vac_cs1,
    act_s0, act_s1, sat_s0, sat_s1, viol_s0, viol_s1, vac_s0, vac_s1;

    void clearResultsVector() {
        act_cs0.clear();
        act_cs1.clear();
        sat_cs0.clear();
        sat_cs1.clear();
        viol_cs0.clear();
        viol_cs1.clear();
        act_s0.clear();
        act_s1.clear();
        sat_s0.clear();
        sat_s1.clear();
        viol_s0.clear();
        viol_s1.clear();
        vac_cs0.clear();
        vac_cs1.clear();
        vac_s0.clear();
        vac_s1.clear();
        for (size_t i = 0; i<2; i++) {
            act_r[i].clear();
            sat_r[i].clear();
            viol_r[i].clear();
            act_p[i].clear();
            sat_p[i].clear();
            viol_p[i].clear();
            act_cr[i].clear();
            sat_cr[i].clear();
            viol_cr[i].clear();
            act_cp[i].clear();
            sat_cp[i].clear();
            viol_cp[i].clear();
            vac_r[i].clear();
            vac_p[i].clear();
            vac_cr[i].clear();
            vac_cp[i].clear();
        }
    }

    std::vector<size_t> event_to_root;

    inline void setKnowledgeBaseAndInit(const KnowledgeBase* ptr) {
        kb = ptr;
        clearResultsVector();
        event_to_root.clear();


        Phi.clear();
        count_table = &kb->getCountTable();
        counter.reset(count_table);

        log_size = kb->nTraces();
        max_act_id = (act_t) kb->nAct();
        event_to_root.clear();
        inv_map.clear();
        Beginnings.clear();
        first.clear();
        last.clear();
        for (int i = 0; i<max_act_id; i++) {
            event_to_root.emplace_back(-1);
            Beginnings.emplace_back(0);
            first.emplace_back(0);
            last.emplace_back(0);
            inv_map.emplace_back();
        }
        for (const auto& [root, children]: ptr->hierarchy_def) {
            for (const auto& child : children) {
                if (kb->event_label_mapper.signed_get(child)!=-1) {
                    DEBUG_ASSERT(kb->event_label_mapper.signed_get(root) != -1);
                    event_to_root[kb->event_label_mapper.get(child)] = kb->event_label_mapper.get(root);
                }
            }
        }

        for (size_t trace_id = 0; trace_id < log_size; trace_id++) {
            const auto& first_last =kb->act_table_by_act_id.secondary_index.at(trace_id);
            for (auto it = first_last.first->begin(), en = first_last.first->end(); it!=en; it++) {
                if (count_table->resolve_length(it->first, trace_id) == 1) {
                    Beginnings[it->first]++;
                }
                first[it->first]++;
            }
            for (auto it = first_last.second->begin(), en = first_last.second->end(); it!=en; it++) {
                last[it->first]++;
            }
        }
        clear();
        graph.reset([](declare_lattice_node& x ) {
            x.reset();
        });
    }

    /**
     * This function takes as an input the mined frequent itemsets, instantiates all the unary clauses such as Init and
     * Ends, exists and absence, and updates such patterns by only retaining the binary ones to be used in the later stages
     *
     */
    inline void generate_unary_clauses(std::vector<std::pair<size_t, std::unordered_set<act_t>>>& frequent_itemset_mining) {
        FastDatalessClause clause;
        std::vector<size_t> indices_to_remove;
        for (size_t i = 0, N = frequent_itemset_mining.size(); i<N; i++) {
            const auto& pattern = frequent_itemset_mining.at(i);
            if (pattern.second.size() == 1) { //Unary patterns
                indices_to_remove.emplace_back(i);
                auto it = *pattern.second.begin();
                if (pattern.first == log_size) {
                    // The non-naif version is exploiting the couting information from
                    // the counting table, and also providing an expected number of times
                    // the event should happen. You can also apply the same reasoning
                    // for mining the absence pattern
                    clause.left = kb->event_label_mapper.get(it);
                    event_t n = std::numeric_limits<event_t>::max(),
                            N = 0;
                    auto cp = count_table->resolve_primary_index2(it);
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
                    Phi.emplace_back(clause,
                                     1.0,
                                     1.0,
                                     1.0,
                                     1.0);
                    clause.n = N+1;
                    clause.casusu = "Absence";
                    Phi.emplace_back(clause,
                                     1.0,
                                     1.0,
                                     1.0,
                                     1.0);
                }
                else {
                    ChoiceFilter.emplace_back(it);
                }
            } else {
                map_for_itemset_support_score[pattern.second] = pattern.first;
            }
        }
//        std::sort(ChoiceFilter.begin(), ChoiceFilter.end());
        remove_index(frequent_itemset_mining, indices_to_remove);

        clause.n = 1;
        for (auto act_id = 0; act_id<max_act_id; act_id++) {
            for (size_t trace_id = 0; trace_id < log_size; trace_id++) {
                event_t count = count_table->resolve_length(act_id, trace_id);
                if (count > 0) {
//                    map[trace_id].emplace_back(act_id);
                    inv_map[act_id].emplace_back(trace_id);
                }
            }
            auto first_occ = first.at(act_id), last_occ = last.at(act_id);
            if (first_occ == log_size) {
                clause.casusu = "Init";
                clause.left = kb->event_label_mapper.get(act_id);
                Phi.emplace_back(clause, 1.0, 1.0, 1.0, 1.0); // 1.0 == ((double)(first_occ)) / ((double)log_size)
            }
            if (last_occ == log_size) {
                clause.casusu = "End";
                clause.left = kb->event_label_mapper.get(act_id);
                Phi.emplace_back(clause, 1.0, 1.0, 1.0, 1.0); // 1.0 == ((double)(last_occ)) / ((double)log_size)
            }
        }
    }

    inline unsigned char association_rules_for_declare(double support,
                                              const std::pair<size_t, std::unordered_set<act_t>>& pattern,
                                              act_t A,
                                              act_t B) {
        unsigned char shift;
        DEBUG_ASSERT(pattern.second.size() == 2);
        auto it = pattern.second.begin();
        lr.head.clear();
        lr.tail.clear();
        rl.head.clear();
        rl.tail.clear();
        lr.head.emplace_back(*it);
        rl.tail.emplace_back(*it);
        it++;
        lr.tail.emplace_back(*it);
        rl.head.emplace_back(*it);

        double lr_conf = counter.confidence(lr);
        double rl_conf = counter.confidence(rl);
#ifdef DEBUG
//        std::cout << "   conf: " << lr_conf << " conf: " << rl_conf << std::endl;
//        std::cout << "   lift: " << counter.lift(lr) << " lift: " << counter.lift(rl) << std::endl;
#endif
        size_t di_rl = counter.decl_int_support(rl), di_lr = counter.decl_int_support(lr);
        if ((lr_conf == rl_conf) && (lr_conf >= support)) {
            shift = 2;
            graph.get(resp_existenceAB).set(di_lr, lr_conf);
            graph.get(resp_existenceBA).set(di_rl, rl_conf);
        } else if (lr_conf >= rl_conf && lr_conf >= support) {
            shift = 0;
            graph.get(resp_existenceAB).set(di_lr, lr_conf);
        } else if (rl_conf >= support) {
            shift = 1;
            graph.get(resp_existenceBA).set(di_rl, rl_conf);
        }
        graph.get(coexistenceAB_BA).set(counter.decl_coex_int_support(A, B, log_size), counter.decl_coex_conf(A, B));
        const auto& aSet = inv_map.at(A);
        const auto& bSet = inv_map.at(B);
        std::pair<size_t, size_t> ratio = yaucl::iterators::ratio(aSet.begin(), aSet.end(), bSet.begin(), bSet.end());
        graph.get(choiceAB_BA).set(ratio.first, ((double)(ratio.first)) / ((double)log_size));
        return shift;
    }

    inline void precedence_response(act_t A, act_t B, size_t minimum_support_threshold, unsigned char shift, const bool polyadic, bool stopWithThreshold) {
        auto flip = shift ^ 0x1;
        bool alles_precedence = true, alles_response = true;
        size_t alles_not_precedence = 0, alles_not_response = 0;

        size_t expected_support = minimum_support_threshold;

        // Only if the heuristic was activated (for next-based conditions
        // to happen I need to have As and Bs in the same number. Otherwise,
        // this is unlikely to happen, and I do not ever start performing the
        // search.
        // 2) Second kind of patters, always starting scanning from the
        // activation condition (that is also the premise of the rule).
        // This is still computed, as it is required for both 1) and 2)
        auto a_beginend = kb->timed_dataless_exists(A);
//        DEBUG_ASSERT(a_beginend.first != a_beginend.second);

        auto b_beginend = kb->timed_dataless_exists(B);
        // As I obtained the rule, there should be some data pertaining to it!
//        DEBUG_ASSERT(b_beginend.first != b_beginend.second);

        uint32_t /*a_activation_count = 0,*/ a_trace_id = a_beginend.first->entry.id.parts.trace_id, a_prev_trace_id = a_trace_id,

                b_trace_id = b_beginend.first->entry.id.parts.trace_id;
        bool doRetain = false;
        size_t last_a_for_retain = -1;
        if(a_trace_id != 0) {
            yaucl::iterators::iota_n(std::back_inserter(vac_r[shift]), a_trace_id, 0);
//            vac_count_r[shift] += a_trace_id;
        }

        while (a_beginend.first != a_beginend.second) { // (B)
            if (stopWithThreshold && (!alles_precedence) && (!alles_response)) {
                break;
            }

            a_trace_id = a_beginend.first->entry.id.parts.trace_id;
            if (b_beginend.first != b_beginend.second)
                b_trace_id = b_beginend.first->entry.id.parts.trace_id;

            if (doRetain && ((b_beginend.first == b_beginend.second) || (b_trace_id >= last_a_for_retain))) {//L 9-10
                doRetain = false;
                last_a_for_retain = -1; // Redundant
            }

            if ((a_trace_id != a_prev_trace_id)) { // Already visited, L12-13
                yaucl::iterators::iota_n(std::back_inserter(vac_r[shift]), (a_trace_id - a_prev_trace_id - 1), a_prev_trace_id+1);
//                vac_count_r[shift] += (a_trace_id - a_prev_trace_id - 1);
                a_prev_trace_id = a_trace_id;
            }

            /* I. We have As on their own */
            if ((b_beginend.first == b_beginend.second) || (a_trace_id < b_trace_id)) {
//                a_activation_count++;
                // Problem 1)
                // This might be a valid precedence, as nothing is stated
                // to what should happen after the A, but I cannot exploit
                // for a response: therefore, I'm counting it
                decrease_support_X(*kb, expected_support, alles_precedence, alles_not_precedence);
                decrease_support_X(*kb, expected_support, alles_response, alles_not_response);

                TRACE_SET_ADD(act_r[shift], a_trace_id);
                TRACE_SET_ADD(viol_r[shift], a_trace_id);

                TRACE_SET_ADD(act_p[shift], a_trace_id);
                TRACE_SET_ADD(viol_p[flip], a_trace_id);

//                if (left_branch) {
//                    TRACE_SET_ADD(data.r_activation_traces.first, a_trace_id);
//                    TRACE_SET_ADD(data.p_activation_traces.second, a_trace_id);
//                    TRACE_SET_ADD(data.r_lb_violations, a_trace_id);
//                    TRACE_SET_ADD(data.p_lb_violations, a_trace_id);
//                } else if (right_branch) {
//                    TRACE_SET_ADD(data.r_activation_traces.second, a_trace_id);
//                    TRACE_SET_ADD(data.p_activation_traces.first, a_trace_id);
//
//                    if (!trace_set_contains(it_p_lb_violations, data.p_lb_violations, a_trace_id)) {
//                        decrease_support_X(kb, expected_support, alles_succession_ba, alles_not_succession_ba);
//                    }
//                }

                // Now, skipping to the next trace, as there is no more information for as
                if (!doRetain) {
                    last_a_for_retain = a_trace_id;
                    if (b_beginend.first == b_beginend.second)
                        yaucl::iterators::iota_n(std::back_inserter(vac_p[flip]), ((log_size) - a_trace_id), a_trace_id);
//                        vac_count_p[flip] += ((log_size) - a_trace_id);
                    else
                        yaucl::iterators::iota_n(std::back_inserter(vac_p[flip]),(b_trace_id - a_trace_id - 1), a_trace_id+1);
//                        vac_count_p[flip] += (b_trace_id - a_trace_id - 1);
                    doRetain = true;
                }
                fast_forward_equals(a_trace_id, a_beginend.first, a_beginend.second);
                continue;
            }
            /* II. We have B's on their own */
            if ((a_beginend.first == a_beginend.second) || a_trace_id > b_trace_id) {
                for (size_t id = b_trace_id; id<a_trace_id; id++) {
                    TRACE_SET_ADD(act_r[flip], id);
                }
                TRACE_SET_ADD(act_p[flip], a_trace_id);


//                if(left_branch) {
//                    TRACE_SET_ADD(data.r_activation_traces.second, a_trace_id);
//                    TRACE_SET_ADD(data.p_activation_traces.first, a_trace_id);
//                } else if (right_branch) {
//                    TRACE_SET_ADD(data.r_activation_traces.first, a_trace_id);
//                    TRACE_SET_ADD(data.p_activation_traces.second, a_trace_id);
//
//                    if (!trace_set_contains(it_r_lb_violations, data.r_lb_violations, a_trace_id)) {
//                        decrease_support_X(kb, expected_support, alles_succession_ab, alles_not_succession_ab);
//                    }
//                }

                // Moving b until I find something related to b. A is kept fixed and not incremented
                if (a_beginend.first == a_beginend.second) {
                    for (size_t trddmn = b_trace_id; trddmn<log_size; trddmn++) {
                        if (count_table->resolve_length(B, trddmn) == 0)
                            vac_p[flip].emplace_back(trddmn);
                    }
                } else {
                    for (size_t trddmn = b_trace_id; trddmn<a_trace_id; trddmn++) {
                        if (count_table->resolve_length(B, trddmn) == 0)
                            vac_p[flip].emplace_back(trddmn);
                    }
                }
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

//            a_activation_count++;

            TRACE_SET_ADD(act_r[shift], a_trace_id);
            TRACE_SET_ADD(act_r[flip], a_trace_id);
            TRACE_SET_ADD(act_p[shift], a_trace_id);
            TRACE_SET_ADD(act_p[flip], a_trace_id);
//            if (left_branch) {
//                TRACE_SET_ADD(data.r_activation_traces.first, a_trace_id);
//                TRACE_SET_ADD(data.r_activation_traces.second, a_trace_id);
//                TRACE_SET_ADD(data.p_activation_traces.first, a_trace_id);
//                TRACE_SET_ADD(data.p_activation_traces.second, a_trace_id);
//            }

            if ((b_beginend.first != b_beginend.second) && (a_beginend.first != a_beginend.second) &&
                    (b_beginend.first->entry.id.parts.event_id >= a_beginend.first->entry.id.parts.event_id)) { // L.39
                decrease_support_X(*kb, expected_support, alles_precedence, alles_not_precedence);
                TRACE_SET_ADD(viol_p[flip], a_trace_id);
//                if (left_branch) {
//                    TRACE_SET_ADD(data.p_lb_violations, a_trace_id);
//                } if (right_branch) {
//                    /* Found a case for Succession(B,A) where Precedence(B,A) is violated (e.g. B) */
//                    if (!trace_set_contains(it_r_lb_violations, data.r_lb_violations, a_trace_id)) {
//                        decrease_support_X(kb, expected_support, alles_succession_ba, alles_not_succession_ba);
//                    }
//                }
            }

            const size_t offset = polyadic ? a_beginend.first->span : 1;

            // While I'm scanning the A events within the same trace
            //        all_altresponse_in_trace = true;
            while ((a_beginend.first != a_beginend.second) &&
                   (a_beginend.first->entry.id.parts.trace_id == a_trace_id)) {
                // ignoring all of the B events that are not relevant for the task!
                while ((b_beginend.first != b_beginend.second) &&
                       (b_beginend.first->entry.id.parts.trace_id == a_trace_id) &&
                       (b_beginend.first->entry.id.parts.event_id + polyadic <=
                        a_beginend.first->entry.id.parts.event_id)) {
                    b_beginend.first++;
                }
                if ((b_beginend.first != b_beginend.second) &&
                    (b_beginend.first->entry.id.parts.trace_id == a_trace_id) &&
                    (b_beginend.first->entry.id.parts.event_id +polyadic >=
                     a_beginend.first->entry.id.parts.event_id)) {
                    // Ok, I have a match!
                }
                else {
                    // If there is no match for the B event, then I'm setting this to false
                    // and quitting the iteration
                    decrease_support_X(*kb, expected_support, alles_response, alles_not_response);
                    TRACE_SET_ADD(viol_r[shift], a_trace_id);
//                    if (left_branch) {
//                        TRACE_SET_ADD(data.r_lb_violations, a_trace_id);
//                    }
//                    if (right_branch && !trace_set_contains(it_p_lb_violations, data.p_lb_violations, a_trace_id)) {
//                        /* Found a case for Succession(B,A) where Response(B,A) is violated (e.g. B) */
//                        decrease_support_X(*kb, expected_support, alles_succession_ba, alles_not_succession_ba);
//                    }

                    break;
                }
                a_beginend.first++;
            }
            fast_forward_equals(a_trace_id, a_beginend.first, a_beginend.second);
            fast_forward_equals(a_trace_id, b_beginend.first, b_beginend.second);
        }

        if((log_size - 1) != a_prev_trace_id) {
            yaucl::iterators::iota_n(std::back_inserter(vac_r[flip]), ((log_size - 1) - a_prev_trace_id), a_prev_trace_id+1);
        }
    }

    inline void chain_precedence_response(act_t A, act_t B, size_t minimum_support_threshold, unsigned char shift, const bool polyadic, bool stopWithThreshold) {
        auto flip = shift ^ 0x1;
        bool forward_response = false, forward_precedence = false;
        bool alles_next = true, alles_prev = true;
        size_t alles_not_next = 0, alles_not_prev = 0;

        auto a_beginend = kb->timed_dataless_exists(A);
        auto start = a_beginend.first;
        uint32_t a_prev_trace_id = a_beginend.first != a_beginend.second ? a_beginend.first->entry.id.parts.trace_id : -1;
        vac_cr[shift] = vac_r[shift];
        vac_cp[shift] = vac_r[shift];

        size_t expected_support = minimum_support_threshold;

        while (a_beginend.first != a_beginend.second) {
            if (stopWithThreshold && (!alles_next) && (!alles_prev)) {
                break; // Breaking only if all conditions are never met
            }

            auto trace_id = a_beginend.first->entry.id.parts.trace_id;
            if (trace_id != a_prev_trace_id) {
                forward_response = forward_precedence = false;
                a_prev_trace_id = trace_id;
            }

            bool test;
            if (polyadic && (event_to_root.at(A) == event_to_root.at(B))) {
                const auto& V = kb->act_table_by_act_id.secondary_index_polyadic.at(trace_id);
                size_t offset = a_beginend.first->entry.id.parts.event_id+a_beginend.first->span;
                test = (V.size() <= offset) || (V.at(offset).find(B) == V.at(offset).end());
            } else {
                test = ((a_beginend.first->next == nullptr) || (a_beginend.first->next->find(B) == a_beginend.first->next->end()));
            }
            if (!forward_response && (test)) {
                decrease_support_X(*kb, expected_support, alles_next, alles_not_next);
                TRACE_SET_ADD(viol_cr[shift], ((trace_t)a_beginend.first->entry.id.parts.trace_id));
                forward_response = true;
            }

            if (a_beginend.first->prev == nullptr) {
                test = false;
            } else if (polyadic && (event_to_root.at(A) == event_to_root.at(B))) {
                const auto& V = kb->act_table_by_act_id.trace_id_to_endTimeId_to_offset.at(trace_id);
                size_t offset = a_beginend.first->entry.id.parts.event_id-1;
                test = V.at(offset).find(B) == V.at(offset).end();
            } else {
                test = ( (a_beginend.first->prev->find(B) == a_beginend.first->prev->end()));
            }
            if (!forward_precedence && (test)) {
                decrease_support_X(*kb, expected_support, alles_prev, alles_not_prev);
                TRACE_SET_ADD(viol_cp[shift], (trace_t)(a_beginend.first->entry.id.parts.trace_id));
                forward_precedence = true;
            }

            if ((a_beginend.first == start) || (a_beginend.first - 1)->entry.id.parts.trace_id != trace_id) {
//                conf_next_counting++;

//                if (left_branch)
//                    TRACE_SET_ADD(data.cr_activation_traces.first, trace_id);
//                else
//                    TRACE_SET_ADD(data.cr_activation_traces.second, trace_id);
                TRACE_SET_ADD(act_cr[shift], trace_id);

                if ((/*a_beginend.first->prev != nullptr*/ a_beginend.first->entry.id.parts.event_id>0) || (kb->getCountTable().resolve_length(A, trace_id) > 1)) {
//                    conf_prev_counting++;

//                    if (left_branch)
//                        TRACE_SET_ADD(data.cp_activation_traces.first, trace_id);
//                    else
//                        TRACE_SET_ADD(data.cp_activation_traces.second, trace_id);
                    TRACE_SET_ADD(act_cp[shift], trace_id);
                }
                else if ((/*a_beginend.first->prev == nullptr*/ a_beginend.first->entry.id.parts.event_id == 0) && (kb->getCountTable().resolve_length(A, trace_id) == 1)) {
//                    conf_prev_not_counting++;
                    vac_cp[shift].emplace_back(trace_id);
                }
            }

            if (forward_response && forward_precedence) {
                fast_forward_equals(trace_id, a_beginend.first, a_beginend.second);
            }
            else {
                a_beginend.first++;
            }
        }
    }

    inline void temporal_refinement(act_t A, act_t B, size_t minimum_support_threshold, unsigned char shift, bool polyadic, bool stopWithThreshold) {
        precedence_response(A, B, minimum_support_threshold, shift, polyadic, stopWithThreshold);
        chain_precedence_response(A, B, minimum_support_threshold, shift, polyadic, stopWithThreshold);
    }

    inline void single_banching_mining(unsigned char shift, bool collectAllEvidence) {
        auto flip = shift ^ 0x1;
        if (collectAllEvidence) {
            graph.get(flip ? crespAB : crespBA).set(sat_cr[shift], vac_cr[shift], viol_cr[shift]);
            graph.get(flip ? respAB : respBA).set(sat_r[shift], vac_r[shift], viol_r[shift]);
            graph.get(flip ? precBA : precAB).set(sat_p[shift], vac_p[shift], viol_p[shift]);
            graph.get(flip ? cprecAB : cprecBA).set(sat_cp[shift], vac_cp[shift], viol_cp[shift]);
        } else {
            graph.get(flip ? crespAB : crespBA).set(sat_cr[shift].size(), ((double)yaucl::iterators::ratio_intersection(sat_cr[shift], act_cr[shift]))/((double)yaucl::iterators::ratio_union(act_cr[shift], viol_cr[shift])));
            graph.get(flip ? respAB : respBA).set(sat_r[shift].size(), ((double)yaucl::iterators::ratio_intersection(sat_r[shift], act_r[shift]))/((double)yaucl::iterators::ratio_union(act_r[shift], viol_r[shift])));
            graph.get(flip ? precBA : precAB).set(sat_p[flip].size(), ((double)yaucl::iterators::ratio_intersection(sat_p[flip], act_p[flip]))/((double)yaucl::iterators::ratio_union(act_p[flip], viol_p[flip])));
            graph.get(flip ? cprecAB : cprecBA).set(sat_cp[shift].size(), ((double)yaucl::iterators::ratio_intersection(sat_cp[shift], act_cp[shift]))/((double)yaucl::iterators::ratio_union(act_cp[shift], viol_cp[shift])));
        }

    }

    inline void cross_branch_entailment(size_t log_theta, bool collectAllEvidence) {
        if ((!collectAllEvidence) && ((sat_r[1].size() < log_theta) && (sat_p[0].size() < log_theta) && (sat_cr[1].size() < log_theta) && (sat_cp[1].size() < log_theta))) {
            return;
        }
        std::set_union(act_cr[0].begin(), act_cr[0].end(),act_cp[1].begin(), act_cp[1].end(), std::back_inserter(act_cs0));
        std::set_union(act_cr[1].begin(), act_cr[1].end(),act_cp[0].begin(), act_cp[0].end(), std::back_inserter(act_cs1));
        std::set_union(viol_cr[0].begin(), viol_cr[0].end(),viol_cp[1].begin(), viol_cp[1].end(), std::back_inserter(viol_cs0));
        std::set_union(viol_cr[1].begin(), viol_cr[1].end(),viol_cp[0].begin(), viol_cp[0].end(), std::back_inserter(viol_cs1));
        std::set_intersection(sat_cr[0].begin(), sat_cr[0].end(),sat_cp[1].begin(), sat_cp[1].end(), std::back_inserter(sat_cs0));
        std::set_intersection(sat_cr[1].begin(), sat_cr[1].end(),sat_cp[0].begin(), sat_cp[0].end(), std::back_inserter(sat_cs1));
        std::set_union(act_r[0].begin(), act_r[0].end(),act_p[0].begin(), act_p[0].end(), std::back_inserter(act_s0));
        std::set_union(act_r[1].begin(), act_r[1].end(),act_p[1].begin(), act_p[1].end(), std::back_inserter(act_s1));
        std::set_union(viol_r[0].begin(), viol_r[0].end(),viol_p[0].begin(), viol_p[0].end(), std::back_inserter(viol_s0));
        std::set_union(viol_r[1].begin(), viol_r[1].end(),viol_p[1].begin(), viol_p[1].end(), std::back_inserter(viol_s1));
        std::set_intersection(sat_r[0].begin(), sat_r[0].end(),sat_p[0].begin(), sat_p[0].end(), std::back_inserter(sat_s0));
        std::set_intersection(sat_r[1].begin(), sat_r[1].end(),sat_p[1].begin(), sat_p[1].end(), std::back_inserter(sat_s1));
        if (collectAllEvidence) {
            std::set_intersection(vac_cr[0].begin(), vac_cr[0].end(),vac_cp[1].begin(), vac_cp[1].end(), std::back_inserter(vac_cs0));
            std::set_intersection(vac_cr[1].begin(), vac_cr[1].end(),vac_cp[0].begin(), vac_cp[0].end(), std::back_inserter(vac_cs1));
            std::set_intersection(vac_r[0].begin(), vac_r[0].end(),vac_p[0].begin(), vac_p[0].end(), std::back_inserter(vac_s0));
            std::set_intersection(vac_r[1].begin(), vac_r[1].end(),vac_p[1].begin(), vac_p[1].end(), std::back_inserter(vac_s1));
            graph.get(csuccAB).set(sat_cs0, vac_cs0, viol_cs0);
            graph.get(csuccBA).set(sat_cs1, vac_cs1, viol_cs1);
            graph.get(succAB).set(sat_s0, vac_s0, viol_s0);
            graph.get(succBA).set(sat_s1, vac_s1, viol_s1);
        } else {
            graph.get(csuccAB).set(sat_cs0.size(), ((double)yaucl::iterators::ratio_intersection(sat_cs0, act_cs0))/((double)yaucl::iterators::ratio_union(act_cs0, viol_cs0)));
            graph.get(csuccBA).set(sat_cs1.size(), ((double)yaucl::iterators::ratio_intersection(sat_cs1, act_cs1))/((double)yaucl::iterators::ratio_union(act_cs1, viol_cs1)));
            graph.get(succAB).set(sat_s0.size(), ((double)yaucl::iterators::ratio_intersection(sat_s0, act_s0))/((double)yaucl::iterators::ratio_union(act_s0, viol_s0)));
            graph.get(succBA).set(sat_s1.size(), ((double)yaucl::iterators::ratio_intersection(sat_s1, act_s1))/((double)yaucl::iterators::ratio_union(act_s1, viol_s1)));
        }
    }

    void extractPatternsForcibly(bool polyadic, act_t A, act_t B, FastDatalessClause &clause,
                                  size_t min_int_supp_patt,
                                 unsigned char hasCoExistence, bool branch, bool stopWithThreshold,  bool collectAllEvidence) {
        if (branch) {
            temporal_refinement(A, B, min_int_supp_patt, 0, polyadic, stopWithThreshold);
            temporal_refinement(B, A, min_int_supp_patt, 1, polyadic, stopWithThreshold);
            mdev(0);
            mdev(1);
        } else if (hasCoExistence){
            temporal_refinement(B, A, min_int_supp_patt, 1, polyadic, stopWithThreshold);
            mdev(1);
        } else {
            temporal_refinement(A, B, min_int_supp_patt, 0, polyadic, stopWithThreshold);
            mdev(0);
        }
        if (branch) {
            single_banching_mining(0, collectAllEvidence);
            single_banching_mining(1, collectAllEvidence);
            cross_branch_entailment(min_int_supp_patt, collectAllEvidence);
        } else {
            single_banching_mining(hasCoExistence, collectAllEvidence);
        }
    }

    inline void binary_clauses_mining(double support, size_t minimum_support_threshold, bool polyadic,
                                      std::vector<std::pair<size_t, std::unordered_set<act_t>>>& frequent_itemset_mining) {
        act_t A, B;
        FastDatalessClause clause;
        std::unordered_map<act_t, std::string> resolveLabelCache;
        clause.n = 2;
        size_t min_int_supp_patt = std::ceil(((double)support) * (minimum_support_threshold));
//        std::set<act_t> discarded_actions, considered_actions;
        result_container rc;
        rc.log_size = log_size;
        std::unordered_set<std::pair<act_t,act_t>> used;
        for (const auto& binary_pattern : frequent_itemset_mining) {
            rc.result.clear();
            clearResultsVector();
            std::vector<unsigned short> AB{binary_pattern.second.begin(), binary_pattern.second.end()};
            rc.A = A = AB[0];
            rc.B = B = AB[1];
            resolveLabelCache.clear();
            resolveLabelCache.emplace(A, kb->event_label_mapper.get(A));
            resolveLabelCache.emplace(B, kb->event_label_mapper.get(B));

            auto A_label = resolveLabelCache.at(A);
            auto B_label = resolveLabelCache.at(B);
            if ((A_label == "__missing") || (B_label == "__missing"))
                continue;
#if 0
            if (((A_label == "DecreaseRapidly(Mean Stance Time_a)") || (A_label == "DecreaseRapidly(Mean Step Length_a)")) && ((B_label == "DecreaseRapidly(Mean Stance Time_a)") || (B_label == "DecreaseRapidly(Mean Step Length_a)"))) {
                std::map<size_t, std::map<size_t, std::vector<std::pair<std::string,size_t>>>>  reconstructor;
//                std::cout << "FOR: " << A_label << std::endl;
                auto a_beginend = kb->timed_dataless_exists(A);
                bool first = true;
                trace_t traceId = 0;
                event_t eventId = 0;
                while (a_beginend.first != a_beginend.second) {
                    reconstructor[a_beginend.first->entry.id.parts.trace_id][a_beginend.first->entry.id.parts.event_id].emplace_back(A_label, a_beginend.first->span);
//                    if (first || (a_beginend.first->entry.id.parts.trace_id != traceId)) {
//                        first = false;
//                        traceId = a_beginend.first->entry.id.parts.trace_id;
//                        eventId= a_beginend.first->entry.id.parts.event_id;
//                        std::cout << "TRACE #" << traceId << std::endl;
//                    }
//                    if (eventId != a_beginend.first->entry.id.parts.event_id) {
//                        std::cout << "- Event #" << traceId  << std::endl;
//                    }
//                    std::cout << "   with span = " << a_beginend.first->span  << std::endl;
                    a_beginend.first++;
                }
//                std::cout << "FOR: " << B_label << std::endl;
                auto b_beginend = kb->timed_dataless_exists(B);
                while (b_beginend.first != b_beginend.second) {
                    reconstructor[b_beginend.first->entry.id.parts.trace_id][b_beginend.first->entry.id.parts.event_id].emplace_back(B_label, b_beginend.first->span);
//
//                    if (first || (b_beginend.first->entry.id.parts.trace_id != traceId)) {
//                        first = false;
//                        traceId = b_beginend.first->entry.id.parts.trace_id;
//                        eventId= b_beginend.first->entry.id.parts.event_id;
//                        std::cout << "TRACE #" << traceId << std::endl;
//                    }
//                    if (eventId != b_beginend.first->entry.id.parts.event_id) {
//                        std::cout << "- Event #" << traceId  << std::endl;
//                    }
//                    std::cout << "   with span = " << b_beginend.first->span  << std::endl;
                    b_beginend.first++;
                }
                first = true;
                traceId = 0;
                for (const auto& [trace_id, trace] : reconstructor) {
                    std::cout << "TRACE #" << trace_id << std::endl;
                    for (const auto& [event_id, content] : trace) {
                        std::cout << "- Event #" << event_id  << std::endl;
                        for (const auto& [label,span] : content) {
                            std::cout << "    " << label << " with span = " << span << std::endl;
                        }
                    }
                    std::cout << std::endl << std::endl << std::endl;
                }
                exit(2);
            }
#endif
            unsigned char hasCoExistence = association_rules_for_declare(support, binary_pattern, A, B);

            /* We want to force a branch if the Bs ever occur at the start of the trace and occur only once.
             * This is due to ChainPrecedence, which has an activation of X(A), and we want to mine a potential
             * ChainPrecedence(A,B) */
            bool branch = (hasCoExistence==2) || (Beginnings.at(B) > 0); // Algorithm 7, L. 15

            extractPatternsForcibly(polyadic, A, B, clause, min_int_supp_patt,
                                    hasCoExistence, branch, true, false);
            QM_DECLARE Q{&graph, min_int_supp_patt};
            graph.visit<QM_DECLARE, result_container>(choiceAB_BA, Q, rc);
            if (!rc.result.empty()) {
                for (const auto& res : rc.result) {
                    clause.casusu = res.name;
                    clause.left = resolveLabelCache.at(res.A);
                    clause.right = resolveLabelCache.at(res.B);
                    Phi.emplace_back(clause, -1.0, res.support, res.rconf, -1.0);
                }
                if (A<B) {
                    used.emplace(A,B);
                } else {
                    used.emplace(B,A);
                }
            }
        }
//        std::set_difference(discarded_actions.begin(), discarded_actions.end(), considered_actions.begin(), considered_actions.end(), std::back_inserter(ChoiceFilter));
        if (ChoiceFilter.empty()) {
            ChoiceFilter.reserve(max_act_id);
            for (auto act_id = 0; act_id<max_act_id; act_id++) {
                ChoiceFilter.emplace_back(act_id);
            }
        }
        remove_duplicates(ChoiceFilter);
        std::pair<act_t, act_t> cp;
        std::unordered_map<act_t, retain_choice> map_for_retain;
        for (size_t act_id_offset = 0; act_id_offset<ChoiceFilter.size(); act_id_offset++) {
            cp.first = ChoiceFilter.at(act_id_offset);
            for (size_t act_idB_offset = act_id_offset+1; act_idB_offset<ChoiceFilter.size(); act_idB_offset++) {
                cp.second = ChoiceFilter.at(act_idB_offset);
                if (!used.contains(cp)) {
                    choice_exclchoice(cp.first, cp.second,
                                      log_size,
                                      minimum_support_threshold,
                                      *kb,
                                      inv_map,
                                      map_for_retain,
                                      map_for_itemset_support_score);
                }
            }
        }
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
                for (auto& clauseX : it_1->second) {
                    cp.first = kb->event_label_mapper.get(clauseX.clause.left);
                    cp.second = kb->event_label_mapper.get(clauseX.clause.right);
//                    if (considered.insert(cp).second) {
                        Phi.emplace_back(std::move(clauseX));
//                    }
                }
                it_1++;
            }
        }
        size_t curr_size_Clauses = Phi.size();
        std::sort(Phi.begin(), Phi.end(), [](const pattern_mining_result<FastDatalessClause>& l, const pattern_mining_result<FastDatalessClause>& r) {
            return std::tie(l.clause.casusu, l.clause.left, l.clause.right, l.clause.n, l.restrictive_confidence_plus_declarative_pattern, l.support_declarative_pattern) > std::tie(r.clause.casusu, r.clause.left, r.clause.right, r.clause.n, r.restrictive_confidence_plus_declarative_pattern, r.support_declarative_pattern);
        });
        Phi.erase(std::unique(Phi.begin(), Phi.end(), [](const pattern_mining_result<FastDatalessClause>& l, const pattern_mining_result<FastDatalessClause>& r) {
            return std::tie(l.clause.casusu, l.clause.left, l.clause.right, l.clause.n) == std::tie(r.clause.casusu, r.clause.left, r.clause.right, r.clause.n);
        }), Phi.end());
        DEBUG_ASSERT(curr_size_Clauses == Phi.size());
    }

    inline void mdev(size_t i) {
        auto flip = i ^ 0x1;
        remove_duplicates(act_r[i]);
        remove_duplicates(viol_r[i]);
        set_complement(log_size, viol_r[i].begin(), viol_r[i].end(), std::back_inserter(sat_r[i]));
        remove_duplicates(act_p[flip]);
        remove_duplicates(viol_p[flip]);
        set_complement(log_size, viol_p[flip].begin(), viol_p[flip].end(), std::back_inserter(sat_p[flip]));
        remove_duplicates(act_cr[i]);
        remove_duplicates(viol_cr[i]);
        set_complement(log_size, viol_cr[i].begin(), viol_cr[i].end(), std::back_inserter(sat_cr[i]));
        remove_duplicates(act_cp[i]);
        remove_duplicates(viol_cp[i]);
        set_complement(log_size, viol_cp[i].begin(), viol_cp[i].end(), std::back_inserter(sat_cp[i]));
    }

    inline void serialize_to_file(std::unordered_map<std::tuple<std::string,std::string,std::string>, std::vector<char>>& map, std::ostream& file) {
        for (const auto& [triplet, vector] : map) {
            if (std::get<2>(triplet).empty()) {
                file << "\"" << std::get<0>(triplet) << "(" << std::get<1>(triplet) << ")\"";
            } else {

                file << "\"" << std::get<0>(triplet) << "(" << std::get<1>(triplet) << "," << std::get<2>(triplet) << ")\"";
            }
            for (size_t i = 0, N = vector.size(); i<N; i++) {
                short tmp = (short)vector.at(i);
                if (tmp==-2) {
                    std::vector<size_t> result1, result0;
                    set_complement(log_size, viol_cp[0].begin(), viol_cp[0].end(), std::back_inserter(result0));
                    set_complement(log_size, viol_cp[1].begin(), viol_cp[1].end(), std::back_inserter(result1));
                    std::cerr << std::get<0>(triplet) << std::endl;
                }
                file << "," << (short)tmp;
            }
            file << std::endl;
            file.flush();
        }
        map.clear();
    }

    void fast_check_and_collector_dataless(bool polyadic, KnowledgeBase* ptr,
                                           const std::set<std::string>& acts,
                                           const std::unordered_map<std::string,std::vector<event_t>>& exists,
                                           const std::unordered_map<std::string,std::vector<event_t>>& absence,
                                           std::ostream& file) {

        std::cout << "Initalization..." << std::endl;
        for (const auto& [root, children]: ptr->hierarchy_def) {
            ptr->event_label_mapper.put(root);
        }
        setKnowledgeBaseAndInit(ptr);
        std::vector<size_t> actLabels;
        std::unordered_map<std::tuple<std::string,std::string,std::string>, std::vector<char>> result_map;
        std::unordered_map<size_t, std::vector<size_t>> act_Labels;
        std::unordered_map<size_t, std::vector<size_t>> noact_Labels;
        std::unordered_map<size_t, std::vector<size_t>> first;
        std::unordered_map<size_t, std::vector<size_t>> last;
        ssize_t trace_id = -1;
        size_t log_size = ptr->nTraces();
        for (const auto& x : acts) {
            size_t id = ptr->event_label_mapper.get(x);
            actLabels.emplace_back(id);
            auto a_beginend = kb->timed_dataless_exists(id);
            auto& v = act_Labels[id];
            trace_id = -1;
            while (a_beginend.first != a_beginend.second) {
                if (trace_id != a_beginend.first->entry.id.parts.trace_id) {
                    trace_id = a_beginend.first->entry.id.parts.trace_id;
                    v.emplace_back(trace_id);
                }
                a_beginend.first++;
            }
            set_complement(log_size, v.begin(), v.end(), std::back_inserter(noact_Labels[id]));
        }
//        remove_duplicates(actLabels);
        FastDatalessClause clause;
        result_container rc;
        std::vector<size_t> act, viol, vac;
        std::vector<size_t> A_and_B, A_or_B, neither_ACT;
        std::vector<size_t> A_not_B, B_not_A, all_VIOL, excl_OCC;

        // Unary Clauses
        std::cout << "Init/End..." << std::endl;
        for (size_t trace_id = 0; trace_id < log_size; trace_id++) {
            const auto& first_last =kb->act_table_by_act_id.secondary_index.at(trace_id);
            for (auto it = first_last.first->begin(), en = first_last.first->end(); it!=en; it++) {
                first[it->first].emplace_back(trace_id);
            }
            for (auto it = first_last.second->begin(), en = first_last.second->end(); it!=en; it++) {
                last[it->first].emplace_back(trace_id);
            }
        }
        std::cout << "First..." << std::endl;
        std::tuple<std::string,std::string,std::string> simplistic_clause{"Init","","§1"};
        for (const auto& [act_id, traces] : first) {
            all_VIOL.clear();
            std::get<1>(simplistic_clause) = ptr->event_label_mapper.get(act_id);
            auto& v = result_map[simplistic_clause];
            v.resize(log_size, -1);
            for (size_t trace_id : traces)
                v[trace_id] = 1;
        }
        all_VIOL.clear();
        std::get<0>(simplistic_clause) = "End";
        std::cout << "Last..." << std::endl;
        for (const auto& [act_id, traces] : last) {
            all_VIOL.clear();
            std::get<1>(simplistic_clause) = ptr->event_label_mapper.get(act_id);
            auto& v = result_map[simplistic_clause];
            v.resize(log_size, -1);
            for (size_t trace_id : traces)
                v[trace_id] = 1;
        }
        all_VIOL.clear();
        serialize_to_file(result_map, file);

        std::get<0>(simplistic_clause) = "Exists";
        std::cout << "Exists..." << std::endl;
        for (const auto& [act_id, countings] : exists) {
            std::get<1>(simplistic_clause) = act_id;
            auto indexes = ptr->resolveCountingData(act_id);
            if ((indexes.first == indexes.second) && (indexes.first == (uint32_t)-1)) {
                exit(5);
            } else {
                std::unordered_map<size_t, std::string> MAP;
                for (size_t count : countings) {
                    std::get<2>(simplistic_clause) = MAP[count] = ("§"+std::to_string(count));
                    auto& v = result_map[simplistic_clause];
                    v.resize(log_size, -1);
                }
                for (auto it = count_table->table.begin() + indexes.first; it != count_table->table.begin() + indexes.second + 1; ++it) {
                    for (const auto& [count, str] : MAP) {
                        std::get<2>(simplistic_clause) = str;
                        if (it->id.parts.event_id >= count)//it->id.parts.event_id >= amount
                            result_map[simplistic_clause][it->id.parts.trace_id] = 1;
                    }
                }
            }
        }
        serialize_to_file(result_map, file);
        std::get<0>(simplistic_clause) = "Absence";
        std::cout << "Absence..." << std::endl;
        for (const auto& [act_id, countings] : absence) {
            std::get<1>(simplistic_clause) = act_id;
            auto indexes = ptr->resolveCountingData(act_id);
            if ((indexes.first == indexes.second) && (indexes.first == (uint32_t)-1)) {
                exit(4);
            } else {
                std::unordered_map<size_t, std::string> MAP;
                for (size_t count : countings) {
                    std::get<2>(simplistic_clause) = MAP[count] = ("§"+std::to_string(count));
                    auto& v = result_map[simplistic_clause];
                    v.resize(log_size, -1);
                }
                for (auto it = count_table->table.begin() + indexes.first; it != count_table->table.begin() + indexes.second + 1; ++it) {
                    for (const auto& [count, str] : MAP) {
                        std::get<2>(simplistic_clause) = str;
                        if (it->id.parts.event_id < count)
                            result_map[simplistic_clause][it->id.parts.trace_id] = 1;
                    }
                }
            }
        }
        serialize_to_file(result_map, file);
        std::cout << "Binary Clauses..." << std::endl;

        for (size_t i = 0, N= actLabels.size(); i<N; i++) {
            auto A = actLabels.at(i);
            rc.A = A;
            std::string labelA = ptr->event_label_mapper.get(A);
            const auto& aAct = act_Labels[A];
            const auto& aNoAct = noact_Labels[A];
//            std::cout << " * " << labelA << std::endl;
            for (size_t j = 0; j<i; j++) {
                auto B = actLabels.at(j);
                std::string labelB = ptr->event_label_mapper.get(B);
//                std::cout << "\t\t - " << labelB << std::endl;
                rc.B = B;
                const auto& bAct = act_Labels[B];
                const auto& bNoAct = noact_Labels[B];
                A_and_B.clear();
                A_or_B.clear();
                neither_ACT.clear();
                A_not_B.clear();
                B_not_A.clear();
                all_VIOL.clear();
                excl_OCC.clear();

                clearResultsVector();
                extractPatternsForcibly(polyadic, A, B, clause, 0, true, true, false, true);

                rc.complex_operator(crespAB, graph.get(crespAB), labelA, labelB, log_size, result_map);
                rc.complex_operator(crespBA, graph.get(crespBA), labelA, labelB, log_size, result_map);
                rc.complex_operator(respAB, graph.get(respAB), labelA, labelB, log_size, result_map);
                rc.complex_operator(respBA, graph.get(respBA), labelA, labelB, log_size, result_map);
                rc.complex_operator(precAB, graph.get(precAB), labelA, labelB, log_size, result_map);
                rc.complex_operator(precBA, graph.get(precBA), labelA, labelB, log_size, result_map);
                rc.complex_operator(cprecAB, graph.get(cprecAB), labelA, labelB, log_size, result_map);
                rc.complex_operator(cprecBA, graph.get(cprecBA), labelA, labelB, log_size, result_map);
                rc.complex_operator(csuccAB, graph.get(csuccAB), labelA, labelB, log_size, result_map);
                rc.complex_operator(csuccBA, graph.get(csuccBA), labelA, labelB, log_size, result_map);
                rc.complex_operator(succAB, graph.get(succAB), labelA, labelB, log_size, result_map);
                rc.complex_operator(succBA, graph.get(succBA), labelA, labelB, log_size, result_map);

                std::set_intersection(aAct.begin(), aAct.end(), bAct.begin(), bAct.end(), std::back_inserter(A_and_B));
                std::set_union(aAct.begin(), aAct.end(), bAct.begin(), bAct.end(), std::back_inserter(A_or_B));
                std::set_intersection(aNoAct.begin(), aNoAct.end(), bNoAct.begin(), bNoAct.end(), std::back_inserter(neither_ACT));
                std::set_difference(aAct.begin(), aAct.end(), bAct.begin(), bAct.end(), std::back_inserter(A_not_B));
                std::set_difference(bAct.begin(), bAct.end(), aAct.begin(), aAct.end(), std::back_inserter(B_not_A));
                std::set_intersection(A_not_B.begin(), A_not_B.end(), B_not_A.begin(), B_not_A.end(), std::back_inserter(excl_OCC));

                graph.get(resp_existenceAB).set(A_and_B, aNoAct, A_not_B);
                graph.get(resp_existenceBA).set(A_and_B, bNoAct, B_not_A);
                graph.get(coexistenceAB_BA).set(A_and_B, neither_ACT, excl_OCC);
                graph.get(choiceAB_BA).set(A_or_B, neither_ACT, neither_ACT);
                std::vector<size_t> tmp;
                std::set_union(neither_ACT.begin(), neither_ACT.end(), A_and_B.begin(), A_and_B.end(), std::back_inserter(tmp));
                graph.get(exclchoiceAB_BA).set(excl_OCC, tmp, tmp);

                rc.complex_operator(resp_existenceAB, graph.get(resp_existenceAB), labelA, labelB, log_size, result_map);
                rc.complex_operator(resp_existenceBA, graph.get(resp_existenceBA), labelA, labelB, log_size, result_map);
                rc.complex_operator(coexistenceAB_BA, graph.get(coexistenceAB_BA), labelA, labelB, log_size, result_map);
                rc.complex_operator(choiceAB_BA, graph.get(choiceAB_BA), labelA, labelB, log_size, result_map);
                rc.complex_operator(exclchoiceAB_BA, graph.get(exclchoiceAB_BA), labelA, labelB, log_size, result_map);
                serialize_to_file(result_map, file);
            }
        }
    }

    void run(double support, bool polyadic, const KnowledgeBase* ptr) {
        if (!ptr) return;
        clear();
        setKnowledgeBaseAndInit(ptr);

        support = std::max(std::min(support, 1.0), 0.0);
        uint64_t minimum_support_threshold = std::min((uint64_t)std::ceil((double)log_size * support), log_size);

        // Algorithm 4. Generate Frequent Itemsets
        std::vector<std::pair<size_t, std::unordered_set<act_t>>> fpt_result;
        {
            std::vector<std::pair<act_t, size_t>> final_element_for_scan;
            fpt_result = std::move(fpgrowth(*count_table, max_act_id, final_element_for_scan, minimum_support_threshold, 2));
        }

        // Algorithm 5. Generate Unary clauses
        generate_unary_clauses(fpt_result);

        // Algorithm 6. Binary Clauses Mining
        binary_clauses_mining(support, minimum_support_threshold, polyadic, fpt_result);
    }

private:
    const KnowledgeBase* kb{nullptr};
    void clear() {
        ChoiceFilter.clear();
        map_for_itemset_support_score.clear();
    }
};


#endif //KNOBAB_SERVER_POLYADIC_BOLT_H
