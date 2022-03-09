//
// Created by giacomo on 14/11/2021.
//


#include "knobab//flloat_deps/flloat_wrapper.h"
#include <cassert>

#ifdef PYTHON_WORKS
namespace python = boost::python;
#endif

FLLOAT::FLLOAT() {
#ifdef PYTHON_WORKS
    Py_Initialize();
    flloat_parser_ltlf = python::import("flloat.parser.ltlf");
    flloat_ltlf = python::import("flloat.ltlf");
#endif
}

std::string FLLOAT::operator()(const std::string &ltlf) {
#ifdef PYTHON_WORKS
    python::object parser = flloat_parser_ltlf.attr("LTLfParser")();
    return boost::python::extract<std::string>(parser(ltlf).attr("to_automaton")().attr("to_graphviz")().attr("__str__")())();
#else
    assert(false);
#endif
}
