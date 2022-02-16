//
// Created by giacomo on 03/01/2022.
//

#ifndef KNOBAB_KB_GROUNDING_H
#define KNOBAB_KB_GROUNDING_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <knobab/KnowledgeBase.h>
#include <yaucl/bpm/structures/declare/CNFDeclareDataAware.h>

struct GroundingStrategyConf {
    enum pruning_strategy {
        NO_EXPANSION,
        ALWAYS_EXPAND_LEFT,
        ALWAYS_EXPAND_LESS_TOTAL_VALUES
    };

    std::unordered_set<std::string> LEFT, RIGHT;
    std::unordered_set<size_t>                                                             trace_ids;
    std::unordered_map<std::string, std::unordered_set<std::string>>                       ActToAttribute;
    std::unordered_set<std::string>                                                        OtherAttributes;
    std::unordered_map<std::string, std::unordered_map<std::string, std::set<union_type>>> I_A_x;
    std::unordered_map<std::string, std::set<union_type>>                                  I_x;
    std::string currentLeftAct, currentRightAct;

    bool ignoreActForAttributes;
    bool doPreliminaryFill;
    bool creamOffSingleValues;
    pruning_strategy strategy1;

    void clear() {
        trace_ids.clear();
        ActToAttribute.clear();
        OtherAttributes.clear();
        I_A_x.clear();
        I_x.clear();
        LEFT.clear();
        RIGHT.clear();
    }

    GroundingStrategyConf();
};

CNFDeclareDataAware GroundWhereStrategy(GroundingStrategyConf& conf,
                                        const KnowledgeBase& db,
                                        const std::vector<DeclareDataAware>& declare);

#endif //KNOBAB_KB_GROUNDING_H
