/*
 * LSVM_Primary.h
 * This file is part of fuzzyStringMatching
 *
 * Copyright (C) 2018 - Giacomo Bergami
 *
 * fuzzyStringMatching is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * fuzzyStringMatching is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fuzzyStringMatching. If not, see <http://www.gnu.org/licenses/>.
 */


//
// Created by giacomo on 25/09/18.
//

#include "libfuzzymatch.h"

#ifndef EDGEINDEXING_LSVM_PRIMARY_H
#define EDGEINDEXING_LSVM_PRIMARY_H



/**
 * This class indicizes an HashMultimap<Long, std::string> into secondary memory, where the primary index is an array
 * from 0 to max_long, and the values file is just an array of array of strings.
 */
class LongStringValueMultimapIndexer_Primary {
    size_t maxLong = 0;
    RBTree<size_t, std::string> ordered_multimap;

public:
    LongStringValueMultimapIndexer_Primary();
    void store(size_t id, std::string& element);
    void serialize(FILE* primaryIndex, FILE* values);
};

#endif //EDGEINDEXING_LSVM_PRIMARY_H
