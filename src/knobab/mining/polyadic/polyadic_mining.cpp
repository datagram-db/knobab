//
// Created by giacomo on 28/02/24.
//

#include "knobab/mining/polyadic/polyadic_mining.h"

#include <string>
#include <unordered_map>
#include <set>
#include <list>
#include "knobab/mining/polyadic/commons.h"

std::pair<double,double> algorithmic_strategy::polyadic_dataful_mining_and_refinement(const std::filesystem::path& folder, double mining_supp, bool polyadic, bool reduction, ServerQueryManager &sqm,
                                                                 std::unordered_map<std::string, std::set<std::tuple<std::string, std::string, std::string>>> &diff) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    mining_supp = std::max(std::min(mining_supp, 1.0), 0.0);

    auto dataful_folder = folder / "dataful_comsis";
    if ((!is_directory(dataful_folder)) && (!is_regular_file(dataful_folder))) {
        std::filesystem::create_directories(dataful_folder);
    }

    std::unordered_map<FastDatalessClause, std::vector<std::string>> cvnc; // clause_unary_name_count
    std::unordered_map<std::string, polyadic_bolt> gv; //mining_environment
    std::unordered_map<std::string, std::vector<std::pair<size_t, std::unordered_set<act_t>>>> frequent_itemsets;
    std::vector<size_t> support;
    std::unordered_map<std::string, std::vector<std::string>> act_to_log_name;
    std::pair<std::string,std::string> cp_acts;
    std::unordered_map<std::pair<std::string,std::string>, std::vector<std::pair<std::string,size_t>>> elements;
    std::unordered_map<std::string, std::unordered_set<size_t>> indices_to_remove;
    std::unordered_map<std::string, std::unordered_map<act_t, std::string> > forAllLogsCacheMap;
    std::unordered_map<std::string, result_container> rcv;
    std::unordered_map<std::string,  std::unordered_set<std::pair<act_t,act_t>>> usedv;
    std::unordered_map<std::string, uint32_t> minimum_support_thresholds;
    std::unordered_map<std::string, size_t> min_int_supp_patts;
    std::vector<std::string> activities;

    std::unordered_map<std::string, std::ofstream> dataless_logs;
    std::unordered_map<std::string, std::ofstream> dataful_logs;
    for (const auto& [log_name, kb] : sqm.multiple_logs) {
        dataless_logs.emplace(log_name,
                              (folder / ("output_csv_"+log_name+".csv")).string());
        dataful_logs.emplace(log_name,
                              (dataful_folder / ("output_csv_"+log_name+"_dataful_.csv")).string());
//        activities.insert(kb.db.event_label_mapper.int_to_T.begin(), kb.db.event_label_mapper.int_to_T.end());
        auto& g = gv.emplace(log_name, log_name).first->second;
        auto& ref = frequent_itemsets[log_name];
        auto& currentCache = forAllLogsCacheMap[log_name];
        minimum_support_thresholds[log_name] = std::min((uint32_t)std::ceil((double)(sqm.multiple_logs[log_name].db.nTraces()) * mining_supp), (sqm.multiple_logs[log_name].db.nTraces()));
        min_int_supp_patts[log_name] = std::ceil(((double)mining_supp) * (minimum_support_thresholds[log_name]));

        // Mining just the unary clauses
        support.emplace_back(g.run1(mining_supp, polyadic, &kb.db, ref));

        // Determining which itemsets can be computed non in-tandem
        // 1) Determining which itemsets are shared and which are not
        size_t count_idx = 0;
        for (size_t idx = 0, N = ref.size(); idx<N; idx++) {
            const auto& count = ref.at(idx).first;
            const auto& itemset = ref.at(idx).second;
            if (itemset.size() == 2) {
                auto it = itemset.begin();

                cp_acts.first = kb.db.event_label_mapper.get(*it);
                activities.emplace_back(cp_acts.first);
                currentCache.emplace(*it, cp_acts.first);
                it++;
                cp_acts.second = kb.db.event_label_mapper.get(*it);
                activities.emplace_back(cp_acts.second);
                currentCache.emplace(*it, cp_acts.second);

                if ((cp_acts.first == "__missing")) {
                    indices_to_remove[log_name].emplace(idx);
                    continue;
                }
                if ((cp_acts.second == "__missing"))  {
                    indices_to_remove[log_name].emplace(idx);
                    continue;
                }
                if (cp_acts.first > cp_acts.second)
                    std::swap(cp_acts.first, cp_acts.second);
                elements[cp_acts].emplace_back(log_name, count_idx);
                count_idx++;
            } else {
                auto it = itemset.begin();
                const auto& ref2 = kb.db.event_label_mapper.get(*it);
                currentCache.emplace(*it, ref2);
                indices_to_remove[log_name].emplace(idx);
            }
        }
    }
    remove_duplicates(activities);

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


    std::unordered_set<std::string> categorical{"__label"}, numerical;
    if (refine_init || refine_ends || refine_existentials) {
        for (const auto& [log, env] : sqm.multiple_logs) {
            for (const auto &[key, table]: env.db.attribute_name_to_table) {
                if (table.type != StringAtt)
//                    categorical.emplace(key);
//                else
                    numerical.emplace(key);
            }
        }
    }

    if (refine_init || refine_ends) {
        std::vector<int> beginsY, endsY;
        std::vector<std::vector<std::pair<std::string,union_minimal>>> beginsX, endsX;
//        std::vector<std::pair<std::vector<std::pair<std::string,union_minimal>>, int>> begins, ends;
        std::unordered_map<std::string, std::vector<std::vector<size_t>>> W1, W2;

        size_t bs = 0, es = 0;
        for (const auto& [log, env] : sqm.multiple_logs) {
            if (refine_init) W1[log].resize(env.db.act_table_by_act_id.secondary_index.size());
            if (refine_ends) W2[log].resize(env.db.act_table_by_act_id.secondary_index.size());
            for (const auto &[key, table]: env.db.attribute_name_to_table) {
                for (size_t trace_id = 0, N = env.db.act_table_by_act_id.secondary_index.size(); trace_id < N; trace_id++) {
                    const auto& IDX = env.db.act_table_by_act_id.secondary_index.at(trace_id);
                    if (refine_init) {
                        for (const auto& [act, event_record_ls] : *IDX.first) {
                            bs += event_record_ls.size();
                        }
                    }
                    if (refine_ends) {
                        for (const auto& [act, event_record_ls] : *IDX.second) {
                            for (const auto& record : event_record_ls) {
                                es += event_record_ls.size();
                            }
                        }
                    }
                }
            }
        }

        beginsX.resize(bs); beginsY.resize(bs);
        endsX.resize(es); endsY.resize(bs);
        bs = 0, es = 0;

        for (const auto& [log, env] : sqm.multiple_logs) {
            for (const auto& [key, table] : env.db.attribute_name_to_table) {
                if (table.type != StringAtt)
//                    categorical.emplace(key);
//                else
                    numerical.emplace(key);
            }
            int clazz = std::stoi(log);

            for (size_t trace_id = 0, N = env.db.act_table_by_act_id.secondary_index.size(); trace_id < N; trace_id++) {
                const auto& IDX = env.db.act_table_by_act_id.secondary_index.at(trace_id);
                if (refine_init) {
                    for (const auto& [act, event_record_ls] : *IDX.first) {
                        for (const auto& record : event_record_ls) {
//                            std::vector<std::pair<std::string,union_minimal>>/*&*/ tuple; //= begins[bs].first;
//                            tuple.reserve(env.db.attribute_name_to_table.size()+1);
                            size_t offset = record - env.db.act_table_by_act_id.table.data();
                            beginsX[bs].reserve(env.db.attribute_name_to_table.size());
                            beginsX[bs].emplace_back("__label", env.db.event_label_mapper.get(act));
                            for (const auto& [key, table] : env.db.attribute_name_to_table) {
                                table.resolve_record_if_exists3(offset, beginsX[bs]);
//                                table.resolve_record_if_exists2(offset, tuple);
                            }
                            DEBUG_ASSERT(trace_id < W1[log].size());
                            W1[log][trace_id].emplace_back(bs);
                            std::sort(beginsX[bs].begin(), beginsX[bs].end());
                            beginsY[bs] = clazz;
//                            std::swap(begins[bs].first, tuple);
//                            begins.emplace_back(tuple, clazz);
                            bs++;
                        }
                    }
                }
//                if (refine_ends) {
//                    for (const auto& [act, event_record_ls] : *IDX.second) {
//                        for (const auto& record : event_record_ls) {
////                            std::vector<std::pair<std::string,union_minimal>>/*&*/ tuple;// = ends[es].first;
//                            size_t offset = record - env.db.act_table_by_act_id.table.data();
//                            ends[es].first.reserve(env.db.attribute_name_to_table.size());
//                            ends[es].first.emplace_back("__label", env.db.event_label_mapper.get(act));
////                            tuple["__label"] = env.db.event_label_mapper.get(act);
//                            for (const auto& [key, table] : env.db.attribute_name_to_table) {
//                                table.resolve_record_if_exists3(offset, ends[es].first);
////                                table.resolve_record_if_exists2(offset, tuple);
//                            }
//                            DEBUG_ASSERT(trace_id < W2[log].size());
//                            W2[log][trace_id].emplace_back(ends.size());
//                            std::sort(ends[es].first.begin(), ends[es].first.end());
//                            ends[es].second = clazz;
////                            std::swap(ends[es].first, tuple);
////                            ends.emplace_back(tuple, clazz);
//                            es++;
//                        }
//                    }
//                }
            }
        }
        if (refine_init) {
            auto refinement = dataful_folder / "Refinement_init";
            train_and_dump_to_csv3(sqm.multiple_logs, W1, refinement.string(), beginsX,beginsY, "InitAll", "InitSome", false, sqm.multiple_logs.size(),numerical, categorical );
            beginsX.clear();
        }
        if (refine_ends) {
            for (const auto& [log, env] : sqm.multiple_logs) {
                int clazz = std::stoi(log);
                for (size_t trace_id = 0, N = env.db.act_table_by_act_id.secondary_index.size();
                     trace_id < N; trace_id++) {
                    const auto &IDX = env.db.act_table_by_act_id.secondary_index.at(trace_id);
                    if (refine_init) {
                        for (const auto &[act, event_record_ls]: *IDX.second) {
                            for (const auto &record: event_record_ls) {
                                size_t offset = record - env.db.act_table_by_act_id.table.data();
                                endsX[es].reserve(env.db.attribute_name_to_table.size());
                                endsX[es].emplace_back("__label", env.db.event_label_mapper.get(act));
                                for (const auto &[key, table]: env.db.attribute_name_to_table) {
                                    table.resolve_record_if_exists3(offset, endsX[es]);
                                }
                                DEBUG_ASSERT(trace_id < W2[log].size());
                                W2[log][trace_id].emplace_back(es);
                                std::sort(endsX[es].begin(), endsX[es].end());
                                endsY[bs] = clazz;
                                es++;
                            }
                        }
                    }
                }
            }

            auto refinement = dataful_folder / "Refinement_end";
            train_and_dump_to_csv3(sqm.multiple_logs, W1, refinement.string(), endsX,endsY, "EndAll", "EndSome", false, sqm.multiple_logs.size(),numerical, categorical );
        }
    }

    bool keepFirstEvent = true;
    if (refine_existentials) {
        for (const auto& [act, kb_ids] : act_to_log_name) {
            if (kb_ids.size() > 1) {
                // Actually performing the refinement
                std::unordered_map<std::string, std::vector<std::vector<size_t>>> W1;

                std::vector<int> beginsY;
                std::vector<std::vector<std::pair<std::string,union_minimal>>> beginsX;
                for (const auto& [log, env] : sqm.multiple_logs) {
                    int clazz = std::stoi(log);
                    auto A = env.db.event_label_mapper.get(act);
                    auto cp = env.db.timed_dataless_exists(A);
                    W1[log].resize(env.db.act_table_by_act_id.secondary_index.size());
                    while (cp.first != cp.second) {
                        if ((keepFirstEvent) || (cp.first->entry.id.parts.trace_id != 0)) {
                            auto& tuple = beginsX.emplace_back();
                            size_t offset = cp.first - env.db.act_table_by_act_id.table.data();
                            for (const auto& [key, table] : env.db.attribute_name_to_table) {
                                table.resolve_record_if_exists3(offset, tuple);
                            }
                            W1[log][cp.first->entry.id.parts.trace_id].emplace_back(beginsY.size());
                            beginsY.emplace_back(clazz);
                        }
                        cp.first++;
                    }
                }

                // TODO: provide a refinement based upon the number of the occurrences of the activity numbers
                auto refinement = dataful_folder / ("Refinement_exists_"+act);
                train_and_dump_to_csv2(sqm.multiple_logs, W1, refinement.string(), beginsX,beginsY, "Exists", "", true, sqm.multiple_logs.size(),numerical, categorical, act ,
                                      false);
            }
        }
    }

    // Now, going for the binary patterns.
    // 0. Initalising the same components from fast SAT
    std::unordered_map<std::string, size_t> total_act_counting;
    std::vector<std::string> final_act_order;
    std::unordered_map<std::string, std::unordered_map<size_t, std::vector<size_t>>> act_Labels, noact_Labels;
    for (auto& [log_name, g] : gv) {
        auto* kb = &sqm.multiple_logs[log_name].db;
        auto& refAct = act_Labels[log_name];
        auto& refNoAct = noact_Labels[log_name];
        auto v = kb->actCounting();
        for (const auto& x : activities) {
            auto activity_id = kb->event_label_mapper.get(x);
            total_act_counting[x] += v[activity_id];
            g.collect_activity_existance(kb, x, refAct, refNoAct);
        }
    }
    std::sort(activities.begin(), activities.end(), [&total_act_counting](const std::string& lhs, const std::string& rhs) {
       return total_act_counting.at(lhs) < total_act_counting.at(rhs);
    });
    // Defining the order of visit for the pairs being shared across elements

    std::unordered_map<std::string, std::vector<std::string>> order_of_visit_for_compactness;

    FastDatalessClause clause;
    clause.n = 2;
    std::unordered_map<std::string, polyadic_bolt::result_map_t> result_map;
    payload_act_tracker pat;
    for (const auto& [pair, entries] : elements) {
        if (entries.size()> 1) {
            if (total_act_counting[pair.first] > total_act_counting[pair.second]) {
                order_of_visit_for_compactness[pair.first].emplace_back(pair.second);
            } else {
                order_of_visit_for_compactness[pair.second].emplace_back(pair.first);
            }
        } else {
            // Running the dataless mining over the single log containing the pair
            // and falling back to the old mining algorithm, using the lattice search (Bolt2 w lattice)
            std::string log_name;
            size_t offset;
            std::tie(log_name, offset) = *entries.begin();
            DEBUG_ASSERT(frequent_itemsets[log_name].size() > offset);
            auto& g = gv.find(log_name)->second;
            g.clear();
            g.setKnowledgeBaseAndInit(&sqm.multiple_logs[log_name].db); // also, Phi clear

            const auto& binary_pattern = frequent_itemsets[log_name][offset];
            auto& rc = rcv[log_name];
            auto& used = usedv[log_name];
            auto it = binary_pattern.second.begin();
            auto A = *it;
            it++;
            auto B = *it;
             std::string cA = forAllLogsCacheMap[log_name].at(A);;
            std::string cB = forAllLogsCacheMap[log_name].at(B);
            if (cB > cA) {
                std::swap(A, B);
                std::swap(cA, cB);
            }
            // False: do not finalise the clause insertion in phi!
            pat.clear();
            result_map.clear();
            g.mine_for_AB_clauses(mining_supp, polyadic, A, B, clause, forAllLogsCacheMap[log_name], min_int_supp_patts[log_name], rc, used, binary_pattern, true);
            g.finalise_binary_run(std::ceil(((double)mining_supp) * (sqm.multiple_logs[log_name].db.nTraces())),
                                  used,
                                  A, B);

            for (size_t idx = 0, N = rc.is_clause_present.size(); idx<N; idx++) {
                if (rc.is_clause_present[idx]) {
                    const auto& simple_cls = rc.for_is_clause_present[idx];
                    auto ternary = result_container::get_simple_ternary_clause(rc.for_is_clause_present[idx], cA, cB);
                    rc.complex_operator(simple_cls, g.graph.get(simple_cls), cA, cB, g.log_size, result_map[log_name]);
                    FastDatalessClause x(std::get<0>(ternary), std::get<1>(ternary), std::get<2>(ternary), 2);
                    g.Phi.erase(
                            std::remove_if(g.Phi.begin(), g.Phi.end(),
                                           [&x](const pattern_mining_result<FastDatalessClause> & o) { return o.clause == x; }),
                            g.Phi.end());
                }
            }

            const auto& aAct = act_Labels[log_name][A];
            const auto& bAct = act_Labels[log_name][B];
            const auto& aNoAct = noact_Labels[log_name][A];
            const auto& bNoAct = noact_Labels[log_name][B];
            std::set_intersection(aAct.begin(), aAct.end(), bAct.begin(), bAct.end(), std::back_inserter(pat.A_and_B));
            std::set_union(aAct.begin(), aAct.end(), bAct.begin(), bAct.end(), std::back_inserter(pat.A_or_B));
            std::set_intersection(aNoAct.begin(), aNoAct.end(), bNoAct.begin(), bNoAct.end(), std::back_inserter(pat.neither_ACT));
            std::set_difference(aAct.begin(), aAct.end(), bAct.begin(), bAct.end(), std::back_inserter(pat.A_not_B));
            std::set_difference(bAct.begin(), bAct.end(), aAct.begin(), aAct.end(), std::back_inserter(pat.B_not_A));
            std::set_intersection(pat.A_not_B.begin(), pat.A_not_B.end(), pat.B_not_A.begin(), pat.B_not_A.end(), std::back_inserter(pat.excl_OCC));
            std::vector<size_t> tmp;
            std::set_union(pat.neither_ACT.begin(), pat.neither_ACT.end(), pat.A_and_B.begin(), pat.A_and_B.end(), std::back_inserter(tmp));

            for (const auto& v : g.extra_mining) {
                auto ternary = result_container::get_simple_ternary_clause(v, cA, cB);
                FastDatalessClause x(std::get<0>(ternary), std::get<1>(ternary), std::get<2>(ternary), 2);
                for (const auto& p : g.Phi) {
                    if (p.clause == x) {
                        auto& node = g.graph.get(v);
                        rc.complex_operator(v, node, cA, cB, g.log_size, result_map[log_name]);
                        break;
                    }
                }
            }
            polyadic_bolt::serialize_to_file(result_map[log_name], dataless_logs[log_name]);
            for ( auto& [other_log_name, log_file] : dataless_logs) {
                if (other_log_name != log_name) {
                    size_t N = sqm.multiple_logs[other_log_name].db.nTraces();
                    for (auto& [clause_name, sat_row] : result_map[log_name]) {
                        sat_row.clear();
                        sat_row.resize(N, -2); // Explicitly determining that this is a missing information, as
                        // we cannot foretell the value that this is going to be associated with
                    }
                    polyadic_bolt::serialize_to_file(result_map[log_name], log_file);
                }
            }
            result_map.clear();
        }

    }

    // 1. Mining jointly the shared clauses, so to pertain the information of which traces satisfy or not specific
    //    activation conditions
    FastDatalessClause cache_clause;
    cache_clause.n = 2;
    std::unordered_map<std::string, result_container> rc_map;
    std::unordered_map<std::string, payload_act_tracker> pat_map;
    std::unordered_set<std::string> logs;
    for (auto& [actA, rest] : order_of_visit_for_compactness) {
        std::sort(rest.begin(), rest.end(), [&total_act_counting](const std::string& lhs, const std::string& rhs) {
            return total_act_counting.at(lhs) < total_act_counting.at(rhs);
        });
    }

    payload_preserving Apayloads, Bpayloads;
    act_target_correlation_preserver act_ab, act_ba;

    // Setting the activations and targets for all the clauses of interest
    for (auto& [_, g] : gv) {
        g.pp = {&act_ab, &act_ba};
    }


    std::vector<std::vector<std::pair<std::string,union_minimal>>> X;
    std::vector<int> y;
    std::vector<size_t> correspondences;
    fill_in_policy fip = DistinguishingPayload;
    double sampling_probability = 1.0;
    std::unordered_map<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> model;
    std::unordered_map<std::string, std::unordered_map<std::vector<dt_predicate>, std::vector<std::unordered_set<ActivationCases>>>> results_for_serialization;
    std::unordered_set<std::vector<dt_predicate>> all_predicates_of_interest;

    for (auto it = activities.rbegin(), en = activities.rend(); it != en; ) {
        const auto& actA = *it;
        const auto& rest = order_of_visit_for_compactness.find(actA)->second;
        std::pair<std::string,std::string> cp;
        cp.first = actA;
        bool isADataBeingCollected = false, isBDataBeingCollected =false; // If there is no purpuse for pre-collecting the data, then this is set to false

        for (const auto& actB : rest) {
            isBDataBeingCollected = false;
            Bpayloads.clear();
            cp.second = actB;
            bool hadContained = elements.contains(cp);
            if (!hadContained) // Given that we are sure that the pair was there, if this was not contained, it was because it was originally swapped
                std::swap(cp.first, cp.second);

            logs.clear();
            act_ab.clear();
            act_ba.clear();
            for (const auto& [log_name, offset]: elements[cp]) {
                logs.emplace(log_name);

                DEBUG_ASSERT(frequent_itemsets[log_name].size() > offset);
                auto& g = gv.find(log_name)->second;
                const auto& binary_pattern = frequent_itemsets[log_name][offset];
                auto& rc = rcv[log_name];
                auto& used = usedv[log_name];
                auto it = binary_pattern.second.begin();
                auto A = *it;
                it++;
                auto B = *it;
                const auto& cA = forAllLogsCacheMap[log_name].at(A);;
                const auto& cB = forAllLogsCacheMap[log_name].at(B);
                if (cB != actB)
                    std::swap(A, B);
//                // False: do not finalise the clause insertion in phi!
                pat.clear();
                rc_map[log_name].clear(g.log_size);
                pat_map[log_name].clear();
                result_map[log_name].clear();

                // Collecting the information for determining the clauses that are shared among the logs
                g.collect_dataless_information(actA, A, act_Labels[log_name][A], noact_Labels[log_name][A],
                                               actB, B, act_Labels[log_name][B], noact_Labels[log_name][B],
                                               polyadic,
                                               rc_map[log_name], pat_map[log_name], dataless_logs[log_name], result_map[log_name]);
            }

            // Determining which are the logs that have the clauses being really satisfied, and not just satisfying vacuously or not being represented
            std::unordered_map<simple_declare, std::vector<std::string>> logs_with_sat_clauses;

            /**
             * Se la clausola senza dati non è soddisfatta, allora la sua versione raffinata potrebbe essere o vacuamente
             * soddisfatta (in quanto il raffinamento dei dati non ne consente la attivazione) o non soddisfatta (violazione
             * del raffinamento target o della condizione di correlazione, o comunque violata come sopra). Quindi, se devo
             * distinguere la clausola, devo confrontare tra istanze che sono soddisfatte, in modo da distinguere possibilmente
             * qual è la differenza tra questi.
             */
            for (const auto& log_name: logs) {
                auto& g = gv.find(log_name)->second;
                auto& results = result_map[log_name];
                for (const auto& [triple, scores]: result_map[log_name]) {
                    // Retrieving the identifier from the triple representation
                    const auto& node = result_container::get_simple_ternary_clause(triple, g.all_nodes, cp.first, cp.second);
                    // Getting how many instances are absences
                    size_t count_absences = 0;
                    for (const auto& val : scores) {
                        if ((val == 0) || (val == -2)) {
                            count_absences++;
                        } else
                            break;
                    }
                    const bool is_absent = count_absences == scores.size();
                    if (!is_absent) {
                        // Adding only if the clause is satisfied or not satisfied
                        logs_with_sat_clauses[node].emplace_back(log_name);
                    }
                }
            }

            for (auto it = logs_with_sat_clauses.begin(); it != logs_with_sat_clauses.end(); it++) {
                auto ternary = result_container::get_simple_ternary_clause(it->first, actA, actB);
                size_t shift = it->first.second ? 0 : 1;

                act_target_correlation_preserver* current = it->first.second ? &act_ab : &act_ba;
                DEBUG_ASSERT(current->payload_map.contains(it->first));

                if (it->second.size() > 1) {
                    if (!isADataBeingCollected) {
                        Apayloads.fill_all_activations(sqm.multiple_logs, cp.first);
                        isADataBeingCollected = true;
                    } //else
//                        pp.clear_from_offset(); // Removing all of the payloads from previous insertions

                    /// XXX: start copy
                    X.clear();
                    y.clear();
                    correspondences.clear();
                    model.clear();
                    all_predicates_of_interest.clear();

                    Apayloads.load_activation_with_policy(it->first,
                                                          current->payload_map,
                                                          X,
                                                          y,
                                                          correspondences,
                                                          fip,
                                                          sampling_probability);

                    DecisionTree activations(X, y, numerical, categorical, 5);
                    activations.populate_children_predicates2(model);
                    results_for_serialization.clear();

                    const std::unordered_map<std::string,std::vector<std::unordered_map<ActTable::record*,
                            std::unordered_map<ActivationCases,std::unordered_set<ActTable::record*>>>>>& def2 = current->payload_map.at(it->first);

                    for (const auto& [log_name, traces] : def2) {
                        std::unordered_map<std::vector<dt_predicate>, std::vector<std::unordered_set<ActivationCases>> >& for_log = results_for_serialization[log_name];
                        size_t N = sqm.multiple_logs[log_name].db.nTraces();

                        DEBUG_ASSERT(traces.size() == N);
                        for (size_t trace_id = 0; trace_id < N; trace_id++) {
                            auto& events = traces[trace_id];
                            if (events.empty()) continue;

                            for (const auto& [record_ptr, map_] : events) {
                                const auto* payload = Apayloads.get_payloads_from_offset(log_name, record_ptr);

                                for (const auto& [_, disjunctions] : model) {
                                    for (const auto& [score, alternative] : disjunctions) {
                                        if (trace_id == 0) {
                                            all_predicates_of_interest.emplace(alternative);
                                        }
                                        auto& results = for_log[alternative];
                                        if (results.empty())
                                            results.resize(N);
                                        bool test = dt_predicate::test_conjunctive_predicate(alternative, *payload);
                                        for (const auto& [cases_, S] : map_) {
                                            switch (cases_) {
                                                case ActivationIsViolated:
                                                    if (test) {
                                                        results[trace_id].emplace(cases_);
                                                    } else {
                                                        results[trace_id].emplace(Vacuity);
                                                    }
                                                    break;

                                                case SATActivationWithNoTarget:
                                                case SATActivationWithTarget:
                                                    if (test) {
                                                        results[trace_id].emplace(GenericSATActivation);
                                                    } else {
                                                        results[trace_id].emplace(Vacuity);
                                                    }
                                                    break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    for (const auto& dt : all_predicates_of_interest) {
                        for (auto& [log_name, dataful] : dataful_logs) {
                            size_t N = sqm.multiple_logs[log_name].db.nTraces();
                            auto it2 = results_for_serialization.find(log_name);
                            dataful << "\"" << std::get<0>(ternary) << "(" << std::get<1>(ternary) << "," << std::get<2>(ternary) << ") act " << dt_predicate::conjunction_to_string(dt) << "\",";
                            if (it2 == results_for_serialization.end()) {
                                for (size_t ntrace = 0; ntrace<N; ntrace++) {
                                    dataful << -2;
                                    if (ntrace != (N-1)) dataful << ",";
                                }
                            } else {
                                auto it3 = it2->second.find(dt);
                                if (it3 == it2->second.end()) {
                                    for (size_t ntrace = 0; ntrace<N; ntrace++) {
                                        dataful << -2;
                                        if (ntrace != (N-1)) dataful << ",";
                                    }
                                } else {
                                    for (size_t ntrace = 0; ntrace<N; ntrace++) {
                                        const auto& set = it3->second[ntrace];
                                        if (set.empty() || ((set.size() == 1) && set.contains(Vacuity))) {
                                            dataful << 0;
                                        } else if (set.contains(ActivationIsViolated)) {
                                            dataful << -1;
                                        } else {
                                            dataful << 1;
                                        }
                                        if (ntrace != (N-1)) dataful << ",";
                                    }
                                }
                            }
                        }
                    }
                    

                    //// XXX: end copy

                    // We are collecting target conditions only if the classification outcome from the activation is not good enough
                    if (activations.goodness <= 0.5) {
                        if (!isBDataBeingCollected) {
                            Bpayloads.fill_all_activations(sqm.multiple_logs, cp.second); // filling in the payload from the current ones
                            isBDataBeingCollected = true;
                        }

                        //// XXX: start paste
                        X.clear();
                        y.clear();
                        correspondences.clear();
                        model.clear();
                        all_predicates_of_interest.clear();

                        Apayloads.load_target_with_policy(it->first,
                                                              current->payload_map,
                                                              X,
                                                              y,
                                                              correspondences,
                                                              fip,
                                                              sampling_probability);

                        DecisionTree activations(X, y, numerical, categorical, 5);
                        activations.populate_children_predicates2(model);
                        results_for_serialization.clear();

                        const std::unordered_map<std::string,std::vector<std::unordered_map<ActTable::record*,
                                std::unordered_map<ActivationCases,std::unordered_set<ActTable::record*>>>>>& def2 = current->payload_map.at(it->first);

                        for (const auto& [log_name, traces] : def2) {
                            std::unordered_map<std::vector<dt_predicate>, std::vector<std::unordered_set<ActivationCases>> >& for_log = results_for_serialization[log_name];
                            size_t N = sqm.multiple_logs[log_name].db.nTraces();

                            DEBUG_ASSERT(traces.size() == N);
                            for (size_t trace_id = 0; trace_id < N; trace_id++) {
                                auto& events = traces[trace_id];
                                if (events.empty()) continue;

                                for (const auto& [record_ptr_old, map_] : events) {
                                    for (const auto& [_, S] : map_) {
                                        for (const auto& record_ptr : S) {
                                            const auto* payload = Apayloads.get_payloads_from_offset(log_name, record_ptr);

                                            for (const auto& [_, disjunctions] : model) {
                                                for (const auto& [score, alternative] : disjunctions) {
                                                    if (trace_id == 0) {
                                                        all_predicates_of_interest.emplace(alternative);
                                                    }
                                                    auto& results = for_log[alternative];
                                                    if (results.empty())
                                                        results.resize(N);
                                                    bool test = dt_predicate::test_conjunctive_predicate(alternative, *payload);
                                                    for (const auto& [cases, S] : map_) {
                                                        switch (cases) {
                                                            case ActivationIsViolated:
                                                                if (test) {
                                                                    results[trace_id].emplace(cases);
                                                                } else {
                                                                    results[trace_id].emplace(Vacuity);
                                                                }
                                                                break;

                                                            case SATActivationWithNoTarget:
                                                            case SATActivationWithTarget:
                                                                if (test) {
                                                                    results[trace_id].emplace(GenericSATActivation);
                                                                } else {
                                                                    results[trace_id].emplace(Vacuity);
                                                                }
                                                                break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        for (const auto& dt : all_predicates_of_interest) {
                            for (auto& [log_name, dataful] : dataful_logs) {
                                size_t N = sqm.multiple_logs[log_name].db.nTraces();
                                auto it2 = results_for_serialization.find(log_name);
                                dataful << "\"" << std::get<0>(ternary) << "(" << std::get<1>(ternary) << "," << std::get<2>(ternary) << ") tgt " << dt_predicate::conjunction_to_string(dt) << "\",";
                                if (it2 == results_for_serialization.end()) {
                                    for (size_t ntrace = 0; ntrace<N; ntrace++) {
                                        dataful << -2;
                                        if (ntrace != (N-1)) dataful << ",";
                                    }
                                } else {
                                    auto it3 = it2->second.find(dt);
                                    if (it3 == it2->second.end()) {
                                        for (size_t ntrace = 0; ntrace<N; ntrace++) {
                                            dataful << -2;
                                            if (ntrace != (N-1)) dataful << ",";
                                        }
                                    } else {
                                        for (size_t ntrace = 0; ntrace<N; ntrace++) {
                                            const auto& set = it3->second[ntrace];
                                            if (set.empty() || ((set.size() == 1) && set.contains(Vacuity))) {
                                                dataful << 0;
                                            } else if (set.contains(ActivationIsViolated)) {
                                                dataful << -1;
                                            } else {
                                                dataful << 1;
                                            }
                                            if (ntrace != (N-1)) dataful << ",";
                                        }
                                    }
                                }
                            }
                        }


                        /// XXX: end paste
                    }


                    // At this stage, I ensure that all the elements are loaded correctly.
                } // else: this is not the place for refinement, as there is only one clause with satisfiability, and
                // we will just resort to the data-less classification
            }

            // TODO: cache just the results for B, and mine using collect_dataless_information
            if (!hadContained)
                std::swap(cp.first, cp.second);
        }
        const auto& lastB = *rest.rbegin();
        it++;
        if (it == en)
            break;
        else if (*it == lastB) {
            std::swap(Bpayloads, Apayloads);
            Bpayloads.clear();
        } else {
            Bpayloads.clear();
            Apayloads.clear();
        }
    }


////    PayloadPreserving pp;
//    for (const auto& [actA, rest] : order_of_visit_for_compactness) {
//        // TODO: cache the payload for a by accessing the activated for a, and store the current size, so that it can be cleared the one for B by resizing
//
//
//    }
//    for (const auto& [pair, entries] : elements) {
//        std::unordered_set<std::string> logs;
//        if (entries.size() > 1) {
//            for (const auto& [log_name, offset] : entries) {
//                bool firstInsertion = logs.insert(log_name).second;
//                DEBUG_ASSERT(firstInsertion);
//                DEBUG_ASSERT(frequent_itemsets[log_name].size() > offset);
//                auto& g = gv[log_name];
//                const auto& binary_pattern = frequent_itemsets[log_name][offset];
//                auto& rc = rcv[log_name];
//                auto& used = usedv[log_name];
//                auto it = binary_pattern.second.begin();
//                auto A = *it;
//                it++;
//                auto B = *it;
//                const auto& cA = forAllLogsCacheMap[log_name].at(A);;
//                const auto& cB = forAllLogsCacheMap[log_name].at(B);
//                if (cB > cA)
//                    std::swap(A, B);
//                // False: do not finalise the clause insertion in phi!
//                g.mine_for_AB_clauses(mining_supp, polyadic, A, B, cache_clause, forAllLogsCacheMap[log_name], min_int_supp_patts[log_name], rc, used, binary_pattern, false);
//            }
//            std::unordered_map<simple_declare, std::unordered_map<std::string,const SimpleDeclare*>> matchedClausesFromBoundary;
//            for (const auto& log_name : logs) {
//                auto& rc = rcv[log_name];
//                auto& map = forAllLogsCacheMap[log_name];
//                for (const auto& fast_clause : rc.boundary_result) {
//                    cache_clause.left = map.at(fast_clause.A);
//                    cache_clause.right = map.at(fast_clause.B);
//                    cache_clause.casusu = fast_clause.name.first;
//                    simple_declare fc = fast_clause.name;
//                    matchedClausesFromBoundary[fc].emplace(log_name, &fast_clause);
//                }
//            }
//            for (const auto&  [scl, mp] : matchedClausesFromBoundary) {
//                if (mp.size() == 1) {
//                    // This is the sole instance of the clause, and appears in only one log
//                    auto it = mp.begin();
//                    for (const auto& [log_name, v] : gv) {
//                        if (log_name != it->first) {
//                            const auto& genOrSelf = v.graph.generalise(scl);
//                            if (!genOrSelf.empty()) {
//                                // ???
//                            }
//                        }
//                    }
//                }
////                else if (mp.size() == entries.size()) {
////                    // This clause is present at all levels of the refinements
////                }
//                else {
//
//                }
//            }
//        }
//
//    }
//
//
//    // 2. Mining for the non-shared clauses
//    // 2a) Marking as to remove the items that are binary and already handled in the previous phase, that is, the ones having
//    // more than one other log containing those
//    for (auto it = elements.begin(); it != elements.end();  ) {
//        if ((it->second.size() != 1)) {
//            for (const auto &ref: it->second) {
//                indices_to_remove[ref.first].emplace(
//                        ref.second);  // Not using directly the frequent itemsets for mining the shared clauses
//            }
//            it++;
//        }
//    }
//    elements.clear();
//    for (auto& [k,v] : indices_to_remove) {
//        remove_index(frequent_itemsets[k], std::vector<size_t>(v.begin(), v.end())); // Removing the shared frequent itemset by index, so that those can be handled joinly along the elements of the map.
//        // Also, removing all the  patterns with __missing
//    }
//
//    // 2b) Last, we are mining the binary patterns that binary and not shared across the logs
//    for (const auto& [log_name, fis] : frequent_itemsets) {
//        auto& g = gv[log_name];
//        auto& rc = rcv[log_name];
//        auto& used = usedv[log_name];
//
//        for (const auto& binary_pattern : fis) {
//            auto it = binary_pattern.second.begin();
//            auto A = *it;
//            it++;
//            auto B = *it;
//            g.mine_for_AB_clauses(mining_supp, polyadic, A, B, cache_clause, forAllLogsCacheMap[log_name], min_int_supp_patts[log_name], rc, used, binary_pattern);
//        }
//    }


//    // 3. Last, finalising the collection of the patterns in Phi for each log
//    for (auto& [log_name, g]: gv) {
//        g.finalise_run(minimum_support_thresholds[log_name], usedv[log_name]);
//    }

    exit(200);

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
        polyadic_bolt g{log_name};
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
        std::string bogus;
        polyadic_bolt bsrl_pd{bogus}; // bogus model, just to query the graph for correlations
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
                                } else if ((std::get<0>(*it1) == "End") && (std::get<0>(*it2) == "Exists") && (std::get<2>(*it2) == "§1")) {
                                    it2 = list2.erase(it2);
                                } else if ((std::get<0>(*it2) == "Init") && (std::get<0>(*it1) == "Exists") && (std::get<2>(*it1) == "§1")) {
                                    it1 = list1.erase(it1);
                                    oneRemoved = true;
                                    break;
                                }  else if ((std::get<0>(*it2) == "End") && (std::get<0>(*it1) == "Exists") && (std::get<2>(*it1) == "§1")) {
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
