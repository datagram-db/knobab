#include <yaucl/learning/MCL.h>
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
#include <knobab/mining/bolt2.h>

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

/**
 * Loading the models from the dumped files
 * @author: Samuel 'Buzz' Appleby, Giacomo
 * @param sqm
 */
void loading_model_from_file(ServerQueryManager& sqm, const std::filesystem::path& path, std::tuple<std::unordered_map<std::string, std::vector<pattern_mining_result<DeclareDataAware>>>,double>& result) {
    std::get<1>(result) = 0.0;
    auto& refRoot = std::get<0>(result);
    for(auto& p : std::filesystem::recursive_directory_iterator(path)) {
        if (p.is_directory()) {
            std::string model_name = p.path().filename();
            std::cout << "Loading model: " << model_name << std::endl;
            auto model_file = p.path() / "model.powerdecl";
            auto sup_conf_file = p.path() / "sup_conf.tab";
            if ((!exists(model_file)) || (!exists(sup_conf_file))) {
                std::cout << "Skipping" << std::endl;
            }

            std::vector<pattern_mining_result<DeclareDataAware>>&  new_vec = refRoot[model_name];
            std::stringstream sSTR;
            sSTR << "file "
                 << std::quoted((model_file).string());

            auto actual_model = sqm.loadModelFromFile(sSTR.str());

            std::ifstream file(sup_conf_file);
            std::string line;
            uint32_t idx = 0;
            std::string delimiter = "\t";
            while (getline (file, line)) {
                pattern_mining_result<DeclareDataAware>& ref = new_vec.emplace_back();
                ref.clause = actual_model.at(idx);
                std::istringstream iss(line);
                std::string token;
                std::getline(iss, token, '\t');
                ref.support_declarative_pattern = stod(token);
                std::getline(iss, token, '\t');
                ref.confidence_declarative_pattern = stod(token);
                idx++;
            }
        }
    }






}


enum MetricCases {
    ConfMetric,
    SuppMetric,
    RestrSuppMetric
};

//void recompute_triplets_for_matrix(const MetricCases &useConfidence,
//                                   const std::tuple<std::unordered_map<std::string, std::vector<pattern_mining_result<DeclareDataAware>>>, double> &model_and_times,
//                                   const std::unordered_set<std::string> &clauses_names_to_consider,
//                                   std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<size_t, std::unordered_map<size_t, std::vector<size_t>>>>> &tripletList_accessor,
//                                   std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Eigen::Triplet<double>>>> &tripletList_confsupp_accessor,
//                                   std::unordered_map<std::string, std::unordered_map<std::string, yaucl::learning::MCL::SparseMatrix<double>>>& model_confsupp_accessor,
//                                   size_t max_val,
//                                   const std::unordered_map<std::string, roaring::Roaring64Map>& model_to_removed_clauses
//                                   ) {
//
//}

int main(int argc, char **argv) {
#if 0
    std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
    log_data_format world_format_to_load = XES1;
    std::string worlds_file_to_load = "data/testing/mining/1000_10_10.xes";
    double supp = 0.1;

    // Loading the different classes
    ServerQueryManager sqm;
    double loading_and_indexing = 0;
    std::vector<std::string> bogus_model_name;

    std::stringstream ss;
    std::string model_name = worlds_file_to_load;
    bogus_model_name.emplace_back(model_name);

    ss << "load "
       << log_parse_format_type.at((size_t)world_format_to_load)
       << " "
       << std::quoted(worlds_file_to_load)
       <<  " with data as "
       << std::quoted(model_name);
    auto tmp = sqm.runQuery(ss.str());
    std::cerr << tmp.first << " && " << tmp.second << std::endl;
    loading_and_indexing += sqm.multiple_logs[model_name].experiment_logger.log_indexing_ms+sqm.multiple_logs[model_name].experiment_logger.log_loading_and_parsing_ms;

    // Setting up the xtLTLf semantics for the Declare clauses
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
                             "     template \"ExclChoice\" args 2       := ((EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)) AND ((ABSENCE 1 #1) OR (ABSENCE 1 #2))\n"
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

    auto it2 = sqm.planname_to_declare_to_ltlf.find("nfmcp23");

    std::cout << "loading+indexing=" << loading_and_indexing << std::endl;

    // "Training" the models
    auto model_and_times = classifier_mining(sqm,
                      bogus_model_name,
                      supp,
                      0,
                      0,
                      0,
                      0);
    auto model = std::get<0>(model_and_times);
    double dataless_mining = std::get<1>(model_and_times);
    double refinery_time = std::get<2>(model_and_times);
    std::cout << "clauses=" << model.at(0).size() << std::endl;
    std::cout << "dataless_mining=" << dataless_mining << std::endl;
    std::cout << "refinery_time=" << refinery_time << std::endl;
#endif

    args::ArgumentParser parser("Boltk", "This extension provides the data-aware declarative mining algorithm presented");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    std::unordered_map<std::string, log_data_format> map{
            {"hrf", log_data_format::HUMAN_READABLE_YAUCL},
            {"xes", log_data_format::XES1},
            {"tab", log_data_format::TAB_SEPARATED_EVENTS}};

    args::Group group(parser, "You can use the following parameters", args::Group::Validators::DontCare, args::Options::Global);
    args::Flag read_dumped_models(group, "Read dumped models", "If set, does not perform any mining, but merely reads the models as dumped", {'r', "read1"});
    std::unordered_map<std::string, MetricCases> metric_map{
            {"conf", ConfMetric},
            {"supp", SuppMetric},
            {"restr", RestrSuppMetric}};
    args::MapFlag<std::string, MetricCases> use_confidence_for_clustering(parser, "c", "Sets up the metric case to be adopted in the ", {'c', "usemetric"}, metric_map);
    args::ValueFlag<std::string>  dump_folder(group, "Testing log", "The location where the models are going to be dumped", {'o', "output_models"});
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
    MetricCases useConfidence = RestrSuppMetric;
    if (use_confidence_for_clustering) {
        useConfidence = args::get(use_confidence_for_clustering);
    }
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
    std::filesystem::path output_models = std::filesystem::path("data")/"benchmarking"/"mining"/"cyber";
    if (dump_folder) {
        output_models = args::get(dump_folder);
    }

    ServerQueryManager sqm;
    std::tuple<std::unordered_map<std::string, std::vector<pattern_mining_result<DeclareDataAware>>>,double> model_and_times;

    if (!read_dumped_models) {
        // Loading the different classes
        double loading_and_indexing = 0;
        std::vector<std::string> bogus_model_name;
        std::vector<ConfusionMatrix> matrices(worlds_format_to_load.size());
        for (size_t i = 0, N = std::min(worlds_format_to_load.size(), worlds_file_to_load.size()); i<N; i++) {
            std::stringstream ss;
            std::string model_name = std::filesystem::path(worlds_file_to_load.at(i)).stem().generic_string();
            bogus_model_name.emplace_back(model_name);
            auto t2 = (size_t)worlds_format_to_load.at(i);
            ss << "load "
               << log_parse_format_type.at((size_t)worlds_format_to_load.at(i))
               << " "
               << std::quoted(worlds_file_to_load.at(i))
               <<  " no stats as " // no stats with data as
               << std::quoted(model_name);
            std::cout << ss.str() << std::endl;
            auto tmp = sqm.runQuery(ss.str());
            std::cerr << tmp.first << " && " << tmp.second << std::endl;
            loading_and_indexing += sqm.multiple_logs[model_name].experiment_logger.log_indexing_ms+sqm.multiple_logs[model_name].experiment_logger.log_loading_and_parsing_ms;
        }
        std::cout << "loading+indexing=" << loading_and_indexing << std::endl;

#ifdef FUTURE
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
#endif

        // TODO: many other algorithms, as in the mining pipeline!
        model_and_times = bolt2_multilog(sqm,
                                              bogus_model_name,
                                              supp,
                                              tau,
                                              purity,
                                              max_set_size,
                                              min_leaf_size);
        auto model = std::get<0>(model_and_times);
        double dataless_mining = std::get<1>(model_and_times);
        std::cout << "dataless_mining=" << dataless_mining << std::endl;

        /// Serialising the model from disk
        for (const auto& [model_name,modelx] : model) {
            serialize_bolt2_outcome(modelx, model_name, output_models);
        }
    } else {
        loading_model_from_file(sqm, output_models, model_and_times);
    }

    // model name -> clause name -> [left -> right|n -> clauseN]
//    std::unordered_map<std::string, std::unordered_map<std::string, yaucl::learning::MCL::SparseMatrix<size_t>>> model_accessor;
    // model name -> clause name -> [left -> right|n -> conf|supp]
    std::unordered_map<std::string, std::unordered_map<std::string, yaucl::learning::MCL::SparseMatrix<double>>> model_confsupp_accessor;
    // Hardcorded configurations, that should have been generalised as configuration files
//    std::unordered_set<std::string> allowed_clauses{"ChainResponse", "ChainPrecedence", "Precedence", "Response", "RespExistence"};
    std::unordered_map<std::string, std::vector<std::pair<std::string, bool>>> clauseExpansionWithNotInversion
    {
        {"ChainSuccession", {std::make_pair("ChainResponse", false), std::make_pair("ChainPrecedence", true)}},
        {"Surround", {std::make_pair("ChainResponse", false), std::make_pair("ChainResponse", false)}},
        {"Succession", {std::make_pair("Response", false), std::make_pair("Precedence", false)}},
        {"CoExistence", {std::make_pair("RespExistence", false), std::make_pair("RespExistence", true)}},
    };

    // string <-> uid
    std::unordered_set<std::string> clauses_names_to_consider;
    for (const auto&  [model_name, actual_model] : std::get<0>(model_and_times))
        for (const auto& clause : actual_model)
            clauses_names_to_consider.insert(clause.clause.casusu);
    yaucl::structures::any_to_uint_bimap<std::string> mapper;
    size_t max_val = 0;
    std::unordered_map<std::string, roaring::Roaring64Map> model_to_removed_clauses;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<size_t, std::unordered_map<size_t, std::vector<size_t>>>>> tripletList_accessor;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Eigen::Triplet<double>>>> tripletList_confsupp_accessor;
    // TODO: solve issue when multiple clauses might be associated to the same element
    {
//        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Eigen::Triplet<size_t>>>> tripletList_accessor;
        size_t left_act, right_act;

        for (const auto& [model_name, actual_model] : std::get<0>(model_and_times)) {
            auto it = model_to_removed_clauses.find(model_name);
            for (size_t model_pos = 0, N = actual_model.size(); model_pos < N; model_pos++) {
//                double selectedScore = 0.0;
                const auto& clause = actual_model.at(model_pos);
                if ((it != model_to_removed_clauses.end()) && it->second.contains(model_pos)) continue;
                if (!clauses_names_to_consider.contains(clause.clause.casusu)) continue;
                    left_act = mapper.put(clause.clause.left_act).first;
                if ((!isUnaryPredicate(clause.clause.casusu)) && (!clause.clause.right_act.empty())) {
                    right_act = mapper.put(clause.clause.right_act).first;
                } else {
                    right_act = clause.clause.n;
                    if (right_act > max_val)
                        max_val = right_act;
                }
                    auto it = clauseExpansionWithNotInversion.find(clause.clause.casusu);
                    if (it != clauseExpansionWithNotInversion.end()) {
                        for (const auto& rewritings : it->second) {
                            auto& cp = tripletList_accessor[model_name][rewritings.first];
                            if (rewritings.second) {
//                                tripletList_confsupp_accessor[model_name][rewritings.first].emplace_back(right_act, left_act, selectedScore);
                                cp[right_act][left_act].emplace_back(model_pos);
                            } else {
//                                tripletList_confsupp_accessor[model_name][rewritings.first].emplace_back(left_act, right_act, selectedScore);
//                                cp.emplace_back(left_act, right_act, model_pos+1);
                                cp[left_act][right_act].emplace_back(model_pos);
                            }
                        }
                    } else {
                        auto& cp = tripletList_accessor[model_name][clause.clause.casusu];
                        cp[left_act][right_act].emplace_back(model_pos);
//                        tripletList_confsupp_accessor[model_name][clause.clause.casusu].emplace_back(left_act, right_act, selectedScore);
                    }
            }
        }
        max_val = std::max(max_val, mapper.size()+1);
        tripletList_confsupp_accessor.clear();

        double selectedScore;
        for (const auto& [model_name, clauses_casusu_maps] : tripletList_accessor) {
            const auto &model = std::get<0>(model_and_times).at(model_name);
            const auto elements = model_to_removed_clauses.find(model_name);
            for (const auto &[clause_name, clauses]: clauses_casusu_maps) {
                if (!clauses_names_to_consider.contains(clause_name)) continue;
                auto &ref = tripletList_confsupp_accessor[model_name][clause_name];
                ref.clear();
                for (const auto &[left_act_id, right_map]: clauses) {
                    for (const auto &[right_act_id, ids]: right_map) {
                        double total = 1.0;
                        for (const size_t id: ids) {
                            if ((elements != model_to_removed_clauses.end()) && (elements->second.contains(id)))
                                continue;
                            switch (useConfidence) {
                                case ConfMetric:
                                    selectedScore = model.at(id).confidence_declarative_pattern;
                                    break;
                                case SuppMetric:
                                    selectedScore = model.at(id).support_generating_original_pattern;
                                    break;
                                case RestrSuppMetric:
                                    selectedScore = model.at(id).restrictive_support_declarative_pattern;
                                    break;

                            }
                            total *= (1.0 - selectedScore);
                        }
                        if (total != 1.0)
                            ref.emplace_back(left_act_id, right_act_id, 1.0 - selectedScore);
                    }
                }
            }
        }
        for (auto& [model_name, triple_map] : tripletList_confsupp_accessor) {
            auto& matrixmap_accessor = model_confsupp_accessor[model_name];
            for (auto& [clause_name, triple] : triple_map) {
                if (!clauses_names_to_consider.contains(clause_name)) continue;
                matrixmap_accessor[clause_name] = Eigen::SparseMatrix<double>(max_val+1,max_val+1);
                auto& ref = matrixmap_accessor[clause_name];
                ref.reserve(triple.size());
                ref.setFromTriplets(triple.begin(), triple.end());
                triple.clear();
            }
            triple_map.clear();
        }

    }



    // TODO: avoiding chains, so to reduce any potential input model with absences or exists
    //       We can use MCL to alleviate the problem of finding all the possible cycles in the graph.
    // Now hardcoded: getting all the clauses that are associated to the cycles
    // TODO: parallelize for each clause in the model
    double inflate = 2;
    size_t steps = 100;
    yaucl::learning::MCL::normalizations type = yaucl::learning::MCL::SYM_NORMALIZED_LAPLACIAN;
    double consideration_threshold = 0.000001;
    DeclareDataAware absence;
    absence.casusu = "Absence";
    absence.n = 1;
    std::unordered_map<std::string, DeclareDataAware> to_absence_clauses_if_cluster{{"ChainResponse", absence},
                                                                               {"ChainPrecedence", absence},
                                                                               {"Precedence",absence},
                                                                               {"Response",absence}};

    // Initialisation
    std::unordered_set<std::string>to_absence_clauses_if_cluster_set;
    for (const auto& [k,v] : to_absence_clauses_if_cluster) to_absence_clauses_if_cluster_set.emplace(k);
    for (auto& [model_name, actual_model] : model_confsupp_accessor) {
        roaring::Roaring64Map& offsets_candidate_removals = model_to_removed_clauses[model_name];
        for (const auto& clause_name : to_absence_clauses_if_cluster) {
            auto it = actual_model.find(clause_name.first);
            if (it != actual_model.end()) {
                auto clusters = yaucl::learning::MCL::perform_MCL_clustering(it->second, type, inflate, steps, true, consideration_threshold);
                const auto& model_offsets = tripletList_accessor.at(model_name).at(clause_name.first);
                for (const auto& cluster : clusters) {
                    for (size_t idI : cluster) {
                        for (size_t idJ : cluster) {
                            auto it2 = model_offsets.find(idI);
                            if (it2 != model_offsets.end()) {
                                auto it3 = it2->second.find(idJ);
                                if (it3 != it2->second.end()) {
                                    for (const auto& ref : it3->second) {
                                        tripletList_accessor[model_name][clause_name.second.casusu][idI][clause_name.second.n].emplace_back(ref);
                                        offsets_candidate_removals.add(ref);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // Now, considering the clauses being clastered for removal
    for (const auto& [k,v] : to_absence_clauses_if_cluster) to_absence_clauses_if_cluster_set.emplace(v.casusu);
    double selectedScore;
    for (const auto& [model_name, clauses_casusu_maps] : tripletList_accessor) {
        auto& matrixmap_accessor = model_confsupp_accessor[model_name];
        const auto &model = std::get<0>(model_and_times).at(model_name);
        const auto elements = model_to_removed_clauses.find(model_name);
        for (const auto &[clause_name, clauses]: clauses_casusu_maps) {
            if (!clauses_names_to_consider.contains(clause_name)) continue;
            auto &ref = tripletList_confsupp_accessor[model_name][clause_name];
            ref.clear();
            for (const auto &[left_act_id, right_map]: clauses) {
                for (const auto &[right_act_id, ids]: right_map) {
                    double total = 1.0;
                    bool found = false;
                    for (const size_t id: ids) {
                        if ((elements != model_to_removed_clauses.end()) && (elements->second.contains(id)))
                            continue;
                        else
                            found = true;
                        switch (useConfidence) {
                            case ConfMetric:
                                selectedScore = model.at(id).confidence_declarative_pattern;
                                break;
                            case SuppMetric:
                                selectedScore = model.at(id).support_generating_original_pattern;
                                break;
                            case RestrSuppMetric:
                                selectedScore = model.at(id).restrictive_support_declarative_pattern;
                                break;

                        }
                        total *= (selectedScore);
                    }
                    if (found) {
                        auto& ref = matrixmap_accessor[clause_name];
                        auto& coeff = ref.coeffRef(left_act_id, right_act_id);
                        if (coeff == 0) {
                            coeff = total;
                        } else {
                            coeff = std::max(coeff, total);
                        }
                    }
                }
            }
        }
    }

    // TODO: parallelize for each clause in the model:
    //       For this, do this over the reduced clauses in the matrix already
//    for (const auto& [model_name, actual_model] : std::get<0>(model_and_times)) {
//        size_t clause_count = 0;
//        const auto& matrixmap_accessor = model_accessor[model_name];
//        for (const auto& clause : actual_model) {
//            auto& src_confidence = clause.confidence_declarative_pattern;
//            auto& src_support = clause.support_declarative_pattern;
//            auto& src_restrictive = clause.restrictive_support_declarative_pattern;
//            auto& src_clause_name = clause.clause.casusu;
//            auto src_left = mapper.get(clause.clause.left_act);
//            auto src_right_or_n = clause.clause.right_act.empty() ? clause.clause.n : mapper.get(clause.clause.right_act);
//            DEBUG_ASSERT(matrixmap_accessor.contains(src_clause_name));
//            auto& ref = matrixmap_accessor.at(src_clause_name);
//            DEBUG_ASSERT(ref.coeff(src_left, src_right_or_n)-1 == clause_count);
//            clause_count++;
//
//
//            // TODO: inference model
//        }
//    }



//    for(uint16_t i = 0; i < 5; ++i){
//        auto model_and_times = classifier_mining(sqm,
//                                                 bogus_model_name,
//                                                 supp,
//                                                 0,
//                                                 0,
//                                                 0,
//                                                 0);
//        auto model = std::get<0>(model_and_times);
//        double dataless_mining = std::get<1>(model_and_times);
//        double refinery_time = std::get<2>(model_and_times);
//        std::cout << "clauses=" << model.at(0).size() << std::endl;
//        std::cout << "dataless_mining=" << dataless_mining << std::endl;
//        std::cout << "refinery_time=" << refinery_time << std::endl;
//    }



    return 0;
}

//int main(int argc, char **argv) {
//    args::ArgumentParser parser("DBolt±",
//                                "This extension provides the data-aware declarative mining algorithm presented");
//    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
//    std::unordered_map<std::string, log_data_format> map{
//            {"hrf", log_data_format::HUMAN_READABLE_YAUCL},
//            {"xes", log_data_format::XES1},
//            {"bar", log_data_format::TAB_SEPARATED_EVENTS}};
//    args::Group group(parser, "You can use the following parameters", args::Group::Validators::DontCare,
//                      args::Options::Global);
//    args::ValueFlag<std::string> testing_log(group, "Testing log", "The log against which conduct the prediction",
//                                             {'e', "testing"});
//    args::ValueFlag<std::string> expected_tab(group, "Expected Classes",
//                                              "The expected prediction classes for the testing log", {'x', "expected"});
//    args::ValueFlag<std::string> testing_f(group, "Testing log format", "The format of the testing log",
//                                           {'f', "format"});
//    args::MapFlagList<std::string, log_data_format> log(parser, "w",
//                                                        "allocates the n-th world associated to a given file to be read in a given format",
//                                                        {'w', "world"}, map);
//    args::ValueFlag<double> tau_val(group, "Tau Value", "If present, specifies the tau value", {'t', "tau"});
//    args::ValueFlag<double> supp_val(group, "Support Value", "If present, specifies the support value", {'s', "supp"});
//    args::ValueFlag<std::string> dt_measure(group, "DT Measure",
//                                            "If present, specifies the quality measure for the decision tree",
//                                            {'q', "quality"});
//    args::ValueFlag<double> dt_purity(group, "DT Minimum Purity",
//                                      "If present, specifies the minimum purity condition (def=1.)", {'p', "minpure"});
//    args::ValueFlag<size_t> dt_max_l(group, "DT Maximum Categoric Set Size",
//                                     "If present, specifies the maximum size of a categorical set (def=1)",
//                                     {'l', "maxlen"});
//    args::ValueFlag<size_t> dt_min_l(group, "DT Maximum Categoric Set Size",
//                                     "If present, specifies the minimum leaf size (def=1)", {'m', "minleaf"});
//    args::PositionalList<std::string> files(parser, "files", "Files associated to the specific worlds");
//
//    try {
//        parser.ParseCLI(argc, argv);
//    } catch (args::Help) {
//        std::cout << parser;
//        return 0;
//    } catch (args::ParseError e) {
//        std::cerr << e.what() << std::endl;
//        std::cerr << parser;
//        return 1;
//    } catch (args::ValidationError e) {
//        std::cerr << e.what() << std::endl;
//        std::cerr << parser;
//        return 1;
//    }
//
//    std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
//    std::string testing_file = args::get(testing_log);
//    std::string expected_File = args::get(expected_tab);
//    log_data_format testing_format = log_data_format::XES1;
//    if (testing_f) {
//        testing_format = magic_enum::enum_cast<log_data_format>(args::get(testing_f)).value();
//    }
//    std::vector<log_data_format> worlds_format_to_load = args::get(log);
//    std::vector<std::string> worlds_file_to_load = args::get(files);
//    ForTheWin::gain_measures measure = ForTheWin::Entropy;
//    double tau = 0.75;
//    double supp = 0.75;
//    double purity = 1.0;
//    size_t max_set_size = 1;
//    size_t min_leaf_size = 1;
//    if (tau_val) {
//        tau = std::clamp(args::get(tau_val), 0.5, 1.);
//    }
//    if (supp_val) {
//        supp = std::clamp(args::get(supp_val), 0.0, 1.);
//    }
//    if (tau_val) {
//        tau = std::clamp(args::get(tau_val), 0.5, 1.);
//    }
//    if (dt_purity) {
//        purity = std::clamp(args::get(tau_val), 0., 1.);
//    }
//    if (dt_max_l) {
//        max_set_size = std::max(args::get(dt_max_l), 1UL);
//    }
//    if (dt_min_l) {
//        min_leaf_size = std::max(args::get(dt_min_l), 1UL);
//    }
//    if (dt_measure) {
//        auto tmp = magic_enum::enum_cast<ForTheWin::gain_measures>(args::get(dt_measure));
//        if (tmp) {
//            measure = tmp.value();
//        }
//    }
//
//    // Loading the different classes
//    ServerQueryManager sqm;
//    double loading_and_indexing = 0;
//    double loading_and_indexing_test = 0;
//    std::vector<std::string> bogus_model_name;
//    std::vector<ConfusionMatrix> matrices(worlds_format_to_load.size());
//    for (size_t i = 0, N = std::min(worlds_format_to_load.size(), worlds_file_to_load.size()); i < N; i++) {
//        std::stringstream ss;
//        std::string model_name = std::to_string(i);
//        bogus_model_name.emplace_back(model_name);
//
//        ss << "load "
//           << log_parse_format_type.at((size_t) worlds_format_to_load.at(i))
//           << " "
//           << std::quoted(worlds_file_to_load.at(i))
//           << " with data as "
//           << std::quoted(model_name);
//        auto tmp = sqm.runQuery(ss.str());
//        std::cerr << tmp.first << " && " << tmp.second << std::endl;
//        loading_and_indexing += sqm.multiple_logs[model_name].experiment_logger.log_indexing_ms +
//                                sqm.multiple_logs[model_name].experiment_logger.log_loading_and_parsing_ms;
//    }
//
//    // Loading the testing log
//    {
//        std::stringstream ss;
//        ss << "load "
//           << log_parse_format_type.at(testing_format)
//           << " "
//           << std::quoted(testing_file)
//           << " with data as "
//           << std::quoted("testing");
//        auto tmp = sqm.runQuery(ss.str());
//        loading_and_indexing_test += sqm.multiple_logs["testing"].experiment_logger.log_indexing_ms +
//                                     sqm.multiple_logs["testing"].experiment_logger.log_loading_and_parsing_ms;
//    }
//    // Setting up the confusion matrices from the models
//    size_t nTestingTraces = 0;
//    {
//        std::fstream myfile(expected_File, std::ios_base::in);
//        size_t expected_class;
//        while (myfile >> expected_class) {
//            for (size_t i = 0, N = matrices.size(); i < N; i++) {
//                if (i == expected_class) {
//                    matrices[i].expected_prediction.emplace_back(1);
//                } else {
//                    matrices[i].expected_prediction.emplace_back(0);
//                }
//            }
//            nTestingTraces++;
//        }
//        for (auto &M: matrices) {
//            M.actual_prediction = std::vector<size_t>(nTestingTraces, 0);
//        }
//    }
//
//    // Setting up the xtLTLf semantics for the Declare clauses
//    std::string query_plan = "queryplan \"nfmcp23\" {\n"
//                             "     template \"Init\"                   := INIT  activation\n"
//                             "     template \"End\"                    := END activation\n"
//                             "     template \"Exists\"                := (EXISTS $ activation)\n"
//                             "     template \"Exists1\"                := (EXISTS 1 activation)\n"
//                             "     template \"Absence\"               := ABSENCE $ activation\n"
//                             "     template \"Absence1\"               := ABSENCE 1 activation\n"
//                             "     template \"Absence2\"               := ABSENCE 2 activation\n"
//                             "     template \"Precedence\" args 2      := ((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)\n"
//                             "     template \"ChainPrecedence\" args 2 := G(((LAST OR t (NEXT EXISTS ~ 1 t #1))) OR t ((NEXT EXISTS 1 t #1 activation) AND t THETA INV (EXISTS 1 t #2 target) ))\n"
//                             "     template \"Choice\" args 2          := (EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)\n"
//                             "     template \"Response\" args 2        := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )\n"
//                             "     template \"ChainResponse\" args 2   := G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
//                             "     template \"RespExistence\" args 2   := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target)))\n"
//                             "     template \"ExlChoice\" args 2       := ((EXISTS 1 t #1 activation) OR THETA (EXISTS 1 t #2 activation)) AND ((ABSENCE 1 #1) OR (ABSENCE 1 #2))\n"
//                             "     template \"CoExistence\" args 2     := ( ((ABSENCE 1 #1)) OR ((EXISTS 1 #1 activation) AND THETA (EXISTS 1 #2 target))) AND ( ((ABSENCE 1 #2)) OR ((EXISTS 1 #2 activation) AND THETA INV (EXISTS 1 #1 target)))\n"
//                             "     template \"NotCoExistence\" args 2  := ~ ((EXISTS 1 t #1 activation) AND THETA (EXISTS 1 t #2 target)) PRESERVE\n"
//                             "\n"
//                             "     template \"Succession\" args 2      := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Ft THETA (EXISTS 1 t #2 target)) )) AND (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 target)) OR (ABSENCE 1 #2))\n"
//                             "     template \"NegSuccession\" args 2   := (G ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) &Gt  (EXISTS ~ 1 t #2)) ))\n"
//                             "     template \"ChainSuccession\" args 2 := G( (((LAST OR t (NEXT EXISTS ~ 1 t #2))) OR t ((NEXT EXISTS 1 t #2 activation) AND t THETA INV (EXISTS 1 t #1 target))) AND t\n"
//                             "                                             ( ((EXISTS ~ 1 t #1)) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT EXISTS 1 t #2 target)))\n"
//                             "                                           )\n"
//                             "     template \"AltResponse\" args 2     := G ( (EXISTS ~ 1 t #1) OR t ((EXISTS 1 t #1 activation) AND t THETA (NEXT ((EXISTS ~ 1 t #1) U t (EXISTS 1 t #2 target)) )))\n"
//                             "     template \"AltPrecedence\" args 2   := (((EXISTS  ~ 1 t #2) U (EXISTS 1 t #1 activation)) OR (ABSENCE 1 #2)) AND\n"
//                             "                                           (G(((EXISTS ~ 1 t #1)) OR t (((EXISTS 1 t #1 activation)) AND t THETA (NEXT (((EXISTS  ~ 1 t #1) U t (EXISTS 1 t #2 target)) OR t (G t (EXISTS  ~ 1 t #1))))  )))\n"
//                             "}";
//    sqm.runQuery(query_plan);
//    auto it2 = sqm.planname_to_declare_to_ltlf.find("nfmcp23");
//
//    std::cout << "loading+indexing=" << loading_and_indexing << std::endl;
//
//    // "Training" the models
//    auto model_and_times = classifier_mining(sqm,
//                                             bogus_model_name,
//                                             supp,
//                                             tau,
//                                             purity,
//                                             max_set_size,
//                                             min_leaf_size);
//    auto model = std::get<0>(model_and_times);
//    double dataless_mining = std::get<1>(model_and_times);
//    double refinery_time = std::get<2>(model_and_times);
//    std::cout << "dataless_mining=" << dataless_mining << std::endl;
//    std::cout << "refinery_time=" << refinery_time << std::endl;
//
////    exit(1);
//
//    // Discarding the resulting clauses not having optimal support
//    {
//        for (size_t i = 0, N = model.size(); i < N; i++) {
//            auto &tmpEnv = sqm.multiple_logs[std::to_string(i)];
//            tmpEnv.clearModel(); // initializing the model pipeline
//            std::unordered_map<std::string, LTLfQuery> *plans = &it2->second;
//            tmpEnv.conjunctive_model = model.at(i);
//            tmpEnv.experiment_logger.model_parsing_ms = 0;
//            tmpEnv.experiment_logger.model_size = model.at(i).size();
//            tmpEnv.experiment_logger.model_filename = "Testing";
//            bool doPreliminaryFill = true;
//            bool ignoreActForAttributes = false;
//            bool creamOffSingleValues = true;
//            GroundingStrategyConf::pruning_strategy grounding_strategy = GroundingStrategyConf::NO_EXPANSION;
//            tmpEnv.set_grounding_parameters(doPreliminaryFill, ignoreActForAttributes, creamOffSingleValues,
//                                            grounding_strategy);
//            tmpEnv.doGrounding();
//            std::string atomj{"p"};
//            AtomizationStrategy atom_strategy = AtomizeOnlyOnDataPredicates;
//            size_t n = 3;
//            tmpEnv.set_atomization_parameters(atomj, n, atom_strategy);
//            tmpEnv.init_atomize_tables();
//            tmpEnv.first_atomize_model();
//            size_t nThreads = 1;
//            auto &ref2 = tmpEnv.experiment_logger;
//            EnsembleMethods em = PerDeclareSupport;
//            OperatorQueryPlan op = FastOperator_v1;
//            tmpEnv.set_maxsat_parameters(nThreads, em, op);
//            MAXSatPipeline ref(plans, nThreads, BLOCK_STATIC_SCHEDULE, 3);
//            ref.final_ensemble = em;
//            ref.operators = op;
//            marked_event me;
//            me.id.parts.type = MARKED_EVENT_ACTIVATION;
//            ref.pipeline(&tmpEnv.grounding, tmpEnv.ap, tmpEnv.db);
//            std::vector<size_t> indices_to_remove;
//            for (size_t j = 0; j < ref.support_per_declare.size(); j++) {
//                if (ref.support_per_declare.at(j) < supp) {
//                    indices_to_remove.emplace_back(j);
//                }
//            }
//            if (!indices_to_remove.empty())
//                remove_index(model[i], indices_to_remove);
//        }
//    }
//
//    for (const auto &ref: model) {
//        std::cout << "~~~ model ~~~" << std::endl;
//        for (const auto &clause: ref) {
//            std::cout << clause << std::endl;
//        }
//    }
//
//    // Testing of the classifier part.
//    {
//        std::vector<std::vector<double>> results(nTestingTraces, std::vector<double>(model.size(), 0));
//        std::vector<size_t> pos;
//        for (size_t i = 0, N = model.size(); i < N; i++) {
//            auto &tmpEnv = sqm.multiple_logs["testing"];
//            tmpEnv.clearModel(); // initializing the model pipeline
//            std::unordered_map<std::string, LTLfQuery> *plans = &it2->second;
//            tmpEnv.conjunctive_model = model.at(i);
//            tmpEnv.experiment_logger.model_parsing_ms = 0;
//            tmpEnv.experiment_logger.model_size = model.at(i).size();
//            tmpEnv.experiment_logger.model_filename = "Testing";
//            bool doPreliminaryFill = true;
//            bool ignoreActForAttributes = false;
//            bool creamOffSingleValues = true;
//            GroundingStrategyConf::pruning_strategy grounding_strategy = GroundingStrategyConf::NO_EXPANSION;
//            tmpEnv.set_grounding_parameters(doPreliminaryFill, ignoreActForAttributes, creamOffSingleValues,
//                                            grounding_strategy);
//            tmpEnv.doGrounding();
//            std::string atomj{"p"};
//            AtomizationStrategy atom_strategy = AtomizeOnlyOnDataPredicates;
//            size_t n = 3;
//            tmpEnv.set_atomization_parameters(atomj, n, atom_strategy);
//            tmpEnv.init_atomize_tables();
//            tmpEnv.first_atomize_model();
//            size_t nThreads = 1;
//            auto &ref2 = tmpEnv.experiment_logger;
//            EnsembleMethods em = TraceMaximumSatisfiability;
//            OperatorQueryPlan op = FastOperator_v1;
//            tmpEnv.set_maxsat_parameters(nThreads, em, op);
//            MAXSatPipeline ref(plans, nThreads, BLOCK_STATIC_SCHEDULE, 3);
//            ref.final_ensemble = em;
//            ref.operators = op;
//            marked_event me;
//            me.id.parts.type = MARKED_EVENT_ACTIVATION;
//            ref.pipeline(&tmpEnv.grounding, tmpEnv.ap, tmpEnv.db);
//            for (size_t j = 0; j < ref.max_sat_per_trace.size(); j++) {
//                results[j][i] = ref.max_sat_per_trace.at(j);
//            }
//        }
//
//        for (size_t j = 0; j < nTestingTraces; j++) {
//            const auto &v = results.at(j);
//            pos.clear();
//            auto it = std::max_element(std::begin(v), std::end(v));
//            while (it != std::end(v)) {
//                size_t class_id = std::distance(std::begin(v), it);
//                matrices[class_id].actual_prediction[j] = 1;
//                it = std::find(std::next(it), std::end(v), *it);
//            }
//        }
//    }
//    double macro_average_precision = 0;
//    double macro_agerage_recall = 0;
//    double micro_average_precision = 0;
//    double micro_average_recall = 0;
//    double micro_prec_rec_tppc = 0;
//    double micro_precision_fppc = 0;
//    double micro_recall_fnpc = 0;
//    for (auto &M: matrices) {
//        M.compute();
//        micro_prec_rec_tppc += M.true_positive;
//        micro_precision_fppc += M.false_positive;
//        micro_recall_fnpc += M.false_negative;
//        macro_average_precision += M.precision();
//        macro_agerage_recall += M.recall();
//    }
//    macro_average_precision = macro_average_precision / ((double) matrices.size());
//    macro_agerage_recall = macro_agerage_recall / ((double) matrices.size());
//    micro_average_precision = micro_prec_rec_tppc / (micro_prec_rec_tppc + micro_precision_fppc);
//    macro_agerage_recall = micro_prec_rec_tppc / (micro_prec_rec_tppc + micro_recall_fnpc);
//
//    bool results_file_exists = std::filesystem::exists("results.csv");
//    std::ofstream out("results.csv", std::ios::app);
//    if (!results_file_exists) {
//        out << "macro_average_precision" << "," << "macro_agerage_recall" << "," << "micro_average_precision" << ","
//            << "macro_agerage_recall" << ","
//            << "loading_and_indexing" << "," << "loading_and_indexing_test" << "," << "dataless_mining" << ","
//            << "refinery_time" << std::endl;
//    } else {
//        out << std::endl;
//    }
//    out << macro_average_precision << "," << macro_agerage_recall << "," << micro_average_precision << ","
//        << macro_agerage_recall << ","
//        << loading_and_indexing << "," << loading_and_indexing_test << "," << dataless_mining << "," << refinery_time;
//
//    return 0;
//}