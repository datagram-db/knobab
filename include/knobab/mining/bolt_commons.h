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
#include "yaucl/learning/dt_predicate.h"
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
}
