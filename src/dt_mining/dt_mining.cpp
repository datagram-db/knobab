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

inline void algo_3_serialization(std::vector<std::map<size_t, std::vector<CacheConstituent>>>& vv,
                                 PolyadicJSONSerialize &pjs,
                                 std::unordered_set<std::string> &pe,
                                 size_t discreteEventOffset,
                                 size_t event_id,
                                 const std::tuple<std::string, std::unordered_map<std::string, double>, size_t> &val) {
    const auto& activity_label = std::get<0>(val);
    pjs.set_up_hierarchy(activity_label);
    pe.emplace(activity_label);
    const auto& payload = std::get<1>(val);
    const auto& span = std::get<2>(val);
    for (const auto& [k,v] : payload) {
        pjs.set_up_numeric_variables(k);
    }
    vv[discreteEventOffset+event_id][span].emplace_back(capnp_constituent_serializer::build(vv.size(), activity_label, span, discreteEventOffset+event_id, payload)); //.second;
}


inline void alog_2_serialization(size_t discreteEventOffset,
                                 std::vector<std::map<size_t, std::vector<CacheConstituent>>>& vv,
                                 PolyadicJSONSerialize &pjs,
                                 std::unordered_set<std::string>& unique_event_label,
//                                 std::unordered_map<size_t, std::vector<std::pair<std::string, std::pair<size_t,std::unordered_map<std::string,double>>>>>& M,
        const std::tuple<size_t, std::string, std::unordered_map<std::string, double>, size_t> &val) {
    size_t event_id = std::get<0>(val);
    const auto& activity_label = std::get<1>(val);
    pjs.set_up_hierarchy(activity_label);
    unique_event_label.emplace(activity_label);
    const auto& payload = std::get<2>(val);
    size_t span = std::get<3>(val);
    while (vv.size()<=discreteEventOffset+event_id)
        vv.emplace_back();
    auto& v = vv[discreteEventOffset+event_id];
    v[span].emplace_back(capnp_constituent_serializer::build(vv.size(), activity_label, span, discreteEventOffset+event_id, payload)); //.second;
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
    std::unordered_set<std::string> already_met_activities;
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
    ds.nProcessors = (std::thread::hardware_concurrency()*4)/5;
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
//    std::vector<CacheConstituent> tmp_constituents;

    {
        std::cout << "# of processors: " << ds.nProcessors << std::endl;
        std::cout << "Mining Algorithm: " << (ds.isAlgo3 ? 3 : 2) << std::endl;

        // Loading the multivariate time series represented singluarly per CSV file
        auto mts = MultivariateTimeSeries::fromDirectory(isDataless, ds.folder, ds.precision, ds.max_val, ds.loading);
        std::unordered_map<std::string, size_t> env_name_to_offset;
        std::unordered_map<std::string, std::map<size_t, std::map<size_t, size_t>>> envtrace_event_constituent_to_offset;

        ThreadPool pool(ds.nProcessors);
        std::unordered_map<std::string,double> raw_payload;
//        capnp_constituent_serializer ccs{polyadic_tmp_file};
//        size_t incrObjId = 0;
        std::ofstream file{polyadic_file}, payload{polyadic_tmp_file};
        size_t nDimensions = mts.begin()->second.dimensions.size();
        PolyadicJSONSerialize pjs{file}, payloads{payload};
        payloads.start();
        payloads.start_serializing_log_within_object();

        std::cout << "Dataset Loading Time: " << ds.loading << " (ms)" << std::endl;
        std::cout << "# of traces/environments: " << mts.size() << std::endl;
        std::cout << "# of dimensions: " << nDimensions << std::endl;

        std::unordered_set<std::string> pe;
        std::unordered_set<std::string> GUEL;
        conditional_structure cs;
        std::vector<std::future<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>> futures;
        std::vector<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>> data;
        std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>> futures_2;
        std::vector<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>> data_2;
        std::unordered_map<std::string, size_t> trace_size;
        trace_size.reserve(mts.size());

        for (/*const*/ auto& [trace_name_or_env, trace] : mts) {
            std::cout << "Trace #" << trace_name_or_env << std::endl;
            size_t off = env_name_to_offset.size();
//            tmp_constituents.clear();
            env_name_to_offset.emplace(trace_name_or_env, off);
            auto& rest = envtrace_event_constituent_to_offset[trace_name_or_env];
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
//            auto& seg_ref = trace.classSegments[0];
            size_t total_events = 0;
            for (const auto& obj : trace.classSegments)
                total_events += obj.size();
            std::vector<std::map<size_t, std::vector<CacheConstituent>>> polyadic_events(total_events);

            for (/*const*/ auto& clazz_segment : trace.classSegments) {
                std::unordered_set<std::string> unique_event_label;
                SegmentStats& per_segment_stats = per_trace_stats.stats_vector.emplace_back();
                per_segment_stats.experiment_number = ds.experiment_number;
                per_segment_stats.env_name = trace_name_or_env;
                per_segment_stats.clazzSegmentSize = clazz_segment.size();
                per_trace_stats.avgSegmentSize += per_segment_stats.clazzSegmentSize;
                per_segment_stats.event_wise_serial_time = per_segment_stats.event_wise_mine_time = per_segment_stats.nConstituents = 0.0;
                const int& clazz = clazz_segment.withClass;
                std::string clazs = std::to_string(clazz_segment.withClass);

                // writing the initial event payloads as the first constituents for the event
                for (size_t event_id = 0; event_id<per_segment_stats.clazzSegmentSize; event_id++) {
                    clazz_segment.retrieve_raw_data(event_id, raw_payload);
                    polyadic_events[discreteEventOffset+event_id][1].emplace_back(capnp_constituent_serializer::build(0, raw_payload, discreteEventOffset+event_id, clazs));
                }

                for (/*const*/ auto& j5 : clazz_segment.dimensions) {
                    if (ds.isAlgo3) {
                        for (size_t event_id = 0; event_id < per_segment_stats.clazzSegmentSize; event_id++) {
                            pe.clear();
                            auto& per_event_stats = per_segment_stats.fineStats.emplace_back();
                            per_event_stats.event_id = event_id;
                            per_event_stats.env_name = trace_name_or_env;
                            per_event_stats.experiment_number = ds.experiment_number;

                            // START: mining+serial
//                            auto t1_cpserial = high_resolution_clock::now();
                            auto& constituent_map = rest[event_id];
                            size_t constCounter = 0;
//                            const size_t element = tmp_constituents.size();
//                            constituent_map[constCounter++] = tmp_constituents.size();

//                            ccs.serialize(incrObjId++, raw_payload, discreteEventOffset+event_id, clazs); //.second;

                            // START: mining
                            auto t1_mining = high_resolution_clock::now();
                            data.clear();
                            if (!isDataless) {
                                cs.set(&futures);
                                futures.clear();
                            } else {
                                cs.set(&data);
                            }

                            std::get<0>(j5).Algorithm3(pool, cs, event_id);
                            std::get<1>(j5).Algorithm3(pool, cs, event_id);
                            std::get<2>(j5).Algorithm3(pool, cs, event_id);
                            std::get<3>(j5).Algorithm3(pool, cs, event_id);

                            std::unordered_map<std::string, std::pair<size_t,size_t>> act_name_to_beging_to_cpLongestAndOffsetid;
                            if (!isDataless) {
                                for (auto& ref : futures) {
                                    auto val = ref.get();
                                    data.emplace_back(val);
                                }
                            }
                            futures.clear();
                            for (size_t i = 0, N = data.size(); i<N; i++) {
                                const auto& val = data.at(i);
                                auto& II = act_name_to_beging_to_cpLongestAndOffsetid[std::get<0>(val)];
                                if ((II.first == 0) || (II.first < std::get<2>(val))) {
                                    II = {std::get<2>(val), i};
                                }
                            }

                            for (const auto& [_, result] : act_name_to_beging_to_cpLongestAndOffsetid) {
                                algo_3_serialization(polyadic_events, pjs, pe, discreteEventOffset, event_id,
                                                     data[result.second]);
                            }
                            auto t2_mining = high_resolution_clock::now();
                            /* Getting number of milliseconds as a double. */
                            duration<double, std::milli> ms_double = t2_mining - t1_mining;
                            auto time_only_mining = ms_double.count();
                            // END: mining


                            auto t2_cpserial = high_resolution_clock::now();
                            /* Getting number of milliseconds as a double. */
//                            duration<double, std::milli> ms_double_cpserial = t2_cpserial - t1_cpserial;
                            // END: mining+serial

//                            auto count = ms_double_cpserial.count();
                            ds.total_time_mining += time_only_mining;
                            per_event_stats.event_wise_serial_time = 0;
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
//                        std::cout << "MINER3" << std::endl;
                    } else {
                        // MINING TIME
                        auto t1_mining = high_resolution_clock::now();
                        data_2.clear();
                        if (!isDataless) {
                            futures_2.clear();
                            cs.set(&futures_2);
                        } else {
                            cs.set(&data_2);
                        }

//#ifdef DEBUG
////                        if (trace_name_or_env == "576")
////                            std::cerr << "BREAK_HERE" << std::endl;
//                        std::get<0>(j5).algorithm_element = std::get<1>(j5).algorithm_element = std::get<2>(j5).algorithm_element = std::get<3>(j5).algorithm_element = trace_name_or_env;
//#endif

                        std::get<0>(j5).Algorithm2(pool, cs);
                        std::get<1>(j5).Algorithm2(pool, cs);
                        std::get<2>(j5).Algorithm2(pool, cs);
                        std::get<3>(j5).Algorithm2(pool, cs);
//                        std::cout << "MINER2" << std::endl;
                        std::unordered_map<std::string, std::unordered_map<size_t, std::pair<size_t,size_t>>> act_name_to_beging_to_cpLongestAndOffsetid;
                        if (!isDataless) {
                            for (auto& ref : futures_2) {
                                auto val = ref.get();
                                data_2.emplace_back(val);
                            }
                        }
                        futures_2.clear();
                        // Optimization 1, before serializing data:
                        for (size_t i = 0, N = data_2.size(); i < N; i++) {
                            const auto& val = data_2.at(i);
                            size_t span = std::get<3>(val);
                            DEBUG_ASSERT((trace_name_or_env != "576") || (span != 18));
                            alog_2_serialization(discreteEventOffset,polyadic_events, pjs,unique_event_label, val);
                        }

                        auto t2_mining = high_resolution_clock::now();
                        /* Getting number of milliseconds as a double. */
                        duration<double, std::milli> ms_double = t2_mining - t1_mining;
                        auto count1 = ms_double.count();
                        per_segment_stats.event_wise_mine_time += count1;
                        per_trace_stats.event_wise_mine_time += count1;
                        per_segment_stats.nConstituents += futures_2.size();
                        per_trace_stats.totalConstituents += futures_2.size();
                        ds.total_time_mining += count1;
                    }
                }
                per_segment_stats.unique_event_label = unique_event_label.size();
                discreteEventOffset += per_segment_stats.clazzSegmentSize;
                GUEL.insert(unique_event_label.begin(), unique_event_label.end());
            }
            const size_t total_trace_size = per_trace_stats.avgSegmentSize;
            per_trace_stats.avgSegmentSize = per_trace_stats.avgSegmentSize / trace.classSegments.size();
            per_trace_stats.unique_event_label = GUEL.size();

            auto t1_cpserial = high_resolution_clock::now();
            payloads.start_serializing_trace_within_log(trace_name_or_env);
            for (size_t event_id = 0; event_id < polyadic_events.size(); event_id++) {
//                        std::cout << "Event #" << event_id << std::endl;
                payloads.start_event();
                std::unordered_map<std::string,size_t> act_level_max_span;
                std::unordered_map<std::string,std::vector<CacheConstituent>> CC;
                std::unordered_map<std::string,std::unordered_map<std::string, std::vector<CacheConstituent>>> CCC;
                for (auto& [span, with_same_span] : polyadic_events[event_id]) {
                    already_met_activities.clear();
                    for (const auto& full_poly_event : with_same_span) {
                        if (already_met_activities.emplace(full_poly_event.label).second) {
                            for (const auto& [k,v] : full_poly_event.payload) {
                                payloads.set_up_numeric_variables(k);
                            }
                            auto att1 = act_level_max_span.emplace(full_poly_event.label, full_poly_event.span);
                            if ((att1.second || ((!att1.second) && (att1.first->second <= full_poly_event.span)))) {
                                if (att1.first->second < full_poly_event.span) {
                                    att1.first->second = full_poly_event.span;
                                    CC[full_poly_event.label].clear();
                                }
                                CC[full_poly_event.label].emplace_back(full_poly_event);
                            }
                        }
                    }

//                                constituent_map[constCounter++] = incrObjId;
//                    payloads.json_serialize_constituent(capnp_constituent_serializer::build(constCounter++, activity_label, span_and_durative_payload.first, discreteEventOffset+event_id, span_and_durative_payload.second)); //.second;
                }
                for (auto& [label, vals] : CC) {
                    for (auto& full_poly_event : vals) {
                        auto begin_offset = label.find_first_of("(");
                        auto end_offset = label.find_last_of(")");
                        auto variable = label.substr(begin_offset+1, end_offset-begin_offset-1);
                        auto cond = label.substr(0, begin_offset);
                        CCC[variable][cond].emplace_back(std::move(full_poly_event));
                    }
                    vals.clear();
                }
                CC.clear();
                auto& map2 = CCC["__raw_data"]["__raw_data"];
                DEBUG_ASSERT(map2.size() == 1);
                payloads.json_serialize_constituent(map2[0]);

                for (auto& [var, map] : CCC) {
                    if (var == "__raw_data") continue;
                    std::unordered_map<size_t, std::vector<CacheConstituent>> C3;
                    for (auto& [cond, objs] : map ) {
                        for (auto& full_poly_event: objs) {
                            C3[full_poly_event.span].emplace_back(std::move(full_poly_event));
                        }
                    }
                    map.clear();
//                    // Removing events composed of spans of 1 if other spans are present
//                    if (C3.contains(1) && C3.size()>1) {
//                        C3.erase(1);
//                    }
                    for (const auto& [s, vv] : C3) {
                        for (auto& full_poly_event: vv) {

#ifdef DEBUG
                            const auto cond = full_poly_event.span+full_poly_event.disctime <= total_trace_size;
#endif
                            DEBUG_ASSERT(full_poly_event.span+full_poly_event.disctime <= total_trace_size);
                            payloads.json_serialize_constituent(full_poly_event);
                        }
                    }
                }
                CCC.clear();
                payloads.finish_event();
            }
            payloads.finish_serializing_trace_within_log();
            auto t2_cpserial = high_resolution_clock::now();
            duration<double, std::milli> ms_double_cpserial = t2_cpserial - t1_cpserial;
            auto count2 = ms_double_cpserial.count();
            ds.total_event_time_serial += count2;
            per_trace_stats.event_wise_serial_time = count2;

#ifdef DEBUG
            payload.flush();
#endif
        }
        payloads.end_serializing_log_within_object();
        payloads.finish();
        payload.flush();
        payload.close();
//        payload.close();




        std::cout << "Starting to json-serialize..." << std::endl;
        auto t1_jserial = high_resolution_clock::now();
        pjs.transfer_information(std::move(payloads));
        pjs.start();
        pjs.heading();
        std::fstream payload_read{polyadic_tmp_file};
        char openPar;
        payload_read >> openPar;
        DEBUG_ASSERT(openPar == '{');
        file << ",";
        size_t bufferSize = sysconf(_SC_PAGE_SIZE);
        char buffer[bufferSize];
        while (payload_read)
        {
            payload_read.read(buffer, bufferSize);
            auto bytesRead = payload_read.gcount();
            if (bytesRead == 0) {
                // EOF
                break;
            } else if (bytesRead < bufferSize) {
                file << std::string_view(buffer, bytesRead-1);
                break;
            }
            // print the buffer data
            file << std::string_view(buffer, bytesRead);
        }

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

        std::filesystem::remove(polyadic_tmp_file);

        // closing the ccs capnp_constituent_serializer, so that we can eventually remove the mapped file on disk after serialization
    }
    std::filesystem::remove(polyadic_tmp_file);
    std::filesystem::remove(polyadic_map_file);

    return polyadic_file;
}


