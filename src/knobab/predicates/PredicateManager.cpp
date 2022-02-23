//
// Created by Sam on 15/02/2022.
//

#include "knobab/predicates/PredicateManager.h"


bool PredicateManager::checkValidity(const env &e1, const env &e2) const {
    if (predicates.empty()) return true;
    for(const auto& pred_withConj : predicates){
        bool result = true;
        for (const auto& pred : pred_withConj) {
            if(!pred(e1, e2)){
                result = false;
                break;
            }
        }
        if (result) return true;
    }
    return false;
}

bool PredicateManager::operator==(const PredicateManager &rhs) const {
    return predicates == rhs.predicates;
}

bool PredicateManager::operator!=(const PredicateManager &rhs) const {
    return !(rhs == *this);
}

/*bool PredicateManager::checkValidity(const env &e1, const env &e2) const {
    for(const auto& pred : predicates){
        if(!pred(e1, e2)){
            return false;
        }
    }
    return true;
}*/

env PredicateManager::GetPayloadDataFromEvent(const std::pair<uint32_t, uint16_t> &pair) const {
    env environment;

    for(const auto& p : kb->getAttributeNameToTable()){
            size_t offset = kb->act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset[pair.first][pair.second];
            std::optional<union_minimal> data = p.second.resolve_record_if_exists2(offset);

            if(data.has_value()) {
                environment[p.first] = data.value();
            }
    }

    return environment;
}

