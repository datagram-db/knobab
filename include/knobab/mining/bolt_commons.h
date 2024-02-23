/*
 * bolt_commons.h
 * This file is part of knobab
 *
 * Copyright (C) 2023 - Giacomo Bergami
 *
 * knobab is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * knobab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with knobab. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <vector>
#include "knobab/server/tables/KnowledgeBase.h"
#include "knobab/server/declare/DeclareDataAware.h"
#include <knobab/mining/apriori.h>
//#include "knobab/algorithms/mining/pattern_mining.h"
#include "yaucl/learning/decision_tree/dt_predicate.h"
#include "yaucl/learning/DecisionTree.h"
#include <chrono>
#include <knobab/server/query_manager/Environment.h>
#include "roaring64map.hh"
#include "knobab/mining/refinery.h"
#include "knobab/server/dataStructures/marked_event.h"
#include <roaring/roaring_array.h>

static inline void decrease_support_X(const KnowledgeBase &kb,
                                      size_t expected_support,
                                      bool& alles_X,
                                      size_t& alles_not_X) {
    if (alles_X) {
        alles_not_X++;
        if ((kb.nTraces() - alles_not_X) < expected_support) {
            alles_X = false;
        }
    }
}

static inline void fast_forward_equals(trace_t trace_id,
                                       ActTable::record*& to_increment,
                                       ActTable::record *&end) {
    while ((to_increment != end) &&
           (to_increment->entry.id.parts.trace_id == trace_id)) {
        to_increment++;
    }
}

static inline void fast_forward_lower(trace_t trace_id,
                                      ActTable::record*& to_increment,
                                      ActTable::record *&end) {
    while ((to_increment != end) &&
           (to_increment->entry.id.parts.trace_id < trace_id)) {
        to_increment++;
    }
    /*    size_t skip_counter = 0;
    size_t prev_trace_id = -1;
    bool first = true;
    while ((to_increment != end) &&
           (to_increment->entry.id.parts.trace_id < trace_id)) {
        if (first) {
            prev_trace_id = to_increment->entry.id.parts.trace_id;
            first = false;
        } else {
            size_t diff = to_increment->entry.id.parts.trace_id-prev_trace_id;
            if (diff) {
                prev_trace_id = to_increment->entry.id.parts.trace_id;
                if ((diff)>1) {
                    skip_counter+=(diff-1);
                }
            }
        }
        to_increment++;
    }
    size_t diff = trace_id-prev_trace_id;
    if (diff) {
        if ((diff)>1) {
            skip_counter+=(diff-1);
        }
    }*/
}

struct retain_choice {
    //roaring::Roaring map;
    std::map<double, std::unordered_set<pattern_mining_result<FastDatalessClause>>> maps;
};


static inline bool
choice_exclchoice(act_t a, act_t b,
                  size_t log_size,
                  uint64_t minimum_support_threshold,
                  const KnowledgeBase& kb,
                  std::vector<std::vector<trace_t>>& inv_map,
                  std::unordered_map<act_t, retain_choice>& map_for_retain,
                  std::unordered_map<std::unordered_set<act_t>, uint64_t>& mapper) {
    const std::unordered_set<act_t> lS{a,b};
//    curr_pair.second = inv_pair.first = b;
//    if (unary && ((!visited_pairs.emplace(curr_pair).second) ||
//        (!visited_pairs.emplace(inv_pair).second))) return false;
    const auto& aSet = inv_map.at(a);
    const auto& bSet = inv_map.at(b);
    std::pair<size_t, size_t> ratio = yaucl::iterators::ratio(aSet.begin(), aSet.end(), bSet.begin(), bSet.end());
    double local_support = ((double)(ratio.first)) / ((double)log_size);
    static FastDatalessClause clause;

//    clause.right_act_id = b;

    if (ratio.first >= minimum_support_threshold) {
//        static pattern_mining_result<FastDatalessClause> c(clause,  0.0, local_support, local_support, local_support);

        if (a>b) {
            std::swap(a,b);
//        std::swap(clause.left_act_id, clause.right_act_id);
        }
        clause.left = kb.event_label_mapper.get(a);
//    clause.left_act_id = a;
        clause.right = kb.event_label_mapper.get(b);
//        c.confidence_declarative_pattern = c.support_declarative_pattern = c.restrictive_support_declarative_pattern = local_support;
        clause.n = 1;

        // I can consider this pattern, again, only if it is within the expected
        // support rate which, in this case, is given by the amount of traces
        // globally setting up this pattern
        auto it = mapper.find(lS);
        if (ratio.second == 0) {
            // If there is no intersection, I can also be more strict if I want,
            // and provide an exclusive choice pattern if I am confident that
            // the two events will never appear in the same trace (according to
            // the "training" data
            clause.casusu = "ExclChoice";
        } else {
            clause.casusu = "Choice";
        }
        auto& refA = map_for_retain[a];
//        auto& refB = map_for_retain[b];
//        refA.map.add(b);
//        refB.map.add(a);
        /*if (it != mapper.end())
            c.support_generating_original_pattern =  ((double)it->second)/((double)log_size);
        else
            c.support_generating_original_pattern = 0;*/
//        std::cout << c << std::endl;
        refA.maps[local_support].emplace(clause, (it != mapper.end())?((double)it->second)/((double)log_size):0.0, local_support, local_support, local_support);
//        refB.maps[local_support].emplace(c);
        return true;
    }

    return false;
}