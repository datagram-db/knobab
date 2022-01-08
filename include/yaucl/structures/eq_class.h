//
// Created by giacomo on 28/02/21.
//

#ifndef CLASSIFIERS_EQUIVALENCE_CLASS_H
#define CLASSIFIERS_EQUIVALENCE_CLASS_H

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

template<typename T>
struct equivalence_class {
    std::vector<std::pair<T,T>> cmpVector;

    void insert(const T& left, const T& right) {
        if (left < right) {
            cmpVector.emplace_back(right, left);
        } else if (right < left) {
            cmpVector.emplace_back(left, right);
        }
    }

    std::unordered_map<T, std::unordered_set<T>> calculateEquivalenceClass() {
        std::sort(cmpVector.begin(), cmpVector.end(), std::greater<>());
        std::unordered_map<T, T> bigger_to_smaller;
        std::unordered_map<T, std::unordered_set<T>> equivalence_class;

        for (size_t i = 0, N = cmpVector.size(); i<N; i++) {
            std::pair<T,T> x = cmpVector.at(i);
            auto it = bigger_to_smaller.find(x.first);

            if (it != bigger_to_smaller.end()) {
                T sec = it->second;
                if (sec > x.second) {
                    // I find a new smaller candidate
                    auto it2 = equivalence_class.find(sec);
                    assert(it2 != equivalence_class.end());
                    equivalence_class[x.second].insert(it2->second.begin(), it2->second.end());
                    equivalence_class[x.second].emplace(x.second);
                    for (auto& y: equivalence_class[x.second]) {
                        bigger_to_smaller[y] = std::min(bigger_to_smaller[y], x.second);
                    }
                    it->second = x.second;
                    bigger_to_smaller[sec] = x.second;
                    equivalence_class.erase(it2);
                } else if (sec < x.second) {
                    equivalence_class[sec].emplace(x.second);
                }
            } else {
                // New entry (for the moment)
                bigger_to_smaller[x.first] = x.second;
                bigger_to_smaller[x.second] = x.second;
                equivalence_class[x.second].emplace(x.first);
                equivalence_class[x.second].emplace(x.second);
                auto it2 = equivalence_class.find(x.first);
                if (it2 != equivalence_class.end()) {
                    equivalence_class[x.second].insert(it2->second.begin(), it2->second.end());
                    equivalence_class.erase(it2);
                }
            }
        }

        return equivalence_class;


    }

};

#endif //CLASSIFIERS_EQUIVALENCE_CLASS_H
