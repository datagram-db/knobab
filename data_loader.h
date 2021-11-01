//
// Created by giacomo on 31/10/2021.
//

#include <istream>
#include "SmallDatabase.h"

#ifndef BZDB_DATA_LOADER_H
#define BZDB_DATA_LOADER_H

enum log_data_format {
    HUMAN_READABLE_YAUCL,
    XES1
};

void load_into_small_databse(log_data_format format, bool loadData, const std::string &filename, SmallDatabase& output);

#endif //BZDB_DATA_LOADER_H
