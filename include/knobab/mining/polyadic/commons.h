//
// Created by giacomo on 27/09/24.
//

#ifndef KNOBAB_SERVER_COMMONS_H
#define KNOBAB_SERVER_COMMONS_H

#include <ostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <knobab/server/declare/DeclareDataAware.h>



static inline void print_rawpayload_csv_header(std::ostream& os,
                                               const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
                                               const std::string& all_label = "□",
                                               const std::string& some_label = "◇",
                                               const bool raw_payload = true,
                                               const std::string& actual_label = "__raw_payload",
                                               const bool do_some = true) {
    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
        const auto& disj = model.at(clazz);
        const auto M = disj.size();
        size_t idx = 0;
        std::stringstream os2;
        for (const auto& [score,map] : disj) {
            os << "\"" << all_label << "(";
            size_t idxj = 0, idxM = map.size();
            for (const auto& [k,v] : map) {
                if (raw_payload) {
                    os << actual_label; os2 << actual_label;
                } else {
                    os << v.label; os2 << v.label;
                }
                os << "." << v; os2 << "." << v;
                idxj++;
                if (idxj != (idxM)) {
                    os << "∧";
                    os2 << "∧";
                }
            }
            os << ")\"";
            if (do_some) {
                os << ",\"" << some_label << "(";
                idxj = 0;
                for (const auto& [k,v] : map) {
                    if (raw_payload) {
                        os << actual_label;
                    } else {
                        os << v.label;
                    }
                    os << "." << v;
                    idxj++;
                    if (idxj != (idxM)) {
                        os << "∧";
                    }
                }
            }

            idx++;
//            if ((idx == M) && (clazz == (N-1)))
//                os << "\"";
//            else {
                os << ")\", ";
//            }
            if (idx < M ) {
                os2 << " ⋁ ";
            }
        }
        auto os2s = os2.str();
        os << "\"" << all_label << "(" << os2s << ")\",";
        if (do_some) os << "\""<<some_label << "("<< os2s << ")\"";
        if (clazz != (N-1))
            os << ",";
    }

}

using env3 = std::vector<std::pair<std::string, union_minimal>>;
using env2 = std::map<std::string, union_minimal>;

static inline void collect_rawpayload_csv_results_row2(std::vector<size_t>& results,
                                                      const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
                                                      const env2& payload) {
    size_t global_idx = 0;
    std::pair<std::string,union_minimal> wd{"", 0.0}, ws{"", ""};
    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
        const auto& disj = model.at(clazz);
        bool hasAMatch = false;
        for (const auto& [score,map] : disj) {
            bool found = true;
            std::stringstream ss;
            for (const auto& [k,v] : map) {
                wd.first = ws.first = k;
//                auto it = std::lower_bound(payload.begin(), payload.end(), std::min(wd, ws));
                auto it = payload.find(k);
                if ((it == payload.end()) ? v.testOverSingleVariable(0.0) : v.testOverSingleVariable(it->second)) {

                } else {
                    found = false;
                    break;
                }
            }
            if (found) {
                DEBUG_ASSERT(global_idx<results.size());
                results[global_idx]++;
                hasAMatch = true;
            }
            global_idx++;
        }
        if (hasAMatch) {
            results[global_idx]++;
        }
        global_idx++;
    }
    DEBUG_ASSERT(global_idx == results.size());
}

static inline void collect_rawpayload_csv_results_row3(std::vector<size_t>& results,
                                                      const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
                                                      const env3& payload) {
    size_t global_idx = 0;
    std::pair<std::string,union_minimal> wd{"", 0.0}, ws{"", ""};
    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
        const auto& disj = model.at(clazz);
        bool hasAMatch = false;
        for (const auto& [score,map] : disj) {
            bool found = true;
            std::stringstream ss;
            for (const auto& [k,v] : map) {
                wd.first = ws.first = k;
                auto it = std::lower_bound(payload.begin(), payload.end(), std::min(wd, ws));
//                auto it = payload.find(k);
                if ((it == payload.end()) ? v.testOverSingleVariable(0.0) : v.testOverSingleVariable(it->second)) {

                } else {
                    found = false;
                    break;
                }
            }
            if (found) {
                DEBUG_ASSERT(global_idx<results.size());
                results[global_idx]++;
                hasAMatch = true;
            }
            global_idx++;
        }
        if (hasAMatch) {
            results[global_idx]++;
        }
        global_idx++;
    }
    DEBUG_ASSERT(global_idx == results.size());
}



#include <yaucl/learning/DecisionTree.h>

static inline void actualClauseRefine(std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>> &result,
                                      const std::pair<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> &pair) {
//    DeclareDataAware c = clause;
    for(const std::pair<double,std::vector<dt_predicate>>& cond : pair.second){
        std::unordered_map<std::string, DataPredicate> current_conds;
        bool hasAFalse = false;

        for(const dt_predicate& dt_p : cond.second){
            DataPredicate p;
            p.is_left_for_activation = false;
            p.var = dt_p.field;
            switch (dt_p.pred) {
                case dt_predicate::LEQ_THAN:
                    p.casusu = LEQ;
                    break;
                case dt_predicate::G_THAN:
                    p.casusu = GT;
                    break;
                case dt_predicate::GEQ_THAN:
                    p.casusu = GEQ;
                    break;
                case dt_predicate::IN_SET:
                    p.casusu = EQ;
                    break;
                case dt_predicate::NOT_IN_SET:
                    p.casusu = NEQ;
                    break;
            }

            if(!dt_p.categoric_set.empty()){
                DEBUG_ASSERT(dt_p.categoric_set.size() == 1);
                p.value = *dt_p.categoric_set.begin();
            } else{
                p.value = dt_p.value;
            }

            auto found = current_conds.find(p.var);

            if(found != current_conds.end()){
                // Our path has two conditions on the same var, perform intersection
//                p.intersect_with(found->second);
                if (!found->second.intersect_with(p)) {
                    found->second.casusu = FFALSE;
                    hasAFalse = true;
                    break;
                }
            }
            else{
                current_conds.insert({p.var, p});
            }
        }
        if (hasAFalse) {
            current_conds.clear();
        } else {
            std::string label = "*";
            if (current_conds.contains("__label")) {
                label = current_conds["__label"].var;
                current_conds.erase("__label");
            }
            for (auto& [k,pred] : current_conds) {
                pred.label = label;
            }
            result.emplace_back(pair.first, current_conds);
        }
    }


}

#include <fstream>
#include <knobab/server/query_manager/Environment.h>

inline void train_and_dump_to_csv2(std::unordered_map<std::string, Environment> &sqmenv,
                                   const std::unordered_map<std::string, std::vector<std::vector<size_t>>> &sax_pyload_trace_id,
                                   const std::string &this_path, std::vector<std::pair<env2, int>> &payload_row,
                                   const std::string &all, const std::string &some, bool raw, size_t n_classes,
                                   const std::unordered_set<std::string> &numerical,
                                   const std::unordered_set<std::string> &categorical,
                                   const std::string& actual_label = "__raw_payload",
                                   const bool do_some = true) {
//    auto it = payload_row.begin();
//    auto en = payload_row.end();
//    DecisionTree<env2> dt(it,
//                          en,
//                          n_classes,
//                          [](const auto& map, const std::string& key) {
//                              std::pair<std::string,union_minimal> wd{key, 0.0}, ws{key, ""};
////                              auto it = std::lower_bound(map.begin(), map.end(), std::min(wd, ws));
//                                                                        auto it = map.find(key);
//                              if (it == map.end())
//                                  return (union_minimal)0.0;
//                              else
//                                  return it->second;
//                          },
//                          numerical,
//                          categorical,
//                          ForTheWin::Gini,
//                          0.97,
//                          1,
//                          payload_row.size(),
//                          1,
//                          false,
//                          nullptr,
//                          nullptr,
//                          5);
//    if ((dt.goodness <= 0.5) || (dt.isLeafNode())) {
//        std::cerr << "Avoiding specification as goodness is below 70% or because it is a leaf node: " << dt.goodness << std::endl;
//        return;
//    } else {
//
//    }
//
//    std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> model(n_classes);
//    size_t number_rows = 0;
//    {
//        std::unordered_map<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> result;
//        dt.populate_children_predicates2(result);
//        for (const auto& kv : result) {
//            std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>> current_conds;
//            actualClauseRefine(current_conds, kv);
//            number_rows += (current_conds.size()+(1));
//            model[kv.first] = std::move(current_conds);
//        }
//    }
//
//    std::vector<size_t> resultsVector(number_rows, 0);
//    size_t vlen = 0;
//
//    for (auto& [log, env] : sqmenv) {
//        std::ofstream payload_out{this_path+"_payload_"+log+".csv"};
//        print_rawpayload_csv_header(payload_out, model, all, some, raw, actual_label, do_some);
//        payload_out << std::endl;
//        const std::vector<std::vector<size_t>>& traces = sax_pyload_trace_id.at(log);
//        DEBUG_ASSERT(env.db.nTraces() == traces.size());
//        for (uint32_t sigma_id = 0, n = env.db.nTraces(); sigma_id < n; sigma_id++) {
//            vlen = traces.at(sigma_id).size();
//            for (auto& dim_count : resultsVector) dim_count = 0; // Re-initialization
//
//            for (const auto& offset : traces.at(sigma_id)) {
//                // Collecting the information associated to the payloads of the events
//                collect_rawpayload_csv_results_row2(resultsVector, model, payload_row.at(offset).first);
//            }
//            payload_out << std::accumulate(
//                    resultsVector.begin(),
//                    resultsVector.end(),
//                    std::string(),
//                    [&vlen,do_some](const std::string& a, size_t b) {
//                        return  a + (a.empty() ? "" : ",") + std::to_string(b == vlen ? 1 : 0) + (do_some ? ("," + std::to_string(b >0 ? 1 : 0)) : "");
//                    }
//            );
//            if (sigma_id != (n-1))
//                payload_out << std::endl;
//        }
//    }
}

inline void train_and_dump_to_csv3(std::unordered_map<std::string, Environment> &sqmenv,
                           const std::unordered_map<std::string, std::vector<std::vector<size_t>>> &sax_pyload_trace_id,
                           const std::string &this_path, std::vector<std::pair<env3, int>> &payload_row,
                           const std::string &all, const std::string &some, bool raw, size_t n_classes,
                           const std::unordered_set<std::string> &numerical,
                           const std::unordered_set<std::string> &categorical,
                           const std::string& actual_label = "__raw_payload",
                           const bool do_some = true) {
//    auto it = payload_row.begin();
//    auto en = payload_row.end();
//    DecisionTree<env3> dt(it,
//                                                                    en,
//                                                                    n_classes,
//                                                                    [](const auto& map, const std::string& key) {
//                                                                        std::pair<std::string,union_minimal> wd{key, 0.0}, ws{key, ""};
//                                                                        auto it = std::lower_bound(map.begin(), map.end(), std::min(wd, ws));
////                                                                        auto it = map.find(key);
//                                                                        if (it == map.end())
//                                                                            return (union_minimal)0.0;
//                                                                        else
//                                                                            return it->second;
//                                                                    },
//                                                                    numerical,
//                                                                    categorical,
//                                                                    ForTheWin::Gini,
//                                                                    1.0,
//                                                                    1,
//                          payload_row.size(),
//                                                                    1,
//                                                                    false,
//                                                                    nullptr,
//                                                                    nullptr,
//                                                                    5);
//    if ((dt.goodness <= 0.5) || (dt.isLeafNode())) {
//        std::cerr << "Avoiding specification as goodness is below 70% or is a leaf node: " << dt.goodness << std::endl;
//        return;
//    }
//
//    std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> model(n_classes);
//    size_t number_rows = 0;
//    {
//        std::unordered_map<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> result;
//        dt.populate_children_predicates2(result);
//        for (const auto& kv : result) {
//            std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>> current_conds;
//            actualClauseRefine(current_conds, kv);
//            number_rows += (current_conds.size()+(1));
//            model[kv.first] = std::move(current_conds);
//        }
//    }
//
//    std::vector<size_t> resultsVector(number_rows, 0);
//    size_t vlen = 0;
//
//    for (auto& [log, env] : sqmenv) {
//        std::ofstream payload_out{this_path+"_payload_"+log+".csv"};
//        print_rawpayload_csv_header(payload_out, model, all, some, raw, actual_label, do_some);
//        payload_out << std::endl;
//        const std::vector<std::vector<size_t>>& traces = sax_pyload_trace_id.at(log);
//        DEBUG_ASSERT(env.db.nTraces() == traces.size());
//        for (uint32_t sigma_id = 0, n = env.db.nTraces(); sigma_id < n; sigma_id++) {
//            vlen = traces.at(sigma_id).size();
//            for (auto& dim_count : resultsVector) dim_count = 0; // Re-initialization
//
//            for (const auto& offset : traces.at(sigma_id)) {
//                // Collecting the information associated to the payloads of the events
//                collect_rawpayload_csv_results_row3(resultsVector, model, payload_row.at(offset).first);
//            }
//            payload_out << std::accumulate(
//                    resultsVector.begin(),
//                    resultsVector.end(),
//                    std::string(),
//                    [&vlen,do_some](const std::string& a, size_t b) {
//                        return  a + (a.empty() ? "" : ",") + std::to_string(b == vlen ? 1 : 0) + (do_some ? ("," + std::to_string(b >0 ? 1 : 0)) : "");
//                    }
//            );
//            if (sigma_id != (n-1))
//                payload_out << std::endl;
//        }
//    }
}

#endif //KNOBAB_SERVER_COMMONS_H
