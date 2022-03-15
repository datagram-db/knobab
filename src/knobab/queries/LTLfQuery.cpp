//
// Created by giacomo on 12/03/2022.
//

#include <knobab/queries/LTLfQuery.h>


bool LTLfQuery::operator==(const LTLfQuery &rhs) const {
    return t == rhs.t &&
           declare_type == rhs.declare_type &&
           fields == rhs.fields &&
           n == rhs.n;
}

bool LTLfQuery::operator!=(const LTLfQuery &rhs) const {
    return !(rhs == *this);
}

LTLfQuery LTLfQuery::qINIT(short declare_argument, bool isTimed) {
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

LTLfQuery LTLfQuery::qEND(short declare_argument, bool isTimed) {
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

LTLfQuery LTLfQuery::qEXISTS(size_t narg, short declare_argument, bool isTimed) {
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

LTLfQuery LTLfQuery::qABSENCE(size_t narg, short declare_argument, bool isTimed) {
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

LTLfQuery LTLfQuery::qNEXT(const LTLfQuery& arg, bool isTimed) {
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

LTLfQuery LTLfQuery::qNOT(const LTLfQuery& arg, bool isTimed, bool preserve) {
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

LTLfQuery LTLfQuery::qOR(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
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

LTLfQuery LTLfQuery::qIMPLICATION(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
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

LTLfQuery LTLfQuery::qIFTE(const LTLfQuery& lhs, const LTLfQuery& middle, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
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

LTLfQuery LTLfQuery::qAND(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
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

LTLfQuery LTLfQuery::qANDNEXTGLOBALLY(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
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

LTLfQuery LTLfQuery::qANDFUTURE(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
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

LTLfQuery LTLfQuery::qUNTIL(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
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

LTLfQuery LTLfQuery::qBOX(const LTLfQuery& lhs, bool isTimed) {
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

LTLfQuery LTLfQuery::qDIAMOND(const LTLfQuery& lhs, bool isTimed) {
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