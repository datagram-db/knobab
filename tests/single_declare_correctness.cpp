//
// Created by giacomo on 27/02/2022.
//

#define CTEST_COLOR_OK
#define CTEST_SEGFAULT

#include <vector>
#include <yaucl/structures/set_operations.h>
#include <knobab/Environment.h>
#include <gtest/gtest.h>

class single_declare : public testing::Test {
protected:
    void SetUp() override {
        env.clear();
        env.doStats = false;
        std::string declare_file_path, maxsat;
        std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();
        std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / "Response";
        auto sizes = root_folder / "data" /"testing"/"declare" / ("Response_pos_neg.txt");
        declare_file_path = (root_folder / "data" /"testing"/"declare" / ("Response.powerdecl")).string();
        maxsat = (root_folder / "scripts" / ("maxsat_pipeline_singledecl_testing.yaml")).string();
        std::ifstream file{curr};
        env.load_log(TAB_SEPARATED_EVENTS, true, curr.filename(), false, file);
        std::ifstream fs{sizes};
        fs >> pos >> neg;
        env.load_model(declare_file_path);
        env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
        env.doGrounding();
        env.set_atomization_parameters("p", 10);
        env.init_atomize_tables();
        env.first_atomize_model();
        env.set_maxsat_parameters(std::filesystem::path(maxsat));
    }
public:
    Environment env;
    size_t pos, neg;
};

TEST_F(single_declare, response) {
    auto ref = env.query_model();
    ASSERT_EQ(ref.final_ensemble, TraceMaximumSatisfiability);
    for (size_t i = 0; i<ref.max_sat_per_trace.size(); i++) {
        if (i<pos)
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 1.0);
        else
            ASSERT_EQ(ref.max_sat_per_trace.at(i), 0.0);
    }
}