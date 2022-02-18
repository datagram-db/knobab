cmake_minimum_required(VERSION 3.20)
project(datarep)
include_directories(include)
set(CMAKE_CXX_STANDARD 23)

add_executable(datarep main.cpp src/data_views/PipelineResultData.cpp include/data_views/PipelineResultData.h include/oid.h src/oid.cpp src/data_views/VariantIterator.cpp include/data_views/VariantIterator.h src/data_views/PipelineResult.cpp include/data_views/PipelineResult.h src/data_operations/variant_iterator_operations.cpp include/data_operations/variant_iterator_operations.h)
