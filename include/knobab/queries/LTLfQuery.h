//
// Created by giacomo on 12/03/2022.
//

#ifndef KNOBAB_LTLFQUERY_H
#define KNOBAB_LTLFQUERY_H

#include <cstddef>
#include <vector>
#include <yaucl/structures/bit_tagged_unions.h>
#include <yaucl/structures/default_constructors.h>

#define DECLARE_TYPE_NONE   (0)
#define DECLARE_TYPE_LEFT   (1)
#define DECLARE_TYPE_RIGHT  (2)
#define DECLARE_TYPE_MIDDLE  (3)

TAGGED_UNION_WITH_ENCAPSULATION_BEGIN(unsigned char, bit_fields, 0, 4, bool has_theta : 1, bool preserve : 1, bool is_atom : 1, bool is_timed:1)
    (bool has_theta, bool preserve, bool is_atom,bool is_timed)  {
        id.parts.has_theta = has_theta;
        id.parts.preserve = preserve;
        id.parts.is_atom = is_atom;
        id.parts.is_timed = is_timed;
    }
TAGGED_UNION_ENCAPSULATOR_END

struct LTLfQuery {
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
    bit_fields fields;
    size_t n;
    std::vector<LTLfQuery> args;

    LTLfQuery() : t{FALSEHOOD_QPT}, declare_type{DECLARE_TYPE_NONE}, n{0}, fields{0} {}
    DEFAULT_COPY_ASSGN(LTLfQuery)

    bool operator==(const LTLfQuery &rhs) const;
    bool operator!=(const LTLfQuery &rhs) const;

    static LTLfQuery qINIT(short declare_argument, bool isTimed);
    static LTLfQuery qEND(short declare_argument, bool isTimed);
    static LTLfQuery qEXISTS(size_t narg, short declare_argument, bool isTimed);
    static LTLfQuery qABSENCE(size_t narg, short declare_argument, bool isTimed);
    static LTLfQuery qNEXT(const LTLfQuery& arg, bool isTimed);
    static LTLfQuery qNOT(const LTLfQuery& arg, bool isTimed, bool preserve);
    static LTLfQuery qOR(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta);
    static LTLfQuery qIMPLICATION(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta);
    static LTLfQuery qIFTE(const LTLfQuery& lhs, const LTLfQuery& middle, const LTLfQuery& rhs, bool isTimed, bool hasTheta);
    static LTLfQuery qAND(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta);
    static LTLfQuery qANDNEXTGLOBALLY(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta);
    static LTLfQuery qANDFUTURE(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta);
    static LTLfQuery qUNTIL(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta);
    static LTLfQuery qBOX(const LTLfQuery& lhs, bool isTimed);
    static LTLfQuery qDIAMOND(const LTLfQuery& lhs, bool isTimed);
};

#include <yaucl/hashing/vector_hash.h>
#include <yaucl/hashing/hash_combine.h>

namespace std {
    template <>
    struct hash<struct LTLfQuery>
    {
        std::size_t operator()(const LTLfQuery& k) const
        {
            using yaucl::hashing::hash_combine;
            size_t init = 31;
            for (const auto& x : k.args)
                init = hash_combine<LTLfQuery>(init, x);
            size_t f= hash_combine<size_t>(hash_combine<unsigned char>(hash_combine<short>(hash_combine<size_t>(init, k.t), k.declare_type), k.fields.id.data), k.n);
            return f;
        }
    };

}

#endif //KNOBAB_LTLFQUERY_H
