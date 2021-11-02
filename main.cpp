#include <iostream>

#include "SmallDatabase.h"
#include "data_loader.h"
#include <yaucl/bpm/DataTraceParse.h>


int main() {
    /*oid id1{1, 0, 0}, id2{2, 0, 0};
    std::cout << sizeof(_oid) << '\t' << sizeof(uint_least64_t) << '\t' << sizeof(_oid::S) << std::endl;
    std::cout << (id1 < id2) << std::endl;

    std::cout << "Hello, World!" << std::endl;*/
    assert(sizeof(double) == sizeof(size_t));

    SmallDatabase db;
    load_into_small_databse(HUMAN_READABLE_YAUCL, true, "/home/giacomo/CLionProjects/bzdb/log_1.txt", db);
    db.index_data_structures();
    db.reconstruct_trace_with_data(std::cout);


    return 0;
}
