#include <iostream>
#include "yaucl/bpm/structures/log/data_loader.h"
#include "yaucl/strings/serializers.h"
#include "knobab/utilities/Aggregators.h"
#include "knobab/dataStructures/TraceData.h"
#include "knobab/utilities/LTLFOperators.h"
#include "knobab/utilities/SetOperators.h"
#include "knobab/utilities/DeclareClauses.h"
#include "yaucl/structures/set_operations.h"

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
        {{"A", {{"x", 2.0},{"y", 3.0}}},
         {"B", {{"x", 1.0},{"y", 6.0}}},
         {"A", {{"x", 2.0},{"y", 3.0}}},
         {"B", {{"x", 2.0},{"y", 6.0}}},
         {"A", {{"x", 2.0},{"y", 5.0}}},
         },

        {{"A", {{"x", 2.0},{"y", 1.0}}},
         {"A", {{"x", 1.0},{"y", 3.0}}},
         {"B", {}},
         {"A", {{"x", 1.0},{"y", 3.0}}},
         {"A", {{"x", 4.0},{"y", 3.0}}}},

        {{"C", {}},
         {"B", {}},
         {"C", {}},
         {"B", {}},
         {"C", {}}}
};

const std::string toSearch = "A";
const uint8_t searchAmount = 2;
const std::vector<SimpleDataPredicate> predicates{{"x", "y", numeric_atom_cases::LT}};
const double minThreshHold = 0;

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
    PredicateManager predManager(predicates, &db);

    std::pair<const uint32_t, const uint32_t>  data = db.resolveCountingData(toSearch);
    std::unordered_map<uint32_t, double> found = db.exists(data, searchAmount);
    std::cout << "========EXISTS " + toSearch + " " + std::to_string(searchAmount) + "TIMES========="  << std::endl << "RESULT: " << std::endl << found << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>> initAVec = db.init<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>(toSearch, minThreshHold);
    std::cout << "========INIT " + toSearch + "=========" << std::endl << "RESULT: " << std::endl << initAVec.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>> endsAVec = db.ends<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>(toSearch, minThreshHold);
    std::cout << "========ENDS " + toSearch + "=========" << std::endl << "RESULT: " << std::endl << endsAVec.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> unionNoPred;
    setUnion(true, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
             std::back_inserter(unionNoPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>);
    std::cout << "========UNION NO PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << unionNoPred.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> unionPred;
    setUnion(true, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
             std::back_inserter(unionPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "========UNION WITH PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << unionPred.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> intersectionNoPred;
    setIntersection(true, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
                    std::back_inserter(intersectionNoPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>);
    std::cout << "========INTERSECTION NO PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << intersectionNoPred.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> intersectionPred;
    setIntersection(true, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
                    std::back_inserter(intersectionPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>, &predManager);
    std::cout << "========INTERSECTION WITH PREDICATE=========" << std::endl << "BETWEEN: " << std::endl << initAVec.getTraceApproximations() << std::endl << "AND" << std::endl
    << endsAVec.getTraceApproximations() << std::endl << "RESULT: " << std::endl << intersectionPred.getTraceApproximations() << std::endl;

    //aaab from trace 0
    dataContainer aOccurences{}, bOccurences{};
    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 0}, {1, {0}}));
    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 2}, {1, {2}}));
    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 4}, {1, {4}}));

    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 3}, {1, {3}}));

    std::cout << "========DATA=========" << std::endl << "SET1: " << std::endl << aOccurences << std::endl << "SET2:" << std::endl << bOccurences << std::endl;

    std::cout << "========LTLf Operators=========" << std::endl;
    auto x1 = next(0, 0, 1, aOccurences);
    auto x2 = next(aOccurences);
    std::cout << "--NEXT--" << std::endl << "TEMPORAL: " << x1 << std::endl << "NON-TEMPORAL" << x2 << std::endl;

    auto x3 = global(0, 0, 2, aOccurences);
    auto x4 = global(aOccurences, db.act_table_by_act_id.getTraceLengths());
    std::cout << "--GLOBAL--" << std::endl << "TEMPORAL: " << x3 << std::endl << "NON-TEMPORAL" << x4 << std::endl;

    auto x5 = future(0, 1, 2, aOccurences);
    auto x6 = future(aOccurences);
    std::cout << "--FUTURE--" << std::endl << "TEMPORAL: " << x5 << std::endl << "NON-TEMPORAL" << x6 << std::endl;

    dataContainer e2, e3;
    setUnion(true, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e2), Aggregators::maxSimilarity<double, double, double>, &predManager);
    setUnion(false, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e3), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "--LTLF UNION--" << std::endl << "TEMPORAL: " << e2 << std::endl <<  "NON-TEMPORAL" << e3 << std::endl;;

    dataContainer e4, e5;
    setIntersection(true, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e4), Aggregators::maxSimilarity<double, double, double>, &predManager);
    setIntersection(false, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e5), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "--LTLF INTERSECTION--" << std::endl << "TEMPORAL: " << e4 << std::endl <<  "NON-TEMPORAL" << e5 << std::endl;;

    dataContainer choice = Choice(aOccurences, bOccurences, nullptr);
    std::cout << "========Choice=========" <<  std::endl << choice << std::endl;

    dataContainer exclusiveChoice = ExclusiveChoice(aOccurences, bOccurences, bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========ExclusiveChoice=========" << std::endl << exclusiveChoice << std::endl;

    dataContainer responseExistence = ResponseExistence(aOccurences, bOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========ResponseExistence=========" << std::endl << responseExistence << std::endl;

    dataContainer coexistence = Coexistence(aOccurences, bOccurences, bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(), nullptr);
    //remove_duplicates(coexistence);
    std::cout << "========Coexistence=========" << std::endl << coexistence << std::endl;
    std::flush(std::cout);

    dataContainer notCoexistence = NotCoexistence(bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========NotCoexistence=========" << std::endl << notCoexistence << std::endl;
    std::flush(std::cout);

    dataContainer chainResponse = ChainResponse(aOccurences, bOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr);
    std::cout << "========ChainResponse=========" << std::endl << chainResponse << std::endl;
    std::flush(std::cout);

    dataContainer chainPrecedence = ChainPrecedence(aOccurences, bOccurences, aOccurences, db.getLastElements(), db.act_table_by_act_id.getTraceLengths(),&predManager);
    std::cout << "========ChainPrecedence=========" << std::endl << chainPrecedence << std::endl;
    std::flush(std::cout);
    return 0;
}