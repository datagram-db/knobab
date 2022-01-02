//
// Created by giacomo on 26/12/20.
//

#ifndef BZDB_ATTRIBUTETABLE_H
#define BZDB_ATTRIBUTETABLE_H

#include <string>
#include <knobab/oid.h>
#include <variant>
class SimplifiedFuzzyStringMatching;

enum AttributeTableType {
    DoubleAtt,
    SizeTAtt,
    LongAtt,
    StringAtt,
    BoolAtt
    // TODO: hierarchical types!, https://dl.acm.org/doi/10.1145/3410566.3410583
};

#include <vector>
#include <map>
#include <unordered_map>

struct AttributeTable {
    std::string attr_name;
    SimplifiedFuzzyStringMatching* ptr;
    AttributeTableType type;

    struct record {
        act_t   act;
        size_t  value;
        size_t  act_table_offset;

        record(act_t act, size_t value, size_t actTableOffset);
        record(const record& ) = default;
        record(record&& ) = default;
        record& operator=(const record& ) = default;
        record& operator=(record&& ) = default;
    };
    std::vector<record> table;
    std::unordered_map<size_t, size_t> secondary_index;

    AttributeTable() : attr_name(""), ptr{nullptr}, type{BoolAtt} {}
    AttributeTable(const std::string& attr, AttributeTableType type, SimplifiedFuzzyStringMatching* ptr = nullptr) : attr_name(attr), ptr{ptr}, type{type} {}
    AttributeTable(const AttributeTable& ) = default;
    AttributeTable(AttributeTable&& ) = default;
    AttributeTable& operator=(const AttributeTable& ) = default;
    AttributeTable& operator=(AttributeTable&& ) = default;
    const record * resolve_record_if_exists(size_t actTableOffset) const;
    std::ostream& resolve_and_print(std::ostream& os, const AttributeTable::record &x) const;

    void record_load(act_t act_id, const std::variant<double, size_t, long long, std::string, bool>& val, trace_t tid, event_t eid);
    void index(const std::vector<std::vector<size_t>> &trace_id_to_event_id_to_offset, size_t maxOffset);

private:
    std::vector<std::map<std::variant<double, size_t, long long, std::string, bool>, std::vector<std::pair<trace_t, event_t>>>> elements;
    std::variant<double, size_t, long long, std::string, bool> resolve(const record& x);
    size_t storeLoad(const std::variant<double, size_t, long long, std::string, bool> &x);
    bool assertVariant(const std::variant<double, size_t, long long, std::string, bool>& val);
};


#endif //BZDB_ATTRIBUTETABLE_H
