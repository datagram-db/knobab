//
// Created by giacomo on 09/03/2022.
//

#ifndef KNOBAB_SEMANTICS_H
#define KNOBAB_SEMANTICS_H

#include <optional>
#include <cstdint>
#include <limits>
#include <vector>

#include <yaucl/structures/set_operations.h>

const uint16_t max = std::numeric_limits<uint16_t>::max();
static const std::vector<uint16_t> maxVec(max,max);

#include <knobab/predicates/PredicateManager.h>
#include <knobab/utilities/Aggregators.h>

#include <map>
#include <functional>

using dataContainer = std::vector<std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>>;

enum LeafType {
    ActivationLeaf,
    TargetLeaf,
    NoneLeaf,
    NotALeaf,
    MatchActivationTarget
};

#include <vector>
#include <utility>

using partial_result = std::vector<std::pair<std::pair<uint32_t , uint16_t>, double>>;

template <typename T>
std::vector<uint16_t> populateAndReturnEvents(T it1, T it2){
    std::vector<uint16_t> vec;

    for(auto itr = it1; itr != it2; ++itr){
        for(const auto& r2 : itr->second.second){
            vec.push_back(r2);
        }
    }

    std::sort( vec.begin(), vec.end() );
    vec.erase( std::unique( vec.begin(), vec.end() ), vec.end());

    return vec;
}

template <typename T> void populateAndReturnEvents(T it1, T it2, std::vector<uint16_t>& vec){
    vec.clear();

    for(auto itr = it1; itr != it2; ++itr){
        for(const auto& r2 : itr->second.second){
            vec.push_back(r2);
        }
    }

    std::sort( vec.begin(), vec.end() );
    vec.erase( std::unique( vec.begin(), vec.end() ), vec.end());
}

#endif //KNOBAB_SEMANTICS_H
