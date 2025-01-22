cmake_minimum_required(VERSION 3.20)
project(knobab_server)
set(CMAKE_CXX_STANDARD 20)
set(WITH_DEMO false)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(PYBIND11_NEWPYTHON ON)
set(PYBIND11_PYTHON_VERSION 3.8 CACHE STRING "")
set(PYBIND11_PYTHON_VERSION 3)
set(PYBIND11_FINDPYTHON ON)
find_package(PythonInterp REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
set(USE_COROUTINES OFF)
set(IGNORE_ROCKS_DB ON)

#include_directories(/usr/include/c++/14/)
set(CMAKE_C_COMPILER /usr/bin/clang-19 CACHE INTERNAL "C compiler")
set(CMAKE_CXX_COMPILER /usr/bin/clang++-19 CACHE INTERNAL "C++ compiler")
include_directories(../submodules/yaucl/submodules/l-bfgs-b/include)
include_directories(../include/knobab/server/query_manager)
include_directories(../include)
include_directories(../submodules/yaucl/submodules/eigen)
include_directories(../submodules/httplib)
include_directories(../submodules/date/include)
include_directories(../submodules/json/include)
include_directories(../submodules/yaucl/submodules/antlr4/runtime/Cpp/runtime/src)
include_directories(../antlr4/cpp/antlr4)
include_directories(../submodules/thread-pool)
add_subdirectory(../submodules/yaucl binbuild/yaucl)
add_subdirectory(../submodules/capnproto binbuild/serialization)
include_directories(../include/capnpn)
include_directories(../submodules/yaucl/include)
include_directories(../submodules/yaucl/submodules/rapidcsv/src)
include_directories(../submodules/yaucl/submodules/yaucl/submodules/json/include)
include_directories(../submodules/yaucl/submodules/magic_enum/include)
include_directories(../submodules/yaucl/submodules/CRoaring/include)
include_directories(../submodules/yaucl/submodules/CRoaring/cpp)
include_directories(../submodules/args)
include_directories(../submodules/catch24)
include_directories(../submodules/jsonwriter/include)
include_directories(../submodules/cpp-lru-cache/include)
include_directories(../submodules/thread_pool)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

include(FetchContent)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_subdirectory(../submodules/pybind11 binbuild/pybind11)

add_library(catch24 OBJECT
        ../submodules/catch24/C/SP_Summaries.cpp
        ../submodules/catch24/C/DN_Spread_Std.h
        ../submodules/catch24/C/DN_HistogramMode_10.cpp
        ../submodules/catch24/C/SC_FluctAnal.cpp
        ../submodules/catch24/C/SB_CoarseGrain.cpp
        ../submodules/catch24/C/splinefit.h
        ../submodules/catch24/C/SB_CoarseGrain.h
        ../submodules/catch24/C/fft.cpp
        ../submodules/catch24/C/SC_FluctAnal.h
        ../submodules/catch24/C/SB_MotifThree.h
        ../submodules/catch24/C/helper_functions.h
        ../submodules/catch24/C/fft.h
        ../submodules/catch24/C/SP_Summaries.h
        ../submodules/catch24/C/IN_AutoMutualInfoStats.h
        ../submodules/catch24/C/stats.cpp
        ../submodules/catch24/C/FC_LocalSimple.cpp
        ../submodules/catch24/C/CO_AutoCorr.h
        ../submodules/catch24/C/PD_PeriodicityWang.h
        ../submodules/catch24/C/CO_AutoCorr.cpp
        ../submodules/catch24/C/butterworth.cpp
        ../submodules/catch24/C/IN_AutoMutualInfoStats.cpp
        ../submodules/catch24/C/SB_TransitionMatrix.h
        ../submodules/catch24/C/PD_PeriodicityWang.cpp
        ../submodules/catch24/C/MD_hrv.cpp
        ../submodules/catch24/C/DN_OutlierInclude.h
        ../submodules/catch24/C/SB_BinaryStats.cpp
        ../submodules/catch24/C/helper_functions.cpp
        ../submodules/catch24/C/runAllTS.sh
        ../submodules/catch24/C/DN_HistogramMode_10.h
        ../submodules/catch24/C/DN_HistogramMode_5.cpp
        ../submodules/catch24/C/MD_hrv.h
        ../submodules/catch24/C/DN_Mean.cpp
        ../submodules/catch24/C/stats.h
        ../submodules/catch24/C/SB_TransitionMatrix.cpp
        ../submodules/catch24/C/FC_LocalSimple.h
        ../submodules/catch24/C/splinefit.cpp
        ../submodules/catch24/C/DN_Mean.h
        ../submodules/catch24/C/SB_BinaryStats.h
        ../submodules/catch24/C/DN_Spread_Std.cpp
        ../submodules/catch24/C/histcounts.cpp
        ../submodules/catch24/C/histcounts.h
        ../submodules/catch24/C/butterworth.h
        ../submodules/catch24/C/DN_HistogramMode_5.h
        ../submodules/catch24/C/DN_OutlierInclude.cpp
        ../submodules/catch24/C/SB_MotifThree.cpp
)

add_library(knobab OBJECT
        ../src/knobab/server/algorithms/atomization/AtomizingPipeline.cpp #Legacy atomisation
        ../src/knobab/server/declare/CNFDeclareDataAware.cpp
        ../src/knobab/server/query_manager/KnoBABQueryBaseListener.cpp
        ../src/SimplifiedFuzzyStringMatching.cpp
        ../src/knobab/server/query_manager/KnoBABQueryBaseVisitor.cpp
        ../src/knobab/server/declare/DeclareDataAware.cpp
        ../src/knobab/server/query_manager/KnoBABQueryLexer.cpp
        ../src/knobab/server/query_manager/KnoBABQueryListener.cpp
        ../src/knobab/server/query_manager/KnoBABQueryParser.cpp
        ../src/knobab/server/query_manager/KnoBABQueryVisitor.cpp
        ../src/knobab/server/query_manager/ServerQueryManager.cpp
        ../src/knobab/server/algorithms/grounding/grounding.cpp
        ../src/knobab/server/tables/CountTable.cpp
        ../src/knobab/server/tables/ActTable.cpp
        ../src/knobab/server/tables/AttributeTable.cpp
        ../src/knobab/server/declare/DisjunctiveDeclareDataAware.cpp
        ../src/knobab/server/dataStructures/oid.cpp
        ../src/knobab/server/dataStructures/knobab_data_loader.cpp
        ../src/knobab/server/tables/CountTable.cpp
        ../src/knobab/server/tables/ActTable.cpp
        ../src/knobab/server/tables/AttributeTable.cpp
        ../src/knobab/server/dataStructures/marked_event.cpp
        ../src/knobab/server/tables/KnowledgeBase.cpp
        ../src/knobab/server/algorithms/grounding/kb_grounding.cpp
        ../src/knobab/server/query_manager/Environment.cpp
        ../src/knobab/server/benchmarking/LoggerInformation.cpp
        ../src/knobab/server/algorithms/querymanager/MAXSatPipeline.cpp
        ../src/knobab/server/operators/base_ltlf.cpp
        ../src/knobab/server/ndp/KnowledgeBaseNDPLoader.cpp
        ../src/knobab/server/algorithms/querymanager/LTLfQueryManager.cpp
        ../src/knobab/server/ndp/NDPFuzzyStringMatching.cpp
        ../include/knobab/server/query_manager/NDPFuzzyStringMatching.h
        ../src/knobab/server/ndp/NDPAttributeTable.cpp
        ../include/knobab/server/ndp/NDPAttributeTable.h
        ../include/knobab/server/algorithms/querymanager/DataQuery.h
        ../src/knobab/server/algorithms/querymanager/DataQuery.cpp
        ../include/knobab/server/algorithms/querymanager/LTLfQuery.h
        ../src/knobab/server/algorithms/querymanager/LTLfQuery.cpp 
        ../src/knobab/server/algorithms/dfa_generator/GenerateGraphFromAtomisedClause.cpp 
        ../include/knobab/server/algorithms/dfa_generator/GenerateGraphFromAtomisedClause.h 
        ../src/knobab/server/algorithms/dfa_generator/GenerateGraphFromEnvironment.cpp 
        ../include/knobab/server/algorithms/dfa_generator/GenerateGraphFromEnvironment.h 
        ../src/knobab/server/dataStructures/knobab_data_loader.cpp 
        ../include/knobab/server/dataStructures/knobab_data_loader.h
        ../include/knobab/v30/EMeriTAte.h)

add_library(knobab_ndp
        ../src/knobab/server/ndp/count_table_rcx.cpp
        ../src/knobab/server/ndp/KnowledgeBaseNDPLoader.cpp
        ../src/knobab/server/ndp/act_table_rcx.cpp
        ../include/knobab/server/ndp/act_table_rcx.h
        ../src/knobab/server/ndp/KnowledgeBaseNDPReader.cpp)


add_library(knobab_mining
        ../src/knobab/mining/CountTableFPTree.cpp
        ../src/knobab/mining/apriori.cpp
        ../include/knobab/mining/apriori.h
        ../include/knobab/mining/refinery.h
        ../src/knobab/dataStructures/polyadic/myParser.cpp 
        ../include/knobab/server/dataStructures/polyadic/myParser.h 
        ../include/knobab/server/dataStructures/polyadic.h 
        ../src/knobab/mining/polyadic/polyadic_loading.cpp 
        ../include/knobab/mining/polyadic/polyadic_loading.h 
        ../src/knobab/mining/polyadic/polyadic_bolt.cpp 
        ../include/knobab/mining/polyadic/polyadic_bolt.h 
        ../include/knobab/mining/polyadic/lattice.h
        )

pybind11_add_module(knobab_emeritate_support
        ../src/knobab/v30/EMeriTAte.cpp
        ../src/knobab/mining/polyadic/polyadic_learning.cpp
        ../include/knobab/mining/polyadic/polyadic_learning.h
        ../src/knobab/mining/polyadic/polyadic_mining.cpp
        ../include/knobab/mining/polyadic/polyadic_mining.h

        ../include/knobab/mining/polyadic/payload_preserver.h
        ../src/knobab/mining/polyadic/payload_preserver.cpp
        ../src/knobab/mining/polyadic/payload_act_tracker.cpp
#        dt_mine_main.cpp
        ../src/dt_mining/utils/fish.cpp
        ../src/dt_mining/utils/BasicRecord.cpp
        ../src/dt_mining/utils/RecordHandling.cpp
        ../src/dt_mining/data/PolyadicJSONSerialize.cpp
        ../src/dt_mining/data/MultivariateTimeSeries.cpp
        ../src/capnpn/schema.propo.c++
        ../submodules/jsonwriter/src/json_writer.cpp
        ../src/dt_mining/data/TemporaryCapnpnConstituentSerializer.cpp
        ../src/dt_mining/benchmarking/FineGrainedStats.cpp
        ../src/dt_mining/benchmarking/SegmentStats.cpp
        ../src/dt_mining/benchmarking/TraceStats.cpp
        ../include/dt_mining/benchmarking/DatasetStats.h
        ../src/dt_mining/benchmarking/DatasetStats.cpp
        ../include/dt_mining/dt_mining.h
        ../src/dt_mining/dt_mining.cpp)

target_link_libraries(knobab_emeritate_support PRIVATE pybind11::headers pybind11::module Threads::Threads antlr4_static yaucl_hashing yaucl_bpm yaucl_structures
        yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp   yaucl_learning  knobab_mining
        catch24 CapnProto::kj CapnProto::capnp )


install(TARGETS knobab_emeritate_support LIBRARY DESTINATION .)

