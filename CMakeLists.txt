cmake_minimum_required(VERSION 3.16)
project(knobab)
set(CMAKE_CXX_STANDARD 20)
include_directories(submodules/rapidxml-1.13)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.74 REQUIRED COMPONENTS python)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(submodules/antlr4/runtime/Cpp)
include_directories(submodules/antlr4/runtime/Cpp/runtime/src)
include_directories(antlr4/cpp/antlr4)
include_directories(submodules/date/include)
include_directories(submodules/json/include)

include_directories(submodules/magic_enum/include)

include_directories(include)
add_library(knobab STATIC src/knobab/oid.cpp src/knobab/CountTemplate.cpp include/knobab/CountTemplate.h src/knobab/ActTable.cpp include/knobab/ActTable.h src/knobab/AttributeTable.cpp include/knobab/AttributeTable.h src/knobab/KnowledgeBase.cpp include/knobab/KnowledgeBase.h
        #src/fuzzyStringMatching/v1.0/STTGSHM_Primary.cpp
        #src/fuzzyStringMatching/v1.0/LSVM_Primary.cpp
        #src/fuzzyStringMatching/v1.0/SLHM_Primary.cpp
        #src/fuzzyStringMatching/v2.0/FuzzyMatchSerializer.cpp)
        src/yaucl/data/xml.cpp src/SimplifiedFuzzyStringMatching.cpp include/SimplifiedFuzzyStringMatching.h include/yaucl/structures/any_to_uint_bimap.h src/yaucl/strings/string_utils.cpp include/yaucl/strings/string_utils.h src/yaucl/bpm/structures/ltlf/ltlf.cpp include/yaucl/bpm/structures/ltlf/ltlf.h include/yaucl/graphs/FlexibleFA.h)

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
        include/yaucl/graphs/NodeLabelBijectionFA.h
        src/yaucl/graphs/adjacency_entry.cpp
        include/yaucl/graphs/adjacency_entry.h
        src/yaucl/graphs/graph_join_pm.cpp
        include/yaucl/graphs/graph_join_pm.h
        src/yaucl/graphs/graph_join_pm_conversion.cpp
        src/yaucl/graphs/graph_join_pm_algorithms.cpp)

add_library(yaucl_data STATIC
         src/yaucl/data/xml.cpp include/yaucl/data/json.h)

add_library(yaucl_bpm STATIC
        src/yaucl/bpm/structures/declare/DeclareModelParse.cpp
        include/yaucl/bpm/structures/declare/DeclareModelParse.h
        src/yaucl/bpm/structures/log/trace_visitor.cpp
        include/yaucl/bpm/structures/log/trace_visitor.h
        src/yaucl/bpm/structures/log/data_loader.cpp
        src/yaucl/bpm/structures/log/TracesBaseListener.cpp
        src/yaucl/bpm/structures/log/TracesBaseVisitor.cpp
        src/yaucl/bpm/structures/log/TracesLexer.cpp
        src/yaucl/bpm/structures/log/TracesListener.cpp
        src/yaucl/bpm/structures/log/TracesParser.cpp
        src/yaucl/bpm/structures/log/TracesVisitor.cpp
        src/yaucl/bpm/structures/log/DataTraceParse.cpp
        src/yaucl/bpm/structures/commons/DataPredicate.cpp
        include/yaucl/bpm/structures/commons/DataPredicate.h
        src/yaucl/bpm/structures/ltlf/PropositionalizedAtomsSet.cpp
        include/yaucl/bpm/structures/ltlf/PropositionalizedAtomsSet.h
        src/yaucl/bpm/structures/declare/DADBaseListener.cpp
        src/yaucl/bpm/structures/declare/DADBaseVisitor.cpp
        src/yaucl/bpm/structures/declare/DADListener.cpp
        src/yaucl/bpm/structures/declare/DADLexer.cpp
        src/yaucl/bpm/structures/declare/DADParser.cpp
        src/yaucl/bpm/structures/declare/DADVisitor.cpp
        src/yaucl/bpm/structures/declare/DeclareDataAware.cpp
        include/yaucl/bpm/structures/declare/DeclareDataAware.h
        src/yaucl/bpm/algos/transformations/declare_to_dfa/DeclareNoDataTemplateCollect.cpp
        include/yaucl/bpm/algos/transformations/declare_to_dfa/DeclareNoDataTemplateCollect.h
        src/yaucl/bpm/algos/transformations/declare_to_dfa/TemplateCollectResult.cpp
        include/yaucl/bpm/algos/transformations/declare_to_dfa/TemplateCollectResult.h src/yaucl/bpm/structures/declare/DisjunctiveDeclareDataAware.cpp include/yaucl/bpm/structures/declare/DisjunctiveDeclareDataAware.h)
target_link_libraries(yaucl_bpm antlr4_static yaucl_numeric yaucl_data yaucl_graph)

add_library(knobab_flloat STATIC
        src/knobab/flloat_deps/flloat_wrapper.cpp
        src/knobab/flloat_deps/DOTBaseListener.cpp
        src/knobab/flloat_deps/DOTBaseVisitor.cpp
        src/knobab/flloat_deps/DOTLexer.cpp
        src/knobab/flloat_deps/DOTListener.cpp
        src/knobab/flloat_deps/DOTParser.cpp
        src/knobab/flloat_deps/DOTVisitor.cpp
        src/knobab/flloat_deps/ParseFFLOATDot.cpp
        src/knobab/flloat_deps/FLLOATPropBaseListener.cpp
        src/knobab/flloat_deps/FLLOATPropBaseVisitor.cpp
        src/knobab/flloat_deps/FLLOATPropLexer.cpp
        src/knobab/flloat_deps/FLLOATPropListener.cpp
        src/knobab/flloat_deps/FLLOATPropParser.cpp
        src/knobab/flloat_deps/FLLOATPropVisitor.cpp
        src/knobab/flloat_deps/FLLOATSimplePropParser.cpp )
target_link_libraries(knobab_flloat ${Boost_LIBRARIES} ${PYTHON_LIBRARIES}  yaucl_bpm yaucl_graph)

add_executable(test main.cpp src/yaucl/bpm/structures/declare/CNFDeclareDataAware.cpp include/yaucl/bpm/structures/declare/CNFDeclareDataAware.h)
target_link_libraries(test knobab antlr4_static ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} yaucl_numeric yaucl_hashing yaucl_data yaucl_bpm yaucl_graph knobab_flloat)