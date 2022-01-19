#include <iostream>
#include "knobab/Environment.h"


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
                human_readable_ltlf_printing(os, syntax.args[0]) << ") ∨ (";
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
        if (isUnaryPredicate(t)) continue; // discarding unary predicates.
        if (t == Response)
            std::cout << "DEBUG" << std::endl;

        std::cout << magic_enum::enum_name(t) << ":" << std::endl << "\t - ";
        auto f = DeclareDataAware::binary(t, "a", "b").toFiniteSemantics(false);
        human_readable_ltlf_printing(std::cout, f) << std::endl;
        auto nnf = f.nnf(false);
        if (nnf != f) {
            std::cout << "\t - ";
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

int main() {
    generate_nonunary_templates();
    //test_data_query();
    //test_fsm();
    //whole_testing();
    //test_declare();
    //test_grounding();
    return 0;
}