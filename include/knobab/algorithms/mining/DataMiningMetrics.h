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
#include "knobab/mining/CountTableFPTree.h"
#include <ranges>

//using VTLexic = LexicographicalOrder<std::vector<std::string>, std::string>;

/**
 * This struct stores the association itemset/number_of_occurrences returned from the FPGrowth algorithm and returns
 * a set of data mining metrics
 */
struct DataMiningMetrics {

    std::vector<std::vector<trace_t>> act_to_traces;
    std::unordered_map<std::vector<act_t>, size_t> compute_and;
    std::unordered_map<std::vector<act_t>, size_t> compute_or;
//    std::map<std::vector<T>, unsigned long, LexicographicalOrder<std::vector<T>, T>> f;///<  Storing the item-support information from the FPGrowth algorithm
    double sumAll = 0.0;                                         ///<  Sum of all the supports for |T|

    /**
     * Initialization via the output of the FPGrowth algorithm
     * @param S
     */
    DataMiningMetrics(const CountTemplate& S) : sumAll{(double)S.nTraces()}, act_to_traces(S.nAct()) {
        for (const auto& ref : S.table) {
            if (ref.id.parts.event_id > 0)
                act_to_traces[ref.id.parts.act].emplace_back(ref.id.parts.trace_id);
        }
    }
    DEFAULT_CONSTRUCTORS(DataMiningMetrics)
    void clear() {
        act_to_traces.clear();
        compute_and.clear();
        compute_or.clear();
        sumAll = 0.0;
    }
    void reset(const CountTemplate* S) {
        clear();
        sumAll = (double ) S->nTraces();
        act_to_traces.resize(S->nAct());
        for (const auto& ref : S->table) {
            if (ref.id.parts.event_id > 0)
                act_to_traces[ref.id.parts.act].emplace_back(ref.id.parts.trace_id);
        }
    }

    /**
     * Support definition for each item
     * @param i
     * @return
     */
    size_t support(const std::vector<act_t>& i) const {
        if (i.empty())
            return sumAll;
        else if (i.size() == 1)
            return act_to_traces.at(i.at(0)).size();
        else {
            std::vector<trace_t> orig = act_to_traces[i.at(0)];
            std::vector<trace_t> res;
            for (size_t j = 1; j<i.size(); j++) {
                const auto& ref = act_to_traces.at(i.at(j));
                std::set_union(orig.begin(), orig.end(), ref.begin(), ref.end(),
                               std::back_inserter(res));
                std::swap(res, orig);
            }
            return orig.size();
        }
    }

    std::vector<trace_t> negate_presence(act_t a, trace_t max_abs) {
        auto range = std::views::iota((trace_t)0, max_abs);
        const std::vector<trace_t>& orig = act_to_traces.at(a);
        std::vector<trace_t> res;
        std::set_difference(range.begin(), range.end(), orig.begin(), orig.end(),
                              std::back_inserter(res));
        return res;
    }


    size_t and_(const std::vector<act_t>& i)  {
        if (i.empty())
            return sumAll;
        else if (i.size() == 1)
            return act_to_traces.at(i.at(0)).size();
        auto it = compute_and.emplace(i, 0.0); //memoizing
        if (it.second) {
            std::vector<trace_t> orig = act_to_traces[i.at(0)];
            std::vector<trace_t> res;
            for (size_t j = 1; j<i.size(); j++) {
                const auto& ref = act_to_traces.at(i.at(j));
                std::set_intersection(orig.begin(), orig.end(), ref.begin(), ref.end(),
                                      std::back_inserter(res));
                std::swap(res, orig);
            }
            it.first->second =  orig.size();
        }
        return it.first->second;
    }

    size_t or_(const std::vector<act_t>& i)  {
        if (i.empty())
            return sumAll;
        else if (i.size() == 1)
            return act_to_traces.at(i.at(0)).size();
        auto it = compute_or.emplace(i, 0.0); //memoizing
        if (it.second) {
            std::vector<trace_t> orig = act_to_traces[i.at(0)];
            std::vector<trace_t> res;
            for (size_t j = 1; j<i.size(); j++) {
                const auto& ref = act_to_traces.at(i.at(j));
                std::set_union(orig.begin(), orig.end(), ref.begin(), ref.end(),
                                      std::back_inserter(res));
                std::swap(res, orig);
            }
            it.first->second =  orig.size();
        }
        return it.first->second;
    }

    /**
     * Rule support
     * @param r
     * @return
     */
    double support(const Rule<act_t>& r) const {
        std::vector<act_t> unione;
        for (const auto& x: r.head) unione.emplace_back(x);
        for (const auto& x: r.tail) unione.emplace_back(x);
        std::sort(unione.begin(), unione.end());
        unione.erase(std::unique(unione.begin(), unione.end()), unione.end());
        return ((double)support(unione)) / sumAll;
    }

    std::unordered_map<Rule<act_t>, double> score_decl_support;
    double decl_support(const Rule<act_t>& r) {
            std::vector<act_t> unione;
            for (const auto& x: r.head) unione.emplace_back(x);
            for (const auto& x: r.tail) unione.emplace_back(x);
            std::sort(unione.begin(), unione.end());
            unione.erase(std::unique(unione.begin(), unione.end()), unione.end());
            return ((double)and_(unione)+(sumAll-and_(r.head))) / sumAll;
    }

    size_t decl_int_support(const Rule<act_t>& r) {
        std::vector<act_t> unione;
        for (const auto& x: r.head) unione.emplace_back(x);
        for (const auto& x: r.tail) unione.emplace_back(x);
        std::sort(unione.begin(), unione.end());
        unione.erase(std::unique(unione.begin(), unione.end()), unione.end());
        return and_(unione)+(sumAll-and_(r.head));
    }

    double decl_coex_support(act_t a, act_t b, trace_t max_trace_id) {
        std::vector<act_t> unione{{a,b}};
        auto not_a = negate_presence(a, max_trace_id);
        const std::vector<trace_t>& orig = act_to_traces.at(b);
        std::vector<trace_t> res;
        std::set_difference(not_a.begin(), not_a.end(), orig.begin(), orig.end(),
                            std::back_inserter(res));
        return ((double)and_(unione)+res.size())/ ((double)sumAll);
    }

    size_t decl_coex_int_support(act_t a, act_t b, trace_t max_trace_id) {
        std::vector<act_t> unione{{a,b}};
        auto not_a = negate_presence(a, max_trace_id);
        const std::vector<trace_t>& orig = act_to_traces.at(b);
        std::vector<trace_t> res;
        std::set_difference(not_a.begin(), not_a.end(), orig.begin(), orig.end(),
                            std::back_inserter(res));
        return (and_(unione)+res.size());
    }

    double decl_coex_conf(act_t a, act_t b) {
        std::vector<act_t> unione{{a,b}};
        double above = and_(unione);
        double below = or_(unione);
        return ((double)and_(unione))/ ((double)or_(unione));
    }

//    std::unordered_map<Rule<act_t>, double> score_conf;

    /**
     * Rule confidence
     * @param r
     * @return
     */
    double confidence(const Rule<act_t>& r) {
//        auto it = score_conf.emplace(r, 0.0); //memoizing
//        if (it.second) {
            std::vector<act_t> unione;
            for (const auto& x: r.head) unione.emplace_back(x);
            for (const auto& x: r.tail) unione.emplace_back(x);
            std::sort(unione.begin(), unione.end());
            unione.erase(std::unique(unione.begin(), unione.end()), unione.end());
            return ((double)and_(unione)) / ((double) support(r.head));
//        }
//        return it.first->second;
    }

    double lift(const Rule<act_t>& r) const {
        std::vector<act_t> unione;
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
