//
// Created by giacomo on 17/09/22.
//

#ifndef KNOBAB_PATTERN_MINING_H
#define KNOBAB_PATTERN_MINING_H
// [DEPRECATED FILE! REMOVE ME]
#include <vector>
#include "knobab/server/tables/KnowledgeBase.h"

std::pair<std::vector<pattern_mining_result<DeclareDataAware>>, double> pattern_mining(const KnowledgeBase& kb,
                                                                    double support,
                                                                    bool naif,
                                                                    bool init_end,
                                                                    bool special_temporal_patterns,
                                                                    bool only_precise_temporal_patterns,
                                                                    bool negative_patterns);
#include <knobab/server/query_manager/ServerQueryManager.h>

std::tuple<std::vector<std::vector<DeclareDataAware>>,double,double> classifier_mining(ServerQueryManager& sqm,
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
                                                                                       bool negative_patterns = false);

#endif //KNOBAB_PATTERN_MINING_H
