#include <iostream>
#include <yaucl/bpm/declare/DeclareModelParse.h>
#include <yaucl/bpm/declare_to_dfa/DeclareNoDataTemplateCollect.h>
#include <bzdb/flloat_deps/flloat_wrapper.h>

#include "bzdb/KnowledgeBase.h"
#include "yaucl/bpm/log/data_loader.h"

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

int main() {
    DeclareModelParse dmp;
    std::ifstream file{"ex_2.powerdecl"};

    DeclareNoDataTemplateCollect tc;
    for (const auto& ref : dmp.load(file)) {
        tc.add(ref);
        std::cout << ref << std::endl;
    }
    std::unordered_set<std::string> SigmaAll{"A", "B"};
    tc.run<FLLOAT>(SigmaAll);

    return 0;
}
