
// Generated from ../src/yaucl/bpm/structures/declare/DAD.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "DADParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by DADParser.
 */
class  DADListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterData_aware_declare(DADParser::Data_aware_declareContext *ctx) = 0;
  virtual void exitData_aware_declare(DADParser::Data_aware_declareContext *ctx) = 0;

  virtual void enterNary_prop(DADParser::Nary_propContext *ctx) = 0;
  virtual void exitNary_prop(DADParser::Nary_propContext *ctx) = 0;

  virtual void enterUnary_prop(DADParser::Unary_propContext *ctx) = 0;
  virtual void exitUnary_prop(DADParser::Unary_propContext *ctx) = 0;

  virtual void enterFields(DADParser::FieldsContext *ctx) = 0;
  virtual void exitFields(DADParser::FieldsContext *ctx) = 0;

  virtual void enterDisj(DADParser::DisjContext *ctx) = 0;
  virtual void exitDisj(DADParser::DisjContext *ctx) = 0;

  virtual void enterConj_or_atom(DADParser::Conj_or_atomContext *ctx) = 0;
  virtual void exitConj_or_atom(DADParser::Conj_or_atomContext *ctx) = 0;

  virtual void enterTop(DADParser::TopContext *ctx) = 0;
  virtual void exitTop(DADParser::TopContext *ctx) = 0;

  virtual void enterIn_atom(DADParser::In_atomContext *ctx) = 0;
  virtual void exitIn_atom(DADParser::In_atomContext *ctx) = 0;

  virtual void enterAtom_conj(DADParser::Atom_conjContext *ctx) = 0;
  virtual void exitAtom_conj(DADParser::Atom_conjContext *ctx) = 0;

  virtual void enterAtom(DADParser::AtomContext *ctx) = 0;
  virtual void exitAtom(DADParser::AtomContext *ctx) = 0;

  virtual void enterLt(DADParser::LtContext *ctx) = 0;
  virtual void exitLt(DADParser::LtContext *ctx) = 0;

  virtual void enterLeq(DADParser::LeqContext *ctx) = 0;
  virtual void exitLeq(DADParser::LeqContext *ctx) = 0;

  virtual void enterGt(DADParser::GtContext *ctx) = 0;
  virtual void exitGt(DADParser::GtContext *ctx) = 0;

  virtual void enterGeq(DADParser::GeqContext *ctx) = 0;
  virtual void exitGeq(DADParser::GeqContext *ctx) = 0;

  virtual void enterEq(DADParser::EqContext *ctx) = 0;
  virtual void exitEq(DADParser::EqContext *ctx) = 0;

  virtual void enterNeq(DADParser::NeqContext *ctx) = 0;
  virtual void exitNeq(DADParser::NeqContext *ctx) = 0;

  virtual void enterVar(DADParser::VarContext *ctx) = 0;
  virtual void exitVar(DADParser::VarContext *ctx) = 0;


};

