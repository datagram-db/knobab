//
// Created by giacomo on 14/02/24.
//

#ifndef KNOBAB_SERVER_POLYADIC_LOADING_H
#define KNOBAB_SERVER_POLYADIC_LOADING_H

#include <fstream>
#include "yaucl/learning/decision_tree/dt_predicate.h"
#include "yaucl/bpm/structures/commons/DataPredicate.h"
#include "yaucl/learning/DecisionTree.h"
#include "ServerQueryManager.h"

#include <string>
#include <unordered_set>

std::tuple<double,double,double> polyadic_loader(const std::unordered_set<std::string>& ignore_keys,
                     const std::string& traceDistinguisher,
                     const std::string& path,
                     bool reclassify,
                     ServerQueryManager& sqm);


#endif //KNOBAB_SERVER_POLYADIC_LOADING_H
