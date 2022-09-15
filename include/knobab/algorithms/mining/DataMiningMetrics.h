//
// Created by giacomo on 31/01/2020.
//

#ifndef TUTORIALS_DATAMININGMETRIC_H
#define TUTORIALS_DATAMININGMETRIC_H

#include <map>
#include <vector>
#include <string>
#include <set>

#include <knobab/algorithms/mining/Rule.h>
#include <yaucl/functional/LexicographicalOrder.h>
#include <knobab/dataStructures/CountTableFPTree.h>


using VTLexic = LexicographicalOrder<std::vector<std::string>, std::string>;

/**
 * This struct stores the association itemset/number_of_occurrences returned from the FPGrowth algorithm and returns
 * a set of data mining metrics
 */
template <typename T> struct DataMiningMetrics {

    std::map<std::vector<T>, unsigned long, LexicographicalOrder<std::vector<T>, T>> f;///<  Storing the item-support information from the FPGrowth algorithm
    double sumAll = 0.0;                                         ///<  Sum of all the supports for |T|

    /**
     * Initialization via the output of the FPGrowth algorithm
     * @param S
     */
    DataMiningMetrics(const std::set<GenPattern<T>>& S) {
        for (auto x : S) { // For each pattern
            std::vector<T> v{}; // Store it as a vector
            v.reserve(x.first.size());
            for (auto it = x.first.begin(); it != x.first.end(); ) {
                v.push_back(std::move(x.first.extract(it++).value()));
            }
            f[v] = x.second; // Associate the support to the rule represented as a vector
            sumAll += x.second; // Increment the number of the (frequent) transactions (Suggestion: replace it with the actual size of |T|)
        }
    }

    /**
     * Support definition for each item
     * @param i
     * @return
     */
    size_t support(const std::vector<T>& i) const {
        size_t sum = 0;
        for (auto it = f.begin(); it != f.end(); it++) {
            if (IsSupsetOf(it->first, i)) {
                sum += it->second;
            }
        }
        return sum;
    }

    /**
     * Rule support
     * @param r
     * @return
     */
    double support(const Rule<T>& r) const {
        std::vector<T> unione;
        for (const auto& x: r.head) unione.emplace_back(x);
        for (const auto& x: r.tail) unione.emplace_back(x);
        std::sort(unione.begin(), unione.end());
        unione.erase(std::unique(unione.begin(), unione.end()), unione.end());
        return ((double)support(unione)) / sumAll;
    }


    /**
     * Rule lift
     * @param r
     * @return
     */
    double lift(const Rule<T>& r) const {
        std::vector<T> unione;
        for (const auto& x: r.head) unione.emplace_back(x);
        for (const auto& x: r.tail) unione.emplace_back(x);
        std::sort(unione.begin(), unione.end());
        unione.erase(std::unique(unione.begin(), unione.end()), unione.end());
        double pup = ((double)support(unione))/sumAll;
        double ppred = ((double)support(r.head) / sumAll);
        double psucc = ((double)support(r.tail) / sumAll);
        double final = (pup)/(ppred*psucc);
        return final;
    }
};

#endif //TUTORIALS_DATAMININGMETRIC_H
