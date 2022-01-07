#include <iostream>
#include <yaucl/bpm/structures/declare/DeclareModelParse.h>
#include <yaucl/bpm/algos/transformations/declare_to_dfa/DeclareNoDataTemplateCollect.h>
#include <knobab/flloat_deps/flloat_wrapper.h>
#include <yaucl/bpm/structures/declare/CNFDeclareDataAware.h>

#include "knobab/KnowledgeBase.h"
#include "yaucl/bpm/structures/log/data_loader.h"
#include <unordered_map>
#include <set>
#include <string>
#include <knobab/predicates/testing_predicates.h>
#include <yaucl/functional/iterators.h>
#include <yaucl/bpm/algos/transformations/grounding.h>
#include <knobab/algorithms/atomization_pipeline.h>
#include <knobab/algorithms/kb_grounding.h>

class Environment {
    /// Creating an instance of the knowledge base, that is going to store all the traces in the log!
    KnowledgeBase db;
    AtomizingPipeline ap;
    GroundingStrategyConf grounding_conf;

    DeclareModelParse dmp;
    std::vector<DeclareDataAware> conjunctive_model;

    CNFDeclareDataAware grounding;

public:

    void clear() {
        db.clear();
        ap.clear();
        conjunctive_model.clear();
    }

    void load_log(log_data_format format, bool loadData, const std::string &filename) {
        load_into_knowledge_base(format, loadData, filename, db);
        db.index_data_structures();
    }

    void load_model(const std::string &model_file) {
        if (!std::filesystem::exists(std::filesystem::path(model_file))) {
            std::cerr << "ERROR: model file does not exist: " << model_file << std::endl;
            exit(1);
        }
        std::ifstream file{model_file};
        conjunctive_model = dmp.load(file, true);
    }

    void set_atomization_parameters(const std::string fresh_atom_label = "p",
                                    size_t mslength = MAXIMUM_STRING_LENGTH) {
        ap.fresh_atom_label = fresh_atom_label;
        ap.s_max = std::string(mslength, std::numeric_limits<char>::max());
        DataPredicate::MAX_STRING = ap.s_max;
        DataPredicate::msl = mslength;
    }

    void set_grounding_parameters(bool doPreliminaryFill = true,
                                  bool ignoreActForAttributes = false,
                                  bool creamOffSingleValues = true,
                                  GroundingStrategyConf::pruning_strategy ps = GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES
    ) {
        grounding_conf.doPreliminaryFill = doPreliminaryFill;
        grounding_conf.ignoreActForAttributes = ignoreActForAttributes;
        grounding_conf.creamOffSingleValues = creamOffSingleValues;
        grounding_conf.strategy1 = ps;
    }

    void init_atomize_tables() {
        collect_data_from_declare_disjunctive_model(ap, grounding);
    }

    void doGrounding() {
        grounding = GroundWhereStrategy(grounding_conf,
                                                db,
                                                conjunctive_model);
    }

    void print_model(std::ostream& os) {
        os << "Declarative Model: " << std::endl;
        os << "----------------------------------------" << std::endl;
        for (const auto& def : conjunctive_model)
            os << def << std::endl;

        os << "----------------------------------------" << std::endl;
        os << std::endl;
    }

    void print_grounded_model(std::ostream& os) {
        os << "Grounded Model: " << std::endl;
        os << "----------------------------------------" << std::endl;
        os << grounding << std::endl;
        os << "----------------------------------------" << std::endl;
        os << std::endl;
    }

    void print_knowledge_base(std::ostream& os) {
        os << "Knowledge Base: " << std::endl;
        os << "----------------------------------------" << std::endl;
        db.reconstruct_trace_with_data(os);
        os << "----------------------------------------" << std::endl;
        os << std::endl;
    }

    void print_grounding_tables(std::ostream& os) {
        os << "Grounding Tables: " << std::endl;
        os << "----------------------------------------" << std::endl;
        os << ap << std::endl;
        os << "----------------------------------------" << std::endl;
        os << std::endl;
    }

    void first_atomize_model() {
        atomize_model(ap, grounding);
    }

};

void test_kb() {
    /// Creating an instance of the knowledge base, that is going to store all the traces in the log!
    KnowledgeBase db;

    /// Loading a log file into the database
    load_into_knowledge_base(HUMAN_READABLE_YAUCL, true, "/home/giacomo/CLionProjects/knobab/log_1.txt", db);

    /// Indexing the data structures
    /// TODO: these indices might be written in secondary memory as well!
    db.index_data_structures();

    /// Debugging purposes: checking whether the thing that I obtain by printing corresponds to the one that I obtained before.
    /// This is just an isomorphism proof, that states that I can always reconstruct the original information from the
    /// given representation
    db.reconstruct_trace_with_data(std::cout);
}

void test_declare() {
    std::map<int, int> M;

    DeclareModelParse dmp;
    std::ifstream file{"ex_2.powerdecl"};

    DeclareNoDataTemplateCollect tc;
    for (const auto& ref : dmp.load(file, true)) {
        tc.add(ref);
        std::cout << ref << std::endl;
    }
    std::unordered_set<std::string> SigmaAll{"A", "B"};
    tc.run<FLLOAT>(SigmaAll);
}

void test_grounding() {
    if (false) {
        // Making sure that an empty interval is returned via invalidating the predicate
        DataPredicate pred1{"x", LT, 3};
        DataPredicate pred2{"x", GT, 5};
        assert(!pred1.intersect_with(pred2));
        std::cout << pred1 << std::endl;
    }

    if (false) {
        DeclareModelParse dmp;
        std::ifstream file{"ex_3.powerdecl"};

        std::unordered_map<std::string, union_minimal> mapLeft{{"x", 0.5},{"y", 0.7}, {"z", 2.3}, {"t", 2.9} };

        for (const auto& ref : dmp.load(file, false)) {
            auto tmp = instantiateWithValues(ref, mapLeft, mapLeft);
            if (tmp)
                std::cout << tmp.value() << std::endl;
        }
    }

    if (false) {
        DeclareModelParse dmp;
        std::ifstream file{"ex_3.powerdecl"};
        std::unordered_map<std::string, union_minimal> mapLeft{{"x", 0.5},{"y", 0.7}, {"z", 2.3}, {"t", 2.9} };
        for (const auto& ref : dmp.load(file, false)) {
            auto tmp = instantiateWithValues(ref, {}, mapLeft);
            if (tmp)
                std::cout << tmp.value() << std::endl;
        }
    }

     if (true){
        DeclareModelParse dmp;
        std::ifstream file{"ex_3.powerdecl"};
        std::unordered_map<std::string, union_minimal> mapLeft{{"x", 0.5},{"y", 0.7}, {"z", 2.3}, {"t", 2.9} };
        for (const auto& ref : dmp.load(file, false)) {
            auto tmp = instantiateWithValues(ref, mapLeft, {});
            if (tmp)
                std::cout << tmp.value() << std::endl;
        }
    }

}

#include <yaml-cpp/yaml.h>

void whole_testing(const std::string& log_file = "testing/log.txt",
                   const std::string& declare_file = "testing/declare1.powerdecl",
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

}

int main() {
    whole_testing();
    //test_declare();
    //test_grounding();

    return 0;
}