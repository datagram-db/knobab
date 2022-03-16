//
// Created by giacomo on 16/03/2022.
//

#ifndef KNOBAB_ALL_OPERATORS_H
#define KNOBAB_ALL_OPERATORS_H

enum OperatorQueryPlan {
    AbidingLogic,
    FastOperator_v1
};

#include <knobab/operators/semantics.h>
#include <knobab/operators/LTLFOperators.h>
#include <knobab/operators/simple_ltlf_operators.h>
#include <knobab/operators/fast_ltlf_operators.h>

#endif //KNOBAB_ALL_OPERATORS_H
