//
// Created by giacomo on 18/02/2022.
//

#include <data_operations/variant_iterator_operations.h>

VariantIterator
lower_bound(VariantIterator first, VariantIterator last, const PipelineResultData &value) {
    std::optional<VariantIterator> vi = VariantIterator::reset_pointers_recursively(first, last, value, true);
    if (vi.has_value())
        return vi.value();

    VariantIterator it = first;
    auto L = (int64_t)first.currentIteratorPosition(), R = (int64_t)last.currentIteratorPosition();
    int64_t count, step;
    count = R - L;
    while (count > 0) {
        it = first;
        step = count / 2;
        it = it + step;
        if (*it < value) {
            it.pop();
            first = it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}


VariantIterator upper_bound(VariantIterator first, VariantIterator last, const PipelineResultData &value) {
    std::optional<VariantIterator> vi = VariantIterator::reset_pointers_recursively(first, last, value, false);
    if (vi.has_value())
        return vi.value();

    VariantIterator it = first;
    auto L = (int64_t)first.currentIteratorPosition(), R = (int64_t)last.currentIteratorPosition();
    int64_t count, step;
    count = R - L;
    while (count > 0) {
        it = first;
        step = count / 2;
        it = it + step;
        if (!(*it < value)) {
            it.pop();
            first = it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}
