//
// Created by giacomo on 12/03/2022.
//

#include <knobab/queries/LTLfQuery.h>

bool LTLfQuery::operator==(const LTLfQuery &rhs) const {
    return t == rhs.t &&
           declare_type == rhs.declare_type &&
           fields == rhs.fields &&
           n == rhs.n &&
           args_from_script == rhs.args_from_script &&
           args == rhs.args &&
           atom == rhs.atom &&
           joinCondition == rhs.joinCondition;
}

bool LTLfQuery::operator!=(const LTLfQuery &rhs) const {
    return !(rhs == *this);
}

std::ostream& operator<<(std::ostream& os, const LTLfQuery& x) {
    char t{'t'};
    char H{'H'};
    char p{'p'};
    char n{'~'};

    switch (x.t) {
        case LTLfQuery::INIT_QP:
            os << (x.fields.id.parts.is_timed ? t : ' ') << "Init";
            if (!x.atom.empty()) {
                return os << x.atom;
            } else {
                return os << "{" << ((x.declare_type == 1)? 'L' : (x.declare_type==2 ? 'R' : 'M'))<< "}";
            }

        case LTLfQuery::END_QP:
            os << (x.fields.id.parts.is_timed ? t : ' ') << "End";
            if (!x.atom.empty()) {
                return os << x.atom;
            } else {
                return os << "{" << ((x.declare_type == 1)? 'L' : (x.declare_type==2 ? 'R' : 'M'))<< "}";
            }

        case LTLfQuery::EXISTS_QP:
            os << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.is_negated ? n : ' ') << "Exists" << x.n ;
            if (!x.atom.empty()) {
                return os << x.atom;
            } else {
                return os << "{" << ((x.declare_type == 1)? 'L' : (x.declare_type==2 ? 'R' : 'M'))<< "}";
            }

        case LTLfQuery::ABSENCE_QP:
            os << (x.fields.id.parts.is_timed ? t : ' ') << "Absence" << x.n;
            if (!x.atom.empty()) {
                return os << x.atom;
            } else {
                return os << "{" << ((x.declare_type == 1)? 'L' : (x.declare_type==2 ? 'R' : 'M'))<< "}";
            }

        case LTLfQuery::NEXT_QP:
            if (!x.fields.id.parts.is_queryplan)
                return os << "X(" << x.args_from_script.at(0) << ")";
            else
                return os << "X(" << *x.args.at(0) << ")";

        case LTLfQuery::OR_QP:
            if (!x.fields.id.parts.is_queryplan)
                return os << "(" << x.args_from_script.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ') << "|| (" << x.args_from_script.at(1) << ")";
            else
                return os << "(" << *x.args.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ') << "|| (" << *x.args.at(1) << ")";

        case LTLfQuery::AND_QP:
            if (!x.fields.id.parts.is_queryplan)
                return os << "(" << x.args_from_script.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ') << "&& (" << x.args_from_script.at(1) << ")";
            else
                return os << "(" << *x.args.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ') << "&& (" << *x.args.at(1) << ")";

        case LTLfQuery::IMPL_QP:
            if (!x.fields.id.parts.is_queryplan)
                return os << "(" << x.args_from_script.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ')  << "=> (" << x.args_from_script.at(1) << ")";
            else
                return os << "(" << *x.args.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ')  << "=> (" << *x.args.at(1) << ")";

        case LTLfQuery::IFTE_QP:
            if (!x.fields.id.parts.is_queryplan)
                return os << "IF (" << x.args_from_script.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ')  << "THEN (" << x.args_from_script.at(1) << ") ELSE (" << x.args_from_script.at(2) << ")";
            else
                return os << "IF (" << *x.args.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ')  << "THEN (" << *x.args.at(1) << ") ELSE (" << *x.args.at(2) << ")";

        case LTLfQuery::U_QP:
            if (!x.fields.id.parts.is_queryplan)
                return os << "(" << x.args_from_script.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ')  << "U (" << x.args_from_script.at(1) << ")";
            else
                return os << "(" << *x.args.at(0) << ") " << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.has_theta ? H : ' ')  << "U (" << *x.args.at(1) << ")";

        case LTLfQuery::G_QP:
            if (!x.fields.id.parts.is_queryplan)
                return os << (x.fields.id.parts.is_timed ? t : ' ') << "G(" << x.args_from_script.at(0) << ")";
            else
                return os << (x.fields.id.parts.is_timed ? t : ' ') << "G(" << *x.args.at(0) << ")";

        case LTLfQuery::F_QP:
            if (!x.fields.id.parts.is_queryplan)
                return os << (x.fields.id.parts.is_timed ? t : ' ') << "F(" << x.args_from_script.at(0) << ")";
            else
                return os << (x.fields.id.parts.is_timed ? t : ' ') << "F(" << *x.args.at(0) << ")";

        case LTLfQuery::NOT_QP:
            if (!x.fields.id.parts.is_queryplan)
                return os << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.preserve ? p : ' ') << "!(" << x.args_from_script.at(0) << ")";
            else
                return os << (x.fields.id.parts.is_timed ? t : ' ') << (x.fields.id.parts.preserve ? p : ' ') << "!(" << *x.args.at(0) << ")";

        case LTLfQuery::AF_QPT:
            if (!x.fields.id.parts.is_queryplan)
                return os << "(" << x.args_from_script.at(0) << ") " << (x.fields.id.parts.has_theta ? H : ' ')<< " &F (" << x.args_from_script.at(1) << ")";
            else
                return os << "(" << *x.args.at(0) << ") " << (x.fields.id.parts.has_theta ? H : ' ')<< " &F (" << *x.args.at(1) << ")";

        case LTLfQuery::AXG_QPT:
            if (!x.fields.id.parts.is_queryplan)
                return os << "(" << x.args_from_script.at(0) << ") " << (x.fields.id.parts.has_theta ? H : ' ') << " &XG (" << x.args_from_script.at(1) << ")";
            else
                return os << "(" << *x.args.at(0) << ") " << (x.fields.id.parts.has_theta ? H : ' ') << " &XG (" << *x.args.at(1) << ")";

        default:
            return os << "FALSE";
    }
}

LTLfQuery LTLfQuery::qINIT(short declare_argument, bool isTimed) {
    LTLfQuery q;
    q.t = INIT_QP;
    q.n = 0;
    q.declare_type = declare_argument;
    q.fields.id.parts.has_theta = false;
    q.fields.id.parts.preserve = false;
    q.fields.id.parts.is_atom = true;
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.fields.id.parts.is_queryplan = false;
    return q;
}

LTLfQuery LTLfQuery::qEND(short declare_argument, bool isTimed) {
    LTLfQuery q;
    q.t = END_QP;
    q.n = 0;
    q.declare_type = declare_argument;
    q.fields.id.parts.has_theta = false;
    q.fields.id.parts.preserve = false;
    q.fields.id.parts.is_atom = true;
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.fields.id.parts.is_queryplan = false;
    return q;
}

LTLfQuery LTLfQuery::qEXISTS(size_t narg, short declare_argument, bool isTimed, bool isNegated) {
    LTLfQuery q;
    q.t = EXISTS_QP;
    q.n = narg;
    q.declare_type = declare_argument;
    q.fields.id.parts.has_theta = false;
    q.fields.id.parts.preserve = false;
    q.fields.id.parts.is_atom = true;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_negated = isNegated;
    q.fields.id.parts.is_numbered = true;
    q.fields.id.parts.is_queryplan = false;
    return q;
}

LTLfQuery LTLfQuery::qABSENCE(size_t narg, short declare_argument, bool isTimed) {
    LTLfQuery q;
    q.t = ABSENCE_QP;
    q.n = narg;
    q.declare_type = declare_argument;
    q.fields.id.parts.has_theta = false;
    q.fields.id.parts.preserve = false;
    q.fields.id.parts.is_atom = true;
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = true;
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(arg);
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(arg);
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(lhs);
    q.args_from_script.emplace_back(rhs);
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(lhs);
    q.args_from_script.emplace_back(rhs);
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(lhs);
    q.args_from_script.emplace_back(middle);
    q.args_from_script.emplace_back(rhs);
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(lhs);
    q.args_from_script.emplace_back(rhs);
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(lhs);
    q.args_from_script.emplace_back(rhs);
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(lhs);
    q.args_from_script.emplace_back(rhs);
    q.fields.id.parts.is_queryplan = false;
    return q;
}

LTLfQuery LTLfQuery::qUNTIL(const LTLfQuery& lhs, const LTLfQuery& rhs, bool isTimed, bool hasTheta) {
    LTLfQuery q;
    q.t = U_QP;
    q.n = 0;
    q.declare_type = DECLARE_TYPE_NONE;
    q.fields.id.parts.has_theta = hasTheta;
    q.fields.id.parts.preserve = false;
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_atom = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(lhs);
    q.args_from_script.emplace_back(rhs);
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_timed = isTimed;
    q.fields.id.parts.is_numbered = false;
    q.args_from_script.emplace_back(lhs);
    q.fields.id.parts.is_queryplan = false;
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
    q.fields.id.parts.is_negated = false;
    q.fields.id.parts.is_numbered = false;
    q.fields.id.parts.is_timed = isTimed;
    q.args_from_script.emplace_back(lhs);
    q.fields.id.parts.is_queryplan = false;
    return q;
}