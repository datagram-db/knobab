//
// Created by giacomo on 26/12/20.
//

#include "bzdb/KnowledgeBase.h"
#include <cassert>
#include <cmath>

KnowledgeBase::KnowledgeBase() : alreadySet{false} {
    status = FinishParsing;
}

void KnowledgeBase::reconstruct_trace_no_data(std::ostream &os) {
    for (size_t trace_id = 0, N = act_table_by_act_id.secondary_index.size(); trace_id < N; trace_id++) {
        os << "Trace #" << trace_id << std::endl << "\t- ";
        const auto& ref = act_table_by_act_id.secondary_index[trace_id];
        auto ptr = ref.first;
        while (ptr) {
            os << event_label_mapper.get(ptr->entry.id.parts.act) << "; ";
            ptr = ptr->next;
        };
        os << std::endl;
    }
}


void KnowledgeBase::reconstruct_trace_with_data(std::ostream &os) {
    for (size_t trace_id = 0, N = act_table_by_act_id.secondary_index.size(); trace_id < N; trace_id++) {
        os << "Trace #" << trace_id << std::endl << "\t- ";
        const auto& ref = act_table_by_act_id.secondary_index[trace_id];
        auto ptr = ref.first;
        while (ptr) {
            os << event_label_mapper.get(ptr->entry.id.parts.act) << "{ ";
            for (const auto& attr_table : this->attribute_name_to_table) {
                ptrdiff_t offset = ptr - act_table_by_act_id.table.data();
                const AttributeTable::record* recordPtr = attr_table.second.resolve_record_if_exists(offset);
                if (recordPtr) {
                    os << attr_table.first << '=';
                    attr_table.second.resolve_and_print(os, *recordPtr);
                    os << ", ";
                }
            }
            os << "} ";
            ptr = ptr->next;
        };
        os << std::endl;
    }
}


void KnowledgeBase::index_data_structures() {
    /// Filling the gaps in the sparse table, so to take into account the events that are missing from a given trace
    count_table.indexing(actId-1, this->noTraces-1);

    /// generating the primary index for the ActTable, and returning its intermediate index, M2
    const auto& idx = act_table_by_act_id.indexing1();

    /// Applying the intermediate index M2 to each attribute table, so to continue with the value indexing
    for (auto& attr_name_to_table_cp : attribute_name_to_table)
        attr_name_to_table_cp.second.index(idx, act_table_by_act_id.table.size()-1);

    /// Continuing to create the secondary index out of M2, as well as clearing M2
    act_table_by_act_id.indexing2();
}

///////////////// Event System

void KnowledgeBase::enterLog(const std::string &source, const std::string &name) {
    assert(!this->alreadySet);
    this->source = source;
    this->name = name;
    this->alreadySet = true;
    status = LogParsing;
    noTraces = 0;
    count_table.table.clear();
}

void KnowledgeBase::exitLog(const std::string &source, const std::string &name) {
    assert(this->alreadySet);
    assert(this->name == name);
    assert(this->source == source);
    status = FinishParsing;
    count_table.sort();
}

size_t KnowledgeBase::enterTrace(const std::string &trace_label) {
    currentEventId = 0;
    counting_reference.clear();
    status = TraceParsing;
    actId = 0;
    return (noTraces++);
}

void KnowledgeBase::exitTrace(size_t traceId) {
    /*if (act_begin_record_ptr != nullptr) {
        act_end_record_ptr = &act_table_by_act_id.table.back();
        for (size_t i = 0, N = std::distance(act_begin_record_ptr, act_end_record_ptr); i<N; i++) {
            act_end_record_ptr[i].entry.id.parts.event_id = cast_to_float(i, N-1);
        }
        act_begin_record_ptr = nullptr;
        act_end_record_ptr = nullptr;
    }*/
    assert(noTraces == (traceId+1));
    status = LogParsing;
}

size_t KnowledgeBase::enterEvent(size_t chronos_tick, const std::string &event_label) {
    actId = event_label_mapper.put(event_label).first;
    auto it = counting_reference.emplace(actId, 0UL);
    if (!it.second) {
        it.first->second++;
    }
    status = EventParsing;
    act_table_by_act_id.load_record(noTraces-1, actId, currentEventId, nullptr, nullptr);
    enterData_part(true);
    size_t currentEventIdRet = currentEventId++;
    visitField("__time", chronos_tick);
    ///miniActTable.emplace_back(actId, noTraces-1, currentEventId);
    return currentEventIdRet;
}

void KnowledgeBase::exitEvent(size_t event_id) {
    assert(currentEventId == (event_id+1));
    // using counting_reference to populate
    std::vector<std::pair<size_t, size_t>> cp;
    for (const auto& it : counting_reference)
        cp.emplace_back(it.first, it.second);
    std::sort(cp.begin(), cp.end());
    for (const auto& it : cp)
        count_table.load_record(it.first, noTraces-1, it.second);
    status = TraceParsing;
}

void KnowledgeBase::enterData_part(bool isEvent) {
    if (isEvent)
        assert(status == EventParsing);
    else
        assert(status == TraceParsing);
}

void KnowledgeBase::exitData_part(bool isEvent) {
    if (isEvent)
        assert(status == EventParsing);
    else
        assert(status == TraceParsing);
}

void KnowledgeBase::visitField(const std::string &key, bool value) {
    if (status == EventParsing) {
        auto it = attribute_name_to_table.find(key);
        if (it == attribute_name_to_table.end()) {
            attribute_name_to_table[key] = {key, BoolAtt};
            it = attribute_name_to_table.find(key);
        }
        it->second.record_load(actId, value, noTraces-1, currentEventId-1);
    }
}

void KnowledgeBase::visitField(const std::string &key, double value) {
    if (status == EventParsing) {
        auto it = attribute_name_to_table.find(key);
        if (it == attribute_name_to_table.end()) {
            attribute_name_to_table[key] = {key, DoubleAtt};
            it = attribute_name_to_table.find(key);
        }
        it->second.record_load(actId, value, noTraces-1, currentEventId-1);
    }
}

void KnowledgeBase::visitField(const std::string &key, const std::string &value) {
    if (status == EventParsing) {
        auto it = attribute_name_to_table.find(key);
        if (it == attribute_name_to_table.end()) {
            attribute_name_to_table[key] = {key, StringAtt, &string_values};
            it = attribute_name_to_table.find(key);
        }
        it->second.record_load(actId, value, noTraces-1, currentEventId-1);
    }
}

void KnowledgeBase::visitField(const std::string &key, size_t value) {
    if (status == EventParsing) {
        auto it = attribute_name_to_table.find(key);
        if (it == attribute_name_to_table.end()) {
            attribute_name_to_table[key] = {key, SizeTAtt};
            it = attribute_name_to_table.find(key);
        }
        it->second.record_load(actId, value, noTraces-1, currentEventId-1);
    }
}
