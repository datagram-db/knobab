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
                    double support);

std::vector<pattern_mining_result<DeclareDataAware>> pattern_mining(const KnowledgeBase& kb,
                                                                    double support,
                                                                    bool naif,
                                                                    bool init_end,
                                                                    bool special_temporal_patterns,
                                                                    bool only_precise_temporal_patterns,
                                                                    bool negative_patterns);

std::vector<pattern_mining_result<DeclareDataAware>> classifier_mining(const KnowledgeBase& pos,
                                                                       const KnowledgeBase& neg,
                                                                    double support,
                                                                    bool naif,
                                                                    bool init_end,
                                                                       bool special_temporal_patterns,
                                                                       bool only_precise_temporal_patterns);

#endif //KNOBAB_PATTERN_MINING_H
