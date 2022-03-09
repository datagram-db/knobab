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

#endif //KNOBAB_SEMANTICS_H
