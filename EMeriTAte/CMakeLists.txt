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
include_directories(../submodules/yaucl/include)
include_directories(../submodules/yaucl/submodules/rapidcsv/src)
include_directories(../submodules/yaucl/submodules/yaucl/submodules/json/include)
include_directories(../submodules/yaucl/submodules/magic_enum/include)
include_directories(../submodules/yaucl/submodules/CRoaring/include)
include_directories(../submodules/yaucl/submodules/CRoaring/cpp)
include_directories(../submodules/args)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

include(FetchContent)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_subdirectory(../submodules/pybind11 binbuild/pybind11)

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
        ../src/knobab/mining/bolt2.cpp
        ../src/knobab/mining/bolt1.cpp
        ../include/knobab/mining/bolt2.h
        ../include/knobab/mining/bolt1.h
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
        ../include/knobab/mining/polyadic/polyadic_mining.h)

target_link_libraries(knobab_emeritate_support PRIVATE pybind11::headers pybind11::module Threads::Threads antlr4_static yaucl_hashing yaucl_bpm yaucl_structures
        yaucl_numeric yaucl_data yaucl_graph  yaucl_data yaucl_bpm knobab  knobab_ndp   yaucl_learning  knobab_mining)


install(TARGETS knobab_emeritate_support LIBRARY DESTINATION .)

