//
// Created by giacomo on 17/02/2022.
//

#include "data_views/PipelineResult.h"

PipelineResult::PipelineResult(const VariantIterator &begin, const VariantIterator &end) : be{begin}, en{end}, casusu{VariantIt} {}

PipelineResult::PipelineResult(const std::vector<PipelineResultData> &data) : actual_data(data), casusu(ActualData) {}

VariantIterator PipelineResult::begin(bool isForJoin) {
    switch (casusu) {
        case ActualData:
            return {actual_data.begin(), isForJoin};

        case VariantIt:
            return be;
    }
}

VariantIterator PipelineResult::end(bool isForJoin) {
    switch (casusu) {
        case ActualData:
            return {actual_data.end(), isForJoin};

        case VariantIt:
            return en;
    }
}

void PipelineResult::makeConcrete() {
    if (casusu == VariantIt) {
        while (be != en) {
            actual_data.push_back(*be);
            be.pop();
        }
        be = VariantIterator{};
        en = VariantIterator{};
        casusu = ActualData;
    }
}

void PipelineResult::push_back(const PipelineResultData &data) {
    makeConcrete();
    actual_data.emplace_back(data);
}

void PipelineResult::ensure_order() {
    makeConcrete();
    std::sort(actual_data.begin(), actual_data.end());
}

void PipelineResult::emplace_back(uint32_t trace_id, uint16_t event_id, bool isForJoin, double satisfiability) {
    makeConcrete();
    actual_data.emplace_back(trace_id, event_id, satisfiability, isForJoin ? std::vector<size_t>{event_id} : std::vector<size_t>{});
}

void PipelineResult::emplace_back(uint32_t trace_id, uint16_t event_id, const std::vector<size_t> &join_events,
                                  double satisfiability) {
    makeConcrete();
    actual_data.emplace_back(trace_id, event_id, satisfiability, join_events);
}
