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
#include <yaucl/numeric/ssize_t.h>
#include <knobab/queries/DeclareQueryLanguageParser.h>

//#define MAXSatPipeline_PARALLEL

#ifdef MAXSatPipeline_PARALLEL
#include <thread_pool.hpp>
#define PARALLELIZE_LOOP_BEGIN(pool, lower, upper, varA, varB)    (pool).parallelize_loop((lower), (upper), [&](const size_t varA, const size_t varB) {
#define PARALLELIZE_LOOP_END                                      });
#else
#define PARALLELIZE_LOOP_BEGIN(pool, lower, upper, varA, varB)    do { auto varA = (lower); auto varB = (upper);
#define PARALLELIZE_LOOP_END                                      } while(0);
#endif



struct MAXSatPipeline {
    ltlf_query_manager qm;

#ifdef MAXSatPipeline_PARALLEL
    // A global thread pool object, automatically determining the threads with the number of the architecture ones
    thread_pool pool;
#endif

    // Input

    double declare_to_ltlf_time = 0.0;
    double ltlf_query_time = 0.0;
    DeclareQueryLanguageParser dqlp;
    std::unordered_map<std::string, LTLfQuery>* ptr = nullptr;

    CNFDeclareDataAware* declare_model = nullptr;
    static std::string LEFT_ATOM, RIGHT_ATOM;

    //std::unordered_map<declare_templates, ltlf> ltlf_semantics;
    std::unordered_map<std::string , std::vector<size_t>> atomToFormulaId;
    size_t maxFormulaId = 0;
    std::vector<LTLfQuery*> fomulaidToFormula;

    MAXSatPipeline(const std::string& plan_file, const std::string& plan, size_t nThreads);
    DEFAULT_COPY_ASSGN(MAXSatPipeline)



    // DATA
    Result result;
    ssize_t maxPartialResultId = -1;
    std::unordered_map<DataQuery, size_t> data_offset;
    std::vector<std::pair<DataQuery, std::vector<std::pair<std::pair<trace_t, event_t>, double>>>> data_accessing;
    std::unordered_map<std::string, std::unordered_map<std::string,std::vector<size_t>>> data_accessing_range_query_to_offsets;
    std::unordered_set<DeclareDataAware> declare_atomization;
    std::vector<std::set<size_t>> atomToResultOffset;
    std::vector<std::string> toUseAtoms; // This is to ensure the insertion of unique elements to the map!
    size_t barrier_to_range_queries, barriers_to_atfo;
    std::vector<std::vector<std::pair<std::pair<trace_t, event_t>, double>>> atomicPartIntersectionResult;

    void pipeline(CNFDeclareDataAware* model,
                  const AtomizingPipeline& atomization,
                  const KnowledgeBase& kb);

    void clear();
    void data_chunk(CNFDeclareDataAware* model, const AtomizingPipeline& atomization, const KnowledgeBase& kb);
    void actual_query_running(const KnowledgeBase& kb);

    void
    localExtract(const AtomizingPipeline &atomization,
                 std::vector<std::string> &toUseAtoms,
                 std::unordered_map<std::pair<bool, std::string>, label_set_t> &ref,
                 std::unordered_map<std::string, std::unordered_set<bool>> &collection,
                 const std::unordered_set<std::string> &decomposition, const std::string &collectionMapKey) ;

    void
    generateAtomQuery(std::vector<std::string> &toUseAtoms,
                      std::vector<std::pair<std::pair<trace_t, event_t>, double>> &empty_result,
                      DeclareDataAware &item, LTLfQuery *formula, DataQueryType r,
                      size_t numeric_argument);

    void localExtract(const AtomizingPipeline &atomization, std::vector<std::string> &toUseAtoms,
                      std::unordered_map<std::pair<bool, std::string>, label_set_t> &ref,
                      const std::unordered_set<std::string> &decomposition, const std::string &collectionMapKey,
                      bool isNegated);
};


#endif //KNOBAB_MAXSATPIPELINE_H
