//
// Created by giacomo on 31/10/2021.
//

#include <unordered_map>
#include <string>
#include <vector>
#include <variant>
#include "yaucl/bpm/log/data_loader.h"
#include "yaucl/data/xml.h"
#include "yaucl/numeric/numeric_base.h"


void parse_payload(bool isTrace, rapidxml::xml_node<>* payloadNode, trace_visitor* tv) {
    std::string tag_name  = payloadNode->name();
    std::string attribute = GET_ATTRIBUTE(payloadNode, "key");
    std::string value     = GET_ATTRIBUTE(payloadNode, "value");
    constexpr std::basic_string_view concept_name{"concept:name"};
    constexpr std::basic_string_view timeTimestamp{"time:timestamp"};
    assert(tv);
    if (tag_name == "string") {
        assert(attribute != concept_name);
        tv->visitField(attribute, value);
    } else if (tag_name == "float") {

        tv->visitField(attribute, (double)std::stod(value));
    } else if (tag_name == "int") {
        tv->visitField(attribute, (size_t)std::stoull(value));
    } else if (tag_name == "date") {
        if (!isTrace) {
            assert(attribute != timeTimestamp);
        }
        tv->visitField(attribute, (double)yaucl::numeric::parse8601(value));
    }
}


enum XML_SCAN_STEPS {
    LABELS_TRACE_INFO = 0,
    TRACE_PAYLOAD = 1,
    EVENTS = 2
};

#include <chrono>

void parse_event(rapidxml::xml_node<>* event, trace_visitor* tv, bool load_data) {
    constexpr std::basic_string_view concept_name{"concept:name"};
    constexpr std::basic_string_view timeTimestamp{"time:timestamp"};
    assert(tv);
    size_t event_id = 0;
    bool hasEventOpened;

    SIBLING_ITERATE2(t, event) {
        std::string tag_name  = t->name();
        std::string attribute = GET_ATTRIBUTE(t, "key");
        std::string value     = GET_ATTRIBUTE(t, "value");
        std::string trace_name = std::to_string(event_id++);
        XML_SCAN_STEPS event_start = XML_SCAN_STEPS::LABELS_TRACE_INFO;
        const auto p1 = std::chrono::system_clock::now();
        unsigned long long int timestamp = std::chrono::duration_cast<std::chrono::hours>(p1.time_since_epoch()).count();

        if (tag_name == "string") {
            if (attribute == concept_name) {
                assert(event_start == XML_SCAN_STEPS::LABELS_TRACE_INFO);
                trace_name = value;
            } else {
                assert(event_start != XML_SCAN_STEPS::EVENTS);
                if (event_start == XML_SCAN_STEPS::LABELS_TRACE_INFO) {
                    event_id = tv->enterEvent(timestamp, trace_name);
                    event_start = XML_SCAN_STEPS::TRACE_PAYLOAD;
                    hasEventOpened = true;
                }
                if (load_data) parse_payload(true, t, tv);
            }
        } else if (tag_name == "date") {
            if (attribute == timeTimestamp) {
                assert(event_start == XML_SCAN_STEPS::LABELS_TRACE_INFO);
                timestamp = yaucl::numeric::parse8601(value);
            } else {
                assert(event_start != XML_SCAN_STEPS::EVENTS);
                if (event_start == XML_SCAN_STEPS::LABELS_TRACE_INFO) {
                    event_id = tv->enterEvent(timestamp, trace_name);
                    event_start = XML_SCAN_STEPS::TRACE_PAYLOAD;
                    hasEventOpened = true;
                }
                if (load_data) parse_payload(true, t, tv);
            }
        } else {
            assert(event_start != XML_SCAN_STEPS::EVENTS);
            if (event_start == XML_SCAN_STEPS::LABELS_TRACE_INFO) {
                event_id = tv->enterEvent(timestamp, trace_name);
                event_start = XML_SCAN_STEPS::TRACE_PAYLOAD;
                hasEventOpened = true;
            }
            if (load_data) parse_payload(true, t, tv);
        }
    }

    if (hasEventOpened) {
        tv->exitEvent(event_id);
    }
}

void load_xes_with_data(const std::string &filename, bool load_data, trace_visitor* tv) {
    assert(tv);
    rapidxml::xml_document<> doc;
    std::vector<char> buffer;
    rapidxml::xml_node<> *root_node =  yaucl::data::init_XML_parse(filename, "log", doc, buffer);
    constexpr std::basic_string_view concept_name{"concept:name"};
    constexpr std::basic_string_view timeTimestamp{"time:timestamp"};
    ///yaucl::bpm::log log;

    tv->enterLog(filename, filename);

    SIBLING_ITERATE(trace, root_node, "trace", false) {

        ///yaucl::bpm::trace& transaction = log.traces.emplace_back();
        long long int previous = 0;
        XML_SCAN_STEPS event_start = XML_SCAN_STEPS::LABELS_TRACE_INFO;
        size_t trace_id = 0;
        bool hasTraceOpened = false;

        SIBLING_ITERATE2(t, trace) {
            std::string tag_name  = t->name();
            std::string attribute = GET_ATTRIBUTE(t, "key");
            std::string value     = GET_ATTRIBUTE(t, "value");
            std::string trace_name = std::to_string(trace_id++);

            if (tag_name == "event") {
                event_start = XML_SCAN_STEPS::EVENTS;
                parse_event(t, tv, load_data);
            } else if (tag_name == "string") {
                if (attribute == concept_name) {
                    assert(event_start == XML_SCAN_STEPS::LABELS_TRACE_INFO);
                    trace_name = value;
                } else {
                    assert(event_start != XML_SCAN_STEPS::EVENTS);
                    if (event_start == XML_SCAN_STEPS::LABELS_TRACE_INFO) {
                        trace_id = tv->enterTrace(trace_name);
                        event_start = XML_SCAN_STEPS::TRACE_PAYLOAD;
                        hasTraceOpened = true;
                    }
                    if (load_data) parse_payload(true, t, tv);
                }
            } else {
                assert(event_start != XML_SCAN_STEPS::EVENTS);
                if (event_start == XML_SCAN_STEPS::LABELS_TRACE_INFO) {
                    trace_id = tv->enterTrace(trace_name);
                    event_start = XML_SCAN_STEPS::TRACE_PAYLOAD;
                    hasTraceOpened = true;
                }
                if (load_data) parse_payload(true, t, tv);
            }
        }

        if (hasTraceOpened) {
            tv->exitTrace(trace_id);
        }
    }

    tv->exitLog(filename, filename);
}

#include "yaucl/bpm/DataTraceParse.h"

void load_into_knowledge_base(log_data_format format, bool loadData, const std::string &filename, KnowledgeBase &output) {
    trace_visitor* tv = (trace_visitor*)&output;
    switch (format) {
        case HUMAN_READABLE_YAUCL: {
            yaucl::bpm::DataTraceParse dp;
            std::ifstream file{filename};
            dp.load(filename, file, tv);
            break;
        }
        case XES1:                 load_xes_with_data(filename, loadData, tv); break;
    }
}
