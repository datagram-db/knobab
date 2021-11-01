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

template<typename T>  struct any_to_uint_bimap {

    std::vector<T> int_to_T;
    std::unordered_map<T, size_t> T_to_int;


    any_to_uint_bimap() = default;
    any_to_uint_bimap(const any_to_uint_bimap&) = default;
    any_to_uint_bimap(any_to_uint_bimap&&) = default;
    any_to_uint_bimap&operator=(const any_to_uint_bimap&) = default;
    any_to_uint_bimap&operator=(any_to_uint_bimap&&) = default;

    std::pair<size_t, bool> put(const T& elem) {
        auto it = T_to_int.find(elem);
        if (it != T_to_int.end())
            return { it->second, false };
        else {
            size_t toRet = int_to_T.size();
            T_to_int.emplace(elem, toRet);
            int_to_T.emplace_back(elem);
            return { toRet, true };
        }
    }
    size_t get(const T& elem) const {
        auto it = T_to_int.find(elem);
        if (it != T_to_int.end())
            return it->second;
        else {
            throw std::runtime_error("Unkown key");
        }
    }
    const T&  get(size_t elem) const {
        assert(int_to_T.size() > elem);
        return int_to_T.at(elem);
    }
};

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

    bool alreadySet;
    std::string source;
    std::string name;
    size_t noTraces;
    size_t currentEventId;
    ParsingState status;


public:
    any_to_uint_bimap<std::string> event_label_mapper;
    std::unordered_map<size_t, size_t> counting_reference;

    void reconstruct_trace_no_data(std::ostream& os);

    SmallDatabase() : alreadySet{false} {
        status = FinishParsing;
    }
    void index_data_structures() {
        act_table_by_act_id.indexing();
    }
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
