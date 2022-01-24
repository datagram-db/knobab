#include <iostream>
#include "yaucl/bpm/structures/log/data_loader.h"
#include "yaucl/strings/serializers.h"
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
    std::unordered_map<uint32_t, float> found = db.exists(data, 2);

    const float minThreshhold = 0;

    TraceData<std::pair<uint32_t, uint16_t>, float> testVec = db.init<std::pair<uint32_t, uint16_t>, float>("a", minThreshhold);
    TraceData<std::pair<uint32_t, uint16_t>, float> testVec1 = db.init<std::pair<uint32_t, uint16_t>, float>("a", minThreshhold);

    TraceData<std::pair<uint32_t, uint16_t>, float> testVec2;
    testVec.setUnion(testVec1.traceApproximations.begin(), testVec1.traceApproximations.end(), std::back_inserter(testVec2.traceApproximations));

    std::cout << "========UNION=========" << std::endl << "BETWEEN: " << std::endl << testVec.traceApproximations << std::endl << "AND" << std::endl << testVec1.traceApproximations << std::endl << "RESULT: " << std::endl << testVec2.traceApproximations << std::endl;
    std::flush(std::cout);

    TraceData<std::pair<uint32_t, uint16_t>, float> testVec3;
    testVec.setIntersection(testVec1.traceApproximations.begin(), testVec1.traceApproximations.end(), std::back_inserter(testVec3.traceApproximations));

    std::cout << "========INTERSECTION=========" << std::endl << "BETWEEN: " << std::endl << testVec.traceApproximations << std::endl << "AND" << std::endl << testVec1.traceApproximations << std::endl << "RESULT: " << std::endl << testVec3.traceApproximations<< std::endl;
    std::flush(std::cout);

    return 0;
}