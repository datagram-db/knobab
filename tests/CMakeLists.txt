
include(GoogleTest)


add_executable(fsm_test fuzzy_string_matching_test.cpp )
target_link_libraries(fsm_test gtest_main knobab)
gtest_discover_tests(fsm_test)

add_executable(gb_test group_by_test.cpp )
target_link_libraries(gb_test gtest_main knobab yaucl_hashing)
gtest_discover_tests(gb_test)

add_executable(bop_test basic_operators_testing.cpp )
target_link_libraries(bop_test gtest_main knobab yaucl_hashing yaucl_bpm knobab_flloat Threads::Threads)
gtest_discover_tests(bop_test)

add_executable(until_test until_test.cpp)
target_link_libraries(until_test gtest_main knobab yaucl_hashing yaucl_bpm knobab_flloat Threads::Threads)
gtest_discover_tests(until_test)

add_executable(ltlf_operators_test ltlf_operators_test.cpp)
target_link_libraries(ltlf_operators_test gtest_main knobab yaucl_hashing yaucl_bpm knobab_flloat Threads::Threads)
gtest_discover_tests(ltlf_operators_test)

add_executable(single_declare single_declare_correctness.cpp)
target_link_libraries(single_declare gtest_main knobab yaucl_hashing yaucl_bpm knobab_flloat Threads::Threads)
gtest_discover_tests(single_declare)

add_executable(slow_declare single_declare_slow.cpp)
target_link_libraries(slow_declare gtest_main knobab yaucl_hashing yaucl_bpm knobab_flloat Threads::Threads)
gtest_discover_tests(slow_declare)

add_executable(complete_response testing_complete_response.cpp)
target_link_libraries(complete_response gtest_main knobab yaucl_hashing yaucl_bpm knobab_flloat Threads::Threads)
gtest_discover_tests(complete_response)