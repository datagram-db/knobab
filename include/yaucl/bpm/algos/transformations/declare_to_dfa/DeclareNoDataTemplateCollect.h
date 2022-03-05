//
// Created by giacomo on 26/07/21.
//

#ifndef BPM21_TEMPLATECOLLECT_H
#define BPM21_TEMPLATECOLLECT_H

#include <unordered_map>
#include <yaucl/bpm/structures/declare/DeclareDataAware.h>
#include <yaucl/graphs/graph_join_pm.h>
//#include <graphs/third-party-wrappers/FLLOATScriptRunner.h>
#include <yaucl/bpm/algos/transformations/declare_to_dfa/TemplateCollectResult.h>
#include <filesystem>
#include <sstream>
#include <magic_enum.hpp>
#include <fstream>

graph_join_pm ReplaceABWithProperLabels(const std::string& dot,
                                        std::unordered_map<std::string, std::string> *ptr,
                                        const std::unordered_set<std::string>& SigmaAll);

/**
 * Given a set of declare templates, it generates it, and initializes the non-data aware representation of the
 * original ones. In fact, please bear in mind that each dataaware template should be already atomized into a
 * non data-aware representation
 */

struct [[deprecated]] DeclareNoDataTemplateCollect {
private:
    std::unordered_map<std::pair<declare_templates, size_t>,
            std::unordered_set<DeclareDataAware>> allTemplates;
    std::unordered_map<std::pair<declare_templates, size_t>, graph_join_pm> graph_map;
    std::unordered_map<DeclareDataAware, graph_join_pm> template_to_graph;
    bool isAdding;
    ///FLLOATScriptRunner pyscript;
    std::filesystem::path base_serialization_path;
    bool doPrune;
public:

    /**
     *
     * @param base_serialization_path   Folder where to serialize the intermediate graphs
     */
    DeclareNoDataTemplateCollect(bool doPrune = true, const std::string& base_serialization_path = ".");

    bool isPruningAllowed() const  { return doPrune; }

    /**
     * Adds a template to the current collector
     * @param left
     */
    void add(const DeclareDataAware& left);

    /**
     * Given a set of sigmas associated to the constraints, generates the intermediate graphs.
     * Temporary files are going to be produced in the current directory where the program is running
     *
     * @param SigmaAll      Complete universe set of all the actions
     * @param T             Callable object, accepting as an Input a ltlf formula using FLLOAT's syntax and
     *                      returning a DOT string representing the DFA associated to it
     */
    template<typename T>
    void run(const std::unordered_set<std::string>& SigmaAll) {
        T callable;
        if (!isAdding) return;
        for (const auto& template_cp : allTemplates) {
            std::pair<declare_templates, size_t> template_ = template_cp.first;
            std::string_view w = magic_enum::enum_name(template_.first);
            std::string name{w.data(), w.size()};
            name = name + "_" + std::to_string(template_.second) ;
            ltlf input_forumla;

            std::vector<DeclareDataAware>                             Sequentialised{template_cp.second.begin(), template_cp.second.end()};
            std::vector<std::unordered_map<std::string, std::string>> VM;
            std::vector<std::string>                                  Names;
            std::unordered_set<std::string> tmpSigmaAll{"a", "b"};

            switch (template_.first) {
                case Existence:
                case Absence:
                //case Absence2:
                //case Exactly:
                case End:
                case Init:
                    input_forumla = DeclareDataAware::unary(template_.first,  "a", template_.second).toFiniteSemantics();
                    for (const auto& template_actual: Sequentialised) {
                        std::unordered_map<std::string, std::string> M;
                        M["a"] = template_actual.left_act;
                        VM.emplace_back(M);
                        Names.emplace_back(name+"["+template_actual.left_act+"]");
                    }
                    break;

                default:
                    input_forumla = DeclareDataAware::binary(template_.first, "a", "b").toFiniteSemantics();
                    for (const auto& template_actual: Sequentialised) {
                        std::unordered_map<std::string, std::string> M;
                        M["a"] = template_actual.left_act;
                        M["b"] = template_actual.right_act;
                        Names.emplace_back(name+"["+template_actual.left_act+", "+template_actual.right_act+"]");
                        VM.emplace_back(M);
                    }
                    break;
            }


            std::cout << name << std::endl;
            std::stringstream s;
            s << input_forumla;
            ///throw std::runtime_error("ERROR: in here, I should load the minimized graph in the original representation");
            std::string dot_graph = callable(s.str());
            ///graph_map[template_] = callable(s.str(), nullptr, tmpSigmaAll);
            ///const auto& gref = graph_map.at(template_);
            ///parseToGraph(graph_map[template_], input_forumla, pyscript, (base_serialization_path/ (name+".ltlf")).string(), nullptr, tmpSigmaAll);

            {
                std::ofstream file{(base_serialization_path/ (name+".dot")).string()};
                file << dot_graph;
                ///dot(graph_map.at(template_), file, false);
            }
            for (size_t i = 0, N = Sequentialised.size(); i<N; i++) {
                auto str = Names.at(i);
                std::cout << "\t * " << str << std::endl;
                auto& ref = template_to_graph[Sequentialised.at(i)];
                //std::stringstream s;
                //s << input_forumla;
                //throw std::runtime_error("ERROR: in here, I should take the graph ");
                ref = ReplaceABWithProperLabels(dot_graph, &VM.at(i), SigmaAll);
                //parseToGraph(ref, input_forumla, pyscript, (base_serialization_path / (str + ".ltlf")).string(), &VM[i], SigmaAll);
                std::ofstream file{(base_serialization_path/str).string()};
                dot(ref, file, false);
            }
        }
        isAdding = false;
    }

    /**
     * Given a subset of the templates provided in the add, generates a model with graphs from it
     *
     * @param Model     Model defined as a conjunction of templates
     * @return          Single graphs associated to the model, as well as the joined graphs
     */
    TemplateCollectResult buildUpModel(const std::vector<DeclareDataAware>& Model);
};


#endif //BPM21_TEMPLATECOLLECT_H
