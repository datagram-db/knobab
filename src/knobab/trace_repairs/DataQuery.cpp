//
// Created by giacomo on 20/01/2022.
//

#include "knobab/trace_repairs/DataQuery.h"
#include <cassert>

DataQuery DataQuery::ExistsQuery(const std::string &atom) {
    DataQuery returnable;
    assert(!atom.empty());
    returnable.label = atom;
    returnable.type = DataQueryType::ExistsQuery;
    return returnable;
}

DataQuery DataQuery::InitQuery(const std::string &atom) {
    DataQuery returnable;
    assert(!atom.empty());
    returnable.label = atom;
    returnable.type = DataQueryType::InitQuery;
    return returnable;
}

DataQuery DataQuery::EndsQuery(const std::string &atom) {
    DataQuery returnable;
    assert(!atom.empty());
    returnable.label = atom;
    returnable.type = DataQueryType::EndsQuery;
    return returnable;
}

DataQuery
DataQuery::RangeQuery(const std::string &atom, const std::string &var, const std::variant<std::string, double> &lb,
                                      const std::variant<std::string, double> &ub) {
    DataQuery returnable;
    assert(!atom.empty());
    returnable.label = atom;
    assert(!var.empty());
    returnable.var = var;
    returnable.lower_bound = lb;
    returnable.upper_bound = ub;
    returnable.type = DataQueryType::RangeQuery;
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
