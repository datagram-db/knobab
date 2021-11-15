//
// Created by giacomo on 14/11/2021.
//

#ifndef KNOBAB_FLLOAT_WRAPPER_H
#define KNOBAB_FLLOAT_WRAPPER_H

#include <boost/python.hpp>
#include <yaucl/graphs/graph_join_pm.h>
#include <bzdb/flloat_deps/ParseFFLOATDot.h>
#include <yaucl/graphs/graph_join_pm_conversion.h>
#include <yaucl/graphs/graph_join_pm_algorithms.h>


class FLLOAT {
    boost::python::object flloat_parser_ltlf, flloat_ltlf;
public:
    FLLOAT();


    std::string operator()(const std::string& ltlf);


};

#endif //KNOBAB_FLLOAT_WRAPPER_H
