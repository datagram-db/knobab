/*
 * FuzzyMatchSerializer.h
 * This file is part of fuzzyStringMatching
 *
 * Copyright (C) 2019 - Giacomo Bergami
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
// Created by giacomo on 25/03/19.
//

#ifndef INCONSISTENCY_FUZZYMATCHSERIALIZER_H
#define INCONSISTENCY_FUZZYMATCHSERIALIZER_H


///#include <serializers/commons/structure.h>
#include "fuzzyStringMatching/v0.1/SLHM_Primary.h"
#include "fuzzyStringMatching/v0.1/LSVM_Primary.h"
#include "fuzzyStringMatching/v0.1/STTGSHM_Primary.h"
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

void compareStringHashmap1(std::string str, std::unordered_map<std::string,size_t>& map, std::vector<size_t>& vec);

void compareStringHashmap2(std::string& string, std::unordered_map<std::string,size_t>& map, std::vector<size_t>& vec);

/**
 * This class creates the fuzzy string matching indices in primary memory, so that they might be later on serialized
 * in secondary memory. Even though such indices might fit per se in primary memory, it is impossible to load all the
 * data in primary memory. Therefore, this indexing part only copes with some data part.
 *
 * The serialized data should be accessed and queried using the FuzzyMatch class.
 */
class FuzzyMatchSerializer {
public:
    /**
     * This method creates all the indices in primary memory
     *
     * @param string                        Preferred string associated to the object
     * @param id                            Object id (eg. an offset)
     * @param associatedOtherStrings        Other secondary strings associated to the preferred object
     */
    void addGramsToMap(std::string& string, size_t id, std::vector<std::string>& associatedOtherStrings);

    /**
     * This method serializes the indices as several secondary memory files.
     * If it is a file, it will try to delete the file first.
     * If the current path does not exists, it will try to create the folder (mkdirs)
     *
     * @param path
     */
    void serialize(std::string path);

private:
    SLHM_Primary gramToObject, objectGramSize, termObject;
    LongStringValueMultimapIndexer_Primary objectMultipleStirngs;
    StringToTwoGramSizeHashMultimapIndexer twogramAndStringMultiplicity;

};


#endif //INCONSISTENCY_FUZZYMATCHSERIALIZER_H
