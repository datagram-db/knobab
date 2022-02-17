//
// Created by Sam on 15/02/2022.
//

#include "knobab/predicates/SimpleDataPredicate.h"

SimpleDataPredicate::SimpleDataPredicate(const std::string &l, const std::string &r, const numeric_atom_cases &c) {
    lhs = l;
    rhs = r;
    casusu = c;
}

bool SimpleDataPredicate::operator()(const env &e1, const env &e2) const {
    auto temp1 = e1.find(lhs), temp2 = e2.find(rhs);
    if((temp1 == e1.end()) || (temp2 == e2.end())){
        return false;
    }

    switch (casusu) {
        case INTERVAL:
            return false;
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
    }
}

bool SimpleDataPredicate::operator==(const SimpleDataPredicate &right) const {
    return casusu == right.casusu &&
           lhs == right.lhs &&
           rhs == right.rhs;
}

bool SimpleDataPredicate::operator!=(const SimpleDataPredicate &rhs) const {
    return !(rhs == *this);
}
