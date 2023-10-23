//
// Created by giacomo on 23/10/23.
//

#include "knobab/server/dataStructures/knobab_data_loader.h"


void load_into_knowledge_base(log_data_format format, bool loadData, std::istream &stream, KnowledgeBase &output,
                              const std::string &filename) {
    trace_visitor* tv = (trace_visitor*)&output;
    scan_through_trace_visitor(format, loadData, stream, tv, filename);
}