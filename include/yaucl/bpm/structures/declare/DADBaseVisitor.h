
// Generated from /home/giacomo/projects/knobab/src/yaucl/bpm/structures/declare/DAD.g by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "DADVisitor.h"


/**
 * This class provides an empty implementation of DADVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  DADBaseVisitor : public DADVisitor {
public:

  virtual antlrcpp::Any visitData_aware_declare(DADParser::Data_aware_declareContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNary_prop(DADParser::Nary_propContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_prop(DADParser::Unary_propContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFields(DADParser::FieldsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDisj(DADParser::DisjContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConj_or_atom(DADParser::Conj_or_atomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTop(DADParser::TopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIn_atom(DADParser::In_atomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtom_conj(DADParser::Atom_conjContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtom(DADParser::AtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLt(DADParser::LtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLeq(DADParser::LeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGt(DADParser::GtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGeq(DADParser::GeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEq(DADParser::EqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNeq(DADParser::NeqContext *ctx) override {
    return visitChildren(ctx);
  }


};

