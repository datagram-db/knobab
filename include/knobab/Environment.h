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
#include <knobab/flloat_deps/DeclareTemplateCollect.h>
#include <yaucl/bpm/algos/transformations/declare_to_dfa/TemplateCollectResult.h>

struct Environment {
    /// Creating an instance of the knowledge base, that is going to store all the traces in the log!
    KnowledgeBase db;
    AtomizingPipeline ap;
    GroundingStrategyConf grounding_conf;

    DeclareModelParse dmp;
    std::vector<DeclareDataAware> conjunctive_model;

    CNFDeclareDataAware grounding;

    DeclareTemplateCollect declare_to_graph;

    //std::unordered_map<DeclareDataAware, FlexibleFA<size_t, std::string>> pattern_graph;

public:

    double min_threshold = 1.0;
    double c = 2.0;

    /// Data Range Queries
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> range_query(DataPredicate prop) const {
        return db.range_query(prop, min_threshold, c);
    }

    Environment(const std::filesystem::path& cache_folder = "data/cache") : declare_to_graph{cache_folder} {}

    /**
     * Returns the atoms associated to both the declare model and the current knowledge base!
     * @return
     */
    semantic_atom_set  getSigmaAll() const;

    /**
     * Clears all of the bits and pieces, thus preparing into a novel test
     */
    void clear();

    /**
     * Loads the data traces from the log
     * @param format        Format of the log
     * @param loadData      Whether to load the trace and event payloads
     * @param filename      Filename associated to the log
     */
    void load_log(log_data_format format, bool loadData, const std::string &filename);

    /**
     * Loading the Declare model in the Extended format
     * @param model_file
     */
    void load_model(const std::string &model_file);

    void set_atomization_parameters(const std::string &fresh_atom_label = "p",
                                    size_t mslength = MAXIMUM_STRING_LENGTH);
    void set_grounding_parameters(bool doPreliminaryFill = true,
                                  bool ignoreActForAttributes = false,
                                  bool creamOffSingleValues = true,
                                  GroundingStrategyConf::pruning_strategy ps = GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES
    );

    /**
     * Extracting the atomization tables from the recently loaded declare model
     */
    void init_atomize_tables();

    /**
     * Grounding the Declare model containing join clauses
     */
    void doGrounding();

    /**
     * Associating to both the premise and the consequence of the declare statement the numerical
     * atoms associated to them
     */
    void first_atomize_model();

    /**
     * Represents the declare predicate into a pattern format
     * @param decl      Declare statement
     * @return          Pattern graph
     */
    FlexibleFA<size_t, std::string> declare_to_graph_for_patterns(const DeclareDataAware& decl);

    /**
     * Represents the declare pattern inot a graph over which the graph conjunctive join can be run efficiently
     * @param decl          Declare statement
     * @param out_result    Resulting Merged/Union graph
     */
    void compute_declare_to_graph_for_joins(const DeclareDataAware& decl, graph_join_pm &out_result);

    /**
     * Given the atomized model, it generates the final conjunctive graph out of it
     * @param doPrune
     * @return      Joined graph, with some intermediate operands
     */
    TemplateCollectResult compute_declare_for_conjunctive(bool doRemoveUnacceptingStates = false);

    /**
     * Given a propositional formula stating the actions allowed by the formula, where some bogus atom
     * identify a placeholder for other atoms, replaces those with the semantic ones, and uses the
     * environment information for computing the difference (given the universe set, sigmaAll).
     *
     * @param prop
     * @param bogus_act_to_set
     * @return
     */
    semantic_atom_set evaluate_easy_prop_to_atoms(const easy_prop &prop,
                                                  //const std::unordered_map<std::string, std::string>& bogus_act_to_atom,
                                                  const std::unordered_map<std::string, semantic_atom_set>& bogus_act_to_set);

    void print_model(std::ostream& os) const ;
    void print_grounded_model(std::ostream& os) const;
    void print_knowledge_base(std::ostream& os) const;
    void print_grounding_tables(std::ostream& os);

private:
    /**
     * Generating the disjunction among the different possible graphs, by generating the union graph.
     * Such union graphs creates a DFA, where no node is a \varepsilon label, and all of the nodes
     * are clustered by label and collected numbers
     * @param decl          Disjunction of clauses
     * @param out_result    Output graph join graph
     */
    void compute_declare_for_disjunctive(const DisjunctiveDeclareDataAware& decl, graph_join_pm &out_result);

};


#endif //KNOBAB_ENVIRONMENT_H
