//
// Created by giacomo on 14/02/24.
//

#ifndef KNOBAB_SERVER_BASIC_POLYADIC_H
#define KNOBAB_SERVER_BASIC_POLYADIC_H

#include <yaucl/structures/default_constructors.h>
#include <string>

struct new_event_coordinates {
    size_t trace_id;
    size_t event_id;
    std::string user_name;
    double specific_time;
    size_t final_class;
    bool first_time = true;

    DEFAULT_CONSTRUCTORS(new_event_coordinates)

    inline std::string get_log_name() const {
        return user_name + "_" + std::to_string(final_class);
    }
};

struct counting_event_coordinates {
    size_t latest_new_trace_id = -1;
    size_t latest_new_event_id = -1;
    size_t global_trace_id = -1;
    size_t global_event_id = -1;

    DEFAULT_CONSTRUCTORS(counting_event_coordinates)
};

#endif //KNOBAB_SERVER_BASIC_POLYADIC_H
