//
// Created by giacomo on 14/02/24.
//

#ifndef KNOBAB_SERVER_MYPARSER_H
#define KNOBAB_SERVER_MYPARSER_H

#include <knobab/server/dataStructures/polyadic/basic_polyadic.h>

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
    std::unordered_map<std::string, Environment>* filler{nullptr};
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
    std::map<std::pair<std::string,size_t>, counting_event_coordinates> log_name_to_traceIdEventId;
    yaucl::structures::any_to_uint_bimap<std::string> class_to_int;
    std::unordered_set<std::string> numerical, categorical;

//    std::vector<std::vector<size_t>> eventClassification;
    bool isClassificationDone = false;
    std::vector<std::pair<size_t,size_t>> components;
    std::vector<std::pair<std::unordered_map<std::string, union_minimal>, int>> for_preliminary_classification;
//    std::vector<size_t> classId;
//    bool storeTimeIntervalsWithClazzez = false;
    std::unordered_map<std::string, std::vector<std::vector<double>>> clazz_to_time;
//    std::unordered_map<std::string, std::map<std::pair<double,double>, size_t>> userName_to_intervals_to_class;
    std::vector<std::vector<new_event_coordinates>> event_coordinates;
    bool event_coordinates_init = true;
//    std::unordered_map<std::string, std::vector<std::vector<std::pair<double,double>>>> clazz_to_time_intervals;
    std::string timeLabel, traceDistinguisher, traceDistinguisherValue;

//    inline bool retrieve_trace_id(const std::string& userName, double time, size_t& out_tuple_id, size_t& out_class_id) const {
//        auto it = userName_to_intervals_to_class.find(userName);
//        if (it == userName_to_intervals_to_class.end())
//            return false;
//        out_tuple_id = 0;
//        for (const auto& [pair, clazz] : it->second) {
//            if ((time >= pair.first) && (time <= pair.second)) {
//                out_class_id = clazz;
//                return true;
//            }
//            out_tuple_id++;
//        }
//        return false;
//    }

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
            if (filler) {
                if ((trace_id != (size_t)-1) && ((event_id != (size_t)-1)) && (!event_coordinates_init)){
                    const auto& zncs = event_coordinates.at(trace_id).at(event_id);
                    filler->at(zncs.get_log_name()).db.exitTrace(zncs.trace_id);
                }
            }
            trace_id++;
            if ((trace_id != 0) && (event_coordinates_init)) {
                event_coordinates.emplace_back().resize(event_id+1);
//                eventClassification.emplace_back().resize(event_id);
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
//                std::cout << trace_id << "," << event_id << ":" << std::get<std::string>(payload["__class"]) << std::endl;
                size_t classInt = class_to_int.put(std::get<std::string>(payload["__class"])).first;
                if (!event_coordinates_init)
                    event_coordinates[trace_id][event_id].user_name = traceDistinguisherValue;
                if (!timeLabel.empty()) {
                    size_t finalClass;
                    if (event_coordinates.empty() && (!event_coordinates_init)) {
                        finalClass = classInt;
                    } else {
                        finalClass = event_coordinates[trace_id][event_id].final_class;
                    }
                    event_coordinates[trace_id][event_id].specific_time = std::get<double>(payload.at(timeLabel));
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
                auto& zncs = event_coordinates.at(trace_id).at(event_id);
                auto& envFiller = filler->at(zncs.get_log_name()).db;
                if ((zncs.event_id == 0) && zncs.first_time) {
                    envFiller.enterTrace(std::to_string(zncs.trace_id));
                }
                zncs.first_time = false;
                size_t eventId = envFiller.enterEvent(0, eventLabel, zncs.event_id);
                if (!payload.empty()) {
                    envFiller.enterData_part(true);
                    for (const auto& [k,v] : payload) {
                        if (std::holds_alternative<std::string>(v)) {
                            envFiller.visitField(k, std::get<std::string>(v));
                        } else {
                            auto it = schema_def.find(keyV);
                            if (it != schema_def.end()) {
                                if (it->second == "discrete") {
                                    envFiller.visitField(k, (size_t)std::get<double>(v));
                                } else if (it->second == "boolean") {
                                    envFiller.visitField(k, (bool)(std::get<double>(v) == 1.0));
                                } else if (it->second == "timestamp") {
                                    envFiller.visitField(k, std::get<double>(v));
//                                    payload[keyV] = std::get<double>(v);
                                } else {
                                    envFiller.visitField(k, std::get<double>(v));
                                }
                            } else {
                                envFiller.visitField(k, std::get<double>(v));
                            }
                        }
                    }
                    envFiller.exitData_part(true);
                }
                envFiller.exitEvent(eventId);
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
            if (event_coordinates_init)
                event_coordinates.emplace_back().resize(event_id+1);
//            eventClassification.emplace_back().resize(event_id);
            return true;
        } else if (state == POLYADIC_EVENT) {
            state = LOG;
            return true;
        } else if (state == EVENT) {
            if (filler &&
                (!event_coordinates.empty()) &&
                (!event_coordinates_init) &&
                (event_coordinates.at(trace_id).size() > event_id)) {
                auto& zncs = event_coordinates.at(trace_id).at(event_id);
                if (zncs.first_time) {
                    auto& envFiller = filler->at(zncs.get_log_name()).db;
                    if ((envFiller.noTraces == zncs.trace_id) && (zncs.event_id == 0)) {
                        envFiller.exitTrace(zncs.trace_id-1);
                        envFiller.enterTrace(std::to_string(zncs.trace_id));
                    }
                    auto e = envFiller.enterEvent(0, "__missing", zncs.event_id);
                    envFiller.exitEvent(e);
                    zncs.first_time = false;
                }
            }
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


#endif //KNOBAB_SERVER_MYPARSER_H
