
// Generated from ../src/knobab/queries/LTLfQuery.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "LTLfQueryParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by LTLfQueryParser.
 */
class  LTLfQueryVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by LTLfQueryParser.
   */
    virtual antlrcpp::Any visitQuery_plans(LTLfQueryParser::Query_plansContext *context) = 0;

    virtual antlrcpp::Any visitQuery_plan(LTLfQueryParser::Query_planContext *context) = 0;

    virtual antlrcpp::Any visitDeclare_syntax(LTLfQueryParser::Declare_syntaxContext *context) = 0;

    virtual antlrcpp::Any visitHas_args(LTLfQueryParser::Has_argsContext *context) = 0;

    virtual antlrcpp::Any visitNext(LTLfQueryParser::NextContext *context) = 0;

    virtual antlrcpp::Any visitInit(LTLfQueryParser::InitContext *context) = 0;

    virtual antlrcpp::Any visitOr(LTLfQueryParser::OrContext *context) = 0;

    virtual antlrcpp::Any visitAbsence(LTLfQueryParser::AbsenceContext *context) = 0;

    virtual antlrcpp::Any visitBox(LTLfQueryParser::BoxContext *context) = 0;

    virtual antlrcpp::Any visitDiamond(LTLfQueryParser::DiamondContext *context) = 0;

    virtual antlrcpp::Any visitNot(LTLfQueryParser::NotContext *context) = 0;

    virtual antlrcpp::Any visitParen(LTLfQueryParser::ParenContext *context) = 0;

    virtual antlrcpp::Any visitAnd_next_globally(LTLfQueryParser::And_next_globallyContext *context) = 0;

    virtual antlrcpp::Any visitAnd(LTLfQueryParser::AndContext *context) = 0;

    virtual antlrcpp::Any visitImplication(LTLfQueryParser::ImplicationContext *context) = 0;

    virtual antlrcpp::Any visitExists(LTLfQueryParser::ExistsContext *context) = 0;

    virtual antlrcpp::Any visitEnd(LTLfQueryParser::EndContext *context) = 0;

    virtual antlrcpp::Any visitUntil(LTLfQueryParser::UntilContext *context) = 0;

    virtual antlrcpp::Any visitAnd_future(LTLfQueryParser::And_futureContext *context) = 0;

    virtual antlrcpp::Any visitIfte(LTLfQueryParser::IfteContext *context) = 0;

    virtual antlrcpp::Any visitDeclare_arguments(LTLfQueryParser::Declare_argumentsContext *context) = 0;


};

