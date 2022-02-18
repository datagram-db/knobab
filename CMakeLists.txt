cmake_minimum_required(VERSION 3.20)
project(datarep)
include_directories(include)
set(CMAKE_CXX_STANDARD 23)

add_executable(datarep main.cpp src/data_views/DataRepresentation.cpp include/data_views/DataRepresentation.h include/oid.h src/oid.cpp src/data_views/VariantIterator.cpp include/data_views/VariantIterator.h PipelineResult.cpp PipelineResult.h)
