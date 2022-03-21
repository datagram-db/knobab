//
// Created by giacomo on 27/02/2022.
//

#define CTEST_COLOR_OK
#define CTEST_SEGFAULT

#include <vector>
#include <yaucl/structures/set_operations.h>
#include <knobab/Environment.h>
#include <gtest/gtest.h>

#define TEST_NAME(name)        name ## _slow
#define TEST_MULTI(name) class TEST_NAME(name) : public testing::Test {\
protected:\
void SetUp() override {\
        env.clear();\
        env.doStats = false;\
        std::string declare_file_path, maxsat;\
        std::string base{#name}; \
        std::filesystem::path root_folder = std::filesystem::current_path().parent_path().parent_path();\
        std::filesystem::path curr = root_folder / "data" / "testing" / "declare" / base;\
        auto sizes = root_folder / "data" /"testing"/"declare" / (base+"_pos_neg.txt");\
        declare_file_path = (root_folder / "data" /"testing"/"declare" / (base+".powerdecl")).string();\
        maxsat = (root_folder / "scripts" / ("maxsat_pipeline_singledecl_testing_slow.yaml")).string();\
        std::ifstream file{curr};\
        env.load_log(TAB_SEPARATED_EVENTS, true, curr.filename(), false, file);\
        std::ifstream fs{sizes};\
        fs >> pos >> neg;\
        env.load_model(declare_file_path);\
        env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);\
        env.doGrounding();\
        env.set_atomization_parameters("p", 10);\
        env.init_atomize_tables();\
        env.first_atomize_model();\
        env.set_maxsat_parameters(std::filesystem::path(maxsat));\
}\
Environment env;\
size_t pos, neg;                                                \
};\

#include "common_tests.cpp"