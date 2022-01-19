//
// Created by giacomo on 26/12/20.
//

#include "knobab/ActTable.h"
#include <cmath>

uint16_t cast_to_float(size_t x, size_t l) {
    const double at16 = std::pow(2, 16) - 1;
    return (uint16_t)((double)x)/((double)l) * at16;
}

ActTable::record::record() : record{0,0,0,nullptr, nullptr} {}

ActTable::record::record(act_t act, trace_t id, time_t time, ActTable::record *prev, ActTable::record *next) : prev{prev}, next{next} {
    //entry.id.parts.future = 0;
    entry.id.parts.event_id = time;
    entry.id.parts.trace_id = id;
    entry.id.parts.act = act;
}

bool ActTable::record::operator<(const ActTable::record &rhs) const { return entry < rhs.entry; }

bool ActTable::record::operator>(const ActTable::record &rhs) const { return rhs < *this; }

bool ActTable::record::operator<=(const ActTable::record &rhs) const { return !(rhs < *this); }

bool ActTable::record::operator>=(const ActTable::record &rhs) const { return !(*this < rhs); }

bool ActTable::record::operator==(const ActTable::record &rhs) const {
    return entry == rhs.entry;
}

bool ActTable::record::operator!=(const ActTable::record &rhs) const {
    return !(rhs == *this);
}

#include <cassert>
#include <iostream>

ActTable::record *ActTable::load_record(trace_t id, act_t act, time_t time, ActTable::record *prev, ActTable::record *next) {
    /*expectedOrdering.emplace_back(id, time, table.size());
    table.emplace_back(id, time, act, prev, next);
    return &table.back();*/
    {
        const size_t N = builder.act_id_to_trace_id_and_time.size();
        assert(N >= act);
        if (N == act) {
            builder.act_id_to_trace_id_and_time.emplace_back().emplace_back(id, time);
        } else if (N > act){
            builder.act_id_to_trace_id_and_time[act].emplace_back(id, time);
        }
    }
    {
        const size_t M = builder.trace_id_to_event_id_to_offset.size();
        assert(M >= id);
        std::vector<size_t>* ptr;
        if (M == id) {
            assert(trace_length.size() == M);
            ptr = &builder.trace_id_to_event_id_to_offset.emplace_back();
            trace_length.emplace_back(1);
        } else {
            assert((M-1) == id);
            ptr = &builder.trace_id_to_event_id_to_offset.back();
            trace_length[id]++;
        }
        assert(ptr->size() == time);
        ptr->emplace_back(time);
    }
}

#include <cassert>
#include <algorithm>

void ActTable::sanityCheck() {
    assert(std::is_sorted(table.begin(), table.end()));
}

const std::vector<std::vector<size_t>> & ActTable::indexing1() { // todo: rename as indexing, and remove expectedOrdering from emplace_back, instead, put in
    size_t offset = 0;
    // Phase 1
    for (size_t k = 0, N = builder.act_id_to_trace_id_and_time.size(); k < N; k++) {
        primary_index.emplace_back(offset);
        auto& ref = builder.act_id_to_trace_id_and_time[k];
        for (const std::pair<trace_t, event_t>& cp : ref) {
            // TALK TO GIACOMO ABOUT BELOW, TIME VALS MAY NOT BE RATIOED PROPERLY -- 0, 0.25, 0.75 ?????
            table.emplace_back(k,
                               cp.first,
                               cast_to_float(cp.second, trace_length.at(cp.first)),
                               nullptr, nullptr);
            builder.trace_id_to_event_id_to_offset[cp.first][cp.second] = offset++;
        }
        ref.clear(); // freeing some memory
    }
    builder.act_id_to_trace_id_and_time.clear(); // freeing some memory
    return builder.trace_id_to_event_id_to_offset;
#if 0
    // Phase 2, creating the secondary index, for accessing the beginning and the end of the trace from the table
    for (size_t sigma_id = 0, M = builder.trace_id_to_event_id_to_offset.size(); sigma_id < M ; sigma_id++) {
        auto& ref = builder.trace_id_to_event_id_to_offset[sigma_id];
        for (size_t time = 0, T = ref.size(); time < T; time++) {
            size_t offset = ref[time];
            auto& real_ref = table[offset];
            if (time == 0) {
                assert(secondary_index.size() == sigma_id);
                secondary_index.emplace_back(&real_ref, &table[ref.back()]);
            }
            if (time < T-1) {
                real_ref.next = &table[ref.at(time+1)];
            }
            if (time > 0) {
                real_ref.prev = &table[ref.at(time-1)];
            }
        }
        //ref.clear();
    }
    //builder.trace_id_to_event_id_to_offset.clear();
#endif
}


void ActTable::indexing2() { // todo: rename as indexing, and remove expectedOrdering from emplace_back, instead, put in

    // Phase 2, creating the secondary index, for accessing the beginning and the end of the trace from the table
    for (size_t sigma_id = 0, M = builder.trace_id_to_event_id_to_offset.size(); sigma_id < M ; sigma_id++) {
        auto& ref = builder.trace_id_to_event_id_to_offset[sigma_id];
        for (size_t time = 0, T = ref.size(); time < T; time++) {
            size_t offset = ref[time];
            auto& real_ref = table[offset];
            if (time == 0) {
                assert(secondary_index.size() == sigma_id);
                secondary_index.emplace_back(&real_ref, &table[ref.back()]);
            }
            if (time < T-1) {
                real_ref.next = &table[ref.at(time+1)];
            }
            if (time > 0) {
                real_ref.prev = &table[ref.at(time-1)];
            }
        }
        ref.clear();
    }
    builder.trace_id_to_event_id_to_offset.clear();
}

std::pair<const ActTable::record *, const ActTable::record *> ActTable::resolve_index(act_t id) const {
    if (primary_index.size() < id)
        return {nullptr, nullptr};
    else {
        return {table.data() + primary_index.at(id),
                ((id == (primary_index.size() - 1)) ? (const record*)primary_index.back() : table.data() + (primary_index.at(id+1) - 1))};      // Pointers to first and last records from Act Table subsection
    }
}
