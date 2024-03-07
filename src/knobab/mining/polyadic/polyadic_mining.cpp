//
// Created by giacomo on 28/02/24.
//

#include "knobab/mining/polyadic/polyadic_mining.h"

#include <string>
#include <unordered_map>
#include <set>
#include <list>


std::pair<double,double> polyadic_dataless_mining_and_refinement(double mining_supp, bool polyadic, bool reduction, ServerQueryManager &sqm,
                                               std::unordered_map<std::string, std::set<std::tuple<std::string, std::string, std::string>>> &diff) {
    std::unordered_map<std::string, std::set<std::tuple<std::string,std::string,std::string>>> for_diff;
    std::unordered_map<std::string, std::set<std::tuple<std::string,std::string,std::string>>> M;
    std::map<std::pair<std::string,std::string>, std::unordered_map<std::string, std::list<std::tuple<std::string,std::string,std::string>>>> rvs;
    std::pair<std::string,std::string> cp;

    double reduction_time = 0;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto r_preprocessing1 = high_resolution_clock::now();
    for (auto& [log_name, kb] : sqm.multiple_logs) {
        for (const auto& [root, children]: kb.db.hierarchy_def) {
            kb.db.event_label_mapper.put(root);
        }
    }

    for (const auto& [log_name, kb] : sqm.multiple_logs) {
        for_diff[log_name] = {};
    }

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
//            std::cout << pair << std::endl;
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
