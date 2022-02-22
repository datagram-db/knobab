//
// Created by giacomo on 16/02/2022.
//

#include "yaucl/bpm/structures/ltlf/ltlf_query.h"

static inline void topological_sort(const std::vector<ltlf_query*>& W,
                                    std::vector<ltlf_query*>& vertexOreder) {
    std::unordered_set<ltlf_query*> toVisit{W.begin(), W.end()};
    std::stack<std::pair<bool, ltlf_query*>> internalStack;
    std::unordered_set<ltlf_query*> visited;
    //auto isEnd = adjMap.end();
    while (!toVisit.empty()) {
        auto first = toVisit.begin();
        if (first != toVisit.cend()) {
            internalStack.push(std::make_pair(false, *first));
        }
        while (!internalStack.empty()) {
            std::pair<bool, ltlf_query*>  cp = internalStack.top();
            internalStack.pop();
            toVisit.erase(cp.second);
            if (cp.first) {
                vertexOreder.emplace_back(cp.second);
            } else {
                if (visited.insert(cp.second).second) {
                    internalStack.push((cp));
                    internalStack.push(std::make_pair(true, cp.second));
                    //auto out = adjMap.find(cp.second);
                    for (auto & i : cp.second->args) {
                        internalStack.push(std::make_pair(false, i));
                    }
                }
            }
        }
    }
    std::reverse(vertexOreder.begin(), vertexOreder.end());
}

bool ltlf_query::operator==(const ltlf_query &rhs) const {
    return isTimed == rhs.isTimed &&
           casusu == rhs.casusu &&
           args == rhs.args &&
           atom == rhs.atom;
}

bool ltlf_query::operator!=(const ltlf_query &rhs) const {
    return !(rhs == *this);
}

void ltlf_query::associateDataQueryIdsToFormulaByAtom(const std::string &x, size_t l) {
    if (atom.contains(x)) {
        //assert(args.empty());
        partial_results.emplace(l);
    } else for (auto& child : args)
            child->associateDataQueryIdsToFormulaByAtom(x, l);
}

#include <cassert>
#include <magic_enum.hpp>




std::unordered_map<std::pair<ltlf, bool>, ltlf_query*> ltlf_query_manager::conversion_map_for_subexpressions;
std::unordered_map<ltlf_query*, size_t> ltlf_query_manager::counter;
std::map<size_t, std::vector<ltlf_query*>> ltlf_query_manager::Q;
std::vector<ltlf_query*> ltlf_query_manager::atomsToDecomposeInUnion;
std::set<ltlf_query*> ltlf_query_manager::VSet;

ltlf_query* ltlf_query_manager::simplify(const ltlf& expr,  bool isTimed, bool insert) {
    assert((expr.casusu != NEG_OF) && (expr.casusu != NUMERIC_ATOM));
    std::pair<ltlf, bool> q{expr, isTimed};
    auto it = conversion_map_for_subexpressions.find(q);
    if (it != conversion_map_for_subexpressions.end()) {
        counter[it->second]++;
        return it->second;
    } else {
        ltlf_query* result = new ltlf_query();
        if (insert) VSet.insert(result);
        bool areArgsTimed = isTimed || (expr.casusu == BOX) || (expr.casusu == DIAMOND)
                || (expr.casusu == UNTIL) || (expr.casusu == RELEASE);
        //size_t h = 0;
        for (const auto& arg : expr.args) {
            auto cp = simplify(arg, areArgsTimed, insert);
            //h = std::max(cp.second, h);
            result->args.emplace_back(cp);
        }
        //h++;
        if (expr.is_exclusive) {
            assert(expr.casusu == OR);
            result->casusu = Q_XOR;
        } else {
            std::string v{magic_enum::enum_name(expr.casusu)};
            v = "Q_" + v;
            result->casusu = magic_enum::enum_cast<ltlf_query_t>(v).value();
        }
        if (expr.is_join_condition_place) {
            std::vector<std::vector<SimpleDataPredicate>> sdp;
            for (const auto& inConj : expr.joinCondition) {
                auto& x = sdp.emplace_back();
                for (const auto& pref : inConj) {
                    //assert(pref.second.BiVariableConditions.empty());
                    for (const auto& refx : pref.second.BiVariableConditions ) {
                        x.emplace_back(refx.var, refx.varRHS, refx.casusu);
                    }
                }
            }
            result->joinCondition = {sdp};
        }
        result->isTimed = isTimed;
        assert((expr.casusu != ACT) || (!expr.rewritten_act.empty()));
        if (expr.casusu==ACT){
            atomsToDecomposeInUnion.emplace_back(result);
        }
        result->atom.insert(expr.rewritten_act.begin(), expr.rewritten_act.end());
        //Q[h].emplace_back(result);
        conversion_map_for_subexpressions[q] =result;// {result, h};
        counter.emplace(result, 1);
        return result;
    }
}

void ltlf_query_manager::clear() {
    for (auto it = conversion_map_for_subexpressions.begin(); it != conversion_map_for_subexpressions.end(); it++) {
        delete it->second;
        it = conversion_map_for_subexpressions.erase(it);
    }
    VSet.clear();
    atomsToDecomposeInUnion.clear();
    counter.clear();
}

ltlf_query *ltlf_query_manager::init1(const std::string &atom, std::unordered_set<std::string> &predicates) {
    return immediateQueries(atom, predicates, "@declare_init1_", Q_INIT);
}

ltlf_query *ltlf_query_manager::immediateQueries(const std::string &atom,
                                                 std::unordered_set<std::string> &predicates,
                                                 const std::string &prefix,
                                                 const ltlf_query_t &casus) {
    ltlf f = ltlf::Act(prefix + atom);
    if (!predicates.empty()) {
        ltlf A = ltlf::Act("bogus_"+atom);
        A.rewritten_act.insert(A.rewritten_act.begin(), predicates.begin(), predicates.end());
        f = ltlf::And(f, A);
    }
    std::pair<ltlf, bool> q{f, false};
    auto it = conversion_map_for_subexpressions.find(q);
    if (it != conversion_map_for_subexpressions.end()) {
        counter[it->second]++;
        return it->second;
    } else {
        ltlf_query* result = new ltlf_query();
        result->casusu = casus;
        result->isTimed = false;
        if (predicates.empty())
            result->atom.emplace(atom);
        else
            result->atom.insert(predicates.begin(), predicates.end());
        //Q[0].emplace_back(result);
        conversion_map_for_subexpressions[q] = result;
        counter.emplace(result, 1);
        return result;
    }
}

ltlf_query *ltlf_query_manager::exists1(const std::string &atom, std::unordered_set<std::string> &predicates) {
    return immediateQueries(atom, predicates, "@declare_exists1_", Q_EXISTS);
}

ltlf_query *ltlf_query_manager::end1(const std::string &atom, std::unordered_set<std::string> &predicates) {
    return immediateQueries(atom, predicates, "@declare_end1_", Q_END);
}

void ltlf_query_manager::finalize_unions(const std::vector<ltlf_query*>& W) {
    std::vector<std::set<std::string>> unionToDecompose;
    for (const auto& ptr : atomsToDecomposeInUnion)
        unionToDecompose.emplace_back(ptr->atom);
    auto result = partition_sets(unionToDecompose);
    size_t isFromFurtherDecomposition = result.minimal_common_subsets.size();
    for (const auto& ref : result.decomposedIndexedSubsets) {
        auto& f = atomsToDecomposeInUnion.at(ref.first);
        bool just = true;
        ltlf r;
        for (size_t i : *ref.second) {
            if (i < isFromFurtherDecomposition) {
                ltlf l;
                l.casusu = ACT;
                l.rewritten_act.insert(l.rewritten_act.end(), result.minimal_common_subsets.at(i).begin(), result.minimal_common_subsets.at(i).end());
                if (just) {
                    r = l;
                    just = false;
                } else {
                    r = ltlf::Or(l, r);
                }
            } else
                for (size_t further : result.minimal_common_subsets_composition.at(i-isFromFurtherDecomposition)) {
                    ltlf l;
                    l.casusu = ACT;
                    l.rewritten_act.insert(l.rewritten_act.end(), result.minimal_common_subsets.at(further).begin(), result.minimal_common_subsets.at(further).end());
                    if (just) {
                        r = l;
                        just = false;
                    } else {
                        r = ltlf::Or(l, r);
                    }
                }

        }
        ltlf_query* q = simplify(r, true, false);
        q->isLeaf = true;
        *atomsToDecomposeInUnion[ref.first] = *q;
        //delete q; //this will not delete the other nodes, recursively. TODO: this should be done in clear() and avoid leaks
        atomsToDecomposeInUnion[ref.first]->isLeaf = true;
    }

    // Making ready for the parallelization of the query execution by setting it into layers
    std::vector<ltlf_query*> topological_order;
    topological_sort(W, topological_order);
    for (const auto& subFormula: topological_order) {
        for (const auto& adj : subFormula->args) {
            adj->dis = std::max(adj->dis, subFormula->dis +1);
            adj->parentMax = std::max(adj->parentMax, subFormula->dis);
            adj->parentMin = std::min(adj->parentMin, subFormula->dis);
        }
    }
    for (const auto& subFormula: topological_order) {
        Q[subFormula->dis].emplace_back(subFormula);
    }
}

#include <yaucl/strings/serializers.h>

std::ostream & human_readable_ltlf_printing(std::ostream &os, const ltlf_query* syntax) {
    std::string reset = "";
    switch (syntax->casusu) {
        case Q_INIT:
            return os << "I" << syntax->atom << reset;
        case Q_END:
            return os << "F" << syntax->atom << reset;
        case Q_ACT:
            return os << "A" <<  syntax->atom << reset;
        case Q_OR:
            os << "(";
            human_readable_ltlf_printing(os, syntax->args.at(0));
            os << ") ";
            if (syntax->isTimed) os << 't';
            if (!syntax->joinCondition.isTruth()) os << 'H';
            os << ("∨ (");
            return human_readable_ltlf_printing(os, syntax->args.at(1)) << ')' << reset;
        case Q_XOR:
            os << "(";
            human_readable_ltlf_printing(os, syntax->args.at(0));
            os << ") ";
            if (syntax->isTimed) os << 't';
            if (!syntax->joinCondition.isTruth()) os << 'H';
            os << ("⊻ (");
            return human_readable_ltlf_printing(os, syntax->args.at(1)) << ')' << reset;
        case Q_AND:
            os << "(";
            human_readable_ltlf_printing(os, syntax->args.at(0));
            os << ") ";
            if (syntax->isTimed) os << 't';
            if (!syntax->joinCondition.isTruth()) os << 'H';
            os << ("∧ (");
            return human_readable_ltlf_printing(os, syntax->args.at(1)) << ')' << reset;
        case Q_NEXT:
            if (syntax->isTimed) os << "t";
            os << "○(";
            return human_readable_ltlf_printing(os, syntax->args.at(0)) << ")" << reset;
        case Q_UNTIL:
            os << "(";
            human_readable_ltlf_printing(os, syntax->args.at(0));
            os << ") ";
            if (syntax->isTimed) os << 't';
            if (!syntax->joinCondition.isTruth()) os << 'H';
            os << ("U (");
            return human_readable_ltlf_printing(os, syntax->args.at(1)) << ')' << reset;
        case Q_RELEASE:
            os << "(";
            human_readable_ltlf_printing(os, syntax->args.at(0));
            os << ") ";
            if (syntax->isTimed) os << 't';
            if (!syntax->joinCondition.isTruth()) os << 'H';
            os << ("R (");
            return human_readable_ltlf_printing(os, syntax->args.at(1)) << ')' << reset;
        case Q_TRUE:
            return os << "true"<< reset;
        case Q_BOX:
            if (syntax->isTimed) os << "t";
            os << "▢(";
            return human_readable_ltlf_printing(os, syntax->args.at(0)) << ")" << reset;
        case Q_DIAMOND:
            if (syntax->isTimed) os << "t";
            os << "◇(";
            return human_readable_ltlf_printing(os, syntax->args.at(0)) << ")" << reset;
        case Q_LAST:
            return os << "LAST" << reset;
        default:
            return os << "false"<< reset;
    }
}