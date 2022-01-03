//
// Created by giacomo on 03/01/2022.
//

#include <cassert>
#include <yaucl/bpm/algos/transformations/grounding.h>
#include <yaucl/bpm/structures/commons/testing_predicates.h>
#include <iostream>

inline bool recursively_run_instantiation(const DataPredicate &toGround,
                                          const std::unordered_map<std::string, union_minimal> &map,
                                          std::vector<DataPredicate>& allPredicates,
                                          std::unordered_map<std::string, std::vector<DataPredicate>>& toIntersectAtAnUpperLevel
                                          ) {
    for (const auto& ref : toGround.BiVariableConditions) {
        auto tmp = instantiateWithValues(ref, map, toIntersectAtAnUpperLevel);
        if (tmp.isInvalid()) {
            return false; // if an internal instance is unsatisfied, everything becomes falsified, as in conjunction.
        } else if (tmp.isValidButPresent()) {
            allPredicates.emplace_back(tmp.get());
            // if the predicate is valid but not present, it would very probably already part in toIntersectAtAnUpperLevel
        }
    }
    return true;
}

inline yaucl::functional::optional_binary<DataPredicate>
eqGrounding(const DataPredicate &toGround,
            const std::unordered_map<std::string, union_minimal> &map,
            const std::unordered_map<std::string, union_minimal>::const_iterator &it,
            std::unordered_map<std::string, std::vector<DataPredicate>>& toIntersectAtAnUpperLevel
) {// If the test is failed, then this predicate reduces to false, which invalidates all
// of the possibile binary predicates in it: so, everything reduces to an empty predicate
    if (!test_minimal_data_predicate(toGround, it->second))
        return yaucl::functional::optional_binary<DataPredicate>::invalid();
    // otherwise, this predicate after grounding reduces to being equal to the given value
// But, before allocating memory for nothing at all, I must first test all of the remaining
// ones if any
    std::vector<DataPredicate> allPredicates;
    if (! recursively_run_instantiation(toGround, map, allPredicates, toIntersectAtAnUpperLevel))
        return yaucl::functional::optional_binary<DataPredicate>::invalid();

    DataPredicate novel{toGround.var, EQ, it->second};
    novel.label = toGround.label; // Inheriting the label information, if any
    std::swap(allPredicates, novel.BiVariableConditions); // moving the memory, instead of copying it!
    return {novel}; // valid, not missing value
}

yaucl::functional::optional_binary<DataPredicate>
instantiateWithValues(const DataPredicate &toGround,
                      const std::unordered_map<std::string, union_minimal> &map,
                      std::unordered_map<std::string, std::vector<DataPredicate>>& toIntersectAtAnUpperLevel) {
    // Sanity check!
    // If there is a variable name on the right handside, I shall not have binary predicates assocated as siblings
    assert(toGround.varRHS.empty() || toGround.BiVariableConditions.empty());

    switch (toGround.casusu) {
        case INTERVAL: {
            // If this is an interval, there should be no right variable!
            // Consistency checking for syntax
            assert(toGround.varRHS.empty());
            // If the map contains the value to be grounded as part of it, then
            // we will need to test the predicate
            auto it = map.find(toGround.var);
            if (it != map.end()) {
                return eqGrounding(toGround, map, it, toIntersectAtAnUpperLevel);
            } else {
                // Otherwise, the evaluation is unknown, and we continue with the internal predicates, if any
                std::vector<DataPredicate> allPredicates;
                if (! recursively_run_instantiation(toGround, map, allPredicates, toIntersectAtAnUpperLevel))
                    return yaucl::functional::optional_binary<DataPredicate>::invalid();

                DataPredicate novel{toGround.label, toGround.var, toGround.value, toGround.value_upper_bound};
                std::swap(allPredicates, novel.BiVariableConditions); // moving the memory, instead of copying it!
                return novel;
            }
        }

        case TTRUE: {
            std::vector<DataPredicate> allPredicates;
            if (! recursively_run_instantiation(toGround, map, allPredicates, toIntersectAtAnUpperLevel))
                return yaucl::functional::optional_binary<DataPredicate>::invalid();

            DataPredicate novel;
            std::swap(allPredicates, novel.BiVariableConditions); // moving the memory, instead of copying it!
            return novel;
        }

        case LT:
        case GT:
        case LEQ:
        case GEQ:
        case EQ:
        case NEQ: {
            if (!toGround.varRHS.empty()) {
                // Due to the previous assertion, I am guaranteed that I do not need to check
                // the binary predicates, as they should never be there as children, but as siblings

                // if the right variable appears in the map, then replace itR with a value, first
                auto itR = map.find(toGround.varRHS);
                if (itR == map.end()) {
                    // Checking for the left variable
                    auto itL = map.find(toGround.var);
                    if (itL == map.end()) {
                        // If the left variable is not in the map either, then return the predicate as it is
                        return {toGround};
                    } else {
                                                      // Otherwise, I will need to revert the predicate...
                        DataPredicate cpy = toGround.reverseBiVariablePredicate()
                                                      // and then instantiate it: as I know for sure that there is no right value, I'm
                                                      // pretty confident that I can return the interval as it is
                                                     .instantiateRHSWith(itL->second);
                        // Now, I need to check whether cpy has the same variable as the old one
                        if (cpy.var == toGround.var) {
                            // If yes, I can immediately return it as a proper value (e.g., x == x)
                            // even if this eventuality is rather improbable
                            std::cerr << "WARNING: an odd eventuality was reached" << std::endl;
                            return {cpy};
                        } else {
                            // Otherwise, this interval shall be intersected with the other one, at the parent level
                            toIntersectAtAnUpperLevel[cpy.var].emplace_back(cpy);
                            return yaucl::functional::optional_binary<DataPredicate>::validButMissing();
                        }
                    }
                } else {
                    // Instantiating the interval appropriately
                    DataPredicate cpy = toGround.instantiateRHSWith(itR->second);
                    // After reducint the predicate to that, use recursion, so to minimize the code writing and
                    // the case analysis. This won't diverge, as I'm not adding data, but simplifying/rewriting
                    return instantiateWithValues(cpy, map, toIntersectAtAnUpperLevel);
                }
            } else {
                // if the RHS is empty, then all I need to do is to test if the left element is part of the map
                auto itL = map.find(toGround.var);
                if (itL == map.end()) {
                    // If it isn't, I'm not grounding it, and therefore I'm returning the interval untouched
                    return {toGround};
                } else {
                    // Otherwise, I just need to test if it is valid and, if yes, I replace the interval
                    // with the exact value that I extracted
                    return eqGrounding(toGround, map, itL, toIntersectAtAnUpperLevel);
                }
            }
        }

    }
}


