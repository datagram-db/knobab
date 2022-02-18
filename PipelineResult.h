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
    std::vector<DataRepresentationEvent> actual_data;

    VariantIterator be, en;
public:


};


#endif //DATAREP_PIPELINERESULT_H
