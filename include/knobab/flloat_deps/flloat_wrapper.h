//
// Created by giacomo on 14/11/2021.
//

//#define PYTHON_WORKS

#ifndef KNOBAB_FLLOAT_WRAPPER_H
#define KNOBAB_FLLOAT_WRAPPER_H
#ifdef PYTHON_WORKS
#include <boost/python.hpp>
#else
#include <string>
#endif


class FLLOAT {
#ifdef PTHON_WORKS
    boost::python::object flloat_parser_ltlf, flloat_ltlf;
#endif
public:
    FLLOAT();


    std::string operator()(const std::string& ltlf);


};

#endif //KNOBAB_FLLOAT_WRAPPER_H
