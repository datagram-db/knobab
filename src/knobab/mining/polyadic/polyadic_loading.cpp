//
// Created by giacomo on 14/02/24.
//

#include "knobab/mining/polyadic/polyadic_loading.h"
#include "knobab/server/dataStructures/polyadic/myParser.h"



//□



//static inline std::pair<int,size_t> print_rawpayload_csv_row(const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
//                                                            const std::unordered_map<std::string, union_minimal>& payload) {
//    size_t sub_class = 0;
//    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
//        const auto& disj = model.at(clazz);
//        for (const auto& [score,map] : disj) {
//            bool found = false;
//            std::stringstream ss;
//            for (const auto& [k,v] : map) {
//                auto it = payload.find(k);
//                if ((it == payload.end()) ? v.testOverSingleVariable(0.0) : v.testOverSingleVariable(it->second)) {
//                    found = true;
//                } else {
//                    found = false;
//                    break;
//                }
//            }
//            if (found) {
//                return {clazz, sub_class};
//            }
//            sub_class++;
//        }
//    }
//    return {-1, 0};
//}

//static inline std::pair<int,size_t> test_single_conjunction(const std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
//                                                            const std::unordered_map<std::string, union_minimal>& payload) {
//    size_t sub_class = 0;
//    for (size_t clazz = 0, N = model.size(); clazz<N; clazz++) {
//        const auto& disj = model.at(clazz);
//        for (const auto& [score,map] : disj) {
//            bool found = false;
//            for (const auto& [k,v] : map) {
//                auto it = payload.find(k);
//                if ((it == payload.end()) ? v.testOverSingleVariable(0.0) : v.testOverSingleVariable(it->second)) {
//                    found = true;
//                } else {
//                    found = false;
//                    break;
//                }
//            }
//            if (found) {
//                return {clazz, sub_class};
//            }
//            sub_class++;
//        }
//    }
//    return {-1, 0};
//}

struct comparator {
    constexpr
    bool
    operator()(const std::pair<std::vector<double>::iterator,size_t>& __x, const std::pair<std::vector<double>::iterator,size_t>& __y) const
    { return (*__x.first > *__y.first) || ((*__x.first == *__y.first) && (__x.second > __y.second));
    }
};



std::tuple<double,double,double,double> polyadic_loader(const std::unordered_set<std::string>& ignore_keys,
                                                        const std::string& traceDistinguisher,
                                                        const std::string& path,
                                                        bool data_aware,
                                                        ServerQueryManager& sqm,
                                                        const std::string& fulltime) {
    myParser sax;
    sax.ignore_keys = ignore_keys;
    sax.traceDistinguisher = traceDistinguisher;
    sax.event_coordinates_init = true;

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto r_preprocessing1 = high_resolution_clock::now();
    ////////////////////////////////////////////////////////////////////////////
    /// 1) Storing in the main memory the "header" event of each polyadic event
    ////////////////////////////////////////////////////////////////////////////
    {
        std::ifstream f{path};

        if (!nlohmann::json::sax_parse(f, &sax)) {
            std::cerr << "ERROR while parsing the file: " << path << std::endl;
            return {-1,-1,-1, -1};
        }
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// 2) Re-classifying the single raw events in terms of traditional machine learning, into different
    ///    explainable sub-classes using a rule-based approach
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Keeping the same original classes for each event
    ssize_t maxClassId = -1;
    for (size_t idx = 0, N = sax.components.size(); idx<N; idx++) {
        const auto& posToTraceInfo = sax.components.at(idx);
        int clazz = std::get<2>(posToTraceInfo);
        maxClassId = std::max((ssize_t)maxClassId, (ssize_t)clazz);
        sax.event_coordinates[std::get<0>(posToTraceInfo)][std::get<1>(posToTraceInfo)].final_class = clazz;
    }

    ///////////////////////////////////\////////////////////////////////////////////////////////////////////////////
    /// 3) We can collect back the time intervals, so to eventually segment continuous events within the same class
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    sax.isClassificationDone = true;
    sax.timeLabel = fulltime;
    sax.event_coordinates_init = false;
    sax.clear();
    {
        std::ifstream f{path};
        nlohmann::json::sax_parse(f, &sax);
    }

    //////////////////////////////////////////////////////////////////
    /// 4) Defining the time intervals within the events are happening
    //////////////////////////////////////////////////////////////////
    std::vector<std::vector<std::pair<double,double>>> intervals;
    std::string filename = path;
    std::unordered_set<size_t> metClasses;
    counting_event_coordinates emptyPair;
    for (auto& [user_key, timestamps] : sax.clazz_to_time) {
        for (auto& inClazz : timestamps)
            remove_duplicates(inClazz);
        intervals.clear();
        intervals.resize(timestamps.size());
        std::priority_queue<std::pair<std::vector<double>::iterator,size_t>, std::vector<std::pair<std::vector<double>::iterator,size_t>>, comparator> minheap;
        int k = timestamps.size();
        int i;
        for (i = 0; i < k; i++){
            auto ptr = timestamps[i].begin(), end = timestamps[i].end();
            if (ptr != end) {
                minheap.emplace(ptr, i);
            }
        }

        ssize_t current = -1;
        double previousValue = -1;

        // Now one by one get the minimum element from min
        // heap and replace it with next element.
        // run till all filled input files reach EOF
        while (!minheap.empty()) {
            // Get the minimum element and store it in output file
            std::pair<std::vector<double>::iterator,size_t> x = minheap.top();
            metClasses.insert(x.second);
            int fileid = x.second;
            if (current == -1) {
                current = x.second;
                previousValue = *x.first;
                intervals[fileid].emplace_back(previousValue, previousValue);
            } else if (current == fileid) {
                intervals[fileid].back().second = previousValue = *x.first;
            } else {
                intervals[current].back().second = previousValue;
                previousValue = *x.first;
                intervals[fileid].emplace_back(previousValue, previousValue);
                current = x.second;
            }
            minheap.pop();

            // Find the next element that will replace current
            // root of heap. The next element belongs to same
            // input file as the current min element.
            if (((x.first+1) != timestamps[fileid].end())) {
                minheap.emplace(x.first+1, fileid);
            }
        }


    }
    for (const auto& clazz : metClasses) {
        auto newLogName = /*user_key+ "_"+*/ std::to_string(clazz);
        sax.log_name_to_traceIdEventId.emplace(std::make_pair(newLogName,clazz), emptyPair);
        if (sqm.multiple_logs.contains(newLogName)) {
            sqm.multiple_logs.erase(newLogName);
        }
        auto& env = sqm.multiple_logs[newLogName];
        env.db.schema_def = sax.schema_def;
        env.db.hierarchy_def = sax.hierarchy_def;
        env.db.enterLog(path, newLogName);
        env.experiment_logger.log_filename = filename;
    }
    intervals.clear();

    std::pair<std::string,size_t> cp;
    for (size_t global_trace_id = 0, N = sax.event_coordinates.size(); global_trace_id<N; global_trace_id++ ) {
        auto& vector =sax.event_coordinates.at(global_trace_id);
        for (size_t global_event_id = 0, M = vector.size(); global_event_id<M; global_event_id++) {
            auto& event = vector.at(global_event_id);
            cp.first = std::to_string(event.final_class);//event.user_name;
            cp.second = event.final_class;
            DEBUG_ASSERT(sax.log_name_to_traceIdEventId.contains(cp));
            auto& traceid_eventid = sax.log_name_to_traceIdEventId[cp];
            if ((traceid_eventid.global_event_id == (size_t)-1) && (traceid_eventid.global_trace_id == (size_t)-1)) {
                traceid_eventid.latest_new_event_id = event.event_id = 0;
                traceid_eventid.latest_new_trace_id = event.trace_id = 0;
            } else {
                if (traceid_eventid.global_trace_id != global_trace_id) {
                    traceid_eventid.latest_new_trace_id++;
                    event.trace_id = traceid_eventid.latest_new_trace_id;
                    event.event_id = traceid_eventid.latest_new_event_id = 0;
                } else if ((traceid_eventid.global_event_id+1) != global_event_id) {
                    traceid_eventid.latest_new_trace_id++;
                    event.trace_id = traceid_eventid.latest_new_trace_id;
                    event.event_id = traceid_eventid.latest_new_event_id = 0;
                } else {
                    traceid_eventid.latest_new_event_id++;
                    event.trace_id = traceid_eventid.latest_new_trace_id;
                    event.event_id = traceid_eventid.latest_new_event_id;
                }
            }
            traceid_eventid.global_trace_id = global_trace_id;
            traceid_eventid.global_event_id = global_event_id;
        }
    }
    auto t_preprocessing2 = high_resolution_clock::now();
    /* Getting number of milliseconds as a double. */
    duration<double, std::milli> ms_double = t_preprocessing2 - r_preprocessing1;
    double log_cpp_preprocessing = ms_double.count();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 5) As the traces are temporally ordered, we can freely assume that those are contiguous in time
    //    Therefore, we only need to check
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool index_missing_data = false;
    log_data_format format;


    double log_loading_and_parsing_ms = 0, log_indexing_ms = 0, payload_extraction = 0;
    {
        auto t1 = high_resolution_clock::now();
        sax.filler = &sqm.multiple_logs;
        sax.clear();
        sax.timeLabel.clear();
        {
            std::ifstream f{path};
            std::cout << nlohmann::json::sax_parse(f, &sax) << std::endl;
        }
        for (auto& [log, env] : sqm.multiple_logs) {
            env.db.exitLog(path, log);
        }
        size_t offset = 0;
        {
            for (auto& [k, v]: sax.tmp_event_paload_aka_rawdata) {
                size_t vsize = v.size();
                sax.event_paload_aka_rawdata.insert(sax.event_paload_aka_rawdata.end(),
                                                        std::make_move_iterator(v.begin()),
                                                        std::make_move_iterator(v.end()));
                v.clear();
                if (offset != 0) {
                    for (auto& ref : sax.payload_trace_id[k]) {
                        for (auto& idx : ref)
                            idx += offset;
                    }
                }
                offset += vsize;
            }
            sax.tmp_event_paload_aka_rawdata.clear();
        }
        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> ms_double = t2 - t1;
        log_loading_and_parsing_ms = ms_double.count();
    }

    {
        auto t1 = high_resolution_clock::now();
        for (auto& [log, env] : sqm.multiple_logs) {
            env.db.index_data_structures(index_missing_data);
        }
        auto t2 = high_resolution_clock::now();
        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;
        log_indexing_ms = ms_double.count();
    }

    if (data_aware) {
        auto t1 = high_resolution_clock::now();
//        std::vector<std::vector<std::vector<size_t>>> traces_info(sqm.multiple_logs.size()); // class -> trace_id -> offsets

        std::unordered_map<std::string,std::vector<std::vector<size_t>>> & sax_pyload_trace_id = sax.payload_trace_id;
        const std::string& this_path = path;
        std::vector<std::pair<env,int>>& payload_row = sax.event_paload_aka_rawdata;
        std::string all = "□";
        std::string some = "◇";
        bool raw = true;
        size_t n_classes = sax.class_to_int.size();
        const std::unordered_set<std::string>& numerical = sax.numerical;
        const std::unordered_set<std::string>& categorical = {};

        train_and_dump_to_csv(sqm.multiple_logs, sax_pyload_trace_id, this_path, payload_row, all, some, raw, n_classes,numerical, categorical );

        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> pe_double = t2 - t1;
        payload_extraction = pe_double.count();
    }

    return {log_cpp_preprocessing, log_loading_and_parsing_ms, log_indexing_ms, payload_extraction};
}