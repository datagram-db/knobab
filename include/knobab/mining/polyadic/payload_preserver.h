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
#include <yaucl/structures/set_operations.h>
#include <knobab/server/query_manager/Environment.h>


using simple_declare = std::pair<std::string, bool>; // originally: defined within polyadic_bolt.h

struct Environment;
struct payload_preserving;

struct constituent_idx_key {
    payload_preserving* ptr;
    size_t offset;
    std::vector<std::pair<std::string,union_minimal>>* payload;
    constituent_idx_key(payload_preserving* ptr, size_t offset) : ptr{ptr}, offset{offset}, payload{nullptr} {};
    constituent_idx_key(std::vector<std::pair<std::string,union_minimal>>* payload) : ptr{nullptr}, offset{0}, payload{payload} {};

    std::vector<std::pair<std::string,union_minimal>>* get() const;
    bool operator<(const constituent_idx_key& rhs) const;
};

using constituent_t = ActTable::record*;

/**
 * Differentiating between all the possible activation scenarios for the elements
 */
enum ActivationCases {
    ActivationIsViolated,
    SATActivationWithNoTarget,
    SATActivationWithTarget,
    GenericSATActivation,
    Vacuity
};

/**
 * Class used to remember the correlations between activation and target across clauses being considered
 */
struct act_target_correlation_preserver {
    using binary_clause = simple_declare;
    using log_id = std::string;
    using M = std::unordered_map<binary_clause,
            std::unordered_map<std::string,std::vector<std::unordered_map<constituent_t,
                    std::unordered_map<ActivationCases,std::unordered_set</*std::pair<event_t, constituent_t>*/constituent_t>>>>>>
                    ;
    M payload_map;
    std::unordered_set<constituent_t> unique_targets;

    void clear() {
        payload_map.clear();
        unique_targets.clear();
    }

    inline void add_activation_without_target(const log_id& log_name, /*trace_t trace_id, event_t event_id,*/ constituent_t constituent_id, const binary_clause& clause, size_t total_traces_val) {
        auto& clauses = payload_map[clause][log_name];
        if (clauses.size() != total_traces_val)
            clauses.resize(total_traces_val);
        auto& trace = clauses[constituent_id->entry.id.parts.trace_id];
        auto& ref = trace[constituent_id][SATActivationWithNoTarget];
        DEBUG_ASSERT(ref.empty());
    }

    inline void add_activation_with_target_violation(const log_id& log_name, /*trace_t trace_id, event_t event_id,*/ constituent_t constituent_id, const binary_clause& clause, size_t total_traces_val) {
        auto& clauses = payload_map[clause][log_name];
        if (clauses.size() != total_traces_val)
            clauses.resize(total_traces_val);
        auto& trace = clauses[constituent_id->entry.id.parts.trace_id];
        auto& ref = trace[constituent_id][ActivationIsViolated];
        DEBUG_ASSERT(ref.empty());
    }

    inline void add_activation_with_target(const log_id& log_name, /*trace_t trace_id, event_t event_id,*/ constituent_t constituent_id, const binary_clause& clause, size_t total_traces_val, /*event_t target_event,*/ constituent_t target_constituent) {
        auto& clauses = payload_map[clause][log_name];
        if (clauses.size() != total_traces_val)
            clauses.resize(total_traces_val);
        auto& trace = clauses[constituent_id->entry.id.parts.trace_id];
        auto& local_ref = trace[constituent_id][SATActivationWithTarget];
        local_ref.emplace(target_constituent);
        unique_targets.emplace(target_constituent);
    }
};

enum fill_in_policy {
    StraightforwardFillIn, // With this policy, I consider straightforward fill-in, not considering values associated with different classes
    DistinguishingPayload //                    I only consider payloads differing
};

/**
 * This preserves the payload across all the logs
 */
struct payload_preserving {
    using X_offset_t = size_t;
    using log_id = std::string;
    using payload_t = std::vector<std::pair<std::string,union_minimal>>;
    std::map<constituent_idx_key, std::unordered_map<int,std::unordered_set<constituent_t>>> uniqueness_as_ptr;
    std::vector<payload_t> redundantX;
    std::unordered_map<std::tuple<log_id,trace_t,event_t>, std::unordered_map<constituent_t, X_offset_t>> classref;

    inline void clear() {
        uniqueness_as_ptr.clear();
        redundantX.clear();
        classref.clear();
    }

    inline const std::unordered_map<int,std::unordered_set<constituent_t>>& get_correlations_from_offset(const log_id& log, constituent_t object) const {
        std::tuple<log_id,trace_t,event_t> idx{log, (trace_t)object->entry.id.parts.trace_id, (event_t)object->entry.id.parts.event_id};
        auto it = classref.find(idx);
        if (it == classref.end())
            return empty_result;
        auto it2 = it->second.find(object);
        if (it2 == it->second.end())
            return empty_result;
        return get_correlations_from_offset(it2->second);
    }
    inline const payload_t * get_payloads_from_offset(const log_id& log, constituent_t object) const {
        std::tuple<log_id,trace_t,event_t> idx{log, (trace_t)object->entry.id.parts.trace_id, (event_t)object->entry.id.parts.event_id};
        auto it = classref.find(idx);
        if (it == classref.end())
            return &empty_payload;
        auto it2 = it->second.find(object);
        if (it2 == it->second.end())
            return &empty_payload;
        return get_payloads_from_offset(it2->second);
    }

    /**
     * Returning the mapping between the classes and the knobab records associated to the payload for each offset associated to the current payload object
     * @param object    redundantX offset to be retrieved from redundantXCorrespondences
     * @return          An empty map if there is no correspondence, otherwise some correpsondence containing at least one class and one record
     */
    inline const std::unordered_map<int,std::unordered_set<constituent_t>>& get_correlations_from_offset(size_t object) const {
        constituent_idx_key obj{(payload_preserving*)this, object};
        auto it = uniqueness_as_ptr.find(obj);
        if (it == uniqueness_as_ptr.end())
            return empty_result;
        else
            return it->second;
    }
    inline const payload_t * get_payloads_from_offset(size_t object) const {
        constituent_idx_key obj{(payload_preserving*)this, object};
        auto it = uniqueness_as_ptr.find(obj);
        if (it == uniqueness_as_ptr.end())
            return &empty_payload;
        else
            return (const payload_t *)it->first.get();
    }

    /**
     * Loading a linear representation for the classification
     * @param X                             Destination payloads to be loaded
     * @param y                             Destination classes to be considered
     * @param redundantXCorrespondences     The pointer to the current
     * @param casus
     * @param sampling_probability
     */
    void load_activation_with_policy(const act_target_correlation_preserver::binary_clause& clause,
                          const act_target_correlation_preserver::M& map,
                          std::vector<payload_t>& X,
                          std::vector<int>& y,
                          std::vector<size_t> redundantXCorrespondences,
                          fill_in_policy casus,
                          double sampling_probability= 1.0) {
        X.clear();
        y.clear();
        std::mt19937 engine;
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        auto it = map.find(clause);
        if (it == map.end())
            return;

        for (const auto& [log_name, all_traces] : it->second) {
            for (const auto& trace : all_traces) {
                for (const auto& [key, offset_with_clazz] : uniqueness_as_ptr) {
                    size_t nOfClazzes = offset_with_clazz.size();
                    const auto* payload = key.get();
                    if ((nOfClazzes == 1) || (casus == StraightforwardFillIn)) {
                        for (const auto&  [clazz, offsets] : offset_with_clazz) {
                            bool found = false;
                            for (auto* ptr : offsets) {
                                if (trace.contains(ptr))  {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) continue; // Inserting the element only if one of the recorts appears among the activations

                            // Inserting one payload speciment per class, so not to bias the classification
                            if (dist(engine) <= sampling_probability) {
                                X.emplace_back(*payload);
                                y.emplace_back(clazz);
                                redundantXCorrespondences.emplace_back(key.offset);
                            }
                        }
                    }
                }
            }
        }
    }

    /**
 * Loading a linear representation for the classification
 * @param X                             Destination payloads to be loaded
 * @param y                             Destination classes to be considered
 * @param redundantXCorrespondences     The pointer to the current
 * @param casus
 * @param sampling_probability
 */
    void load_target_with_policy(const act_target_correlation_preserver::binary_clause& clause,
                              const act_target_correlation_preserver::M& map,
                              std::vector<payload_t>& X,
                              std::vector<int>& y,
                              std::vector<size_t> redundantXCorrespondences,
                              fill_in_policy casus,
                              double sampling_probability= 1.0) {
        X.clear();
        y.clear();
        std::mt19937 engine;
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        auto it = map.find(clause);
        if (it == map.end())
            return;

        for (const auto& [log_name, all_traces] : it->second) {
            for (const auto &trace: all_traces) {
                for (const auto& [key, offset_with_clazz] : uniqueness_as_ptr) {
                    size_t nOfClazzes = offset_with_clazz.size();
                    const auto* payload = key.get();
                    if ((nOfClazzes == 1) || (casus == StraightforwardFillIn)) {
                        for (const auto&  [clazz, offsets] : offset_with_clazz) {
                            bool found = false;
                            // Looding for the value among the targets
                            for (auto* ptr : offsets) {
                                for (const auto& [key2, vals2] : trace) {
                                    for (const auto& [casus2, set] : vals2) {
                                        if (set.contains(ptr)) {
                                            found = true;
                                            break;
                                        }
                                    }
                                    if (found) break;
                                }
                                if (found) break;
                            }
                            if (!found) continue; // Inserting the element only if one of the recorts appears among the activations

                            // Inserting one payload speciment per class, so not to bias the classification
                            if (dist(engine) <= sampling_probability) {
                                X.emplace_back(*payload);
                                y.emplace_back(clazz);
                                redundantXCorrespondences.emplace_back(key.offset);
                            }
                        }
                    }
                }
            }
        }
    }

    // TODO: correlation condition between activation and target

    /**
 * Returns -1 if the payload is not there for the current element, and otherwise the offset in the redundantX
 * assumption: needs to add clazz before payload
 */
    inline ssize_t has_payload(const log_id& log_name, /*trace_t trace_id, event_t event_id,*/ constituent_t constituent_id) {
        const auto& trace_id = constituent_id->entry.id.parts.trace_id;
        const auto& event_id = constituent_id->entry.id.parts.event_id;
        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
        auto it = classref.find(key);
        if (it == classref.end()) {
            return -1;
        }
        auto it2 = it->second.find(constituent_id);
        return (it2 == it->second.end()) ? -1 : (ssize_t)it2->second;
    }

    inline bool add_payload(const log_id& log_name, /*trace_t trace_id, event_t event_id,*/ constituent_t constituent_id, payload_t& payload) {
        std::sort(payload.begin(), payload.end());
        int clazz = std::stoi(log_name);
        const auto& trace_id = constituent_id->entry.id.parts.trace_id;
        const auto& event_id = constituent_id->entry.id.parts.event_id;
        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
        constituent_idx_key ptr{&payload};
        auto it = uniqueness_as_ptr.find(ptr);
        size_t offset = redundantX.size();
        if (it == uniqueness_as_ptr.end()) {
            redundantX.emplace_back(payload);
            constituent_idx_key vals{this, offset};
            uniqueness_as_ptr[vals][clazz].emplace(constituent_id);
            classref[key][constituent_id] = offset;
            return true;
        } else {
            offset = it->first.offset;
            auto& ref = classref[key];
            return ref.emplace(constituent_id, offset).second;
        }
    }

    inline void fill_by_record(const std::string& log_name, const Environment& env, std::vector<std::pair<std::string,union_minimal>>& cached_record, ActTable::record* record) {
        const auto& act_id = record->entry.id.parts.act;
        if (this->has_payload(log_name, /*trace_id, event_id,*/ record) == -1) {
            size_t offset = record - env.db.act_table_by_act_id.table.data();
//            DEBUG_ASSERT(act_id == found);
            cached_record.clear();
            for (const auto& [key, table] : env.db.attribute_name_to_table) {
                table.resolve_record_if_exists3(offset, cached_record);
//                                table.resolve_record_if_exists2(offset, tuple);
            }
            this->add_payload(log_name, /*trace_id, event_id,*/ record, cached_record);
        }
    }

    void fill_all_activations(const std::unordered_map<std::string, Environment>& databases, const std::string& act_label) {
        std::vector<std::pair<std::string,union_minimal>> cached_record;
        for (const auto& [log_name, env ] : databases) {
            const auto& kb = env.db;
            // MEMENTO: auto class_id = std::stoi(log_name); !!!
            ssize_t found = kb.event_label_mapper.signed_get(act_label);
            DEBUG_ASSERT(found != -1);
            auto it = kb.timed_dataless_exists(found);
            DEBUG_ASSERT(it.first != it.second);
            while (it.first != it.second) {
                fill_by_record(log_name, env, cached_record, (ActTable::record*)it.first);
                it.first++;
            }
        }
    }

private:
    std::unordered_map<int,std::unordered_set<constituent_t>> empty_result;
    payload_t empty_payload;
};

//struct PayloadPreserving {
//    std::string current_log_being_assessed;
//    using binary_clause = simple_declare;
//    using log_id = std::string;
//    using constituent_idx = std::tuple<log_id, trace_t, event_t, constituent_t>;
//    using payload_t = std::vector<std::pair<std::string,union_minimal>>;
//    using X_offset_t = size_t;
//    using M = std::unordered_map<binary_clause,
//                                 std::vector<std::unordered_map<constituent_idx,
//                                                                std::pair<bool,std::vector</*std::pair<event_t, constituent_t>*/constituent_t>>>>>;
//
////    std::vector<int>       y;
//    M payload_map;
//
//    std::unordered_map<std::string, std::unordered_set<ActTable::record*>> non_activation_records_to_collect;
//    std::map<constituent_idx_key, size_t> uniqueness_as_ptr;
//    std::vector<payload_t> redundantX;
//    size_t set_step{0};
//    std::unordered_map<std::tuple<log_id,trace_t,event_t>, std::unordered_map<constituent_t, X_offset_t>> classref;
////    M emptyM;
//
//    /// clearing methods
//
//    bool clear_from_offset(/* size_t idx */) {
//        /**
//         * Clearing the payloads up until a given size assumption. This works under the assumption that all the payloads
//         *
//         */
//        size_t idx = set_step;
//        DEBUG_ASSERT(set_step > 0);
//        if (redundantX.size() <= idx)
//            return false;
//
//        redundantX.resize(idx);
//        constituent_idx const_;
//        for (auto it = uniqueness_as_ptr.begin(); it != uniqueness_as_ptr.end(); ) {
//            if (it->first.offset != it->second)
//                return false;
//            if (it->first.offset >= idx)
//                it = uniqueness_as_ptr.erase(it);
//            else
//                it++;
//        }
//        for (const auto& [key, clazz_constituents] : classref) {
//            std::get<0>(const_) = std::get<0>(key);
//            std::get<1>(const_) = std::get<1>(key);
//            std::get<2>(const_) = std::get<2>(key);
//            for (const auto& [constituent, offset] : (clazz_constituents)) {
//                if (offset <= idx) {
//                    std::get<3>(const_) = constituent;
//                    for (auto& [bc, vals] : payload_map) {
//                        for (auto& val : vals) {
//                            val.erase(const_);
//                            for (auto it = val.begin(); it != val.end(); ){
//                                std::vector<size_t> idxToRemove;
//                                for (size_t idx2 = 0, N = it->second.second.size(); idx2 <N; idx2++) {
//                                    auto& ref2 = it->second.second.at(idx2);
//                                    if (/*(ref2.first == std::get<2>(const_)) &&*/ (ref2/*.second*/ == constituent))
//                                        idxToRemove.emplace_back(idx2);
//                                }
//                                if (!idxToRemove.empty())
//                                    remove_index(it->second.second, idxToRemove);
//                                if (it->second.second.empty())
//                                    it = val.erase(it);
//                                else
//                                    it++;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//        return true;
//    }
//
//    /**
//     * Fully clearing all the hosted elements.
//     * To be called when we are sure
//     */
//    inline void full_clear() {
////        y.clear();
////        payload_map.clear();
//        uniqueness_as_ptr.clear();
//        redundantX.clear();
//        classref.clear();
////        payload_map.clear();
//        set_step = 0;
//    }
//
//    /// Global initialization methods. This assumes a full clearance to begin with
//    void fill_all_activations(const std::unordered_map<std::string, Environment>& databases, const std::string& act_label);
//    void fill_all_recorded_targets(const std::unordered_map<std::string, Environment>& databases);
//
//    inline void fill_by_record(const std::string& log_name, const Environment& env, std::vector<std::pair<std::string,union_minimal>>& cached_record, ActTable::record* record) {
//        const auto& act_id = record->entry.id.parts.act;
//        if (this->has_payload(log_name, /*trace_id, event_id,*/ record) == -1) {
//            size_t offset = record - env.db.act_table_by_act_id.table.data();
////            DEBUG_ASSERT(act_id == found);
//            cached_record.clear();
//            for (const auto& [key, table] : env.db.attribute_name_to_table) {
//                table.resolve_record_if_exists3(offset, cached_record);
////                                table.resolve_record_if_exists2(offset, tuple);
//            }
//            this->add_payload(log_name, /*trace_id, event_id,*/ record, cached_record);
//        }
//    }
//
//    /// Initialization methods: those are appearing in order of assumed invocation
//
//    // Under the data loading assumption, the log id itself represents the class id, so, there is no reason to store this explicitly
////    inline bool has_event_clazz(const log_id& log_name, trace_t trace_id, event_t event_id) {
////        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
////        return classref.contains(key);
////    }
////
////    inline void add_clazz(const log_id& log_name, trace_t trace_id, event_t event_id, int clazz) {
////        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
////        classref[key].first = clazz;
////    }
//
//    /**
//     * Returns -1 if the payload is not there for the current element, and otherwise the offset in the redundantX
//     * assumption: needs to add clazz before payload
//     */
//    inline ssize_t has_payload(const log_id& log_name, /*trace_t trace_id, event_t event_id,*/ constituent_t constituent_id) {
//        const auto& trace_id = constituent_id->entry.id.parts.trace_id;
//        const auto& event_id = constituent_id->entry.id.parts.event_id;
//        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
//        auto it = classref.find(key);
//        if (it == classref.end()) {
//          return -1;
//        }
//        auto it2 = it->second.find(constituent_id);
//        return (it2 == it->second.end()) ? -1 : (ssize_t)it2->second;
//    }
//
//    inline bool add_payload(const log_id& log_name, /*trace_t trace_id, event_t event_id,*/ constituent_t constituent_id, payload_t& payload) {
//        std::sort(payload.begin(), payload.end());
//        const auto& trace_id = constituent_id->entry.id.parts.trace_id;
//        const auto& event_id = constituent_id->entry.id.parts.event_id;
//        std::tuple<log_id,trace_t,event_t> key{log_name,trace_id,event_id};
//        constituent_idx_key ptr{&payload};
//        auto it = uniqueness_as_ptr.find(ptr);
//        size_t offset = redundantX.size();
//        if (it == uniqueness_as_ptr.end()) {
//            redundantX.emplace_back(payload);
//            constituent_idx_key vals{this, offset};
//            uniqueness_as_ptr[vals] = offset;
//            classref[key][constituent_id] = offset;
//            return true;
//        } else {
//            auto& ref = classref[key];
//            return ref.emplace(constituent_id, offset).second;
//        }
//    }
//
////    inline void add_activation_without_target(/*const log_id& log_name, trace_t trace_id, event_t event_id,*/ constituent_t constituent_id, const binary_clause& clause, size_t total_traces_val) {
////        DEBUG_ASSERT(!current_log_being_assessed.empty());
////        const auto& trace_id = constituent_id->entry.id.parts.trace_id;
////        const auto& event_id = constituent_id->entry.id.parts.event_id;
////        auto& clauses = payload_map[clause];
////        if (clauses.empty())
////            clauses.resize(total_traces_val);
////        constituent_idx cons{current_log_being_assessed,trace_id,event_id,constituent_id};
////        auto& ref = clauses[trace_id][cons];
////        ref.first = true;
////        DEBUG_ASSERT(ref.second.empty());
////    }
////
////    inline void add_activation_with_target(/*const log_id& log_name, trace_t trace_id, event_t event_id,*/ constituent_t constituent_id, const binary_clause& clause, size_t total_traces_val, /*event_t target_event,*/ constituent_t target_constituent) {
////        DEBUG_ASSERT(!current_log_being_assessed.empty());
////        const auto& trace_id = constituent_id->entry.id.parts.trace_id;
////        const auto& event_id = constituent_id->entry.id.parts.event_id;
////        auto& clauses = payload_map[clause];
////        if (clauses.empty())
////            clauses.resize(total_traces_val);
////        constituent_idx cons{current_log_being_assessed,trace_id,event_id,constituent_id};
////        auto& local_ref = clauses[trace_id];
////        auto it = local_ref.find(cons);
////        bool value=false;
////#ifdef DEBUG
////        bool doTest = local_ref.find(cons) != local_ref.end();
////        if (doTest)
////            DEBUG_ASSERT(!clauses[trace_id][cons].first);
////#endif
////        auto& ref = clauses[trace_id][cons];
////        ref.first = false;
////        ref.second.emplace_back(/*target_event,*/ target_constituent);
////        non_activation_records_to_collect[current_log_being_assessed].emplace(target_constituent);
////    }
//
//
//
//};
//
//

#endif //KNOBAB_SERVER_PAYLOAD_PRESERVER_H
