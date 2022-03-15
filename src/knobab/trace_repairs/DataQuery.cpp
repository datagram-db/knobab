//
// Created by giacomo on 20/01/2022.
//

#include "knobab/trace_repairs/DataQuery.h"
#include <yaucl/functional/assert.h>

DataQuery DataQuery::ExistsQuery(const std::string &atom, LeafType lt) {
    DataQuery returnable;
    DEBUG_ASSERT(!atom.empty());
    returnable.label = atom;
    returnable.type = DataQueryType::ExistsQuery;
    returnable.t = lt;
    return returnable;
}

DataQuery DataQuery::AtomQueries (DataQueryType t, const std::string &atom, LeafType lt) {
    DataQuery returnable;
    DEBUG_ASSERT(!atom.empty());
    returnable.label = atom;
    returnable.type = t;
    returnable.t = lt;
    return returnable;
}

DataQuery DataQuery::InitQuery(const std::string &atom) {
    DataQuery returnable;
    DEBUG_ASSERT(!atom.empty());
    returnable.label = atom;
    returnable.type = DataQueryType::InitQuery;
    return returnable;
}

DataQuery DataQuery::AtomQuery(const std::string &atom, LeafType lt) {
    DataQuery returnable;
    DEBUG_ASSERT(!atom.empty());
    returnable.label = atom;
    returnable.type = DataQueryType::AtomQuery;
    returnable.t = lt;
    return returnable;
}

DataQuery DataQuery::EndsQuery(const std::string &atom) {
    DataQuery returnable;
    DEBUG_ASSERT(!atom.empty());
    returnable.label = atom;
    returnable.type = DataQueryType::EndsQuery;
    return returnable;
}

DataQuery
DataQuery::RangeQuery(const std::string &atom, const std::string &var, const std::variant<std::string, double> &lb,
                                      const std::variant<std::string, double> &ub, LeafType lt) {
    DataQuery returnable;
    DEBUG_ASSERT(!atom.empty());
    returnable.label = atom;
    DEBUG_ASSERT(!var.empty());
    returnable.var = var;
    returnable.lower_bound = lb;
    returnable.upper_bound = ub;
    returnable.type = DataQueryType::RangeQuery;
    returnable.t = lt;
    return returnable;
}

bool DataQuery::operator==(const DataQuery &rhs) const {
    return type == rhs.type &&
           label == rhs.label &&
           var == rhs.var &&
           lower_bound == rhs.lower_bound &&
           upper_bound == rhs.upper_bound;
}

bool DataQuery::operator!=(const DataQuery &rhs) const {
    return !(rhs == *this);
}

bool DataQuery::operator<(const DataQuery &rhs) const {
    if (type < rhs.type)
        return true;
    if (rhs.type < type)
        return false;
    if (label < rhs.label)
        return true;
    if (rhs.label < label)
        return false;
    if (var < rhs.var)
        return true;
    if (rhs.var < var)
        return false;
    if (lower_bound < rhs.lower_bound)
        return true;
    if (rhs.lower_bound < lower_bound)
        return false;
    return upper_bound < rhs.upper_bound;
}

bool DataQuery::operator>(const DataQuery &rhs) const {
    return rhs < *this;
}

bool DataQuery::operator<=(const DataQuery &rhs) const {
    return !(rhs < *this);
}

bool DataQuery::operator>=(const DataQuery &rhs) const {
    return !(*this < rhs);
}
