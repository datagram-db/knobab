/*
 * <filename>
 * This file is part of KnoBAB
 *
 * Copyright (C) 2023 - Samuel 'Buzz' Appleby
 *
 * KnoBAB is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * KnoBAB is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with KnoBAB. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef KNOBAB_SERVER_REFINERY_H
#define KNOBAB_SERVER_REFINERY_H

#include <vector>
#include <yaucl/bpm/structures/commons/DeclareDataAware.h>
#include <knobab/server/query_manager/Environment.h>
#include <yaucl/learning/dt_predicate.h>
#include <unordered_map>

// @author: S Giacomo Bergami
enum RefineOver {
    RefineOverActivation,
    RefineOverTarget,
    RefineOverMatch
};

using worlds_activations = std::unordered_map<Environment*, std::vector<payload_data>>;
using refining_extraction = std::unordered_map<std::vector<Environment*>, std::vector<DeclareDataAware>>;


// @author: Samuel Appleby and Giacomo Bergami
static inline void extractPayload(const Environment *e,
                                  bool hasActivations,
                                  bool hasTargets,
                                  std::vector<std::pair<payload_data, int>> &activations,
                                  std::vector<std::pair<payload_data, int>> &targets,
                                  std::vector<std::pair<payload_data, int>> &correlations,
                                  const Result &result,
                                  int clazz) {
    env envM;
    static std::unordered_set<std::string> toIgnore{"__time"};
    for (const ResultRecord &rec: result) {      // Every trace
        ResultIndex match;
        match.first = rec.first.first;
        std::vector<std::vector<event_t>> act_targ(2, std::vector<event_t>{});
        for (const marked_event &ev: rec.second.second) {        // Every activation/target
            bool isMatch = IS_MARKED_EVENT_MATCH(ev) && (hasActivations && hasTargets);
            if ((IS_MARKED_EVENT_ACTIVATION(ev) && hasActivations) || isMatch) {
                match.second = GET_ACTIVATION_EVENT(ev);
                auto tmp = e->GetPayloadDataFromEvent(match, toIgnore);
                activations.emplace_back(tmp, clazz);
                if (isMatch) {
                    envM.clear();
                    for (const auto&[k,v] : tmp)
                        envM["A."+k] = v;
                } else
                    act_targ[0].emplace_back( GET_ACTIVATION_EVENT(ev));
            }
            if ((IS_MARKED_EVENT_TARGET(ev) && hasTargets) || isMatch) {
                match.second = GET_TARGET_EVENT(ev);
                auto tmp = e->GetPayloadDataFromEvent(match, toIgnore);
                targets.emplace_back(tmp, clazz);
                if (isMatch) {
                    for (const auto&[k,v] : tmp)
                        envM["T."+k] = v;
                    correlations.emplace_back(envM, clazz);
                } else
                    act_targ[1].emplace_back( GET_TARGET_EVENT(ev));
            }
        }
        if ((!act_targ.at(0).empty()) && (!act_targ.at(1).empty())) {
            std::function<void(const std::vector<event_t>&)> f = [&match, &e,&correlations,clazz](const std::vector<event_t>& act_arg) {
                match.second = act_arg.at(0);
                auto envL = e->GetPayloadDataFromEvent(match, toIgnore, "A.");
                match.second = act_arg.at(1);
                auto envR = e->GetPayloadDataFromEvent(match, toIgnore, "T.");
                envL.insert(envR.begin(), envR.end());
                correlations.emplace_back(envL, clazz);
            };
            cartesian_product<event_t>(act_targ, f);
        }
    }
}

static inline union_minimal selector(const payload_data &x, const std::string &key) {
    auto found = x.find(key);
    return found != x.end() ? found->second : 0.0;
};

static inline DeclareDataAware actualClauseRefine(const DeclareDataAware &clause,
                                    const RefineOver &what,
                                    const std::pair<int, std::vector<std::vector<dt_predicate>>> &pair) {
    DeclareDataAware c = clause;

    for(const std::vector<dt_predicate>& cond : pair.second){
        std::unordered_map<std::string, DataPredicate> current_conds;

        for(const dt_predicate& dt_p : cond){
            DataPredicate p;
            switch (what) {
                case RefineOverMatch:{
                    auto isA = dt_p.field.find("A.");
                    if (isA == 0) {
                        // Activation
                        p.label = c.left_act;
                        p.var = dt_p.field.substr(2);
                    } else if ((isA = dt_p.field.find("T.")) == 0) {
                        // Target
                        p.label = c.right_act;
                        p.var = dt_p.field.substr(2);
                    }
                } break;
                case RefineOverActivation:
                {
                    p.label = c.left_act;
                    p.var = dt_p.field;
                } break;
                case RefineOverTarget:
                {
                    p.label = c.right_act;
                    p.var = dt_p.field;
                } break;
            }
            switch (dt_p.pred) {
                case dt_predicate::LEQ_THAN:
                    p.casusu = LEQ;
                    break;
                case dt_predicate::GEQ_THAN:
                    p.casusu = GEQ;
                    break;
                case dt_predicate::IN_SET:
                    p.casusu = IN_SET;
                    break;
                case dt_predicate::NOT_IN_SET:
                    p.casusu = NOT_IN_SET;
                    break;
            }

            if(dt_p.categoric_set.size()){
                p.categoric_set = dt_p.categoric_set;
            }
            else{
                p.value = dt_p.value;
            }

            auto found = current_conds.find(p.var);

            if(found != current_conds.end()){
                // Our path has two conditions on the same var, perform intersection
                p.intersect_with(found->second);
                found->second = p;
            }
            else{
                current_conds.insert({p.var, p});
            }
        }

        switch (what) {
            case RefineOverMatch:{
                c.conjunctive_map.push_back(current_conds);
            } break;
            case RefineOverActivation:
            {
                // Activation conditions
                c.dnf_left_map.push_back(current_conds);
            } break;
            case RefineOverTarget:
            {
                // Activation conditions
                c.dnf_right_map.push_back(current_conds);
            } break;
        }
    }
    return c;
}

#endif //KNOBAB_SERVER_REFINERY_H
