//
// Created by giacomo on 21/03/2022.
//

#define ALL_TESTS(name) TEST_MULTI(name); \
TEST_F(TEST_NAME(name), test_single_declare_clause) { \
auto ref = env.query_model();            \
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability); \
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {      \
             if (i<pos) \
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);\
else \
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);\
}\
}                            \

ALL_TESTS(Response)
ALL_TESTS(Choice)
ALL_TESTS(ChainResponse)
ALL_TESTS(RespExistence)
ALL_TESTS(ExlChoice)
ALL_TESTS(CoExistence)
ALL_TESTS(NotCoExistence)
ALL_TESTS(Precedence)
ALL_TESTS(Succession)
ALL_TESTS(NegSuccession)
ALL_TESTS(ChainPrecedence)
ALL_TESTS(ChainSuccession)
ALL_TESTS(AltResponse)
ALL_TESTS(AltPrecedence)