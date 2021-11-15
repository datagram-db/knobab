//
// Created by giacomo on 26/12/20.
//

#include "bzdb/CountTemplate.h"

void CountTemplate::load_record(uint16_t act, uint32_t trace_id, uint16_t event_pos) {
    sparseTable[act][trace_id] = event_pos;
    //table.emplace_back(act, trace_id, event_pos);
}
/*void CountTemplate::emplace_back(const uint_least64_t &monotone_hash) {
    table.emplace_back(monotone_hash);
}*/
void CountTemplate::sanityCheck() {
    assert(std::is_sorted(table.begin(), table.end()));
}

void CountTemplate::sort() {
    std::sort(table.begin(), table.end());
}

void CountTemplate::indexing(uint16_t maxAct, uint32_t maxTraceId) {
    this->maxAct = maxAct;
    this->maxTraceId = maxTraceId;
    for (uint16_t fact = 0; fact <= maxAct; fact++) {
        auto it = sparseTable.find(fact);
        for (uint32_t sigma_id = 0; sigma_id <= maxTraceId; sigma_id++) {
            if (it == sparseTable.end())
                table.emplace_back(fact, sigma_id, 0);
            else {
                auto it2 = it->second.find(sigma_id);
                if (it2 == it->second.end())
                    table.emplace_back(fact, sigma_id, 0);
                else
                    table.emplace_back(fact, sigma_id, it2->second);
            }
        }
    }
    sparseTable.clear();
}

std::pair<const oid *, const oid *> CountTemplate::resolve_primary_index(uint16_t actId) {
    if (actId < maxAct) {
        return {table.data() + (maxTraceId * actId), table.data() + (maxTraceId * (actId+1) -1)};
    } else {
        return {nullptr, nullptr};
    }
}