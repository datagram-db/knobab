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
    uint16_t resolve_length(const uint16_t actId, const uint32_t traceId) const {
        return (actId < maxAct) && (traceId < maxTraceId) ? table.at(
                (maxTraceId + 1) * actId + traceId).id.parts.event_id : 0;
    }
    std::pair<const oid*, const oid*> resolve_primary_index2(const uint16_t actId) const {
        if (actId < maxAct) {
            const uint32_t start = (maxTraceId + 1) * actId;
            const uint32_t end = start + maxTraceId;
            return {table.data() + start, table.data() + end};       // Pointers to first and last oid from Count Table subsection
        } else {
            return {nullptr, nullptr};
        }
    }
    std::pair<const uint32_t, const uint32_t> resolve_primary_index(const uint16_t actId) const;

    std::vector<size_t> actCounting() {
        std::vector<size_t> result(maxAct, 0);
        for (const auto& ref : table)
            result[ref.id.parts.act] += ref.id.parts.event_id;
        return result;
    }

    ///void emplace_back(const uint_least64_t& monotone_hash);
    void sort();
    void sanityCheck();
    void clear() {
        table.clear();
    }

    uint32_t nTraces() const { return maxTraceId+1; }
    uint16_t nAct() const { return maxAct+1; }

    friend std::ostream &operator<<(std::ostream &os, const CountTemplate &aTemplate);

private:
    uint16_t maxAct;
    uint32_t maxTraceId;
    std::unordered_map<uint16_t, std::unordered_map<uint32_t, uint16_t>> sparseTable;
};


#endif //BZDB_COUNTTEMPLATE_H
