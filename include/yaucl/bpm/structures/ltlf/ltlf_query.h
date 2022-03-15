//
// Created by giacomo on 16/02/2022.
//

#ifndef KNOBAB_LTLF_QUERY_H
#define KNOBAB_LTLF_QUERY_H

#if 0
#include <vector>
#include <string>
#include <yaucl/bpm/structures/ltlf/ltlf.h>
#include <knobab/predicates/PredicateManager.h>
#include <knobab/operators/LTLFOperators.h>

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
    Q_LAST, // TODO: atomless

    Q_INIT,
    Q_END,      // With an atom
    Q_EXISTS,
    Q_ABSENCE
};

#include <knobab/operators/semantics.h>

struct ltlf_query {
    bool isTimed = false;
    ltlf_query_t casusu = Q_TRUE;
    std::vector<ltlf_query*> args;
    std::set<std::string> atom;
    std::set<size_t> partial_results;
    size_t result_id = 0;
    PredicateManager joinCondition;
    LeafType isLeaf = NotALeaf;
    size_t numeric_arg = 0;
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

    Result result;
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
#endif

#include <knobab/queries/LTLfQuery.h>
#include <unordered_map>

struct ltlf_query_manager {
    std::unordered_map<LTLfQuery, LTLfQuery*> conversion_map_for_subexpressions;
    std::map<size_t, std::vector<LTLfQuery*>> Q;
    std::unordered_map<LTLfQuery*, size_t> counter;
    std::vector<LTLfQuery*> atomsToDecomposeInUnion;
    //std::set<ltlf_query*> VSet;

    void generateGraph(std::map<LTLfQuery*, std::vector<LTLfQuery*>>& ref, LTLfQuery*q) const;
    std::string generateGraph() const;

    void clear();

    LTLfQuery* simplify(const LTLfQuery& q);
    LTLfQuery *simplify(size_t formulaId,
                        const LTLfQuery &input,
                        const DeclareDataAware *joinCondition,
                        const std::unordered_set<std::string> &atom_universe,
                        const std::unordered_set<std::string> &left,
                        const std::unordered_set<std::string> &right,
                        std::vector<std::string> &toUseAtoms,
                        std::unordered_map<std::string , std::vector<size_t>>& atomToFormulaId);

    void finalize_unions(const std::vector<LTLfQuery*>& W, KnowledgeBase* ptr);

#if 0
    ltlf_query*  init1(const std::string& atom, std::unordered_set<std::string>& predicates);
    ltlf_query* end1(const std::string &atom, std::unordered_set<std::string> &predicates);
    ltlf_query *exists(const std::string &atom, std::unordered_set<std::string> &predicates, size_t atLeast);
    ltlf_query *absence(const std::string &atom, std::unordered_set<std::string> &predicates, size_t atLeast);

    ltlf_query *
    immediateQueries(const std::string &atom, std::unordered_set<std::string> &predicates,
                     const std::string &prefix, const ltlf_query_t &casus,
                     size_t numeric_arg);


    ltlf_query*  simplify(const ltlf& expr,  bool isTimed = false, KnowledgeBase* ptr = nullptr);

    ltlf_query *getQuerySemiInstantiated(const std::vector<std::string> &expr, bool isTimed,
                                         ltlf_query *result, bool areArgsTimed, const std::vector<ltlf> *ARGS,
                                         const ltlf_query_t &casusu,
                                         const std::vector<std::unordered_map<std::string, DataPredicate>> *joinCondition,
                                         bool isAct,
                                         KnowledgeBase* ptr);
#endif
};

//std::ostream & human_readable_ltlf_printing(std::ostream &os, const ltlf_query* syntax);

#endif //KNOBAB_LTLF_QUERY_H
