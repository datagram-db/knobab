//
// Created by giacomo on 24/02/2022.
//

#define CTEST_COLOR_OK
#define CTEST_SEGFAULT

#include <vector>
#include <yaucl/hashing/pair_hash.h>
#include <yaucl/hashing/uset_hash.h>
#include <yaucl/structures/set_operations.h>

#include "../submodules/ctest/ctest.h"

CTEST_DATA(groupby) {
    std::vector <std::pair<size_t, double>> W;
};

CTEST_SETUP(groupby) {
    data->W = {{1,2}, {3,0.5}, {3, 0.7}, {2, 0.5}, {1, 7}};
    std::sort(data->W.begin(), data->W.end());
}

CTEST2(groupby, groupby) {
    auto S = GroupByKeyExtractorIgnoreKey<std::vector<std::pair<size_t, double>>::iterator, size_t, std::pair<size_t, double>>(data->W.begin(), data->W.end(), [](const std::pair<size_t, double>& x) {return x.first; });
    ASSERT_TRUE(S == (std::vector<std::vector<std::pair<size_t, double>>>{{{1,2},{1,7}}, {{2,0.5}}, {{3,0.5}, {3,0.7}}}));
}

CTEST2(groupby, cartesian_product) {
    auto S = GroupByKeyExtractorIgnoreKey<std::vector<std::pair<size_t, double>>::iterator, size_t, std::pair<size_t, double>>(data->W.begin(), data->W.end(), [](const std::pair<size_t, double>& x) {return x.first; });
    auto M = cartesian_product(S);
    ASSERT_TRUE(M ==
           (std::unordered_set<std::unordered_set<std::pair<size_t, double>>>{{{1,7},{2,0.5},{3,0.7}},
                                                                              {{1,7},{2,0.5},{3,0.5}},
                                                                              {{1,2},{2,0.5}, {3,0.7}},
                                                                              {{1,2}, {2,0.5}, {3,0.5}}}));
}