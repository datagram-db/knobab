//
// Created by giacomo on 14/11/2021.
//


#include "../../../flloat_wrapper.h"

namespace python = boost::python;

FLLOAT::FLLOAT() {
    Py_Initialize();
    python::object flloat_parser_ltlf = python::import("flloat.parser.ltlf");
    python::object flloat_ltlf = python::import("flloat.ltlf");
    python::object parser = flloat_parser_ltlf.attr("LTLfParser")();
}

std::string FLLOAT::parseToGraphViz(const std::string &ltlf) {
    return boost::python::extract<std::string>(parser("F (a)").attr("to_automaton")().attr("to_graphviz")().attr("__str__")())();
}
