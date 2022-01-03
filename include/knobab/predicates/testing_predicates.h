//
// Created by giacomo on 03/01/2022.
//

#ifndef KNOBAB_TESTING_PREDICATES_H
#define KNOBAB_TESTING_PREDICATES_H

#include <yaucl/bpm/structures/commons/DataPredicate.h>
#include <knobab/AttributeTable.h>
#include <knobab/KnowledgeBase.h>

bool test_data_predicate(const DataPredicate& dp,
                         const union_type& val);

bool test_data_predicate(const DataPredicate &dp,
                         const union_minimal &val);

void filter_some_values_from_disjunctive_map(const std::vector<std::unordered_map<std::string, DataPredicate>> &theMapToScan,
                      std::unordered_map<std::string, std::set<union_type>> &theMapToFilter);

bool test_conjunctive_predicate(KnowledgeBase& db,
                                ActTable::record* eventFromTrace,
                                const std::string& expectedActFromDeclare,
                                const std::unordered_map<std::string, DataPredicate>& dp,
                                bool testSingleVariable = true,
                                bool testBiVariables = true);



bool test_disjunctive_predicate(KnowledgeBase& db,
                                ActTable::record* eventFromTrace,
                                const std::string& expectedActFromDeclare,
                                const std::vector<std::unordered_map<std::string, DataPredicate>>& dnf,
                                bool testSingleVariable = true,
                                bool testBiVariables = true);

#endif //KNOBAB_TESTING_PREDICATES_H
