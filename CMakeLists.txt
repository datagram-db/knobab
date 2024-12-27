cmake_minimum_required(VERSION 3.28)
project(DTMining)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message(WARNING "Compiling in debug mode")
    add_compile_definitions(DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
else()
    message(WARNING "Compiling in optimized mode. Level=2")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

set(CMAKE_SYSTEM_NAME Linux)
set(WITH_DEMO false)
include_directories(/usr/include/c++/13/)
set(CMAKE_C_COMPILER clang-19 CACHE INTERNAL "C compiler")
set(CMAKE_CXX_COMPILER clang++-19 CACHE INTERNAL "C++ compiler")
set(CMAKE_CXX_STANDARD 23)

add_library(catch24 OBJECT
        submodules/catch24/C/SP_Summaries.cpp
        submodules/catch24/C/DN_Spread_Std.h
        submodules/catch24/C/DN_HistogramMode_10.cpp
        submodules/catch24/C/SC_FluctAnal.cpp
        submodules/catch24/C/SB_CoarseGrain.cpp
        submodules/catch24/C/splinefit.h
        submodules/catch24/C/SB_CoarseGrain.h
        submodules/catch24/C/fft.cpp
        submodules/catch24/C/SC_FluctAnal.h
        submodules/catch24/C/SB_MotifThree.h
        submodules/catch24/C/helper_functions.h
        submodules/catch24/C/fft.h
        submodules/catch24/C/SP_Summaries.h
        submodules/catch24/C/IN_AutoMutualInfoStats.h
        submodules/catch24/C/stats.cpp
        submodules/catch24/C/FC_LocalSimple.cpp
        submodules/catch24/C/CO_AutoCorr.h
        submodules/catch24/C/PD_PeriodicityWang.h
        submodules/catch24/C/CO_AutoCorr.cpp
        submodules/catch24/C/butterworth.cpp
        submodules/catch24/C/IN_AutoMutualInfoStats.cpp
        submodules/catch24/C/SB_TransitionMatrix.h
        submodules/catch24/C/PD_PeriodicityWang.cpp
        submodules/catch24/C/MD_hrv.cpp
        submodules/catch24/C/DN_OutlierInclude.h
        submodules/catch24/C/SB_BinaryStats.cpp
        submodules/catch24/C/helper_functions.cpp
        submodules/catch24/C/runAllTS.sh
        submodules/catch24/C/DN_HistogramMode_10.h
        submodules/catch24/C/DN_HistogramMode_5.cpp
        submodules/catch24/C/MD_hrv.h
        submodules/catch24/C/DN_Mean.cpp
        submodules/catch24/C/stats.h
        submodules/catch24/C/SB_TransitionMatrix.cpp
        submodules/catch24/C/FC_LocalSimple.h
        submodules/catch24/C/splinefit.cpp
        submodules/catch24/C/DN_Mean.h
        submodules/catch24/C/SB_BinaryStats.h
        submodules/catch24/C/DN_Spread_Std.cpp
        submodules/catch24/C/histcounts.cpp
        submodules/catch24/C/histcounts.h
        submodules/catch24/C/butterworth.h
        submodules/catch24/C/DN_HistogramMode_5.h
        submodules/catch24/C/DN_OutlierInclude.cpp
        submodules/catch24/C/SB_MotifThree.cpp
        include/dt_mining/benchmarking/SegmentStats.h
        include/dt_mining/benchmarking/TraceStats.h
)

add_library(json_writer OBJECT
submodules/jsonwriter/src/json_writer.cpp
submodules/jsonwriter/include/json_writer.hpp)

include_directories(submodules/jsonwriter/include)
include_directories(submodules/date/include)
include_directories(submodules/cpp-lru-cache/include)
include_directories(submodules/json/include)
add_subdirectory(submodules/capnproto)

include_directories(submodules/thread_pool)
include_directories(submodules/catch24/)
include_directories(include/)
include_directories(include/capnpn)
add_executable(DTMining main.cpp
        soruce/dt_mining/utils/fish.cpp
        soruce/dt_mining/utils/BasicRecord.cpp
        soruce/dt_mining/utils/RecordHandling.cpp
        soruce/dt_mining/data/PolyadicJSONSerialize.cpp
        soruce/dt_mining/data/dateparse.cpp
        soruce/dt_mining/data/MultivariateTimeSeries.cpp
        soruce/capnpn/schema.propo.c++
        submodules/jsonwriter/src/json_writer.cpp
        soruce/dt_mining/data/TemporaryCapnpnConstituentSerializer.cpp
        soruce/dt_mining/benchmarking/FineGrainedStats.cpp
        soruce/dt_mining/benchmarking/SegmentStats.cpp
        soruce/dt_mining/benchmarking/TraceStats.cpp
        include/dt_mining/benchmarking/DatasetStats.h
        soruce/dt_mining/benchmarking/DatasetStats.cpp
        include/dt_mining/dt_mining.h
        soruce/dt_mining/dt_mining.cpp
)
target_link_libraries(DTMining  PRIVATE catch24 CapnProto::kj CapnProto::capnp )
#target_link_libraries(DTMining /usr/lib/gcc/x86_64-linux-gnu/11/ /usr/lib/x86_64-linux-gnu/)