#include <iostream>
#include <yaucl/data/json.h>
#include <variant>
#include <yaucl/structures/any_to_uint_bimap.h>
//using union_minimal = std::variant<std::string, size_t, long long, double>;

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
    std::vector<std::pair<int,size_t>> subclasses;
    std::vector<std::pair<size_t,size_t>> components;
    parsing_states state{BEGINNING};
    bool isSchemaOk = false;
    bool isEventHierarchyOk = false;
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
    std::vector<std::pair<std::unordered_map<std::string, union_minimal>, int>> for_preliminary_classification;

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
                payload[keyV] = (double)val;
                return true;
            }
        }
        return false;
    }
    bool number_unsigned(nlohmann::json::number_unsigned_t val) {
        if (state == TRACE) {
            if (tracePayloadOk) {
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
                auto it = schema_def.find(keyV);
                if (it != schema_def.end()) {
                    if (it->second == "continuous") {
                        payload[keyV] = (double)std::stod(val);
                    } else if (it->second == "discrete") {
                        payload[keyV] = (double)std::stoll(val);
                    } else if (it->second == "boolean") {
                        payload[keyV] = (tolower(val[0]) == 't') ? 1.0 : 0.0;
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
            trace_id++;
            event_id = -1;
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
//                    auto& array = for_preliminary_classification[];
//                    array.emplace_back(payload);
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
//                if (!payload.empty())
                components.emplace_back(trace_id, event_id);
                for_preliminary_classification.emplace_back(payload, (int)classInt);
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
    for (const auto& [clazz, disj] : model) {
        size_t sub_class = 0;
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

int main() {
    myParser sax;
    sax.ignore_keys = {"day","span","__class","__label"};
    std::string path{"/home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json"};
    std::ifstream f{path};
    std::cout << nlohmann::json::sax_parse(f, &sax) << std::endl;
    auto it = sax.for_preliminary_classification.begin();
    auto en = sax.for_preliminary_classification.end();

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
//    for (const auto& [clazz, cases] : actual_result) {
//        std::cout << "class = " << clazz << std::endl;
//        for (const auto& [score,map] : cases) {
//            std::cout << " * with score = " << score << " having: ";
//            for (const auto& [k,v] : map) {
//                std::cout << v << " && ";
//            }
//            std::cout << std::endl;
//        }
//    }

    double ok = 0;
    for (const auto& elements : sax.for_preliminary_classification) {
        auto cp = test_single_conjunction(actual_result, elements.first);
        sax.subclasses.emplace_back(cp);
        if (elements.second == cp.first) {
            ok++;
        }
    }
    std::cout << ok/((double)sax.for_preliminary_classification.size()) << std::endl;

    return 0;
}
