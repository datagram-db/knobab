//
// Created by giacomo on 26/12/20.
//

#include "knobab/KnowledgeBase.h"
#include <cassert>
#include <cmath>

KnowledgeBase::KnowledgeBase() : alreadySet{false} {
    status = FinishParsing;
}

void KnowledgeBase::reconstruct_trace_no_data(std::ostream &os) const {
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


void KnowledgeBase::reconstruct_trace_with_data(std::ostream &os) const {
    for (size_t trace_id = 0, N = act_table_by_act_id.secondary_index.size(); trace_id < N; trace_id++) {
        os << "Trace #" << trace_id << std::endl << "\t- ";
        const auto& ref = act_table_by_act_id.secondary_index[trace_id];
        auto ptr = ref.first;
        while (ptr) {
            // printing one event at a time
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
    count_table.indexing(actId, this->noTraces-1);

    /// generating the primary index for the ActTable, and returning its intermediate index, M2
    const auto& idx = act_table_by_act_id.indexing1();

    /// Applying the intermediate index M2 to each attribute table, so to continue with the value indexing
    for (auto& attr_name_to_table_cp : attribute_name_to_table)
        attr_name_to_table_cp.second.index(idx, act_table_by_act_id.table.size()-1);

    /// Continuing to create the secondary index out of M2, as well as clearing M2
    act_table_by_act_id.indexing2();
    act_table_by_act_id.sanityCheck();
}

///////////////// Event System
#include <iostream>

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
    auto it = counting_reference.emplace(actId, 1UL);
    if (!it.second) {
        // Existing key, increment the count val
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

void
KnowledgeBase::collectValuesFrom(std::set<union_type> &S, ssize_t trace_id, uint16_t act_id,
                                 const std::string &attribute_name) const {
    bool hasNoAttribute = attribute_name.empty();
    bool hasNoActId = act_id == (uint16_t)-1;
    ssize_t N = act_table_by_act_id.secondary_index.size();
    if (trace_id == -1) {
        for (size_t currentTraceId = 0; currentTraceId < N; currentTraceId++) {
            collectValuesAmongTraces(S, currentTraceId, act_id, hasNoActId, attribute_name, hasNoAttribute);
        }
    } else if ((trace_id >= 0) && (trace_id < N)) {
        collectValuesAmongTraces(S, trace_id, act_id, hasNoActId, attribute_name, hasNoAttribute);
    }
}

#include <bitset>


void KnowledgeBase::collectValuesAmongTraces(std::set<union_type> &S, size_t trace_id, act_t acts, bool hasNoActId,
                                             const std::string &attribute_name, bool hasNoAttribute) const {
    const auto& ref = act_table_by_act_id.secondary_index[trace_id];
    auto ptr = ref.first;
    while (ptr) {
        if (hasNoActId || (ptr->entry.id.parts.act == acts)) {
            for (const auto& attr_table : attribute_name_to_table) {
                ptrdiff_t offset = ptr - act_table_by_act_id.table.data();
                const AttributeTable::record* recordPtr = attr_table.second.resolve_record_if_exists(offset);
                if (recordPtr &&  (hasNoAttribute || (attribute_name == attr_table.first))) {
                    S.insert(attr_table.second.resolve(*recordPtr));
                }
            }
        }
        ptr = ptr->next;
    }
}

void
KnowledgeBase::collectValuesFrom(std::set<union_type> &S, const std::unordered_set<size_t> &trace_ids, uint16_t act_id,
                                 const std::string &attribute_name) const {

    bool hasNoAttribute = attribute_name.empty();
    bool hasNoActId = act_id == (uint16_t)-1;
    ssize_t N = act_table_by_act_id.secondary_index.size();
    for (size_t currentTraceId : trace_ids) {
        if (currentTraceId < N)
            collectValuesAmongTraces(S, currentTraceId, act_id, hasNoActId, attribute_name, hasNoAttribute);
    }
}

void KnowledgeBase::collectValuesFrom(
        std::unordered_map<std::string, std::unordered_map<std::string, std::set<union_type>>> &result,
        std::unordered_map<std::string, std::set<union_type>> &resultOtherValues,
        const std::unordered_set<trace_t> &trace_ids,
        const std::unordered_map<std::string, std::unordered_set<std::string>> &actToTables,
        const std::unordered_set<std::string> &otherValues) const {
    size_t N = act_table_by_act_id.secondary_index.size();
    if (trace_ids.empty()) {
        for (size_t currentTraceId = 0; currentTraceId < N; currentTraceId++) {
            collectValuesAmongTraces(result, resultOtherValues, actToTables, otherValues, currentTraceId);
        }
    } else {
        for (trace_t traceId : trace_ids) {
            if (traceId < N) {
                collectValuesAmongTraces(result, resultOtherValues, actToTables, otherValues, traceId);
            }
        }
    }
}

void KnowledgeBase::collectValuesAmongTraces(
        std::unordered_map<std::string, std::unordered_map<std::string, std::set<union_type>>> &result,
        std::unordered_map<std::string, std::set<union_type>> &resultOtherValues,
        const std::unordered_map<std::string, std::unordered_set<std::string>> &actToTables,
        const std::unordered_set<std::string> &otherValues, trace_t traceId) const {
    const auto& ref = act_table_by_act_id.secondary_index[traceId];
    ActTable::record* ptr = ref.first;
    while (ptr) {
        std::string sAct = event_label_mapper.get(ptr->entry.id.parts.act);
        auto it = actToTables.find(sAct);
        bool hasId = it != actToTables.end();
        if ((!resultOtherValues.empty()) || (hasId)) {
            for (const auto& attr_table : attribute_name_to_table) {
                ptrdiff_t offset = ptr - act_table_by_act_id.table.data();
                const AttributeTable::record* recordPtr = attr_table.second.resolve_record_if_exists(offset);
                if (recordPtr) {
                    if (hasId && it->second.contains(attr_table.first)) {
                        result[sAct][attr_table.first].insert(attr_table.second.resolve(*recordPtr));
                    }
                    if (otherValues.contains(attr_table.first)) {
                        resultOtherValues[attr_table.first].insert(attr_table.second.resolve(*recordPtr));
                    }
                }
            }
        }
        ptr = ptr->next;
    }
}

void KnowledgeBase::clear() {
    status = FinishParsing;
    source.clear();
    name.clear();
    noTraces = 0;
    count_table.clear();
    act_table_by_act_id.clear();
    attribute_name_to_table.clear();
    string_values.clear();
    noTraces = 0;
    currentEventId = 0;
    actId = 0;
}

void KnowledgeBase::load_data_without_antlr4(const KnowledgeBase::no_antlr_log &L, const std::string &source,
                                             const std::string &name) {
    enterLog(source, name);
    for (auto const& trace : L ){ //sigma_i
        size_t strace_id = enterTrace("bogus");
        size_t chronos = 0;
        enterData_part(false);
        exitData_part(false);
        for (auto const & event : trace) {
            auto const act = event.first;
            size_t event_id = enterEvent(chronos++, act);
            enterData_part(true);
            for (auto const& cp : event.second) {
                if (std::holds_alternative<bool>(cp.second)) {
                    visitField(cp.first, std::get<bool>(cp.second));
                } else if (std::holds_alternative<double>(cp.second)) {
                    visitField(cp.first, std::get<double>(cp.second));
                } else if (std::holds_alternative< std::string>(cp.second)) {
                    visitField(cp.first, std::get< std::string>(cp.second));
                }else if (std::holds_alternative< size_t>(cp.second)) {
                    visitField(cp.first, std::get< size_t>(cp.second));
                }
            }
            exitData_part(true);
            exitEvent(event_id);
        }
        exitTrace(strace_id);
    }
    exitLog(source, name);
}

uint16_t KnowledgeBase::getMappedValueFromAction(const std::string &act) const {
    try{
        return event_label_mapper.get(act);
    }
    catch(const std::exception& e){
        return -1;
    }
}

std::pair<const uint32_t, const uint32_t> KnowledgeBase::resolveCountingData(const std::string &act) const {
    const uint16_t& mappedVal = getMappedValueFromAction(act);

    if(mappedVal < 0){
        return {-1, -1};
    }

    return count_table.resolve_primary_index(mappedVal);
}

std::unordered_map<uint32_t, double> KnowledgeBase::exists(const std::pair<const uint32_t, const uint32_t>& indexes, const uint16_t& amount) const {
    std::unordered_map<uint32_t, double> foundElems = std::unordered_map<uint32_t, double>();

    for (auto it = count_table.table.begin() + indexes.second; it != count_table.table.begin() + indexes.first - 1; --it) {
        uint16_t approxConstant = act_table_by_act_id.getTraceLength(it->id.parts.trace_id) / 2;
        double satisfiability = getSatisifiabilityBetweenValues(amount, it->id.parts.event_id, approxConstant);
        foundElems.emplace(it->id.parts.trace_id, satisfiability);
    }

    return foundElems;
}

float KnowledgeBase::getSatisifiabilityBetweenValues(const uint16_t& val1, const uint16_t& val2, const uint16_t& approxConstant) const {
    return 1 / (((float)std::abs(val1 - val2) / approxConstant) + 1);
}

uint16_t KnowledgeBase::getPositionFromEventId(const std::pair<uint32_t, uint16_t> pair) const {
    uint16_t traceLength = act_table_by_act_id.getTraceLength(pair.first);

    /* Guard against length 1 traces */
    if(traceLength == 1){
        return 0;
    }

    uint16_t posFromEventId = std::ceil((float)(pair.second / (float)MAX_UINT16) * (traceLength - 1));
    return posFromEventId;
}

