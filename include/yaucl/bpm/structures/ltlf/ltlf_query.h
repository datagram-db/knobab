//
// Created by giacomo on 16/02/2022.
//

#ifndef KNOBAB_LTLF_QUERY_H
#define KNOBAB_LTLF_QUERY_H

#include <knobab/queries/LTLfQuery.h>
#include <unordered_map>

struct ltlf_query_manager {
    std::unordered_map<LTLfQuery, LTLfQuery*> conversion_map_for_subexpressions;
    std::map<size_t, std::vector<LTLfQuery*>> Q;
    std::unordered_map<LTLfQuery*, size_t> counter;
    std::vector<LTLfQuery*> atomsToDecomposeInUnion;

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
};


#endif //KNOBAB_LTLF_QUERY_H
