#include <iostream>
#include <iomanip>


#include <args.hxx>

#include "yaucl/bpm/structures/log/data_loader.h"
#include "yaucl/strings/serializers.h"
#include "knobab/utilities/Aggregators.h"
#include "knobab/dataStructures/TraceData.h"
#include "knobab/operators/LTLFOperators.h"

#include "knobab/Environment.h"


#include <yaml-cpp/yaml.h>
#include <knobab/Environment.h>
#include <yaucl/graphs/algorithms/minimizeDFA.h>
#include <yaucl/graphs/graph_join_pm_conversion.h>


void whole_testing(const std::string& log_file = "data/testing/log.txt",
                   log_data_format format = HUMAN_READABLE_YAUCL,
                   const std::vector<std::string>& declare_files = {"data/testing/SimpleComposition.txt"},
                   bool doDebugServer = false,
                   const std::string& benchmarking_file = "",
                   const std::string& sqlminer_dump_dir = "",
                   bool doStats = true) {
    Environment env;
    env.clear();
    env.doStats = doStats;

    if (!std::filesystem::exists(std::filesystem::path(log_file))) {
        std::cerr << "ERROR: the log file is missing: cannot run the pipeline! " << log_file << std::endl;
        exit(1);
    }

    std::cout << "Loading the log file: " << log_file << std::endl;
    env.load_log(format, true, log_file, false);
    if (!sqlminer_dump_dir.empty()) {
        env.dump_log_for_sqlminer(sqlminer_dump_dir);
    }

    if (declare_files.empty()) {
        std::cout << env.experiment_logger << std::endl;
    } else for (const auto& declare_file : declare_files) {
        std::cout << "Loading the declarative model from file: " << declare_file << std::endl;
        env.load_model(declare_file);
        env.print_model(std::cout); // DEBUG
        //////////////////////////////////////////////////////////////////

        env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
        //env.set_grounding_parameters(grounding_strategy);
        env.doGrounding();
        env.print_grounded_model(std::cout); // DEBUG
        //////////////////////////////////////////////////////////////////

        env.set_atomization_parameters("p", 10);
        //env.set_atomization_parameters(std::filesystem::path(atomization_conf));
        //////////////////////////////////////////////////////////////////

        std::cout << "Loading the atomization tables given the model" << std::endl;
        env.init_atomize_tables();
        env.print_grounding_tables(std::cout);
        //////////////////////////////////////////////////////////////////

        std::cout << "Atomizing the declare formulae" << std::endl;
        env.first_atomize_model();
        env.print_grounded_model(std::cout); // DEBUG
        //////////////////////////////////////////////////////////////////

        auto ref = env.query_model(0);
        std::cout << ref.result << std::endl;
        std::cout << env.experiment_logger << std::endl;
        if (doDebugServer) {
            env.server(ref);
        }
        if (!benchmarking_file.empty()) {
            std::filesystem::path F(benchmarking_file);
            bool doIHaveToWriteTheHeader = !std::filesystem::exists(F);
            std::ofstream outF{benchmarking_file, std::ios_base::app};
            if (doIHaveToWriteTheHeader)
                env.experiment_logger.log_csv_file_header(outF);
            env.experiment_logger.log_csv_file(outF);
        }
    }


}

void test_data_query(const std::string& log_file = "data/testing/log_until.txt",
                   const std::string& declare_file = "data/testing/declare5.powerdecl",
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
    env.min_threshold = 1;
    env.c = 10.0;

    //std::cout << env.range_query({"x", LEQ, 2.0}) << std::endl;

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
        {{"A", {{"x", 2.0},{"y", 3.0}}},
         {"A", {{"x", 1.0},{"y", 6.0}}},
         {"A", {{"x", 2.0},{"y", 3.0}}},
         {"B", {{"x", 2.0},{"y", 6.0}}},
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
#include <knobab/flloat_deps/ParseFFLOATDot.h>


template <typename T>
size_t
generateBenchmarkForTests(const std::string &log_file,
                          size_t modelNo,
                          const T &templatu,
                          std::ofstream &file) {
    Environment env;
    env.clear();
    env.cache_declare_templates_as_graphs();
    env.load_log(HUMAN_READABLE_YAUCL, true, log_file, false);
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
    env.cache_declare_templates_as_graphs();

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

    env.load_log(HUMAN_READABLE_YAUCL, true, log_file, false);
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
                                                                                            //DeclareDataAware::unary(Absence2, "C", 1),
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

    std::cout << "========BASIC OPERATORS=========" << std::endl;
    std::pair<const uint32_t, const uint32_t>  data = db.resolveCountingData(toSearch);

    //std::unordered_map<uint32_t, double> found = db.exists(data, searchAmount);
    //std::cout << "========EXISTS " + toSearch + " " + std::to_string(searchAmount) + "TIMES========="  << std::endl << "RESULT: " << std::endl << found << std::endl;
    Result initAVec = db.init(toSearch, minThreshHold);
    //std::cout << "========Init " + toSearch + "=========" << std::endl << std::setw(8) << "Result: " << initAVec << std::endl;

    Result endsAVec = db.ends(toSearch, minThreshHold);
    //std::cout << "========Ends " + toSearch + "=========" << std::endl << std::setw(8) << "Result: " << endsAVec << std::endl << std::endl;

//
    std::cout << "========SET OPERATORS=========" << std::endl;
    Result timedUnionNoPred, timedUnionWithPred, untimedUnionNoPred, untimedUnionWithPred;
    setUnion( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
             std::back_inserter(timedUnionNoPred), Aggregators::maxSimilarity<double, double, double>);
    setUnion( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
             std::back_inserter(timedUnionNoPred), Aggregators::maxSimilarity<double, double, double>, &predManager);
    setUnionUntimed( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
             std::back_inserter(untimedUnionNoPred), Aggregators::maxSimilarity<double, double, double>);
    setUnionUntimed( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
             std::back_inserter(untimedUnionWithPred), Aggregators::maxSimilarity<double, double, double>, &predManager);

    std::cout << "========Union timed no predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec << std::endl << std::setw(9) << "and: "
              << endsAVec << std::endl << std::setw(9) << "Result: " << timedUnionNoPred << std::endl;
    std::cout << "========Union timed with predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec << std::endl << std::setw(9) << "and: "
              << endsAVec << std::endl << std::setw(9) << "Result: " << timedUnionNoPred << std::endl;
    std::cout << "========Union untimed no predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec << std::endl << std::setw(9) << "and: "
              << endsAVec << std::endl << std::setw(9) << "Result: " << untimedUnionNoPred << std::endl;
    std::cout << "========Union untimed with predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec << std::endl << std::setw(9) << "and: "
              << endsAVec << std::endl << std::setw(9) << "Result: " << untimedUnionWithPred << std::endl;

    Result timedIntersectionNoPred, timedIntersectionWithPred, untimedIntersectionNoPred, untimedIntersectionWithPred;
    setIntersection( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
                    std::back_inserter(timedIntersectionNoPred), Aggregators::joinSimilarity<double, double, double>);
    setIntersection( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
                    std::back_inserter(timedIntersectionWithPred), Aggregators::joinSimilarity<double, double, double>, &predManager);
    setIntersectionUntimed( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
                    std::back_inserter(untimedIntersectionNoPred), Aggregators::joinSimilarity<double, double, double>);
    setIntersectionUntimed( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
                    std::back_inserter(untimedIntersectionWithPred), Aggregators::joinSimilarity<double, double, double>, &predManager);

    std::cout << "========Intersection timed no predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec << std::endl << std::setw(9) << "and: "
    << endsAVec << std::endl << std::setw(9) << "Result: " << timedIntersectionNoPred << std::endl;
    std::cout << "========Intersection timed with predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec << std::endl << std::setw(9) << "and: "
              << endsAVec << std::endl << std::setw(9) << "Result: " << timedIntersectionWithPred << std::endl;
    std::cout << "========Intersection untimed no predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec << std::endl << std::setw(9) << "and: "
    << endsAVec << std::endl << std::setw(9) << "Result: " << untimedIntersectionNoPred << std::endl << std::endl;
    std::cout << "========Intersection untimed with predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec << std::endl << std::setw(9) << "and: "
              << endsAVec << std::endl << std::setw(9) << "Result: " << untimedIntersectionWithPred << std::endl << std::endl;
/*
    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> unionNoPred;

    setUnion( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
             std::back_inserter(unionNoPred), Aggregators::maxSimilarity<double, double, double>);

    std::cout << "========UNION NO PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec << std::endl << "AND" << std::endl
    << endsAVec << std::endl << "RESULT: " << std::endl << unionNoPred << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> unionPred;
    setUnion( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
             std::back_inserter(unionPred), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "========UNION WITH PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec << std::endl << "AND" << std::endl
    << endsAVec << std::endl << "RESULT: " << std::endl << unionPred << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> intersectionNoPred;
    setIntersection( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
                    std::back_inserter(intersectionNoPred), Aggregators::joinSimilarity<double, double, double>);
    std::cout << "========INTERSECTION NO PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec << std::endl << "AND" << std::endl
    << endsAVec << std::endl << "RESULT: " << std::endl << intersectionNoPred << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> intersectionPred;
    setIntersection( initAVec.begin(), initAVec.end(), endsAVec.begin(), endsAVec.end(),
                    std::back_inserter(intersectionPred), Aggregators::joinSimilarity<double, double, double>, &predManager);
    std::cout << "========INTERSECTION WITH PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec << std::endl << "AND" << std::endl
    << endsAVec << std::endl << "RESULT: " << std::endl << intersectionPred << std::endl;
*/

    //aaab from trace 0
    Result aOccurences{}, bOccurences{}, aBOccurences{};
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

    std::cout << "========DATA=========" << std::endl;
    std::cout << "Set 1: "  << aOccurences << std::endl << "Set 2: " << bOccurences << std::endl << std::endl;

    std::cout << "========LTLF OPERATAORS=========" << std::endl;
    std::cout << "========Next========" << std::endl << std::setw(14) << "Temporal: " << next(0, 0, 1, aOccurences) << std::endl << std::setw(14) << "Non-Temporal: " << next(aOccurences) << std::endl;
    std::cout << "========Global========" << std::endl << std::setw(14) << "Temporal: " << global(0, 0, 2, aOccurences) << std::endl << std::setw(14) << "Non-Temporal: " << global(aOccurences, db.act_table_by_act_id.getTraceLengths()) << std::endl;
    std::cout << "========Future========" << std::endl << std::setw(14) << "Temporal: " <<  future(0, 1, 2, aOccurences) << std::endl << std::setw(14) << "Non-Temporal: " <<  future(aOccurences) << std::endl;
    std::cout << "========Until========" << std::endl << std::setw(14) << "Temporal: " << until(0, 0, 2, aOccurences, bOccurences) << std::endl << std::setw(14) << "Non-Temporal: " << until(aOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths()) << std::endl << std::endl;

    std::cout << "========SET OPERATORS=========" << std::endl;
    Result e1, e2, e3, e4, e5, e6, e7, e8;
    setUnion( aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e1), Aggregators::maxSimilarity<double, double, double>);
    setUnion( aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e2), Aggregators::maxSimilarity<double, double, double>,&predManager);
    setUnionUntimed( aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e3), Aggregators::maxSimilarity<double, double, double>);
    setUnionUntimed( aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e4), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "========Ltlf Union timed no predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e1 << std::endl;
    std::cout << "========Ltlf Union timed with predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e2 << std::endl;
    std::cout << "========Ltlf Union untimed no predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e3 << std::endl;
    ;std::cout << "========Ltlf Union untimed with predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
               << bOccurences << std::endl << std::setw(9) << "Result: " << e4 << std::endl;

    setIntersection( aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e5), Aggregators::maxSimilarity<double, double, double>);
    setIntersection( aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e6), Aggregators::maxSimilarity<double, double, double>, &predManager);
    setIntersectionUntimed( aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e7), Aggregators::maxSimilarity<double, double, double>);
    setIntersectionUntimed( aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e8), Aggregators::maxSimilarity<double, double, double>,nullptr);
    std::cout << "========Ltlf Intersection timed no predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e5 << std::endl;
    std::cout << "========Ltlf Intersection timed with predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e6 << std::endl;
    std::cout << "========Ltlf Intersection untimed no predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e7 << std::endl;
    std::cout << "========Ltlf Intersection untimed with predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e8 << std::endl <<std::endl;

    std::cout << "========DECLARE CLAUSES=========" << std::endl;
    std::cout << "========Choice=========" <<  std::endl << DChoice(aOccurences, bOccurences, nullptr) << std::endl;
    std::cout << "========ExclusiveChoice=========" << std::endl << ExclusiveChoice(aOccurences, bOccurences, bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::cout << "========ResponseExistence=========" << std::endl << ResponseExistence(aOccurences, bOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::cout << "========Coexistence=========" << std::endl << Coexistence(aOccurences, bOccurences, bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(), nullptr) << std::endl;
    std::cout << "========NotCoexistence=========" << std::endl << NotCoexistence(bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(), nullptr, true) << std::endl;
    std::cout << "========ChainResponse=========" << std::endl << DChainResponse(aOccurences, bOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::cout << "========ChainPrecedence=========" << std::endl << DChainPrecedence(aOccurences, bOccurences, aOccurences,  db.getLastElements(), db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::cout << "========ChainSuccession=========" << std::endl << DChainSuccession(aOccurences, bOccurences,  bOccurences, aOccurences,  db.getLastElements(), db.act_table_by_act_id.getTraceLengths(), nullptr) << std::endl;
    std::cout << "========NegationChainSuccession=========" << std::endl << NegationChainSuccession(aOccurences, bOccurences,  bOccurences, aOccurences, db.getLastElements(), db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::flush(std::cout);
#if 0
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


    dataContainer chainPrecedence = DChainPrecedence(aOccurences, bOccurences, aOccurences, /*db.getNotFirstElements(),*/ db.getLastElements(), db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========ChainPrecedence=========" << std::endl << chainPrecedence << std::endl;
    std::flush(std::cout);

    dataContainer chainSuccession = DChainSuccession(aOccurences, bOccurences,  bOccurences, aOccurences, /*db.getNotFirstElements(),*/ db.getLastElements(), db.act_table_by_act_id.getTraceLengths(), nullptr);
    std::cout << "========ChainSuccession=========" << std::endl << chainSuccession << std::endl;
    std::flush(std::cout);

    dataContainer negationChainSuccession = NegationChainSuccession(aOccurences, bOccurences,  bOccurences, aOccurences, db.getLastElements(), db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========NegationChainSuccession=========" << std::endl << negationChainSuccession << std::endl;
    std::flush(std::cout);
  #endif
}



int main(int argc, char **argv) {
    bool setUpServer = false;
    bool doStats = true;
    log_data_format format = HUMAN_READABLE_YAUCL;
    std::string log_file = "data/testing/log.txt";
    std::string benchmark = "";
    std::string sql_miner_dump_folder = "";
    std::vector<std::string> queriesV{};
    args::ArgumentParser parser("KnoBAB  (c) 2020-2022 by Giacomo Bergami & Samuel 'Sam' Appleby.", "This free and open software program implements the MaxSat problem via a Knowledge Base, KnoBAB. Nicer things are still to come!");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Group file_format(parser, "This group is all exclusive:", args::Group::Validators::AtMostOne);
    args::ValueFlag<std::string> logFile(file_format, "Log", "The Log, in human readable format, to load into the knowledgebase", {'l', "log"});
    args::ValueFlag<std::string> xesFile(file_format, "XES", "The Log in xes format to load into the knowledgebase", {'x', "xes"});
    args::ValueFlag<std::string> tabFile(file_format, "TAB", "The Log in a tab separated format, with no event payload, to load into the knowledgebase", {'t', "tab"});

    args::Group group(parser, "You can use the following parameters", args::Group::Validators::DontCare, args::Options::Global);
    args::Flag server(group, "server", "Runs the HTTP server for visualizing the internal representation of both the knowledge base and the associated query plan", {'s', "server"});
    args::Flag do_notcompute_trace_Stats(group, "do_not_compute_trace_stats", "Whether the code will lose time in calculating the statistics for the traces", {'n', "nostats"});
    args::ValueFlagList<std::string> queries(group, "Models/Queries", "The queries expressed as Declare models", {'d', "declare"});
    args::ValueFlag<std::string> benchmarkFile(group, "Benchmark File", "Appends the current Result data into a benchmark file", {'b', "csv"});
    args::ValueFlag<std::string>  sqlMinerDump(group, "SQLMinerDump", "If present, specifies the dump for the SQL miner representation", {'s', "sqlminer"});

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

    if (server) {
        setUpServer = true;
    }
    if (do_notcompute_trace_Stats) {
        doStats = false;
    }
    if (logFile) {
        log_file = args::get(logFile);
        format = HUMAN_READABLE_YAUCL;
    }
    if (xesFile) {
        log_file = args::get(xesFile);
        format = XES1;
    }
    if (tabFile) {
        log_file = args::get(tabFile);
        format = TAB_SEPARATED_EVENTS;
    }
    if (queries) {
        queriesV.clear();
        for (const auto& query: args::get(queries))
            queriesV.emplace_back(query);
    }
    if (benchmarkFile) {
        benchmark = args::get(benchmarkFile);
    }
    if (sqlMinerDump) {
        sql_miner_dump_folder = args::get(sqlMinerDump);
    }
    whole_testing(log_file, format, queriesV, setUpServer, benchmark, sql_miner_dump_folder, doStats);

    //generate_nonunary_templates();
    //test_data_query();
    //test_fsm();
    //test_declare();
    //test_grounding();
    //generate_traces();
    //ltlf_operators_testing();
    //sam_testing();

    // --declare data/testing/AbsenceA.txt --server --log data/testing/log_until.txt

    // --sqlminer /home/giacomo/IdeaProjects/JavaConcurrentAPI/SQLMinerBenchmarker/log --log data/testing/log_until.txt

    // --sqlminer=/home/giacomo/IdeaProjects/JavaConcurrentAPI/SQLMinerBenchmarker/log --log=data/testing/log_response.txt --declare=data/testing/response.powerdecl --server
    // --sqlminer=C:/Users/Sam/Documents/Codebases/knobabBenchmark/knobab/competitors/SQLMinerBenchmarker/log --csv=test.csv --log=data/testing/log_response.txt
    // --sqlminer=C:/Users/Sam/Documents/Codebases/knobabBenchmark/knobab/competitors/SQLMinerBenchmarker/log --csv=test.csv --xes=data/testing/xes/concept_drift_detection_10000.xes
    //  https://ieee-dataport.org/open-access/synthetic-event-logs-concept-drift-detection
    // --tab=data/testing/ltlf/WeakUntil --sqlminer=/home/giacomo/IdeaProjects/JavaConcurrentAPI/SQLMinerBenchmarker/log

    return 0;
}
