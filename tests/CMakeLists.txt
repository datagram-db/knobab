
enable_testing ()
add_executable(test fuzzy_string_matching_test.cpp
                    group_by_test.cpp
                    ../submodules/ctest/main.c )
target_link_libraries(test knobab yaucl_hashing)
add_test (NAME tests COMMAND fuzzy_string_matching_test)