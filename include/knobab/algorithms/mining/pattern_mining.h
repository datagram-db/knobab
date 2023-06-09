//
// Created by giacomo on 17/09/22.
//

#ifndef KNOBAB_PATTERN_MINING_H
#define KNOBAB_PATTERN_MINING_H

#include <vector>
#include "knobab/server/tables/KnowledgeBase.h"
#include "knobab/server/declare/DeclareDataAware.h"
#include <knobab/mining/apriori.h>

void bolt_algorithm(const std::string& logger_file,
                    const FeedQueryLoadFromFile& conf,
                    double support,
                    uint16_t iter_num,
                    bool no_stats = false);
std::pair<std::vector<pattern_mining_result<DeclareDataAware>>, double> pattern_mining(const KnowledgeBase& kb,
                                                                    double support,
                                                                    bool naif = false,
                                                                    bool init_end = true,
                                                                    bool special_temporal_patterns = true,
                                                                    bool only_precise_temporal_patterns = false,
                                                                    bool negative_patterns = false);


#include <knobab/server/query_manager/ServerQueryManager.h>

std::vector<std::vector<DeclareDataAware>> classifier_mining(ServerQueryManager sqm,
                                                                                const std::vector<std::string>& model_entry_names,
                                                                    double support,
                                                                       double tau,// @author: Samuel Appleby
                                                                       bool naif = false,
                                                                       bool init_end = true,
                                                                       bool special_temporal_patterns = true,
                                                                       bool only_precise_temporal_patterns = false,
                                                                       bool negative_patterns = false);

#endif //KNOBAB_PATTERN_MINING_H
