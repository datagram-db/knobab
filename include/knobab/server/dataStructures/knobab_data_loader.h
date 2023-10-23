//
// Created by giacomo on 23/10/23.
//

#ifndef KNOBAB_SERVER_KNOBAB_DATA_LOADER_H
#define KNOBAB_SERVER_KNOBAB_DATA_LOADER_H

#include "knobab/server/tables/KnowledgeBase.h"
#include "yaucl/bpm/structures/log/data_loader.h"

void load_into_knowledge_base(log_data_format format, bool loadData, std::istream &stream, KnowledgeBase &output,
                              const std::string &filename);

#endif //KNOBAB_SERVER_KNOBAB_DATA_LOADER_H
