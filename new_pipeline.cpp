
#include <string>
#include <unordered_set>
#include <knobab/algorithms/polyadic/polyadic_loading.h>


int main() {
    double loading, indexing;
    ServerQueryManager sqm;
    std::tie(loading, indexing) = polyadic_loader({"day","span","__class","__label","time","fulltime"},
                                                   "user",
                                                   "/home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json",
                                                   false,
                                                   sqm);
    std::cout << "Loading and parsing: " << loading << " (ms)" << std::endl;
    std::cout << "Indexing: " << indexing << " (ms)" << std::endl;


//    ServerQueryManager sqm;
//    myParser sax;
//    std::cout << std::setprecision (15);
//    sax.ignore_keys = {"day","span","__class","__label","time","fulltime"};
//    sax.traceDistinguisher = "user";
//    std::string path{"/home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json"}; // "simple.json" /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json
//    sax.event_coordinates_init = true;
//    // 1) Storing in the main memory the "header" event of each polyadic event
//    {
//        std::ifstream f{path};
//
//        if (!nlohmann::json::sax_parse(f, &sax)) {
//            std::cerr << "ERROR while parsing the file: " << path << std::endl;
//            return 1;
//        }
//    }

//    std::set<std::string> forComparison, gotFromActual;

//    bool reclassify = true;
//    ssize_t maxClassId = -1;
//    // 2) Re-classifying the single raw events in terms of traditional machine learning, into different
//    //    explainable sub-classes using a rule-based approach
//    if (reclassify) {
//        // Using the sub-cases identified within the decision tree to target different sub-classes or cases
//        auto higgins = sax.for_preliminary_classification;
//        auto it = higgins.begin();
//        auto en = higgins.end();
//
//        DecisionTree<std::unordered_map<std::string, union_minimal>> dt(it,
//                                                                        en,
//                                                                        sax.class_to_int.size(),
//                                                                        [](const auto& map, const std::string& key) {
//                                                                            auto it = map.find(key);
//                                                                            if (it == map.end())
//                                                                                return (union_minimal)0.0;
//                                                                            else
//                                                                                return it->second;
//                                                                        },
//                                                                        sax.numerical,
//                                                                        {},
//                                                                        ForTheWin::gain_measures::Gini,
//                                                                        0.97,
//                                                                        1,
//                                                                        1,
//                                                                        1,
//                                                                        false,
//                                                                        nullptr,
//                                                                        nullptr,
//                                                                        5);
//
//        std::vector<std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> actual_result(sax.class_to_int.size());
//        {
//
//            std::unordered_map<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> result;
//            dt.populate_children_predicates2(result);
//            for (const auto& kv : result) {
//                std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>> current_conds;
//                actualClauseRefine(current_conds, kv);
//                actual_result[kv.first] = std::move(current_conds);
//            }
//        }
//        double ok = 0;
//        for (size_t idx = 0, N = sax.for_preliminary_classification.size(); idx<N; idx++) {
//            const auto& posToTraceInfo = sax.components.at(idx);
//            const auto& elements = sax.for_preliminary_classification.at(idx);
//            auto cp = test_single_conjunction(actual_result, elements.first);
//            maxClassId = std::max((ssize_t)maxClassId, (ssize_t)cp.second);
////            std::cout << posToTraceInfo.first<< "," << posToTraceInfo.second << ":" << cp.second << std::endl;
//            sax.event_coordinates[posToTraceInfo.first][posToTraceInfo.second].final_class = cp.second;
//            if (elements.second == cp.first) {
//                ok++;
//            }
//        }
//        std::cout << "Re-classification precision: " << ok/((double)sax.for_preliminary_classification.size()) << std::endl;
//    } else {
//        // Keeping the same original classes for each event
//        for (size_t idx = 0, N = sax.for_preliminary_classification.size(); idx<N; idx++) {
//            const auto& posToTraceInfo = sax.components.at(idx);
//            const auto& elements = sax.for_preliminary_classification.at(idx);
//            maxClassId = std::max((ssize_t)maxClassId, (ssize_t)elements.second);
//            sax.event_coordinates[posToTraceInfo.first][posToTraceInfo.second].final_class = elements.second;
//            forComparison.insert(sax.event_coordinates[posToTraceInfo.first][posToTraceInfo.second].get_log_name());
//        }
//        // No reclassification, so precision = 1.0
//    }

//    // 3) We can collect back the time intervals, so to eventually segment continuous events within the same class
//    sax.isClassificationDone = true;
//    sax.timeLabel = "fulltime";
//    sax.event_coordinates_init = false;
//    sax.clear();
//    {
//        std::ifstream f{path};
//        std::cout << nlohmann::json::sax_parse(f, &sax) << std::endl;
//    }
//    for (size_t idx = 0, N = sax.for_preliminary_classification.size(); idx<N; idx++) {
//        const auto& posToTraceInfo = sax.components.at(idx);
//        forComparison.insert(sax.event_coordinates[posToTraceInfo.first][posToTraceInfo.second].get_log_name());
//    }

//    // 4) Defining the time intervals within the events are happening
//    std::vector<std::vector<std::pair<double,double>>> intervals;
//    std::string filename = path;
//    for (auto& [user_key, timestamps] : sax.clazz_to_time) {
//        std::cout << user_key << std::endl;
//        std::unordered_set<size_t> metClasses;
//        for (auto& inClazz : timestamps)
//            remove_duplicates(inClazz);
//        intervals.clear();
//        intervals.resize(timestamps.size());
////        std::vector<std::vector<std::pair<double,double>>>& intervals = sax.clazz_to_time_intervals[user_key];
////        intervals.resize(sax.clazz_to_time.size());
//        std::priority_queue<std::pair<std::vector<double>::iterator,size_t>, std::vector<std::pair<std::vector<double>::iterator,size_t>>, comparator> minheap;
//        int k = timestamps.size();
//        int i;
//        for (i = 0; i < k; i++){
//            auto ptr = timestamps[i].begin(), end = timestamps[i].end();
//            if (ptr != end) {
//                minheap.emplace(ptr, i);
//            }
//        }
//
//        ssize_t current = -1;
//        double previousValue = -1;
//
//        // Now one by one get the minimum element from min
//        // heap and replace it with next element.
//        // run till all filled input files reach EOF
//        while (!minheap.empty()) {
//            // Get the minimum element and store it in output file
//            std::pair<std::vector<double>::iterator,size_t> x = minheap.top();
//            metClasses.insert(x.second);
////            std::cout << *x.first << "@" << x.second << std::endl;
//            int fileid = x.second;
//            if (current == -1) {
//                current = x.second;
//                previousValue = *x.first;
//                intervals[fileid].emplace_back(previousValue, previousValue);
//            } else if (current == fileid) {
//                intervals[fileid].back().second = previousValue = *x.first;
//            } else {
//                intervals[current].back().second = previousValue;
//                previousValue = *x.first;
//                intervals[fileid].emplace_back(previousValue, previousValue);
//                current = x.second;
//            }
//            minheap.pop();
//
//            // Find the next element that will replace current
//            // root of heap. The next element belongs to same
//            // input file as the current min element.
//            if (((x.first+1) != timestamps[fileid].end())) {
//                minheap.emplace(x.first+1, fileid);
//            }
//        }
//
////        auto& userIntervalMap = sax.userName_to_intervals_to_class[user_key];
////        for (size_t classId = 0, N = intervals.size(); classId<N; classId++) {
////            for (const auto& interval : intervals.at(classId)) {
////                if (userIntervalMap.emplace(interval, classId).second)
////                    DEBUG_ASSERT(false);
////            }
////        }
//        counting_event_coordinates emptyPair;
//        for (const auto& clazz : metClasses) {
//            auto newLogName = user_key+ "_"+ std::to_string(clazz);
//            gotFromActual.insert(newLogName);
//            sax.log_name_to_traceIdEventId.emplace(std::make_pair(newLogName,clazz), emptyPair);
//            if (sqm.multiple_logs.contains(newLogName)) {
//                sqm.multiple_logs.erase(newLogName);
//            }
//            auto& env = sqm.multiple_logs[newLogName];
//            env.db.enterLog(path, newLogName);
//            env.experiment_logger.log_filename = filename;
//        }
//    }
//    intervals.clear();
////    std::cout << gotFromActual << std::endl;
////    std::cout << forComparison << std::endl;

    // TODO: while defining the labels, you should also consider the classes, too
//    std::pair<std::string,size_t> cp;
//    for (size_t global_trace_id = 0, N = sax.event_coordinates.size(); global_trace_id<N; global_trace_id++ ) {
//        auto& vector =sax.event_coordinates.at(global_trace_id);
//        for (size_t global_event_id = 0, M = vector.size(); global_event_id<M; global_event_id++) {
//            auto& event = vector.at(global_event_id);
//            cp.first = event.user_name;
//            cp.second = event.final_class;
//            auto& traceid_eventid = sax.log_name_to_traceIdEventId[cp];
//            if ((traceid_eventid.global_event_id == (size_t)-1) && (traceid_eventid.global_trace_id == (size_t)-1)) {
//                traceid_eventid.latest_new_event_id = event.event_id = 0;
//                traceid_eventid.latest_new_trace_id = event.trace_id = 0;
//            } else {
//                if (traceid_eventid.global_trace_id != global_trace_id) {
//                    traceid_eventid.latest_new_trace_id++;
//                    event.trace_id = traceid_eventid.latest_new_trace_id;
//                    event.event_id = traceid_eventid.latest_new_event_id = 0;
//                } else if ((traceid_eventid.global_event_id+1) != global_event_id) {
//                    traceid_eventid.latest_new_trace_id++;
//                    event.trace_id = traceid_eventid.latest_new_trace_id;
//                    event.event_id = traceid_eventid.latest_new_event_id = 0;
//                } else {
//                    traceid_eventid.latest_new_event_id++;
//                    event.trace_id = traceid_eventid.latest_new_trace_id;
//                    event.event_id = traceid_eventid.latest_new_event_id;
//                }
//            }
//            traceid_eventid.global_trace_id = global_trace_id;
//            traceid_eventid.global_event_id = global_event_id;
//        }
//    }
//
//    // 5) As the traces are temporally ordered, we can freely assume that those are contiguous in time
//    //    Therefore, we only need to check
//
//        bool oldLoading = true;
//        bool load_also_data = true;
//        bool doStats = false;
//        bool index_missing_data = false;
////        std::string env_name = "test1";
//        log_data_format format;
//
//        using std::chrono::high_resolution_clock;
//        using std::chrono::duration_cast;
//        using std::chrono::duration;
//        using std::chrono::milliseconds;
//    double log_loading_and_parsing_ms = 0, log_indexing_ms = 0;
//
//    {
//        auto t1 = high_resolution_clock::now();
//        sax.filler = &sqm.multiple_logs;
//        sax.clear();
//        sax.timeLabel.clear();
//        {
//            std::ifstream f{path};
//            std::cout << nlohmann::json::sax_parse(f, &sax) << std::endl;
//        }
//        for (auto& [log, env] : sqm.multiple_logs) {
//            env.db.exitLog(path, log);
//        }
//        auto t2 = high_resolution_clock::now();
//        duration<double, std::milli> ms_double = t2 - t1;
//        log_loading_and_parsing_ms = ms_double.count();
//    }
//
//    {
//        auto t1 = high_resolution_clock::now();
//        for (auto& [log, env] : sqm.multiple_logs) {
//            env.db.index_data_structures(index_missing_data);
//        }
//        auto t2 = high_resolution_clock::now();
//        /* Getting number of milliseconds as a double. */
//        duration<double, std::milli> ms_double = t2 - t1;
//        log_indexing_ms = ms_double.count();
//    }
//
//    std::cout << "Loading and parsing: " << log_loading_and_parsing_ms << " (ms)" << std::endl;
//    std::cout << "Indexing: " << log_indexing_ms << " (ms)" << std::endl;

#if 0
        auto tmp = env.db.getMaximumStringLength();
        env.ap.s_max = std::string(tmp, std::numeric_limits<char>::max());
        DataPredicate::MAX_STRING = env.ap.s_max;
        DataPredicate::msl = tmp;

        env.db.reconstruct_trace_no_data(std::cout);
//        env.print_act_table(std::cout);

        env.experiment_logger.n_traces = env.db.noTraces;
        env.experiment_logger.n_acts = env.db.event_label_mapper.int_to_T.size();
        // Compute some more trace statistics

        double trace_avg = 0, trace_pow2 = 0, N;
        N = env.db.act_table_by_act_id.trace_length.size();
        size_t frequency_of_trace_length = 0;
        size_t previousLength = 0;
        std::multiset<size_t> O;


#endif

    return 0;
}
