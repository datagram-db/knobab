//
// Created by giacomo on 03/01/2022.
//

#ifndef KNOBAB_GROUNDING_H
#define KNOBAB_GROUNDING_H

#include <string>
#include <unordered_map>
#include <optional>
#include <yaucl/bpm/structures/commons/DataPredicate.h>
#include <yaucl/functional/optional_binary.h>

/**
 * This function provides the grounding of a single data predicate associated to a variable, if possible
 *
 * @param toGround                          Predicate to be grounded
 * @param map                               Substitution map, mapping each variable in the predicate to the value
 *                                          to be replaced
 * @param toIntersectAtAnUpperLevel         If the resulting predicate has a change of variable after grounding, in this
 *                                          return map you will have the result of it
 * @return There are two cases of missing value: either we had a failure, thus implying that the value
 *         did not satisfy the predicate, and therefore the whole part shall be discarded, or an empty
 *         value, meaning that the interval was successfully rewritten, but that, in the meantime, has
 *         changed the variable towards which it belongs. If the value is present, then this is the substitute
 *         candidate (the grounded one) for the one that was originally provided.
 */
yaucl::functional::optional_binary<DataPredicate> instantiateWithValues(
                                                    const DataPredicate& toGround,
                                                   const std::unordered_map<std::string, union_minimal>& map,
                                                   std::unordered_map<std::string, std::vector<DataPredicate>>& toIntersectAtAnUpperLevel
                                                   );


yaucl::functional::optional_binary<std::unordered_map<std::string, DataPredicate>> instantiateWithValues(
                                                    const std::unordered_map<std::string, DataPredicate>& toGround,
                                                    const std::unordered_map<std::string, union_minimal>& map) {
    std::unordered_map<std::string, std::vector<DataPredicate>> toIntersectAtAnUpperLevel;
    for (const auto& cp : toGround) {

    }
}

#endif //KNOBAB_GROUNDING_H
