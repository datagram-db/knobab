//
// Created by giacomo on 17/02/2022.
//

#ifndef DATAREP_EXISTSITERATOR_H
#define DATAREP_EXISTSITERATOR_H

#include <vector>
#include <data_views/DataRepresentation.h>

/**
 * Implementation of the iterator over the Exist representation, that now shall also return the event id
 */
class ExistsIterator {
    std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>::iterator ptr;
    bool isSet = false;
    DataRepresentationEvent data;
    bool isForJoin;

public:
    ExistsIterator(const std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>>::iterator &ptr, bool isForJoin);

    ~ExistsIterator() {}
    ExistsIterator  operator++(int);
    ExistsIterator& operator++();
    const DataRepresentationEvent& operator* () ;
    const DataRepresentationEvent& operator->() ;
    ExistsIterator  operator+ (size_t v)   const;
    bool operator==(const ExistsIterator &rhs) const;
    bool operator!=(const ExistsIterator &rhs) const;
};


#endif //DATAREP_EXISTSITERATOR_H
