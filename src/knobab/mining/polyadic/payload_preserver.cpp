//
// Created by giacomo on 29/12/24.
//

#include <knobab/server/query_manager/Environment.h>
#include <knobab/mining/polyadic/payload_preserver.h>



std::vector<std::pair<std::string,union_minimal>>* constituent_idx_key::get() const {
    if (payload)
        return payload;
    else
        return &ptr->redundantX.at(offset);
}

bool constituent_idx_key::operator<(const constituent_idx_key& rhs) const {
    return   *get() < *rhs.get();
}

void PayloadPreserving::fill_all_recorded_targets(const std::unordered_map<std::string, Environment>& databases) {
    clear_from_offset(set_step+1);
    std::vector<std::pair<std::string,union_minimal>> cached_record;
    for (const auto& [log_name, records] : non_activation_records_to_collect) {
        const Environment& env = databases.at(log_name);
        const auto& kb = env.db;
        for (const auto& record: records) {
            fill_by_record(log_name, env, cached_record, (ActTable::record*)record);
        }
    }
    non_activation_records_to_collect.clear();
}

void PayloadPreserving::fill_all_activations(const std::unordered_map<std::string, Environment>& databases, const std::string& act_label) {
    std::vector<std::pair<std::string,union_minimal>> cached_record;
    full_clear(); // Ensuring all data is parted away!
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
    set_step = redundantX.size();
}


