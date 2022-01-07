//
// Created by giacomo on 07/01/2022.
//

#ifndef KNOBAB_EVALUATE_EASY_PROP_TO_ATOMS_H
#define KNOBAB_EVALUATE_EASY_PROP_TO_ATOMS_H

#include <knobab/algorithms/atomization_pipeline.h>

class evaluate_easy_prop_to_atoms {
    AtomizingPipeline ap;
    const std::unordered_map<std::string, std::string>* bogus_act_to_atom;
    const std::unordered_map<std::string, semantic_atom_set>* bogus_act_to_set;

public:
    evaluate_easy_prop_to_atoms(const AtomizingPipeline &ap) : ap(ap) {}


    semantic_atom_set operator()(const easy_prop &prop,
                                 const std::unordered_map<std::string, std::string>* bogus_act_to_atom,
                                 const std::unordered_map<std::string, semantic_atom_set>* bogus_act_to_set);

private:
    semantic_atom_set run(const easy_prop &prop);
};


#endif //KNOBAB_EVALUATE_EASY_PROP_TO_ATOMS_H
