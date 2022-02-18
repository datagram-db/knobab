//
// Created by giacomo on 18/02/2022.
//

#ifndef DATAREP_VARIANT_ITERATOR_OPERATIONS_H
#define DATAREP_VARIANT_ITERATOR_OPERATIONS_H

#include <data_views/VariantIterator.h>

// TODO: all of these functions should be later on macros for efficiency reasons

/**
 * Implements the std::lower_bound for the VariantIterators, which might also have filtering conditions to consider!
 *
 * @param first     Begin iterator
 * @param last      End iterator
 * @param value     Value to be matched
 * @return          As per std::lower_bound
 */
VariantIterator lower_bound(VariantIterator first, VariantIterator last, const PipelineResultData& value);


/**
 * Implements the std::upper_bound for the VariantIterators, which might also have filtering conditions to consider!
 *
 * @param first     Begin iterator
 * @param last      End iterator
 * @param value     Value to be matched
 * @return          As per std::upper_bound
 */
VariantIterator upper_bound(VariantIterator first, VariantIterator last, const PipelineResultData &value);

#endif //DATAREP_VARIANT_ITERATOR_OPERATIONS_H
