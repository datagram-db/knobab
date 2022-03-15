//
// Created by giacomo on 15/03/2022.
//

#ifndef KNOBAB_DECLAREQUERYLANGUAGEPARSER_H
#define KNOBAB_DECLAREQUERYLANGUAGEPARSER_H

#include <istream>
#include <knobab/queries/LTLfQuery.h>
#include <knobab/queries/LTLfQueryParser.h>
#include <knobab/queries/LTLfQueryVisitor.h>
#include <stack>

inline unsigned char decleare_templates_determine(LTLfQueryParser::Declare_argumentsContext* ptr) {
    if (!ptr) return DECLARE_TYPE_NONE;
    else {
        if (ptr->LEFT()) return DECLARE_TYPE_LEFT;
        else if (ptr->MIDDLE()) return DECLARE_TYPE_MIDDLE;
        else if (ptr->RIGHT()) return DECLARE_TYPE_RIGHT;
        else return DECLARE_TYPE_NONE;
    }
}

#include <unordered_map>
#include <yaucl/functional/assert.h>

#define GET_TIMING(context)     (((context) && ((context)->TIMED() != nullptr)) || (isAutoTimed && fromNowOnTimed))
#define DEFAULT_TIMING()     ((isAutoTimed && fromNowOnTimed))
#define ASSERT_ON_TIMING(context)       DEBUG_ASSERT((!isAutoTimed) || (GET_TIMING(context) == fromNowOnTimed))
#define ASSERT_ON_GIVEN_TIMING(b)       DEBUG_ASSERT(((b) == fromNowOnTimed))

struct DeclareQueryLanguageParser : public LTLfQueryVisitor {

    bool fromNowOnTimed = false;
    bool isAutoTimed = false;
    std::stack<bool> fromNowOnTimedStack;
    unsigned char max_aspect;
    std::unordered_map<std::string, std::unordered_map<std::string, LTLfQuery>> planname_to_declare_to_ltlf;

    void parse(std::istream& stream);

    LTLfQuery visitQuery(LTLfQueryParser::QueryContext *pContext) {
        if (!pContext) return LTLfQuery{}; // if the context is empty, return a query returning empty (the falsehood statement)
        fromNowOnTimed = false;
        max_aspect = DECLARE_TYPE_NONE;
        pContext->accept(this);
        fromNowOnTimed = false;
    }

    antlrcpp::Any visitInit(LTLfQueryParser::InitContext *context) override {
        ASSERT_ON_TIMING(context);
        auto argument = decleare_templates_determine(context->declare_arguments());
        max_aspect = std::max(max_aspect, argument);
        return {LTLfQuery::qINIT(argument, GET_TIMING(context))};
    }

    antlrcpp::Any visitEnd(LTLfQueryParser::EndContext *context) override {
        ASSERT_ON_TIMING(context);
        auto argument = decleare_templates_determine(context->declare_arguments());
        max_aspect = std::max(max_aspect, argument);
        return {LTLfQuery::qEND(argument, GET_TIMING(context))};
    }

    antlrcpp::Any visitAbsence(LTLfQueryParser::AbsenceContext *context) override {
        ASSERT_ON_TIMING(context);
        auto argument = decleare_templates_determine(context->declare_arguments());
        max_aspect = std::max(max_aspect, argument);
        return {LTLfQuery::qABSENCE(std::stoull(context->INTNUMBER()->getText()), argument, GET_TIMING(context))};
    }

    antlrcpp::Any visitExists(LTLfQueryParser::ExistsContext *context) override {
        ASSERT_ON_TIMING(context);
        auto argument = decleare_templates_determine(context->declare_arguments());
        max_aspect = std::max(max_aspect, argument);
        return {LTLfQuery::qEXISTS(std::stoull(context->INTNUMBER()->getText()), argument, GET_TIMING(context))};
    }

    antlrcpp::Any visitNext(LTLfQueryParser::NextContext *context) override {
        fromNowOnTimedStack.push(fromNowOnTimed);
        auto q = LTLfQuery::qNEXT(visit(context->query()).as<LTLfQuery>(), DEFAULT_TIMING());
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {q};
    }

    antlrcpp::Any visitOr(LTLfQueryParser::OrContext *context) override {
        ASSERT_ON_TIMING(context);
        fromNowOnTimedStack.push(fromNowOnTimed);
        auto q = LTLfQuery::qOR(visit(context->query(0)).as<LTLfQuery>(),
                                 visit(context->query(1)).as<LTLfQuery>(),
                                GET_TIMING(context),
                                         context->THETA() != nullptr);
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {q};
    }

    antlrcpp::Any visitAnd(LTLfQueryParser::AndContext *context) override {
        ASSERT_ON_TIMING(context);
        fromNowOnTimedStack.push(fromNowOnTimed);
        auto q = LTLfQuery::qAND(visit(context->query(0)).as<LTLfQuery>(),
                                 visit(context->query(1)).as<LTLfQuery>(),
                                 GET_TIMING(context),
                                 context->THETA() != nullptr);
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {q};
    }

    antlrcpp::Any visitBox(LTLfQueryParser::BoxContext *context) override {
        fromNowOnTimedStack.push(fromNowOnTimed);
        fromNowOnTimed = true;
        auto q = LTLfQuery::qBOX(visit(context->query()).as<LTLfQuery>(), GET_TIMING(context));
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {q};
    }

    antlrcpp::Any visitDiamond(LTLfQueryParser::DiamondContext *context) override {
        fromNowOnTimedStack.push(fromNowOnTimed);
        fromNowOnTimed = true;
        auto q = LTLfQuery::qDIAMOND(visit(context->query()).as<LTLfQuery>(), DEFAULT_TIMING());
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {q};
    }

    antlrcpp::Any visitNot(LTLfQueryParser::NotContext *context) override {
        fromNowOnTimedStack.push(fromNowOnTimed);
        auto q = LTLfQuery::qNOT(
                                 visit(context->query()).as<LTLfQuery>(),
                                 GET_TIMING(context),
                                 context->PRESERVE() != nullptr
                                 );
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {q};
    }

    antlrcpp::Any visitParen(LTLfQueryParser::ParenContext *context) override {
        return visit(context->query());
    }

    antlrcpp::Any visitAnd_next_globally(LTLfQueryParser::And_next_globallyContext *context) override {
        ASSERT_ON_GIVEN_TIMING(true);
        fromNowOnTimedStack.push(fromNowOnTimed);
        auto lhs = visit(context->query(0)).as<LTLfQuery>();
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        fromNowOnTimedStack.push(fromNowOnTimed);
        fromNowOnTimed = true;
        auto rhs = visit(context->query(1)).as<LTLfQuery>();
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {LTLfQuery::qANDNEXTGLOBALLY(lhs, rhs, true, context->THETA() != nullptr)};
    }

    antlrcpp::Any visitAnd_future(LTLfQueryParser::And_futureContext *context) override {
        ASSERT_ON_GIVEN_TIMING(true);
        fromNowOnTimedStack.push(fromNowOnTimed);
        auto lhs = visit(context->query(0)).as<LTLfQuery>();
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        fromNowOnTimedStack.push(fromNowOnTimed);
        fromNowOnTimed = true;
        auto rhs = visit(context->query(1)).as<LTLfQuery>();
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {LTLfQuery::qANDFUTURE(lhs, rhs, true, context->THETA() != nullptr)};
    }

    antlrcpp::Any visitImplication(LTLfQueryParser::ImplicationContext *context) override {
        ASSERT_ON_TIMING(context);
        fromNowOnTimedStack.push(fromNowOnTimed);
        auto q = LTLfQuery::qIMPLICATION(visit(context->query(0)).as<LTLfQuery>(),
                                visit(context->query(1)).as<LTLfQuery>(),
                                GET_TIMING(context),
                                context->THETA() != nullptr);
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {q};
    }

    antlrcpp::Any visitUntil(LTLfQueryParser::UntilContext *context) override {
        fromNowOnTimedStack.push(fromNowOnTimed);
        fromNowOnTimed = true;
        auto q = LTLfQuery::qUNTIL(visit(context->query(0)).as<LTLfQuery>(),
                                   visit(context->query(1)).as<LTLfQuery>(),
                                   GET_TIMING(context),
                                   context->THETA() != nullptr);
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {q};
    }

    antlrcpp::Any visitIfte(LTLfQueryParser::IfteContext *context) override {
        ASSERT_ON_TIMING(context);
        fromNowOnTimedStack.push(fromNowOnTimed);
        auto q = LTLfQuery::qIFTE(visit(context->query(0)).as<LTLfQuery>(),
                                         visit(context->query(1)).as<LTLfQuery>(),
                                        visit(context->query(2)).as<LTLfQuery>(),
                                         GET_TIMING(context),
                                         context->THETA() != nullptr);
        fromNowOnTimed = fromNowOnTimedStack.top();
        fromNowOnTimedStack.pop();
        return {q};
    }

    antlrcpp::Any visitQuery_plan(LTLfQueryParser::Query_planContext *context) override {
        return antlrcpp::Any();
    }
    antlrcpp::Any visitDeclare_syntax(LTLfQueryParser::Declare_syntaxContext *context) override {
        return antlrcpp::Any();
    }
    antlrcpp::Any visitHas_args(LTLfQueryParser::Has_argsContext *context) override {
        return antlrcpp::Any();
    }
    antlrcpp::Any visitDeclare_arguments(LTLfQueryParser::Declare_argumentsContext *context) override {
        return antlrcpp::Any();
    }
    antlrcpp::Any visitQuery_plans(LTLfQueryParser::Query_plansContext *context) override {
        return antlrcpp::Any();
    }


};


#endif //KNOBAB_DECLAREQUERYLANGUAGEPARSER_H
