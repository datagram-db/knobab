//
// Created by giacomo on 05/10/24.
//

#ifndef DTMINING_POLYADICJSONSERIALIZE_H
#define DTMINING_POLYADICJSONSERIALIZE_H

#include <unordered_map>
#include <unordered_set>
#include "json_writer.hpp"
#include "schema.propo.h"
#include <dt_mining/data/CacheConstituent.h>

enum XESTypes {
    XES_Literal,
    XES_Continuous,
    XES_Boolean,
    XES_Discrete
};



class PolyadicJSONSerialize {
    JsonWriter jw;
    std::unordered_map<std::string,std::unordered_set<std::string>> hierarchy;
    std::unordered_map<std::string, XESTypes> types;

    std::unordered_map<std::string,XESTypes> conversion_recto;
    std::unordered_map<XESTypes,std::string> conversion_verso;
public:
    PolyadicJSONSerialize(std::ostream& os);



    inline void start() {
        jw.begin_object();
    }

    inline void heading() {
        serialize_schema();
        serialize_hierarchy();
    }

    inline void set_up_hierarchy(const std::string& label) {
        auto begin_offset = label.find_first_of("(");
        auto end_offset = label.find_last_of(")");
        if ((begin_offset != std::string::npos) && (end_offset != std::string::npos)) {
            hierarchy[label.substr(0, begin_offset)].insert(label.substr(begin_offset+1, end_offset-begin_offset-1));
        } else {
            hierarchy[label].insert(label);
        }
    }
    inline void set_up_numeric_variables(const std::string& numeric) {
        types.emplace(numeric, XES_Continuous);
    }

    inline void start_serializing_log_within_object() {
        jw.name("log");
        jw.begin_array();
    }

    inline void start_serializing_trace_within_log(const std::string& env_name) {
        jw.begin_object();
        jw.name("__name");      jw.value(env_name);
        jw.name("__payload");
          jw.begin_object();
            jw.name("__user");  jw.value(env_name);
          jw.end_object();
        jw.name("__events");
        jw.begin_array();
    }

    inline void start_event() {
        jw.begin_array();
    }

    /**
 * This serializes the raw data from an original time series dataset, and dumps it as a JSON object constituent
 * @param dimNames              Original name of the dimensions within the dataset
 * @param allDimensions         All the loaded dimensions from the dataset
 * @param timeOfInterest        The specific timeframe of interest to be serialized
 * @param clazz                 The class associated to this specific element
 */
    inline void json_serialize_raw_data_constituent(const std::vector<std::string>& dimNames,
                                               const std::vector<std::vector<double>>& allDimensions,
                                               size_t timeOfInterest,
                                               const std::string& clazz) {
        jw.begin_object();
        jw.name("__label"); jw.value("__raw_data");
        jw.name("__class"); jw.value(clazz);
        jw.name("span");    jw.value(1);
        jw.name("time");    jw.value((long)timeOfInterest);
        for (size_t idx = 0, N = dimNames.size(); idx<N; idx++) {
            types.emplace(dimNames.at(idx), XES_Continuous);
            jw.name(dimNames.at(idx));
            jw.value(allDimensions[idx][timeOfInterest]);
        }
        jw.end_object();
    }

    inline void json_serialize_constituent(const CacheConstituent& constituent) {
        if ((!constituent.clazz.empty()) && (constituent.label == "__raw_data") && (constituent.span == 1)) {
            jw.begin_object();
            jw.name("__label"); jw.value(constituent.label);
            jw.name("__class"); jw.value(constituent.clazz);
            jw.name("span");    jw.value((long)constituent.span);
            jw.name("time");    jw.value((long)constituent.disctime);
            for (const auto&  [k,v] : constituent.payload) {
                types.emplace(k, XES_Continuous);
                jw.name(k);     jw.value(v);
            }
            jw.end_object();
        } else {
            jw.begin_object();
            jw.name("__label"); jw.value(constituent.label);
            jw.name("span");    jw.value((long)constituent.span);
            jw.name("time");    jw.value((long)constituent.disctime);
            for (const auto&  [k,v] : constituent.payload) {
                types.emplace(k, XES_Continuous);
                jw.name(k);     jw.value(v);
            }
            jw.end_object();
        }
    }


    /**
     * This serializes a durative event being generated from the
     * @param dt_pattern_name
     * @param dimension_name
     * @param span
     * @param timeOfInterest
     * @param payload
     */
    inline void json_serialize_durational_event(const std::string& dt_pattern_name,
                                           const std::string& dimension_name,
                                           size_t span,
                                           size_t timeOfInterest,
                                           const std::unordered_map<std::string,double>& payload) {
        auto label = dt_pattern_name + "(" + dimension_name +")";
        hierarchy[dimension_name].emplace(label);
        jw.begin_object();
        jw.name("__label"); jw.value(label);
        jw.name("span");    jw.value((long)span);
        jw.name("time");    jw.value((long)timeOfInterest);
        for (const auto&  [k,v] : payload) {
            types.emplace(k, XES_Continuous);
            jw.name(k);     jw.value(v);
        }
        jw.end_object();
    }

    inline void finish_event() {
        jw.end_array();
    }

    inline void finish_serializing_trace_within_log() {
        jw.end_array();
        jw.end_object();
    }

    inline void end_serializing_log_within_object() {
        jw.end_array();
    }

    inline void finish() {
        jw.end_object();
        jw.close_all();
        jw.assert_finished();
    }

private:
    inline void serialize_hierarchy() {
        jw.name("event_hierarchy");
        jw.begin_object();
        for (const auto& [k,S] : hierarchy) {
            jw.name(k);
            jw.begin_array();
            for (const auto& x : S) {
                jw.value(x+"("+k+")");
            }
            jw.end_array();
        }
        jw.end_object();
    }

    inline void serialize_schema() {
        jw.name("schema");
        jw.begin_object();
        for (const auto& [k,v] : types) {
            jw.name(k); jw.value(conversion_verso[v]);
        }
        jw.end_object();
    }

};


#endif //DTMINING_POLYADICJSONSERIALIZE_H
