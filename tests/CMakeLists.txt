
enable_testing ()
add_executable(global_test fuzzy_string_matching_test.cpp
                    group_by_test.cpp
                    ../submodules/ctest/main.c )
target_link_libraries(global_test knobab yaucl_hashing)
add_test (NAME global_test COMMAND global_test)
