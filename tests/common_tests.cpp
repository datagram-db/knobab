//
// Created by giacomo on 21/03/2022.
//

TEST_MULTI(Response)
TEST_F(TEST_NAME(Response), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(Choice)
TEST_F(TEST_NAME(Choice), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(ChainResponse)
TEST_F(TEST_NAME(ChainResponse), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(RespExistence)
TEST_F(TEST_NAME(RespExistence), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(ExlChoice)
TEST_F(TEST_NAME(ExlChoice), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(CoExistence)
TEST_F(TEST_NAME(CoExistence), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(NotCoExistence)
TEST_F(TEST_NAME(NotCoExistence), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(Precedence)
TEST_F(TEST_NAME(Precedence), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(Succession)
TEST_F(TEST_NAME(Succession), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

// NegSuccession

TEST_MULTI(NegSuccession)
TEST_F(TEST_NAME(NegSuccession), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(ChainPrecedence)
TEST_F(TEST_NAME(ChainPrecedence), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}


TEST_MULTI(ChainSuccession)
TEST_F(TEST_NAME(ChainSuccession), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}

TEST_MULTI(AltResponse)
TEST_F(TEST_NAME(AltResponse), test_single_declare_clause) {
auto ref = env.query_model();
ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
if (i<pos)
ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
else
ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
}
}
