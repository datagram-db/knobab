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
#include <yaucl/graphs/graph_join_pm_conversion.h>


void whole_testing(const std::string& log_file = "data/testing/log.txt",
                   const std::string& declare_file = "data/testing/declare2.powerdecl",
                   const std::string& atomization_conf = "data/testing/atomization_pipeline.yaml",
                   const std::string& grounding_strategy = "data/testing/grounding_strategy.yaml") {
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



    //env.print_knowledge_base(std::cout); // DEBUG
    //////////////////////////////////////////////////////////////////

    std::cout << "Loading the declarative model from file: " << declare_file << std::endl;
    env.load_model(declare_file);
    //env.print_model(std::cout); // DEBUG
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
    //env.print_grounded_model(std::cout); // DEBUG
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
    //env.print_grounding_tables(std::cout);
    //////////////////////////////////////////////////////////////////

    std::cout << "Atomizing the declare formulae" << std::endl;
    env.first_atomize_model();
    //env.print_grounded_model(std::cout); // DEBUG
    //////////////////////////////////////////////////////////////////


    env.query_model();

    env.server();


#if 0
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
#endif

}

void test_data_query(const std::string& log_file = "data/testing/log.txt",
                   const std::string& declare_file = "data/testing/declare2.powerdecl",
                   const std::string& atomization_conf = "data/testing/atomization_pipeline.yaml",
                   const std::string& grounding_strategy = "data/testing/grounding_strategy.yaml") {
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

#include "knobab/utilities/SetOperators.h"
#include "knobab/utilities/DeclareClauses.h"
#include "yaucl/structures/set_operations.h"

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"a", {}}, {"a", {}}, {"b", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"a", {}}, {"b", {}}, {"a", {}}, {"c", {}}, {"a", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"c", {}}, {"b", {}}, {"c", {}}, {"c", {}}, {"c", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"c", {}}, {"c", {}}, {"a", {}}, {"c", {}}, {"c", {}}}};
//
//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"c", {}}, {"c", {}}, {"a", {}}, {"c", {}}, {"c", {}}, {"a", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"c", {}}, {"b", {}}, {"c", {}}, {"b", {}}, {"c", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"a", {}}, {"a", {}}, {"b", {}}},
//{{"a", {}}, {"c", {{"x", 2.0}}}}};

const KnowledgeBase::no_antlr_log LogTrace = {
        {{"B", {{"x", 2.0},{"y", 3.0}}},
         {"A", {{"x", 1.0},{"y", 6.0}}},
         {"B", {{"x", 2.0},{"y", 3.0}}},
         {"A", {{"x", 2.0},{"y", 6.0}}},
         {"B", {{"x", 2.0},{"y", 5.0}}},
         {"B", {{"x", 2.0},{"y", 5.0}}},},

        {{"A", {{"x", 2.0},{"y", 1.0}}},
         {"A", {{"x", 1.0},{"y", 3.0}}},
         {"B", {}},
         {"A", {{"x", 1.0},{"y", 3.0}}},
         {"A", {{"x", 4.0},{"y", 3.0}}}},

        {{"C", {}},
         {"B", {}},
         {"C", {}},
         {"B", {}},
         {"C", {}}}
};

const std::string toSearch = "A";
const uint8_t searchAmount = 2;
//<<<<<<< sam
const std::vector<SimpleDataPredicate> predicates{{"x", "y", numeric_atom_cases::LT}};
const double minThreshHold = 0;
//=======
//const std::vector<SimpleDataPredicate> predicates{{"x", "x", numeric_atom_cases::EQ}};
//const double minThreshHold = 1;


KnowledgeBase test_kb(const KnowledgeBase::no_antlr_log& L, const std::string &source, const std::string &name) {
    /// Creating an instance of the knowledge base, that is going to store all the traces in the log!
    KnowledgeBase db;

    db.load_data_without_antlr4(L, source, name);

    /// Loading a log file into the database
    /// load_into_knowledge_base(HUMAN_READABLE_YAUCL, true, "testing/log.txt", db);

    /// Indexing the data structures
    /// TODO: these indices might be written in secondary memory as well!
    db.index_data_structures(true);
    

    /// Debugging purposes: checking whether the thing that I obtain by printing corresponds to the one that I obtained before.
    /// This is just an isomorphism proof, that states that I can always reconstruct the original information from the
    /// given representation
    db.reconstruct_trace_with_data(std::cout);

    return db;
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

void generate_traces(const std::string& log_file = "data/testing/nologolog.txt",
                     const std::string& declare_file = "data/testing/declare4.powerdecl",
                     const std::string& atomization_conf = "data/testing/atomization_pipeline.yaml",
                     const std::string& grounding_strategy = "data/testing/grounding_strategy.yaml") {
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


void sam_testing() {
    KnowledgeBase db = test_kb(LogTrace, "", "");
    PredicateManager predManager({predicates}, &db);

    std::pair<const uint32_t, const uint32_t>  data = db.resolveCountingData(toSearch);
    //std::unordered_map<uint32_t, double> found = db.exists(data, searchAmount);
    //std::cout << "========EXISTS " + toSearch + " " + std::to_string(searchAmount) + "TIMES========="  << std::endl << "RESULT: " << std::endl << found << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>> initAVec = db.init<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>(toSearch, minThreshHold);
    std::cout << "========INIT " + toSearch + "=========" << std::endl << "RESULT: " << std::endl << initAVec.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>> endsAVec = db.ends<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>(toSearch, minThreshHold);
    std::cout << "========ENDS " + toSearch + "=========" << std::endl << "RESULT: " << std::endl << endsAVec.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> unionNoPred;

    setUnion( initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
             std::back_inserter(unionNoPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>);

    std::cout << "========UNION NO PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << unionNoPred.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> unionPred;
    setUnion( initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
             std::back_inserter(unionPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "========UNION WITH PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << unionPred.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> intersectionNoPred;
    setIntersection( initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
                    std::back_inserter(intersectionNoPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>);
    std::cout << "========INTERSECTION NO PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << intersectionNoPred.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> intersectionPred;
    setIntersection( initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
                    std::back_inserter(intersectionPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>, &predManager);
    std::cout << "========INTERSECTION WITH PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << intersectionPred.getTraceApproximations() << std::endl;

    //aaab from trace 0
    dataContainer aOccurences{}, bOccurences{}, aBOccurences{};
    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 0}, {1, {0}}));
    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 2}, {1, {2}}));
    //    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 3}, {1, {3}}));
//    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 4}, {1, {4}}));
//    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 5}, {1, {5}}));
//    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 6}, {1, {6}}));

//    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 0}, {1, {0}}));
//    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
//    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 2}, {1, {2}}));
    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 3}, {1, {3}}));
    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 4}, {1, {4}}));
    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 5}, {1, {5}}));
//    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 6}, {1, {6}}));

    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 0}, {1, {0}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 2}, {1, {2}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 3}, {1, {3}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 4}, {1, {4}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 5}, {1, {5}}));

    std::cout << "========DATA=========" << std::endl << "SET1: " << std::endl << aOccurences << std::endl << "SET2:" << std::endl << bOccurences << std::endl;

    std::cout << "========LTLf Operators=========" << std::endl;
    dataContainer x1 = next(0, 0, 1, aOccurences);
    dataContainer x2 = next(aOccurences);
    std::cout << "========NEXT========" << std::endl << "TEMPORAL: " << x1 << std::endl << "NON-TEMPORAL" << x2 << std::endl;

    dataContainer x3 = global(0, 0, 2, aOccurences);
    dataContainer x4 = global(aOccurences, db.act_table_by_act_id.getTraceLengths());
    std::cout << "========GLOBAL========" << std::endl << "TEMPORAL: " << x3 << std::endl << "NON-TEMPORAL" << x4 << std::endl;

    dataContainer x5 = future(0, 1, 2, aOccurences);
    dataContainer x6 = future(aOccurences);
    std::cout << "========FUTURE========" << std::endl << "TEMPORAL: " << x5 << std::endl << "NON-TEMPORAL" << x6 << std::endl;

    dataContainer x7 = until(0, 0, 2, aOccurences, bOccurences, aBOccurences);
    dataContainer x8 = until(aOccurences, bOccurences, aBOccurences, db.act_table_by_act_id.getTraceLengths());
    std::cout << "========UNTIL========" << std::endl << "TEMPORAL: " << x7 << std::endl << "NON-TEMPORAL" << x8 << std::endl;

    dataContainer e2, e3;
    setUnion(aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e2), Aggregators::maxSimilarity<double, double, double>, &predManager);
    setUnionUntimed(aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e3), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "========LTLF UNION========" << std::endl << "TEMPORAL: " << e2 << std::endl <<  "NON-TEMPORAL" << e3 << std::endl;;

    dataContainer e4, e5;
    setIntersection( aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e4), Aggregators::maxSimilarity<double, double, double>, &predManager);
    setIntersectionUntimed(aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e5), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "========LTLF INTERSECTION========" << std::endl << "TEMPORAL: " << e4 << std::endl <<  "NON-TEMPORAL" << e5 << std::endl;;

    dataContainer choice = DChoice(aOccurences, bOccurences, nullptr);
    std::cout << "========Choice=========" <<  std::endl << choice << std::endl;


    dataContainer exclusiveChoice = ExclusiveChoice(aOccurences, bOccurences, bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========ExclusiveChoice=========" << std::endl << exclusiveChoice << std::endl;

    dataContainer responseExistence = ResponseExistence(aOccurences, bOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========ResponseExistence=========" << std::endl << responseExistence << std::endl;

    dataContainer coexistence = Coexistence(aOccurences, bOccurences, bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(), nullptr);
    //remove_duplicates(coexistence);
    std::cout << "========Coexistence=========" << std::endl << coexistence << std::endl;
    std::flush(std::cout);

    dataContainer notCoexistence = NotCoexistence(bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========NotCoexistence=========" << std::endl << notCoexistence << std::endl;
    std::flush(std::cout);

    dataContainer chainResponse = DChainResponse(aOccurences, bOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========ChainResponse=========" << std::endl << chainResponse << std::endl;
    std::flush(std::cout);
  
  
    dataContainer chainPrecedence = DChainPrecedence(aOccurences, bOccurences, aOccurences, db.getNotFirstElements(), db.getLastElements(), db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========ChainPrecedence=========" << std::endl << chainPrecedence << std::endl;
    std::flush(std::cout);

    dataContainer chainSuccession = DChainSuccession(aOccurences, bOccurences,  bOccurences, aOccurences, db.getNotFirstElements(), db.getLastElements(), db.act_table_by_act_id.getTraceLengths(), nullptr);
    std::cout << "========ChainSuccession=========" << std::endl << chainSuccession << std::endl;
    std::flush(std::cout);

    dataContainer negationChainSuccession = NegationChainSuccession(aOccurences, bOccurences,  bOccurences, aOccurences, db.getLastElements(), db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========NegationChainSuccession=========" << std::endl << negationChainSuccession << std::endl;
    std::flush(std::cout);
    }


int main() {

    //generate_nonunary_templates();
    //test_data_query();
    //test_fsm();
    //whole_testing();
    //test_declare();
    //test_grounding();
    //generate_traces();
    //ltlf_operators_testing();
    //sam_testing();

    return 0;
}
