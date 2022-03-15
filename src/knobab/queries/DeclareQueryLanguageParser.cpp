//
// Created by giacomo on 15/03/2022.
//

#include "knobab/queries/DeclareQueryLanguageParser.h"
#include <knobab/queries/LTLfQueryParser.h>
#include <knobab/queries/LTLfQueryLexer.h>
#include <yaucl/data/json.h>

void DeclareQueryLanguageParser::parse(std::istream &stream) {
    antlr4::ANTLRInputStream input(stream);
    LTLfQueryLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    LTLfQueryParser parser(&tokens);
    auto ptr = parser.query_plans();
    if (ptr) {
        for (const auto& query_plan : ptr->query_plan()) {
            std::string query_plan_name = UNESCAPE(query_plan->STRING()->getText());
            auto& ref = planname_to_declare_to_ltlf[query_plan_name];
            isAutoTimed = query_plan->AUTO_TIMED();
            for (const auto& declare : query_plan->declare_syntax()) {
                auto Q = visitQuery(declare->query());
                std::string declare_name = UNESCAPE(declare->STRING()->getText());
                if (declare->has_args()) {
                    if (max_aspect != std::stoull(declare->has_args()->INTNUMBER()->getText())) {
                        throw std::runtime_error(std::string("ERROR: declare '")+declare_name+ "' from query plan '"+query_plan_name+"' was associated to "+std::to_string(max_aspect)+" arguments, but "+declare->has_args()->INTNUMBER()->getText()+" were expected.");
                    }
                }
                ref.emplace(UNESCAPE(declare->STRING()->getText()),
                            visitQuery(declare->query()));
            }
        }
    }
}

LTLfQuery DeclareQueryLanguageParser::visitQuery(LTLfQueryParser::QueryContext *pContext) {
    if (!pContext) return LTLfQuery{}; // if the context is empty, return a query returning empty (the falsehood statement)
    fromNowOnTimed = false;
    max_aspect = DECLARE_TYPE_NONE;
    pContext->accept(this);
    fromNowOnTimed = false;
}

antlrcpp::Any DeclareQueryLanguageParser::visitInit(LTLfQueryParser::InitContext *context) {
    ASSERT_ON_TIMING(context);
    auto argument = decleare_templates_determine(context->declare_arguments());
    max_aspect = std::max(max_aspect, argument);
    return {LTLfQuery::qINIT(argument, GET_TIMING(context))};
}

antlrcpp::Any DeclareQueryLanguageParser::visitEnd(LTLfQueryParser::EndContext *context) {
    ASSERT_ON_TIMING(context);
    auto argument = decleare_templates_determine(context->declare_arguments());
    max_aspect = std::max(max_aspect, argument);
    return {LTLfQuery::qEND(argument, GET_TIMING(context))};
}

antlrcpp::Any DeclareQueryLanguageParser::visitAbsence(LTLfQueryParser::AbsenceContext *context) {
    ASSERT_ON_TIMING(context);
    auto argument = decleare_templates_determine(context->declare_arguments());
    max_aspect = std::max(max_aspect, argument);
    return {LTLfQuery::qABSENCE(std::stoull(context->INTNUMBER()->getText()), argument, GET_TIMING(context))};
}

antlrcpp::Any DeclareQueryLanguageParser::visitExists(LTLfQueryParser::ExistsContext *context) {
    ASSERT_ON_TIMING(context);
    auto argument = decleare_templates_determine(context->declare_arguments());
    max_aspect = std::max(max_aspect, argument);
    return {LTLfQuery::qEXISTS(std::stoull(context->INTNUMBER()->getText()), argument, GET_TIMING(context))};
}

antlrcpp::Any DeclareQueryLanguageParser::visitNext(LTLfQueryParser::NextContext *context) {
    fromNowOnTimedStack.push(fromNowOnTimed);
    auto q = LTLfQuery::qNEXT(visit(context->query()).as<LTLfQuery>(), DEFAULT_TIMING());
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();
    return {q};
}

antlrcpp::Any DeclareQueryLanguageParser::visitOr(LTLfQueryParser::OrContext *context) {
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

antlrcpp::Any DeclareQueryLanguageParser::visitAnd(LTLfQueryParser::AndContext *context) {
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

antlrcpp::Any DeclareQueryLanguageParser::visitBox(LTLfQueryParser::BoxContext *context) {
    fromNowOnTimedStack.push(fromNowOnTimed);
    fromNowOnTimed = true;
    auto q = LTLfQuery::qBOX(visit(context->query()).as<LTLfQuery>(), GET_TIMING(context));
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();
    return {q};
}

antlrcpp::Any DeclareQueryLanguageParser::visitDiamond(LTLfQueryParser::DiamondContext *context) {
    fromNowOnTimedStack.push(fromNowOnTimed);
    fromNowOnTimed = true;
    auto q = LTLfQuery::qDIAMOND(visit(context->query()).as<LTLfQuery>(), DEFAULT_TIMING());
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();
    return {q};
}

antlrcpp::Any DeclareQueryLanguageParser::visitNot(LTLfQueryParser::NotContext *context) {
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

antlrcpp::Any DeclareQueryLanguageParser::visitAnd_next_globally(LTLfQueryParser::And_next_globallyContext *context) {
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

antlrcpp::Any DeclareQueryLanguageParser::visitAnd_future(LTLfQueryParser::And_futureContext *context) {
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

antlrcpp::Any DeclareQueryLanguageParser::visitParen(LTLfQueryParser::ParenContext *context) {
    return visit(context->query());
}

antlrcpp::Any DeclareQueryLanguageParser::visitImplication(LTLfQueryParser::ImplicationContext *context) {
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

antlrcpp::Any DeclareQueryLanguageParser::visitUntil(LTLfQueryParser::UntilContext *context) {
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

antlrcpp::Any DeclareQueryLanguageParser::visitIfte(LTLfQueryParser::IfteContext *context) {
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
