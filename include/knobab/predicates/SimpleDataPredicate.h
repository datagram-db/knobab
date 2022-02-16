//
// Created by Sam on 15/02/2022.
//

#ifndef KNOBAB_SIMPLEDATAPREDICATE_H
#define KNOBAB_SIMPLEDATAPREDICATE_H


#include <string>
#include <variant>
#include <unordered_map>
#include "yaucl/structures/default_constructors.h"
#include "yaucl/bpm/structures/commons/DataPredicate.h"

// 

/* 
 * Sid -> samuel
 * 
 * 
 * */
using env = std::unordered_map<std::string, union_minimal>;

class SimpleDataPredicate {
public:
    DEFAULT_CONSTRUCTORS(SimpleDataPredicate);
    SimpleDataPredicate(const std::string& l, const std::string& r, const numeric_atom_cases& c);
    bool operator()(const env& e1, const env& e2) const;

    bool operator==(const SimpleDataPredicate &rhs) const;

    bool operator!=(const SimpleDataPredicate &rhs) const;


private:
    numeric_atom_cases casusu;
    std::string lhs, rhs;
};


#endif //KNOBAB_SIMPLEDATAPREDICATE_H
