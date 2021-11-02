#include <iostream>

#include "KnowledgeBase.h"
#include "data_loader.h"
#include <yaucl/bpm/DataTraceParse.h>


int main() {


    /// Creating an instance of the knowledge base, that is going to store all the traces in the log!
    KnowledgeBase db;

    /// Loading a log file into the database
    load_into_small_databse(HUMAN_READABLE_YAUCL, true, "/home/giacomo/CLionProjects/bzdb/log_1.txt", db);

    /// Indexing the data structures
    /// TODO: these indices might be written in secondary memory as well!
    db.index_data_structures();

    /// Debugging purposes: checking whether the thing that I obtain by printing corresponds to the one that I obtained before.
    /// This is just an isomorphism proof, that states that I can always reconstruct the original information from the
    /// given representation
    db.reconstruct_trace_with_data(std::cout);


    return 0;
}
