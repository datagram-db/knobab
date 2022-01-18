//
// Created by giacomo on 07/01/2022.
//

#include "knobab/Environment.h"

semantic_atom_set Environment::getSigmaAll() const {
    semantic_atom_set S = ap.act_atoms;
    for (const auto& ref : ap.interval_map) {
        std::pair<std::string, size_t> cp;
        cp.first = ref.first;
        for (size_t i = 0, N = ap.max_ctam_iteration.at(cp.first); i<N; i++) {
            cp.second = i;
            S.insert(ap.clause_to_atomization_map.at(cp));
        }
    }
    for (auto & ref : db.act_table_by_act_id.secondary_index) {
        auto ptr = ref.first;
        while (ptr) {
            auto lx = db.event_label_mapper.get(ptr->entry.id.parts.act);
            if (!ap.interval_map.contains(lx))
                S.insert(lx);
            ptr = ptr->next;
        };
    }
    return S;
}

void Environment::clear() {
    db.clear();
    ap.clear();
    conjunctive_model.clear();
}

#include <filesystem>
#include <yaucl/graphs/algorithms/minimizeDFA.h>
#include <yaucl/graphs/graph_join_pm_conversion.h>


void Environment::load_model(const std::string &model_file) {
    if (!std::filesystem::exists(std::filesystem::path(model_file))) {
        std::cerr << "ERROR: model file does not exist: " << model_file << std::endl;
        exit(1);
    }
    std::ifstream file{model_file};
    conjunctive_model = dmp.load(file, true);
}

void Environment::load_log(log_data_format format, bool loadData, const std::string &filename) {
    load_into_knowledge_base(format, loadData, filename, db);
    db.index_data_structures();
}

void Environment::set_atomization_parameters(const std::string &fresh_atom_label, size_t mslength) {
    ap.fresh_atom_label = fresh_atom_label;
    ap.s_max = std::string(mslength, std::numeric_limits<char>::max());
    DataPredicate::MAX_STRING = ap.s_max;
    DataPredicate::msl = mslength;
}

void
Environment::set_grounding_parameters(bool doPreliminaryFill, bool ignoreActForAttributes, bool creamOffSingleValues,
                                      GroundingStrategyConf::pruning_strategy ps) {
    grounding_conf.doPreliminaryFill = doPreliminaryFill;
    grounding_conf.ignoreActForAttributes = ignoreActForAttributes;
    grounding_conf.creamOffSingleValues = creamOffSingleValues;
    grounding_conf.strategy1 = ps;
}

void Environment::init_atomize_tables() {
    collect_data_from_declare_disjunctive_model(ap, grounding);
}

void Environment::doGrounding() {
    grounding = GroundWhereStrategy(grounding_conf,
                                    db,
                                    conjunctive_model);
}

void Environment::print_model(std::ostream &os) const {
    os << "Declarative Model: " << std::endl;
    os << "----------------------------------------" << std::endl;
    for (const auto& def : conjunctive_model)
        os << def << std::endl;

    os << "----------------------------------------" << std::endl;
    os << std::endl;
}

void Environment::print_grounded_model(std::ostream &os) const {
    os << "Grounded Model: " << std::endl;
    os << "----------------------------------------" << std::endl;
    os << grounding << std::endl;
    os << "----------------------------------------" << std::endl;
    os << std::endl;
}

void Environment::print_knowledge_base(std::ostream &os) const {
    os << "Knowledge Base: " << std::endl;
    os << "----------------------------------------" << std::endl;
    db.reconstruct_trace_with_data(os);
    os << "----------------------------------------" << std::endl;
    os << std::endl;
}

void Environment::print_grounding_tables(std::ostream &os) {
    os << "Grounding Tables: " << std::endl;
    os << "----------------------------------------" << std::endl;
    os << ap << std::endl;
    os << "----------------------------------------" << std::endl;
    os << std::endl;
}

void Environment::first_atomize_model() {
    atomize_model(ap, grounding);
}

semantic_atom_set Environment::evaluate_easy_prop_to_atoms(const easy_prop &prop,
                                                           //const std::unordered_map<std::string, std::string> &bogus_act_to_atom,
                                                           const std::unordered_map<std::string, semantic_atom_set> &bogus_act_to_set) {
    switch (prop.casusu) {
        case easy_prop::E_P_AND:
            assert(prop.args.size() == 2);
            assert(!prop.isAtomNegated);
            return unordered_intersection(evaluate_easy_prop_to_atoms( prop.args.at(0)/*, bogus_act_to_atom*/, bogus_act_to_set),
                                          evaluate_easy_prop_to_atoms( prop.args.at(1)/*, bogus_act_to_atom*/, bogus_act_to_set) );
        case easy_prop::E_P_OR: {
            assert(prop.args.size() == 2);
            assert(!prop.isAtomNegated);
            semantic_atom_set S = evaluate_easy_prop_to_atoms( prop.args.at(0)/*, bogus_act_to_atom*/, bogus_act_to_set);
            auto tmp = evaluate_easy_prop_to_atoms( prop.args.at(1)/*, bogus_act_to_atom*/, bogus_act_to_set);
            S.insert(tmp.begin(), tmp.end());
            return S;
        }
        case easy_prop::E_P_ATOM:
            assert(prop.args.empty());
            assert(bogus_act_to_set.contains(prop.single_atom_if_any));
            ///assert(bogus_act_to_atom.contains(prop.single_atom_if_any));
            if (prop.isAtomNegated) {
                ///semantic_atom_set S = ap.atom_decomposition(bogus_act_to_atom.at(prop.single_atom_if_any));
                return unordered_difference(getSigmaAll(), bogus_act_to_set.at(prop.single_atom_if_any));
            } else {
                return bogus_act_to_set.at(prop.single_atom_if_any);
            }
        case easy_prop::E_P_TRUE:
            return getSigmaAll();
        case easy_prop::E_P_FALSE:
            return {};
    }
}

FlexibleFA<size_t, std::string> Environment::declare_to_graph_for_patterns(const DeclareDataAware &decl) {
    assert(!decl.left_decomposed_atoms.empty());
    assert(!decl.right_decomposed_atoms.empty());
    /*{
        auto it = pattern_graph.find(decl);
        if (it != pattern_graph.end()) return it->second;
    }*/

    NodeLabelBijectionFA<std::string, easy_prop> patternGraphToInstantiate =
            declare_to_graph.getDeclareTemplate(decl.casusu, decl.n);

    //std::unordered_map<std::string, std::string> bogus_act_to_atom;
    //bogus_act_to_atom[declare_to_graph.left_act] = decl.left_act;

    std::unordered_map<std::string, semantic_atom_set> bogus_act_to_set;
    bogus_act_to_set[declare_to_graph.left_act] = decl.left_decomposed_atoms;

    if (!isUnaryPredicate(decl.casusu)) {
        //bogus_act_to_atom[declare_to_graph.right_act] = decl.right_act;
        bogus_act_to_set[declare_to_graph.right_act] = decl.right_decomposed_atoms;
    }

    FlexibleFA<size_t, std::string> result;
    std::unordered_map<size_t, size_t> idConv;
    for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId<N; nodeId++) {
        size_t src = result.addNewNodeWithLabel(nodeId);
        idConv[nodeId] = src;
    }
    for (size_t finalNodes : patternGraphToInstantiate.fini()) {
        result.addToFinalNodesFromId(idConv.at(finalNodes));
    }
    for (size_t initialNodes : patternGraphToInstantiate.init()) {
        result.addToInitialNodesFromId(idConv.at(initialNodes));
    }
    for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId<N; nodeId++) {
        size_t src = idConv.at(nodeId);
        for (const auto&edge : patternGraphToInstantiate.outgoingEdges(nodeId)) {
            // std::cerr << edge.first << std::endl;
            size_t dst = idConv.at(edge.second);
            for (const std::string& act : evaluate_easy_prop_to_atoms(edge.first/*, bogus_act_to_atom*/, bogus_act_to_set)) {
                result.addNewEdgeFromId(src, dst, act);
            }
        }
    }
    return (/*pattern_graph[decl] =*/ result.makeDFAAsInTheory(getSigmaAll()));
}

void Environment::compute_declare_to_graph_for_joins(const DeclareDataAware &decl, graph_join_pm &out_result) {
    //graph_loader.need_back_conversion = ptr != nullptr;
    //graph_loader.back_conv = ptr;
    static size_t i = 0;
    {
        graph_join_pm g2;
        {
            auto g3 = declare_to_graph_for_patterns(decl);
            std::ofstream ofile{std::to_string(i++)+"_tmp.dot"};
            g3.dot(ofile);
            auto g = g3.shiftLabelsToNodes();
            g.pruneUnreachableNodes();

            convert_to_novel_graph(g, g2);
        }
        minimize(g2, out_result);
    }
}

void Environment::compute_declare_for_disjunctive(const DisjunctiveDeclareDataAware &decl, graph_join_pm &out_result) {
    graph_join_pm tmp;
    make_graph_deterministic det;
    for (const auto& declare : decl.elementsInDisjunction) {
        compute_declare_to_graph_for_joins(declare, tmp);
    }
    det.generate_out_graph(tmp, out_result);
}

TemplateCollectResult Environment::compute_declare_for_conjunctive(bool doPrune) {
    std::vector<graph_join_pm> distinct_graph_model;
    graph_join_pm              joined_graph_model;
    size_t M = grounding.singleElementOfConjunction.size();
    if (M == 0) {
        return {};
    } else {
        auto& zeroModel = grounding.singleElementOfConjunction.at(0);
        ///assert(allTemplates.contains(std::make_pair(zeroModel.casusu, zeroModel.n)));
        ///assert(allTemplates[std::make_pair(zeroModel.casusu, zeroModel.n)].contains(zeroModel));
        graph_join_pm currGraph;
        compute_declare_for_disjunctive(zeroModel, currGraph); // // = template_to_graph.at(zeroModel);
        TemplateCollectResult result;
        conditionalPruningGraph(doPrune, true, result, currGraph);
        {
            auto g = convert_to_dfa_graph(currGraph);

            // TODO: merge sink un-accepting nodes in makeDFAAsInTheory
            auto DFA = minimizeDFA(g).makeDFAAsInTheory(getSigmaAll());

            std::ofstream output_el_model{std::to_string(0)+"_test.g"};
            DFA.dot(output_el_model);
        }
        for (size_t j = 1; j<M; j++) {
            auto& zeroModelJ = grounding.singleElementOfConjunction.at(j);
            ///assert(allTemplates.contains(std::make_pair(zeroModelJ.casusu, zeroModelJ.n)));
            ///assert(allTemplates[std::make_pair(zeroModelJ.casusu, zeroModelJ.n)].contains(zeroModelJ));
            graph_join_pm currGraph2;
            compute_declare_for_disjunctive(zeroModelJ, currGraph2);
            ///auto& currGraph2 = template_to_graph.at(zeroModelJ);
            conditionalPruningGraph(doPrune, false, result, currGraph2);
            {
                auto g = convert_to_dfa_graph(currGraph2);

                // TODO: merge sink un-accepting nodes in makeDFAAsInTheory
                auto DFA = minimizeDFA(g).makeDFAAsInTheory(getSigmaAll());

                std::ofstream output_el_model{std::to_string(j)+"_test.g"};
                DFA.dot(output_el_model);
            }
        }
        if (doPrune) {
            graph_join_pm result_;
            remove_unaccepting_states(result.joined_graph_model, result_);
            result.joined_graph_model = result_;
        }
        return result;
    }
}
