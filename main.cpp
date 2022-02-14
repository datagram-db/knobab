#include <iostream>
#include "yaucl/bpm/structures/log/data_loader.h"
#include "yaucl/strings/serializers.h"
#include "knobab/utilities/Aggregators.h"
#include "knobab/dataStructures/TraceData.h"
#include "knobab/utilities/LTLFOperators.h"

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
        {{"a", {}}, {"a", {}}, {"a", {}}, {"b", {}}},
        {{"b", {}}, {"b", {}}, {"b", {}}, {"b", {}}, {"a", {}}},
        {{"c", {}}, {"b", {}}, {"c", {}}, {"b", {}}, {"c", {}}}};

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
    std::pair<const uint32_t, const uint32_t>  data = db.resolveCountingData("a");

    // Second part
    std::unordered_map<uint32_t, double> found = db.exists(data, 2);

    const double minThreshHold = 0;

    TraceData<std::pair<uint32_t, uint16_t>, double> testVec = db.init<std::pair<uint32_t, uint16_t>, double>("a", minThreshHold);
    TraceData<std::pair<uint32_t, uint16_t>, double> testVec1 = db.ends<std::pair<uint32_t, uint16_t>, double>("a", minThreshHold);

    TraceData<std::pair<uint32_t, uint16_t>, double> testVec2;
    testVec.setUnion(testVec1.getTraceApproximations().begin(), testVec1.getTraceApproximations().end(), std::back_inserter(testVec2.getTraceApproximations()),  Aggregators::maxSimilarity<double, double, double>);

    std::cout << "========UNION=========" << std::endl << "BETWEEN: " << std::endl << testVec.getTraceApproximations() << std::endl << "AND" << std::endl << testVec1.getTraceApproximations() << std::endl << "RESULT: " << std::endl << testVec2.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, double> testVec3;
    testVec.setIntersection(testVec1.getTraceApproximations().begin(), testVec1.getTraceApproximations().end(), std::back_inserter(testVec3.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>);

    std::cout << "========INTERSECTION=========" << std::endl << "BETWEEN: " << std::endl << testVec.getTraceApproximations() << std::endl << "AND" << std::endl << testVec1.getTraceApproximations() << std::endl << "RESULT: " << std::endl << testVec3.getTraceApproximations()<< std::endl;

    struct example{
        std::vector<std::pair<std::pair<uint32_t, uint16_t>, double>> elems;
    };

    example e;
    // aaab
    e.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(0, 1), 0));
    e.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(0, 2), 0));
    e.elems.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, double>(std::pair<uint32_t, uint16_t>(0, 3), 0));

    auto x1 = next(0, 1, 2, e);
    auto x2 = next(e);
    auto x3 = global(0, 1, 2, e);
    auto x4 = global(e, db.act_table_by_act_id.getTraceLengths());
    auto x5 = future(0, 1, 1, e);
    auto x6 = future(e);

    std::cout << "========LTLf Operators=========" << std::endl;
    std::cout << "--NEXT--" << std::endl << "TEMPORAL: " << *x1 << std::endl << "NON-TEMPORAL" << *x2 << std::endl;
    std::cout << "--GLOBAL--" << std::endl << "TEMPORAL: " << *x3 << std::endl << "NON-TEMPORAL" << *x4 << std::endl;
    std::cout << "--FUTURE--" << std::endl << "TEMPORAL: " << *x5 << std::endl << "NON-TEMPORAL" << *x6 << std::endl;
    std::flush(std::cout);
    return 0;
}