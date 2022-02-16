#include <iostream>

#include "yaucl/bpm/structures/log/data_loader.h"
#include "yaucl/strings/serializers.h"
#include "knobab/utilities/Aggregators.h"
#include "knobab/dataStructures/TraceData.h"
#include "knobab/utilities/LTLFOperators.h"

#include "knobab/Environment.h"


#include <yaml-cpp/yaml.h>
#include <knobab/Environment.h>
#include <yaucl/graphs/algorithms/minimizeDFA.h>

void whole_testing(const std::string& log_file = "testing/log.txt",
                   const std::string& declare_file = "testing/declare4.powerdecl",
                   const std::string& atomization_conf = "testing/atomization_pipeline.yaml",
                   const std::string& grounding_strategy = "testing/grounding_strategy.yaml") {
    Environment env;
    env.clear();

    env.load_all_clauses();

    std::string fresh_atom_label{"p"};
    size_t msl = 10;
    bool doPreliminaryFill = true;
    bool ignoreActForAttributes = false;
    bool creamOffSingleValues = true;
    GroundingStrategyConf::pruning_strategy ps = GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES;

    if (!std::filesystem::exists(std::filesystem::path(log_file))) {
        std::cerr << "ERROR: the log file is missing: cannot run the pipeline! " << log_file << std::endl;
        exit(1);
    }

    std::cout << "Loading the log file: " << log_file << std::endl;
    env.load_log(HUMAN_READABLE_YAUCL, true, log_file);
    env.print_knowledge_base(std::cout); // DEBUG
    //////////////////////////////////////////////////////////////////

    std::cout << "Loading the declarative model from file: " << declare_file << std::endl;
    env.load_model(declare_file);
    env.print_model(std::cout); // DEBUG
    //////////////////////////////////////////////////////////////////

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

        env.set_grounding_parameters(doPreliminaryFill,
                                     ignoreActForAttributes,
                                     creamOffSingleValues,
                                     ps);
    }
    env.doGrounding();
    env.print_grounded_model(std::cout); // DEBUG
    //////////////////////////////////////////////////////////////////

    if (std::filesystem::exists(std::filesystem::path(atomization_conf))) {
        std::cout << "Loading the atomization configuration file: " << atomization_conf << std::endl;
        YAML::Node n = YAML::LoadFile(atomization_conf);
        if (n["fresh_atom_label"]) {
            fresh_atom_label = n["fresh_atom_label"].Scalar();
        }
        if (n["MAXIMUM_STRING_LENGTH"]) {
            msl = n["MAXIMUM_STRING_LENGTH"].as<size_t>();
        }
        env.set_atomization_parameters(fresh_atom_label, msl);
    }
    //////////////////////////////////////////////////////////////////

    std::cout << "Loading the atomization tables given the model" << std::endl;
    env.init_atomize_tables();
    env.print_grounding_tables(std::cout);
    //////////////////////////////////////////////////////////////////

    std::cout << "Atomizing the declare formulae" << std::endl;
    env.first_atomize_model();
    env.print_grounded_model(std::cout); // DEBUG
    //////////////////////////////////////////////////////////////////

    {
        auto result = env.compute_declare_for_conjunctive(false);
        {
            std::ofstream output_nl_model{"node_labelled_graph.dot"};
            dot(result.joined_graph_model, output_nl_model, true);
        }
        {
            auto g = convert_to_dfa_graph(result.joined_graph_model);

            // TODO: merge sink un-accepting nodes in makeDFAAsInTheory
            auto DFA = minimizeDFA(g).makeDFAAsInTheory(env.getSigmaAll());

            std::ofstream output_el_model{"edge_labelled_graph.dot"};
            DFA.dot(output_el_model);
        }
    }

}

void test_data_query(const std::string& log_file = "testing/log.txt",
                   const std::string& declare_file = "testing/declare2.powerdecl",
                   const std::string& atomization_conf = "testing/atomization_pipeline.yaml",
                   const std::string& grounding_strategy = "testing/grounding_strategy.yaml") {
    Environment env;
    env.clear();
    std::string fresh_atom_label{"p"};
    GroundingStrategyConf::pruning_strategy ps = GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES;

    env.index_missing_data = true; // Force to index the approximate data

    if (!std::filesystem::exists(std::filesystem::path(log_file))) {
        std::cerr << "ERROR: the log file is missing: cannot run the pipeline! " << log_file << std::endl;
        exit(1);
    }

    std::cout << "Loading the log file: " << log_file << std::endl;
    env.load_log(HUMAN_READABLE_YAUCL, true, log_file, true);

    // DEBUG
    env.print_count_table(std::cout);
    env.print_act_table(std::cout);
    env.print_attribute_tables(std::cout);
    env.print_knowledge_base(std::cout);

    /// EXACT QUERIES
//    std::cout << env.range_query({}) << std::endl;
//    std::cout << env.range_query({"x", GEQ, 0.0, "A"}) << std::endl;
//    std::cout << env.range_query({"x", GEQ, 0.0}) << std::endl;
//    std::cout << env.range_query({"x", LEQ, 2.0}) << std::endl;

    /// SETTING FOR THE APPROX QUERY
    env.min_threshold = 0.3;
    env.c = 10.0;
    std::cout << env.range_query({"x", LEQ, 2.0}) << std::endl;

}

std::ostream & human_readable_ltlf_printing(std::ostream &os, const ltlf& syntax) {
        std::string reset = "";
        if (syntax.is_negated)
            os << "¬";
        switch (syntax.casusu) {
            case ACT:
                return os << syntax.act << reset;
            case NUMERIC_ATOM:
                return os << syntax.numeric_atom<< reset;
            case NEG_OF:
                os << "(¬(";
                return human_readable_ltlf_printing(os, syntax.args[0]) << "))" << reset;
            case OR:
                os << "(";
                human_readable_ltlf_printing(os, syntax.args[0]) << (syntax.is_exclusive ? ") ⊻ (" : ") ∨ (");
                return human_readable_ltlf_printing(os, syntax.args[1]) << ')' << reset;
            case AND:
                os << "(";
                human_readable_ltlf_printing(os, syntax.args[0]) << ") ∧ (";
                return human_readable_ltlf_printing(os, syntax.args[1]) << ')' << reset;
            case NEXT:
                os << "○(";
                return human_readable_ltlf_printing(os, syntax.args[0]) << ")" << reset;
            case UNTIL:
                os << "(";
                human_readable_ltlf_printing(os, syntax.args[0]) << ") U (";
                return human_readable_ltlf_printing(os, syntax.args[1]) << ')' << reset;
            case RELEASE:
                os << "(";
                human_readable_ltlf_printing(os, syntax.args[0]) << ") R (";
                return human_readable_ltlf_printing(os, syntax.args[1]) << ')' << reset;
            case TRUE:
                return os << "true"<< reset;
            case BOX:
                os << "▢(";
                return human_readable_ltlf_printing(os, syntax.args[0]) << ")" << reset;
            case DIAMOND:
                os << "◇(";
                return human_readable_ltlf_printing(os, syntax.args[0]) << ")" << reset;
            case LAST:
                return os << "LAST" << reset;
            default:
                return os << "false"<< reset;
        }
}

void generate_nonunary_templates() {
    for (declare_templates t : magic_enum::enum_values<declare_templates>()) {
        if (isUnaryPredicate(t)) continue; // discarding unary predicates

        std::cout << magic_enum::enum_name(t) << ":" << std::endl << "\t - ";
        auto f = DeclareDataAware::binary(t, "a", "b").toFiniteSemantics(false);


       // human_readable_ltlf_printing(std::cout, f) << std::endl;
        auto nnf = f.nnf(false);
        /*if (nnf != f)*/ {
            //std::cout << "\t - ";
            human_readable_ltlf_printing(std::cout, nnf) << std::endl;
        }
        std::cout << std::endl;
    }
}

void test_fsm() {
    SimplifiedFuzzyStringMatching matcher;

    matcher.put("hello");
    matcher.put("bello");
    matcher.put("bel");
    matcher.put("hell");
    matcher.put("call");
    matcher.put("fall");
    matcher.put("tall");
    matcher.put("all");
    matcher.put("elementary");

    std::multimap<double, std::string> result;
    matcher.fuzzyMatch(0.0, 100, "fall", result);
    for (const auto& cp : result)
        std::cout << cp.first << " - " << cp.second << std::endl;
}



    // First part
    std::pair<const uint32_t, const uint32_t>  data = db.resolveCountingData("a");

    // Second part
    std::unordered_map<uint32_t, double> found = db.exists(data, 2);

    const double minThreshHold = 0;

    TraceData<std::pair<uint32_t, uint16_t>, double> testVec = db.init<std::pair<uint32_t, uint16_t>, double>("a", minThreshHold);
    TraceData<std::pair<uint32_t, uint16_t>, double> testVec1 = db.ends<std::pair<uint32_t, uint16_t>, double>("a", minThreshHold);

    TraceData<std::pair<uint32_t, uint16_t>, double> testVec2;
    testVec.setUnion(testVec1.getTraceApproximations().begin(), testVec1.getTraceApproximations().end(), std::back_inserter(testVec2.getTraceApproximations()),  Aggregators::maxSimilarity<double, double, double>);

    std::cout << "========UNION=========" << std::endl << "BETWEEN: " << std::endl << testVec.getTraceApproximations() << std::endl << "AND" << std::endl << testVec1.getTraceApproximations() << std::endl << "RESULT: " << std::endl << testVec2.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, double> testVec3;
    testVec.setIntersection(testVec1.getTraceApproximations().begin(), testVec1.getTraceApproximations().end(), std::back_inserter(testVec3.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>);

    std::cout << "========INTERSECTION=========" << std::endl << "BETWEEN: " << std::endl << testVec.getTraceApproximations() << std::endl << "AND" << std::endl << testVec1.getTraceApproximations() << std::endl << "RESULT: " << std::endl << testVec3.getTraceApproximations()<< std::endl;

    struct example{
        std::vector<std::pair<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>> elems;
    };

    example e;
    // aaab
    e.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
    e.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 2}, {1, {2}}));
    e.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 3}, {1, {3}}));


    auto x1 = next(0, 1, 4, e);
    auto x2 = next(e);
    auto x3 = global(0, 1, 3, e);
    auto x4 = global(e, db.act_table_by_act_id.getTraceLengths());
    auto x5 = future(0, 1, 2, e);
    auto x6 = future(e);

    std::cout << "========LTLf Operators=========" << std::endl;
    std::cout << "--NEXT--" << std::endl << "TEMPORAL: " << x1 << std::endl << "NON-TEMPORAL" << x2 << std::endl;
    std::cout << "--GLOBAL--" << std::endl << "TEMPORAL: " << x3 << std::endl << "NON-TEMPORAL" << x4 << std::endl;
    std::cout << "--FUTURE--" << std::endl << "TEMPORAL: " << x5 << std::endl << "NON-TEMPORAL" << x6 << std::endl;
    std::flush(std::cout);
}
=======


void test_group_by() {
    std::vector<std::pair<size_t, double>> W{{1,2}, {3,0.5}, {3, 0.7}, {2, 0.5}, {1, 7}};
    std::sort(W.begin(), W.end());

    auto M = cartesian_product(GroupByKeyExtractorIgnoreKey<std::vector<std::pair<size_t, double>>::iterator, size_t, std::pair<size_t, double>>(W.begin(), W.end(), [](const std::pair<size_t, double>& x) {return x.first; }));
    std::cout << M << std::endl;
}
#include <random>

template <typename T>
size_t
generateBenchmarkForTests(const std::string &log_file,
                          size_t modelNo,
                          const T &templatu,
                          std::ofstream &file) {
    Environment env;
    env.clear();
    env.load_all_clauses();
    env.load_log(HUMAN_READABLE_YAUCL, true, log_file);
    env.load_model(templatu.begin(), templatu.end());
    env.doGrounding();
    env.init_atomize_tables();
    std::cout << "Atomizing the declare formulae" << std::endl;
    env.first_atomize_model();

    {

        if (modelNo == 4)
            std::cout << "debug" << std::endl;
        auto result = env.compute_declare_for_conjunctive(false);
        {
            auto g = convert_to_dfa_graph(result.joined_graph_model).makeDFAAsInTheory(env.getSigmaAll());
            /*{
                std::ofstream GF{std::to_string(modelNo)+"G.dot"};
                g.dot(GF, false);
            }*/
            auto DFA = minimizeDFA(g);
            /*{
                std::ofstream GF{std::to_string(modelNo)+"DFA.dot"};
                DFA.dot(GF, false);
            }*/

            for (const auto& trace : DFA.generative(10)) {
                for (size_t j = 0, N = trace.size()-1; j<=N; j++) {
                    file << trace[j];
                    if (j != N) file << ",";
                }
                file << std::endl;
            }

            modelNo++;
        }
    }
    return modelNo;
}

void generate_traces(const std::string& log_file = "testing/nologolog.txt",
                     const std::string& declare_file = "testing/declare4.powerdecl",
                     const std::string& atomization_conf = "testing/atomization_pipeline.yaml",
                     const std::string& grounding_strategy = "testing/grounding_strategy.yaml") {
    Environment env;
    env.clear();
    env.load_all_clauses();

    std::string fresh_atom_label{"p"};
    size_t msl = 10;
    bool doPreliminaryFill = true;
    bool ignoreActForAttributes = false;
    bool creamOffSingleValues = true;
    GroundingStrategyConf::pruning_strategy ps = GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES;

    if (!std::filesystem::exists(std::filesystem::path(log_file))) {
        std::cerr << "ERROR: the log file is missing: cannot run the pipeline! " << log_file << std::endl;
        exit(1);
    }

    env.load_log(HUMAN_READABLE_YAUCL, true, log_file);
    env.load_model(declare_file);

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

        env.set_grounding_parameters(doPreliminaryFill,
                                     ignoreActForAttributes,
                                     creamOffSingleValues,
                                     ps);
    }
    env.doGrounding();
    semantic_atom_set Sigma = env.getSigmaAll();
    std::vector<std::string> atomSet;
    for (const auto str : env.getSigmaAll()) {
        atomSet.emplace_back(str);
    }
    std::uniform_int_distribution<> distribEnv(0, atomSet.size()-1);

    std::vector<declare_templates> W;
    for (declare_templates t : magic_enum::enum_values<declare_templates>()) {
        W.emplace_back(t);
    }
    std::uniform_int_distribution<> distribTemplates(1, W.size());
    size_t modelNo = 1;

    if (false) {
        size_t modelThis = 14;
        std::ofstream file{"tests/test.txt"};
        generateBenchmarkForTests(log_file, 4,std::vector<DeclareDataAware>{DeclareDataAware::binary(NotCoExistence, "C", "B"),
                                                                                            DeclareDataAware::unary(Absence2, "C", 1),
                                                                                            DeclareDataAware::unary(Existence, "B", 1),
                                                                                            DeclareDataAware::binary(Succession, "C", "B"),
                                                                                            DeclareDataAware::binary(AltPrecedence, "C", "B")
                                                                                            }, file);

        exit(1);

    }

    std::mt19937_64 gen{1};
    for (size_t len : std::vector<size_t>{1, 3, 5, 7, 10}) {
        for (size_t i = 0; i<10; i++) {
            std::unordered_set<DeclareDataAware> templatu;

            while (templatu.size() < len) {
                declare_templates t = W[distribTemplates(gen)];
                if ((len > 3) && (isPredicateNegative(t))) continue;
                if (isUnaryPredicate(t)) {
                    std::string left = atomSet[distribEnv(gen)];
                    templatu.emplace(DeclareDataAware::unary(t, left, 1));
                } else {
                    std::string left = atomSet[distribEnv(gen)];
                    std::string right = atomSet[distribEnv(gen)];
                    templatu.emplace(DeclareDataAware::binary(t, left, right));
                }
            }

            std::ofstream file{"tests/" + std::to_string(modelNo)+"_"+std::to_string(len)+"_"+std::to_string(i)+".txt"};
            for (const auto& ref : templatu)
                file << '#' << ref << std::endl;
            file << std::endl << std::flush;

            modelNo = generateBenchmarkForTests(log_file, modelNo, templatu, file);
        }
    }
}


int main() {
    //test_group_by();
    //generate_nonunary_templates();
    test_data_query();
    //test_fsm();
    //whole_testing();
    //test_declare();
    //test_grounding();
    //generate_traces();

    return 0;
}