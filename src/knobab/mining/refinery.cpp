#include <iostream>
#include <functional>
//#include "submodules/yaucl/submodules/csv.h"
#include "yaucl/learning/DecisionTree.h"
#include "yaucl/bpm/structures/commons/DeclareDataAware.h"
#include "knobab/server/query_manager/Environment.h"
#include "knobab/mining/refinery.h"
#include "args.hxx"
#include "knobab/server/query_manager/ServerQueryManager.h"

////using result = std::variant<std::monostate, std::pair<DeclareDataAware, DeclareDataAware>>;
////result refine_clause(const DeclareDataAware& dec_temp, const DecisionTree<DeclareDataAware>& tree){
////
////    // Test leaf node, if not candidates, append to vector of candidates per branch, WH
////    result res;
////
////    std::vector<std::pair<dt_predicate,double>> vec_candidates;
////
////    if(!tree.isLeafNode()){
////        const DecisionTree<DeclareDataAware>* success = tree.getSatisfyingConditionSplit();
////        if(success){
////            vec_candidates.push_back(tree.candidate);
////        }
////
////        const DecisionTree<DeclareDataAware>* fail = tree.getUnsatisfyingConditionSplit();
////        if(fail){
////            vec_candidates.push_back(tree.candidate);
////        }
////    }
////
////    return res;
////}
//
//using worlds_activations = std::unordered_map<Environment*, std::vector<payload_data>>;
//using refining_extraction = std::unordered_map<std::vector<Environment*>, std::vector<DeclareDataAware>>;
//
//
//
//
//void refine_clause(refining_extraction& extraction, float theta){
//    std::vector<std::pair<payload_data, int>> V;
//    std::unordered_set<std::string> numeric_keys{};
//    std::unordered_set<std::string> categorical_keys{};
//
//    for(const std::pair<std::vector<Environment*>, std::vector<DeclareDataAware>>& shared_clauses : extraction) {
//        worlds_activations w_activations;
//        std::unordered_map<int, Environment*> tree_to_env;
//
//        // For each pair, we want to laod the model again for all environemnts and then do a decision tree
//        for (const DeclareDataAware& clause : shared_clauses.second) {
//            for(Environment* env: shared_clauses.first){
//                tree_to_env.insert({w_activations.size(), env});
//                std::pair<std::unordered_map<Environment*, std::vector<payload_data>>::iterator, bool> world_it = w_activations.insert(
//                        {env, {}});
//                DEBUG_ASSERT(world_it.second);
//
//                std::vector<DeclareDataAware> mono_clause{clause};
//
//                /* Run the atomization pipeline again but with the shared clauses only */
//                Environment cpy = *env;
//
//                cpy.clearModel();
//                cpy.load_model(mono_clause.begin(), mono_clause.end());
//                cpy.doGrounding();
//                cpy.init_atomize_tables();
//                cpy.first_atomize_model();
//
//                Result result = cpy.query_model().result;
//                RefineOver refiningOn = RefineOverActivation;
//
////                extractPayload(env, world_it, result);
//
//                for (const payload_data &e: world_it.first->second) {
////                    for (auto e_it = e.begin(); e_it != e.end(); e_it++) {
////                        if (std::holds_alternative<double>(e_it->second)) {
////                            numeric_keys.insert(e_it->first);
////                        } else {
////                            categorical_keys.insert(e_it->first);
////                        }
////                    }
//
//                    V.emplace_back(e, w_activations.size() - 1);
//                }
//            }
//
////            std::function<union_minimal(const payload_data &, const std::string &)> selector = [](const payload_data &x,
////                                                                                                const std::string &key) -> union_minimal {
////                auto found = x.find(key);
////                return found != x.end() ? found->second : 0.0;
////            };
//
//            auto it = V.begin(), en = V.end();
//            DecisionTree<payload_data> dt(it,
//                                          en,
//                                          1,
//                                          selector,
//                                          numeric_keys,
//                                          categorical_keys,
//                                          ForTheWin::gain_measures::Entropy,
//                                          0.9,
//                                          1,
//                                          V.size(),
//                                          1);
//
//            std::cout << "----------DECISION TREE----------" << std::endl;
//            dt.print_rec(std::cout, 1);
//
//            if(dt.goodness < theta){
//                /* We can't refine this clause well enough, continue to the next*/
//                std::cout << "CANNOT REFINE:" << dt.goodness  << " < " << theta << std::endl;
//                continue;
//            }
//
//            std::cout << "CAN REFINE:" << dt.goodness  << " >= " << theta << std::endl;
//
//            std::unordered_map<int, std::vector<std::vector<dt_predicate>>> world_to_paths = {};
//            dt.populate_children_predicates(world_to_paths);
//            RefineOver what = RefineOverActivation;
//
//            for(const std::pair<int, std::vector<std::vector<dt_predicate>>>& pair : world_to_paths){
//                std::unordered_map<int, Environment*>::iterator ref = tree_to_env.find(pair.first);
//                DEBUG_ASSERT(ref != tree_to_env.end());
//
//                DeclareDataAware c = actualClauseRefine(clause, what, pair);
//
//                ref->second->conjunctive_model.push_back(c);
//            }
//        }
//    }
//}

#include <magic_enum.hpp>
#include <knobab/mining/pattern_mining.h>

struct ConfusionMatrix {
    std::vector<size_t> expected_prediction;
    std::vector<size_t> actual_prediction;
    double true_positive, true_negative, false_positive, false_negative;

    double precision() const {
        return true_positive / (true_positive+false_positive);
    }
    double recall() const {
        return true_positive / (true_positive+false_positive);
    }
    void compute() {
        for (size_t i = 0, N = expected_prediction.size(); i<N; i++) {
            size_t true_label = expected_prediction.at(i);
            size_t predicted_label = actual_prediction.at(i);
            if (true_label == predicted_label) {
                // True
                if (true_label == 0) {
                    true_positive+=1.0;
                } else {
                    true_negative+=1.0;
                }
            } else {
                // False
                if (predicted_label == 0) {
                    false_positive+=1.0;
                } else {
                    false_negative+=1.0;
                }
            }
        }
    }

    ConfusionMatrix() = default;
    ConfusionMatrix(const ConfusionMatrix&) = default;
    ConfusionMatrix(ConfusionMatrix&&) = default;
    ConfusionMatrix& operator=(const ConfusionMatrix&) = default;
    ConfusionMatrix& operator=(ConfusionMatrix&&) = default;
};

int main(int argc, char **argv) {
    args::ArgumentParser parser("DBolt±", "This extension provides the data-aware declarative mining algorithm presented");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    std::unordered_map<std::string, log_data_format> map{
            {"hrf", log_data_format::HUMAN_READABLE_YAUCL},
            {"xes", log_data_format::XES1},
            {"bar", log_data_format::TAB_SEPARATED_EVENTS}};
    args::Group group(parser, "You can use the following parameters", args::Group::Validators::DontCare, args::Options::Global);
    args::ValueFlag<std::string>  testing_log(group, "Testing log", "The log against which conduct the prediction", {'e', "testing"});
    args::ValueFlag<std::string>  expected_tab(group, "Expected Classes", "The expected prediction classes for the testing log", {'x', "expected"});
    args::ValueFlag<std::string>  testing_f(group, "Testing log format", "The format of the testing log", {'f', "format"});
    args::MapFlagList<std::string, log_data_format> log(parser, "w", "allocates the n-th world associated to a given file to be read in a given format", {'w', "world"}, map);
    args::ValueFlag<double>  tau_val(group, "Tau Value", "If present, specifies the tau value", {'t', "tau"});
    args::ValueFlag<double>  supp_val(group, "Support Value", "If present, specifies the support value", {'s', "supp"});
    args::ValueFlag<std::string> dt_measure(group, "DT Measure", "If present, specifies the quality measure for the decision tree", {'q', "quality"});
    args::ValueFlag<double> dt_purity(group, "DT Minimum Purity", "If present, specifies the minimum purity condition (def=1.)", {'p', "minpure"});
    args::ValueFlag<size_t> dt_max_l(group, "DT Maximum Categoric Set Size", "If present, specifies the maximum size of a categorical set (def=1)", {'l', "maxlen"});
    args::ValueFlag<size_t> dt_min_l(group, "DT Maximum Categoric Set Size", "If present, specifies the minimum leaf size (def=1)", {'m', "minleaf"});
    args::PositionalList<std::string> files(parser, "files", "Files associated to the specific worlds");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
    std::string testing_file = args::get(testing_log);
    std::string expected_File = args::get(expected_tab);
    log_data_format testing_format = log_data_format::XES1;
    if (testing_f) {
        testing_format = magic_enum::enum_cast<log_data_format>(args::get(testing_f)).value();
    }
    std::vector<log_data_format> worlds_format_to_load = args::get(log);
    std::vector<std::string>     worlds_file_to_load = args::get(files);
    ForTheWin::gain_measures measure = ForTheWin::Entropy;
    double tau = 0.75;
    double supp = 0.75;
    double purity = 1.0;
    size_t max_set_size = 1;
    size_t min_leaf_size = 1;
    if(tau_val) {
        tau = std::clamp(args::get(tau_val), 0.5, 1.);
    }
    if(supp_val) {
        supp = std::clamp(args::get(supp_val), 0.0, 1.);
    }
    if(tau_val) {
        tau = std::clamp(args::get(tau_val), 0.5, 1.);
    }
    if(dt_purity) {
        purity = std::clamp(args::get(tau_val), 0., 1.);
    }
    if(dt_max_l) {
        max_set_size = std::max(args::get(dt_max_l), 1UL);
    }
    if(dt_min_l) {
        min_leaf_size = std::max(args::get(dt_min_l), 1UL);
    }
    if (dt_measure) {
        auto tmp = magic_enum::enum_cast<ForTheWin::gain_measures>(args::get(dt_measure));
        if (tmp) {
            measure = tmp.value();
        }
    }


    // Loading the different classes
    ServerQueryManager sqm;
    double loading_and_indexing = 0;
    double loading_and_indexing_test = 0;
    std::vector<std::string> bogus_model_name;
    std::vector<ConfusionMatrix> matrices(worlds_format_to_load.size());
    for (size_t i = 0, N = std::min(worlds_format_to_load.size(), worlds_file_to_load.size()); i<N; i++) {
        std::stringstream ss;
        std::string model_name = std::to_string(i);
        bogus_model_name.emplace_back(model_name);

        ss << "load "
           << log_parse_format_type.at((size_t)worlds_format_to_load.at(i))
           << " "
           << std::quoted(worlds_file_to_load.at(i))
           <<  " with data as "
           << std::quoted(model_name);
        auto tmp = sqm.runQuery(ss.str());
        std::cerr << tmp.first << " && " << tmp.second << std::endl;
        loading_and_indexing += sqm.multiple_logs[model_name].experiment_logger.log_indexing_ms+sqm.multiple_logs[model_name].experiment_logger.log_loading_and_parsing_ms;
    }

    // Loading the testing log
    {
        std::stringstream ss;
        ss << "load "
           << log_parse_format_type.at(testing_format)
           << " "
           << std::quoted(testing_file)
           <<  " with data as "
           << std::quoted("testing");
        auto tmp = sqm.runQuery(ss.str());
        loading_and_indexing_test += sqm.multiple_logs["testing"].experiment_logger.log_indexing_ms+sqm.multiple_logs["testing"].experiment_logger.log_loading_and_parsing_ms;
    }
    // Setting up the confusion matrices from the models
    size_t nTestingTraces = 0;
    {
        std::fstream myfile(expected_File, std::ios_base::in);
        size_t expected_class;
        while (myfile >> expected_class)
        {
            for (size_t i = 0, N = matrices.size(); i<N; i++) {
                if (i==expected_class) {
                    matrices[i].expected_prediction.emplace_back(1);
                } else {
                    matrices[i].expected_prediction.emplace_back(0);
                }
            }
            nTestingTraces++;
        }
        for (auto& M : matrices) {
            M.actual_prediction = std::vector<size_t>(nTestingTraces, 0);
        }
    }

    // Setting up the xtLTLf semantics for the Declare clauses
    {
        std::string query_plan = "queryplan \"nfmcp23\" {\n"
                                 "     template \"Init\"                   := INIT  activation\n"
                                 "     template \"End\"                    := END activation\n"
                                 "     template \"Exists\"                := (EXISTS $ activation)\n"
                                 "     template \"Exists1\"                := (EXISTS 1 activation)\n"
                                 "     template \"Absence\"               := ABSENCE $ activation\n"
                                 "     template \"Absence1\"               := ABSENCE 1 activation\n"
                                 "     template \"Absence2\"               := ABSENCE 2 activation\n"
                                 "     template \"Precedence\" args 2      := ((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)\n"
                                 "     template \"ChainPrecedence\" args 2 := G(((LAST OR t (NEXT EXISTS ~ 1 t #1))) OR t ((NEXT EXISTS 1 t #1 activation) AND t THETA INV (EXISTS 1 t #2 target) ))\n"
                                 "     template \"Choice\" args 2          := (EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)\n"
                                 "     template \"Response\" args 2        := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )\n"
                                 "     template \"ChainResponse\" args 2   := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                                 "     template \"RespExistence\" args 2   := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target)))\n"
                                 "     template \"ExlChoice\" args 2       := ((EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)) AND ((ABSENCE 1 #1) OR (ABSENCE 1 #2))\n"
                                 "     template \"CoExistence\" args 2     := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target))) AND ( ((ABSENCE 1 #2)) OR ((EXISTS 1 #2 activation) AND THETA INV (EXISTS 1 #1 target)))\n"
                                 "     template \"NotCoExistence\" args 2  := ~ ((EXISTS 1 t #1 activation) AND THETA (EXISTS 1 t #2 target)) PRESERVE\n"
                                 "\n"
                                 "     template \"Succession\" args 2      := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )) AND (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 target)) OR (ABSENCE 1 #2))\n"
                                 "     template \"NegSuccession\" args 2   := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Gt  (EXISTS ~ 1 t #2)) ))\n"
                                 "     template \"ChainSuccession\" args 2 := G( (((LAST OR t (NEXT EXISTS ~ 1 t #2))) OR t ((NEXT EXISTS 1 t #2 activation) AND t THETA INV (EXISTS 1 t #1 target))) AND t\n"
                                 "                                             ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
                                 "                                           )\n"
                                 "     template \"AltResponse\" args 2     := G ( (EXISTS ~ 1 t #1) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT ((EXISTS ~ 1 t #1) U t (EXISTS 1 t #2 target)) )))\n"
                                 "     template \"AltPrecedence\" args 2   := (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)) AND\n"
                                 "                                           (G(((EXISTS ~ 1 t #1)) OR t (((EXISTS 1 t #1 activation)) AND t THETA (NEXT (((EXISTS  ~ 1 t #1) U t (EXISTS 1 t #2 target)) OR t (G t (EXISTS  ~ 1 t #1))))  )))\n"
                                 "}";
        sqm.runQuery(query_plan);
    }
    auto it2 = sqm.planname_to_declare_to_ltlf.find("nfmcp23");


    std::cout << "loading+indexing=" << loading_and_indexing << std::endl;

    // "Training" the models
    auto model_and_times = classifier_mining(sqm,
                      bogus_model_name,
                      supp,
                      tau,
                      purity,
                      max_set_size,
                      min_leaf_size);
    auto model = std::get<0>(model_and_times);
    double dataless_mining = std::get<1>(model_and_times);
    double refinery_time = std::get<2>(model_and_times);
    std::cout << "dataless_mining=" << dataless_mining << std::endl;
    std::cout << "refinery_time=" << refinery_time << std::endl;

    exit(1);

    for (const auto& ref : model) {
        std::cout <<"~~~ model ~~~" << std::endl;
        for (const auto& clause : ref) {
            std::cout << clause << std::endl;
        }
    }

    // Running the MaxSat per model
    {
        std::vector<std::vector<double>> results(nTestingTraces, std::vector<double>(model.size(), 0));
        std::vector<size_t> pos;
        auto& tmpEnv = sqm.multiple_logs["testing"];
        for (size_t i = 0, N = model.size(); i<N; i++) {
            tmpEnv.clearModel(); // initializing the model pipeline
            std::unordered_map<std::string, LTLfQuery>* plans = &it2->second;
            tmpEnv.conjunctive_model = model.at(i);
            tmpEnv.experiment_logger.model_parsing_ms = 0;
            tmpEnv.experiment_logger.model_size = model.at(i).size();
            tmpEnv.experiment_logger.model_filename = "Testing";
            bool doPreliminaryFill = true;
            bool ignoreActForAttributes = false;
            bool creamOffSingleValues = true;
            GroundingStrategyConf::pruning_strategy grounding_strategy = GroundingStrategyConf::NO_EXPANSION;
            tmpEnv.set_grounding_parameters(doPreliminaryFill, ignoreActForAttributes, creamOffSingleValues, grounding_strategy);
            tmpEnv.doGrounding();
            std::string atomj{"p"};
            AtomizationStrategy atom_strategy = AtomizeOnlyOnDataPredicates;
            size_t n = 3;
            tmpEnv.set_atomization_parameters(atomj, n , atom_strategy);
            tmpEnv.init_atomize_tables();
            tmpEnv.first_atomize_model();
            size_t nThreads = 1;
            auto& ref2 = tmpEnv.experiment_logger;
            EnsembleMethods em = TraceMaximumSatisfiability;
            OperatorQueryPlan op = FastOperator_v1;
            tmpEnv.set_maxsat_parameters(nThreads, em, op);
            MAXSatPipeline ref(plans, nThreads, BLOCK_STATIC_SCHEDULE, 3);
            ref.final_ensemble = em;
            ref.operators = op;
            marked_event me;
            me.id.parts.type =MARKED_EVENT_ACTIVATION;
            ref.pipeline(&tmpEnv.grounding, tmpEnv.ap, tmpEnv.db);
            for (size_t j = 0; j < ref.max_sat_per_trace.size(); j++) {
                results[j][i] = ref.max_sat_per_trace.at(i);
            }
        }

        for (size_t j = 0; j<nTestingTraces; j++) {
            const auto& v = results.at(j);
            pos.clear();
            auto it = std::max_element(std::begin(v), std::end(v));
            while (it != std::end(v))
            {
                size_t class_id = std::distance(std::begin(v), it);
                matrices[class_id].actual_prediction[j] = 1;
                it = std::find(std::next(it), std::end(v), *it);
            }
        }
    }
    double macro_average_precision = 0;
    double macro_agerage_recall = 0;
    double micro_average_precision = 0;
    double micro_average_recall = 0;
    double micro_prec_rec_tppc = 0;
    double micro_precision_fppc = 0;
    double micro_recall_fnpc = 0;
    for (auto& M : matrices) {
        M.compute();
        micro_prec_rec_tppc += M.true_positive;
        micro_precision_fppc += M.false_positive;
        micro_recall_fnpc += M.false_negative;
        macro_average_precision += M.precision();
        macro_agerage_recall += M.recall();
    }
    macro_average_precision = macro_average_precision/((double)matrices.size());
    macro_agerage_recall = macro_agerage_recall/((double)matrices.size());
    micro_average_precision = micro_prec_rec_tppc / (micro_prec_rec_tppc+micro_precision_fppc);
    macro_agerage_recall = micro_prec_rec_tppc / (micro_prec_rec_tppc+micro_recall_fnpc);

    bool results_file_exists = std::filesystem::exists("results.csv");
    std::ofstream out("results.csv", std::ios::app);
    if (!results_file_exists) {
        out << "macro_average_precision" << "," << "macro_agerage_recall" << "," << "micro_average_precision" << "," << "macro_agerage_recall" << ","
            << "loading_and_indexing" << "," << "loading_and_indexing_test" << "," << "dataless_mining" << "," << "refinery_time" << std::endl;
    } else {
        out << std::endl;
    }
    out << macro_average_precision << "," << macro_agerage_recall << "," << micro_average_precision << "," << macro_agerage_recall << ","
        << loading_and_indexing << "," << loading_and_indexing_test << "," << dataless_mining << "," << refinery_time;

    return 0;

//    std::vector<Environment*> envs{};
//
//    {
//        std::vector<std::string> world_dirs{world_1_file_dir, world_2_file_dir};
//
//        for(const std::string& dir : world_dirs){
////            std::vector<DeclareDataAware> model = {
////                    DeclareDataAware::binary("Response", "A", "B")
////            };
////                env.load_model(model.begin(), model.end());
//            std::filesystem::path file_path(dir);
//
//            Environment* ptr = envs.emplace_back(new Environment());
//            Environment& env = *ptr;
//            env.doStats = false;
//            std::ifstream if_{file_path / "log.txt"};
//            env.load_log(HUMAN_READABLE_YAUCL, true, (file_path / "log.txt").string(), true, if_);
//
//            // N.B. This is a placeholder for e.g. the mining pipeline
//            env.clearModel();
//            env.load_model(file_path / "model.powerdecl");
//
//            env.set_grounding_parameters(true, false, true, GroundingStrategyConf::NO_EXPANSION);
//            env.doGrounding();
//
//            env.set_atomization_parameters(std::filesystem::path(std::filesystem::current_path()  / "scripts" / "atomization_pipeline.yaml"));
//            env.init_atomize_tables();
//            env.first_atomize_model();
//            env.set_maxsat_parameters(std::filesystem::current_path()  / "scripts" / "intersection_pipeline.yaml");
//        }
//    }
//
//    std::cout << "----------BEFORE----------" << std::endl;
//    for(uint16_t i = 0; i < envs.size(); ++i){
//        std::cout  << "World " << i <<  "\n" << envs.at(i)->conjunctive_model << std::endl;
//    }
//
//    refining_extraction refine_map;
//
//    // To refine, we are going to extract (literally) from the original models the dataless clauses found across envs, refine_clause() will try to refine or neglect if unrefineable
//    {
//        for(uint16_t i = 0; i < envs.size(); ++i){
//            Environment* env_1 = envs[i];
//
//            // Iterate over the vector while erasing elements
//            std::vector<DeclareDataAware>::iterator it = env_1->conjunctive_model.begin();
//
//            while (it != env_1->conjunctive_model.end()) {
//                std::vector<Environment*> envs_found{ env_1 };
//
//                for(uint16_t j = i+1; j < envs.size(); ++j){
//                    Environment* env_2 = envs.at(j);
//                    std::vector<DeclareDataAware>::const_iterator found = std::find(env_2->conjunctive_model.begin(), env_2->conjunctive_model.end(),*it);
//                    if(found != env_2->conjunctive_model.end()){
//                        /* Erase this clause from the found environments */
//                        env_2->conjunctive_model.erase(found);
//                        envs_found.emplace_back(env_2);
//                    }
//                }
//
//                // Extract the similar clauses for further refining and optimize
//                if(envs_found.size() > 1){
//                    std::unordered_map<std::vector<Environment*>, std::vector<DeclareDataAware>>::iterator ref_it = refine_map.find(envs_found);
//                    if(ref_it != refine_map.end()){
//                        ref_it->second.push_back(*it);
//                    }
//                    else{
//                        refine_map.insert({envs_found, {*it}});
//                    }
//
//                    // DOnt erase, do at end
//                    it = env_1->conjunctive_model.erase(it);
//                }
//                else{
//                    ++it;
//                }
//            }
//        }
//    }
//
//    std::cout << "----------CLAUSES TO REFINE----------" << std::endl;
//    std::cout << refine_map << std::endl;
//
//    refine_clause(refine_map, tau);
//
//    std::cout << "----------AFTER----------" << std::endl;
//    for(uint16_t i = 0; i < envs.size(); ++i){
//        std::cout  << "World " << i <<  "\n" << envs.at(i)->conjunctive_model << std::endl;
//    }
}

//int main(){
//        io::CSVReader<4> in("declare_example.csv");
//    in.read_header(io::ignore_extra_column, "template","activation","target","model");
//
//    std::pair<DeclareDataAware, int> obj;
//    std::vector<std::pair<DeclareDataAware,int>> V;
//    while(in.read_row(obj.first.casusu, obj.first.left_act, obj.first.right_act, obj.second)){
//        V.emplace_back(obj);
//    }
//
//    std::function<simple_data(const DeclareDataAware&, const std::string&)> selector = [](const DeclareDataAware& x, const std::string& key) -> simple_data {
//        if (key == "template")
//            return x.casusu;
//        else if (key == "activation")
//            return x.left_act;
//        else if (key == "target")
//            return x.right_act;
//        else
//            return 0.0;
//    };
//
//    auto it = V.begin(), en = V.end();
//    DecisionTree<DeclareDataAware> tree(it,
//                            en,
//                            1,
//                            selector,
//                            std::unordered_set<std::string>{"template","activation","target"},
//                            std::unordered_set<std::string>{},
//                            ForTheWin::gain_measures::Entropy,
//                            0.9,
//                            1,
//                            1);
//    tree.print_rec(std::cout, 1);
//
//    DeclareDataAware test = DeclareDataAware::binary("Response", "A", "B");
//    result res = refine_clause(test, tree);
//
//    try {
//        std::pair<DeclareDataAware, DeclareDataAware> pair = std::get<std::pair<DeclareDataAware, DeclareDataAware>>(res);
//    }
//    catch(const std::bad_variant_access& e) {
//        std::cout << e.what() << '\n';
//    }
//}