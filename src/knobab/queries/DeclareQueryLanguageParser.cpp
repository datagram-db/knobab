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
            if (!ref.empty()) {
                throw std::runtime_error(std::string("ERROR: the query plan '" + query_plan_name+"' was already declared before with a non-empty number of clauses!"));
            }
            isAutoTimed = query_plan->AUTO_TIMED();
            for (const auto& declare : query_plan->declare_syntax()) {
                auto Q = visitQuery(declare->query());
                std::string declare_name = UNESCAPE(declare->STRING()->getText());
                if (declare->has_args()) {
                    if (max_aspect != std::stoull(declare->has_args()->INTNUMBER()->getText())) {
                        throw std::runtime_error(std::string("ERROR: declare '").append(declare_name).append("' from query plan '").append(query_plan_name).append("' was associated to ").append(std::to_string(max_aspect)).append(" arguments, but ").append(declare->has_args()->INTNUMBER()->getText()).append(" were expected."));
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
    LTLfQuery q = pContext->accept(this).as<LTLfQuery>();
    fromNowOnTimed = false;
    return q;
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
    return {LTLfQuery::qEXISTS(std::stoull(context->INTNUMBER()->getText()), argument, GET_TIMING(context), context->NEGATED())};
}

antlrcpp::Any DeclareQueryLanguageParser::visitNext(LTLfQueryParser::NextContext *context) {
    fromNowOnTimedStack.push(fromNowOnTimed);
    auto arg = visit(context->query()).as<LTLfQuery>();
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();
    return {LTLfQuery::qNEXT(arg, DEFAULT_TIMING())};
}

antlrcpp::Any DeclareQueryLanguageParser::visitOr(LTLfQueryParser::OrContext *context) {
    ASSERT_ON_TIMING(context);
    fromNowOnTimedStack.push(fromNowOnTimed);
    auto lhs = visit(context->query(0)).as<LTLfQuery>(), rhs = visit(context->query(1)).as<LTLfQuery>();
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();
    return {LTLfQuery::qOR(lhs, rhs,
                           GET_TIMING(context),
                           context->THETA() != nullptr)};
}

antlrcpp::Any DeclareQueryLanguageParser::visitAnd(LTLfQueryParser::AndContext *context) {
    ASSERT_ON_TIMING(context);
    fromNowOnTimedStack.push(fromNowOnTimed);
    auto lhs = visit(context->query(0)).as<LTLfQuery>(), rhs = visit(context->query(1)).as<LTLfQuery>();
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();
    return {LTLfQuery::qAND(lhs,
                             rhs,
                             GET_TIMING(context),
                             context->THETA() != nullptr)};
}

antlrcpp::Any DeclareQueryLanguageParser::visitBox(LTLfQueryParser::BoxContext *context) {

    fromNowOnTimedStack.push(fromNowOnTimed);
    fromNowOnTimed = true;
    auto arg = visit(context->query()).as<LTLfQuery>();
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();

    return {LTLfQuery::qBOX(arg, GET_TIMING(context))};
}

antlrcpp::Any DeclareQueryLanguageParser::visitDiamond(LTLfQueryParser::DiamondContext *context) {

    fromNowOnTimedStack.push(fromNowOnTimed);
    fromNowOnTimed = true;
    auto arg = visit(context->query()).as<LTLfQuery>();
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();

    return {LTLfQuery::qDIAMOND(arg, GET_TIMING(context))};
}

antlrcpp::Any DeclareQueryLanguageParser::visitNot(LTLfQueryParser::NotContext *context) {

    fromNowOnTimedStack.push(fromNowOnTimed);
    auto arg = visit(context->query()).as<LTLfQuery>();
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();

    return { LTLfQuery::qNOT(arg,
            GET_TIMING(context),
            context->PRESERVE() != nullptr
    )};
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
    auto lhs = visit(context->query(0)).as<LTLfQuery>();
    auto rhs = visit(context->query(1)).as<LTLfQuery>();
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();
    return {LTLfQuery::qIMPLICATION(lhs,
                                    rhs,
                                    GET_TIMING(context),
                                    context->THETA() != nullptr)};
}

antlrcpp::Any DeclareQueryLanguageParser::visitUntil(LTLfQueryParser::UntilContext *context) {
    fromNowOnTimedStack.push(fromNowOnTimed);
    fromNowOnTimed = true;
    auto lhs = visit(context->query(0)).as<LTLfQuery>();
    auto rhs = visit(context->query(1)).as<LTLfQuery>();
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();
    return {LTLfQuery::qUNTIL(lhs,
                               rhs,
                               GET_TIMING(context),
                               context->THETA() != nullptr)};
}

antlrcpp::Any DeclareQueryLanguageParser::visitIfte(LTLfQueryParser::IfteContext *context) {
    ASSERT_ON_TIMING(context);
    fromNowOnTimedStack.push(fromNowOnTimed);
    auto lhs = visit(context->query(0)).as<LTLfQuery>();
    auto mhs = visit(context->query(1)).as<LTLfQuery>();
    auto rhs = visit(context->query(2)).as<LTLfQuery>();
    fromNowOnTimed = fromNowOnTimedStack.top();
    fromNowOnTimedStack.pop();
    return {LTLfQuery::qIFTE(lhs,
                             mhs,
                             rhs,
                             GET_TIMING(context),
                             context->THETA() != nullptr)};
}
