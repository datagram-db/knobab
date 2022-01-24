#include <iostream>
#include "knobab/Environment.h"

#include <yaml-cpp/yaml.h>
#include <knobab/Environment.h>
#include <yaucl/graphs/algorithms/minimizeDFA.h>

void whole_testing(const std::string& log_file = "testing/log.txt",
                   const std::string& declare_file = "testing/declare2.powerdecl",
                   const std::string& atomization_conf = "testing/atomization_pipeline.yaml",
                   const std::string& grounding_strategy = "testing/grounding_strategy.yaml") {
    Environment env;
    env.clear();
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



void test_group_by() {
    std::vector<std::pair<size_t, double>> W{{1,2}, {3,0.5}, {3, 0.7}, {2, 0.5}, {1, 7}};
    std::sort(W.begin(), W.end());

    auto M = cartesian_product(GroupByKeyExtractorIgnoreKey<std::vector<std::pair<size_t, double>>::iterator, size_t, std::pair<size_t, double>>(W.begin(), W.end(), [](const std::pair<size_t, double>& x) {return x.first; }));
    std::cout << M << std::endl;
}


int main() {

    test_group_by();
    //generate_nonunary_templates();
    //test_data_query();
    //test_fsm();
    //whole_testing();
    //test_declare();
    //test_grounding();
    return 0;
}