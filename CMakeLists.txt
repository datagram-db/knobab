cmake_minimum_required(VERSION 3.16)
project(knobab)
set(CMAKE_CXX_STANDARD 20)
include_directories(submodules/rapidxml-1.13)
include_directories(submodules/thread-pool)
include_directories(submodules/cpp-httplib)

add_subdirectory(submodules/args)
include_directories(submodules/args)


set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.74 COMPONENTS python)
find_package(PythonLibs)

if(NOT DEFINED PARALLEL)
    message(WARNING "Compiling in sequential mode")
else()
    message(WARNING "Compiling in parallel mode")
    add_compile_definitions(MAXSatPipeline_PARALLEL)
endif()

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message(WARNING "Compiling in debug mode")
    add_compile_definitions(DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
else()
    message(WARNING "Compiling in optimized mode. Level=2")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)


if(Boost_FOUND AND PYTHONLIBS_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${PYTHON_INCLUDE_DIRS})
    set(CppPythonBindings ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
    add_compile_definitions(PYTHON_WORKS)
else()
    message(WARNING "Compiling without the Python support for C++!")
    set(CppPythonBindings "")
endif()

        #set(UTF8_TESTS OFF)
add_subdirectory(submodules/antlr4/runtime/Cpp)
include_directories(submodules/antlr4/runtime/Cpp/runtime/src)
include_directories(antlr4/cpp/antlr4)
include_directories(submodules/date/include)
include_directories(submodules/json/include)
#include_directories(submodules/aaltaf/include)
#include_directories(submodules/aaltaf/)
#add_subdirectory(submodules/aaltaf)

add_subdirectory(submodules/yamlcpp)
include_directories(submodules/yamlcpp/include)

include_directories(submodules/magic_enum/include)


#set(CMAKE_VERBOSE_MAKEFILE  TRUE)
#set(CMAKE_CXX_COMPILER g++)
include_directories(include)
add_library(knobab STATIC
        src/knobab/dataStructures/TaggedMatchedEvents.cpp
        include/knobab/dataStructures/TaggedMatchedEvents.h
        src/knobab/trace_repairs/MarkedEvent.cpp
        src/knobab/algorithms/MAXSatPipeline.cpp
        src/knobab/dataStructures/oid.cpp
        src/knobab/CountTemplate.cpp
        src/knobab/ActTable.cpp
        src/knobab/AttributeTable.cpp
        src/knobab/KnowledgeBase.cpp
        src/SimplifiedFuzzyStringMatching.cpp
        src/knobab/predicates/testing_predicates.cpp
        src/knobab/algorithms/kb_grounding.cpp
        src/knobab/algorithms/kb_grounding.cpp
        include/knobab/algorithms/kb_grounding.h
        src/knobab/algorithms/atomization_pipeline.cpp

        include/knobab/algorithms/atomization_pipeline.h #src/knobab/Environment.cpp include/knobab/Environment.h
        include/knobab/utilities/Aggregators.h include/knobab/dataStructures/TraceData.h include/knobab/operators/LTLFOperators.h
        src/knobab/Environment.cpp include/knobab/Environment.h src/knobab/trace_repairs/DataQuery.cpp include/knobab/trace_repairs/DataQuery.h src/knobab/utilities/LTLFOperators.cpp src/knobab/utilities/LoggerInformation.cpp include/knobab/utilities/LoggerInformation.h src/knobab/operators/simple_ltlf_operators.cpp include/knobab/operators/simple_ltlf_operators.h include/knobab/operators/semantics.h include/knobab/operators/fast_ltlf_operators.h src/knobab/queries/LTLfQuery.cpp include/knobab/queries/LTLfQuery.h DeclareQueryLanguageParser.cpp DeclareQueryLanguageParser.h)
target_link_libraries(knobab yaml-cpp yaucl_structures)

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
        include/yaucl/graphs/graph_join_pm.h
        src/yaucl/graphs/graph_join_pm_conversion.cpp
        src/yaucl/graphs/graph_join_pm_algorithms.cpp src/yaucl/graphs/minimizeDFA.cpp include/yaucl/graphs/algorithms/minimizeDFA.h)

add_library(yaucl_stringutils STATIC
        src/yaucl/strings/string_utils.cpp include/yaucl/strings/string_utils.h)

add_library(yaucl_data STATIC
        src/yaucl/data/xml.cpp include/yaucl/data/json.h)

add_library(yaucl_structures STATIC src/yaucl/structures/StringPrevNext.cpp include/yaucl/structures/StringPrevNext.h src/yaucl/structures/DoublePrevNext.cpp include/yaucl/structures/DoublePrevNext.h)

add_library(yaucl_bpm STATIC
        src/yaucl/bpm/structures/ltlf/ltlf_query.cpp
        src/yaucl/bpm/structures/ltlf/ltlf.cpp
        include/yaucl/bpm/structures/ltlf/ltlf.h
        src/yaucl/bpm/structures/declare/CNFDeclareDataAware.cpp
        include/yaucl/bpm/structures/declare/CNFDeclareDataAware.h
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
        src/knobab/predicates/SimpleDataPredicate.cpp include/knobab/predicates/SimpleDataPredicate.h src/knobab/predicates/PredicateManager.cpp include/knobab/predicates/PredicateManager.h
        include/yaucl/bpm/algos/transformations/declare_to_dfa/TemplateCollectResult.h src/yaucl/bpm/structures/declare/DisjunctiveDeclareDataAware.cpp include/yaucl/bpm/structures/declare/DisjunctiveDeclareDataAware.h src/yaucl/bpm/algos/transformations/grounding.cpp src/yaucl/bpm/algos/transformations/grounding.cpp include/yaucl/bpm/algos/transformations/grounding.h src/yaucl/bpm/structures/commons/testing_predicates.cpp include/yaucl/bpm/structures/commons/testing_predicates.h src/yaucl/bpm/structures/ltlf/easy_prop.cpp include/yaucl/bpm/structures/commons/easy_prop.h src/yaucl/bpm/structures/ltlf/ltlf_query.cpp include/yaucl/bpm/structures/ltlf/ltlf_query.h)
target_link_libraries(yaucl_bpm antlr4_static yaucl_numeric yaucl_data yaucl_graph yaucl_hashing) #aaltaf)

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
        src/knobab/flloat_deps/FLLOATSimplePropParser.cpp
        src/knobab/flloat_deps/DeclareTemplateCollect.cpp
        include/knobab/flloat_deps/DeclareTemplateCollect.h)
target_link_libraries(knobab_flloat ${CppPythonBindings} yaucl_bpm yaucl_graph)



add_subdirectory(submodules/backward-cpp)
add_executable(server main.cpp ) #${BACKWARD_ENABLE})

target_link_libraries(server Threads::Threads yaml-cpp  knobab antlr4_static ${CppPythonBindings}
                                  yaucl_stringutils yaucl_numeric yaucl_hashing yaucl_data yaucl_bpm yaucl_graph knobab_flloat yaucl_structures)
add_backward(server)

add_executable(tests_generator tests_generator.cpp) #${BACKWARD_ENABLE})
target_link_libraries(tests_generator knobab
        yaucl_stringutils yaucl_numeric yaucl_hashing yaucl_data yaucl_bpm yaucl_graph knobab_flloat yaucl_structures)

enable_testing ()
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/58b71c9ea2fa52a7412e2267895d016abe14a971.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_subdirectory (tests)


