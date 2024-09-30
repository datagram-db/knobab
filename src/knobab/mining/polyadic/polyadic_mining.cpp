//
// Created by giacomo on 28/02/24.
//

#include "knobab/mining/polyadic/polyadic_mining.h"

#include <string>
#include <unordered_map>
#include <set>
#include <list>
#include "knobab/mining/polyadic/commons.h"

std::pair<double,double> algorithmic_strategy::polyadic_dataful_mining_and_refinement(const std::string& ser_path, double mining_supp, bool polyadic, bool reduction, ServerQueryManager &sqm,
                                                                 std::unordered_map<std::string, std::set<std::tuple<std::string, std::string, std::string>>> &diff) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    std::unordered_map<FastDatalessClause, std::vector<std::string>> cvnc; // clause_unary_name_count
    std::unordered_map<std::string, polyadic_bolt> gv; //mining_environment
    std::unordered_map<std::string, std::vector<std::pair<size_t, std::unordered_set<act_t>>>> frequent_itemsets;
    std::vector<size_t> support;
    std::unordered_map<std::string, std::vector<std::string>> act_to_log_name;
    std::pair<std::string,std::string> cp_acts;
    std::unordered_map<std::pair<std::string,std::string>, std::vector<std::pair<std::string,size_t>>> elements;
    std::unordered_map<std::string, std::vector<size_t>> indices_to_remove;
    std::unordered_map<std::string, std::unordered_map<act_t, std::string> > forAllLogsCacheMap;
    std::unordered_map<std::string, result_container> rcv;
    std::unordered_map<std::string,  std::unordered_set<std::pair<act_t,act_t>>> usedv;


    for (const auto& [log_name, kb] : sqm.multiple_logs) {
        auto& g = gv[log_name];
        auto& ref = frequent_itemsets[log_name];
        auto& currentCache = forAllLogsCacheMap[log_name];
        // Mining just the unary clauses
        support.emplace_back(g.run1(mining_supp, polyadic, &kb.db, ref));

        // Determining which itemsets can be computed non in-tandem
        // 1) Determining which itemsets are shared and which are not
        for (size_t idx = 0, N = ref.size(); idx<N; idx++) {
            const auto& count = ref.at(idx).first;
            const auto& itemset = ref.at(idx).second;
            if (itemset.size() == 2) {
                auto it = itemset.begin();

                cp_acts.first = kb.db.event_label_mapper.get(*it);
                currentCache.emplace(*it, cp_acts.first);
                cp_acts.second = kb.db.event_label_mapper.get(*it);
                it++;
                currentCache.emplace(*it, cp_acts.second);

                if ((cp_acts.first == "__missing")) {
                    indices_to_remove[log_name].emplace_back(idx);
                    continue;
                }
                if ((cp_acts.second == "__missing"))  {
                    indices_to_remove[log_name].emplace_back(idx);
                    continue;
                }
                if (cp_acts.first > cp_acts.second)
                    std::swap(cp_acts.first, cp_acts.second);
                elements[cp_acts].emplace_back(log_name, idx);
            } else {
                auto it = itemset.begin();
                const auto& ref2 = kb.db.event_label_mapper.get(*it);
                currentCache.emplace(*it, ref2);
            }
        }
    }
    for (auto it = elements.begin(); it != elements.end();  ) {
        if (it->second.size() == 1) {
            it = elements.erase(it); // Using the joing mining, so, I can remove this from the set
        } else {
            for (const auto& ref : it->second) {
                indices_to_remove[ref.first].emplace_back(ref.second);  // Not using directly the frequent itemsets for mining the shared clauses
            }
            it++;
        }
    }
    for (auto& [k,v] : indices_to_remove) {
        remove_duplicates(v);
        remove_index(frequent_itemsets[k], v); // Removing the shared frequent itemset by index, so that those can be handled joinly along the elements of the map.
        // Also, removing all the  patterns with __missing
    }
    indices_to_remove.clear();


    // Retaining only the clauses that are shared across at least two logs
    bool refine_init = false, refine_ends = false, refine_existentials = false;
    for (const auto& [log_name, g] : gv) {
        if (refine_init && refine_ends && (!refine_existentials))
            break;
        for (const auto& cl: g.Phi) {
            if (cl.clause.casusu == "Init") {
                if (refine_init) {
                    continue;
                } else {
                    cvnc[cl.clause].emplace_back(log_name);
                    if (cvnc[cl.clause].size() == 2) {
                        refine_init = true;
                    }
                }
            }
            if (cl.clause.casusu == "End") {
                if (refine_ends) {
                    continue;
                } else {
                    cvnc[cl.clause].emplace_back(log_name);
                    if (cvnc[cl.clause].size() == 2) {
                        refine_ends = true;
                    }
                }
            }
            if (cl.clause.casusu == "Exists") {
                auto& ref = act_to_log_name[cl.clause.left];
                ref.emplace_back(log_name);
                refine_existentials = refine_existentials || (ref.size()>1);
            }
            if (refine_init && refine_ends && (!refine_existentials))
                break;
        }
    }
    cvnc.clear();


    std::unordered_set<std::string> categorical, numerical;
    if (refine_init || refine_ends || refine_existentials) {
        for (const auto& [log, env] : sqm.multiple_logs) {
            for (const auto &[key, table]: env.db.attribute_name_to_table) {
                if (table.type == StringAtt)
                    categorical.emplace(key);
                else
                    numerical.emplace(key);
            }
        }
    }

    if (refine_init || refine_ends) {
        std::vector<std::pair<env, int>> begins, ends;
        std::unordered_map<std::string, union_minimal > tuple;


        std::unordered_map<std::string, std::vector<std::vector<size_t>>> W1, W2;
        for (const auto& [log, env] : sqm.multiple_logs) {
            for (const auto& [key, table] : env.db.attribute_name_to_table) {
                if (table.type == StringAtt)
                    categorical.emplace(key);
                else
                    numerical.emplace(key);
            }
            int clazz = std::stoi(log);
            if (refine_init) W1[log].resize(env.db.act_table_by_act_id.secondary_index.size());
            if (refine_ends) W2[log].resize(env.db.act_table_by_act_id.secondary_index.size());
            for (size_t trace_id = 0, N = env.db.act_table_by_act_id.secondary_index.size(); trace_id < N; trace_id++) {
                const auto& IDX = env.db.act_table_by_act_id.secondary_index.at(trace_id);
                if (refine_init) {
                    for (const auto& [act, event_record_ls] : *IDX.first) {
                        for (const auto& record : event_record_ls) {
                            tuple.clear();
                            size_t offset = record - env.db.act_table_by_act_id.table.data();
                            tuple["__label"] = env.db.event_label_mapper.get(act);
                            for (const auto& [key, table] : env.db.attribute_name_to_table) {
                                table.resolve_record_if_exists2(offset, tuple);
                            }
                            W1[log][trace_id].emplace_back(begins.size());
                            begins.emplace_back(tuple, clazz);
                        }
                    }
                } else if (refine_ends) {
                    for (const auto& [act, event_record_ls] : *IDX.second) {
                        for (const auto& record : event_record_ls) {
                            tuple.clear();
                            size_t offset = record - env.db.act_table_by_act_id.table.data();
                            tuple["__label"] = env.db.event_label_mapper.get(act);
                            for (const auto& [key, table] : env.db.attribute_name_to_table) {
                                table.resolve_record_if_exists2(offset, tuple);
                            }
                            W2[log][trace_id].emplace_back(ends.size());
                            ends.emplace_back(tuple, clazz);
                        }
                    }
                }
            }
        }
        if (refine_init) {
            train_and_dump_to_csv(sqm.multiple_logs, W1, ser_path+"_Refinement_init", begins, "InitAll", "InitSome", false, sqm.multiple_logs.size(),numerical, categorical );
        }
        if (refine_ends) {
            train_and_dump_to_csv(sqm.multiple_logs, W1, ser_path+"_Refinement_end", ends, "EndAll", "EndSome", false, sqm.multiple_logs.size(),numerical, categorical );
        }
    }

    if (refine_existentials) {
        std::unordered_map<std::string, union_minimal > tuple;
        for (const auto& [act, kb_ids] : act_to_log_name) {
            if (kb_ids.size() > 1) {
                // Actually performing the refinement
                std::unordered_map<std::string, std::vector<std::vector<size_t>>> W1;
                std::vector<std::pair<env, int>> begins;
                for (const auto& [log, env] : sqm.multiple_logs) {
                    int clazz = std::stoi(log);
                    auto A = env.db.event_label_mapper.get(act);
                    auto cp = env.db.timed_dataless_exists(A);
                    W1[log].resize(env.db.act_table_by_act_id.secondary_index.size());
                    while (cp.first != cp.second) {
                        tuple.clear();
                        size_t offset = cp.first - env.db.act_table_by_act_id.table.data();
                        for (const auto& [key, table] : env.db.attribute_name_to_table) {
                            table.resolve_record_if_exists2(offset, tuple);
                        }
                        W1[log][cp.first->entry.id.parts.trace_id].emplace_back(begins.size());
                        begins.emplace_back(tuple, clazz);
                    }
                }
                train_and_dump_to_csv(sqm.multiple_logs, W1, ser_path+"_Refinement_Exists_"+act, begins, "Exists", "Absence", true, sqm.multiple_logs.size(),numerical, categorical, act );
            }
        }
    }

    // Now, going for the binary patterns.
    // First, we are mining the binary patterns that binary and not shared across the logs
    FastDatalessClause cache_clause;
    cache_clause.n = 2;
    for (const auto& [log_name, fis] : frequent_itemsets) {
        auto& g = gv[log_name];
        uint64_t minimum_support_threshold = std::min((uint32_t)std::ceil((double)(sqm.multiple_logs[log_name].db.nTraces()) * mining_supp), (sqm.multiple_logs[log_name].db.nTraces()));
        size_t min_int_supp_patt = std::ceil(((double)mining_supp) * (minimum_support_threshold));
        auto& rc = rcv[log_name];
        auto& used = usedv[log_name];

        for (const auto& binary_pattern : fis) {
            auto it = binary_pattern.second.begin();
            auto A = *it;
            it++;
            auto B = *it;
            g.mine_for_AB_clauses(mining_supp, polyadic, A, B, cache_clause, forAllLogsCacheMap[log_name], min_int_supp_patt, rc, used, binary_pattern);
        }
    }

    // TODO: mining for the remaining parts

    for (auto& [log_name, g]: gv) {
        uint64_t minimum_support_threshold = std::min((uint32_t)std::ceil((double)(sqm.multiple_logs[log_name].db.nTraces()) * mining_supp), (sqm.multiple_logs[log_name].db.nTraces()));
        g.finalise_run(minimum_support_threshold, usedv[log_name]);
    }

    return  {-1,-1};
}


std::pair<double,double> algorithmic_strategy::polyadic_dataless_mining_and_refinement(double mining_supp,
                                                                                       bool polyadic, bool reduction, ServerQueryManager &sqm,
                                               std::unordered_map<std::string, std::set<std::tuple<std::string, std::string, std::string>>> &diff) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    for (const auto& [log_name, kb] : sqm.multiple_logs) {
        polyadic_bolt g;
        g.run(mining_supp, polyadic, &kb.db);
        for (const auto& clause : g.Phi) {
            if (clause.clause.casusu == "Choice" || clause.clause.casusu == "CoExistence" || clause.clause.casusu == "ExclChoice") {
                if (clause.clause.left<clause.clause.right) {
                    M[log_name].emplace(clause.clause.casusu, clause.clause.left, clause.clause.right);
                } else {
                    M[log_name].emplace(clause.clause.casusu, clause.clause.right, clause.clause.left);
                }
            } else if ((clause.clause.n == 2) && (!clause.clause.right.empty())) {
                M[log_name].emplace(clause.clause.casusu, clause.clause.left, clause.clause.right);
            } else {
                M[log_name].emplace(clause.clause.casusu, clause.clause.left, "§"+std::to_string(clause.clause.n));
            }
        }
        for (const auto& [log_name, kb] : sqm.multiple_logs) {
            for_diff[log_name] = {};
        }
    }
    for (auto& [log, ref]: for_diff) {
        for (const auto& [curr_log, set] : M) {
            if (log != curr_log) {
                ref.insert(set.begin(), set.end());
            }
        }
    }
    for (const auto& [log_name, set] : M) {
        std::set_difference(set.begin(), set.end(), for_diff[log_name].begin(), for_diff[log_name].end(), std::inserter(diff[log_name], diff[log_name].begin()));
        if (reduction) {
            auto r_reduction_in_for = high_resolution_clock::now();
            for (const auto& tuple : diff[log_name]) {
                if (std::get<2>(tuple).starts_with("§")) {
                    cp.second.clear();
                    cp.first = std::get<1>(tuple);
                    if (cp.first == "__missing") {
                        continue;
                    }
                    rvs[cp][log_name].emplace_back(tuple);
                } else {
                    if (std::get<1>(tuple) < std::get<2>(tuple)) {
                        cp.first = std::get<1>(tuple);
                        cp.second = std::get<2>(tuple);
                    } else {
                        cp.first = std::get<2>(tuple);
                        cp.second = std::get<1>(tuple);
                    }
                    if ((cp.first == "__missing") || cp.second == "__missing" )
                        continue;
                    if (std::get<0>(tuple) == "Succession") {
                        rvs[cp][log_name].emplace_back("Response", std::get<1>(tuple), std::get<2>(tuple));
                        rvs[cp][log_name].emplace_back("Precedence", std::get<1>(tuple), std::get<2>(tuple));
                    } else if (std::get<0>(tuple) == "ChainSuccession") {
                        rvs[cp][log_name].emplace_back("ChainResponse", std::get<1>(tuple), std::get<2>(tuple));
                        rvs[cp][log_name].emplace_back("ChainPrecedence", std::get<2>(tuple), std::get<1>(tuple));
                    } else if (std::get<0>(tuple) == "CoExistence") {
                        rvs[cp][log_name].emplace_back("RespExistence", std::get<1>(tuple), std::get<2>(tuple));
                        rvs[cp][log_name].emplace_back("RespExistence", std::get<2>(tuple), std::get<1>(tuple));
                    }
                }
            }
            diff[log_name].clear();
            auto r_reduction_in_for_end = high_resolution_clock::now();
            duration<double, std::milli> ms_double = (r_reduction_in_for_end-r_reduction_in_for );
            reduction_time += ms_double.count();
        }
    }
    if (reduction) {
        auto r_reduction_in_for = high_resolution_clock::now();
        polyadic_bolt bsrl_pd;
        simple_declare PSD_SX, PSD_DX;
        constexpr size_t PAR_LEN = length("§");
        for (auto& [pair, map] : rvs) {
            std::vector<std::pair<std::string, std::__cxx11::list<std::tuple<std::string,std::string,std::string>>>> vector_pairs;
            for (auto& [log1, list1] : map) {
                vector_pairs.emplace_back(log1, list1);
            }
            map.clear();
            bool isUnary = pair.second.empty();
            for (size_t i = 0, N = vector_pairs.size(); i<N; i++) {
                auto& list1 = vector_pairs[i].second;
                for (size_t j = i+1; j<N; j++) {
                    auto& list2 = vector_pairs[j].second;
                    for (auto it1 = list1.begin(); it1 != list1.end(); ) {
                        bool oneRemoved = false;
                        if (list2.empty()) break;
                        for (auto it2 = list2.begin(); it2 != list2.end(); ) {
                            oneRemoved = false;
                            if (isUnary) {
                                if ((std::get<0>(*it1) == "Init") && (std::get<0>(*it2) == "Exists") && (std::get<2>(*it2) == "§1")) {
                                    it2 = list2.erase(it2);
                                } else if ((std::get<0>(*it2) == "Init") && (std::get<0>(*it1) == "Exists") && (std::get<2>(*it1) == "§1")) {
                                    it1 = list1.erase(it1);
                                    oneRemoved = true;
                                    break;
                                } else if ((std::get<0>(*it1) == "Exists") && (std::get<0>(*it2) == "Exists") && (std::stoul(std::get<2>(*it1).substr(PAR_LEN)) < std::stoul(std::get<2>(*it2).substr(PAR_LEN)))) {
                                    it1 = list1.erase(it1);
                                    oneRemoved = true;
                                    break;
                                } else if ((std::get<0>(*it1) == "Exists") && (std::get<0>(*it2) == "Exists") && (std::stoul(std::get<2>(*it1).substr(PAR_LEN)) > std::stoul(std::get<2>(*it2).substr(PAR_LEN)))) {
                                    it2 = list2.erase(it2);
                                } else if ((std::get<0>(*it1) == "Absence") && (std::get<0>(*it2) == "Absence") && (std::stoul(std::get<2>(*it1).substr(PAR_LEN)) < std::stoul(std::get<2>(*it2).substr(PAR_LEN)))) {
                                    it2 = list2.erase(it2);
                                } else if ((std::get<0>(*it1) == "Absence") && (std::get<0>(*it2) == "Absence") && (std::stoul(std::get<2>(*it1).substr(PAR_LEN)) > std::stoul(std::get<2>(*it2).substr(PAR_LEN)))) {
                                    it1 = list1.erase(it1);
                                    oneRemoved = true;
                                    break;
                                }
                                else {
                                    ++it2;
                                }
                            } else {
                                PSD_SX.first = std::get<0>(*it1);
                                PSD_DX.first = std::get<0>(*it2);
                                PSD_SX.second = std::get<1>(*it1) <= std::get<2>(*it1);
                                PSD_DX.second = std::get<1>(*it2) <= std::get<2>(*it2);
                                if (*it1 == *it2) {
                                    it2 = list2.erase(it2);
                                    it1 = list1.erase(it1);
                                    oneRemoved = true;
                                    break;
                                } else if (bsrl_pd.graph.descendantOrEqualTo(PSD_SX, PSD_DX)) {
                                    it2 = list2.erase(it2);
                                } else if (bsrl_pd.graph.descendantOrEqualTo(PSD_DX, PSD_SX)) {
                                    it1 = list1.erase(it1);
                                    oneRemoved = true;
                                    break;
                                } else
                                    ++it2;
                            }
                        }
                        if (!oneRemoved)
                            ++it1;
                    }
                }
            }
            for (auto& [k,v] : vector_pairs) {
                if (!v.empty()) {
                    diff[k].insert( v.begin(), v.end());
                }
            }
        }
        auto r_reduction_in_for_end = high_resolution_clock::now();
        duration<double, std::milli> ms_double = (r_reduction_in_for_end-r_reduction_in_for );
        reduction_time += ms_double.count();
    }

    for (auto& [k,vals] : diff) {
        std::vector<std::tuple<std::string,std::string,std::string>> toAdd, toRemove;
        for (auto it = vals.begin(); it != vals.end(); ) {
            if (std::get<0>(*it) == "Response") {
                auto tmp = *it;
                std::get<0>(tmp) = "Precedence";
                auto it2 = vals.find(tmp);
                if (it2 != vals.end()) {
                    toAdd.emplace_back("Succession", std::get<1>(*it), std::get<2>(*it));
                    it = vals.erase(it);
                    toRemove.emplace_back(*it2);
                }else {
                    ++it;
                }
            } else if (std::get<0>(*it) == "ChainResponse") {
                auto tmp = *it;
                std::get<0>(tmp) = "ChainPrecedence";
                std::swap(std::get<1>(tmp), std::get<2>(tmp));
                auto it2 = vals.find(tmp);
                if (it2 != vals.end()) {
                    toAdd.emplace_back("ChainSuccession", std::get<1>(*it), std::get<2>(*it));
                    it = vals.erase(it);
                    toRemove.emplace_back(*it2);
                }else {
                    ++it;
                }
            } else if (std::get<0>(*it) == "RespExistence") {
                auto tmp = *it;
                std::swap(std::get<1>(tmp), std::get<2>(tmp));
                auto it2 = vals.find(tmp);
                if (it2 != vals.end()) {
                    toAdd.emplace_back("CoExistence", std::get<1>(*it), std::get<2>(*it));
                    it = vals.erase(it);
                    toRemove.emplace_back(*it2);
                } else {
                    ++it;
                }
            }
            else {
                ++it;
            }
        }
        for (const auto& x : toRemove)
            vals.erase(x);
        for (const auto& x : toAdd)
            vals.emplace(x);
    }
    auto r_preprocessing2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = (r_preprocessing2-r_preprocessing1 );
    double total_time =  ms_double.count();
    return {total_time-reduction_time, reduction_time};
}
