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

std::pair<double,double> polyadic_dataless_mining_and_refinement(double mining_supp, bool polyadic, bool reduction, ServerQueryManager &sqm,
                                               std::unordered_map<std::string, std::set<std::tuple<std::string, std::string, std::string>>> &diff);

#endif //KNOBAB_SERVER_POLYADIC_MINING_H
