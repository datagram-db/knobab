//
// Created by giacomo on 17/09/22.
//

#ifndef KNOBAB_PATTERN_MINING2_H
#define KNOBAB_PATTERN_MINING2_H
// [DEPRECATED FILE! REMOVE ME]
#include <knobab/mining/bolt_commons.h>



std::pair<std::vector<pattern_mining_result<FastDatalessClause>>, double> bolt_algorithm2(const std::string& logger_file,
                    const FeedQueryLoadFromFile& conf,
                    double support,
                    uint16_t iter_num,
                    bool no_stats);

std::pair<std::vector<pattern_mining_result<FastDatalessClause>>, double> bolt2(const KnowledgeBase& kb,
                                                                              double support,
                                                                              bool naif,
                                                                              bool init_end,
                                                                              bool special_temporal_patterns,
                                                                              bool only_precise_temporal_patterns,
                                                                              bool negative_patterns);
#include <knobab/server/query_manager/ServerQueryManager.h>

std::tuple<std::unordered_map<std::string, std::vector<pattern_mining_result<FastDatalessClause>>>,double> bolt2_multilog(ServerQueryManager& sqm,
                                                                                       const std::vector<std::string>& model_entry_names,
                                                                                       double support,
                                                                                       double tau,
                                                                                       double purity,
                                                                                       size_t maxL,
                                                                                       size_t minL,
                                                                                       bool naif = false,
                                                                                    bool init_end = true,
                                                                                    bool special_temporal_patterns = true,
                                                                                    bool only_precise_temporal_patterns = false,
                                                                                    bool negative_ones = false);

std::tuple<std::vector<std::vector<DeclareDataAware>>,double,double> boltk(ServerQueryManager& sqm,
                                                                           const std::vector<std::string>& model_entry_names,
                                                                           double support,
                                                                           double tau,// @author: Samuel Appleby
                                                                                       double purity,
                                                                           size_t maxL,
                                                                           size_t minL,
                                                                           bool naif = false,
                                                                           bool init_end = true,
                                                                           bool special_temporal_patterns = true,
                                                                           bool only_precise_temporal_patterns = false,
                                                                           bool negative_ones = false);

#include <filesystem>

/**
 * Serialising the model to disk
 *
 * @param result_bolt2
 * @param model_name
 * @param path
 * @param logsize
 * @param theta_parameter
 *
 * @authors Samuel 'Buzz' Appleby, Giacomo Bergami
 *
 */
static inline void serialize_bolt2_outcome(const std::vector<pattern_mining_result<FastDatalessClause>>& result_bolt2,
                                           const std::string& model_name,
                                           const std::filesystem::path& path
//                                           size_t logsize,
//                                           double theta_parameter
                                           ) {
    if (!exists(path))
        create_directories(path);
//    auto benchmark = path / "benchmark.csv";
//    bool benchmark_header = !exists(benchmark);
//    {
//        std::ofstream benchmark_file(benchmark, std::ios_base::app);
//        if (benchmark_header)
//            benchmark_file << "algorithm,modelname,logsize,theta_parameter,time_ms" << std::endl;
//        benchmark_file << "Bolt2," << model_name << "," <<  logsize << "," << theta_parameter << "," << result_bolt2.second << std::endl;
////        benchmark_file << std::endl << "DBoltk," << name << "," << clause.support_declarative_pattern;
//    }
    auto model_path = path / model_name;
    if (!exists(model_path)) {
        create_directories(model_path);
    }
    auto model = model_path / "model.powerdecl";
    auto sup_conf = model_path / "sup_conf.tab";
    {
        std::ofstream model_file(model, std::ios_base::trunc);
        std::ofstream sup_conf_file(sup_conf, std::ios_base::trunc);
        model_file << "declare " << std::endl;
        for (size_t i = 0, N = result_bolt2.size(); i<N; ) {
            const pattern_mining_result<FastDatalessClause>& clause = result_bolt2.at(i);
//            std::cout << clause;
            model_file << "\t" << std::quoted(clause.clause.casusu) << " ( " << std::quoted(clause.clause.left) << ", true, ";
            std::string name = clause.clause.casusu + "(" + clause.clause.left;
            if (isUnaryPredicate(clause.clause.casusu)) {
                model_file << clause.clause.n << ")" << std::endl;
            } else {
                model_file << std::quoted(clause.clause.right) << ", true)" << std::endl;
            }
            sup_conf_file << clause.support_declarative_pattern << "\t" << clause.restrictive_confidence_plus_declarative_pattern << "\t" << clause.restrictive_support_declarative_pattern;
            i++;
            if ((i)!=N) {
                sup_conf_file << std::endl;
                model_file << std::endl;
            }
//            if(clause.clause.right_act != "") {
//                name += "," + clause.clause.right_act + ")";
//            }
//            else if (clause.clause.casusu == "Exists" || clause.clause.casusu == "Absence") {
//                name += " " + std::to_string(clause.clause.n) + ")";
//            }
//            else {
//                name += ")";
            }
//            model_file << (!first ? "\n" : "") << clause.clause;
//            first = false;
        }
}

#endif //KNOBAB_PATTERN_MINING_H
