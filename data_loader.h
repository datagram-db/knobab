//
// Created by giacomo on 31/10/2021.
//

#include <istream>
#include "KnowledgeBase.h"

#ifndef BZDB_DATA_LOADER_H
#define BZDB_DATA_LOADER_H

enum log_data_format {
    HUMAN_READABLE_YAUCL,
    XES1
};

void load_into_knowledge_base(log_data_format format, bool loadData, const std::string &filename, KnowledgeBase& output);

#endif //BZDB_DATA_LOADER_H
