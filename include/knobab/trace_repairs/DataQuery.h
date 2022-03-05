//
// Created by giacomo on 20/01/2022.
//

#ifndef KNOBAB_DATAQUERY_H
#define KNOBAB_DATAQUERY_H

#include <string>
#include <variant>
#include <yaucl/hashing/hashing.h>
#include <yaucl/hashing/hash_combine.h>
#include <yaucl/structures/default_constructors.h>
#include <knobab/dataStructures/LeafType.h>

enum DataQueryType {
    ExistsQuery,
    AbsenceQuery,
    RangeQuery,
    InitQuery,
    EndsQuery,
    AtomQuery
};


struct DataQuery {
    DataQueryType type;
    std::string label;
    std::string var;
    LeafType   t;
    std::variant<std::string, double> lower_bound, upper_bound;
    size_t numeric_argument;

    static DataQuery AtomQueries (DataQueryType t, const std::string &atom, LeafType lt = NoneLeaf);
    static DataQuery ExistsQuery(const std::string &atom, LeafType lt = NoneLeaf);
    static DataQuery AtomQuery(const std::string &atom, LeafType lt = NoneLeaf);
    static DataQuery InitQuery(const std::string &atom);
    static DataQuery EndsQuery(const std::string &atom);

    static DataQuery
    RangeQuery(const std::string &atom, const std::string &var, const std::variant<std::string, double> &lb, const std::variant<std::string, double> &ub, LeafType lt = NoneLeaf);

    DEFAULT_CONSTRUCTORS(DataQuery);

    bool operator==(const DataQuery &rhs) const;
    bool operator!=(const DataQuery &rhs) const;
    bool operator<(const DataQuery &rhs) const;
    bool operator>(const DataQuery &rhs) const;
    bool operator<=(const DataQuery &rhs) const;
    bool operator>=(const DataQuery &rhs) const;


};

namespace std {
    template<>
    struct hash<struct DataQuery> {
        std::size_t operator()(const struct DataQuery &k) const {
            return yaucl::hashing::hash_combine(yaucl::hashing::hash_combine(yaucl::hashing::hash_combine(
                                                                                     yaucl::hashing::hash_combine(yaucl::hashing::hash_combine(31, ((size_t)k.type)), k.label), k.var),
                                                                             k.lower_bound), k.upper_bound);
        }
    };

}


#endif //KNOBAB_DATAQUERY_H
