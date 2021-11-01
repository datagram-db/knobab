//
// Created by giacomo on 26/12/20.
//

#ifndef BZDB_ATTRIBUTETABLE_H
#define BZDB_ATTRIBUTETABLE_H

#include <string>
#include <bzdb/oid.h>


struct AttributeTable {
    std::string attr_name;

    struct record {
        act_t   act;

        size_t  act_table_offset;
    };


};


#endif //BZDB_ATTRIBUTETABLE_H
