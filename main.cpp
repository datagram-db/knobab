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
#include <knobab/predicates/testing_predicates.h>
#include <yaucl/functional/iterators.h>


struct GroundingStrategyConf {
    enum pruning_strategy {
        ALWAYS_EXPAND_LEFT,
        ALWAYS_EXPAND_LESS_TOTAL_VALUES
    };

    std::unordered_set<std::string> LEFT, RIGHT;
    const KnowledgeBase&                                                                   db;
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


    GroundingStrategyConf(const KnowledgeBase& kb) : strategy1{ALWAYS_EXPAND_LESS_TOTAL_VALUES}, db{kb}, doPreliminaryFill{true}, trace_ids{}, I_A_x{}, ignoreActForAttributes{false}
    {}
};





std::pair<std::unordered_set<std::string>,
        std::unordered_set<std::string>>
        collectFromDeclare(GroundingStrategyConf &conf, const DeclareDataAware &d) {
    std::unordered_set<std::string> leftAttrSet, rightAttrSet;
    //auto& refLeft = conf.I_A_x[declare.left_act];
    auto cp =
    std::make_pair(d.collectLeftAttributes([&leftAttrSet](const auto& x) { leftAttrSet.insert(x);}, conf.creamOffSingleValues, true),
    //auto& refRight = conf.I_A_x[declare.right_act];
    d.collectRightAttributes([&rightAttrSet](const auto& x) { rightAttrSet.insert(x);}, conf.creamOffSingleValues, true));

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
    return cp;
}

void fillForCartesian(std::vector<std::string> &names, std::vector<std::vector<union_type>> &toCartesianProduct,
                      std::unordered_set<std::string> &refL,
                      std::unordered_map<std::string, std::set<union_type>> &map) {
    for (const auto& name : refL) {
        auto it = map.find(name);
        if (it != map.end()) {
            names.emplace_back(name);
            toCartesianProduct.emplace_back(it->second.begin(), it->second.end());
        }
    }
}




DisjunctiveDeclareDataAware GroundWhereStrategy(GroundingStrategyConf& conf,
                                                const KnowledgeBase& db,
                                                const DeclareDataAware& d) {

    if (!conf.doPreliminaryFill) {
        conf.OtherAttributes.clear();
        conf.ActToAttribute.clear();
        std::tie(conf.LEFT, conf.RIGHT) = collectFromDeclare(conf, d);
        db.collectValuesFrom(conf.I_A_x, conf.I_x, {}, conf.ActToAttribute, conf.OtherAttributes);
    }

    std::string left_act = d.left_act, right_act = d.right_act;
    std::unordered_map<std::string, std::set<union_type>> I_X_left;
    std::unordered_map<std::string, std::set<union_type>> I_Y_right;

    if (conf.ignoreActForAttributes) {
        I_X_left = conf.I_x;
        if (conf.strategy1 == GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES)
            I_Y_right = conf.I_x;
    } else {
        assert(conf.I_A_x.contains(left_act));
        assert(conf.I_A_x.contains(right_act));
        I_X_left = conf.I_A_x.at(left_act);
        if (conf.strategy1 == GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES)
            I_Y_right = conf.I_A_x.at(right_act);
    }

    if (conf.creamOffSingleValues) {
        // TODO: TESTING IF THIS ACTUALLY WORKS! (begin)
        auto S = d.toFiniteSemantics().nnf().possibleActionsUpToNext();
        if (S.containsInSet(left_act)) {
            filter_some_values_from_disjunctive_map(d.dnf_left_map, I_X_left);
        }
        if (S.containsInSet(right_act)) {
            filter_some_values_from_disjunctive_map(d.dnf_right_map, I_Y_right);
        }
        // TODO: (end)
    }

    // If there are some incompatibilites with the predicates, none of them will be a good solutuion
    if (I_X_left.empty() || I_Y_right.empty()) return {};

    // Selecting the most advantageous part for performing the expansion, id est, the one leading to less elements
    bool isLeft = true;
    if (conf.strategy1 == GroundingStrategyConf::ALWAYS_EXPAND_LESS_TOTAL_VALUES) {
        size_t leftSize = 1, rightSize = 1;
        bool leftEmpty = true, rightEmpty = true;
        for (const auto& element: I_X_left) {
            if (conf.LEFT.contains(element.first)) {
                leftEmpty = false;
                leftSize *= element.second.size();
            }
        }
        if (leftEmpty) return {};
        for (const auto& element: I_Y_right) {
            if (conf.LEFT.contains(element.first)) {
                rightEmpty = false;
                rightSize *= element.second.size();
            }
        }
        if (rightEmpty) return {};
        if (rightSize < leftSize)
            isLeft = false;
    }

    std::vector<std::string> names;
    std::vector<std::vector<union_type>> toCartesianProduct;
    if (isLeft) {
        fillForCartesian(names, toCartesianProduct, conf.LEFT, I_X_left);
    } else {
        fillForCartesian(names, toCartesianProduct, conf.RIGHT, I_Y_right);
    }

    /**
     * TODO: the missing part for tomorrow is the following: I need to generate from d a novel DeclareDataAware,
     * where all of the values in it are replaced. This means that:
     *  - if the value is within the dnf, non in the where, needs to be tested; please observe that this is now required,
     *    as previously we did not test the comporaneity of the elements. If in the process, in dnf there is a false,
     *    then we need to discard this solution (this should never happen if creamOffSingleValues was previously run).
     *  - next, we replace all the values also in the multiple variable vector part for each element DataPredicate:
     *    if a condition is tested to false, the whole declare clause is discarded. If true, the current element is still
     *    ignored. Otherwise, we obtain a new predicate/interval:
     *     - If the value is to be associated to the right part, 1) clear varRHS and set the value to the current one.
     *       next, perform the intersection with the parent: 2) if the resulting object becomes empty interval, discard
     *       the predicate; otherwise, keep the resulting intersection.
     *     - If the value to be applied is only on the left part, swap the left variable with the right
     *       first, and then do as in the previous case (1). Also, find a numeric interval containing it (or create a new
     *       one) and attempt at performing the intersection with it, then do the same tests with it as in 2):
     */
    auto cartesianResult = yaucl::iterators::cartesian(toCartesianProduct, [](const auto& x) {return true;});

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
            auto it = collectFromDeclare(conf, d);
            conf.LEFT.insert(it.first.begin(), it.first.end());
            conf.RIGHT.insert(it.second.begin(), it.second.end());
        }
        db.collectValuesFrom(conf.I_A_x, conf.I_x, {}, conf.ActToAttribute, conf.OtherAttributes);
    }

    CNFDeclareDataAware cnf;
    for (const auto& ref : declare) {
        auto V = GroundWhereStrategy(conf, db, ref);
        if (!V.elementsInDisjunction.empty())
            cnf.singleElementOfConjunction.emplace_back(V);
    }
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
