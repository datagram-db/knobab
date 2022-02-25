//
// Created by giacomo on 16/02/2022.
//

#ifndef KNOBAB_LTLF_QUERY_H
#define KNOBAB_LTLF_QUERY_H

#include <vector>
#include <string>
#include <yaucl/bpm/structures/ltlf/ltlf.h>
#include <knobab/predicates/PredicateManager.h>

enum ltlf_query_t {
    Q_TRUE,
    Q_NEXT,
    Q_FALSE,
    Q_ACT,
    Q_AND,
    Q_OR,
    Q_XOR,
    Q_BOX,
    Q_DIAMOND,
    Q_UNTIL,
    Q_RELEASE,
    Q_LAST,

    Q_INIT,
    Q_END,
    Q_EXISTS
};

struct ltlf_query {
    bool isTimed = false;
    bool extractActivationTargetConditions = false;
    ltlf_query_t casusu = Q_TRUE;
    std::vector<ltlf_query*> args;
    std::set<std::string> atom;
    std::set<size_t> partial_results;
    bool hasResult = false;
    size_t result_id = 0;
    PredicateManager joinCondition;
    bool isLeaf = false;
    bool hasPremamentMark, hasTempMark;
    size_t parentMin = std::numeric_limits<size_t>::max(), parentMax = 0, dis = 0;

    size_t currentLayer() const {
        return parentMax + 1;
    }

    void associateDataQueryIdsToFormulaByAtom(const std::string& x, size_t l);

    ltlf_query() = default;
    ltlf_query(const ltlf_query&) = default;
    ltlf_query(ltlf_query&& ) = default;
    ltlf_query& operator=(const ltlf_query&) = default;
    ltlf_query& operator=(ltlf_query&& ) = default;

    bool operator==(const ltlf_query &rhs) const;
    bool operator!=(const ltlf_query &rhs) const;

    std::vector<std::pair<std::pair<uint32_t , uint16_t>, double>> result;
};

namespace std {
    template <> struct hash<ltlf_query> {
        const std::hash<std::string> sh;
        size_t operator()(const ltlf_query& q) const {
            size_t val = (size_t)q.casusu;
            size_t ptr_seed = 3;
            for (const auto& ref : q.args)
                ptr_seed = (size_t)(ref) * 17 + ptr_seed;
            size_t ptr_string = 7;
            for (const auto& ref : q.atom)
                ptr_string = sh(ref) * 13 + ptr_string;
            return ((val * (ptr_string ^ ptr_seed)) << 2) + (q.isTimed ? 1 : 0);
        }
    };
}

#include <map>
#include <knobab/algorithms/atomization_pipeline.h>
#include <cassert>

#include <nlohmann/json.hpp>
#include <magic_enum.hpp>

struct ltlf_query_manager {
    static std::unordered_map<std::pair<ltlf, bool>, ltlf_query*> conversion_map_for_subexpressions;
    static std::map<size_t, std::vector<ltlf_query*>> Q;
    static std::unordered_map<ltlf_query*, size_t> counter;
    static std::vector<ltlf_query*> atomsToDecomposeInUnion;
    static std::set<ltlf_query*> VSet;

    static void generateGraph(std::map<ltlf_query*, std::vector<ltlf_query*>>& ref, ltlf_query*q) {
        auto it = ref.emplace(q, q->args);
        if (it.second) {
            for (const auto& arg : q->args)
                generateGraph(ref, arg);
        }
    }

    static std::string generateGraph() {
        std::map<ltlf_query*, std::vector<ltlf_query*>> ref;
        std::map<ltlf_query*,size_t> layerId;
        if (Q.empty()) return "{nodes: [], edges: []}";
        auto it = Q.begin();
        ref[nullptr] = it->second;
        layerId[nullptr] = 0;
        for (; it != Q.end(); it++) {
            for (const auto& arg : it->second) {
                layerId[arg] = 1+it->first;
                generateGraph(ref, arg);
            }
        }
        nlohmann::json json;
        json["nodes"] = {};
        json["edges"] = {};
        auto& nodes = json["nodes"];
        auto& edges = json["edges"];
        for (const auto& cp : ref) {
            nlohmann::json node;
            node["id"] = cp.first ? (size_t)cp.first : 0;
            node["group"] = layerId[cp.first];
            if ((cp.first) && (cp.first->casusu==Q_ACT)) {
                std::stringstream aa;
                aa << cp.first->atom;
                node["label"] = aa.str();
            } else {
                node["label"] = cp.first ? ((cp.first->isTimed ? "t" : "") + std::string(magic_enum::enum_name(cp.first->casusu))) : "Ensemble";
            }
            nodes.push_back(node);
            for (const auto& out : cp.second) {
                nlohmann::json edge;
                edge["from"] = cp.first ? (size_t)cp.first : 0;
                edge["to"] = (size_t)out;
                edges.push_back(edge);
            }
        }
        return json.dump(4);
    }


    static void clear();
    static ltlf_query*  init1(const std::string& atom, std::unordered_set<std::string>& predicates);
    static ltlf_query* end1(const std::string &atom, std::unordered_set<std::string> &predicates);
    static ltlf_query* exists1(const std::string &atom, std::unordered_set<std::string> &predicates);

    static ltlf_query *
    immediateQueries(const std::string &atom, std::unordered_set<std::string> &predicates, const std::string &prefix,
                     const ltlf_query_t &casus);

    static void finalize_unions(const std::vector<ltlf_query*>& W);

    static ltlf_query*  simplify(const ltlf& expr,  bool isTimed = false, bool insert = true);

private:
};

std::ostream & human_readable_ltlf_printing(std::ostream &os, const ltlf_query* syntax);

#endif //KNOBAB_LTLF_QUERY_H
