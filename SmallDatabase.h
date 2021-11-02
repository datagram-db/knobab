//
// Created by giacomo on 26/12/20.
//

#ifndef BZDB_SMALLDATABASE_H
#define BZDB_SMALLDATABASE_H

#include <vector>
#include <string>
#include <bzdb/CountTemplate.h>
#include <bzdb/ActTable.h>
#include <unordered_map>
#include <bzdb/AttributeTable.h>
#include <yaucl/bpm/trace_visitor.h>
#include <yaucl/structures/any_to_uint_bimap.h>
#include <SimplifiedFuzzyStringMatching.h>

enum ParsingState {
    LogParsing,
    TraceParsing,
    EventParsing,
    FinishParsing
};

class SmallDatabase : public trace_visitor {
    CountTemplate                                   count_table;
    ActTable                                        act_table_by_act_id;
    std::unordered_map<std::string, AttributeTable> attribute_name_to_table;
    SimplifiedFuzzyStringMatching                   string_values;

    bool alreadySet;
    std::string source;
    std::string name;
    size_t noTraces;
    size_t currentEventId;
    ParsingState status;
    size_t actId;

public:
    yaucl::structures::any_to_uint_bimap<std::string> event_label_mapper;
    std::unordered_map<size_t, size_t> counting_reference;

    void reconstruct_trace_no_data(std::ostream& os);
    void reconstruct_trace_with_data(std::ostream& os);

    SmallDatabase();
    void index_data_structures();
    void enterLog(const std::string &source, const std::string &name) override;
    void exitLog(const std::string &source, const std::string &name) override;
    size_t enterTrace(const std::string &trace_label) override;
    void exitTrace(size_t traceId) override;
    size_t enterEvent(size_t chronos_tick, const std::string &event_label) override;
    void exitEvent(size_t event_id) override;
    void enterData_part(bool isEvent) override;
    void exitData_part(bool isEvent) override;
    void visitField(const std::string &key, bool value) override;
    void visitField(const std::string &key, double value) override;
    void visitField(const std::string &key, const std::string &value) override;
    void visitField(const std::string &key, size_t value) override;
};


#endif //BZDB_SMALLDATABASE_H
