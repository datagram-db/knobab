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
    Q_END
};

struct ltlf_query {
    bool isTimed = false;
    bool extractActivationTargetConditions = false;
    ltlf_query_t casusu = Q_TRUE;
    std::vector<ltlf_query*> args;
    std::unordered_set<std::string> atom;
    std::vector<size_t> partial_results;
    bool hasResult = false;
    size_t result_id = 0;
    PredicateManager joinCondition;

    void associateDataQueryIdsToFormulaByAtom(const std::string& x, const std::vector<size_t>& l) {
        if (atom.contains(x)) {
            //assert(args.empty());
            partial_results.insert(partial_results.begin(), l.begin(), l.end());
        } else for (auto& child : args)
            child->associateDataQueryIdsToFormulaByAtom(x, l);
    }

    ltlf_query() = default;
    ltlf_query(const ltlf_query&) = default;
    ltlf_query(ltlf_query&& ) = default;
    ltlf_query& operator=(const ltlf_query&) = default;
    ltlf_query& operator=(ltlf_query&& ) = default;

    bool operator==(const ltlf_query &rhs) const;
    bool operator!=(const ltlf_query &rhs) const;
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


struct ltlf_query_manager {
    static std::unordered_map<std::pair<ltlf, bool>, std::pair<ltlf_query*, size_t>> conversion_map_for_subexpressions;
    static std::map<size_t, std::vector<ltlf_query*>> Q;
    static std::unordered_map<ltlf_query*, size_t> counter;

    static void clear();
    static ltlf_query*  init1(const std::string& atom, std::unordered_set<std::string>& predicates);
    static ltlf_query* end1(const std::string &atom, std::unordered_set<std::string> &predicates) {
        return immediateQueries(atom, predicates, "@declare_end1_", Q_END);
    }

    static ltlf_query *
    immediateQueries(const std::string &atom, std::unordered_set<std::string> &predicates, const std::string &prefix,
                     const ltlf_query_t &casus);

    static ltlf_query*  simplify(const ltlf& expr) {
        return _simplify(expr, false).first;
    }

private:
    static std::pair<ltlf_query*, size_t>  _simplify(const ltlf& expr,  bool isTimed = false);
};

std::ostream & human_readable_ltlf_printing(std::ostream &os, const ltlf_query* syntax);

#endif //KNOBAB_LTLF_QUERY_H
