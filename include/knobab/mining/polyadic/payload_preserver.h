//
// Created by giacomo on 29/12/24.
//

#ifndef KNOBAB_SERVER_PAYLOAD_PRESERVER_H
#define KNOBAB_SERVER_PAYLOAD_PRESERVER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <knobab/server/dataStructures/oid.h>
#include <yaucl/hashing/pair_hash.h>
#include <yaucl/bpm/structures/commons/DataPredicate.h>
#include <functional>
#include <yaucl/functional/assert.h>
//#include <knobab/server/declare/DeclareDataAware.h>

struct PayloadPreserving;

struct constituent_idx_key {
    PayloadPreserving* ptr;
    size_t offset;
    std::vector<std::pair<std::string,union_minimal>>* payload;
    constituent_idx_key(PayloadPreserving* ptr, size_t offset) : ptr{ptr}, offset{offset}, payload{nullptr} {};
    constituent_idx_key(std::vector<std::pair<std::string,union_minimal>>* payload) : ptr{nullptr}, offset{0}, payload{payload} {};

    std::vector<std::pair<std::string,union_minimal>>* get() const;
    bool operator<(const constituent_idx_key& rhs) const;
};

struct PayloadPreserving {
    using binary_clause = std::tuple<std::string,std::string,std::string>;
    using log_id = std::string;
    using constituent_idx = std::tuple<log_id, trace_t, event_t, constituent_t>;
    using payload_t = std::vector<std::pair<std::string,union_minimal>>;
    using X_offset_t = size_t;
    using M = std::unordered_map<binary_clause,
                                 std::vector<std::unordered_map<constituent_idx,
                                                                std::pair<bool,std::vector<std::pair<event_t, constituent_t>>>>>>;

    std::vector<int>       y;
    M payload_map;

    std::map<constituent_idx_key, size_t> uniqueness_as_ptr;
    std::vector<payload_t> redundantX;
    std::unordered_map<std::tuple<log_id,trace_t,event_t>, std::pair<int, std::unordered_map<constituent_t, X_offset_t>>> classref;
    M emptyM;

    inline bool has_event_clazz(const log_id& log_name, trace_t trace_id, event_t event_id) {
        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
        return classref.contains(key);
    }

    inline void add_clazz(const log_id& log_name, trace_t trace_id, event_t event_id, int clazz) {
        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
        classref[key].first = clazz;
    }

    /**
     * Returns -1 if the payload is not there for the current element, and otherwise the offset in the redundantX
     */
    inline ssize_t has_payload(const log_id& log_name, trace_t trace_id, event_t event_id, constituent_t constituent_id) {
        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
        auto it = classref.find(key);
        if (it == classref.end()) {
          return false;
        }
        auto it2 = it->second.second.find(constituent_id);
        return (it2 == it->second.second.end()) ? -1 : (ssize_t)it2->second;
    }

    inline bool add_payload(const log_id& log_name, trace_t trace_id, event_t event_id, constituent_t constituent_id, payload_t& payload) {
        std::sort(payload.begin(), payload.end());
        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
        constituent_idx_key ptr{&payload};
        auto it = uniqueness_as_ptr.find(ptr);
        size_t offset = redundantX.size();
        if (it == uniqueness_as_ptr.end()) {
            redundantX.emplace_back(payload);
        } else {
            auto& ref = classref[key];
            return ref.second.emplace(constituent_id, offset).second;
        }
        classref[key].second[constituent_id] = offset;
        return true;
    }

    inline void add_activation_without_target(const log_id& log_name, trace_t trace_id, event_t event_id, constituent_t constituent_id, const binary_clause& clause, size_t total_traces_val) {
        auto& clauses = payload_map[clause];
        if (clauses.empty())
            clauses.resize(total_traces_val);
        constituent_idx cons{log_name,trace_id,event_id,constituent_id};
        auto& ref = clauses[trace_id][cons];
        ref.first = true;
        DEBUG_ASSERT(ref.second.empty());
    }

    inline void add_activation_with_target(const log_id& log_name, trace_t trace_id, event_t event_id, constituent_t constituent_id, const binary_clause& clause, size_t total_traces_val, event_t target_event, constituent_t target_constituent) {
        auto& clauses = payload_map[clause];
        if (clauses.empty())
            clauses.resize(total_traces_val);
        constituent_idx cons{log_name,trace_id,event_id,constituent_id};
        auto& local_ref = clauses[trace_id];
        auto it = local_ref.find(cons);
        bool value=false;
#ifdef DEBUG
        bool doTest = local_ref.find(cons) != local_ref.end();
        if (doTest)
            DEBUG_ASSERT(!clauses[trace_id][cons].first);
#endif
        auto& ref = clauses[trace_id][cons];
        ref.first = false;
        ref.second.emplace_back(target_event, target_constituent);
    }


//    std::vector<std::pair<event_t, constituent_t>>& insert_payload(const log_id& log_name, trace_t trace_id, event_t event_id, constituent_t constituent_id, int clazz,
//                        payload_t& payload, FastDatalessClause* clause_of_reference, bool isActivation) {
//
//        // Attempting at inserting the class information, if not provided yet. This is unique, and associated per timestamped event
//        std::tuple<log_id,trace_t,event_t> classref_insert{log_name,trace_id,event_id};
//        constituent_idx cons{log_name,trace_id,event_id,constituent_id};
//        std::tuple<FastDatalessClause*,constituent_idx,activation_otherwise_target> mKey{clause_of_reference,cons,isActivation};
//        classref.emplace(classref_insert, clazz);
//
//        // Sorting the payload according to the increasing key value
//        std::sort(payload.begin(), payload.end());
//        constituent_idx_key ptr{&payload};
//
//        X_offset_t current_offset = X.size();
//        auto it2 = payload_map.find(ptr);
//
//        if (it2 == payload_map.end()) {
//            // If the element does not exist, then inserting actually this element as an actual pointer to X;
//            X.emplace_back(payload);
//            constituent_idx_key actual{this, current_offset};
//            auto it = payload_map.emplace(actual, emptyM);
//            DEBUG_ASSERT(it.second);
//            it2 = it.first;
//        }
//        // Otherwise, this is already contained within the map
//
//        // Returning the map to the targets, just in case we want to fill them in
//        return it2->second[mKey];
//    }
};



#endif //KNOBAB_SERVER_PAYLOAD_PRESERVER_H
