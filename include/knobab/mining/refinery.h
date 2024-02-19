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
#include "yaucl/learning/decision_tree/dt_predicate.h"
#include <unordered_map>

// @author: S Giacomo Bergami
enum RefineOver {
    RefineOverActivation,
    RefineOverTarget,
    RefineOverMatch
};

struct ForResultReference {
    RefineOver  type_for_single;
    std::string world;
    std::vector<std::pair<trace_t,event_t>> result;
    DEFAULT_CONSTRUCTORS(ForResultReference)

    ForResultReference(bool isActivation, const std::string& db, trace_t t, event_t e) : type_for_single{isActivation ? RefineOverActivation : RefineOverTarget}, world{db} {
        result.emplace_back(t, e);
    }
    ForResultReference(const std::string& db, trace_t tA, event_t eA, trace_t tT, event_t eT) : type_for_single{RefineOverMatch}, world{db} {
        result.emplace_back(tA, eA);
        result.emplace_back(tT, eT);
    }
};

using worlds_activations = std::unordered_map<Environment*, std::vector<payload_data>>;
using refining_extraction = std::unordered_map<std::vector<Environment*>, std::vector<DeclareDataAware>>;




// @author: Samuel Appleby and Giacomo Bergami
static inline void extractPayload(size_t leftAct, size_t rightAct,
                                  const Environment *e,
const std::string& envName,
                                  bool hasActivations,
                                  bool hasTargets,
                                  std::vector<std::pair<ForResultReference, int>> &activations,
                                  std::vector<std::pair<ForResultReference, int>> &targets,
                                  std::vector<std::pair<ForResultReference, int>> &correlations,
//                                  std::vector<std::pair<payload_data, int>> &activations,
//                                  std::vector<std::pair<payload_data, int>> &targets,
//                                  std::vector<std::pair<payload_data, int>> &correlations,
                                  const Result &result,
                                  int clazz) {
    ForResultReference ref(envName, 0,0,0,0);
    static std::unordered_set<std::string> toIgnore{"__time"};
    for (const ResultRecord &rec: result) {      // Every trace
        ResultIndex match;
        ref.result[0].first = ref.result[1].first = match.first = rec.first.first;
        const auto& thisRef =
                e->db.act_table_by_act_id.getBuilder().trace_id_to_event_id_to_offset.at(match.first);
        std::vector<std::vector<event_t>> act_targ(2, std::vector<event_t>{});
        for (const marked_event &ev: rec.second.second) {        // Every activation/target
            bool isMatch = IS_MARKED_EVENT_MATCH(ev) && (hasActivations && hasTargets);
            if ((IS_MARKED_EVENT_ACTIVATION(ev) && hasActivations) || isMatch) {
                match.second = GET_ACTIVATION_EVENT(ev);
                auto it = thisRef.at(match.second).find(leftAct);
                if (it != thisRef.at(match.second).end()) {
                    for (size_t id : it->second) {
                        DEBUG_ASSERT(e->db.act_table_by_act_id.table.at(id).entry.id.parts.act == leftAct);
                        activations.emplace_back(/*tmp, clazz*/ ForResultReference{true, envName, match.first, match.second}, clazz);
                        if (isMatch) {
                            ref.result[0] = match;
//                        envM.clear();
//                        for (const auto&[k,v] : tmp)
//                            envM["A."+k] = v;
                        } else
                            act_targ[0].emplace_back( GET_ACTIVATION_EVENT(ev));
                    }
//                    if (e->db.act_table_by_act_id.table.at(thisRef.at(match.second)).entry.id.parts.act == leftAct) {
////                    auto tmp = e->GetPayloadDataFromEvent(match, toIgnore);
//                        activations.emplace_back(/*tmp, clazz*/ ForResultReference{true, envName, match.first, match.second}, clazz);
//                        if (isMatch) {
//                            ref.result[0] = match;
////                        envM.clear();
////                        for (const auto&[k,v] : tmp)
////                            envM["A."+k] = v;
//                        } else
//                            act_targ[0].emplace_back( GET_ACTIVATION_EVENT(ev));
//                    }
                }


            }
            if ((IS_MARKED_EVENT_TARGET(ev) && hasTargets) || isMatch) {
                match.second = GET_TARGET_EVENT(ev);
                auto it = thisRef.at(match.second).find(rightAct);
                if (it != thisRef.at(match.second).end()) {
                    for (size_t id : it->second) {
                        DEBUG_ASSERT(e->db.act_table_by_act_id.table.at(id).entry.id.parts.act == rightAct);
                        targets.emplace_back(/*tmp, clazz*/ ForResultReference{true, envName, match.first, match.second}, clazz);
                        if (isMatch) {
                            ref.result[1] = match;
//                        for (const auto&[k,v] : tmp)
//                            envM["T."+k] = v;
                            correlations.emplace_back(/*envM*/ ref, clazz);
                        } else
                            act_targ[1].emplace_back( GET_TARGET_EVENT(ev));
                    }
                }
//                if (e->db.act_table_by_act_id.table.at(thisRef.at(match.second)).entry.id.parts.act == rightAct) {
////                    auto tmp = e->GetPayloadDataFromEvent(match, toIgnore);
//                    targets.emplace_back(/*tmp, clazz*/ ForResultReference{true, envName, match.first, match.second}, clazz);
//                    if (isMatch) {
//                        ref.result[1] = match;
////                        for (const auto&[k,v] : tmp)
////                            envM["T."+k] = v;
//                        correlations.emplace_back(/*envM*/ ref, clazz);
//                    } else
//                        act_targ[1].emplace_back( GET_TARGET_EVENT(ev));
//                }
            }
        }
        if ((!act_targ.at(0).empty()) && (!act_targ.at(1).empty())) {
            std::function<void(const std::vector<event_t>&)> f = [&match, &e,&ref,&correlations,clazz](const std::vector<event_t>& act_arg) {
//                match.second = act_arg.at(0);
//                auto envL = e->GetPayloadDataFromEvent(match, toIgnore, "A.");
//                match.second = act_arg.at(1);
//                auto envR = e->GetPayloadDataFromEvent(match, toIgnore, "T.");
//                envL.insert(envR.begin(), envR.end());
                ref.result[0].second = act_arg.at(0);
                ref.result[1].second =act_arg.at(1);
                        correlations.emplace_back(/*envL*/ ref, clazz);
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
                                    const std::pair<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> &pair) {
    DeclareDataAware c = clause;

    for(const std::pair<double,std::vector<dt_predicate>>& cond : pair.second){
        std::unordered_map<std::string, DataPredicate> current_conds;
        bool hasAFalse = false;

        for(const dt_predicate& dt_p : cond.second){
            DataPredicate p;
            switch (what) {
                case RefineOverMatch:{
                    if (dt_p.field.find("A.") == 0) {
                        // Activation
                        p.is_left_for_activation = true;
                        p.label = c.left_act;
                        p.var = dt_p.field.substr(2);
                    } else if ((dt_p.field.find("T.")) == 0) {
                        // Target
                        p.is_left_for_activation = false;
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
                case dt_predicate::G_THAN:
                    p.casusu = GT;
                    break;
                case dt_predicate::GEQ_THAN:
                    p.casusu = GEQ;
                    break;
                case dt_predicate::IN_SET:
                    p.casusu = EQ;
                    // TODO: express in-set predicates even in the atomisation pipeline
                    break;
                case dt_predicate::NOT_IN_SET:
                    p.casusu = NEQ;
                    // TODO: express in-set predicates even in the atomisation pipeline
                    break;
            }

            if(!dt_p.categoric_set.empty()){
                DEBUG_ASSERT(dt_p.categoric_set.size() == 1);
                p.value = *dt_p.categoric_set.begin();
            } else{
                p.value = dt_p.value;
            }

            auto found = current_conds.find(p.var);

            if(found != current_conds.end()){
                // Our path has two conditions on the same var, perform intersection
//                p.intersect_with(found->second);
                if (!found->second.intersect_with(p)) {
                    found->second.casusu = FFALSE;
                    hasAFalse = true;
                    break;
                }
            }
            else{
                current_conds.insert({p.var, p});
            }
        }
        if (hasAFalse) {
            current_conds.clear();
        } else {
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
    }
    return c;
}

#endif //KNOBAB_SERVER_REFINERY_H
