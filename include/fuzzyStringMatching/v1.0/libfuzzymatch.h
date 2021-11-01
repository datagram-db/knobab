/*
 * libfuzzymatch.h
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

#ifndef EDGEINDEXING_FMATCH_H
#define EDGEINDEXING_FMATCH_H

#include <cstdio>
#include <unistd.h>
#include <string>

/*#include <yaucl/structures/rbtree/rbtree.h>
using namespace yaucl::structures::rbtree;
#include <yaucl/algorithms/binsearch.h>
using namespace yaucl::algorithms;
#include <serializers/commons/structure.h>
#include <yaucl/fs/mmapFile.h>
//using namespace yaucl::fs;
#include <yaucl/java/java_classes_name.h>

#include <yaucl/algorithms/binsearch.h>
using namespace yaucl::algorithms;

#include <yaucl/structures/PollMap.h>
using namespace yaucl::structures;*/


/**
 * Data structure used for comparing pairs in primary memory where the fist element is a string. Please note that
 * only the fist parameter (the string) will be compared.
 *
 * @tparam T    Type of the data ignored during the comparison
 */
template <typename T> struct less_than_stringKey
{
    inline bool operator() (const std::pair<std::string, T>& struct1, const std::pair<std::string, T>& struct2)
    {
        return (struct1.first < struct2.first);
    }
};

/**
 * Pimary indexing data structure
 */
struct long_index_file {
    /**
     * Id of the element or bucket's hash
     */
    size_t id;

    /**
     * Offset to the current bucket or element id
     */
    size_t offset;
};

/*#include "versions/v0.1/LongStringValueMultimapIndexer/LSVM_Primary.h"
#include "versions/v0.1/StringLongHashMultimapIndexer/SLHM_Primary.h"
#include "versions/v0.1/StringToTwoGramSizeHashMultimapIndexer/STTGSHM_Primary.h"
#include "versions/v0.1/LongStringValueMultimapIndexer/LSVM_Secondary.h"
#include "versions/v0.1/StringLongHashMultimapIndexer/SLHM_Secondary.h"
#include "versions/v0.1/StringToTwoGramSizeHashMultimapIndexer/STTGSHM_Secondary.h"

#include "FuzzyMatch_FDContainer.h"*/

#endif //EDGEINDEXING_FMATCH_H
