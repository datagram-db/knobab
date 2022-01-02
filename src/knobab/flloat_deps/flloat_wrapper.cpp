//
// Created by giacomo on 14/11/2021.
//


#include "knobab//flloat_deps/flloat_wrapper.h"

namespace python = boost::python;

FLLOAT::FLLOAT() {
    Py_Initialize();
    flloat_parser_ltlf = python::import("flloat.parser.ltlf");
    flloat_ltlf = python::import("flloat.ltlf");
}

std::string FLLOAT::operator()(const std::string &ltlf) {
    //std::cout << ltlf << std::endl;
    python::object parser = flloat_parser_ltlf.attr("LTLfParser")();
    return boost::python::extract<std::string>(parser(ltlf).attr("to_automaton")().attr("to_graphviz")().attr("__str__")())();
}
