#include <iostream>

#include "bzdb/KnowledgeBase.h"
#include "yaucl/bpm/data_loader.h"

int main() {
    /// Creating an instance of the knowledge base, that is going to store all the traces in the log!
    KnowledgeBase db;

    /// Loading a log file into the database
    load_into_knowledge_base(HUMAN_READABLE_YAUCL, true, "/home/giacomo/CLionProjects/bzdb/log_1.txt", db);

    /// Indexing the data structures
    /// TODO: these indices might be written in secondary memory as well!
    db.index_data_structures();

    /// Debugging purposes: checking whether the thing that I obtain by printing corresponds to the one that I obtained before.
    /// This is just an isomorphism proof, that states that I can always reconstruct the original information from the
    /// given representation
    db.reconstruct_trace_with_data(std::cout);


    return 0;
}
