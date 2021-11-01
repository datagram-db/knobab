/*
 * SLHM_Primary.cpp
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

#include <fuzzyStringMatching/v1.0/SLHM_Primary.h>

SLHM_Primary::SLHM_Primary() : ordered_multimap{&compareUL_Int} {}

less_than_stringKey<std::vector<size_t>>                   ltk1{};

struct secondary_memory_tmp {

};

void SLHM_Primary::store(std::string &elem, size_t value) {
    Node<size_t, std::pair<std::string, std::vector<size_t>>> *hashKey = ordered_multimap.insertKey(hfunc(elem));
    if (hashKey->getSize() != 0) {
        for (std::pair<std::string, std::vector<size_t>> &x : hashKey->overflowList) {
            if (x.first == elem) {
                x.second.emplace_back(value);
                return;
            }
        }
    }
    std::vector<size_t> element;
    element.push_back(value);
    hashKey->add(elem, element);
}

void SLHM_Primary::serialize(FILE *hashing, FILE *values) {
    RBTree<size_t, std::pair<std::string, std::vector<size_t>>>::rbiterator it = ordered_multimap.iterator();
    size_t offset = 0;
    while (it.hasNext()) {
        Node<size_t, std::pair<std::string, std::vector<size_t>>> *element = it.disposeWhileIterating();
        // A, Write Hashing for this bucket
        size_t bucket = it.getCurrentK();
        fwrite(&bucket, sizeof(size_t), 1, hashing);
        fwrite(&offset, sizeof(size_t), 1, hashing);
        size_t nElement = element->overflowList.size();

        // B. Sorting
        // Sorting the array by string elements, so that the strings may be searched in log time within the bucket
        std::sort(element->overflowList.begin(), element->overflowList.end(), ltk1);

        // Each element in the block is composed as so:
        // 1) number of strings associated to longs.
        //    the values are stored after the number of the elements...
        size_t offsetsInBucketForKey = sizeof(size_t);
        fwrite(&nElement, sizeof(size_t), 1, values);

        // 2) offset of each string associated to long (see 3)
        ///   .., after the nElement offsets
        offsetsInBucketForKey += (sizeof(size_t)*(nElement));

        // Then I serialize the values. Before doing that, I must evaluate which are the actual offsets that I have to use
        for (std::pair<std::string, std::vector<size_t>>& keyValueAssociation : element->overflowList) {
            // For each pair, I write the current offset for the keyMultivalue association
            fwrite(&offsetsInBucketForKey, sizeof(size_t), 1, values);
            offsetsInBucketForKey += (sizeof(size_t)*(keyValueAssociation.second.size()+2)+keyValueAssociation.first.size()+1);
        }

        // 3) for each string, we have the size of the block associated to it
        //    such size is offsetToString + vecSize + [vector] + stirng.size+1
        for (std::pair<std::string, std::vector<size_t>>& keyValueAssociation : element->overflowList) {
            size_t stirngSize = keyValueAssociation.first.size()+1;

            // 1) string length
            fwrite(&stirngSize, sizeof(size_t), 1, values);
            // 2) string
            fwrite(keyValueAssociation.first.c_str(), stirngSize, 1, values);

            // 3) vector size after string_length+sizeof(size_t)
            size_t size = keyValueAssociation.second.size();
            fwrite(&size, sizeof(size_t), 1, values);

            // 4) vector
            // Writing the longs
            for (int i = 0; i<keyValueAssociation.second.size(); i++)
                fwrite(&keyValueAssociation.second[i], sizeof(size_t), 1, values);

        }
        offset += offsetsInBucketForKey;

    }
    fclose(hashing);
    fclose(values);
}
