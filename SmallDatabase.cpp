//
// Created by giacomo on 26/12/20.
//

#include "SmallDatabase.h"
#include <cassert>
#include <cmath>



void SmallDatabase::enterLog(const std::string &source, const std::string &name) {
    assert(!this->alreadySet);
    this->source = source;
    this->name = name;
    this->alreadySet = true;
    status = LogParsing;
    noTraces = 0;
    count_table.table.clear();
}

void SmallDatabase::exitLog(const std::string &source, const std::string &name) {
    assert(this->alreadySet);
    assert(this->name == name);
    assert(this->source == source);
    status = FinishParsing;
    count_table.sort();
}

size_t SmallDatabase::enterTrace(const std::string &trace_label) {
    currentEventId = 0;
    counting_reference.clear();
    status = TraceParsing;
    return (noTraces++);
}

void SmallDatabase::exitTrace(size_t traceId) {
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

size_t SmallDatabase::enterEvent(size_t chronos_tick, const std::string &event_label) {
    size_t actId = event_label_mapper.put(event_label).first;
    auto it = counting_reference.emplace(actId, 0UL);
    if (!it.second) {
        it.first->second++;
    }
    status = EventParsing;
    act_table_by_act_id.load_record(noTraces-1, actId, currentEventId, nullptr, nullptr);
    enterData_part(true);
    visitField("__time", chronos_tick);
    ///miniActTable.emplace_back(actId, noTraces-1, currentEventId);
    return currentEventId++;
}

void SmallDatabase::exitEvent(size_t event_id) {
    assert(currentEventId == (event_id+1));
    // using counting_reference to populate
    std::vector<std::pair<size_t, size_t>> cp;
    for (const auto& it : counting_reference)
        cp.emplace_back(it.first, it.second);
    std::sort(cp.begin(), cp.end());
    for (const auto& it : cp)
        count_table.emplace_back(it.first, noTraces-1, it.second);
    status = TraceParsing;
}

void SmallDatabase::enterData_part(bool isEvent) {
    if (isEvent)
        assert(status == EventParsing);
    else
        assert(status == TraceParsing);
}

void SmallDatabase::exitData_part(bool isEvent) {
    if (isEvent)
        assert(status == EventParsing);
    else
        assert(status == TraceParsing);
}

void SmallDatabase::visitField(const std::string &key, bool value) {
    if (status == EventParsing) {
        visitField(key, value ? 1.0 : 0.0);
    } // For the moment, we are ignoring the trace payload
}

void SmallDatabase::visitField(const std::string &key, double value) {
    if (status == EventParsing) {
        // TODO: directly write the numerical value
    } // For the moment, we are ignoring the trace payload
}

void SmallDatabase::visitField(const std::string &key, const std::string &value) {
    if (status == EventParsing) {
        // TODO: directly write the numerical value, as well as create a database for the strings.
    } // For the moment, we are ignoring the trace payload
}

void SmallDatabase::visitField(const std::string &key, size_t value) {
    if (status == EventParsing) {
        visitField(key, (double)value);
    } // For the moment, we are ignoring the trace payload
}

void SmallDatabase::reconstruct_trace_no_data(std::ostream &os) {
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
