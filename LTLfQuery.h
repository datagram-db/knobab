//
// Created by giacomo on 12/03/2022.
//

#ifndef KNOBAB_LTLFQUERY_H
#define KNOBAB_LTLFQUERY_H

#include <cstddef>
#include <yaucl/structures/bit_tagged_unions.h>

#define DECLARE_TYPE_NONE   (0)
#define DECLARE_TYPE_LEFT   (1)
#define DECLARE_TYPE_RIGHT  (2)


class LTLfQuery {
    struct bit_fields {
        bool has_theta : 1, preserve : 1, is_atom : 1;
    };
    enum __attribute__((__packed__)) type {
        INIT_Q = 0,
        END_Q = 1,
        EXISTS_Q = 2,
        ABSENCE_Q = 3,
        NEXT_Q = 4,
        OR_Q = 5,
        AND_Q = 6,
        IMPL_Q = 7,
        U_Q = 8,
        G_Q = 9,
        F_Q = 10,
        NOTU_Q = 11
    };
    type t;
    short declare_type = 0;
    bit_fields fields = {0};
    size_t n;

    void test();
};


#endif //KNOBAB_LTLFQUERY_H
