//
// Created by giacomo on 31/01/2020.
//

#ifndef TUTORIALS_RULESFROMFREQUENTITEMSET_H
#define TUTORIALS_RULESFROMFREQUENTITEMSET_H

#include "GeneralToSpecificHeuristic.h"
#include "Rule.h"
#include "DataMiningMetrics.h"

/**
 * Implementation of the General-to-Speicific Heuristic algorithm for the Rule mining algorithm
 */
ForAll(T)
class RulesFromFrequentItemset : public GeneralToSpecificHeuristic<Rule<T>> {
    DataMiningMetrics<T>& scorer;

protected:
    /**
     * The quality assessment is going to return only the most plausible elements
     *
     * @param hypothesis
     * @return
     */
    bool quality_assessment(const std::pair<double, Rule<T>> &hypothesis) const override {
        return hypothesis.first > 1; // Getting only the plausible elements
    }

    /**
     *
     * @param hypothesis
     * @return
     */
    std::vector<std::pair<double, Rule<T>>> specialize_hypothesis(const Rule<T> &hypothesis) const override {
        std::vector<std::pair<double, Rule<T>>> result;
        for (const T& x : hypothesis.head) { // For each x appearing in the head of a hypothesis...
            Rule<T> newRule;                              // ... generate a novel rule ...
            {
                // ... having as a tail the tail of hypothesis where x is added ...
                std::vector<T> succ{hypothesis.tail};
                succ.emplace_back(x);
                // (Please Note: the fastest approach of removing duplicates from a vector is to create a set, and then move it back to a vector)
                {
                    std::set<T> setSucc( succ.begin(), succ.end() );
                    newRule.tail.assign(setSucc.begin(), setSucc.end() );
                }
            }

            // ... and as a head all the elements belonging to the hypothesis's head minus x, that was currently added to the tail
            for (const T& y : hypothesis.head)
                if (x != y) newRule.head.emplace_back(y);

            // ... Associate a score (e.g., lift) to the newly generated rule
            result.emplace_back(scorer.lift(newRule), newRule);
        }
        // Return all the ranked specializations of the hypothesis
        return result;
    }

    /**
     * Pruning all the elements from the queue that do not have a positive lift (i.e., a lift greater than 1)
     * @param queue
     */
    void prune(typename GeneralToSpecificHeuristic<Rule<T>>::PriorityQueue &queue) override {
        /*
         * Priority queues do not allow to directly remove the elements: therefore, I need to always remove the elements
         * from the queue, and then emplace the element in v if it meets the lift requirements
         */
        std::vector<std::pair<double, Rule<T>>> v;
        while (!queue.empty()) {
            auto qt = queue.top();
            if ((scorer.lift(qt.second)) > 1) {
                v.emplace_back(qt);                 // Preserving the element
            }
            queue.pop();                            // Always removing the element from the queue
        }
        auto it = v.begin();                        // Iterating over the vector, while freeing the memory
        while (it != v.end()) {
            queue.push(*it);                        // Re-adding the preserved element back
            it = v.erase(it);                       // Clearing the memory from the vector.
        }
    }


public:
    RulesFromFrequentItemset(DataMiningMetrics<T> &scorer) : scorer(scorer) {}

    std::unordered_set<Rule<T>> generate_hypotheses(const GenPattern<T>& pattern) {
        Rule<T> r{pattern.first};
        return GeneralToSpecificHeuristic<Rule<T>>::generate_hypotheses(r, scorer.lift(r));
    }
};


#endif //TUTORIALS_RULESFROMFREQUENTITEMSET_H
