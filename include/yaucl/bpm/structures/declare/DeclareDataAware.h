/*
 * DeclareDataAware.h
 * This file is part of bpm21
 *
 * Copyright (C) 2021 - Giacomo Bergami
 *
 * bpm21 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * bpm21 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bpm21. If not, see <http://www.gnu.org/licenses/>.
 */
//
// Created by giacomo on 10/03/21.
//

#ifndef BPM21_DECLAREDATAAWARE_H
#define BPM21_DECLAREDATAAWARE_H

/**
 * Definition of all the types of Declare statements

enum declare_templates {
    End,
    Init,
    Existence,
    //Absence2,
    Absence,
    Choice,
    ExlChoice,
    RespExistence,
    CoExistence,
    Response,
    Precedence,
    Succession,
    AltResponse,
    AltPrecedence,
    AltSuccession,
    ChainResponse,
    ChainPrecedence,
    ChainSuccession,
    NotCoExistence,
    NegSuccession,
    NegChainSuccession
}; */
#include <string>
using declare_templates = std::string;

bool isUnaryPredicate(const std::string& type);
bool isPredicateNegative(const std::string& type);

//bool isUnaryPredicate(declare_templates t);

#include <string>
#include <unordered_map>
#include <vector>
#include <yaucl/bpm/structures/commons/DataPredicate.h>
#include <yaucl/hashing/pair_hash.h>
#include <yaucl/hashing/uset_hash.h>
#include <yaucl/hashing/vector_hash.h>
#include <yaucl/hashing/hash_combine.h>
#include <ostream>
#include <yaucl/bpm/structures/ltlf/ltlf.h>

void print_conj(std::ostream &os, const std::unordered_map<std::string, DataPredicate>& map);
void print_dnf(std::ostream &os, const std::vector<std::unordered_map<std::string, DataPredicate>>& map);

ltlf map_conj(const std::unordered_map<std::string, DataPredicate> &map);
ltlf map_disj(const std::vector<std::unordered_map<std::string, DataPredicate>> &map);

struct DeclareDataAware {
    declare_templates casusu;
    size_t n;
    std::string left_act, right_act;

    // Each map represents a conjunction among different atoms over distinct variables, while the vector represents the disjunction
    std::vector<std::unordered_map<std::string, DataPredicate>> dnf_left_map, dnf_right_map, conjunctive_map;
    std::unordered_set<std::string> left_decomposed_atoms, right_decomposed_atoms;

    DEFAULT_CONSTRUCTORS(DeclareDataAware)
    static DeclareDataAware unary(const declare_templates&, const std::string& argument, size_t n);
    static DeclareDataAware binary(const declare_templates& t, const std::string& left, const std::string right);

    static DeclareDataAware doExistence(size_t n, const std::string& left_act, const std::vector<std::unordered_map<std::string, DataPredicate>>& dnf_left_map);
    static DeclareDataAware doAbsence(size_t n, const std::string& left_act, const std::vector<std::unordered_map<std::string, DataPredicate>>& dnf_left_map);

    static DeclareDataAware parse_declare_non_data_string(const std::string& line);
    static std::vector<DeclareDataAware> load_simplified_declare_model(std::istream &file);
    friend std::ostream &operator<<(std::ostream &os, const DeclareDataAware &aware);
    bool operator==(const DeclareDataAware &rhs) const;
    bool operator!=(const DeclareDataAware &rhs) const;


    ltlf toFiniteSemantics(bool isForGraph = true) const;

    template <typename Lambda>
    std::unordered_set<std::string> collectLeftAttributes(Lambda outResult,
                               bool collectFromLeftMap = true,
                               bool collectFromJoinMap = true) const {
        std::unordered_set<std::string> forJoin;
        if (collectFromLeftMap) {
            for (const auto& ref : dnf_left_map) {
                for (const auto& cp: ref){
                    if ((cp.second.casusu  != TTRUE) && (!cp.second.var.empty()))
                        outResult(cp.second.var);
                    for (const auto& elem : cp.second.BiVariableConditions)
                        if ((elem.casusu  != TTRUE) && (!elem.var.empty()))
                            outResult(elem.var);
                }
            }
        }
        for (const auto& ref : conjunctive_map)
            for (const auto& cp: ref) {
                if ((cp.second.casusu  != TTRUE) && (!cp.second.var.empty())) {
                    if (collectFromJoinMap)
                        outResult(cp.second.var);
                    forJoin.insert(cp.second.var);
                }
                for (const auto& elem : cp.second.BiVariableConditions) {
                    if ((elem.casusu  != TTRUE) && (!elem.var.empty())) {
                        if (collectFromJoinMap)
                            outResult(elem.var);
                        forJoin.insert(elem.var);
                    }
                }
            }
        return forJoin;
    }

    template <typename Lambda> std::unordered_set<std::string>
    collectRightAttributes(Lambda outResult, bool collectFromRightMap = true,
                                             bool collectFromJoinMap = true) const {
        std::unordered_set<std::string> forJoin;
        if (collectFromRightMap) {
            for (const auto& ref : dnf_right_map)
                for (const auto& cp: ref) {
                    if ((cp.second.casusu  != TTRUE) && (!cp.second.var.empty()))
                        outResult(cp.second.var);
                    for (const auto& elem : cp.second.BiVariableConditions)
                        if ((elem.casusu  != TTRUE) && (!elem.var.empty()))
                            outResult(elem.var);
                }
        }
        {
            for (const auto& ref : conjunctive_map)
                for (const auto& cp: ref) {
                    if ((cp.second.casusu != TTRUE) && (!cp.second.varRHS.empty())) {
                        if (collectFromJoinMap)
                            outResult(cp.second.varRHS);
                        forJoin.insert(cp.second.varRHS);
                    }
                    for (const auto& elem : cp.second.BiVariableConditions) {
                        if ((elem.casusu != TTRUE) && (!elem.varRHS.empty())) {
                            if (collectFromJoinMap)
                                outResult(elem.varRHS);
                            forJoin.insert(elem.varRHS);
                        }
                    }
                }
        }
        return forJoin;
    }





};

//ltlf toFiniteSemantics(declare_templates casusu, size_t n, const std::string& left_act, const std::string& right_act = "");

namespace std {
    template <>
    struct hash<DeclareDataAware> {
        std::size_t operator()(const DeclareDataAware& k) const {
            using yaucl::hashing::hash_combine;
            size_t seed = 31;
            seed = hash_combine(seed, k.casusu);
            seed = hash_combine(seed, k.n);
            seed = hash_combine(seed, k.left_act);
            //seed = hash_combine(seed, k.dnf_left_map); --> TODO
            seed = hash_combine(seed, k.right_act);
            //seed = hash_combine(seed, k.dnf_right_map); --> TODO
            return seed;
        }
    };

}

#endif //BPM21_DECLAREDATAAWARE_H
