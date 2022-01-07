//
// Created by giacomo on 07/01/2022.
//

#ifndef KNOBAB_ENVIRONMENT_H
#define KNOBAB_ENVIRONMENT_H

#include <knobab/KnowledgeBase.h>
#include <knobab/algorithms/atomization_pipeline.h>
#include <knobab/algorithms/kb_grounding.h>
#include <yaucl/bpm/structures/declare/DeclareModelParse.h>
#include <yaucl/bpm/structures/log/data_loader.h>

struct Environment {
    /// Creating an instance of the knowledge base, that is going to store all the traces in the log!
    KnowledgeBase db;
    AtomizingPipeline ap;
    GroundingStrategyConf grounding_conf;

    DeclareModelParse dmp;
    std::vector<DeclareDataAware> conjunctive_model;

    CNFDeclareDataAware grounding;

public:

    /**
     * Returns the atoms associated to both the declare model and the current knowledge base!
     * @return
     */
    semantic_atom_set  getSigmaAll() const;

    /**
     * Clears all of the bits and pieces, thus preparing into a novel test
     */
    void clear();

    void load_log(log_data_format format, bool loadData, const std::string &filename);
    void load_model(const std::string &model_file);

    void set_atomization_parameters(const std::string fresh_atom_label = "p",
                                    size_t mslength = MAXIMUM_STRING_LENGTH);
    void set_grounding_parameters(bool doPreliminaryFill = true,
                                  bool ignoreActForAttributes = false,
                                  bool creamOffSingleValues = true,
                                  GroundingStrategyConf::pruning_strategy ps = GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES
    );

    void init_atomize_tables();
    void doGrounding();
    void first_atomize_model();


    semantic_atom_set evaluate_easy_prop_to_atoms(const easy_prop &prop,
                                                  const std::unordered_map<std::string, std::string>& bogus_act_to_atom,
                                                  const std::unordered_map<std::string, semantic_atom_set>& bogus_act_to_set);

    void print_model(std::ostream& os) const ;
    void print_grounded_model(std::ostream& os) const;
    void print_knowledge_base(std::ostream& os) const;
    void print_grounding_tables(std::ostream& os);

};


#endif //KNOBAB_ENVIRONMENT_H
