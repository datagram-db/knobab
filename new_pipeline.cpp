//#define BACKWARD_HAS_BFD 1
//#define _GLIBCXX_DEBUG
//#define _GLIBXX_DEBUG_PEDANTIC
//#include <backward.hpp>
#include <iostream>
#include <yaucl/data/json.h>
#include <variant>
#include <yaucl/structures/any_to_uint_bimap.h>
//using union_minimal = std::variant<std::string, size_t, long long, double>;

#include "knobab/server/query_manager/ServerQueryManager.h"
#include "yaucl/numeric/numeric_base.h"
#include <yaucl/learning/DecisionTree.h>

enum parsing_states {
    BEGINNING,
    SCHEMA,
    EVENT_HIERARCHY,
    LOG,
    TRACE,
    POLYADIC_EVENT,
    EVENT
};

#include <stack>

struct myParser {
    struct trace_visitor* filler{nullptr};
//    std::vector<std::pair<int,size_t>> subclasses;
    parsing_states state{BEGINNING};
    bool isSchemaOk = false;
    bool isEventHierarchyOk = false;
    bool isBeginEvent = true;
    std::stack<bool> object_stack;
    std::unordered_set<std::string> ignore_keys;
    std::unordered_map<std::string, std::string> schema_def;
    std::unordered_map<std::string, std::vector<std::string>> hierarchy_def;
    std::string keyV;
    std::string hierarchy;
    bool inHierarchyDef= false;
    std::string traceName;
    bool traceNameOk = false;
    bool tracePayloadOk = false;
    std::unordered_map<std::string, union_minimal> payload;
    std::string eventLabel;
    size_t trace_id = -1;
    size_t event_id = -1;
    yaucl::structures::any_to_uint_bimap<std::string> class_to_int;
    std::unordered_set<std::string> numerical, categorical;

    std::vector<std::vector<size_t>> eventClassification;
    bool isClassificationDone = false;
    std::vector<std::pair<size_t,size_t>> components;
    std::vector<std::pair<std::unordered_map<std::string, union_minimal>, int>> for_preliminary_classification;
//    std::vector<size_t> classId;
//    bool storeTimeIntervalsWithClazzez = false;
    std::unordered_map<std::string, std::vector<std::vector<double>>> clazz_to_time;
    std::unordered_map<std::string, std::vector<std::vector<std::pair<double,double>>>> clazz_to_time_intervals;
    std::string timeLabel, traceDistinguisher, traceDistinguisherValue;

    void clear() {
        trace_id = event_id = -1;
        isBeginEvent = true;
        schema_def.clear();
        hierarchy_def.clear();
        while (!object_stack.empty())
            object_stack.pop();
        object_stack.emplace(false);
        inHierarchyDef = traceNameOk = tracePayloadOk =  isSchemaOk = isEventHierarchyOk = false;
        keyV.clear();
        hierarchy.clear();
        traceName.clear();
        payload.clear();
        eventLabel.clear();
        state = BEGINNING;
    }

    myParser() {
        object_stack.emplace(false);
    }

    // called when null is parsed
    bool null() {
    }

// called when a boolean is parsed; value is passed
    bool boolean(bool val) {
        if (state == TRACE) {
            if (tracePayloadOk) {
                payload[keyV] = val ? 1.0 : 0.0;
                return true;
            }
        }
        return false;
    }

// called when a signed or unsigned integer number is parsed; value is passed
    bool number_integer(nlohmann::json::number_integer_t val) {
        if (state == TRACE) {
            if (tracePayloadOk) {
                if ((!traceDistinguisher.empty()) && (keyV == traceDistinguisher)) {
                    traceDistinguisherValue =std::to_string(val);
                }
                payload[keyV] = (double)val;
                return true;
            }
        }
        return false;
    }
    bool number_unsigned(nlohmann::json::number_unsigned_t val) {
        if (state == TRACE) {
            if (tracePayloadOk) {
                if ((!traceDistinguisher.empty()) && (keyV == traceDistinguisher)) {
                    traceDistinguisherValue =std::to_string(val);
                }
                payload[keyV] = (double)val;
                return true;
            }
        }
        return false;
    }

// called when a floating-point number is parsed; value and original string is passed
    bool number_float(nlohmann::json::number_float_t val, const nlohmann::json::string_t& s) {
        if (state == TRACE) {
            if (tracePayloadOk) {
                if ((!traceDistinguisher.empty()) && (keyV == traceDistinguisher)) {
                    traceDistinguisherValue =std::to_string(val);
                }
                payload[keyV] = (double)val;
                return true;
            }
        }
        return false;
    }

// called when a string is parsed; value is passed and can be safely moved away
    bool string(nlohmann::json::string_t& val) {
        if ((state == SCHEMA) && (object_stack.top())) {
            schema_def.emplace(keyV, val);
            return true;
        } else if ((state == EVENT_HIERARCHY) && (object_stack.top()) && (inHierarchyDef)) {
            hierarchy_def[hierarchy].emplace_back(val);
            return true;
        } else if ((state == TRACE)) {
            if (traceNameOk) {
                traceName = val;
                traceNameOk = false;
                return true;
            } else if (tracePayloadOk) {
                if ((!traceDistinguisher.empty()) && (keyV == traceDistinguisher)) {
                    traceDistinguisherValue = val;
                }
                auto it = schema_def.find(keyV);
                if (it != schema_def.end()) {
                    if (it->second == "continuous") {
                        payload[keyV] = (double)std::stod(val);
                    } else if (it->second == "discrete") {
                        payload[keyV] = (double)std::stoll(val);
                    } else if (it->second == "boolean") {
                        payload[keyV] = (tolower(val[0]) == 't') ? 1.0 : 0.0;
                    } else if (it->second == "timestamp") {
                        payload[keyV] = (double)yaucl::numeric::parse8601(val);
                    } else {
                        payload[keyV] = val;
                    }
                } else {
                    payload[keyV] = val;
                }
                return true;
            }
        } else if (state == EVENT) {
            if (keyV == "__label") {
                eventLabel = val;
            } else {
                auto it = schema_def.find(keyV);
                if (it != schema_def.end()) {
                    if (it->second == "continuous") {
                        payload[keyV] = (double)std::stod(val);
                    } else if (it->second == "discrete") {
                        payload[keyV] = (double)std::stoll(val);
                    } else if (it->second == "boolean") {
                        payload[keyV] = (tolower(val[0]) == 't') ? 1.0 : 0.0;
                    } else if (it->second == "timestamp") {
                        payload[keyV] = (double)yaucl::numeric::parse8601(val);
                    } else {
                        payload[keyV] = val;
                    }
                } else {
                    payload[keyV] = val;
                }
            }
            return true;
        }
        return false;
    }
// called when a binary value is parsed; value is passed and can be safely moved away
    bool binary(nlohmann::json::binary_t& val) {
    }

// called when an object or array begins or ends, resp. The number of elements is passed (or -1 if not known)
    bool start_object(std::size_t elements) {
        if ((state == LOG) && (object_stack.top()) && isSchemaOk && isEventHierarchyOk) {
            state = TRACE;
            if (filler)
                filler->exitTrace(trace_id);
            trace_id++;
            if (trace_id != 0) {
                eventClassification.emplace_back().resize(event_id);
            }
            event_id = -1;
            isBeginEvent = true;
        }
        if (state == EVENT) {
            payload.clear();
        }
        object_stack.emplace(true);
        return true;
    }
    bool end_object() {
        if (((state == SCHEMA) || (state == EVENT_HIERARCHY)) && (object_stack.top())) {
            if ((state == SCHEMA) && (!isSchemaOk))
                isSchemaOk = true;
            if ((state == EVENT_HIERARCHY) && isSchemaOk)
                isEventHierarchyOk = true;
            state = BEGINNING;
        } else if (state == TRACE) {
            if (tracePayloadOk) {
                // TODO: store the event as a payload for the trace
                std::cout << "storing trace: " << trace_id << std::endl;
                tracePayloadOk = false;
            }
        }
        else if (state == EVENT) {
            // TODO: store the single event at the current position
            // TODO: trace id and event id counter
            if (payload.contains("__class")) {
                size_t classInt = class_to_int.put(std::get<std::string>(payload["__class"])).first;
                if (!timeLabel.empty()) {
                    size_t finalClass;
                    if (eventClassification.empty()) {
                        finalClass = classInt;
                    } else {
                        finalClass = eventClassification[trace_id][event_id];
                    }
                    while (clazz_to_time[traceDistinguisherValue].size() <= finalClass)
                        clazz_to_time[traceDistinguisherValue].emplace_back();
                    clazz_to_time[traceDistinguisherValue][finalClass].emplace_back(std::get<double>(payload.at(timeLabel)));
                }
                payload.erase("__class");
                for (const auto& [k,v] : payload) {
                    if (std::holds_alternative<std::string>(v))
                        categorical.insert(k);
                    else
                        numerical.insert(k);
                }
                for (const auto& k : ignore_keys) {
                    payload.erase(k);
                }
                if (!isClassificationDone) {
                    components.emplace_back(trace_id, event_id);
                    for_preliminary_classification.emplace_back(payload, (int)classInt);
                }
            } else if (filler) {
                if ((event_id == 0) && isBeginEvent) {
                    filler->enterTrace(std::to_string(trace_id));
                    isBeginEvent = false;
                }
                size_t eventId = filler->enterEvent(0, eventLabel, event_id);
                if (!payload.empty()) {
                    filler->enterData_part(true);
                    for (const auto& [k,v] : payload) {
                        if (std::holds_alternative<std::string>(v)) {
                            filler->visitField(k, std::get<std::string>(v));
                        } else {
                            auto it = schema_def.find(keyV);
                            if (it != schema_def.end()) {
                                if (it->second == "discrete") {
                                    filler->visitField(k, (size_t)std::get<double>(v));
                                } else if (it->second == "boolean") {
                                    filler->visitField(k, (bool)(std::get<double>(v) == 1.0));
                                } else if (it->second == "timestamp") {
                                    payload[keyV] = std::get<double>(v);
                                } else {
                                    filler->visitField(k, std::get<double>(v));
                                }
                            } else {
                                filler->visitField(k, std::get<double>(v));
                            }
                        }
                    }
                    filler->exitData_part(true);
                }
                filler->exitEvent(eventId);
            }
//            std::cout << "storing polyadic event: " << trace_id << ", " << event_id << std::endl;
//            std::cout << "\t- event label: " << eventLabel << std::endl;
//            for (const auto& [k,v] : payload) {
//                if (std::holds_alternative<std::string>(v))
//                    std::cout << "\t\t" << k << ": " << std::get<std::string>(v) << std::endl;
//                else if (std::holds_alternative<double>(v))
//                    std::cout << "\t\t" << k << ": " << std::get<double>(v) << std::endl;
//            }
        }
        object_stack.pop();
        return true;
    }
    bool start_array(std::size_t elements) {
        if ((state == EVENT_HIERARCHY) && inHierarchyDef) {
            if (elements != -1)
                hierarchy_def[hierarchy].reserve(elements);
            return true;
        } else if (state == LOG) {
            return true;
        } else if (state == POLYADIC_EVENT) {
            event_id++;
            state = EVENT;
            return true;
        } else if (state == EVENT) {
            return true;
        }
        return false;
    }
    bool end_array() {
        if ((state == EVENT_HIERARCHY) && inHierarchyDef) {
            inHierarchyDef = false;
            hierarchy.clear();
            return true;
        } else if (state == LOG) {
            eventClassification.emplace_back().resize(event_id);
            return true;
        } else if (state == POLYADIC_EVENT) {
            state = LOG;
            return true;
        } else if (state == EVENT) {
            state = POLYADIC_EVENT;
            return true;
        }
        return false;
    }
// called when an object key is parsed; value is passed and can be safely moved away
    bool key(nlohmann::json::string_t& val) {
        if ((state == BEGINNING) && (object_stack.top()) && (!isSchemaOk) && (!isEventHierarchyOk) && (val == "schema")) {
            trace_id = -1;
            event_id = -1;
            isBeginEvent = true;
            state = SCHEMA;
            return true;
        } else if ((state == SCHEMA) && (object_stack.top())) {
            keyV = val;
            return true;
        } else if ((state == BEGINNING) && (object_stack.top()) && isSchemaOk && (!isEventHierarchyOk) && (val == "event_hierarchy")) {
            state = EVENT_HIERARCHY;
            return true;
        } else if ((state == EVENT_HIERARCHY) && (object_stack.top()) && isSchemaOk && (!isEventHierarchyOk) && (!inHierarchyDef)) {
            inHierarchyDef = true;
            hierarchy = val;
            return true;
        } else if ((state == BEGINNING) && (object_stack.top()) && isSchemaOk && isEventHierarchyOk && (val == "log")) {
            state = LOG;
            return true;
        } else if ((state == TRACE)) {
            if (val == "__name") {
                traceNameOk = true;
                return true;
            } else if (val == "__payload") {
                payload.clear();
                tracePayloadOk = true;
                return true;
            } else if (val == "__events") {
                state = POLYADIC_EVENT;
                event_id = -1;
                isBeginEvent = true;
                return true;
            }
            if (tracePayloadOk) {
                keyV = val;
                return true;
            }
        } else if (state == EVENT) {
            keyV = val;
            return true;
        }
        return true;
    }

// called when a parse error occurs; byte position, the last token, and an exception is passed
    bool parse_error(std::size_t position, const std::string& last_token, const nlohmann::json::exception& ex) {
        std::cerr << "Error at " << position << " near to " << last_token << ": " << ex.what() << std::endl;
    }
};
#include <fstream>
#include "submodules/yaucl/include/yaucl/learning/decision_tree/dt_predicate.h"
#include "submodules/yaucl/include/yaucl/bpm/structures/commons/DataPredicate.h"

static inline void actualClauseRefine(std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>> &result,
        const std::pair<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> &pair) {
//    DeclareDataAware c = clause;
    for(const std::pair<double,std::vector<dt_predicate>>& cond : pair.second){
        std::unordered_map<std::string, DataPredicate> current_conds;
        bool hasAFalse = false;

        for(const dt_predicate& dt_p : cond.second){
            DataPredicate p;
            p.is_left_for_activation = false;
            p.var = dt_p.field;
            switch (dt_p.pred) {
                case dt_predicate::LEQ_THAN:
                    p.casusu = LEQ;
                    break;
                case dt_predicate::G_THAN:
                    p.casusu = GT;
                    break;
                case dt_predicate::GEQ_THAN:
                    p.casusu = GEQ;
                    break;
                case dt_predicate::IN_SET:
                    p.casusu = EQ;
                    break;
                case dt_predicate::NOT_IN_SET:
                    p.casusu = NEQ;
                    break;
            }

            if(!dt_p.categoric_set.empty()){
                DEBUG_ASSERT(dt_p.categoric_set.size() == 1);
                p.value = *dt_p.categoric_set.begin();
            } else{
                p.value = dt_p.value;
            }

            auto found = current_conds.find(p.var);

            if(found != current_conds.end()){
                // Our path has two conditions on the same var, perform intersection
//                p.intersect_with(found->second);
                if (!found->second.intersect_with(p)) {
                    found->second.casusu = FFALSE;
                    hasAFalse = true;
                    break;
                }
            }
            else{
                current_conds.insert({p.var, p});
            }
        }
        if (hasAFalse) {
            current_conds.clear();
        } else {
            result.emplace_back(pair.first, current_conds);
        }
    }
}

static inline std::pair<int,size_t> test_single_conjunction(const std::unordered_map<int, std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> & model,
                                                            const std::unordered_map<std::string, union_minimal>& payload) {
    size_t sub_class = 0;
    for (const auto& [clazz, disj] : model) {
        for (const auto& [score,map] : disj) {
            bool found = false;
            for (const auto& [k,v] : map) {
                auto it = payload.find(k);
                if ((it == payload.end()) ? v.testOverSingleVariable(0.0) : v.testOverSingleVariable(it->second)) {
                    found = true;
                } else {
                    found = false;
                    break;
                }
            }
            if (found) {
                return {clazz, sub_class};
            }
            sub_class++;
        }
    }
    return {-1, 0};
}

struct comparator {
    _GLIBCXX14_CONSTEXPR
    bool
    operator()(const std::pair<std::vector<double>::iterator,size_t>& __x, const std::pair<std::vector<double>::iterator,size_t>& __y) const
    { return (*__x.first > *__y.first) || ((*__x.first == *__y.first) && (__x.second > __y.second));
    }
};

int main() {
    ServerQueryManager sqm;
    myParser sax;
    std::cout << std::setprecision (15);
    sax.ignore_keys = {"day","span","__class","__label"};
    sax.traceDistinguisher = "user";
    std::string path{"/home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json"}; // "simple.json" /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json

    // Storing in the main memory the "header" event of each polyadic event
    {
        std::ifstream f{path};

        if (!nlohmann::json::sax_parse(f, &sax)) {
            std::cerr << "ERROR while parsing the file: " << path << std::endl;
            return 1;
        }
    }
    auto it = sax.for_preliminary_classification.begin();
    auto en = sax.for_preliminary_classification.end();

    bool reclassify = true;
    if (reclassify) {
        // Using the sub-cases identified within the decision tree to target different sub-classes or cases
        DecisionTree<std::unordered_map<std::string, union_minimal>> dt(it,
                                                                        en,
                                                                        sax.class_to_int.size(),
                                                                        [](const auto& map, const std::string& key) {
                                                                            auto it = map.find(key);
                                                                            if (it == map.end())
                                                                                return (union_minimal)0.0;
                                                                            else
                                                                                return it->second;
                                                                        },
                                                                        sax.numerical,
                                                                        {},
                                                                        ForTheWin::gain_measures::Gini,
                                                                        0.97,
                                                                        1,
                                                                        1,
                                                                        1,
                                                                        false,
                                                                        nullptr,
                                                                        nullptr,
                                                                        5);

        std::unordered_map<int, std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>>> actual_result;
        {

            std::unordered_map<int, std::vector<std::pair<double,std::vector<dt_predicate>>>> result;
            dt.populate_children_predicates2(result);
            for (const auto& kv : result) {
                std::vector<std::pair<double,std::unordered_map<std::string, DataPredicate>>> current_conds;
                actualClauseRefine(current_conds, kv);
                actual_result.emplace(kv.first, current_conds);
            }
        }
        double ok = 0;
        for (size_t idx = 0, N = sax.for_preliminary_classification.size(); idx<N; idx++) {
            const auto& posToTraceInfo = sax.components.at(idx);
            const auto& elements = sax.for_preliminary_classification.at(idx);
            auto cp = test_single_conjunction(actual_result, elements.first);
            sax.eventClassification[posToTraceInfo.first][posToTraceInfo.second] = cp.second;
            if (elements.second == cp.first) {
                ok++;
            }
        }
        std::cout << "Re-classification precision: " << ok/((double)sax.for_preliminary_classification.size()) << std::endl;
    } else {
        // Keeping the same original classes for each event
        for (size_t idx = 0, N = sax.for_preliminary_classification.size(); idx<N; idx++) {
            const auto& posToTraceInfo = sax.components.at(idx);
            const auto& elements = sax.for_preliminary_classification.at(idx);
            sax.eventClassification[posToTraceInfo.first][posToTraceInfo.second] = elements.second;
        }
        // No reclassification, so precision = 1.0
    }


    // Now, after the re-classification, we can collect back the time intervals
    sax.isClassificationDone = true;
    sax.timeLabel = "fulltime";
    sax.clear();
    {
        std::ifstream f{path};
        std::cout << nlohmann::json::sax_parse(f, &sax) << std::endl;
    }

    for (auto& [user_key, timestamps] : sax.clazz_to_time) {
        std::cout << user_key << std::endl;
        remove_duplicates(timestamps);
        std::vector<std::vector<std::pair<double,double>>>& intervals = sax.clazz_to_time_intervals[user_key];
        intervals.resize(sax.clazz_to_time.size());
        std::priority_queue<std::pair<std::vector<double>::iterator,size_t>, std::vector<std::pair<std::vector<double>::iterator,size_t>>, comparator> minheap;
        int k = timestamps.size();
        int i;
        for (i = 0; i < k; i++){
            auto ptr = timestamps[i].begin(), end = timestamps[i].end();
            if (ptr != end) {
                minheap.emplace(ptr, i);
            }
        }

        //int count = 0;
//    long written = 0;
        ssize_t current = -1;
        double previousValue = -1;

        // Now one by one get the minimum element from min
        // heap and replace it with next element.
        // run till all filled input files reach EOF
        while (!minheap.empty()) {
            // Get the minimum element and store it in output file
            std::pair<std::vector<double>::iterator,size_t> x = minheap.top();
            std::cout << *x.first << "@" << x.second << std::endl;
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
            //std::cout << std::string((const char*)x.iovec.iov_base, x.iovec.iov_len) << std::endl;
//        out.writeKeyAndValue(x.iovec);
            minheap.pop();

            // Find the next element that will replace current
            // root of heap. The next element belongs to same
            // input file as the current min element.
            if (((x.first+1) != timestamps[fileid].end())) {
                minheap.emplace(x.first+1, fileid);
            }
        }
    }

    std::string filename = path;
        bool oldLoading = true;
        bool load_also_data = true;
        bool doStats = false;
        bool index_missing_data = false;
        std::string env_name = "test1";
        if (sqm.multiple_logs.contains(env_name)) {
            sqm.multiple_logs.erase(env_name);
        }
        auto& env = sqm.multiple_logs[env_name];
        log_data_format format;

        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::duration;
        using std::chrono::milliseconds;
        env.experiment_logger.log_filename = filename;

        {
            //log_data_format format, bool loadData, std::istream &stream, KnowledgeBase &output,
            //                              std::string &filename
            auto t1 = high_resolution_clock::now();
            // Loading
            env.db.enterLog(path, env_name);
            sax.filler = &env.db;
            sax.clear();
            {
                // TODO: store the log in different environments depending on the annotated class being provided
                std::ifstream f{path};
                std::cout << nlohmann::json::sax_parse(f, &sax) << std::endl;
            }
//            env.db.exitLog(path, env_name);
            auto t2 = high_resolution_clock::now();

            /* Getting number of milliseconds as a double. */
            duration<double, std::milli> ms_double = t2 - t1;
            env.experiment_logger.log_loading_and_parsing_ms = ms_double.count();
        }


        {
            auto t1 = high_resolution_clock::now();
            env.db.index_data_structures(index_missing_data);
            auto t2 = high_resolution_clock::now();

            /* Getting number of milliseconds as a double. */
            duration<double, std::milli> ms_double = t2 - t1;
            env.experiment_logger.log_indexing_ms = ms_double.count();
        }
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

        std::cout << "Loading and parsing: " << env.experiment_logger.log_loading_and_parsing_ms << " (ms)" << std::endl;
        std::cout << "Indexing: " << env.experiment_logger.log_indexing_ms << " (ms)" << std::endl;

    return 0;
}
