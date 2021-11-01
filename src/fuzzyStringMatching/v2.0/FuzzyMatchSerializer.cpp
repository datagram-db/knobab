/*
 * FuzzyMatchSerializer.cpp
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

#include "fuzzyStringMatching/v0.2/FuzzyMatchSerializer.h"

#include <yaucl/strings/stringutils.h>
using namespace yaucl::strings;

//#define BOOST_SYSTEM_NO_DEPRECATED
//#include <boost/filesystem.hpp>
#include <cstdio>
#include <filesystem>
namespace fs = std::filesystem;


void compareStringHashmap1(std::string str, std::unordered_map<std::string, size_t> &map,
                           std::vector<size_t> &vec) {
    size_t numPairs = str.length() - 1;
    if (numPairs == 0) {
        map[str] = 0;
        vec.emplace_back(1);
    } else {
        if (numPairs < 0) numPairs = 0;
        int singleGrams = 0;
        for (int i = 0; i < numPairs; i++) {
            std::string s{utf8_tolower(str.substr(i, 2))};
            if (!s.empty()) {
                std::unordered_map<std::string, size_t>::iterator pos = map.find(s);
                if (pos == map.end()) {
                    map[s] = singleGrams++;
                    vec.emplace_back(1);
                } else {
                    size_t x = vec[pos->second];
                    vec[pos->second] = x + 1;
                }
            }
        }
    }
}

void compareStringHashmap2(std::string &string, std::unordered_map<std::string, size_t> &map,
                           std::vector<size_t> &vec) {
    {
        std::string str;
        std::stringstream ss(string);
        std::vector<std::string> pairs;

        // Getting the grams within each word, thus avoiding to create 2grams with a space
        while (ss >> str) {
            compareStringHashmap1(str, map, vec);
        }
    }
    // Updating the gram count with each elements' output
    for (std::unordered_map<std::string,size_t>::iterator kp = map.begin(), end = map.end(); kp != end; ++kp) {
        kp->second = vec[kp->second];
    }
}

void FuzzyMatchSerializer::serialize(std::string path) {
    fs::path p{path};
    if (!fs::exists(p)) {
        fs::create_directories(p);
    } else {
        if (!fs::is_directory(p)) {
            std::cout << "Removing the file associated to " << p << std::endl;
            fs::remove(p);
            fs::create_directories(p);
        }

        // gramToObject serialization
        {
            std::string gramToObject_hash = fs::canonical(p / "gramToObjects.csv_vector.bin").string();
            std::string gramToObject_vals = fs::canonical(p / "gramToObjects.csv_values.bin").string();
            FILE *hash = fopen64(gramToObject_hash.c_str(), "w");
            FILE *valu = fopen64(gramToObject_vals.c_str(), "w");
            this->gramToObject.serialize(hash, valu);
            fclose(hash);
            fclose(valu);
        }

        // objectGramSize serialization
        {
            std::string gramToObject_hash = fs::canonical(p / "objectGramSize.csv_vector.bin").string();
            std::string gramToObject_vals = fs::canonical(p / "objectGramSize.csv_values.bin").string();
            FILE *hash = fopen64(gramToObject_hash.c_str(), "w");
            FILE *valu = fopen64(gramToObject_vals.c_str(), "w");
            this->objectGramSize.serialize(hash, valu);
            fclose(hash);
            fclose(valu);
        }

        // termObject serialization
        {
            std::string gramToObject_hash = fs::canonical(p / "termToObjects.csv_vector.bin").string();
            std::string gramToObject_vals = fs::canonical(p / "termToObjects.csv_values.bin").string();
            FILE *hash = fopen64(gramToObject_hash.c_str(), "w");
            FILE *valu = fopen64(gramToObject_vals.c_str(), "w");
            this->termObject.serialize(hash, valu);
            fclose(hash);
            fclose(valu);
        }

        // objectMultipleStirngs serialization
        {
            std::string gramToObject_hash = fs::canonical(
                    p / "objectMultipleStirngs.csv_vector.bin").string();
            std::string gramToObject_vals = fs::canonical(
                    p / "objectMultipleStirngs.csv_values.bin").string();
            FILE *hash = fopen64(gramToObject_hash.c_str(), "w");
            FILE *valu = fopen64(gramToObject_vals.c_str(), "w");
            this->objectMultipleStirngs.serialize(hash, valu);
            fclose(hash);
            fclose(valu);
        }

        // twogramAndStringMultiplicity serialization
        {
            std::string gramToObject_hash = fs::canonical(
                    p / "twogramAndStringToMultiplicity.csv_vector.bin").string();
            std::string gramToObject_vals = fs::canonical(
                    p / "twogramAndStringToMultiplicity.csv_values.bin").string();
            FILE *hash = fopen64(gramToObject_hash.c_str(), "w");
            FILE *valu = fopen64(gramToObject_vals.c_str(), "w");
            this->twogramAndStringMultiplicity.serialize(hash, valu);
            fclose(hash);
            fclose(valu);
        }



    }
}

void FuzzyMatchSerializer::addGramsToMap(std::string &string, size_t id,
                                         std::vector<std::string> &associatedOtherStrings) {
    if (string.empty()) return;

    termObject.store(string, id);
    objectMultipleStirngs.store(id, string);
    for (std::string& x : associatedOtherStrings)
        objectMultipleStirngs.store(id, x);

    std::unordered_map<std::string,size_t> cp;
    std::vector<size_t> vec;

    compareStringHashmap2(string, cp, vec);
    size_t sum = 0;
    for (size_t& j : vec) {
        sum += j;
    }
    objectGramSize.store(string, sum);

    for (std::unordered_map<std::string,size_t>::iterator begin = cp.begin(), end = cp.end(); begin!=end; begin++) {
        std::string x = begin->first;
        twogramAndStringMultiplicity.store(string, x, begin->second);
        gramToObject.store(x, id);
    }
}
