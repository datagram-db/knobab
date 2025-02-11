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
#include <yaucl/structures/any_to_uint_bimap.h>

using env3 = std::vector<std::pair<std::string, union_minimal>>;
using env2 = std::map<std::string, union_minimal>;

struct CSV_SERIALIZATION {
    using clazz_t = size_t;
    using disj_case_t = ssize_t;
    using offset_t = size_t;
    using is_all_some_otherwise = bool;
    std::string all_label = "□";
    std::string some_label;
    std::string actual_label = "__raw_payload";
    CSV_SERIALIZATION(const std::string& actual_label,
                        const std::string& all_label,
                      const std::string& some_label) :  all_label{all_label}, actual_label{actual_label}, some_label{some_label}  {

    }

    void init_header_information(const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model) {
        std::tuple<clazz_t, disj_case_t, is_all_some_otherwise> record;
        for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
            std::get<0>(record) = clazz;
            const auto& disj = model.at(clazz);
            if (disj.empty()) continue;
            std::vector<std::pair<std::string,size_t>> components;
            for (size_t offset = 0, M = disj.size(); offset<M; offset++) {
                if (disj.at(offset).second.empty()) continue;
                std::stringstream os2;
                size_t idxj = 0;
                size_t idxM = disj.at(offset).second.size();
                os2 << actual_label << ", ";
                if (idxM == 0) continue;
                for (const auto& [k,v] : disj.at(offset).second) {

                    os2 << v.label << "." << v;
                    if (idxj != (idxM)) {
                        os2 << " ∧ ";
                    }
                    idxj++;
                }
                components.emplace_back(os2.str(), offset);
            }
            if (!components.empty()) {
                for (const auto& [component,offset] : components) {
                    std::get<1>(record) = offset;
                    std::get<2>(record) = true;
                    {
                        auto position_in_csv = order_of_appearance.put(record);
                        std::ostringstream ss;
                        ss << std::quoted(all_label+"("+component+")");
                        auto final_label = std::quoted(all_label+"("+component+")");
                        DEBUG_ASSERT(M.size() == position_in_csv.first);
                        DEBUG_ASSERT(position_in_csv.second);
                        M.emplace_back(ss.str());
                    }
                    {
                        std::get<2>(record) = false;
                        auto position_in_csv = order_of_appearance.put(record);
                        std::ostringstream ss;
                        ss << std::quoted(some_label+"("+component+")");
                        auto final_label = std::quoted(some_label+"("+component+")");
                        DEBUG_ASSERT(M.size() == position_in_csv.first);
                        DEBUG_ASSERT(position_in_csv.second);
                        M.emplace_back(ss.str());
                    }
                }
                auto all_cases = std::accumulate(
                        std::next(components.begin()),
                        components.end(),
                        "("+components[0].first+")",
                        [](std::string a, const std::pair<std::string,size_t>& b) {
                            return a + " ⋁ (" + b.first+")";
                        }
                );
                std::get<1>(record) = -1;
                std::get<2>(record) = true;
                auto position_in_csv = order_of_appearance.put(record);
                {
                    std::ostringstream ss;
                    ss << std::quoted(all_label+"("+all_cases+")");
                    auto final_label = std::quoted(all_label+"("+all_cases+")");
                    DEBUG_ASSERT(M.size() == position_in_csv.first);
                    DEBUG_ASSERT(position_in_csv.second);
                    M.emplace_back(ss.str());
                }
                {
                    std::get<2>(record) = false;
                    position_in_csv = order_of_appearance.put(record);
                    std::ostringstream ss;
                    ss << std::quoted(some_label+"("+all_cases+")");
                    DEBUG_ASSERT(M.size() == position_in_csv.first);
                    DEBUG_ASSERT(position_in_csv.second);
                    M.emplace_back(ss.str());
                }
            }
        }
    }

    void write_header(std::ostream& os) {
        for (size_t i = 0, N = M.size(); i<N; i++) {
            os << M.at(i);
            if (i != (N-1)) {
                os << ",";
            }
        }
        os << std::endl;
    }

    size_t size() const {
        return M.size();
    }

    void serialize_to_vector(std::ostream& file,
                             const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>>& model,
                             const std::vector<std::vector<std::pair<std::string,union_minimal>>>& X,
                             const std::vector<std::vector<size_t>>& traces,
                             size_t sigma_id) {
//        const auto sizeCurr = size();
        static std::vector<int> resultsVector;
        resultsVector.resize(size());
        for (auto& val : resultsVector) val = 0;
        for (const auto& offset : traces.at(sigma_id)) {
            // Collecting the information associated to the payloads of the events
            collect_rawpayload_csv_results_row(resultsVector, model, X[offset]);
        }
        auto vlen = traces.at(sigma_id).size();
        std::tuple<clazz_t, disj_case_t, is_all_some_otherwise> record_true{0,-1,true}, record_false{0,-1,false};
//        const auto nowSize = size();
        DEBUG_ASSERT(resultsVector.size() == size());
        for (size_t i = 0, N = size(); i<N; i++) {
            const auto& val = order_of_appearance.get(i);
            size_t clazz = std::get<0>(val);
            std::get<0>(record_true) = std::get<1>(record_false) = clazz;
            if (std::get<2>(val)) {
                resultsVector[i] = (vlen == resultsVector[i]) ? 1 : -1;
            } else {
                resultsVector[i] = (resultsVector[i]>0) ? 1 : -1;
            }
        }
        for (size_t i = 0, N = size(); i<N; i++) {
            file << resultsVector[i];
            if (i != (N-1))
                file << ",";
            else
                file << std::endl;
        }
    }

private:
    std::vector<std::string> M;
    yaucl::structures::any_to_uint_bimap<std::tuple<clazz_t, disj_case_t,bool>> order_of_appearance;
    void collect_rawpayload_csv_results_row(std::vector<int>& results,
                                            const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
                                            const env3& payload) {
        std::tuple<clazz_t, disj_case_t, is_all_some_otherwise> record_true, record_false;
        std::pair<std::string,union_minimal> wd{"", 0.0}, ws{"", ""};
        std::get<2>(record_true) = true;
        std::get<2>(record_false) = false;
        for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
            const auto& disj = model.at(clazz);
            if (disj.empty()) continue;
            std::get<0>(record_true) = std::get<0>(record_false) = clazz;
            bool hasAMatch = false;
            for (size_t offset = 0, M = disj.size(); offset<M; offset++) {
                if (disj.at(offset).second.empty()) continue;
                std::get<1>(record_true) = std::get<1>(record_false) = offset;
                const auto& map = disj.at(offset).second;
                bool found = true;
                std::stringstream ss;
                for (const auto& [k,v] : map) {
                    wd.first = ws.first = k;
                    auto it = std::lower_bound(payload.begin(), payload.end(), std::min(wd, ws));
                    if ((it == payload.end()) ? v.testOverSingleVariable(0.0) : v.testOverSingleVariable(it->second)) {
                    } else {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    const auto key1 = order_of_appearance.getKey(record_true);
                    const auto key2 = order_of_appearance.getKey(record_false);
                    DEBUG_ASSERT(key1 < results.size());
                    DEBUG_ASSERT(key2 < results.size());
                    results[key1]++;
                    results[key2]++;
                    hasAMatch = true; //!some_label.empty();
                }
            }
            std::get<1>(record_true) = std::get<1>(record_false) = -1;
            if (hasAMatch) {
                const auto key1 = order_of_appearance.getKey(record_true);
                const auto key2 = order_of_appearance.getKey(record_false);
                DEBUG_ASSERT(key1 < results.size());
                DEBUG_ASSERT(key2 < results.size());
                results[key1]++;
                results[key2]++;
            }
        }
    }
};

//static inline void print_rawpayload_csv_header(std::ostream& os,
//                                               const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
//                                               const std::string& all_label = "□",
//                                               const std::string& some_label = "◇",
//                                               const bool raw_payload = true,
//                                               const std::string& actual_label = "__raw_payload",
//                                               const bool do_some = true) {
//
//    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
//        const auto& disj = model.at(clazz);
//        const auto M = disj.size();
//        size_t idx = 0;
//        std::stringstream os2;
//        for (const auto& [score,map] : disj) {
//            if (map.empty())
//                continue;
//            os << "\"" << all_label << "(";
//            size_t idxj = 0, idxM = map.size();
//            for (const auto& [k,v] : map) {
//                if (raw_payload) {
//                    os << actual_label;
//                    os2 << actual_label;
//                } else {
//                    os << v.label;
//                    os2 << v.label;
//                }
//                os << "." << v; os2 << "." << v;
//                idxj++;
//                if (idxj != (idxM)) {
//                    os << "∧";
//                    os2 << "∧";
//                }
//            }
//            os << ")\"";
//            if (do_some) {
//                os << ",\"" << some_label << "(";
//                idxj = 0;
//                for (const auto& [k,v] : map) {
//                    if (raw_payload) {
//                        os << actual_label;
//                    } else {
//                        os << v.label;
//                    }
//                    os << "." << v;
//                    idxj++;
//                    if (idxj != (idxM)) {
//                        os << "∧";
//                    }
//                }
//            }
//
//            idx++;
////            if ((idx == M) && (clazz == (N-1)))
////                os << "\"";
////            else {
//                os << ")\", ";
////            }
//            if (idx < M ) {
//                os2 << " ⋁ ";
//            }
//        }
//        auto os2s = os2.str();
//        os << "\"" << all_label << "(" << os2s << ")\",";
//        if (do_some) os << "\""<<some_label << "("<< os2s << ")\"";
//        if (clazz != (N-1))
//            os << ",";
//    }
//
//}
//
//
//static inline void collect_rawpayload_csv_results_row2(std::vector<size_t>& results,
//                                                      const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
//                                                      const env2& payload) {
//    size_t global_idx = 0;
//    std::pair<std::string,union_minimal> wd{"", 0.0}, ws{"", ""};
//    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
//        const auto& disj = model.at(clazz);
//        bool hasAMatch = false;
//        for (const auto& [score,map] : disj) {
//            bool found = true;
//            std::stringstream ss;
//            for (const auto& [k,v] : map) {
//                wd.first = ws.first = k;
////                auto it = std::lower_bound(payload.begin(), payload.end(), std::min(wd, ws));
//                auto it = payload.find(k);
//                if ((it == payload.end()) ? v.testOverSingleVariable(0.0) : v.testOverSingleVariable(it->second)) {
//
//                } else {
//                    found = false;
//                    break;
//                }
//            }
//            if (found) {
//                DEBUG_ASSERT(global_idx<results.size());
//                results[global_idx]++;
//                hasAMatch = true;
//            }
//            global_idx++;
//        }
//        if (hasAMatch) {
//            results[global_idx]++;
//        }
//        global_idx++;
//    }
//    DEBUG_ASSERT(global_idx == results.size());
//}

//static inline void collect_rawpayload_csv_results_row3(std::vector<size_t>& results,
//                                                      const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
//                                                      const env3& payload) {
//    size_t global_idx = 0;
//    std::pair<std::string,union_minimal> wd{"", 0.0}, ws{"", ""};
//    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
//        const auto& disj = model.at(clazz);
//        bool hasAMatch = false;
//        for (const auto& [score,map] : disj) {
//            bool found = true;
//            std::stringstream ss;
//            for (const auto& [k,v] : map) {
//                wd.first = ws.first = k;
//                auto it = std::lower_bound(payload.begin(), payload.end(), std::min(wd, ws));
////                auto it = payload.find(k);
//                if ((it == payload.end()) ? v.testOverSingleVariable(0.0) : v.testOverSingleVariable(it->second)) {
//
//                } else {
//                    found = false;
//                    break;
//                }
//            }
//            if (found) {
//                DEBUG_ASSERT(global_idx<results.size());
//                results[global_idx]++;
//                hasAMatch = true;
//            }
//            global_idx++;
//        }
//        if (hasAMatch) {
//            results[global_idx]++;
//        }
//        if (!disj.empty()) global_idx++;
//    }
//    DEBUG_ASSERT(global_idx == results.size());
//}



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
                    p.casusu = IN_SET;
                    for (const auto& x : dt_p.categoric_set)
                        p.categoric_set.emplace(x);
                    break;
                case dt_predicate::NOT_IN_SET:
                    p.casusu = NOT_IN_SET;
                    for (const auto& x : dt_p.categoric_set)
                        p.categoric_set.emplace(x);
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
                // TODO: problem with string-based operations intersections
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
                auto& current_cond = current_conds["__label"];
                if ((current_cond.casusu == IN_SET) && (current_cond.categoric_set.size() == 1)) {
                    label = current_cond.var;
                    current_conds.erase("__label");
                }
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
                                   const std::string &this_path,
                                   const std::vector<std::vector<std::pair<std::string,union_minimal>>>& X,
                                   const  std::vector<int>& y,
                                   const std::string &all, const std::string &some, bool raw, size_t n_classes,
                                   const std::unordered_set<std::string> &N,
                                   const std::unordered_set<std::string> &C,
                                   const std::string& actual_label = "__raw_payload",
                                   const bool do_some = true) {
    int max = *std::max_element(y.begin(), y.end());
    DecisionTree dt{X,y, N, C, ((size_t)3*(max-1))};
    dt.splitTree(false);

    if ((dt.goodness <= 0.5) || (dt.children.size() == 1)) {
        std::cerr << "Avoiding specification as goodness is below 50% or is a leaf node: " << dt.goodness << std::endl;
        return;
    }

    std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> model(n_classes);
//    size_t number_rows = 0;
    {
        std::unordered_map<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> result;
        dt.populate_children_predicates2(result);
        for (const auto& kv : result) {
            auto& current_conds = model[kv.first];
            actualClauseRefine(current_conds, kv);
//            number_rows += (current_conds.size()+(1));
        }
    }
    CSV_SERIALIZATION ser_collector{ actual_label,all, some};
    ser_collector.init_header_information(model);

//    std::vector<size_t> resultsVector(number_rows, 0);
//    size_t vlen = 0;

    for (auto& [log, env] : sqmenv) {
        std::ofstream payload_out{this_path+"_payload_"+log+".csv"};
        ser_collector.write_header(payload_out);
//        print_rawpayload_csv_header(payload_out, model, all, some, raw, actual_label, do_some);
//        payload_out << std::endl;
        const std::vector<std::vector<size_t>>& traces = sax_pyload_trace_id.at(log);
        DEBUG_ASSERT(env.db.nTraces() == traces.size());
        for (uint32_t sigma_id = 0, n = env.db.nTraces(); sigma_id < n; sigma_id++) {
//            vlen = traces.at(sigma_id).size();
            ser_collector.serialize_to_vector(payload_out, model, X, traces, sigma_id);
        }
    }
}

inline void train_and_dump_to_csv3(std::unordered_map<std::string, Environment> &sqmenv,
                           const std::unordered_map<std::string, std::vector<std::vector<size_t>>> &sax_pyload_trace_id,
                           const std::string &this_path,
const std::vector<std::vector<std::pair<std::string,union_minimal>>>& X,
                                   const  std::vector<int>& y,
                           const std::string &all, const std::string &some, bool raw, size_t n_classes,
                           const std::unordered_set<std::string> &N,
                           const std::unordered_set<std::string> &C,
                           const std::string& actual_label = "__raw_payload",
                           const bool do_some = true) {
    train_and_dump_to_csv2(sqmenv, sax_pyload_trace_id, this_path, X, y, all, some, raw, n_classes, N, C, actual_label, do_some);
//    int max = *std::max_element(y.begin(), y.end());
//    DecisionTree dt{X,y, N, C, ((size_t)5*(max-1))};
//    dt.splitTree(false);
//
//    if ((dt.goodness <= 0.5) || (dt.children.size() == 1)) {
//        std::cerr << "Avoiding specification as goodness is below 50% or is a leaf node: " << dt.goodness << std::endl;
//        return;
//    } else {
//#ifdef DEBUG
//        std::cout << dt.goodness << " as goodness" << std::endl;
//#endif
//    }
//
//    std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> model(n_classes);
//    size_t number_rows = 0;
//    {
//        std::unordered_map<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> results;
//        dt.populate_children_predicates2(results);
//        for (const auto& kv : results) {
//            if (!kv.second.empty()) {
//                std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>> current_conds;
//                actualClauseRefine(current_conds, kv);
//                number_rows += (current_conds.size()+(1));
//                model[kv.first] = std::move(current_conds);
//            }
//        }
//    }
//
//    std::vector<size_t> resultsVector(number_rows, 0);
//    size_t vlen = 0;
//
//    CSV_SERIALIZATION ser_collector{all,  actual_label, raw, 0, do_some ? some : ""};
//    ser_collector.init_header_information(model);
//
//    for (auto& [log, env] : sqmenv) {
//        std::ofstream payload_out{this_path+"_payload_"+log+".csv"};
//        ser_collector.write_header(payload_out);
////        print_rawpayload_csv_header(payload_out, model, all, some, raw, actual_label, do_some);
////        payload_out << std::endl;
//        const std::vector<std::vector<size_t>>& traces = sax_pyload_trace_id.at(log);
//        DEBUG_ASSERT(env.db.nTraces() == traces.size());
//        for (uint32_t sigma_id = 0, n = env.db.nTraces(); sigma_id < n; sigma_id++) {
//            vlen = traces.at(sigma_id).size();
//            ser_collector.serialize_to_vector(payload_out, model, X, traces, sigma_id);
//
//            for (auto& dim_count : resultsVector) dim_count = 0; // Re-initialization
//
//            for (const auto& offset : traces.at(sigma_id)) {
//                // Collecting the information associated to the payloads of the events
//                collect_rawpayload_csv_results_row3(resultsVector, model, X.at(offset));
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
