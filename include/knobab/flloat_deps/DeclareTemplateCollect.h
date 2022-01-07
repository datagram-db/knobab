//
// Created by giacomo on 07/01/2022.
//

#ifndef KNOBAB_DECLARETEMPLATECOLLECT_H
#define KNOBAB_DECLARETEMPLATECOLLECT_H

#include <unordered_map>
#include <yaucl/bpm/structures/declare/DeclareDataAware.h>
#include <yaucl/bpm/structures/commons/easy_prop.h>
#include <yaucl/graphs/NodeLabelBijectionFA.h>
#include <yaucl/graphs/FlexibleFA.h>
#include <filesystem>
#include <knobab/flloat_deps/flloat_wrapper.h>

class DeclareTemplateCollect {
    std::unordered_map<std::pair<declare_templates, size_t>, NodeLabelBijectionFA<std::string, easy_prop>> graph_map;
    const std::filesystem::path& element;
    FLLOAT callable; // python callable

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
