//
// Created by giacomo on 20/01/2022.
//

#ifndef KNOBAB_MAXSATPIPELINE_H
#define KNOBAB_MAXSATPIPELINE_H

#include <knobab/KnowledgeBase.h>
#include <yaucl/bpm/structures/declare/CNFDeclareDataAware.h>
#include "atomization_pipeline.h"
#include "knobab/trace_repairs/DataQuery.h"
#include "knobab/queries/LTLfQueryManager.h"
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

enum EnsembleMethods {
    PerDeclareSupport,
    TraceMaximumSatisfiability,
    TraceIntersection,
};

enum OperatorQueryPlan {
    AbidingLogic,
    FastOperator_v1
};

struct MAXSatPipeline {
    LTLfQueryManager qm;

#ifdef MAXSatPipeline_PARALLEL
    // A global thread pool object, automatically determining the threads with the number of the architecture ones
    thread_pool pool;
#endif

    // Input
    double declare_to_ltlf_time = 0.0;
    double ltlf_query_time = 0.0;
    DeclareQueryLanguageParser dqlp;
    std::unordered_map<std::string, LTLfQuery>* ptr = nullptr;
    std::vector<LTLfQuery*> declare_to_query;

    CNFDeclareDataAware* declare_model = nullptr;
    static std::string LEFT_ATOM, RIGHT_ATOM;

    //std::unordered_map<declare_templates, ltlf> ltlf_semantics;
    std::unordered_map<std::string , std::vector<size_t>> atomToFormulaId;
    size_t maxFormulaId = 0;
    std::vector<LTLfQuery*> fomulaidToFormula;

    MAXSatPipeline(const std::string& plan_file, const std::string& plan, size_t nThreads);
    DEFAULT_COPY_ASSGN(MAXSatPipeline)

    EnsembleMethods final_ensemble;
    OperatorQueryPlan operators;

    // Ensemble methods' results
    Result result;
    std::vector<double> support_per_declare;
    std::vector<double> max_sat_per_trace;

    // DATA
    ssize_t maxPartialResultId = -1;
    std::unordered_map<DataQuery, size_t> data_offset;
    std::vector<std::pair<DataQuery, std::vector<std::pair<std::pair<trace_t, event_t>, double>>>> data_accessing;
    std::unordered_map<std::string, std::unordered_map<std::string,std::vector<size_t>>> data_accessing_range_query_to_offsets;
    std::unordered_map<DeclareDataAware, size_t> declare_atomization;
    std::vector<std::set<size_t>> atomToResultOffset;
    std::vector<std::string> toUseAtoms; // This is to ensure the insertion of unique elements to the map!
    size_t barrier_to_range_queries, barriers_to_atfo;
    std::vector<std::vector<std::pair<std::pair<trace_t, event_t>, double>>> atomicPartIntersectionResult;

    void pipeline(CNFDeclareDataAware* model,
                  const AtomizingPipeline& atomization,
                  const KnowledgeBase& kb);

    void clear();


    std::string generateGraph() const { return qm.generateGraph(); }

private:
    void data_chunk(CNFDeclareDataAware* model, const AtomizingPipeline& atomization, const KnowledgeBase& kb);
    std::vector<PartialResult> subqueriesRunning(const KnowledgeBase &kb);
    void abidinglogic_query_running(const std::vector<PartialResult>& results_cache, const KnowledgeBase& kb);
    void fast_v1_query_running(const std::vector<PartialResult>& results_cache, const KnowledgeBase& kb);

    size_t pushAtomDataQuery(const DataQuery &q, bool directlyFromCache);
};


#endif //KNOBAB_MAXSATPIPELINE_H
