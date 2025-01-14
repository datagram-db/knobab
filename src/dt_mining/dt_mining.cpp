//
// Created by giacomo on 27/12/24.
//

#include <iostream>
#include <future>
#include <functional>
#include "dt_mining/data/PolyadicJSONSerialize.h"
#include <ThreadPool.h>

#include <dt_mining/data/MultivariateTimeSeries.h>
#include <map>
#include "dt_mining/data/TemporaryCapnpnConstituentSerializer.h"

#include <dt_mining/benchmarking/DatasetStats.h>

#include <dt_mining/dt_mining.h>

inline void algo_3_serialization(capnp_constituent_serializer &ccs, size_t& incrObjId, PolyadicJSONSerialize &pjs,
                          std::unordered_set<std::string> &pe, size_t discreteEventOffset, size_t event_id,
                          std::map<size_t, size_t> &constituent_map, size_t constCounter,
                          const std::tuple<std::string, std::unordered_map<std::string, double>, size_t> &val) {
    const auto& activity_label = std::get<0>(val);
    pjs.set_up_hierarchy(activity_label);
    pe.emplace(activity_label);
    const auto& payload = std::get<1>(val);
    const auto& span = std::get<2>(val);
    for (const auto& [k,v] : payload) {
        pjs.set_up_numeric_variables(k);
    }
    constituent_map[constCounter++] = incrObjId;
    ccs.serialize(incrObjId++, activity_label, span, discreteEventOffset+event_id, payload); //.second;
}

inline void alog_2_serialization(PolyadicJSONSerialize &pjs,
                                 std::unordered_set<std::string>& unique_event_label,
                                 std::unordered_map<size_t, std::vector<std::pair<std::string, std::pair<size_t,std::unordered_map<std::string,double>>>>>& M,
        const std::tuple<size_t, std::string, std::unordered_map<std::string, double>, size_t> &val) {
    const auto& event_id = std::get<0>(val);
    const auto& activity_label = std::get<1>(val);
    pjs.set_up_hierarchy(activity_label);
    unique_event_label.emplace(activity_label);
    const auto& payload = std::get<2>(val);
    M[event_id].emplace_back(activity_label, std::make_pair(std::get<3>(val),std::get<2>(val)));
}

std::string DTMining::dt_mine_and_ts_to_polyadic(const std::string& benchmark_result_file,
                                const std::string& folder,
                                double precision,
                                double max_val,
                                bool isAlgo3,
                                          bool isDataless) {
    //    MultivariateTimeSeries mts;
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    DatasetStats ds;
    ds.folder = folder;
    ds.precision = precision;
    ds.max_val = max_val;
    ds.isAlgo3 = isAlgo3;
    ds.isDataless = isDataless;
    nlohmann::json dom;
    if (std::filesystem::exists(benchmark_result_file)) {
        std::ifstream f(benchmark_result_file);
        dom = nlohmann::json::parse(f);
    } else {
        dom = nlohmann::json::array();
    }
    ds.experiment_number = dom.size();

    // Derived parameters
    ds.loading = ds.serial = ds.total_event_time_serial = ds.total_time = ds.total_time_mining = 0;
    // polyadic_Algo1_dataless.json
    ds.nProcessors = (std::thread::hardware_concurrency()*2)/3;
    std::string file_premise = "polyadic_Algo";
    if (isAlgo3) {
        file_premise+="3_data";
    } else {
        file_premise+="2_data";
    }
    if (isDataless) {
        file_premise +="less";
    } else {
        file_premise+="ful";
    }
    std::string polyadic_file = ds.folder / (file_premise+".json");
    std::string polyadic_tmp_file = ds.folder / (file_premise+".tmp");
    std::string polyadic_map_file = ds.folder / (file_premise+".tmp_map.tab");
    std::filesystem::remove(polyadic_file);
    std::filesystem::remove(polyadic_tmp_file);
    std::filesystem::remove(polyadic_map_file);

    {
        std::cout << "# of processors: " << ds.nProcessors << std::endl;
        std::cout << "Mining Algorithm: " << (ds.isAlgo3 ? 3 : 2) << std::endl;

        // Loading the multivariate time series represented singluarly per CSV file
        auto mts = MultivariateTimeSeries::fromDirectory(isDataless, ds.folder, ds.precision, ds.max_val, ds.loading);
        std::unordered_map<std::string, size_t> env_name_to_offset;
        std::unordered_map<std::string, std::map<size_t, std::map<size_t, size_t>>> envtrace_event_constituent_to_offset;

        ThreadPool pool(ds.nProcessors);
        std::unordered_map<std::string,double> raw_payload;
        capnp_constituent_serializer ccs{polyadic_tmp_file};
        size_t incrObjId = 0;
        std::ofstream file{polyadic_file};
        size_t nDimensions = mts.begin()->second.dimensions.size();
        PolyadicJSONSerialize pjs{file};

        std::cout << "Dataset Loading Time: " << ds.loading << " (ms)" << std::endl;
        std::cout << "# of traces/environments: " << mts.size() << std::endl;
        std::cout << "# of dimensions: " << nDimensions << std::endl;

        std::unordered_set<std::string> pe;
        std::unordered_set<std::string> GUEL;
        conditional_structure cs;

        for (const auto& [trace_name_or_env, trace] : mts) {
            std::cout << "Trace #" << trace_name_or_env << std::endl;
            size_t off = env_name_to_offset.size();
            env_name_to_offset.emplace(trace_name_or_env, off);
            auto& event_constituent_to_offset = envtrace_event_constituent_to_offset[trace_name_or_env];
            size_t discreteEventOffset = 0;
            auto& per_trace_stats = ds.all_stats.emplace_back();
            per_trace_stats.experiment_number = ds.experiment_number;
            per_trace_stats.env_name = trace_name_or_env;
            per_trace_stats.nOfSegments = trace.classSegments.size();
            if (!trace.classSegments.empty()) {
                per_trace_stats.dimensions = trace.classSegments[0].dimensions.size();
            } else {
                per_trace_stats.dimensions = 0;
            }
            per_trace_stats.avgSegmentSize = per_trace_stats.event_wise_mine_time = per_trace_stats.event_wise_serial_time =  0;
            GUEL.clear();


            for (const auto& clazz_segment : trace.classSegments) {
                std::unordered_set<std::string> unique_event_label;
                SegmentStats& per_segment_stats = per_trace_stats.stats_vector.emplace_back();
                per_segment_stats.experiment_number = ds.experiment_number;
                per_segment_stats.env_name = trace_name_or_env;
                per_segment_stats.clazzSegmentSize = clazz_segment.size();
                per_trace_stats.avgSegmentSize += per_segment_stats.clazzSegmentSize;
                per_segment_stats.event_wise_serial_time = per_segment_stats.event_wise_mine_time = per_segment_stats.nConstituents = 0.0;
                const int& clazz = clazz_segment.withClass;
                std::string clazs = std::to_string(clazz);

                for (const auto& j5 : clazz_segment.dimensions) {
                    if (ds.isAlgo3) {
                        for (size_t event_id = 0; event_id < per_segment_stats.clazzSegmentSize; event_id++) {
                            pe.clear();
                            auto& per_event_stats = per_segment_stats.fineStats.emplace_back();
                            per_event_stats.event_id = event_id;
                            per_event_stats.env_name = trace_name_or_env;
                            per_event_stats.experiment_number = ds.experiment_number;

                            // START: mining+serial
                            auto t1_cpserial = high_resolution_clock::now();
                            auto& constituent_map = event_constituent_to_offset[event_id];
                            clazz_segment.retrieve_raw_data(event_id, raw_payload);
                            size_t constCounter = 0;
                            constituent_map[constCounter++] = incrObjId;
                            ccs.serialize(incrObjId++, raw_payload, discreteEventOffset+event_id, clazs); //.second;

                            // START: mining
                            auto t1_mining = high_resolution_clock::now();
                            std::vector<std::future<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>> futures;
                            std::vector<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>> data;
                            if (isDataless)
                                cs.set(&futures);
                            else
                                cs.set(&data);

                            std::get<0>(j5).Algorithm3(pool, cs, event_id);
                            std::get<1>(j5).Algorithm3(pool, cs, event_id);
                            std::get<2>(j5).Algorithm3(pool, cs, event_id);
                            std::get<3>(j5).Algorithm3(pool, cs, event_id);
                            auto t2_mining = high_resolution_clock::now();
                            /* Getting number of milliseconds as a double. */
                            duration<double, std::milli> ms_double = t2_mining - t1_mining;
                            auto time_only_mining = ms_double.count();
                            // END: mining

                            if (isDataless) {
                                for (auto& val :data) {
                                    algo_3_serialization(ccs, incrObjId, pjs, pe, discreteEventOffset, event_id,
                                                         constituent_map, constCounter, val);
                                }
                            } else {
                                for (auto& ref : futures) {
                                    auto val = ref.get();
                                    algo_3_serialization(ccs, incrObjId, pjs, pe, discreteEventOffset, event_id,
                                                         constituent_map, constCounter, val);
                                }
                            }


                            auto t2_cpserial = high_resolution_clock::now();
                            /* Getting number of milliseconds as a double. */
                            duration<double, std::milli> ms_double_cpserial = t2_cpserial - t1_cpserial;
                            // END: mining+serial

                            auto count = ms_double_cpserial.count();
                            ds.total_time_mining += time_only_mining;
                            per_event_stats.event_wise_serial_time = (count - time_only_mining);
                            ds.total_event_time_serial += per_event_stats.event_wise_serial_time;
                            ds.total_event_time_serial += per_event_stats.event_wise_serial_time;
                            per_segment_stats.event_wise_serial_time += per_event_stats.event_wise_serial_time;
                            per_trace_stats.event_wise_serial_time += per_event_stats.event_wise_serial_time;

                            per_event_stats.event_wise_mine_time = time_only_mining;
                            per_segment_stats.event_wise_mine_time += per_event_stats.event_wise_mine_time;
                            per_trace_stats.event_wise_mine_time += per_event_stats.event_wise_mine_time;
                            per_event_stats.nConstituents = futures.size();
                            per_segment_stats.nConstituents += per_event_stats.nConstituents;
                            per_trace_stats.totalConstituents += futures.size();
                            unique_event_label.insert(pe.begin(), pe.end());
                        }
                    } else {
                        // MINING TIME
                        auto t1_mining = high_resolution_clock::now();
                        std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>> futures;
                        std::vector<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>> data;
                        if (isDataless)
                            cs.set(&futures);
                        else
                            cs.set(&data);

                        std::get<0>(j5).Algorithm2(pool, cs);
                        std::get<1>(j5).Algorithm2(pool, cs);
                        std::get<2>(j5).Algorithm2(pool, cs);
                        std::get<3>(j5).Algorithm2(pool, cs);

                        auto t2_mining = high_resolution_clock::now();
                        /* Getting number of milliseconds as a double. */
                        duration<double, std::milli> ms_double = t2_mining - t1_mining;
                        auto count1 = ms_double.count();
                        per_segment_stats.event_wise_mine_time += count1;
                        per_trace_stats.event_wise_mine_time += count1;
                        per_segment_stats.nConstituents += futures.size();
                        per_trace_stats.totalConstituents += futures.size();
                        ds.total_time_mining += count1;

                        // SERIALIZATION TIME
                        auto t1_cpserial = high_resolution_clock::now();
                        std::unordered_map<size_t, std::vector<std::pair<std::string, std::pair<size_t,std::unordered_map<std::string,double>>>>> M;
                        if (isDataless) {
                            for (auto& val : data) {
                                alog_2_serialization(pjs,unique_event_label,M, val);
                            }
                        } else {
                            for (auto& ref : futures) {
                                auto val = ref.get();
                                alog_2_serialization(pjs,unique_event_label,M, val);
                            }
                        }

                        for (size_t event_id = 0; event_id < per_segment_stats.clazzSegmentSize; event_id++) {
//                        std::cout << "Event #" << event_id << std::endl;
                            const auto& elements = M[event_id];
                            auto& constituent_map = event_constituent_to_offset[event_id];
                            size_t constCounter = 0;
                            clazz_segment.retrieve_raw_data(event_id, raw_payload);
                            constituent_map[constCounter++] = incrObjId;
                            ccs.serialize(incrObjId++, raw_payload, discreteEventOffset+event_id, clazs); //.second;
                            for (auto& [activity_label, span_and_durative_payload] : elements) {
                                for (const auto& [k,v] : span_and_durative_payload.second) {
                                    pjs.set_up_numeric_variables(k);
                                }
                                constituent_map[constCounter++] = incrObjId;
                                ccs.serialize(incrObjId++, activity_label, span_and_durative_payload.first, discreteEventOffset+event_id, span_and_durative_payload.second); //.second;
                            }
                        }
                        auto t2_cpserial = high_resolution_clock::now();
                        /* Getting number of milliseconds as a double. */
                        duration<double, std::milli> ms_double_cpserial = t2_cpserial - t1_cpserial;
                        auto count2 = ms_double_cpserial.count();

                        ds.total_event_time_serial += count2;
                        per_segment_stats.event_wise_serial_time += (count2);
                        per_trace_stats.event_wise_serial_time += (count2);
                    }
                }
                per_segment_stats.unique_event_label = unique_event_label.size();
                discreteEventOffset += per_segment_stats.clazzSegmentSize;
                GUEL.insert(unique_event_label.begin(), unique_event_label.end());
            }
            per_trace_stats.avgSegmentSize = per_trace_stats.avgSegmentSize / trace.classSegments.size();
            per_trace_stats.unique_event_label = GUEL.size();
        }

        std::cout << "Starting to json-serialize..." << std::endl;
        auto t1_jserial = high_resolution_clock::now();
        pjs.start();
        pjs.heading();
        pjs.start_serializing_log_within_object();
        for (const auto& [env, rest] : envtrace_event_constituent_to_offset) {
            pjs.start_serializing_trace_within_log(env);
            for (const auto& [event, constituents] : rest) {
                pjs.start_event();
                for (const auto& [constituent_id, capnpn_offset] : constituents) {
                    pjs.json_serialize_constituent(ccs.retrieve(capnpn_offset));
                }
                pjs.finish_event();
            }
            pjs.finish_serializing_trace_within_log();
        }
        pjs.end_serializing_log_within_object();
        pjs.finish();
        auto t2_jserial = high_resolution_clock::now();
        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double_cpserial = t2_jserial - t1_jserial;
        ds.serial = ms_double_cpserial.count();
        ds.total_time = ds.serial + ds.total_time_mining + ds.total_event_time_serial + ds.loading;

        std::cout << "Dataset JSON Streaming Final Serialization Time: " << ds.serial << " (ms)" << std::endl;
        std::cout << "Dataset JSON Streaming Only Mining Time: " << ds.total_time_mining << " (ms)" << std::endl;
        std::cout << "Dataset JSON Streaming Only Event Serialization Time: " << ds.total_event_time_serial << " (ms)" << std::endl;

        dom.emplace_back(ds.asJson());
        {
            std::ofstream ofs{benchmark_result_file};
            ofs << dom;
        }

        // closing the ccs capnp_constituent_serializer, so that we can eventually remove the mapped file on disk after serialization
    }
    std::filesystem::remove(polyadic_tmp_file);
    std::filesystem::remove(polyadic_map_file);

    return polyadic_file;
}


