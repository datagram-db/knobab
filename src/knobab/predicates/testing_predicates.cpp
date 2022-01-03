//
// Created by giacomo on 03/01/2022.
//

#include <knobab/predicates/testing_predicates.h>

bool test_data_predicate(const DataPredicate &dp, const union_type &val) {
    if (std::holds_alternative<std::string>(val)) {
        return dp.testOverSingleVariable(std::get<std::string>(val));
    } else {
        double db;
        if (std::holds_alternative<double>(val))
            db = std::get<double>(val);
        else if (std::holds_alternative<size_t>(val))
            db = std::get<size_t>(val);
        else if (std::holds_alternative<long long>(val))
            db = std::get<long long>(val);
        else if (std::holds_alternative<bool>(val))
            db = std::get<bool>(val) ? 1.0 : 0.0;
        else
            throw std::runtime_error(std::to_string(val.index())+" Unexpected index type");
        return dp.testOverSingleVariable(db);
    }
}

bool test_data_predicate(const DataPredicate &dp, const union_minimal &val) {
    if (std::holds_alternative<std::string>(val)) {
        return dp.testOverSingleVariable(std::get<std::string>(val));
    } else {
        if (std::holds_alternative<double>(val))
            return dp.testOverSingleVariable(std::get<double>(val));
        else
            throw std::runtime_error(std::to_string(val.index())+" Unexpected index type");
    }
}

bool test_conjunctive_predicate(KnowledgeBase &db, ActTable::record *eventFromTrace,
                                const std::string &expectedActFromDeclare,
                                const std::unordered_map<std::string, DataPredicate> &dp,
                                bool testSingleVariable ,
                                bool testBiVariables ) {
    assert(testSingleVariable || testBiVariables);
    if (!eventFromTrace)
        return false; // I can assess neither the label, nor the map: therefore, I cannot say it is true. More likely, this is an error, so false!

    // label associated to the current event from the trace
    std::string sAct = db.event_label_mapper.get(eventFromTrace->entry.id.parts.act);

    // If the label do not match, the predicate is vacuously true
    if (sAct != expectedActFromDeclare) return true;

    // Otherwise, I need to iterate over all of the possible variables/attributes in it
    auto cp = db.getAttrNameTableIt();
    std::unordered_map<std::string, union_minimal> eventObject;
    std::unordered_map<std::string, std::vector<const std::vector<DataPredicate>*>> eventObjectPredicatesToAssess;

    while (cp.first != cp.second) {
        std::string attributeOrVar = cp.first->first;
        union_minimal value = db.resolveMinimalRecord(eventFromTrace, cp.first);
        auto it = dp.find(attributeOrVar);
        if (it != dp.end()) {
            // If the current element does not satisfy the present element of the conjunction, then all of the other
            // elements will be false, too: so, I can quit the iteration
            if (testSingleVariable)
                if (!test_data_predicate(it->second, value)) return false;
            eventObject[attributeOrVar] = value;
            if (testBiVariables && (!it->second.BiVariableConditions.empty()))
                eventObjectPredicatesToAssess[attributeOrVar].emplace_back(&it->second.BiVariableConditions);
        }  // otherwise, it resolves to true, and so I need to check the remaining elements
        cp.first++;
    }

    for (const auto& varNameToVectorPtr : eventObjectPredicatesToAssess) {
        for (const auto dpV : varNameToVectorPtr.second) {
            for (const auto dptr : *dpV) {
                auto itL = eventObject.find(dptr.var);
                if (itL == eventObject.end())
                    return false; // if the value is missing, is like null, and therefore, no comparison holds
                auto itR = eventObject.find(dptr.varRHS);
                if (itR == eventObject.end())
                    return false; // same as above;

                // Create a copy of the former predicate
                DataPredicate dp{dptr};
                // From the copy, remove the reference to the variable, so to replace it with its associated value
                dp.varRHS.clear();
                // Associating the right's variable value to the element
                dp.value = itR->second;
                // if the data does not jointly satisfy this predicate, then return the falsehood
                if (!test_data_predicate(dp, itL->second)) return false;
            }
        }
    }

    return true;
}

bool test_disjunctive_predicate(KnowledgeBase &db, ActTable::record *eventFromTrace,
                                const std::string &expectedActFromDeclare,
                                const std::vector<std::unordered_map<std::string, DataPredicate>> &dnf,
                                bool testSingleVariable ,
                                bool testBiVariables ) {
    if (!eventFromTrace)
        return false; // I can assess neither the label, nor the map: therefore, I cannot say it is true. More likely, this is an error, so false!

    // label associated to the current event from the trace
    std::string sAct = db.event_label_mapper.get(eventFromTrace->entry.id.parts.act);

    // If the label do not match, the predicate is vacuously true
    if (sAct != expectedActFromDeclare) return true;

    for (const auto& Map : dnf) {
        // If at least one element is satisfied by the disjunction, everything is satisfied!
        if (test_conjunctive_predicate(db, eventFromTrace, expectedActFromDeclare, Map, testSingleVariable, testBiVariables)) return true;
    }
    // If no element from the disjunction satisfied the predicate, then everything is globally unsatisfied
    return false;
}

void
filter_some_values_from_disjunctive_map(const std::vector<std::unordered_map<std::string, DataPredicate>> &theMapToScan,
                                        std::unordered_map<std::string, std::set<union_type>> &theMapToFilter) {
    for (const auto& Map : theMapToScan) {
        for (const auto& varPred : Map) {
            auto it = theMapToFilter.find(varPred.first);
            if (it != theMapToFilter.end()) {
                for (auto it2 = it->second.begin(); it2 != it->second.end(); ) {
                    if (!test_data_predicate(varPred.second, *it2)) {
                        it2 = it->second.erase(it2);
                    } else {
                        it2++;
                    }
                }
            }
        }
    }
}
