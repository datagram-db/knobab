//
// Created by giacomo on 07/01/2022.
//

<<<<<<< HEAD
#define PYTHON_RUNS
#ifndef KNOBAB_DECLARETEMPLATECOLLECT_H
#define KNOBAB_DECLARETEMPLATECOLLECT_H


=======
#ifndef KNOBAB_DECLARETEMPLATECOLLECT_H
#define KNOBAB_DECLARETEMPLATECOLLECT_H

>>>>>>> sam
#include <unordered_map>
#include <yaucl/bpm/structures/declare/DeclareDataAware.h>
#include <yaucl/bpm/structures/commons/easy_prop.h>
#include <yaucl/graphs/NodeLabelBijectionFA.h>
#include <yaucl/graphs/FlexibleFA.h>
#include <filesystem>
#ifdef PYTHON_RUNS
#include <knobab/flloat_deps/flloat_wrapper.h>
#endif

class DeclareTemplateCollect {
    std::unordered_map<std::pair<declare_templates, size_t>, NodeLabelBijectionFA<std::string, easy_prop>> graph_map;
    std::filesystem::path element;

#ifdef PYTHON_RUNS
    FLLOAT callable; // python callable
#endif

public:
    const static std::string left_act, right_act;
    DeclareTemplateCollect(const std::filesystem::path& cachePath);

    /**
     * Returning the template graph associated to the sketched template
     * @param type      Template type
     * @param nargs     Associated argument number
     * @return          Template graph
     */
    NodeLabelBijectionFA<std::string, easy_prop> getDeclareTemplate(declare_templates type, size_t nargs);

};


#endif //KNOBAB_DECLARETEMPLATECOLLECT_H
