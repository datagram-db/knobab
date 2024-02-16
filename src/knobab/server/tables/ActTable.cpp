//
// Created by giacomo on 16/04/2022.
//

#include <knobab/server/tables/ActTable.h>
#include <cmath>

uint16_t cast_to_float(size_t x, size_t l) {
    const double at16 = std::pow(2, 16) - 1;
    return (uint16_t)((double)x)/((double)l) * at16;
}

uint16_t cast_to_float2(size_t x, size_t l) {
    const static double at16 = std::pow(2, 16) - 1;
    return l == 1 ? 0 : (uint16_t) (((double) x) / ((double) (l - 1)) * at16);
}

ActTable::record::record() : record{0,0,0,1} {}

ActTable::record::record(act_t act, trace_t id, time_t time, event_t span) /*: prev{prev}, next{next}*/ : span(span){
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

#include <yaucl/functional/assert.h>
#include <iostream>

void ActTable::load_record(trace_t id, act_t act, event_t time, event_t span) {
    {
        const size_t N = builder.act_id_to_trace_id_and_time.size();
        //DEBUG_ASSERT(N >= act);
        if (N == act) {
            builder.act_id_to_trace_id_and_time.emplace_back().emplace_back(id, time, span);
        } else if (N > act){
            //DEBUG_ASSERT(builder.act_id_to_trace_id_and_time.size() > act);
            builder.act_id_to_trace_id_and_time[act].emplace_back(id, time, span);
        }
    }
    {
        const size_t M = builder.trace_id_to_event_id_to_offset.size();
        //DEBUG_ASSERT(M >= id);
        if (M == id) {
            //DEBUG_ASSERT(trace_length.size() == M);
            if (trace_length.size() == M)
            {
                secondary_index_polyadic.emplace_back();
                builder.trace_id_to_event_id_to_offset.emplace_back();
                trace_length.push_back(1);
            }
        } else {
            //DEBUG_ASSERT((M-1) == id);
            if (((M-1) == id) && (trace_length.size() > id))
            {
//                DEBUG_ASSERT();
                trace_length[id]++;
            }
        }
        if (time == builder.trace_id_to_event_id_to_offset[id].size()) {
            builder.trace_id_to_event_id_to_offset[id].emplace_back();
            secondary_index_polyadic[id].emplace_back();
        }
    }
}

#include <cassert>
#include <algorithm>
#include <yaucl/functional/assert.h>


void ActTable::sanityCheck() {
    //assert(std::is_sorted(table.begin(), table.end()));
}

const std::vector<std::vector<std::unordered_map<act_t, std::vector<size_t>>>> & ActTable::indexing1() { // todo: rename as indexing, and remove expectedOrdering from emplace_back, instead, put in
    size_t offset = 0;
    // Phase 1
    for (size_t k = 0, N = builder.act_id_to_trace_id_and_time.size(); k < N; k++) {
        primary_index.emplace_back(offset);
        auto& ref = builder.act_id_to_trace_id_and_time[k];
        for (const std::tuple<trace_t, event_t, event_t>& cp : ref) {
            table.emplace_back(k,
                               std::get<0>(cp),
                               std::get<1>(cp),//cast_to_float(cp.second, trace_length.at(cp.first) - 1),
                               std::get<2>(cp));
            builder.trace_id_to_event_id_to_offset[std::get<0>(cp)][std::get<1>(cp)][k].emplace_back(offset++);
        }
        ref.clear(); // freeing some memory
    }
    builder.act_id_to_trace_id_and_time.clear(); // freeing some memory
    return builder.trace_id_to_event_id_to_offset;
}


void ActTable::indexing2() { // todo: rename as indexing, and remove expectedOrdering from emplace_back, instead, put in
    for (size_t sigma_id = 0, M = builder.trace_id_to_event_id_to_offset.size(); sigma_id < M ; sigma_id++) {
        auto& ref = builder.trace_id_to_event_id_to_offset[sigma_id];
        secondary_index.emplace_back(&secondary_index_polyadic[sigma_id][0], &secondary_index_polyadic[sigma_id][ref.size()-1]);
        for (size_t time = 0, T = ref.size(); time < T; time++) {
            for (const auto& [act, offsets] : ref[time]) {
                for (size_t offset : offsets) {
                    secondary_index_polyadic[sigma_id][time][act].emplace_back(table.data()+offset);
                    auto& real_ref = table[offset];
                    if (time < T-1) {
                        real_ref.next = &secondary_index_polyadic[sigma_id][time+1];
                    }
                    if (time > 0) {
                        real_ref.prev = &secondary_index_polyadic[sigma_id][time-1];
                    }
                }
            }

        }
    }
//
//    // Phase 2, creating the secondary index, for accessing the beginning and the end of the trace from the table
//    for (size_t sigma_id = 0, M = builder.trace_id_to_event_id_to_offset.size(); sigma_id < M ; sigma_id++) {
//        auto& ref = builder.trace_id_to_event_id_to_offset[sigma_id];
//        for (size_t time = 0, T = ref.size(); time < T; time++) {
//            size_t offset = ref[time];
//            auto& real_ref = table[offset];
//            if (time == 0) {
//                DEBUG_ASSERT(secondary_index.size() == sigma_id);
//                secondary_index.emplace_back(&real_ref, &table[ref.back()]);
//            }
//            if (time < T-1) {
//                real_ref.next = &table[ref.at(time+1)];
//            }
//            if (time > 0) {
//                real_ref.prev = &table[ref.at(time-1)];
//            }
//        }
//        ///ref.clear();
//    }
//    ///builder.trace_id_to_event_id_to_offset.clear();
}

//std::pair<const ActTable::record *, const ActTable::record *> ActTable::resolve_index(act_t id) const {
//    if (primary_index.size() < id)
//        return {nullptr, nullptr};
//    else {
//        return {table.data() + primary_index.at(id),
//                ((id == (primary_index.size() - 1)) ? (const record*)primary_index.back() : table.data() + (primary_index.at(id+1) - 1))};      // Pointers to first and last records from Act Table subsection
//    }
//}

std::pair<uint32_t, uint32_t> ActTable::resolve_index(act_t id) const {
    if (primary_index.size() < id)
        return {-1, -1};
    else {
        return {primary_index.at(id),
                ((id == (primary_index.size() - 1)) ? (table.size() - 1) : primary_index.at(id+1) - 1)};      // Pointers to first and last records from Act Table subsection
    }
}


std::ostream &operator<<(std::ostream &os, const ActTable &table) {
    const double at16 = std::pow(2, 16);
    //os << "          ActTable" << std::endl << "-------------------------------" << std::endl;
    size_t i = 0;
    auto ptr = table.table.data();
    os << "Act,TraceId,EventId,PREVPTR,NEXTPTR" << std::endl;
    for (const auto& ref : table.table) {
        os <<  ref.entry.id.parts.act << "," << ref.entry.id.parts.trace_id << "," << /*std::trunc((((double)ref.entry.id.parts.event_id)/at16)*table.trace_length[ ref.entry.id.parts.trace_id])*/ref.entry.id.parts.event_id  << ","//<< (
//                ref.prev == nullptr ? "-1" : "+"+std::to_string((ref.prev-ptr)))<<","<< (
//                   ref.next == nullptr ? "-1" : "+"+std::to_string((ref.next-ptr)))
                   << std::endl;
    }
    //os << std::endl << "-------------------------------" << std::endl;
    return os;
}

void ActTable::clear() {
    secondary_index.clear();
    primary_index.clear();
    table.clear();
}

