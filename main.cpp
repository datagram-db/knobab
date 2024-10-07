#include <iostream>
#include <future>
#include <functional>
#include "data/PolyadicJSONSerialize.h"


#include <data/MultivariateTimeSeries.h>
#include <map>
#include "data/TemporaryCapnpnConstituentSerializer.h"


int main() {
//    MultivariateTimeSeries mts;

    double loading_ts = 0;
    std::filesystem::path folder = "/home/giacomo/projects/knobab2_loggen/EMeriTAte/test";
    std::string polyadic_file = folder / "polyadic.json";
    std::string polyadic_tmp_file = folder / "polyadic.tmp";

    auto mts = MultivariateTimeSeries::fromDirectory(folder, 0.0001, 1000000000.0, loading_ts);
    std::unordered_map<std::string, size_t> env_name_to_offset;
    std::unordered_map<std::string, std::map<size_t, std::map<size_t, size_t>>> envtrace_event_constituent_to_offset;
    bool isAlgo3 = true;
    size_t nProcessors = (std::thread::hardware_concurrency()*2)/3;
    ThreadPool pool(nProcessors);
    std::unordered_map<std::string,double> raw_payload;
    capnp_constituent_serializer ccs{polyadic_tmp_file};
    size_t incrObjId = 0;
    std::ofstream file{polyadic_file};
    PolyadicJSONSerialize pjs{file};

    for (const auto& [trace_name_or_env, trace] : mts) {
        std::cout << "Trace #" << trace_name_or_env << std::endl;
        size_t off = env_name_to_offset.size();
        env_name_to_offset.emplace(trace_name_or_env, off);
        auto& event_constituent_to_offset = envtrace_event_constituent_to_offset[trace_name_or_env];
        size_t discrete_timestamp = 0;
        size_t discreteEventOffset = 0;
        for (const auto& clazz_segment : trace.classSegments ) {
            auto clazzSegmentSize = clazz_segment.size();
            const int& clazz = clazz_segment.withClass;
            std::string clazs = std::to_string(clazz);
            for (const auto& j5 : clazz_segment.dimensions) {
                if (isAlgo3) {
                    for (size_t event_id = 0; event_id < clazzSegmentSize; event_id++) {
                        auto& constituent_map = event_constituent_to_offset[event_id];
                        clazz_segment.retrieve_raw_data(event_id, raw_payload);
                        size_t constCounter = 0;
                        constituent_map[constCounter++] = incrObjId;
                        ccs.serialize(incrObjId++, raw_payload, discreteEventOffset+event_id, clazs).second;
//                        std::cout << event_id << std::endl;
                        std::vector<std::future<std::tuple<std::string, std::unordered_map<std::string,double>,size_t>>> futures;
                        std::get<0>(j5).Algorithm3(pool, futures, event_id);
                        std::get<1>(j5).Algorithm3(pool, futures, event_id);
                        std::get<2>(j5).Algorithm3(pool, futures, event_id);
                        std::get<3>(j5).Algorithm3(pool, futures, event_id);
                        for (auto& ref : futures) {
                            auto val = ref.get();
                            const auto& activity_label = std::get<0>(val);
                            pjs.set_up_hierarchy(activity_label);
                            const auto& payload = std::get<1>(val);
                            const auto& span = std::get<2>(val);
                            for (const auto& [k,v] : payload) {
                                pjs.set_up_numeric_variables(k);
                            }
                            constituent_map[constCounter++] = incrObjId;
                            ccs.serialize(incrObjId++, activity_label, span, discreteEventOffset+event_id, payload).second;
                        }
                    }
                } else {
                    std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>,size_t>>> futures;
                    std::get<0>(j5).Algorithm2(pool, futures);
                    std::get<1>(j5).Algorithm2(pool, futures);
                    std::get<2>(j5).Algorithm2(pool, futures);
                    std::get<3>(j5).Algorithm2(pool, futures);
                    std::unordered_map<size_t, std::vector<std::pair<std::string, std::pair<size_t,std::unordered_map<std::string,double>>>>> M;
                    for (auto& ref : futures) {
                        auto val = ref.get();
                        const auto& event_id = std::get<0>(val);
                        const auto& activity_label = std::get<1>(val);
                        pjs.set_up_hierarchy(activity_label);
                        const auto& payload = std::get<2>(val);
                        M[event_id].emplace_back(activity_label, std::make_pair(std::get<3>(val),std::get<2>(val)));
                    }
                    for (size_t event_id = 0; event_id < clazzSegmentSize; event_id++) {
//                        std::cout << "Event #" << event_id << std::endl;
                        const auto& elements = M[event_id];
                        auto& constituent_map = event_constituent_to_offset[event_id];
                        size_t constCounter = 0;
                        clazz_segment.retrieve_raw_data(event_id, raw_payload);
                        constituent_map[constCounter++] = incrObjId;
                        ccs.serialize(incrObjId++, raw_payload, discreteEventOffset+event_id, clazs).second;
                        for (auto& [activity_label, span_and_durative_payload] : elements) {
                            for (const auto& [k,v] : span_and_durative_payload.second) {
                                pjs.set_up_numeric_variables(k);
                            }
                            constituent_map[constCounter++] = incrObjId;
                            ccs.serialize(incrObjId++, activity_label, span_and_durative_payload.first, discreteEventOffset+event_id, span_and_durative_payload.second).second;
                        }
                    }
                }
            }
            discreteEventOffset += clazzSegmentSize;
        }
    }

    std::cout << "Starting to json-serialize..." << std::endl;
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



//
//    ThreadPool pool(nProcessors);
//    for (size_t dim = 0; dim<nDimensions; dim++) {
//        auto j5 = transform_series(dimName[dim], dimensions[dim]);
//        if (isAlgo3) {
//            for (size_t idx = 0; idx<len; idx++) {
//                std::cout << idx << std::endl;
//                std::vector<std::future<std::pair<std::string, std::unordered_map<std::string,double>>>> futures;
//                std::get<0>(j5).Algorithm3(pool, futures, idx);
//                std::get<1>(j5).Algorithm3(pool, futures, idx);
//                std::get<2>(j5).Algorithm3(pool, futures, idx);
//                std::get<3>(j5).Algorithm3(pool, futures, idx);
//                for (auto& ref : futures) {
//                    auto val = ref.get();
//                }
//            }
//        } else {
//            std::vector<std::future<std::tuple<size_t, std::string, std::unordered_map<std::string,double>>>> futures;
//            std::get<0>(j5).Algorithm2(pool, futures);
//            std::get<1>(j5).Algorithm2(pool, futures);
//            std::get<2>(j5).Algorithm2(pool, futures);
//            std::get<3>(j5).Algorithm2(pool, futures);
//            std::unordered_map<size_t, std::vector<std::pair<std::string, std::unordered_map<std::string,double>>>> M;
//            for (auto& ref : futures) {
//                auto val = ref.get();
//                M[std::get<0>(val)].emplace_back(std::get<1>(val), std::get<2>(val));
//            }
//        }
//
//    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
