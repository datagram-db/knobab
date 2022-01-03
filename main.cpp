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

int main() {
    //test_declare();
    test_grounding();

    return 0;
}
