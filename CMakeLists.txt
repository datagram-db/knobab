cmake_minimum_required(VERSION 3.20)
project(datarep)

set(CMAKE_CXX_STANDARD 14)

add_executable(datarep main.cpp DataRepresentation.cpp DataRepresentation.h RangeQueryIterator.cpp RangeQueryIterator.h)
