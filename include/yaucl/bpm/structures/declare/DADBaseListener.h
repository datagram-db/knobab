
// Generated from ./src/yaucl/bpm/structures/declare/DAD.g by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include <yaucl/bpm/structures/declare/DADListener.h>


/**
 * This class provides an empty implementation of DADListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  DADBaseListener : public DADListener {
public:

  virtual void enterData_aware_declare(DADParser::Data_aware_declareContext * /*ctx*/) override { }
  virtual void exitData_aware_declare(DADParser::Data_aware_declareContext * /*ctx*/) override { }

  virtual void enterNary_prop(DADParser::Nary_propContext * /*ctx*/) override { }
  virtual void exitNary_prop(DADParser::Nary_propContext * /*ctx*/) override { }

  virtual void enterUnary_prop(DADParser::Unary_propContext * /*ctx*/) override { }
  virtual void exitUnary_prop(DADParser::Unary_propContext * /*ctx*/) override { }

  virtual void enterFields(DADParser::FieldsContext * /*ctx*/) override { }
  virtual void exitFields(DADParser::FieldsContext * /*ctx*/) override { }

  virtual void enterDisj(DADParser::DisjContext * /*ctx*/) override { }
  virtual void exitDisj(DADParser::DisjContext * /*ctx*/) override { }

  virtual void enterConj_or_atom(DADParser::Conj_or_atomContext * /*ctx*/) override { }
  virtual void exitConj_or_atom(DADParser::Conj_or_atomContext * /*ctx*/) override { }

  virtual void enterTop(DADParser::TopContext * /*ctx*/) override { }
  virtual void exitTop(DADParser::TopContext * /*ctx*/) override { }

  virtual void enterIn_atom(DADParser::In_atomContext * /*ctx*/) override { }
  virtual void exitIn_atom(DADParser::In_atomContext * /*ctx*/) override { }

  virtual void enterAtom_conj(DADParser::Atom_conjContext * /*ctx*/) override { }
  virtual void exitAtom_conj(DADParser::Atom_conjContext * /*ctx*/) override { }

  virtual void enterAtom(DADParser::AtomContext * /*ctx*/) override { }
  virtual void exitAtom(DADParser::AtomContext * /*ctx*/) override { }

  virtual void enterLt(DADParser::LtContext * /*ctx*/) override { }
  virtual void exitLt(DADParser::LtContext * /*ctx*/) override { }

  virtual void enterLeq(DADParser::LeqContext * /*ctx*/) override { }
  virtual void exitLeq(DADParser::LeqContext * /*ctx*/) override { }

  virtual void enterGt(DADParser::GtContext * /*ctx*/) override { }
  virtual void exitGt(DADParser::GtContext * /*ctx*/) override { }

  virtual void enterGeq(DADParser::GeqContext * /*ctx*/) override { }
  virtual void exitGeq(DADParser::GeqContext * /*ctx*/) override { }

  virtual void enterEq(DADParser::EqContext * /*ctx*/) override { }
  virtual void exitEq(DADParser::EqContext * /*ctx*/) override { }

  virtual void enterNeq(DADParser::NeqContext * /*ctx*/) override { }
  virtual void exitNeq(DADParser::NeqContext * /*ctx*/) override { }

  virtual void enterVar(DADParser::VarContext * /*ctx*/) override { }
  virtual void exitVar(DADParser::VarContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

