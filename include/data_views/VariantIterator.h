//
// Created by giacomo on 17/02/2022.
//

#ifndef DATAREP_VARIANTITERATOR_H
#define DATAREP_VARIANTITERATOR_H

#include <string>
#include <vector>
#include <data_views/PipelineResultData.h>
#include <oid.h>
#include <functional>
#include <optional>


enum VariantIterator_t {
    RangeQueryIt,
    ExistsIt,
    OIDIt,
    FilterIt,
    FilterAndTransformIt,
    VectorIt,
    NoneIt
};

class VariantIterator {
    uint32_t no_trace_ids;
    uint64_t current_range_data_ptr_pos;
    const std::vector<size_t> *no_event_ids;
    bool isForJoin, isSet;
    PipelineResultData data;
    bool isCurrentSet;

    /**
     * If the first element of the pair is zero, then the result represents an empty set
     * If the first element of the pair is one, then the result represent the universal relationship,
     * Otherwise, the first element of the pair shall be two, and the remainder shall contain actual data.
     */
    std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> *range_data_ptr;

    /** Iterator over the exists */
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>::iterator ex_ptr;

    /** Iterator over the oid tables */
    std::vector<oid>::iterator oid_ptr;

    /**
     * Iterator over concrete table representations
     */
    std::vector<PipelineResultData>::iterator data_ptr;

    VariantIterator(uint32_t noTraceIds,
                    uint32_t currentTraceId,
                    uint16_t currentEventId,
                    uint64_t currentRangeDataPtrPos,
                    const std::vector<size_t> *noEventIds,
                    bool isEventJoinCondition,
                    std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> *rangeDataPtr);


    std::vector<VariantIterator> iterators;
    std::function<bool(const PipelineResultData&)> f;
    std::function<PipelineResultData(const PipelineResultData&)> T, Tinv;

public:
    VariantIterator_t casusu;
    /**
     * Default iterator
     */
    VariantIterator() : casusu{NoneIt}, no_event_ids{nullptr}, range_data_ptr{nullptr} {}
    VariantIterator(const VariantIterator&) = default;
    VariantIterator(VariantIterator&&) = default;
    VariantIterator& operator=(const VariantIterator&) = default;
    VariantIterator& operator=(VariantIterator&&) = default;

    /**
     * Iterator for range queries
     * @param v                 Pointer to the result of a range query
     * @param isBegin           Whether this should be the begin iterator. it is the End iterator otherwise
     * @param no_trace_ids      Number of the maximum trace id from the KB
     * @param no_event_ids      Map associating the event id to its length
     * @param isEventJoinCondition      Whether the events should be replicated for the join condition
     */
    VariantIterator(std::pair<int, std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>> *v,
                    bool isBegin,
                    uint32_t no_trace_ids,
                    const std::vector<size_t> *no_event_ids,
                    bool isEventJoinCondition);

    /**
     * Iterators as coming from the Exists function
     * @param ptr       Pointer to the computed result
     * @param isForJoin Whether the events should be replicated for the join condition
     */
    VariantIterator(const std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>::iterator &ptr, bool isForJoin);

    /**
     * Iterators directly coming from the tables, where trace id and event id should be returned
     * @param ptr           Pointer to the oid table
     * @param isForJoin     Whether the events should be replicated for the join condition
     */
    VariantIterator(const std::vector<oid>::iterator &ptr, bool isForJoin);


    VariantIterator(const std::vector<PipelineResultData>::iterator &ptr, bool isForJoin);

    /**
     * Iterators for filtering the data from another iterator: the aim is to avoid data replication
     * when possible
     * @param begin     Begin iterator
     * @param end       End iterator, for continuing the scan
     * @param f         Boolean filtering function over the DataRepresentationEvent
     *                  representation of the underlying data
     */
    VariantIterator(const VariantIterator& begin,
                    const VariantIterator& end,
                    std::function<bool(const PipelineResultData&)> f);

    /**
     * Iterators for both filtering and then transforming data contained in the iterator.
     * The aim is to reduce the data allocation as much as possible.
     * @param begin     Begin iterator
     * @param end       End iterator, for continuing the scan
     * @param f         Boolean filtering function over the DataRepresentationEvent
     *                  representation of the underlying data
     * @param T         Transformation function, adding a constant allocation for compying the
     *                  element to a local temporary variable.
     * @param T         Inverse transformation function: that is going to be useful for look-up queries, where
     *                  the look up value needs to be back transformed to the original data operator, so that
     *                  the range query can be computed effectively over the non-filtered data
     */
    VariantIterator(const VariantIterator &begin, const VariantIterator &end,
                                     std::function<bool(const PipelineResultData &)> f,
                                     std::function<PipelineResultData(const PipelineResultData&)> T,
                    std::function<PipelineResultData(const PipelineResultData&)> Tinv);

    /**
     * Implementing the unsigned distance between the two pointers
     * @param begin     RHS towards which compute the distance
     * @return Distance value
     */
    int64_t operator-(const VariantIterator &begin) const;

    /**
     * Implements an
     * @param begin
     * @param end
     * @return
     */
    static VariantIterator nextUntimedIterator(const VariantIterator &begin, const VariantIterator &end) {
        const static auto f = [](const PipelineResultData& x) {return x.event_id > 0;};
        const static auto T = [](const PipelineResultData& x) { auto y = x; y.event_id--; return y; };
        const static auto Tinv = [](const PipelineResultData& x) { auto y = x; y.event_id++; return y; };
        return {begin, end, f, T, Tinv};
    }

    /**
     * Provides an estimate to the current pointer condition, given the current offset and the data structure of choice
     * @return
     */
    size_t currentIteratorPosition() const;

    /**
     * Memory efficient implementation of operator++, which doesn't really require to implicitely dereference, thus
     * not necessarily return some data.
     */
    void pop();
    VariantIterator operator++(int);
    VariantIterator operator++();
    const PipelineResultData& operator*();
    const PipelineResultData& operator->();
    VariantIterator operator+(size_t v) const;
    bool operator==(const VariantIterator &rhs) const;
    bool operator!=(const VariantIterator &rhs) const;

    static std::optional<VariantIterator> reset_pointers_recursively(VariantIterator first, VariantIterator last,
                                                                     const PipelineResultData &value,
                                                                     bool isLower);

private:
    /**
     * Returns the type associated to the non-filter-transform iterators
     * @return
     */
    VariantIterator_t getConcreteOperatorType() const;

    /**
     * Resets the current iterator conditions using another iterator's information
     * @param begin
     */
    void setPointerAtStep(const VariantIterator& begin);

    void settingRestrained(const VariantIterator &begin, VariantIterator_t kase);


};





#endif //DATAREP_VARIANTITERATOR_H
