/*
 * LSVM_Primary.cpp
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

#include <iostream>
#include "fuzzyStringMatching/v0.1/LSVM_Primary.h"

LongStringValueMultimapIndexer_Primary::LongStringValueMultimapIndexer_Primary() : ordered_multimap{&compareUL_Int} {}

void LongStringValueMultimapIndexer_Primary::store(size_t id, std::string &element) {
    ordered_multimap.insertKey(id)->overflowList.push_back(element);
    if (id > maxLong) maxLong = id;
}

void LongStringValueMultimapIndexer_Primary::serialize(FILE *primaryIndex, FILE *values) {
    RBTree<size_t, std::string>::rbiterator it = ordered_multimap.iterator();
    char init = ' ';

    // Initial header: position zero is set to nothing.
    // For this data structure the zero is just a
    fwrite(&init, sizeof(char), 1, values);
    size_t offset = 1;
    while (it.hasNext()) {
        Node<size_t, std::string>* element = it.disposeWhileIterating();
        size_t key = it.getCurrentK();
        // Writing all the longs as not appearing, that is stored at position zero
        // --> This approach would have quicker performances, via an O(1) access time, but I cannot "pay"
        //     for several gigabites of secondary memory. Therefore, even the longs will be sorted
        //     This also makes the serialization far way more efficient
        //for (size_t i = prevElement; i<key; i++)
        //    fwrite(&noOffset, sizeof(size_t), 1, primaryIndex);

        // Writing the key and the offset for the current element
        fwrite(&key, sizeof(size_t), 1, primaryIndex);
        fwrite(&offset, sizeof(size_t), 1, primaryIndex);

        // -- serializing the size of the properties vector
        size_t n = element->overflowList.size();
        size_t buffSize = (n+1)*(sizeof(size_t));

        fwrite(&n, sizeof(buffSize), 1, values);
        for (size_t i = 0; i<n; i++) {
            std::string& x = element->overflowList[i];
            fwrite(&buffSize, sizeof(buffSize), 1, values);
            buffSize += (x.size()+1);
        }

        for (size_t i = 0; i<n; i++) {
            std::string& x = element->overflowList[i];
            fwrite(x.c_str(), x.size()+1, 1, values);
        }
        offset += buffSize;
    }

    fclose(primaryIndex);
    fclose(values);
}
