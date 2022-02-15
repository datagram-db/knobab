//
// Created by Sam on 15/02/2022.
//

#ifndef KNOBAB_DATAPREDICATE_H
#define KNOBAB_DATAPREDICATE_H


#include <string>
#include <variant>

using union_minimal = std::variant<std::string, double>;

enum numeric_atom_cases {
    UNDEFINED,
    LT,
    GT,
    LEQ,
    GEQ,
    EQ,
    NEQ,
    TTRUE
};

class DataPredicate {
public:
    DataPredicate();
    DataPredicate(const union_minimal& min, const numeric_atom_cases& c);
    bool testPredicate(const union_minimal& val) const;

    bool operator==(const DataPredicate &rhs) const;
    bool operator!=(const DataPredicate &rhs) const;
private:
    numeric_atom_cases casusu;
    union_minimal value;
};


#endif //KNOBAB_DATAPREDICATE_H
