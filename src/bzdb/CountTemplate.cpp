//
// Created by giacomo on 26/12/20.
//

#include "bzdb/CountTemplate.h"

void CountTemplate::emplace_back(uint16_t fact, uint32_t trace_id, uint16_t event_pos) {
    table.emplace_back(fact, trace_id, event_pos);
}
void CountTemplate::emplace_back(const uint_least64_t &monotone_hash) {
    table.emplace_back(monotone_hash);
}
void CountTemplate::sanityCheck() {
    assert(std::is_sorted(table.begin(), table.end()));
}

void CountTemplate::sort() {
    std::sort(table.begin(), table.end());
}
