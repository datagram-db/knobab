//
// Created by giacomo on 23/01/23.
//

#ifndef KNOBAB_SERVER_GENERATEGRAPHFROMATOMISEDCLAUSE_H
#define KNOBAB_SERVER_GENERATEGRAPHFROMATOMISEDCLAUSE_H

#include <filesystem>
#include <string>
#include <unordered_set>
#include <yaucl/graphs/FlexibleFA.h>
#include <knobab/server/declare/DeclareDataAware.h>

void generateGraphFromPattern(const std::filesystem::path& cache_path,
                              const DeclareDataAware& atomised_clause,
                              const std::unordered_set<std::string>& sigma,
                              FlexibleFA<size_t, std::string>& result);

#endif //KNOBAB_SERVER_GENERATEGRAPHFROMATOMISEDCLAUSE_H
