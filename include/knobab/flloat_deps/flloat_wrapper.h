//
// Created by giacomo on 14/11/2021.
//

#define PYTHON_WORKS

#ifndef KNOBAB_FLLOAT_WRAPPER_H
#define KNOBAB_FLLOAT_WRAPPER_H
#ifdef PYTHON_WORKS
#include <boost/python.hpp>
#include <yaucl/graphs/graph_join_pm.h>
#include <knobab/flloat_deps/ParseFFLOATDot.h>
#include <yaucl/graphs/graph_join_pm_conversion.h>
#include <yaucl/graphs/graph_join_pm_algorithms.h>
#endif


class FLLOAT {
#ifdef PYTHON_WORKS
    boost::python::object flloat_parser_ltlf, flloat_ltlf;
#endif
public:
    FLLOAT();


    std::string operator()(const std::string& ltlf);


};

#endif //KNOBAB_FLLOAT_WRAPPER_H
