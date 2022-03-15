//
// Created by Sam on 15/02/2022.
//

#include <knobab/predicates/PredicateManager.h>

#if 0
#include <knobab/KnowledgeBase.h>


bool PredicateManager::checkValidity(const env &e1, const env &e2) const {
    if ((!predicates) || predicates->empty()) return true;
    for(const auto& pred_withConj : *predicates){
        bool result = true;
        for (const auto& pred : pred_withConj) {
            if(!test_decomposed_data_predicate(e1, e2, pred.second.var, pred.second.varRHS, pred.second.casusu)){
                result = false;
                break;
            }
        }
        if (result) return true;
    }
    return false;
}

bool PredicateManager::checkValidity(uint32_t t1, uint16_t e1, const env &e2) const {
    if ((!predicates) || predicates->empty()) return true;
    for(const auto& pred_withConj : *predicates){
        bool result = true;
        for (const auto & pred : pred_withConj) {
            bool test = false;
            auto temp1 = e2.find(pred.second.varRHS);
            if (temp1 == e2.end())
                test = false;
            else {
                auto temp2_a = kb->attribute_name_to_table.find(pred.second.var);
                if (temp2_a != kb->attribute_name_to_table.end()) {
                    size_t offset = kb->act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(t1).at(e1);
                    std::optional<union_minimal> data = temp2_a->second.resolve_record_if_exists2(offset);
                    if (data.has_value()) {
                        auto lhs = data.value();
                        switch (pred.second.casusu) {
                            case LT:
                                test = lhs < temp1->second ; break;
                            case LEQ:
                                test = lhs <= temp1->second; break;
                            case GT:
                                test = lhs > temp1->second; break;
                            case GEQ:
                                test =  lhs >= temp1->second; break;
                            case EQ:
                                test =  lhs == temp1->second; break;
                            case NEQ:
                                test =  lhs != temp1->second; break;
                            case TTRUE:
                                test =  true; break;
                            default:
                                test =  false; break;
                        }
                    } else
                        test = false;
                } else {
                    test = false;
                }
            }
            if(!test){
                result = false;
                break;
            }
        }
        if (result) return true;
    }
    return false;
}

bool PredicateManager::checkValidity(const env &e1, uint32_t t2, uint16_t e2) const {
    if ((!predicates) || predicates->empty()) return true;
    for(const auto& pred_withConj : *predicates){
        bool result = true;
        for (const auto& pred : pred_withConj) {
            bool test = false;
            auto temp1 = e1.find(pred.second.var);
            if (temp1 == e1.end())
                test = false;
            else {
                auto temp2_a = kb->attribute_name_to_table.find(pred.second.varRHS);
                if (temp2_a != kb->attribute_name_to_table.end()) {
                    size_t offset = kb->act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(t2).at(e2);
                    std::optional<union_minimal> data = temp2_a->second.resolve_record_if_exists2(offset);
                    if (data.has_value()) {
                        auto rhs = data.value();
                        switch (pred.second.casusu) {
                            case LT:
                                test = temp1->second < rhs; break;
                            case LEQ:
                                test =  temp1->second <= rhs; break;
                            case GT:
                                test =  temp1->second > rhs; break;
                            case GEQ:
                                test =  temp1->second >= rhs; break;
                            case EQ:
                                test =  temp1->second == rhs; break;
                            case NEQ:
                                test =  temp1->second != rhs; break;
                            case TTRUE:
                                test =  true; break;
                            default:
                                test =  false; break;
                        }
                    } else
                        test = false;
                } else {
                    test = false;
                }
            }
            if(!test){
                result = false;
                break;
            }
        }
        if (result) return true;
    }
    return false;
}


bool PredicateManager::operator==(const PredicateManager &rhs) const {
    return *predicates == *rhs.predicates;
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

    for(const auto& p : kb->attribute_name_to_table){
        size_t offset = kb->act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(pair.first).at(pair.second);
        std::optional<union_minimal> data = p.second.resolve_record_if_exists2(offset);
        if(data.has_value()) {
            environment[p.first] = data.value();
        }
    }

    return environment;
}

env PredicateManager::GetPayloadDataFromEvent(uint32_t first, uint16_t second, bool isLeft, std::unordered_set<std::string>& cache) const {
    env environment;

    if (cache.empty()) {
        for(const auto& pred_withConj : *predicates){
            for (const auto & pred : pred_withConj) {
                cache.insert(isLeft ? pred.second.var : pred.second.varRHS);
            }
        }
    }

    for (const auto& x : cache) {
        auto it = kb->attribute_name_to_table.find(x);
        if (it != kb->attribute_name_to_table.end()) {
            size_t offset = kb->act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(first).at(second);
            std::optional<union_minimal> data = it->second.resolve_record_if_exists2(offset);
            if(data.has_value()) {
                environment[x] = data.value();
            }
        }
    }

    return environment;
}

#endif