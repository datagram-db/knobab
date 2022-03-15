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
#define DECLARE_TYPE_MIDDLE  (2)


struct LTLfQuery {
    struct bit_fields {
        bool has_theta : 1, preserve : 1, is_atom : 1;
    };
    enum __attribute__((__packed__)) type {
        INIT_QP = 0,
        END_QP = 1,
        EXISTS_QP = 2,
        ABSENCE_QP = 3,
        NEXT_QP = 4,
        OR_QP = 5,
        AND_QP = 6,
        IMPL_QP = 7,
        IFTE_QP = 8,
        U_QP = 9,
        G_QP = 10,
        F_QP = 11,
        NOT_QP = 12,
        AF_QPT = 13,
        AXG_QPT = 14,
        FALSEHOOD_QPT = 15
    };
    type t;
    short declare_type = 0;
    bit_fields fields = {0};
    size_t n;

    LTLfQuery() : t{FALSEHOOD_QPT}, declare_type{DECLARE_TYPE_NONE}, n{0}, fields{0} {}
};


#endif //KNOBAB_LTLFQUERY_H
