//
// Created by giacomo on 20/01/2022.
//

#ifndef KNOBAB_MAXSATPIPELINE_H
#define KNOBAB_MAXSATPIPELINE_H

#include <knobab/KnowledgeBase.h>
#include <yaucl/bpm/structures/declare/CNFDeclareDataAware.h>
#include "atomization_pipeline.h"
#include "knobab/trace_repairs/DataQuery.h"
#include "yaucl/bpm/structures/ltlf/ltlf_query.h"


struct MAXSatPipeline {
    // Input

    CNFDeclareDataAware* declare_model = nullptr;
    static std::string LEFT_ATOM, RIGHT_ATOM;

    std::unordered_map<declare_templates, ltlf> ltlf_semantics;
    std::unordered_map<std::string , std::vector<size_t>> atomToFormulaId;
    size_t maxFormulaId = 0;
    std::vector<ltlf_query*> fomulaidToFormula;

    MAXSatPipeline();
    DEFAULT_COPY_ASSGN(MAXSatPipeline)


    // DATA
    std::unordered_map<DataQuery, size_t> data_offset;
    std::vector<std::pair<DataQuery, std::vector<std::pair<std::pair<trace_t, event_t>, double>>>> data_accessing;
    std::unordered_map<DeclareDataAware, std::unordered_map<std::pair<bool, std::string>, label_set_t>> declare_atomization;
    std::vector<std::set<size_t>> atomToFormulaOffset;
    std::vector<std::string> toUseAtoms; // This is to ensure the insertion of unique elements to the map!
    std::vector<std::pair<std::pair<trace_t, event_t>, double>> results_cache;

    void pipeline(CNFDeclareDataAware* model,
                  const AtomizingPipeline& atomization) {
        /// Clearing the previous spurious computation values
        clear();

        /// Extracting the predicates from both the LTLf semantics and the data extracted from it
        data_chunk(model, atomization);

        /// First element of the pipeline: given the data query, fills in the
        data_pipeline_first();

        /// Second part of the pipeline: compute each possible instance of the operator that there exists

        /// Third part of the pipeline: aggregate the results together
    }

    void clear();
    void data_chunk(CNFDeclareDataAware* model,
                    const AtomizingPipeline& atomization);
    void data_pipeline_first();

    void
    localExtract(const AtomizingPipeline &atomization,
                 std::vector<std::string> &toUseAtoms,
                 std::unordered_map<std::pair<bool, std::string>, label_set_t> &ref,
                 std::unordered_map<std::string, std::unordered_set<bool>> &collection,
                 const std::unordered_set<std::string> &decomposition, const std::string &collectionMapKey) ;

    void
    generateConcreteQuery(std::vector<std::string> &toUseAtoms,
                          std::vector<std::pair<std::pair<trace_t, event_t>, double>> &empty_result,
                          DeclareDataAware &item,
                          ltlf_query *formula, DataQueryType r);
};


#endif //KNOBAB_MAXSATPIPELINE_H
