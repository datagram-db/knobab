#<<<<<<< HEAD
cmake_minimum_required(VERSION 3.20)
project(knobab_server)
set(CMAKE_SYSTEM_NAME Linux)
set(WITH_DEMO false)


include_directories(/usr/include/c++/14/)
#set(CMAKE_C_COMPILER clang-19 CACHE INTERNAL "C compiler")
#set(CMAKE_CXX_COMPILER clang++-19 CACHE INTERNAL "C++ compiler")
set(CMAKE_CXX_STANDARD 23)
set(WITH_DEMO false)
add_definitions(-DNO_PYBIND)
include_directories(submodules/yaucl/submodules/l-bfgs-b/include)
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
#set(THREADS_PREFER_PTHREAD_FLAG FALSE)
#find_package(Threads REQUIRED)

include(FetchContent)
## Also requires one of: libbfd (gnu binutils), libdwarf, libdw (elfutils)
#FetchContent_Declare(backward
#        GIT_REPOSITORY https://github.com/bombela/backward-cpp
#        GIT_TAG v1.6  # or a version tag, such as v1.6
#        #SYSTEM          # optional, the Backward include directory will be treated as system directory
#        )
#FetchContent_MakeAvailable(backward)
#include_directories(${backward_SOURCE_DIR})

add_library(catch24 OBJECT
        submodules/catch24/C/SP_Summaries.cpp
        submodules/catch24/C/DN_Spread_Std.h
        submodules/catch24/C/DN_HistogramMode_10.cpp
        submodules/catch24/C/SC_FluctAnal.cpp
        submodules/catch24/C/SB_CoarseGrain.cpp
        submodules/catch24/C/splinefit.h
        submodules/catch24/C/SB_CoarseGrain.h
        submodules/catch24/C/fft.cpp
        submodules/catch24/C/SC_FluctAnal.h
        submodules/catch24/C/SB_MotifThree.h
        submodules/catch24/C/helper_functions.h
        submodules/catch24/C/fft.h
        submodules/catch24/C/SP_Summaries.h
        submodules/catch24/C/IN_AutoMutualInfoStats.h
        submodules/catch24/C/stats.cpp
        submodules/catch24/C/FC_LocalSimple.cpp
        submodules/catch24/C/CO_AutoCorr.h
        submodules/catch24/C/PD_PeriodicityWang.h
        submodules/catch24/C/CO_AutoCorr.cpp
        submodules/catch24/C/butterworth.cpp
        submodules/catch24/C/IN_AutoMutualInfoStats.cpp
        submodules/catch24/C/SB_TransitionMatrix.h
        submodules/catch24/C/PD_PeriodicityWang.cpp
        submodules/catch24/C/MD_hrv.cpp
        submodules/catch24/C/DN_OutlierInclude.h
        submodules/catch24/C/SB_BinaryStats.cpp
        submodules/catch24/C/helper_functions.cpp
        submodules/catch24/C/runAllTS.sh
        submodules/catch24/C/DN_HistogramMode_10.h
        submodules/catch24/C/DN_HistogramMode_5.cpp
        submodules/catch24/C/MD_hrv.h
        submodules/catch24/C/DN_Mean.cpp
        submodules/catch24/C/stats.h
        submodules/catch24/C/SB_TransitionMatrix.cpp
        submodules/catch24/C/FC_LocalSimple.h
        submodules/catch24/C/splinefit.cpp
        submodules/catch24/C/DN_Mean.h
        submodules/catch24/C/SB_BinaryStats.h
        submodules/catch24/C/DN_Spread_Std.cpp
        submodules/catch24/C/histcounts.cpp
        submodules/catch24/C/histcounts.h
        submodules/catch24/C/butterworth.h
        submodules/catch24/C/DN_HistogramMode_5.h
        submodules/catch24/C/DN_OutlierInclude.cpp
        submodules/catch24/C/SB_MotifThree.cpp
)

add_library(json_writer OBJECT
        submodules/jsonwriter/src/json_writer.cpp
        submodules/jsonwriter/include/json_writer.hpp)

include_directories(submodules/jsonwriter/include)
include_directories(submodules/cpp-lru-cache/include)
include_directories(submodules/json/include)
add_subdirectory(submodules/capnproto)
include_directories(submodules/thread_pool)
include_directories(submodules/catch24/)
include_directories(include/capnpn)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message(WARNING "Compiling in debug mode")
    add_compile_definitions(DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
else()
    message(WARNING "Compiling in optimized mode. Level=2")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

#add_executable(knobab_client src/knobab/client/client.cpp)

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
        src/knobab/server/algorithms/querymanager/LTLfQuery.cpp src/knobab/server/algorithms/dfa_generator/GenerateGraphFromAtomisedClause.cpp include/knobab/server/algorithms/dfa_generator/GenerateGraphFromAtomisedClause.h src/knobab/server/algorithms/dfa_generator/GenerateGraphFromEnvironment.cpp include/knobab/server/algorithms/dfa_generator/GenerateGraphFromEnvironment.h src/knobab/server/dataStructures/knobab_data_loader.cpp include/knobab/server/dataStructures/knobab_data_loader.h
        include/knobab/v30/EMeriTAte.h)


#add_executable(knobab_server src/knobab/server/server.cpp)
#add_executable(tests_main src/knobab/tests/main.cpp)

add_library(knobab_ndp
        src/knobab/server/ndp/count_table_rcx.cpp
        src/knobab/server/ndp/KnowledgeBaseNDPLoader.cpp
        src/knobab/server/ndp/act_table_rcx.cpp
        include/knobab/server/ndp/act_table_rcx.h
        src/knobab/server/ndp/KnowledgeBaseNDPReader.cpp)


add_library(knobab_mining
##        src/knobab/mining/bolt2.cpp
#        src/knobab/mining/bolt1.cpp
##        include/knobab/mining/bolt2.h
#        include/knobab/mining/bolt1.h
        src/knobab/mining/CountTableFPTree.cpp
        src/knobab/mining/apriori.cpp
        include/knobab/mining/apriori.h
#        include/knobab/mining/refinery.h
        src/knobab/dataStructures/polyadic/myParser.cpp include/knobab/server/dataStructures/polyadic/myParser.h include/knobab/server/dataStructures/polyadic.h src/knobab/mining/polyadic/polyadic_loading.cpp include/knobab/mining/polyadic/polyadic_loading.h src/knobab/mining/polyadic/polyadic_bolt.cpp include/knobab/mining/polyadic/polyadic_bolt.h include/knobab/mining/polyadic/lattice.h
        include/knobab/mining/polyadic/payload_act_tracker.h
)


#add_executable(benchmarking benchmarking/benchmarking.cpp benchmarking/benchmarking.cpp) #${BACKWARD_ENABLE})
#
#add_executable(mining
#        src/knobab/mining/mining.cpp
#)
#
#
#add_executable(log_generator generate_traces.cpp)
#add_executable(multimodel_mining src/knobab/mining/refinery.cpp)
#add_executable(knobab2-benchmarking src/knobab/server/benchmarking_binary.cpp)
#if(MSVC OR MSYS OR MINGW)
#    target_link_libraries(knobab_server Threads::Threads antlr4_shared yaucl_hashing yaucl_bpm yaucl_structures
#            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
#    target_link_libraries(tests_main antlr4_shared yaucl_bpm yaucl_structures
#            yaucl_numeric yaucl_data yaucl_graph yaucl_hashing yaucl_data yaucl_bpm knobab  knobab_ndp)
#    target_link_libraries(benchmarking knobab antlr4_shared yaucl_hashing yaucl_data knobab_ndp
#            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures)
#    target_link_libraries(mining Threads::Threads knobab antlr4_shared yaucl_hashing
#            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
#    target_link_libraries(multimodel_mining Threads::Threads knobab antlr4_shared yaucl_hashing
#            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
#    target_link_libraries(knobab2-benchmarking Threads::Threads antlr4_shared yaucl_hashing yaucl_bpm yaucl_structures
#            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
#else()
##    target_link_libraries(knobab_server Threads::Threads antlr4_static yaucl_hashing yaucl_bpm yaucl_structures
##            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
##    target_link_libraries(log_generator Threads::Threads antlr4_static yaucl_hashing yaucl_bpm yaucl_structures
##            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
##    target_link_libraries(tests_main antlr4_static yaucl_hashing yaucl_bpm yaucl_structures
##            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
##    target_link_libraries(mining Threads::Threads knobab antlr4_static yaucl_hashing
##            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
##    target_link_libraries(benchmarking knobab antlr4_static yaucl_hashing yaucl_data knobab_ndp
##            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures)
##    target_link_libraries(multimodel_mining Threads::Threads knobab antlr4_static yaucl_hashing
##            yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
##    target_link_libraries(knobab2-benchmarking Threads::Threads antlr4_shared yaucl_hashing yaucl_bpm yaucl_structures
##            yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
#endif()
#enable_testing ()
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
#
#
#Include(FetchContent)
#
#FetchContent_Declare(
#        Catch2
#        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#        GIT_TAG        v3.5.2 # or a later release
#)

add_executable(knobab_json new_pipeline.cpp src/knobab/v30/EMeriTAte.cpp
        src/knobab/mining/polyadic/polyadic_learning.cpp
        include/knobab/mining/polyadic/polyadic_learning.h
        src/knobab/mining/polyadic/polyadic_mining.cpp
        include/knobab/mining/polyadic/polyadic_loading.h
        include/knobab/mining/polyadic/polyadic_mining.h
        include/knobab/mining/polyadic/commons.h
        include/knobab/mining/polyadic/payload_preserver.h
        src/knobab/mining/polyadic/payload_preserver.cpp
        src/knobab/mining/polyadic/payload_act_tracker.cpp)

target_link_libraries(knobab_json antlr4_static yaucl_hashing yaucl_bpm yaucl_structures
        yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp   yaucl_learning  knobab_mining)

add_executable(DTMining dt_mine_main.cpp
        src/dt_mining/utils/fish.cpp
        src/dt_mining/utils/BasicRecord.cpp
        src/dt_mining/utils/RecordHandling.cpp
        src/dt_mining/data/PolyadicJSONSerialize.cpp
        src/dt_mining/data/MultivariateTimeSeries.cpp
        src/capnpn/schema.propo.c++
        submodules/jsonwriter/src/json_writer.cpp
        src/dt_mining/data/TemporaryCapnpnConstituentSerializer.cpp
        src/dt_mining/benchmarking/FineGrainedStats.cpp
        src/dt_mining/benchmarking/SegmentStats.cpp
        src/dt_mining/benchmarking/TraceStats.cpp
        include/dt_mining/benchmarking/DatasetStats.h
        src/dt_mining/benchmarking/DatasetStats.cpp
        include/dt_mining/dt_mining.h
        src/dt_mining/dt_mining.cpp
)
target_link_libraries(DTMining  PRIVATE catch24 CapnProto::kj CapnProto::capnp yaucl_numeric)

#FetchContent_MakeAvailable(Catch2)
#list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
#include(CTest)
#include(Catch)
#
#add_executable(mining_tests src/knobab/tests/mining_tests.cpp )
#target_link_libraries(mining_tests Catch2::Catch2WithMain Threads::Threads knobab antlr4_static yaucl_hashing
#        yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
#
#add_executable(polyadic_mining_tests src/knobab/tests/polyadic_mining_tests.cpp )
#target_link_libraries(polyadic_mining_tests Catch2::Catch2WithMain Threads::Threads knobab antlr4_static yaucl_hashing
#        yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining )
#
#
#add_executable(until_tests src/knobab/tests/until_tests.cpp)
#target_link_libraries(until_tests Catch2::Catch2WithMain Threads::Threads knobab antlr4_static yaucl_hashing
#        yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
#
#add_executable(complete_response src/knobab/tests/complete_response.cpp)
#target_link_libraries(complete_response Catch2::Catch2WithMain Threads::Threads knobab antlr4_static yaucl_hashing
#        yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
#
#add_executable(basic_operators_testing src/knobab/tests/basic_operators_testing.cpp include/knobab/server/dataStructures/polyadic/basic_polyadic.h)
#target_link_libraries(basic_operators_testing Catch2::Catch2WithMain Threads::Threads knobab antlr4_static yaucl_hashing
#        yaucl_stringutils yaucl_numeric  yaucl_data yaucl_bpm yaucl_graph yaucl_structures yaucl_data knobab_ndp yaucl_learning knobab_mining)
#
#
#add_executable(knobab2-edbt24-benchmarking benchmarking/edbt24.cpp)
#target_link_libraries(knobab2-edbt24-benchmarking Threads::Threads antlr4_shared yaucl_hashing yaucl_bpm yaucl_structures
#        yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp)
#
#
#
#catch_discover_tests(mining_tests)
#catch_discover_tests(until_tests)
#catch_discover_tests(complete_response)
#catch_discover_tests(basic_operators_testing)
#catch_discover_tests(polyadic_mining_tests)
#=======
#cmake_minimum_required(VERSION 3.28)
#project(DTMining)
#
#IF(CMAKE_BUILD_TYPE MATCHES Debug)
#    message(WARNING "Compiling in debug mode")
#    add_compile_definitions(DEBUG)
#    set(CMAKE_CXX_FLAGS_DEBUG "-g")
#else()
#    message(WARNING "Compiling in optimized mode. Level=2")
#    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
#ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
#
#set(CMAKE_SYSTEM_NAME Linux)
#set(WITH_DEMO false)
#include_directories(/usr/include/c++/13/)
#set(CMAKE_C_COMPILER clang-19 CACHE INTERNAL "C compiler")
#set(CMAKE_CXX_COMPILER clang++-19 CACHE INTERNAL "C++ compiler")
#set(CMAKE_CXX_STANDARD 23)
#
#add_library(catch24 OBJECT
#        submodules/catch24/C/SP_Summaries.cpp
#        submodules/catch24/C/DN_Spread_Std.h
#        submodules/catch24/C/DN_HistogramMode_10.cpp
#        submodules/catch24/C/SC_FluctAnal.cpp
#        submodules/catch24/C/SB_CoarseGrain.cpp
#        submodules/catch24/C/splinefit.h
#        submodules/catch24/C/SB_CoarseGrain.h
#        submodules/catch24/C/fft.cpp
#        submodules/catch24/C/SC_FluctAnal.h
#        submodules/catch24/C/SB_MotifThree.h
#        submodules/catch24/C/helper_functions.h
#        submodules/catch24/C/fft.h
#        submodules/catch24/C/SP_Summaries.h
#        submodules/catch24/C/IN_AutoMutualInfoStats.h
#        submodules/catch24/C/stats.cpp
#        submodules/catch24/C/FC_LocalSimple.cpp
#        submodules/catch24/C/CO_AutoCorr.h
#        submodules/catch24/C/PD_PeriodicityWang.h
#        submodules/catch24/C/CO_AutoCorr.cpp
#        submodules/catch24/C/butterworth.cpp
#        submodules/catch24/C/IN_AutoMutualInfoStats.cpp
#        submodules/catch24/C/SB_TransitionMatrix.h
#        submodules/catch24/C/PD_PeriodicityWang.cpp
#        submodules/catch24/C/MD_hrv.cpp
#        submodules/catch24/C/DN_OutlierInclude.h
#        submodules/catch24/C/SB_BinaryStats.cpp
#        submodules/catch24/C/helper_functions.cpp
#        submodules/catch24/C/runAllTS.sh
#        submodules/catch24/C/DN_HistogramMode_10.h
#        submodules/catch24/C/DN_HistogramMode_5.cpp
#        submodules/catch24/C/MD_hrv.h
#        submodules/catch24/C/DN_Mean.cpp
#        submodules/catch24/C/stats.h
#        submodules/catch24/C/SB_TransitionMatrix.cpp
#        submodules/catch24/C/FC_LocalSimple.h
#        submodules/catch24/C/splinefit.cpp
#        submodules/catch24/C/DN_Mean.h
#        submodules/catch24/C/SB_BinaryStats.h
#        submodules/catch24/C/DN_Spread_Std.cpp
#        submodules/catch24/C/histcounts.cpp
#        submodules/catch24/C/histcounts.h
#        submodules/catch24/C/butterworth.h
#        submodules/catch24/C/DN_HistogramMode_5.h
#        submodules/catch24/C/DN_OutlierInclude.cpp
#        submodules/catch24/C/SB_MotifThree.cpp
#        include/dt_mining/benchmarking/SegmentStats.h
#        include/dt_mining/benchmarking/TraceStats.h
#)
#
#add_library(json_writer OBJECT
#submodules/jsonwriter/src/json_writer.cpp
#submodules/jsonwriter/include/json_writer.hpp)
#
#include_directories(submodules/jsonwriter/include)
#include_directories(submodules/date/include)
#include_directories(submodules/cpp-lru-cache/include)
#include_directories(submodules/json/include)
#add_subdirectory(submodules/capnproto)
#
#include_directories(submodules/thread_pool)
#include_directories(submodules/catch24/)
#include_directories(include/)
#include_directories(include/capnpn)
#add_executable(DTMining main.cpp
#        soruce/dt_mining/utils/fish.cpp
#        soruce/dt_mining/utils/BasicRecord.cpp
#        soruce/dt_mining/utils/RecordHandling.cpp
#        soruce/dt_mining/data/PolyadicJSONSerialize.cpp
#        soruce/dt_mining/data/dateparse.cpp
#        soruce/dt_mining/data/MultivariateTimeSeries.cpp
#        soruce/capnpn/schema.propo.c++
#        submodules/jsonwriter/src/json_writer.cpp
#        soruce/dt_mining/data/TemporaryCapnpnConstituentSerializer.cpp
#        soruce/dt_mining/benchmarking/FineGrainedStats.cpp
#        soruce/dt_mining/benchmarking/SegmentStats.cpp
#        soruce/dt_mining/benchmarking/TraceStats.cpp
#        include/dt_mining/benchmarking/DatasetStats.h
#        soruce/dt_mining/benchmarking/DatasetStats.cpp
#        include/dt_mining/dt_mining.h
#        soruce/dt_mining/dt_mining.cpp
#)
#target_link_libraries(DTMining  PRIVATE catch24 CapnProto::kj CapnProto::capnp )
##target_link_libraries(DTMining /usr/lib/gcc/x86_64-linux-gnu/11/ /usr/lib/x86_64-linux-gnu/)
#>>>>>>> dtmining
