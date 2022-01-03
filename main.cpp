#include <iostream>
#include <yaucl/bpm/structures/declare/DeclareModelParse.h>
#include <yaucl/bpm/algos/transformations/declare_to_dfa/DeclareNoDataTemplateCollect.h>
#include <knobab/flloat_deps/flloat_wrapper.h>
#include <yaucl/bpm/structures/declare/CNFDeclareDataAware.h>

#include "knobab/KnowledgeBase.h"
#include "yaucl/bpm/structures/log/data_loader.h"
#include <unordered_map>
#include <set>
#include <string>

struct GroundingStrategyConf {
    std::unordered_set<size_t>                                                             trace_ids;
    std::unordered_map<std::string, std::unordered_set<std::string>>                       ActToAttribute;
    std::unordered_set<std::string>                                                        OtherAttributes;
    std::unordered_map<std::string, std::unordered_map<std::string, std::set<union_type>>> I_A_x;
    std::unordered_map<std::string, std::set<union_type>>                                  I_x;
    std::string currentLeftAct, currentRightAct;
    bool useLeftAttribute;
    bool useRightAttribute;
    bool ignoreActForAttributes;
    bool doPreliminaryFill;

    GroundingStrategyConf() : doPreliminaryFill{true}, trace_ids{}, I_A_x{},
                              useLeftAttribute{true}, useRightAttribute{true}, ignoreActForAttributes{false}
    {}
};


void collectFromDeclare(GroundingStrategyConf &conf, const DeclareDataAware &d) {
    std::unordered_set<std::string> leftAttrSet, rightAttrSet;
    //auto& refLeft = conf.I_A_x[declare.left_act];
    d.collectLeftAttributes([&leftAttrSet](const auto& x) { leftAttrSet.insert(x);}, false, true);
    //auto& refRight = conf.I_A_x[declare.right_act];
    d.collectLeftAttributes([&rightAttrSet](const auto& x) { rightAttrSet.insert(x);}, false, true);

    std::set<union_type> leftValues, rightValues;

    // Clearing the element, as I'm filling for each decleare now
    if (conf.ignoreActForAttributes) {
        conf.currentLeftAct.clear();
        conf.currentRightAct.clear();
        conf.OtherAttributes.insert(d.left_act);
        conf.OtherAttributes.insert(d.right_act);
    } else {
        conf.currentLeftAct = d.left_act;
        conf.currentRightAct = d.right_act;
        conf.ActToAttribute[d.left_act] = leftAttrSet;
        conf.ActToAttribute[d.right_act] = rightAttrSet;
    }
}

DisjunctiveDeclareDataAware GroundWhereStrategy(GroundingStrategyConf& conf,
                                                const KnowledgeBase& db,
                                                const DeclareDataAware& d) {

    if (!conf.doPreliminaryFill) {
        conf.OtherAttributes.clear();
        conf.ActToAttribute.clear();
        collectFromDeclare(conf, d);
        db.collectValuesFrom(conf.I_A_x, conf.I_x, {}, conf.ActToAttribute, conf.OtherAttributes);
    }

}


CNFDeclareDataAware GroundWhereStrategy(GroundingStrategyConf& conf,
                                        const KnowledgeBase& db,
                                        const std::vector<DeclareDataAware>& declare) {



    /**
     * Scanning the Knowledge Base for attributes via declare, before
     */
    if (conf.doPreliminaryFill) {
        conf.OtherAttributes.clear();
        conf.ActToAttribute.clear();
        for (const auto& d : declare) {
            collectFromDeclare(conf, d);
        }
        db.collectValuesFrom(conf.I_A_x, conf.I_x, {}, conf.ActToAttribute, conf.OtherAttributes);
    }

    CNFDeclareDataAware cnf;
    for (const auto& ref : declare)
        cnf.singleElementOfConjunction.emplace_back(GroundWhereStrategy(conf, db, ref));
    return cnf;
}



void test_kb() {
    /// Creating an instance of the knowledge base, that is going to store all the traces in the log!
    KnowledgeBase db;

    /// Loading a log file into the database
    load_into_knowledge_base(HUMAN_READABLE_YAUCL, true, "/home/giacomo/CLionProjects/knobab/log_1.txt", db);

    /// Indexing the data structures
    /// TODO: these indices might be written in secondary memory as well!
    db.index_data_structures();

    /// Debugging purposes: checking whether the thing that I obtain by printing corresponds to the one that I obtained before.
    /// This is just an isomorphism proof, that states that I can always reconstruct the original information from the
    /// given representation
    db.reconstruct_trace_with_data(std::cout);
}

void test_declare() {
    std::map<int, int> M;

    DeclareModelParse dmp;
    std::ifstream file{"ex_2.powerdecl"};

    DeclareNoDataTemplateCollect tc;
    for (const auto& ref : dmp.load(file)) {
        tc.add(ref);
        std::cout << ref << std::endl;
    }
    std::unordered_set<std::string> SigmaAll{"A", "B"};
    tc.run<FLLOAT>(SigmaAll);
}


int main() {
    test_declare();

    return 0;
}
