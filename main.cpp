#include <iostream>
#include <iomanip>
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
         {"A", {{"x", 1.0},{"y", 6.0}}},
         {"A", {{"x", 2.0},{"y", 3.0}}},
         {"B", {{"x", 2.0},{"y", 6.0}}},
         {"B", {{"x", 2.0},{"y", 5.0}}},
         {"B", {{"x", 2.0},{"y", 5.0}}},},

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

    std::cout << "========BASIC OPERATORS=========" << std::endl;
    std::pair<const uint32_t, const uint32_t>  data = db.resolveCountingData(toSearch);
    std::unordered_map<uint32_t, double> found = db.exists(data, searchAmount);
    std::cout << "========Exists " + toSearch + " " + std::to_string(searchAmount) + " TIMES========="  << std::endl << std::setw(8) <<  "Result: " << found << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>> initAVec = db.init<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>(toSearch, minThreshHold);
    std::cout << "========Init " + toSearch + "=========" << std::endl << std::setw(8) << "Result: " << initAVec.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>> endsAVec = db.ends<std::pair<uint32_t, uint16_t>,  std::pair<double, std::vector<uint16_t>>>(toSearch, minThreshHold);
    std::cout << "========Ends " + toSearch + "=========" << std::endl << std::setw(8) << "Result: " << endsAVec.getTraceApproximations() << std::endl << std::endl;

    std::cout << "========SET OPERATORS=========" << std::endl;
    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> timedUnionNoPred, timedUnionWithPred, untimedUnionNoPred, untimedUnionWithPred;
    setUnion(true, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
             std::back_inserter(timedUnionNoPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>);
    setUnion(true, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
             std::back_inserter(timedUnionNoPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>, &predManager);
    setUnion(false, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
             std::back_inserter(untimedUnionNoPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>);
    setUnion(false, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
             std::back_inserter(untimedUnionWithPred.getTraceApproximations()), Aggregators::maxSimilarity<double, double, double>, &predManager);

    std::cout << "========Union timed no predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec.getTraceApproximations() << std::endl << std::setw(9) << "and: "
              << endsAVec.getTraceApproximations() << std::endl << std::setw(9) << "Result: " << timedUnionNoPred.getTraceApproximations() << std::endl;
    std::cout << "========Union timed with predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec.getTraceApproximations() << std::endl << std::setw(9) << "and: "
              << endsAVec.getTraceApproximations() << std::endl << std::setw(9) << "Result: " << timedUnionNoPred.getTraceApproximations() << std::endl;
    std::cout << "========Union untimed no predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec.getTraceApproximations() << std::endl << std::setw(9) << "and: "
              << endsAVec.getTraceApproximations() << std::endl << std::setw(9) << "Result: " << untimedUnionNoPred.getTraceApproximations() << std::endl;
    std::cout << "========Union untimed with predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec.getTraceApproximations() << std::endl << std::setw(9) << "and: "
              << endsAVec.getTraceApproximations() << std::endl << std::setw(9) << "Result: " << untimedUnionWithPred.getTraceApproximations() << std::endl;

    TraceData<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>> timedIntersectionNoPred, timedIntersectionWithPred, untimedIntersectionNoPred, untimedIntersectionWithPred;
    setIntersection(true, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
                    std::back_inserter(timedIntersectionNoPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>);
    setIntersection(true, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
                    std::back_inserter(timedIntersectionWithPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>, &predManager);
    setIntersection(false, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
                    std::back_inserter(untimedIntersectionNoPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>);
    setIntersection(false, initAVec.getTraceApproximations().begin(), initAVec.getTraceApproximations().end(), endsAVec.getTraceApproximations().begin(), endsAVec.getTraceApproximations().end(),
                    std::back_inserter(untimedIntersectionWithPred.getTraceApproximations()), Aggregators::joinSimilarity<double, double, double>, &predManager);

    std::cout << "========Intersection timed no predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec.getTraceApproximations() << std::endl << std::setw(9) << "and: "
    << endsAVec.getTraceApproximations() << std::endl << std::setw(9) << "Result: " << timedIntersectionNoPred.getTraceApproximations() << std::endl;
    std::cout << "========Intersection timed with predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec.getTraceApproximations() << std::endl << std::setw(9) << "and: "
              << endsAVec.getTraceApproximations() << std::endl << std::setw(9) << "Result: " << timedIntersectionWithPred.getTraceApproximations() << std::endl;
    std::cout << "========Intersection untimed no predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec.getTraceApproximations() << std::endl << std::setw(9) << "and: "
    << endsAVec.getTraceApproximations() << std::endl << std::setw(9) << "Result: " << untimedIntersectionNoPred.getTraceApproximations() << std::endl << std::endl;
    std::cout << "========Intersection untimed with predicate=========" << std::endl << std::setw(9) << "Between: " << initAVec.getTraceApproximations() << std::endl << std::setw(9) << "and: "
              << endsAVec.getTraceApproximations() << std::endl << std::setw(9) << "Result: " << untimedIntersectionWithPred.getTraceApproximations() << std::endl << std::endl;

    //aaab from trace 0
    dataContainer aOccurences{}, bOccurences{}, aBOccurences{};
    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 0}, {1, {0}}));
    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 2}, {1, {2}}));
    //    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 3}, {1, {3}}));
//    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 4}, {1, {4}}));
//    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 5}, {1, {5}}));
//    aOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 6}, {1, {6}}));

//    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 0}, {1, {0}}));
//    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
//    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 2}, {1, {2}}));
    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 3}, {1, {3}}));
    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 4}, {1, {4}}));
    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 5}, {1, {5}}));
//    bOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 6}, {1, {6}}));

    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 0}, {1, {0}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 1}, {1, {1}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 2}, {1, {2}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 3}, {1, {3}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 4}, {1, {4}}));
    aBOccurences.emplace_back(std::pair<std::pair<uint32_t, uint16_t>, std::pair<double, std::vector<uint16_t>>>({0, 5}, {1, {5}}));

    std::cout << "========DATA=========" << std::endl;
    std::cout << "Set 1: "  << aOccurences << std::endl << "Set 2: " << bOccurences << std::endl << std::endl;

    std::cout << "========LTLF OPERATAORS=========" << std::endl;
    std::cout << "========Next========" << std::endl << std::setw(14) << "Temporal: " << next(0, 0, 1, aOccurences) << std::endl << std::setw(14) << "Non-Temporal: " << next(aOccurences) << std::endl;
    std::cout << "========Global========" << std::endl << std::setw(14) << "Temporal: " << global(0, 0, 2, aOccurences) << std::endl << std::setw(14) << "Non-Temporal: " << global(aOccurences, db.act_table_by_act_id.getTraceLengths()) << std::endl;
    std::cout << "========Future========" << std::endl << std::setw(14) << "Temporal: " <<  future(0, 1, 2, aOccurences) << std::endl << std::setw(14) << "Non-Temporal: " <<  future(aOccurences) << std::endl;
    std::cout << "========Until========" << std::endl << std::setw(14) << "Temporal: " << until(0, 0, 2, aOccurences, bOccurences) << std::endl << std::setw(14) << "Non-Temporal: " << until(aOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths()) << std::endl << std::endl;

    std::cout << "========SET OPERATORS=========" << std::endl;
    dataContainer e1, e2, e3, e4, e5, e6, e7, e8;
    setUnion(true, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e1), Aggregators::maxSimilarity<double, double, double>);
    setUnion(true, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e2), Aggregators::maxSimilarity<double, double, double>,&predManager);
    setUnion(false, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e3), Aggregators::maxSimilarity<double, double, double>);
    setUnion(false, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e4), Aggregators::maxSimilarity<double, double, double>, &predManager);
    std::cout << "========Ltlf Union timed no predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e1 << std::endl;
    std::cout << "========Ltlf Union timed with predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e2 << std::endl;
    std::cout << "========Ltlf Union untimed no predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e3 << std::endl;
    ;std::cout << "========Ltlf Union untimed with predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
               << bOccurences << std::endl << std::setw(9) << "Result: " << e4 << std::endl;

    setIntersection(true, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e5), Aggregators::maxSimilarity<double, double, double>);
    setIntersection(true, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e6), Aggregators::maxSimilarity<double, double, double>, &predManager);
    setIntersection(false, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e7), Aggregators::maxSimilarity<double, double, double>);
    setIntersection(false, aOccurences.begin(), aOccurences.end(), bOccurences.begin(), bOccurences.end(), std::back_inserter(e8), Aggregators::maxSimilarity<double, double, double>,nullptr);
    std::cout << "========Ltlf Intersection timed no predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e5 << std::endl;
    std::cout << "========Ltlf Intersection timed with predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e6 << std::endl;
    std::cout << "========Ltlf Intersection untimed no predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e7 << std::endl;
    std::cout << "========Ltlf Intersection untimed with predicate=========" << std::endl << std::setw(9) << "Between: " << aOccurences << std::endl << std::setw(9) << "and: "
              << bOccurences << std::endl << std::setw(9) << "Result: " << e8 << std::endl <<std::endl;

    std::cout << "========DECLARE CLAUSES=========" << std::endl;
    std::cout << "========Choice=========" <<  std::endl << Choice(aOccurences, bOccurences, nullptr) << std::endl;
    std::cout << "========ExclusiveChoice=========" << std::endl << ExclusiveChoice(aOccurences, bOccurences, bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::cout << "========ResponseExistence=========" << std::endl << ResponseExistence(aOccurences, bOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::cout << "========Coexistence=========" << std::endl << Coexistence(aOccurences, bOccurences, bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(), nullptr) << std::endl;
    std::cout << "========NotCoexistence=========" << std::endl << NotCoexistence(bOccurences, aOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::cout << "========ChainResponse=========" << std::endl << ChainResponse(aOccurences, bOccurences, bOccurences, db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::cout << "========ChainPrecedence=========" << std::endl << ChainPrecedence(aOccurences, bOccurences, aOccurences, db.getNotFirstElements(), db.getLastElements(), db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::cout << "========ChainSuccession=========" << std::endl << ChainSuccession(aOccurences, bOccurences,  bOccurences, aOccurences, db.getNotFirstElements(), db.getLastElements(), db.act_table_by_act_id.getTraceLengths(), nullptr) << std::endl;
    std::cout << "========NegationChainSuccession=========" << std::endl << NegationChainSuccession(aOccurences, bOccurences,  bOccurences, aOccurences, db.getLastElements(), db.act_table_by_act_id.getTraceLengths(),nullptr) << std::endl;
    std::flush(std::cout);

    return 0;
}