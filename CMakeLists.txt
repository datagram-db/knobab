cmake_minimum_required(VERSION 3.20)
project(knobab_server)
set(CMAKE_CXX_STANDARD 20)
set(WITH_DEMO false)
include_directories(include/knobab/server/query_manager)
include_directories(include)
include_directories(submodules/yaucl/submodules/eigen)
include_directories(submodules/httplib)
include_directories(submodules/date/include)
include_directories(submodules/json/include)
include_directories(submodules/yaucl/submodules/antlr4/runtime/Cpp/runtime/src)
include_directories(antlr4/cpp/antlr4)
include_directories(submodules/thread-pool)
add_subdirectory(submodules/yaucl)
include_directories(submodules/yaucl/include)
include_directories(submodules/yaucl/submodules/rapidcsv/src)
include_directories(submodules/yaucl/submodules/yaucl/submodules/json/include)
include_directories(submodules/yaucl/submodules/magic_enum/include)
include_directories(submodules/yaucl/submodules/CRoaring/include)
include_directories(submodules/yaucl/submodules/CRoaring/cpp)
include_directories(submodules/args)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message(WARNING "Compiling in debug mode")
    add_compile_definitions(DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
else()
    message(WARNING "Compiling in optimized mode. Level=2")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

add_executable(knobab_client src/knobab/client/client.cpp)

add_library(knobab OBJECT
        src/knobab/server/algorithms/atomization/AtomizingPipeline.cpp #Legacy atomisation
        src/knobab/server/declare/CNFDeclareDataAware.cpp
        src/knobab/server/query_manager/KnoBABQueryBaseListener.cpp
        src/SimplifiedFuzzyStringMatching.cpp
        src/knobab/server/query_manager/KnoBABQueryBaseVisitor.cpp
        src/knobab/server/declare/DeclareDataAware.cpp
        src/knobab/server/query_manager/KnoBABQueryLexer.cpp
        src/knobab/server/query_manager/KnoBABQueryListener.cpp
        src/knobab/server/query_manager/KnoBABQueryParser.cpp
        src/knobab/server/query_manager/KnoBABQueryVisitor.cpp
        src/knobab/server/query_manager/ServerQueryManager.cpp
        src/knobab/server/algorithms/grounding/grounding.cpp
        src/knobab/server/tables/CountTable.cpp
        src/knobab/server/tables/ActTable.cpp
        src/knobab/server/tables/AttributeTable.cpp
        src/knobab/server/declare/DisjunctiveDeclareDataAware.cpp
        src/knobab/server/dataStructures/oid.cpp
        src/knobab/server/dataStructures/knobab_data_loader.cpp
        src/knobab/server/tables/CountTable.cpp
        src/knobab/server/tables/ActTable.cpp
        src/knobab/server/tables/AttributeTable.cpp
        src/knobab/server/dataStructures/marked_event.cpp
        src/knobab/server/tables/KnowledgeBase.cpp
        src/knobab/server/algorithms/grounding/kb_grounding.cpp
        src/knobab/server/query_manager/Environment.cpp
        src/knobab/server/benchmarking/LoggerInformation.cpp
        src/knobab/server/algorithms/querymanager/MAXSatPipeline.cpp
        src/knobab/server/operators/base_ltlf.cpp
        src/knobab/server/ndp/KnowledgeBaseNDPLoader.cpp
        src/knobab/server/algorithms/querymanager/LTLfQueryManager.cpp
        src/knobab/server/ndp/NDPFuzzyStringMatching.cpp
        include/knobab/server/query_manager/NDPFuzzyStringMatching.h
        src/knobab/server/ndp/NDPAttributeTable.cpp
        include/knobab/server/ndp/NDPAttributeTable.h
        include/knobab/server/algorithms/querymanager/DataQuery.h
        src/knobab/server/algorithms/querymanager/DataQuery.cpp
        include/knobab/server/algorithms/querymanager/LTLfQuery.h
        src/knobab/server/algorithms/querymanager/LTLfQuery.cpp src/knobab/server/algorithms/dfa_generator/GenerateGraphFromAtomisedClause.cpp include/knobab/server/algorithms/dfa_generator/GenerateGraphFromAtomisedClause.h src/knobab/server/algorithms/dfa_generator/GenerateGraphFromEnvironment.cpp include/knobab/server/algorithms/dfa_generator/GenerateGraphFromEnvironment.h src/knobab/server/dataStructures/knobab_data_loader.cpp include/knobab/server/dataStructures/knobab_data_loader.h)

add_executable(knobab_server src/knobab/server/server.cpp)
add_executable(tests_main src/knobab/tests/main.cpp)

add_library(knobab_ndp
        src/knobab/server/ndp/count_table_rcx.cpp
        src/knobab/server/ndp/KnowledgeBaseNDPLoader.cpp
        src/knobab/server/ndp/act_table_rcx.cpp
        include/knobab/server/ndp/act_table_rcx.h
        src/knobab/server/ndp/KnowledgeBaseNDPReader.cpp)


add_library(knobab_mining
        src/knobab/mining/bolt2.cpp
        src/knobab/mining/bolt1.cpp
        include/knobab/mining/bolt2.h
        include/knobab/mining/bolt1.h
        src/knobab/mining/CountTableFPTree.cpp
        src/knobab/mining/apriori.cpp
        include/knobab/mining/apriori.h
        include/knobab/mining/refinery.h

        )

add_executable(benchmarking benchmarking/benchmarking.cpp benchmarking/benchmarking.cpp) #${BACKWARD_ENABLE})

add_executable(mining
        src/knobab/mining/mining.cpp
)

# TODO: refinery

add_executable(log_generator generate_traces.cpp)
add_executable(multimodel_mining src/knobab/mining/refinery.cpp)
add_executable(knobab2-benchmarking src/knobab/server/benchmarking_binary.cpp)


add_executable(old_log_generator src/knobab/old_log_generator.cpp)
target_link_libraries(old_log_generator Threads::Threads antlr4_shared yaucl_hashing yaucl_bpm yaucl_structures
        yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)


if(MSVC OR MSYS OR MINGW)
    target_link_libraries(knobab_server Threads::Threads antlr4_shared yaucl_hashing yaucl_bpm yaucl_structures
            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
    target_link_libraries(tests_main antlr4_shared yaucl_bpm yaucl_structures
            yaucl_numeric yaucl_data yaucl_graph yaucl_hashing yaucl_data yaucl_bpm knobab  knobab_ndp)
    target_link_libraries(benchmarking knobab antlr4_shared yaucl_hashing yaucl_data knobab_ndp
            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures)
    target_link_libraries(mining Threads::Threads knobab antlr4_shared yaucl_hashing
            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
    target_link_libraries(multimodel_mining Threads::Threads knobab antlr4_shared yaucl_hashing
            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
    target_link_libraries(knobab2-benchmarking Threads::Threads antlr4_shared yaucl_hashing yaucl_bpm yaucl_structures
            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
else()
    target_link_libraries(knobab_server Threads::Threads antlr4_static yaucl_hashing yaucl_bpm yaucl_structures
            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
    target_link_libraries(log_generator Threads::Threads antlr4_static yaucl_hashing yaucl_bpm yaucl_structures
            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
    target_link_libraries(tests_main antlr4_static yaucl_hashing yaucl_bpm yaucl_structures
            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
    target_link_libraries(mining Threads::Threads knobab antlr4_static yaucl_hashing
            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
    target_link_libraries(benchmarking knobab antlr4_static yaucl_hashing yaucl_data knobab_ndp
            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures)
    target_link_libraries(multimodel_mining Threads::Threads knobab antlr4_static yaucl_hashing
            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
    target_link_libraries(knobab2-benchmarking Threads::Threads antlr4_shared yaucl_hashing yaucl_bpm yaucl_structures
            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
endif()

#add_executable(refinery refinery.cpp)
#target_link_libraries(refinery Threads::Threads yaml-cpp antlr4_static ${CppPythonBindings}
#        yaucl_stringutils yaucl_numeric yaucl_hashing yaucl_data yaucl_bpm yaucl_graph knobab_flloat yaucl_structures yaucl_learning knobab)

enable_testing ()
#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/58b71c9ea2fa52a7412e2267895d016abe14a971.zip
#)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#link_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(mining_tests src/knobab/tests/mining_tests.cpp )
target_link_libraries(mining_tests Catch2::Catch2WithMain Threads::Threads knobab antlr4_static yaucl_hashing
        yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)

add_executable(until_tests src/knobab/tests/until_tests.cpp)
target_link_libraries(until_tests Catch2::Catch2WithMain Threads::Threads knobab antlr4_static yaucl_hashing
        yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)

add_executable(complete_response src/knobab/tests/complete_response.cpp)
target_link_libraries(complete_response Catch2::Catch2WithMain Threads::Threads knobab antlr4_static yaucl_hashing
        yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)

add_executable(basic_operators_testing src/knobab/tests/basic_operators_testing.cpp)
target_link_libraries(basic_operators_testing Catch2::Catch2WithMain Threads::Threads knobab antlr4_static yaucl_hashing
        yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)


add_executable(knobab2-edbt24-benchmarking benchmarking/edbt24.cpp)
target_link_libraries(knobab2-edbt24-benchmarking Threads::Threads antlr4_shared yaucl_hashing yaucl_bpm yaucl_structures
        yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)

add_executable(knobab_json new_pipeline.cpp)
target_link_libraries(knobab_json antlr4_static yaucl_numeric yaucl_data yaucl_learning yaucl_graph yaucl_hashing yaucl_bpm)

catch_discover_tests(mining_tests)
catch_discover_tests(until_tests)
catch_discover_tests(complete_response)
catch_discover_tests(basic_operators_testing)