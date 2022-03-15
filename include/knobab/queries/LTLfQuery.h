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

    bool operator==(const LTLfQuery &rhs) const {
        return t == rhs.t &&
               declare_type == rhs.declare_type &&
               fields == rhs.fields &&
               n == rhs.n;
    }

    bool operator!=(const LTLfQuery &rhs) const {
        return !(rhs == *this);
    }

    static LTLfQuery qINIT(short declare_argument, bool isTimed) {
        LTLfQuery q;
        q.t = INIT_QP;
        q.n = 0;
        q.declare_type = declare_argument;
        q.fields.id.parts.has_theta = false;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        return q;
    }
    static LTLfQuery qEND(short declare_argument, bool isTimed) {
        LTLfQuery q;
        q.t = END_QP;
        q.n = 0;
        q.declare_type = declare_argument;
        q.fields.id.parts.has_theta = false;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        return q;
    }
    static LTLfQuery qEXISTS(size_t narg, short declare_argument, bool isTimed) {
        LTLfQuery q;
        q.t = EXISTS_QP;
        q.n = narg;
        q.declare_type = declare_argument;
        q.fields.id.parts.has_theta = false;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        return q;
    }
    static LTLfQuery qABSENCE(size_t narg, short declare_argument, bool isTimed) {
        LTLfQuery q;
        q.t = ABSENCE_QP;
        q.n = narg;
        q.declare_type = declare_argument;
        q.fields.id.parts.has_theta = false;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        return q;
    }
    static LTLfQuery qNEXT(const LTLfQuery& arg, bool isTimed) {
        LTLfQuery q;
        q.t = NEXT_QP;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = false;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(arg);
        return q;
    }
    static LTLfQuery qNOT(const LTLfQuery& arg, bool isTimed, bool preserve) {
        LTLfQuery q;
        q.t = NOT_QP;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = false;
        q.fields.id.parts.preserve = preserve;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(arg);
        return q;
    }
    static LTLfQuery qOR(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
        LTLfQuery q;
        q.t = OR_QP;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = hasTheta;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(lhs);
        q.args.emplace_back(rhs);
        return q;
    }
    static LTLfQuery qIMPLICATION(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
        LTLfQuery q;
        q.t = IMPL_QP;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = hasTheta;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(lhs);
        q.args.emplace_back(rhs);
        return q;
    }
    static LTLfQuery qIFTE(const LTLfQuery& lhs, const LTLfQuery& middle, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
        LTLfQuery q;
        q.t = IFTE_QP;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = hasTheta;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(lhs);
        q.args.emplace_back(middle);
        q.args.emplace_back(rhs);
        return q;
    }
    static LTLfQuery qAND(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
        LTLfQuery q;
        q.t = AND_QP;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = hasTheta;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(lhs);
        q.args.emplace_back(rhs);
        return q;
    }
    static LTLfQuery qANDNEXTGLOBALLY(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
        LTLfQuery q;
        q.t = AXG_QPT;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = hasTheta;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(lhs);
        q.args.emplace_back(rhs);
        return q;
    }
    static LTLfQuery qANDFUTURE(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
        LTLfQuery q;
        q.t = AF_QPT;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = hasTheta;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(lhs);
        q.args.emplace_back(rhs);
        return q;
    }
    static LTLfQuery qUNTIL(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
        LTLfQuery q;
        q.t = U_QP;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = hasTheta;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(lhs);
        q.args.emplace_back(rhs);
        return q;
    }
    static LTLfQuery qBOX(const LTLfQuery& lhs, bool isTimed) {
        LTLfQuery q;
        q.t = G_QP;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = false;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(lhs);
        return q;
    }
    static LTLfQuery qDIAMOND(const LTLfQuery& lhs, bool isTimed) {
        LTLfQuery q;
        q.t = F_QP;
        q.n = 0;
        q.declare_type = DECLARE_TYPE_NONE;
        q.fields.id.parts.has_theta = false;
        q.fields.id.parts.preserve = false;
        q.fields.id.parts.is_atom = false;
        q.fields.id.parts.is_timed = isTimed;
        q.args.emplace_back(lhs);
        return q;
    }
};


#endif //KNOBAB_LTLFQUERY_H
