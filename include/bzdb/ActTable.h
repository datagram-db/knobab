//
// Created by giacomo on 26/12/20.
//

#ifndef BZDB_ACTTABLE_H
#define BZDB_ACTTABLE_H


#include <bzdb/oid.h>
#include <tuple>
#include <vector>

uint16_t cast_to_float(size_t x, size_t l);

/**
 * TODO: use a graph to store the prev/next information
 *        look up at the original setting, too
 *  then, populate using emplace_back
 */

struct ActTable {

    struct record {
        oid             entry;
        struct record*  prev;
        struct record*  next;

        record();
        record(trace_t id, time_t time, act_t act, struct record* prev, struct record* next);
        record(const record& ) = default;
        record& operator=(const record&) = default;

        bool operator<(const record &rhs) const;
        bool operator>(const record &rhs) const;
        bool operator<=(const record &rhs) const;
        bool operator>=(const record &rhs) const;
        bool operator==(const record &rhs) const;
        bool operator!=(const record &rhs) const;
    };

    std::vector<record> table;
    std::vector<size_t> primary_index;
    std::vector<std::pair<record*, record*>> secondary_index;

    record* load_record(trace_t id, act_t act, time_t time, struct record* prev = nullptr, struct record* next = nullptr); // rename: loading_step (emplace_back)
    void indexing();
    void sanityCheck();

    size_t getTraceLength(size_t id) const { return trace_length.at(id); }

private:
    std::vector<std::tuple<trace_t, event_t, size_t>> expectedOrdering; // TODO: remove?
    std::vector<size_t> trace_length; // L1

    struct table_builder {
        std::vector<std::vector<std::pair<trace_t, event_t>>> act_id_to_trace_id_and_time; // M1
        std::vector<std::vector<size_t>> trace_id_to_event_id_to_offset; // M2
    };

    table_builder builder;

};


#endif //BZDB_ACTTABLE_H
