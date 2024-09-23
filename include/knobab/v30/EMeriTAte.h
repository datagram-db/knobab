//
// Created by giacomo on 08/08/24.
//

#ifndef KNOBAB_SERVER_EMERITATE_H
#define KNOBAB_SERVER_EMERITATE_H

#ifdef NO_PYBIND
#else
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#endif

#include <string>
#include <unordered_set>

#include <knobab/mining/polyadic/polyadic_loading.h>
#include <knobab/mining/polyadic/polyadic_mining.h>

#define COUNT_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
#define COUNT(...)   COUNT_N(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
// Warning: COUNT() return 1 (as COUNT(A)) :-/

#define IDENTITY(N) N
#define APPLY(macro, ...) IDENTITY(macro(__VA_ARGS__))

#define STRINGIFY_1(a) #a
#define STRINGIFY_2(a, b) #a "," #b
#define STRINGIFY_3(a, b, c) #a "," #b "," #c
#define STRINGIFY_4(a, b, c, d) #a "," #b "," #c "," #d
#define STRINGIFY_5(a, b, c, d, e) #a "," #b "," #c "," #d "," #e
#define STRINGIFY_6(a, b, c, d, e, f) #a "," #b "," #c "," #d "," #e "," #f
#define STRINGIFY_7(a, b, c, d, e, f, g) #a "," #b "," #c "," #d "," #e "," #f "," #g
#define STRINGIFY_8(a, b, c, d, e, f, g, h) #a "," #b "," #c "," #d "," #e "," #f "," #g "," #h
#define STRINGIFY_9(a, b, c, d, e, f, g, h, i) #a "," #b "," #c "," #d "," #e "," #f "," #g "," #h "," #i
#define STRINGIFY_10(a, b, c, d, e, f, g, h, i, j) #a "," #b "," #c "," #d "," #e "," #f "," #g "," #h "," #i "," #j
#define BESTIA_1(a) a
#define BESTIA_2(a, b) a << "," <<  b
#define BESTIA_3(a, b, c) a << "," <<  b << "," <<  c
#define BESTIA_4(a, b, c, d) a << "," <<  b << "," <<  c << "," <<  d
#define BESTIA_5(a, b, c, d, e) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e
#define BESTIA_6(a, b, c, d, e, f) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f
#define BESTIA_7(a, b, c, d, e, f, g) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f << "," <<  g
#define BESTIA_8(a, b, c, d, e, f, g, h) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f << "," <<  g << "," <<  h
#define BESTIA_9(a, b, c, d, e, f, g, h, i) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f << "," <<  g << "," <<  h << "," <<  i
#define BESTIA_10(a, b, c, d, e, f, g, h, i, j) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f << "," <<  g << "," <<  h << "," <<  i << "," <<  j
#define US_1(a) a
#define US_2(a, b) a << "_" <<  b
#define US_3(a, b, c) a << "_" <<  b << "_" <<  c
#define US_4(a, b, c, d) a << "_" <<  b << "_" <<  c << "_" <<  d
#define US_5(a, b, c, d, e) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e
#define US_6(a, b, c, d, e, f) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f
#define US_7(a, b, c, d, e, f, g) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f << "_" <<  g
#define US_8(a, b, c, d, e, f, g, h) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f << "_" <<  g << "_" <<  h
#define US_9(a, b, c, d, e, f, g, h, i) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f << "_" <<  g << "_" <<  h << "_" <<  i
#define US_10(a, b, c, d, e, f, g, h, i, j) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f << "_" <<  g << "_" <<  h << "_" <<  i << "_" <<  j

#define DISPATCH_S(N) STRINGIFY_ ## N
#define DISPATCH_B(N) BESTIA_ ## N
#define DISPATCH_U(N) US_ ## N

#define STRINGIFY(...) IDENTITY(APPLY(DISPATCH_S, COUNT(__VA_ARGS__)))(__VA_ARGS__)
#define BESTIA(...) IDENTITY(APPLY(DISPATCH_B, COUNT(__VA_ARGS__)))(__VA_ARGS__)
#define UNDERSCORED(...) IDENTITY(APPLY(DISPATCH_U, COUNT(__VA_ARGS__)))(__VA_ARGS__)

#include <filesystem>
#include <yaucl/strings/string_utils.h>

#include <args.hxx>
#include <filesystem>
#include <yaucl/strings/string_utils.h>

void  original_main_entrypoint(bool reclassify,
                               bool reduction,
                               double mining_supp,
                               bool isFilenamePolyadic,
                               const std::string &traceDistinguisher,
                               const std::string &filename_polyadic,
                               const std::unordered_set<std::string> &ignore_keys,
                               bool isFastSat,
                               const std::vector<log_data_format> &worlds_format_to_load,
                               const std::vector<std::string> &worlds_file_to_load,
                               std::filesystem::path &folder,
                               const std::string& fulltime);

void  python_main_entrypoint(bool reclassify,
                               bool reduction,
                               double mining_supp,
                               bool isFilenamePolyadic,
                               const std::string &traceDistinguisher,
                               const std::string &filename_polyadic,
                               const std::vector<std::string> &ignore_keys,
                               bool isFastSat,
                               const std::vector<std::string> &worlds_format_to_load,
                               const std::vector<std::string> &worlds_file_to_load,
                               const std::string &folder,
                               const std::string& fulltime);

#endif //KNOBAB_SERVER_EMERITATE_H
