//
// Created by giacomo on 31/10/2021.
//

#include <istream>
#include "knobab/KnowledgeBase.h"

#ifndef BZDB_DATA_LOADER_H
#define BZDB_DATA_LOADER_H

enum log_data_format {
    HUMAN_READABLE_YAUCL,
    XES1,
    TAB_SEPARATED_EVENTS
};

void load_into_knowledge_base(log_data_format format, bool loadData, std::istream &stream, KnowledgeBase &output,
                              const std::string &filename);

#endif //BZDB_DATA_LOADER_H
