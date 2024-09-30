//
// Created by giacomo on 28/02/24.
//

#ifndef KNOBAB_SERVER_POLYADIC_MINING_H
#define KNOBAB_SERVER_POLYADIC_MINING_H

#include <string>
#include "polyadic_bolt.h"
#include "ServerQueryManager.h"

template< size_t N >
constexpr size_t length( char const (&)[N] )
{
    return N-1;
}

struct algorithmic_strategy {
    double reduction_time = 0;
    std::unordered_map<std::string, std::set<std::tuple<std::string,std::string,std::string>>> for_diff;
    std::unordered_map<std::string, std::set<std::tuple<std::string,std::string,std::string>>> M;
    std::map<std::pair<std::string,std::string>, std::unordered_map<std::string, std::list<std::tuple<std::string,std::string,std::string>>>> rvs;
    std::pair<std::string,std::string> cp;
    std::chrono::high_resolution_clock::time_point r_preprocessing1;
    ServerQueryManager &sqm;

    algorithmic_strategy(ServerQueryManager &sqm) : sqm{sqm} {
        r_preprocessing1 = std::chrono::high_resolution_clock::now();
        for (auto& [log_name, kb] : sqm.multiple_logs) {
            for (const auto& [root, children]: kb.db.hierarchy_def) {
                kb.db.event_label_mapper.put(root);
            }
        }
    }

std::pair<double,double> polyadic_dataful_mining_and_refinement(const std::string& ser_path, double mining_supp, bool polyadic, bool reduction, ServerQueryManager &sqm,
                                                                 std::unordered_map<std::string, std::set<std::tuple<std::string, std::string, std::string>>> &diff);

std::pair<double,double> polyadic_dataless_mining_and_refinement(double mining_supp, bool polyadic, bool reduction, ServerQueryManager &sqm,
                                               std::unordered_map<std::string, std::set<std::tuple<std::string, std::string, std::string>>> &diff);

};

#endif //KNOBAB_SERVER_POLYADIC_MINING_H
