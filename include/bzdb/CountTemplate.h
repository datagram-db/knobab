//
// Created by giacomo on 26/12/20.
//

#ifndef BZDB_COUNTTEMPLATE_H
#define BZDB_COUNTTEMPLATE_H

#include <vector>
#include <bzdb/oid.h>
#include <algorithm>
#include <cassert>


struct CountTemplate {
    std::vector<oid>    table;

    CountTemplate() = default;
    CountTemplate(const CountTemplate&) = default;
    CountTemplate& operator=(const CountTemplate&) = default;

    void emplace_back(uint16_t fact, uint32_t trace_id, uint16_t event_pos);
    void emplace_back(const uint_least64_t& monotone_hash);
    void sort();
    void sanityCheck();
};


#endif //BZDB_COUNTTEMPLATE_H
