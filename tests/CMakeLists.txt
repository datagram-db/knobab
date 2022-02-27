add_executable(fsm_test fuzzy_string_matching_test.cpp
        ../submodules/ctest/main.c )
target_link_libraries(fsm_test knobab)
add_test (NAME fsm_test COMMAND fsm_test)

add_executable(gb_test group_by_test.cpp
                    ../submodules/ctest/main.c )
target_link_libraries(gb_test knobab yaucl_hashing)
add_test (NAME gb_test COMMAND gb_test)

add_executable(bop_test basic_operators_testing.cpp
        ../submodules/ctest/main.c )
target_link_libraries(bop_test knobab yaucl_hashing yaucl_bpm knobab_flloat Threads::Threads)
add_test (NAME bop_test COMMAND bop_test)

add_executable(until_test until_test.cpp
        ../submodules/ctest/main.c )
target_link_libraries(until_test knobab yaucl_hashing yaucl_bpm knobab_flloat Threads::Threads)
add_test (NAME until_test COMMAND until_test)