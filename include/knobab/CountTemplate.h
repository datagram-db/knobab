//
// Created by giacomo on 26/12/20.
//

#ifndef BZDB_COUNTTEMPLATE_H
#define BZDB_COUNTTEMPLATE_H

#include <vector>
#include <knobab/dataStructures/oid.h>
#include <algorithm>
#include <cassert>
#include <ostream>


struct CountTemplate {
    std::vector<oid>    table;

    CountTemplate() = default;
    CountTemplate(const CountTemplate&) = default;
    CountTemplate& operator=(const CountTemplate&) = default;

    void load_record(uint16_t act, uint32_t trace_id, uint16_t event_pos);
    void indexing(uint16_t maxAct, uint32_t maxTraceId);

    std::pair<const oid*, const oid*> resolve_primary_index(const uint16_t actId, uint32_t& start, uint32_t& send) const;

    std::pair<const uint32_t, const uint32_t> resolve_primary_index(const uint16_t actId) const;

    ///void emplace_back(const uint_least64_t& monotone_hash);
    void sort();
    void sanityCheck();
    void clear() {
        table.clear();
    }

    friend std::ostream &operator<<(std::ostream &os, const CountTemplate &aTemplate);

private:
    uint16_t maxAct;
    uint32_t maxTraceId;
    std::unordered_map<uint16_t, std::unordered_map<uint32_t, uint16_t>> sparseTable;
};


#endif //BZDB_COUNTTEMPLATE_H
