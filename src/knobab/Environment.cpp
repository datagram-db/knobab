//
// Created by giacomo on 07/01/2022.
//

#include "knobab/Environment.h"
#include "yaucl/bpm/structures/ltlf/ltlf_query.h"

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
    conjunctive_model.clear();
    if (!std::filesystem::exists(std::filesystem::path(model_file))) {
        std::cerr << "ERROR: model file does not exist: " << model_file << std::endl;
        exit(1);
    }
    std::ifstream file{model_file};
    conjunctive_model = dmp.load(file, true);
    experiment_logger.model_parsing_ms = dmp.parsing_time_ms;
    experiment_logger.model_size = conjunctive_model.size();
    experiment_logger.model_filename = model_file;
}

void Environment::load_log(log_data_format format, bool loadData, const std::string &filename, bool setMaximumStrLen,
                           std::istream &input_stream) {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    experiment_logger.log_filename = filename;

    {
        //log_data_format format, bool loadData, std::istream &stream, KnowledgeBase &output,
        //                              std::string &filename
        auto t1 = high_resolution_clock::now();
        load_into_knowledge_base(format, loadData, input_stream, db, filename);
        auto t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;
        experiment_logger.log_loading_and_parsing_ms = ms_double.count();
        //std::cout << "Loading and parsing time = " << ms_double.count() << std::endl;
    }

    {
        auto t1 = high_resolution_clock::now();
        db.index_data_structures(index_missing_data);
        auto t2 = high_resolution_clock::now();

        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;
        experiment_logger.log_indexing_ms = ms_double.count();
        //std::cout << "Indexing time = " << ms_double.count() << std::endl;
    }
    if (setMaximumStrLen) {
        auto tmp = db.getMaximumStringLength();
        ap.s_max = std::string(tmp, std::numeric_limits<char>::max());
        DataPredicate::MAX_STRING = ap.s_max;
        DataPredicate::msl = tmp;
    }


    experiment_logger.n_traces = db.noTraces;
    experiment_logger.n_acts = db.actId;
    // Compute some more trace statistics

    double trace_avg, trace_pow2, N;
    N = db.act_table_by_act_id.trace_length.size();
    size_t frequency_of_trace_length = 0;
    size_t previousLength = 0;
    std::multiset<size_t> O;
    for (const size_t i : db.act_table_by_act_id.trace_length) {
        trace_avg += i;
        trace_pow2 += std::pow(i, 2);
        O.insert(i);

    }
    for (size_t len : O) {
        size_t currFreq = O.count(len);
        if (currFreq > frequency_of_trace_length) {
            frequency_of_trace_length = currFreq;
            previousLength = len;
        }
    }
    trace_avg = trace_avg / N;

    experiment_logger.log_trace_average_length = trace_avg;
    experiment_logger.log_trace_variance = (trace_pow2 / N) - std::pow(trace_avg, 2);
    experiment_logger.most_frequent_trace_length = previousLength;
    experiment_logger.trace_length_frequency = frequency_of_trace_length;
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
    experiment_logger.model_data_decomposition_time = collect_data_from_declare_disjunctive_model(ap, grounding);
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
    experiment_logger.model_atomization_time = atomize_model(ap, grounding);
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
            if (prop.isAtomNegated) {
                return unordered_difference(getSigmaAll(), bogus_act_to_set.at(prop.single_atom_if_any));
            } else {
                return bogus_act_to_set.at(prop.single_atom_if_any);
            }
        case easy_prop::E_P_TRUE:
            return getSigmaAll();
        default: //case easy_prop::E_P_FALSE:
            return {};
    }
}

FlexibleFA<size_t, std::string> Environment::declare_to_graph_for_patterns(const DeclareDataAware &decl) {
    assert(!decl.left_decomposed_atoms.empty());
    assert((isUnaryPredicate(decl.casusu)) || (!decl.right_decomposed_atoms.empty()));
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
            /*std::ofstream ofile{std::to_string(i++)+"_tmp.dot"};
            g3.dot(ofile);*/
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
        /**{
            auto g = convert_to_dfa_graph(currGraph).makeDFAAsInTheory(getSigmaAll());
            {
                std::ofstream output_el_model{std::to_string(0)+"_test1.g"};
                dot(currGraph, output_el_model, false);
            }
            {
                std::ofstream output_el_model{std::to_string(0)+"_test2.g"};
                g.dot(output_el_model);
            }

            // TODO: merge sink un-accepting nodes in makeDFAAsInTheory
            auto DFA = minimizeDFA(g);
        }*/
        for (size_t j = 1; j<M; j++) {
            auto& zeroModelJ = grounding.singleElementOfConjunction.at(j);
            ///assert(allTemplates.contains(std::make_pair(zeroModelJ.casusu, zeroModelJ.n)));
            ///assert(allTemplates[std::make_pair(zeroModelJ.casusu, zeroModelJ.n)].contains(zeroModelJ));
            graph_join_pm currGraph2;
            compute_declare_for_disjunctive(zeroModelJ, currGraph2);
            ///auto& currGraph2 = template_to_graph.at(zeroModelJ);
            conditionalPruningGraph(doPrune, false, result, currGraph2);
            {
                ///auto g = convert_to_dfa_graph(currGraph2).makeDFAAsInTheory(getSigmaAll());

                /**std::ofstream output_el_model{std::to_string(j)+"_testNOW.g"};
                g.dot(output_el_model);*/

                // TODO: merge sink un-accepting nodes in makeDFAAsInTheory
                ///auto DFA = minimizeDFA(g);

                //dot(currGraph2, output_el_model, false);
                //DFA.dot(output_el_model);
            }
            {
                ///auto g = convert_to_dfa_graph(result.joined_graph_model).makeDFAAsInTheory(getSigmaAll());

                // TODO: merge sink un-accepting nodes in makeDFAAsInTheory
                ///auto DFA = minimizeDFA(g);

                ///std::ofstream output_el_model{std::to_string(j)+"_jointmp.g"};
                //dot(result.joined_graph_model, output_el_model, false);
                //DFA.dot(output_el_model);
            }
        }
        /*if (doPrune) {
            graph_join_pm result_;
            remove_unaccepting_states(result.joined_graph_model, result_);
            result.joined_graph_model = result_;
        }*/
        return result;
    }
}

void Environment::print_count_table(std::ostream &os) const {
    db.print_count_table(os);
}

void Environment::print_act_table(std::ostream &os) const {
    db.print_act_table(os);
}

void Environment::print_attribute_tables(std::ostream &os) const {
    db.print_attribute_tables(os);
}

void Environment::load_all_clauses() {
    for (declare_templates t : magic_enum::enum_values<declare_templates>()) {
        ///std::cout << "INIT: " << magic_enum::enum_name(t) << std::endl;
        if (isUnaryPredicate(t)) {
            for (size_t i = 1; i<3; i++) {
                declare_to_graph.getDeclareTemplate(t, i);
            }
        } else {
            declare_to_graph.getDeclareTemplate(t, 2);
        }
    }
}


#include <httplib.h>

void Environment::server(MAXSatPipeline& pipeline) {

    using namespace httplib;
    Server svr;

    // Representing the count table as a csv file, with headers
    svr.Get("/count_table.csv", [this](const httplib::Request& req, httplib::Response& res) {
        std::stringstream ss;
        db.print_count_table(ss);
        res.set_content(ss.str(), "text/csv");
    });
    svr.Get("/count.html", [this](const httplib::Request& req, httplib::Response& res) {
        std::ifstream t("client/count_table.html");
        t.seekg(0, std::ios::end);
        size_t size = t.tellg();
        std::string buffer(size, ' ');
        t.seekg(0);
        t.read(&buffer[0], size);
        res.set_content(buffer, "text/html");
    });

    // Representing the ACT Table
    svr.Get("/act_table.csv", [this](const httplib::Request& req, httplib::Response& res) {
        std::stringstream ss;
        db.print_act_table(ss);
        res.set_content(ss.str(), "text/csv");
    });
    svr.Get("/act.html", [this](const httplib::Request& req, httplib::Response& res) {
        std::ifstream t("client/act_table.html");
        t.seekg(0, std::ios::end);
        size_t size = t.tellg();
        std::string buffer(size, ' ');
        t.seekg(0);
        t.read(&buffer[0], size);
        res.set_content(buffer, "text/html");
    });

    // Returning the attribute tables
    svr.Get("/att_table_names.csv",[this](const httplib::Request& req, httplib::Response& res) {
        std::stringstream ss;
        for (auto it = db.attribute_name_to_table.begin(), en = db.attribute_name_to_table.end(); it != en; ) {
            ss << it->first;
            it++;
            if (it != en) ss << std::endl;
        }
        res.set_content(ss.str(), "text/csv");
    });
    svr.Get("/att.csv",[this](const httplib::Request& req, httplib::Response& res) {
        std::stringstream ss;
        auto it = db.attribute_name_to_table.find(req.get_param_value("f",0));
        if (it != db.attribute_name_to_table.end())
            ss << it->second;
        res.set_content(ss.str(), "text/csv");
    });
    svr.Get("/atts.html",[this](const httplib::Request& req, httplib::Response& res) {
        std::ifstream t("client/att_tables.html");
        t.seekg(0, std::ios::end);
        size_t size = t.tellg();
        std::string buffer(size, ' ');
        t.seekg(0);
        t.read(&buffer[0], size);
        res.set_content(buffer, "text/html");
    });


    svr.Get("/query_plan.json",[this, &pipeline](const httplib::Request& req, httplib::Response& res) {
        std::stringstream ss;
        ss << pipeline.qm.generateGraph();
        res.set_content(ss.str(), "text/json");
    });
    svr.Get("/graph.html",[this](const httplib::Request& req, httplib::Response& res) {
        std::ifstream t("client/graph.html");
        t.seekg(0, std::ios::end);
        size_t size = t.tellg();
        std::string buffer(size, ' ');
        t.seekg(0);
        t.read(&buffer[0], size);
        res.set_content(buffer, "text/html");
    });
    svr.Get("/pipeline_data.csv",[this](const httplib::Request& req, httplib::Response& res) {
        std::stringstream ss;
        auto it = std::stoull(req.get_param_value("f",0));
        if (it != 0) {
            ss << "TraceId,EventId,Sim,{Events}" << std::endl;
            for (const auto& ref : ((ltlf_query*)   it)->result) {
                ss << ref.first.first << "," << ref.first.second << "," << ref.second << ",{TODO}" << std::endl;
            }
        }
        res.set_content(ss.str(), "text/csv");
    });

    svr.listen("localhost", 8080);
}

void Environment::set_grounding_parameters(const std::string &grounding_strategy) {
    GroundingStrategyConf::pruning_strategy ps = GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES;
    size_t msl = 10;
    bool doPreliminaryFill = true;
    bool ignoreActForAttributes = false;
    bool creamOffSingleValues = true;
    if (std::filesystem::exists(std::filesystem::path(grounding_strategy))) {
        std::cout << "Loading the grounding_conf strategy configuration file: " << grounding_strategy << std::endl;
        YAML::Node n = YAML::LoadFile(grounding_strategy);

        if (n["strategy"]) {
            auto x = n["strategy"].Scalar();
            auto v = magic_enum::enum_cast<GroundingStrategyConf::pruning_strategy>(x);
            if (v.has_value()) {
                ps = v.value();
            }
        }

        if (n["doPreliminaryFill"]) {
            auto x = n["doPreliminaryFill"].Scalar();
            doPreliminaryFill = (x == "1") || (x == "T") || (x == "true");
        }

        if (n["ignoreActForAttributes"]) {
            auto x = n["ignoreActForAttributes"].Scalar();
            ignoreActForAttributes = (x == "1") || (x == "T") || (x == "true");
        }

        if (n["creamOffSingleValues"]) {
            auto x = n["creamOffSingleValues"].Scalar();
            creamOffSingleValues = (x == "1") || (x == "T") || (x == "true");
        }

        set_grounding_parameters(doPreliminaryFill,
                                 ignoreActForAttributes,
                                 creamOffSingleValues,
                                 ps);
    }
}

void Environment::set_atomization_parameters(const std::filesystem::path &atomization_conf) {
    std::string fresh_atom_label{"p"};
    size_t msl = 10;
    if (std::filesystem::exists((atomization_conf))) {
        std::cout << "Loading the atomization configuration file: " << atomization_conf << std::endl;
        YAML::Node n = YAML::LoadFile((atomization_conf).string());
        if (n["fresh_atom_label"]) {
            fresh_atom_label = n["fresh_atom_label"].Scalar();
        }
        if (n["MAXIMUM_STRING_LENGTH"]) {
            msl = n["MAXIMUM_STRING_LENGTH"].as<size_t>();
        }
        set_atomization_parameters(fresh_atom_label, msl);
    }
}

