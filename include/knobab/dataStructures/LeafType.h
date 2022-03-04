//
// Created by giacomo on 03/03/2022.
//

#ifndef KNOBAB_LEAFTYPE_H
#define KNOBAB_LEAFTYPE_H

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

#endif //KNOBAB_LEAFTYPE_H
