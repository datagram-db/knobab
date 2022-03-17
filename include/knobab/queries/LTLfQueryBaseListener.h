
// Generated from ../src/knobab/queries/LTLfQuery.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "LTLfQueryListener.h"


/**
 * This class provides an empty implementation of LTLfQueryListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  LTLfQueryBaseListener : public LTLfQueryListener {
public:

  virtual void enterQuery_plans(LTLfQueryParser::Query_plansContext * /*ctx*/) override { }
  virtual void exitQuery_plans(LTLfQueryParser::Query_plansContext * /*ctx*/) override { }

  virtual void enterQuery_plan(LTLfQueryParser::Query_planContext * /*ctx*/) override { }
  virtual void exitQuery_plan(LTLfQueryParser::Query_planContext * /*ctx*/) override { }

  virtual void enterDeclare_syntax(LTLfQueryParser::Declare_syntaxContext * /*ctx*/) override { }
  virtual void exitDeclare_syntax(LTLfQueryParser::Declare_syntaxContext * /*ctx*/) override { }

  virtual void enterHas_args(LTLfQueryParser::Has_argsContext * /*ctx*/) override { }
  virtual void exitHas_args(LTLfQueryParser::Has_argsContext * /*ctx*/) override { }

  virtual void enterNext(LTLfQueryParser::NextContext * /*ctx*/) override { }
  virtual void exitNext(LTLfQueryParser::NextContext * /*ctx*/) override { }

  virtual void enterInit(LTLfQueryParser::InitContext * /*ctx*/) override { }
  virtual void exitInit(LTLfQueryParser::InitContext * /*ctx*/) override { }

  virtual void enterOr(LTLfQueryParser::OrContext * /*ctx*/) override { }
  virtual void exitOr(LTLfQueryParser::OrContext * /*ctx*/) override { }

  virtual void enterAbsence(LTLfQueryParser::AbsenceContext * /*ctx*/) override { }
  virtual void exitAbsence(LTLfQueryParser::AbsenceContext * /*ctx*/) override { }

  virtual void enterBox(LTLfQueryParser::BoxContext * /*ctx*/) override { }
  virtual void exitBox(LTLfQueryParser::BoxContext * /*ctx*/) override { }

  virtual void enterDiamond(LTLfQueryParser::DiamondContext * /*ctx*/) override { }
  virtual void exitDiamond(LTLfQueryParser::DiamondContext * /*ctx*/) override { }

  virtual void enterNot(LTLfQueryParser::NotContext * /*ctx*/) override { }
  virtual void exitNot(LTLfQueryParser::NotContext * /*ctx*/) override { }

  virtual void enterParen(LTLfQueryParser::ParenContext * /*ctx*/) override { }
  virtual void exitParen(LTLfQueryParser::ParenContext * /*ctx*/) override { }

  virtual void enterAnd_next_globally(LTLfQueryParser::And_next_globallyContext * /*ctx*/) override { }
  virtual void exitAnd_next_globally(LTLfQueryParser::And_next_globallyContext * /*ctx*/) override { }

  virtual void enterAnd(LTLfQueryParser::AndContext * /*ctx*/) override { }
  virtual void exitAnd(LTLfQueryParser::AndContext * /*ctx*/) override { }

  virtual void enterImplication(LTLfQueryParser::ImplicationContext * /*ctx*/) override { }
  virtual void exitImplication(LTLfQueryParser::ImplicationContext * /*ctx*/) override { }

  virtual void enterExists(LTLfQueryParser::ExistsContext * /*ctx*/) override { }
  virtual void exitExists(LTLfQueryParser::ExistsContext * /*ctx*/) override { }

  virtual void enterEnd(LTLfQueryParser::EndContext * /*ctx*/) override { }
  virtual void exitEnd(LTLfQueryParser::EndContext * /*ctx*/) override { }

  virtual void enterUntil(LTLfQueryParser::UntilContext * /*ctx*/) override { }
  virtual void exitUntil(LTLfQueryParser::UntilContext * /*ctx*/) override { }

  virtual void enterAnd_future(LTLfQueryParser::And_futureContext * /*ctx*/) override { }
  virtual void exitAnd_future(LTLfQueryParser::And_futureContext * /*ctx*/) override { }

  virtual void enterIfte(LTLfQueryParser::IfteContext * /*ctx*/) override { }
  virtual void exitIfte(LTLfQueryParser::IfteContext * /*ctx*/) override { }

  virtual void enterDeclare_arguments(LTLfQueryParser::Declare_argumentsContext * /*ctx*/) override { }
  virtual void exitDeclare_arguments(LTLfQueryParser::Declare_argumentsContext * /*ctx*/) override { }

  virtual void enterDeclare_act_target(LTLfQueryParser::Declare_act_targetContext * /*ctx*/) override { }
  virtual void exitDeclare_act_target(LTLfQueryParser::Declare_act_targetContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

