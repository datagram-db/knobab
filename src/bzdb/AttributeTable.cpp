//
// Created by giacomo on 26/12/20.
//

#include "bzdb/AttributeTable.h"
#include "SimplifiedFuzzyStringMatching.h"
#include <cassert>

std::variant<double, size_t, long long, std::string, bool> AttributeTable::resolve(const AttributeTable::record &x) {
    switch (type) {
        case DoubleAtt:
            return *(double*)(&x.value);
        case LongAtt:
            return *(long long*)(&x.value);
        case StringAtt:
            assert(ptr);
            return ptr->get(x.value);
        case BoolAtt:
            return x.value != 0;
        //case SizeTAtt:
        default:
            // TODO: hierarchical types!, https://dl.acm.org/doi/10.1145/3410566.3410583
            return x.value;
    }
}

bool AttributeTable::assertVariant(const std::variant<double, size_t, long long int, std::string, bool> &val) {
    switch (type) {
        case DoubleAtt:
            return std::holds_alternative<double>(val);
        case SizeTAtt:
            return std::holds_alternative<size_t>(val);
        case LongAtt:
            return std::holds_alternative<long long>(val);
        case StringAtt:
            return std::holds_alternative<std::string>(val);
        case BoolAtt:
            return std::holds_alternative<bool>(val);
        default:
            return false;
    }
}

void
AttributeTable::record_load(act_t act_id, const std::variant<double, size_t, long long int, std::string, bool> &val,
                            trace_t tid, event_t eid) {
    assert(assertVariant(val));
    while (elements.size() <= act_id)
        elements.emplace_back();
    elements[act_id][val].emplace_back(tid, eid);
}

size_t AttributeTable::storeLoad(const std::variant<double, size_t, long long int, std::string, bool> &x) {
    switch (type) {
        case DoubleAtt: {
            double tmp = std::get<double>(x);
            return *(size_t*)(&tmp);
        }

        case LongAtt: {
            long long tmp = std::get<long long>(x);
            return *(size_t*)(&tmp);
        }

        case StringAtt: {
            std::string tmp = std::get<std::string>(x);
            return ptr->put(tmp).first;
        }
        case BoolAtt:
            return std::get<bool>(x) ? 1 : 0;
            //case SizeTAtt:
        default:
            // TODO: hierarchical types!, https://dl.acm.org/doi/10.1145/3410566.3410583
            return std::get<size_t>(x);
    }
}

void AttributeTable::index(const std::vector<std::vector<size_t>> &trace_id_to_event_id_to_offset, size_t maxOffset) {
    for (size_t act_id = 0, N = elements.size(); act_id < N; act_id++) {
        auto& ref = elements[act_id];
        if (!ref.empty()) {
            for (const auto& val_offset : ref) {
                size_t val = storeLoad(val_offset.first);
                for (const auto& traceid_eventid : val_offset.second) {
                    size_t offset =
                            trace_id_to_event_id_to_offset.at(traceid_eventid.first).at(traceid_eventid.second);
                    secondary_index[offset] = table.size();
                    table.emplace_back(act_id, val, offset);
                }
            }
            ref.clear();
        }
    }
    elements.clear();
}

const AttributeTable::record * AttributeTable::resolve_record_if_exists(size_t actTableOffset) const {
    auto it = secondary_index.find(actTableOffset);
    if (it == secondary_index.end())
        return nullptr;
    else
        return (table.data()) + it->second;
}

std::ostream &AttributeTable::resolve_and_print(std::ostream &os, const AttributeTable::record &x) const {
    switch (type) {
        case DoubleAtt:
            return os << *(double*)(&x.value);
        case LongAtt:
            return os << *(long long*)(&x.value);
        case StringAtt:
            assert(ptr);
            return os << ptr->get(x.value);
        case BoolAtt:
            return os << ((x.value != 0) ? "true" : "false");
            //case SizeTAtt:
        default:
            // TODO: hierarchical types!, https://dl.acm.org/doi/10.1145/3410566.3410583
            return os << x.value;
    }
}

AttributeTable::record::record(act_t act, size_t value, size_t actTableOffset) : act(act), value(value),
                                                                                 act_table_offset(actTableOffset) {}
