#include <iostream>
#include "yaucl/bpm/structures/log/data_loader.h"
#include "yaucl/strings/serializers.h"
#include "knobab/utilities/Aggregators.h"
#include "knobab/dataStructures/TraceData.h"
#include "knobab/utilities/LTLFOperators.h"
#include "knobab/setOperators/operators.h"

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"a", {}}, {"a", {}}, {"b", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"a", {}}, {"b", {}}, {"a", {}}, {"c", {}}, {"a", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"c", {}}, {"b", {}}, {"c", {}}, {"c", {}}, {"c", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"c", {}}, {"c", {}}, {"a", {}}, {"c", {}}, {"c", {}}}};
//
//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"c", {}}, {"c", {}}, {"a", {}}, {"c", {}}, {"c", {}}, {"a", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"c", {}}, {"b", {}}, {"c", {}}, {"b", {}}, {"c", {}}}};

//const KnowledgeBase::no_antlr_log LogTrace = {
//        {{"a", {}}, {"a", {}}, {"b", {}}},
//{{"a", {}}, {"c", {{"x", 2.0}}}}};

const KnowledgeBase::no_antlr_log LogTrace = {
        {{"a", {{"x", 2.0},{"y", 3.0}}},
         {"a", {{"x", 5.0},{"y", 1.0}}},
         {"a", {{"x", 1.0},{"y", 1.0}}},
         {"b", {{"x", 4.0},{"y", 3.0}}}},

        {{"b", {}},
         {"b", {}},
         {"b", {}},
         {"b", {}},
         {"a", {}}},

        {{"c", {}},
         {"b", {}},
         {"c", {}},
         {"b", {}},
         {"c", {}}}
};

const std::string toSearch = "a";
const std::vector<SimpleDataPredicate> preds{{"x", "x", numeric_atom_cases::EQ}};

KnowledgeBase test_kb(const KnowledgeBase::no_antlr_log& L, const std::string &source, const std::string &name) {
    /// Creating an instance of the knowledge base, that is going to store all the traces in the log!
    KnowledgeBase db;

    db.load_data_without_antlr4(L, source, name);

    /// Loading a log file into the database
    /// load_into_knowledge_base(HUMAN_READABLE_YAUCL, true, "testing/log.txt", db);

    /// Indexing the data structures
    /// TODO: these indices might be written in secondary memory as well!
    db.index_data_structures();

    /// Debugging purposes: checking whether the thing that I obtain by printing corresponds to the one that I obtained before.
    /// This is just an isomorphism proof, that states that I can always reconstruct the original information from the
    /// given representation
    db.reconstruct_trace_with_data(std::cout);

    return db;
}

int main() {
    KnowledgeBase db = test_kb(LogTrace, "", "");

    // First part
    std::pair<const uint32_t, const uint32_t>  data = db.resolveCountingData(toSearch);

    // Second part
    std::unordered_map<uint32_t, double> found = db.exists(data, 2);

    std::cout << "========EXISTS=========" << std::endl << toSearch << std::endl << "RESULT: " << found << std::endl;

    PredicateManager predManager(preds, &db);

    const double minThreshHold = 0;

    TraceData<std::pair<uint32_t, uint16_t>, double> initAVec = db.init<std::pair<uint32_t, uint16_t>, double>(toSearch, minThreshHold);
    TraceData<std::pair<uint32_t, uint16_t>, double> endsAVec = db.ends<std::pair<uint32_t, uint16_t>, double>(toSearch, minThreshHold);

//    TraceData<std::pair<uint32_t, uint16_t>, double> unionNoPred;
//    initAVec.setUnion(endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(), std::back_inserter(unionNoPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>);
//    std::cout << "========UNION NO PREDICATE =========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
//    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << unionNoPred.getTraceApproximations() << std::endl;
//
////    TraceData<std::pair<uint32_t, uint16_t>, double> unionPred;
////    initAVec.setUnion(endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(), std::back_inserter(unionPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>, &predManager);
////    std::cout << "========UNION WITH PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
////    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << unionPred.getTraceApproximations() << std::endl;
//
//    TraceData<std::pair<uint32_t, uint16_t>, double> intersectionNoPred;
//    initAVec.setIntersection(endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(), std::back_inserter(intersectionNoPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>);
//    std::cout << "========INTERSECTION NO PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
//    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << intersectionNoPred.getTraceApproximations() << std::endl;

//    TraceData<std::pair<uint32_t, uint16_t>, double> intersectionPred;
//    initAVec.setIntersection(endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(), std::back_inserter(intersectionPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>, &predManager);
//    std::cout << "========INTERSECTION NO PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
//    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << intersectionPred.getTraceApproximations() << std::endl;

    struct example{
        std::vector<std::pair<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>> elems;
    };

    example e;
    // aaab
    e.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 0}, {1, {0}}));
    e.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
    e.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 2}, {1, {2}}));


    auto x1 = next(0, 1, 4, e);
    auto x2 = next(e);
    auto x3 = global(0, 1, 3, e);
    auto x4 = global(e, db.act_table_by_act_id.getTraceLengths());
    auto x5 = future(0, 1, 2, e);
    auto x6 = future(e);

    std::cout << "========LTLf Operators=========" << std::endl;
    std::cout << "--NEXT--" << std::endl << "TEMPORAL: " << x1 << std::endl << "NON-TEMPORAL" << x2 << std::endl;
    std::cout << "--GLOBAL--" << std::endl << "TEMPORAL: " << x3 << std::endl << "NON-TEMPORAL" << x4 << std::endl;
    std::cout << "--FUTURE--" << std::endl << "TEMPORAL: " << x5 << std::endl << "NON-TEMPORAL" << x6 << std::endl;
    std::flush(std::cout);


    example e1;
    // aaab
    e1.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 0}, {1, {0}}));
    e1.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
    e1.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 3}, {1, {3}}));

    example e2;

    setUnion(e.elems.begin(), e.elems.end(), e1.elems.begin(), e1.elems.end(), std::back_inserter(e2.elems), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "LTLF UNION" << e2.elems << std::endl;

    return 0;
}