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

#include <yaucl/functional/assert.h>

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
//#include <yaucl/bpm/structures/ltlf/ltlf.h>

/**
 * @author Samuel 'Sam' Appleby
 */
using env = std::unordered_map<std::string, union_minimal>;
using payload_data = env;

/**
 *
 * Imported from the 'SimpleDataPredicate'
 * @author Samuel 'Sam' Appleby
 *
 * @param e1
 * @param e2
 * @param lhs
 * @param rhs
 * @param casusu
 * @return
 */
inline bool test_decomposed_data_predicate(const env& e1, const env& e2, const std::string& lhs, const std::string& rhs, const numeric_atom_cases& casusu) {
    union_minimal lhsV = 0.0, rhsV = 0.0;
    auto temp1 = e1.find(lhs), temp2 = e2.find(rhs);
    if (temp1 != e1.end()) lhsV = temp1->second;
    if (temp2 != e2.end()) rhsV = temp2->second;
//    if((temp1 == e1.end()) || (temp2 == e2.end())){
//        return false;
//    }

    switch (casusu) {
        case LT:
            return lhsV < rhsV;
        case LEQ:
            return lhsV <= rhsV;
        case GT:
            return lhsV > rhsV;
        case GEQ:
            return lhsV >= rhsV;
        case EQ:
            return lhsV == rhsV;
        case NEQ:
            return lhsV != rhsV;
        case TTRUE:
            return true;
        case INTERVAL:
            DEBUG_ASSERT(false);
        default:
            return false;

    }
}


void print_conj(std::ostream &os, const std::unordered_map<std::string, DataPredicate>& map);
void print_dnf(std::ostream &os, const std::vector<std::unordered_map<std::string, DataPredicate>>& map);
//ltlf map_conj(const std::unordered_map<std::string, DataPredicate> &map);
//ltlf map_disj(const std::vector<std::unordered_map<std::string, DataPredicate>> &map);
class KnowledgeBase;

struct FastDatalessClause {
    std::string casusu, left, right;
    size_t n = 1;

    DEFAULT_CONSTRUCTORS(FastDatalessClause)
    FastDatalessClause(const std::string& cas, const std::string& L, const std::string& R, size_t n) : casusu{cas}, left{L}, right{R}, n{n} {};
    friend std::ostream &operator<<(std::ostream &os, const FastDatalessClause &aware);
    bool operator==(const FastDatalessClause &rhs) const;
    bool operator!=(const FastDatalessClause &rhs) const;
    friend bool operator<(const FastDatalessClause& l, const FastDatalessClause& r) {
        if (l.casusu < r.casusu) return true;
        if (l.casusu > r.casusu) return false;
        if (l.left < r.left) return true;
        if (l.left > r.left) return false;
        if (l.right < r.right) return true;
        if (l.right > r.right) return false;
        return l.n < r.n;
    }
};

struct DeclareDataAware {
    declare_templates casusu;
    size_t n;
    std::string left_act, right_act;
//    uint16_t left_act_id = 0, right_act_id = 0;
    const KnowledgeBase* kb = nullptr;
    bool isFlippedComputed = false;
    bool isOriginal = true;
    DeclareDataAware* flipped_equivalent = nullptr;

    // Each map represents a conjunction among different atoms over distinct variables, while the vector represents the disjunction
    std::vector<std::unordered_map<std::string, DataPredicate>> dnf_left_map, dnf_right_map, conjunctive_map;
    std::unordered_set<std::string> left_decomposed_atoms, right_decomposed_atoms; /// TODO: generalization to the number of the possible arguments of a declare clause

    bool compareAsThetaPredicate(const struct DeclareDataAware* ptr) const {
        if (ptr == nullptr) return false;
        return n == ptr->n
                && left_act == ptr->left_act && ptr->dnf_left_map == dnf_left_map
//                && left_act_id == ptr->left_act_id
                && right_act == ptr->right_act && ptr->dnf_right_map == dnf_right_map
//                && right_act_id == ptr->right_act_id
                && conjunctive_map == ptr->conjunctive_map;
    }

    DEFAULT_CONSTRUCTORS(DeclareDataAware)
    DeclareDataAware(FastDatalessClause& clause) : n{clause.n}, casusu{clause.casusu}, left_act(clause.left), right_act(clause.right) {}
    DeclareDataAware(const std::vector<std::vector<DataPredicate>>& predicate, const KnowledgeBase* kb);
    ~DeclareDataAware() {
        if (isOriginal && isFlippedComputed && flipped_equivalent) {
            delete flipped_equivalent;
            isFlippedComputed = false;
            flipped_equivalent = nullptr;
        }
    }

    static DeclareDataAware unary(const declare_templates&, const std::string& argument, size_t n = 1);
    static DeclareDataAware binary(const declare_templates& t, const std::string& left, const std::string right);
    static DeclareDataAware doExistence(size_t n, const std::string& left_act, const std::vector<std::unordered_map<std::string, DataPredicate>>& dnf_left_map);
    static DeclareDataAware doAbsence(size_t n, const std::string& left_act, const std::vector<std::unordered_map<std::string, DataPredicate>>& dnf_left_map);
    static DeclareDataAware unary_for_testing(const declare_templates& t, const std::string &argument, size_t n);
    static DeclareDataAware binary_for_testing(const declare_templates& t, const std::string &left, const std::string& right);

    static DeclareDataAware parse_declare_non_data_string(const std::string& line);
    static std::vector<DeclareDataAware> load_simplified_declare_model(std::istream &file);
    friend std::ostream &operator<<(std::ostream &os, const DeclareDataAware &aware);
    bool operator==(const DeclareDataAware &rhs) const;
    bool operator!=(const DeclareDataAware &rhs) const;

    /**
     * Imported from the 'PredicateManager'
     * @author Samuel 'Sam' Appleby
     *
     * @param e1
     * @param e2
     * @return
     */
    bool checkValidity(const env &e1, const env &e2) const;
    bool checkValidity(const env &e1, uint32_t t2, uint16_t e2) const;

    /**
     * @author Samuel 'Sam' Appleby
     * @return
     */
    bool isTruth() const {
        return conjunctive_map.empty();
    }

    /**
     * Imported from the 'PredicateManager'
     * @author Samuel 'Sam' Appleby
     *
     * @param pair
     * @return
     */
    std::vector<env> GetPayloadDataFromEvent(const std::pair<uint32_t , uint16_t>& pair) const;
    std::vector<env> GetPayloadDataFromEvent(uint32_t first, uint16_t second, bool isLeft, std::unordered_set<std::string>& leftArgs) const;

    DeclareDataAware flip() const;
    DeclareDataAware* flipLocal();

    //[[deprecated]] ltlf toFiniteSemantics(bool isForGraph = true) const;

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

    friend bool operator<(const DeclareDataAware& l, const DeclareDataAware& r)
    {
        return std::tie(l.casusu, l.left_act, /*l.left_act_id,*/ l.right_act, /*l.right_act_id,*/ l.n) < std::tie(r.casusu, r.left_act, /*r.left_act_id,*/ r.right_act, /*r.right_act_id,*/ r.n); // keep the same order
    }

//    bool checkValidity(bool isLeftMap, uint32_t t2, uint16_t e2) const;
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
//            seed = hash_combine(seed, k.left_act_id);
            //seed = hash_combine(seed, k.dnf_left_map); --> TODO
            seed = hash_combine(seed, k.right_act);
//            seed = hash_combine(seed, k.right_act_id);
            //seed = hash_combine(seed, k.dnf_right_map); --> TODO
            return seed;
        }
    };

}

namespace std {
    template <>
    struct hash<FastDatalessClause> {
        std::size_t operator()(const FastDatalessClause& k) const {
            using yaucl::hashing::hash_combine;
            size_t seed = 31;
            seed = hash_combine(seed, k.casusu);
            seed = hash_combine(seed, k.n);
            seed = hash_combine(seed, k.left);
            seed = hash_combine(seed, k.right);
            return seed;
        }
    };

}

using PredicateManager = DeclareDataAware;

#endif //BPM21_DECLAREDATAAWARE_H
