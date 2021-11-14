//
// Created by giacomo on 01/11/2021.
//

#include "SimplifiedFuzzyStringMatching.h"
#include "yaucl/strings/string_utils.h"


void compareStringHashmap1(const std::string& str, std::unordered_map<std::string, size_t> &map,
                           std::vector<size_t> &vec) {
    size_t numPairs = str.length() - 1;
    if (numPairs == 0) {
        map[str] = 0;
        vec.emplace_back(1);
    } else {
        if (numPairs < 0) numPairs = 0;
        int singleGrams = 0;
        for (int i = 0; i < numPairs; i++) {
            std::string s{yaucl::strings::utf8_tolower(str.substr(i, 2))};
            if (!s.empty()) {
                auto pos = map.find(s);
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

#include <sstream>

void compareStringHashmap2(const std::string &string, std::unordered_map<std::string, size_t> &map,
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
    for (auto & kp : map) {
        kp.second = vec[kp.second];
    }
}

//Credits to http://www.catalysoft.com/articles/StrikeAMatch.html
void compareString_letterPairs(const std::string& str, std::vector<std::string>& pairs) {
    pairs.clear();
    unsigned long numPairs = str.length()-1;
    if (!numPairs) {
        pairs.emplace_back(str);
    }
    if (numPairs < 0)
        numPairs = 0;
    for (unsigned long i = 0; i<numPairs; i++) {
        pairs.emplace_back(str.substr(i, 2));
    }
}

std::pair<size_t, bool> SimplifiedFuzzyStringMatching::put(const std::string &value) {
    auto it = unique_string.put(value);
    if (it.second) {
        std::unordered_map<std::string, size_t>& m = gram_multiplicity[value];
        std::vector<size_t> vec;
        compareStringHashmap2(value, m, vec);
        size_t sum = 0;
        for (size_t& j : vec) {
            sum += j;
        }
        objectGramSize[value] = sum;
        for (auto & twoGramFreq : m) {
            shift0_2grams_containment[twoGramFreq.first].emplace_back(it.first);
        }
    }
    return it;
}

#include <cassert>

std::string SimplifiedFuzzyStringMatching::get(size_t id) const {
    return unique_string.get(id);
}
