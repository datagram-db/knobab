//
// Created by giacomo on 17/02/2022.
//

#ifndef DATAREP_OIDITERATOR_H
#define DATAREP_OIDITERATOR_H


#include <vector>
#include <oid.h>
#include <data_views/DataRepresentation.h>


class OIDIterator {
    std::vector<oid>::iterator ptr;
    bool isSet = false;
    DataRepresentationEvent data;
    bool isForJoin;

public:
    OIDIterator(const std::vector<oid>::iterator &ptr, bool isForJoin);

    OIDIterator  operator++(int);
    OIDIterator& operator++();
    const DataRepresentationEvent& operator* ();
    const DataRepresentationEvent&    operator->();
    OIDIterator  operator+ (size_t v)   const;
    bool      operator==(const OIDIterator& rhs) const;
    bool      operator!=(const OIDIterator& rhs) const;
};


#endif //DATAREP_OIDITERATOR_H
