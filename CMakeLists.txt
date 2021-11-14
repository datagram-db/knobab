cmake_minimum_required(VERSION 3.16)
project(knobab)
set(CMAKE_CXX_STANDARD 20)


set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.71.0 COMPONENTS python)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_library(rapidxml INTERFACE)
add_subdirectory(submodules/antlr4/runtime/Cpp)
include_directories(submodules/antlr4/runtime/Cpp/runtime/src)
include_directories(antlr4/cpp/antlr4)
include_directories(submodules/date/include)
include_directories(submodules/json/include)

include_directories(include)
add_library(knobab STATIC src/bzdb/oid.cpp src/bzdb/CountTemplate.cpp include/bzdb/CountTemplate.h src/bzdb/ActTable.cpp include/bzdb/ActTable.h src/bzdb/AttributeTable.cpp include/bzdb/AttributeTable.h src/bzdb/KnowledgeBase.cpp include/bzdb/KnowledgeBase.h
        #src/fuzzyStringMatching/v1.0/STTGSHM_Primary.cpp
        #src/fuzzyStringMatching/v1.0/LSVM_Primary.cpp
        #src/fuzzyStringMatching/v1.0/SLHM_Primary.cpp
        #src/fuzzyStringMatching/v2.0/FuzzyMatchSerializer.cpp)
        src/yaucl/data/xml.cpp src/SimplifiedFuzzyStringMatching.cpp include/SimplifiedFuzzyStringMatching.h include/yaucl/structures/any_to_uint_bimap.h src/yaucl/strings/string_utils.cpp include/yaucl/strings/string_utils.h src/yaucl/bpm/ltlf/ltlf.cpp include/yaucl/bpm/ltlf/ltlf.h include/yaucl/graphs/FlexibleFA.h)

add_library(yaucl_numeric STATIC
                src/yaucl/numeric/numeric_base.cpp
                include/yaucl/numeric/numeric_base.h)

add_library(yaucl_hashing STATIC
                include/yaucl/hashing/evaluateHashes.h
                src/yaucl/hashing/evaluateHashes.c
                src/yaucl/hashing/hash_combine.cpp
                include/yaucl/hashing/hash_combine.h
                src/yaucl/hashing/hashing.cpp
                include/yaucl/hashing/hashing.h
                include/yaucl/hashing/pair_hash.h
                include/yaucl/hashing/vector_hash.h
                include/yaucl/hashing/uset_hash.h)
target_include_directories(yaucl_hashing PUBLIC ${CMAKE_SOURCE_DIR}/include/yaucl/hashing/ ${CMAKE_CURRENT_BINARY_DIR})

add_library(yaucl_graph STATIC
        src/yaucl/graphs/adjacency_graph.cpp
        include/yaucl/graphs/adjacency_graph.h
        include/yaucl/graphs/FlexibleGraph.h
        include/yaucl/graphs/NodeLabelBijectionGraph.h
        src/yaucl/graphs/graph_join_pm_algorithms.cpp
        src/yaucl/graphs/graph_join_pm_conversion.cpp
        include/yaucl/graphs/NodeLabelBijectionFA.h src/yaucl/graphs/adjacency_entry.cpp include/yaucl/graphs/adjacency_entry.h src/yaucl/graphs/graph_join_pm.cpp include/yaucl/graphs/graph_join_pm.h)

add_library(yaucl_data STATIC
         src/yaucl/data/xml.cpp)

add_library(yaucl_bpm STATIC
                src/yaucl/bpm/trace_visitor.cpp
                include/yaucl/bpm/trace_visitor.h
                src/yaucl/bpm/data_loader.cpp
                src/yaucl/bpm/TracesBaseListener.cpp
                src/yaucl/bpm/TracesBaseVisitor.cpp
                src/yaucl/bpm/TracesLexer.cpp
                src/yaucl/bpm/TracesListener.cpp
                src/yaucl/bpm/TracesParser.cpp
                src/yaucl/bpm/TracesVisitor.cpp
                src/yaucl/bpm/DataTraceParse.cpp src/yaucl/bpm/ltlf/DataPredicate.cpp
                include/yaucl/bpm/ltlf/DataPredicate.h
                src/yaucl/bpm/ltlf/PropositionalizedAtomsSet.cpp
                include/yaucl/bpm/ltlf/PropositionalizedAtomsSet.h)
target_link_libraries(yaucl_bpm antlr4_static yaucl_numeric yaucl_data)

add_library(knobab_flloat src/bzdb/flloat_deps/flloat_wrapper.cpp flloat_wrapper.h
        src/bzdb/flloat_deps/DOTBaseListener.cpp
        src/bzdb/flloat_deps/DOTBaseVisitor.cpp
        src/bzdb/flloat_deps/DOTLexer.cpp
        src/bzdb/flloat_deps/DOTListener.cpp
        src/bzdb/flloat_deps/DOTParser.cpp
        src/bzdb/flloat_deps/DOTVisitor.cpp
        src/bzdb/flloat_deps/ParseFFLOATDot.cpp
        include/bzdb/flloat_deps/ParseFFLOATDot.h)

add_executable(test main.cpp )
target_link_libraries(test knobab antlr4_static ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} yaucl_numeric yaucl_data yaucl_bpm knobab_flloat)