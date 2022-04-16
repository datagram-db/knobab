//
// Created by giacomo on 15/04/2022.
//

#ifndef KNOBAB_SERVER_SERVERQUERYMANAGER_H
#define KNOBAB_SERVER_SERVERQUERYMANAGER_H

#include <knobab/server/query_manager/KnoBABQueryVisitor.h>
#include <unordered_map>
#include <string>
#include <knobab/server/operators/LTLfQuery.h>

#include "Environment.h"
#include "KnoBABQueryBaseVisitor.h"

inline unsigned char decleare_templates_determine(KnoBABQueryParser::Declare_argumentsContext* ptr) {
    if (!ptr) return DECLARE_TYPE_NONE;
    else return std::stoull(ptr->INTNUMBER()->getText());
}

inline LeafType decleare_leaf_determine(KnoBABQueryParser::Declare_act_targetContext* ptr) {
    if (!ptr) return NoneLeaf;
    else if (ptr->ACTIVATION()) return ActivationLeaf;
    else if (ptr->TARGET()) return TargetLeaf;
    else return NoneLeaf;
}

#define GET_TIMING(context)     (((context) && ((context)->TIMED() != nullptr)) || (isAutoTimed && fromNowOnTimed))
#define DEFAULT_TIMING()     ((isAutoTimed && fromNowOnTimed))
#define ASSERT_ON_TIMING(context)       DEBUG_ASSERT((!isAutoTimed) || (GET_TIMING(context) == fromNowOnTimed))
#define ASSERT_ON_GIVEN_TIMING(b)       DEBUG_ASSERT(((b) == fromNowOnTimed))

class ServerQueryManager : public KnoBABQueryBaseVisitor {
    bool asConjunctiveModel;
public:
    double parsing_time_ms = -1.0;

    /// Plan Visitor
    bool fromNowOnTimed = false;
    bool isAutoTimed = false;
    std::stack<bool> fromNowOnTimedStack;
    unsigned char max_aspect;
    std::unordered_map<std::string, std::unordered_map<std::string, LTLfQuery>> planname_to_declare_to_ltlf;
    void analyse(const std::string &approach, const DeclareDataAware &clause);
    LTLfQuery visitQuery(KnoBABQueryParser::LtlfContext *pContext);
    std::any visitLast(KnoBABQueryParser::LastContext *context) override;
    std::any visitFirst(KnoBABQueryParser::FirstContext *context) override;
    std::any visitInit(KnoBABQueryParser::InitContext *context) override;
    std::any visitEnd(KnoBABQueryParser::EndContext *context) override;
    std::any visitAbsence(KnoBABQueryParser::AbsenceContext *context) override;
    std::any visitExists(KnoBABQueryParser::ExistsContext *context) override;
    std::any visitNext(KnoBABQueryParser::NextContext *context) override;
    std::any visitOr(KnoBABQueryParser::OrContext *context) override;
    std::any visitAnd(KnoBABQueryParser::AndContext *context) override;
    std::any visitBox(KnoBABQueryParser::BoxContext *context) override;
    std::any visitDiamond(KnoBABQueryParser::DiamondContext *context) override;
    std::any visitNot(KnoBABQueryParser::NotContext *context) override;
    std::any visitAnd_next_globally(KnoBABQueryParser::And_next_globallyContext *context) override;
    std::any visitAnd_future(KnoBABQueryParser::And_futureContext *context) override;
    std::any visitParen(KnoBABQueryParser::ParenContext *context) override;
    std::any visitImplication(KnoBABQueryParser::ImplicationContext *context) override;
    std::any visitUntil(KnoBABQueryParser::UntilContext *context) override;
    std::any visitIfte(KnoBABQueryParser::IfteContext *context) override;
    std::any visitAnd_globally(KnoBABQueryParser::And_globallyContext *context) override;
    std::any visitQuery_plan(KnoBABQueryParser::Query_planContext *context) override;

    trace_visitor* tv = nullptr;
    std::unordered_map<std::string, Environment> multiple_logs;
    std::any visitLoad_data_query(KnoBABQueryParser::Load_data_queryContext *context) override;

    /// DECLARE DATA AWARE PARSING
    std::any visitData_aware_declare(KnoBABQueryParser::Data_aware_declareContext *context) override;
    std::any visitNary_prop(KnoBABQueryParser::Nary_propContext *context) override;
    std::any visitUnary_prop(KnoBABQueryParser::Unary_propContext *context) override;
    std::any visitFields(KnoBABQueryParser::FieldsContext *context) override;
    std::any visitDisj(KnoBABQueryParser::DisjContext *context) override;
    std::any visitConj_or_atom(KnoBABQueryParser::Conj_or_atomContext *context) override;
    std::any visitTop(KnoBABQueryParser::TopContext *context) override;
    std::any visitIn_atom(KnoBABQueryParser::In_atomContext *context) override;
    std::any visitAtom_conj(KnoBABQueryParser::Atom_conjContext *context) override;
    std::any visitAtom(KnoBABQueryParser::AtomContext *context) override;
    std::any visitLt(KnoBABQueryParser::LtContext *ctx) override;
    std::any visitLeq(KnoBABQueryParser::LeqContext *ctx) override;
    std::any visitGt(KnoBABQueryParser::GtContext *ctx) override;
    std::any visitGeq(KnoBABQueryParser::GeqContext *ctx) override;
    std::any visitEq(KnoBABQueryParser::EqContext *ctx) override;
    std::any visitNeq(KnoBABQueryParser::NeqContext *ctx) override;

    /// HRF Parser
    std::any visitField(KnoBABQueryParser::FieldContext *context) override;

};


#endif //KNOBAB_SERVER_SERVERQUERYMANAGER_H