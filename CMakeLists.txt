cmake_minimum_required(VERSION 3.16)
project(knobab)
set(CMAKE_CXX_STANDARD 20)

add_library(rapidxml INTERFACE)
add_subdirectory(submodules/antlr4/runtime/Cpp)
include_directories(submodules/antlr4/runtime/Cpp/runtime/src)
include_directories(antlr4/cpp/antlr4)
include_directories(submodules/date/include)
include_directories(submodules/json/include)

include_directories(include)
add_library(knobab STATIC src/bzdb/oid.cpp src/bzdb/CountTemplate.cpp include/bzdb/CountTemplate.h src/bzdb/ActTable.cpp include/bzdb/ActTable.h src/bzdb/AttributeTable.cpp include/bzdb/AttributeTable.h KnowledgeBase.cpp KnowledgeBase.h src/yaucl/bpm/trace_visitor.cpp include/yaucl/bpm/trace_visitor.h data_loader.cpp src/yaucl/numeric/numeric_base.cpp include/yaucl/numeric/numeric_base.h src/yaucl/bpm/TracesBaseListener.cpp src/yaucl/bpm/TracesBaseVisitor.cpp src/yaucl/bpm/TracesLexer.cpp src/yaucl/bpm/TracesListener.cpp src/yaucl/bpm/TracesParser.cpp src/yaucl/bpm/TracesVisitor.cpp src/yaucl/bpm/DataTraceParse.cpp
        #src/fuzzyStringMatching/v1.0/STTGSHM_Primary.cpp
        #src/fuzzyStringMatching/v1.0/LSVM_Primary.cpp
        #src/fuzzyStringMatching/v1.0/SLHM_Primary.cpp
        #src/fuzzyStringMatching/v2.0/FuzzyMatchSerializer.cpp)
        src/yaucl/data/xml.cpp src/SimplifiedFuzzyStringMatching.cpp include/SimplifiedFuzzyStringMatching.h include/yaucl/structures/any_to_uint_bimap.h string_utils.cpp string_utils.h)

add_executable(test main.cpp )
target_link_libraries(test knobab antlr4_static)