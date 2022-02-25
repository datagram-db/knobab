//
// Created by giacomo on 25/02/2022.
//

#define CTEST_COLOR_OK
#define CTEST_SEGFAULT

#include <vector>
#include <yaucl/hashing/pair_hash.h>
#include <yaucl/hashing/uset_hash.h>
#include <yaucl/structures/set_operations.h>
#include <knobab/Environment.h>
#include <fstream>
#include <filesystem>

#include "../submodules/ctest/ctest.h"
#include "log_for_tests.h"

CTEST_DATA(basic_operators) {
    Environment env;
};

#define DATA_EMPLACE_BACK(l,trace,event,isMatch)    (l).emplace_back(std::make_pair((trace),(event)), std::make_pair((1.0),std::vector<uint16_t>{((isMatch) ? ((uint16_t)event) : (uint16_t)0)}))

CTEST_SETUP(basic_operators) {
    Environment& env = data->env;
    env.clear();
    std::filesystem::path file{"log1.txt"};
    {
        std::ofstream of{file};
        of << log1 << std::endl;
        of.close();
    }
    env.load_log(HUMAN_READABLE_YAUCL, true, file);
};

CTEST_TEARDOWN(basic_operators) {
    {
        std::filesystem::path file{"log1.txt"};
        assert(std::filesystem::exists(file));
        std::filesystem::remove(file);
    }
};

CTEST2(basic_operators, A) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        auto resultA = data->env.db.exists("A", value);

        dataContainer A;
        DATA_EMPLACE_BACK(A, 0, 0, value);
        DATA_EMPLACE_BACK(A, 3, 0, value);
        DATA_EMPLACE_BACK(A, 4, 0, value);
        DATA_EMPLACE_BACK(A, 5, 0, value);
        DATA_EMPLACE_BACK(A, 6, 0, value);
        DATA_EMPLACE_BACK(A, 7, 0, value);
        DATA_EMPLACE_BACK(A, 7, 3, value);
        DATA_EMPLACE_BACK(A, 8, 0, value);
        DATA_EMPLACE_BACK(A, 8, 3, value);
        DATA_EMPLACE_BACK(A, 9, 0, value);
        DATA_EMPLACE_BACK(A, 9, 3, value);
        DATA_EMPLACE_BACK(A, 10, 0, value);
        DATA_EMPLACE_BACK(A, 10, 2, value);
        DATA_EMPLACE_BACK(A, 11, 0, value);
        DATA_EMPLACE_BACK(A, 11, 3, value);
        DATA_EMPLACE_BACK(A, 12, 0, value);
        DATA_EMPLACE_BACK(A, 12, 2, value);

        ASSERT_TRUE(resultA == A);
    }
}

CTEST2(basic_operators, B) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        auto resultA = data->env.db.exists("B", value);

        dataContainer A;
        DATA_EMPLACE_BACK(A, 1, 0, value);
        DATA_EMPLACE_BACK(A, 3, 1, value);
        DATA_EMPLACE_BACK(A, 5, 1, value);
        DATA_EMPLACE_BACK(A, 7, 1, value);
        DATA_EMPLACE_BACK(A, 7, 2, value);
        DATA_EMPLACE_BACK(A, 8, 2, value);
        DATA_EMPLACE_BACK(A, 9, 1, value);
        DATA_EMPLACE_BACK(A, 9, 2, value);
        DATA_EMPLACE_BACK(A, 9, 4, value);
        DATA_EMPLACE_BACK(A, 9, 5, value);
        DATA_EMPLACE_BACK(A, 10, 1, value);
        DATA_EMPLACE_BACK(A, 10, 3, value);
        DATA_EMPLACE_BACK(A, 10, 4, value);
        DATA_EMPLACE_BACK(A, 11, 1, value);
        DATA_EMPLACE_BACK(A, 11, 2, value);
        DATA_EMPLACE_BACK(A, 11, 4, value);
        DATA_EMPLACE_BACK(A, 12, 1, value);
        DATA_EMPLACE_BACK(A, 12, 3, value);

        ASSERT_TRUE(resultA == A);
    }
}

CTEST2(basic_operators, C) {
    for (int i = 0; i<2; i++) {
        bool value = (i == 0);

        auto resultA = data->env.db.exists("C", value);

        dataContainer A;
        DATA_EMPLACE_BACK(A, 2, 0, value);
        DATA_EMPLACE_BACK(A, 4, 1, value);
        DATA_EMPLACE_BACK(A, 6, 1, value);
        DATA_EMPLACE_BACK(A, 8, 1, value);

        ASSERT_TRUE(resultA == A);
    }
}