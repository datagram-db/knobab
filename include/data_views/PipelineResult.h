//
// Created by giacomo on 17/02/2022.
//

#ifndef DATAREP_PIPELINERESULT_H
#define DATAREP_PIPELINERESULT_H

#include <data_views/VariantIterator.h>

enum PipelineResult_t {
    ActualData,
    VariantIt
};

class PipelineResult {
    PipelineResult_t casusu;
    std::vector<PipelineResultData> actual_data;

    VariantIterator be, en;
public:
    PipelineResult() : casusu{ActualData} {}
    PipelineResult(const VariantIterator& begin, const VariantIterator& end);
    PipelineResult(const std::vector<PipelineResultData>& data);
    PipelineResult(const PipelineResult& ) = default;
    PipelineResult(PipelineResult&& ) = default;
    PipelineResult& operator=(const PipelineResult& ) = default;
    PipelineResult& operator=(PipelineResult&& ) = default;

    VariantIterator begin(bool isForJoin = true);
    VariantIterator end(bool isForJoin = true);

    /**
     * If the data is stored as a range of pointers, it makes the representation concrete, and replaces the iterators
     * with some actual data
     */
    void makeConcrete();

    /**
     * It adds some data to the final representation. If the data is not concrete, then it will instantiate the table
     * into a vector
     * @param data
     */
    void push_back(const PipelineResultData& data);
    void emplace_back(uint32_t trace_id, uint16_t event_id, bool isForJoin = true, double satisfiability = 1.0);
    void emplace_back(uint32_t trace_id, uint16_t event_id, const std::vector<size_t>& join_events = {}, double satisfiability = 1.0);

    /**
     * This ensures that the data representation is ordered. If you are always sure that the data is always ordered,
     * please do not do that. Otherwise, this might be the only way to make the data sorted, at any point.
     */
    void ensure_order();
};


#endif //DATAREP_PIPELINERESULT_H
