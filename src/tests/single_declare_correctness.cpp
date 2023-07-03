#include <iostream>
#include <functional>
//#include "submodules/yaucl/submodules/csv.h"
#include "yaucl/learning/DecisionTree.h"
#include "yaucl/bpm/structures/commons/DeclareDataAware.h"
#include "knobab/server/query_manager/Environment.h"
#include "knobab/mining/refinery.h"
#include "args.hxx"
#include "knobab/server/query_manager/ServerQueryManager.h"
#include <gtest/gtest.h>
#include "query_plan.h"

int main() {
    ServerQueryManager sqm;
    std::stringstream ss;
    auto root_folder = std::filesystem::current_path().parent_path();
    std::string base;//{#name};

    std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;
        auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");
        auto declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();
    ss << "load TAB "
       << std::quoted(curr.string())
       <<  " as "
       << std::quoted("log");
    auto tmp = sqm.runQuery(ss.str());
    std::cerr << tmp.first << " && " << tmp.second << std::endl;

    sqm.runQuery(query_plan);
    auto it2 = sqm.planname_to_declare_to_ltlf.find("declare2");



    auto &tmpEnv = sqm.multiple_logs["log"];
    tmpEnv.clearModel(); // initializing the model pipeline
    std::unordered_map<std::string, LTLfQuery> *plans = &it2->second;
    tmpEnv.load_model(declare_file_path);
    tmpEnv.conjunctive_model = model.at(i);
    tmpEnv.experiment_logger.model_parsing_ms = 0;
    tmpEnv.experiment_logger.model_size = model.at(i).size();
    tmpEnv.experiment_logger.model_filename = "Testing";
    bool doPreliminaryFill = true;
    bool ignoreActForAttributes = false;
    bool creamOffSingleValues = true;
    GroundingStrategyConf::pruning_strategy grounding_strategy = GroundingStrategyConf::NO_EXPANSION;
    tmpEnv.set_grounding_parameters(doPreliminaryFill, ignoreActForAttributes, creamOffSingleValues,
                                    grounding_strategy);
    tmpEnv.doGrounding();
    std::string atomj{"p"};
    AtomizationStrategy atom_strategy = AtomizeOnlyOnDataPredicates;
    size_t n = 3;
    tmpEnv.set_atomization_parameters(atomj, n, atom_strategy);
    tmpEnv.init_atomize_tables();
    tmpEnv.first_atomize_model();
    size_t nThreads = 1;
    auto &ref2 = tmpEnv.experiment_logger;
    EnsembleMethods em = PerDeclareSupport;
    OperatorQueryPlan op = FastOperator_v1;
    tmpEnv.set_maxsat_parameters(nThreads, em, op);
    MAXSatPipeline ref(plans, nThreads, BLOCK_STATIC_SCHEDULE, 3);
    ref.final_ensemble = em;
    ref.operators = op;
    marked_event me;
    me.id.parts.type = MARKED_EVENT_ACTIVATION;
    ref.pipeline(&tmpEnv.grounding, tmpEnv.ap, tmpEnv.db);
}

#include "common_tests.cpp"