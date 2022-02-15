//
// Created by Sam on 15/02/2022.
//

#include "knobab/predicates/DataPredicate.h"

DataPredicate::DataPredicate() {
    value = "";
    casusu = UNDEFINED;
}

DataPredicate::DataPredicate(const union_minimal &min, const numeric_atom_cases &c) {
    casusu = c;
    value = min;
}

bool DataPredicate::testPredicate(const union_minimal& val) const {
    try {
        std::get<std::string>(value);
        try {
            std::get<std::string>(val);
        }
        catch (const std::bad_variant_access &e) {
            return false;
        }
    }
    catch (const std::bad_variant_access &e) {
        try {
            std::get<double>(value);
        }
        catch (const std::bad_variant_access &e) {
            return false;
        }
    }

    switch (casusu) {
        case UNDEFINED:
            return false;
        case LT:
            return value < val;
        case LEQ:
            return value <= val;
        case GT:
            return value > val;
        case GEQ:
            return value >= val;
        case EQ:
            return value == val;
        case NEQ:
            return value != val;
        case TTRUE:
            return true;
    }
}

bool DataPredicate::operator==(const DataPredicate &rhs) const {
    return casusu == rhs.casusu && value == rhs.value;
}

bool DataPredicate::operator!=(const DataPredicate &rhs) const {
    return !(*this == rhs);
}