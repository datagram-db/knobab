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

int main(int argc, char **argv) {
    args::ArgumentParser parser("DBolt±", "This extension provides the data-aware declarative mining algorithm presented");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    std::unordered_map<std::string, log_data_format> map{
            {"hrf", log_data_format::HUMAN_READABLE_YAUCL},
            {"xes", log_data_format::XES1},
            {"bar", log_data_format::TAB_SEPARATED_EVENTS}};
    args::Group group(parser, "You can use the following parameters", args::Group::Validators::DontCare, args::Options::Global);
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

    ServerQueryManager sqm;
    double loading_and_indexing = 0;
    std::vector<std::string> bogus_model_name;
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

    std::cout << "loading+indexing=" << loading_and_indexing << std::endl;
    classifier_mining(sqm,
                      bogus_model_name,
                      supp,
                      tau,
                      purity,
                      max_set_size,
                      min_leaf_size);


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