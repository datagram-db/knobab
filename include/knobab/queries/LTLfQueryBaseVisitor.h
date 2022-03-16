
// Generated from ../src/knobab/queries/LTLfQuery.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "knobab/queries/LTLfQueryVisitor.h"


/**
 * This class provides an empty implementation of LTLfQueryVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  LTLfQueryBaseVisitor : public LTLfQueryVisitor {
public:

  virtual antlrcpp::Any visitQuery_plans(LTLfQueryParser::Query_plansContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitQuery_plan(LTLfQueryParser::Query_planContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeclare_syntax(LTLfQueryParser::Declare_syntaxContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitHas_args(LTLfQueryParser::Has_argsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNext(LTLfQueryParser::NextContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInit(LTLfQueryParser::InitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOr(LTLfQueryParser::OrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAbsence(LTLfQueryParser::AbsenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBox(LTLfQueryParser::BoxContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDiamond(LTLfQueryParser::DiamondContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNot(LTLfQueryParser::NotContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParen(LTLfQueryParser::ParenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAnd_next_globally(LTLfQueryParser::And_next_globallyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAnd(LTLfQueryParser::AndContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitImplication(LTLfQueryParser::ImplicationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExists(LTLfQueryParser::ExistsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEnd(LTLfQueryParser::EndContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUntil(LTLfQueryParser::UntilContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAnd_future(LTLfQueryParser::And_futureContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfte(LTLfQueryParser::IfteContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeclare_arguments(LTLfQueryParser::Declare_argumentsContext *ctx) override {
    return visitChildren(ctx);
  }


};

