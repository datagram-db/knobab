/*
 * SLHM_Primary.h
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

#include "versions/v0.1/libfuzzymatch.h"

#ifndef EDGEINDEXING_SLHMI_PRIMARY_H
#define EDGEINDEXING_SLHMI_PRIMARY_H

/**
 * This class allows to serialize a HashMultimap<std::string, long> as two indices: a primary index for the hashes
 * associated to the keys (the strings), and each bucket is string ordered, and contains an array of pairs of string
 * (the actual key) and values.
 */
class SLHM_Primary {
    std::hash<std::string> hfunc;
    RBTree<size_t, std::pair<std::string, std::vector<size_t>>> ordered_multimap;

public:
    SLHM_Primary();

    void store(std::string& elem, size_t value);

    void serialize(FILE* hashing, FILE* values);
};
//// </class>


#endif //EDGEINDEXING_SLHMI_PRIMARY_H
