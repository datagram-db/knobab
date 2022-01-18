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
    test_data_query();
    //test_fsm();
    //whole_testing();
    //test_declare();
    //test_grounding();
    return 0;
}