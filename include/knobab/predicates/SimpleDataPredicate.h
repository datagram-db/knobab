//
// Created by Sam on 15/02/2022.
//

#ifndef KNOBAB_SIMPLEDATAPREDICATE_H
#define KNOBAB_SIMPLEDATAPREDICATE_H

#if 0

#include <string>
#include <variant>
#include <unordered_map>
#include <yaucl/bpm/structures/commons/DataPredicate.h>

#include "yaucl/structures/default_constructors.h"

// 
#include "yaucl/structures/default_constructors.h"
#include "yaucl/bpm/structures/commons/DataPredicate.h"

// 

/* 
 * Sid -> samuel
 * 
 * 
 * */
using env = std::unordered_map<std::string, union_minimal>;


inline bool test_decomposed_data_predicate(const env& e1, const env& e2, const std::string& lhs, const std::string& rhs, const numeric_atom_cases& casusu) {
    auto temp1 = e1.find(lhs), temp2 = e2.find(rhs);
    if((temp1 == e1.end()) || (temp2 == e2.end())){
        return false;
    }

    switch (casusu) {
        case LT:
            return temp1->second < temp2->second;
        case LEQ:
            return temp1->second <= temp2->second;
        case GT:
            return temp1->second > temp2->second;
        case GEQ:
            return temp1->second >= temp2->second;
        case EQ:
            return temp1->second == temp2->second;
        case NEQ:
            return temp1->second != temp2->second;
        case TTRUE:
            return true;
        default:
            return false;

    }
}


class SimpleDataPredicate {
public:
    DEFAULT_CONSTRUCTORS(SimpleDataPredicate);
    SimpleDataPredicate(const std::string& l, const std::string& r, const numeric_atom_cases& c);
    bool operator()(const env& e1, const env& e2) const;

    bool operator==(const SimpleDataPredicate &rhs) const;

    bool operator!=(const SimpleDataPredicate &rhs) const;

    SimpleDataPredicate flip() const {
        return SimpleDataPredicate{rhs, lhs, casusu};
    }

    numeric_atom_cases casusu;
    std::string lhs, rhs;
};



namespace std {
    template <> struct hash<SimpleDataPredicate> {
        const std::hash<std::string> hs;
        size_t operator()(const SimpleDataPredicate& x) const {
            return (hs(x.lhs) ^ hs(x.rhs)) * (size_t)x.casusu;
        }
    };
}

#endif

#endif //KNOBAB_SIMPLEDATAPREDICATE_H
