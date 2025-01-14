//
// Created by giacomo on 14/02/24.
//

#ifndef KNOBAB_SERVER_POLYADIC_LOADING_H
#define KNOBAB_SERVER_POLYADIC_LOADING_H


#include "commons.h"
#include <fstream>
#include  "yaucl/learning/dt/dt_predicate.h"
#include "yaucl/bpm/structures/commons/DataPredicate.h"
#include "yaucl/learning/DecisionTree.h"
#include "ServerQueryManager.h"
#include <string>
#include <unordered_set>
//
//static inline void print_rawpayload_csv_header(std::ostream& os,
//                                               const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
//                                               const std::string& all_label = "□",
//                                               const std::string& some_label = "◇",
//                                               bool raw_payload = true) {
//    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
//        const auto& disj = model.at(clazz);
//        const auto M = disj.size();
//        size_t idx = 0;
//        for (const auto& [score,map] : disj) {
//            os << "\"" << all_label << "(";
//            size_t idxj = 0, idxM = map.size();
//            for (const auto& [k,v] : map) {
//                if (raw_payload)
//                    os << "__raw_payload";
//                else
//                    os << v.label;
//                os << "." << v;
//                idxj++;
//                if (idxj != (idxM)) os << "∧";
//            }
//            os << ")\",\"" << some_label << "(";
//            idxj = 0;
//            for (const auto& [k,v] : map) {
//                if (raw_payload)
//                    os << "__raw_payload";
//                else
//                    os << v.label;
//                os << "." << v;
//                idxj++;
//                if (idxj != (idxM)) os << "∧";
//            }
//            idx++;
//            if ((idx == M) && (clazz == (N-1)))
//                os << "\"";
//            else os << "\", ";
//        }
//    }
//
//}
//
//static inline void collect_rawpayload_csv_results_row(std::vector<size_t>& results,
//                                                      const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
//                                                      const std::unordered_map<std::string, union_minimal>& payload) {
//    size_t global_idx = 0;
//    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
//        const auto& disj = model.at(clazz);
//        for (const auto& [score,map] : disj) {
//            bool found = true;
//            std::stringstream ss;
//            for (const auto& [k,v] : map) {
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
//            }
//            global_idx++;
//
//        }
//    }
//}

std::tuple<double,double,double,double> polyadic_loader(const std::unordered_set<std::string>& ignore_keys,
                     const std::string& traceDistinguisher,
                     const std::string& path,
                     bool reclassify,
                     ServerQueryManager& sqm,
                     const std::string& fulltime);


#endif //KNOBAB_SERVER_POLYADIC_LOADING_H
