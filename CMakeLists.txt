cmake_minimum_required(VERSION 3.20)
project(datarep)
include_directories(include)
set(CMAKE_CXX_STANDARD 14)

add_executable(datarep main.cpp src/data_views/DataRepresentation.cpp include/data_views/DataRepresentation.h src/data_views/RangeQueryIterator.cpp include/data_views/RangeQueryIterator.h src/data_views/ExistsIterator.cpp include/data_views/ExistsIterator.h include/oid.h src/oid.cpp src/data_views/OIDIterator.cpp include/data_views/OIDIterator.h)
