
// Generated from ../src/knobab/queries/LTLfQuery.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "LTLfQueryParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by LTLfQueryParser.
 */
class  LTLfQueryListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterQuery_plans(LTLfQueryParser::Query_plansContext *ctx) = 0;
  virtual void exitQuery_plans(LTLfQueryParser::Query_plansContext *ctx) = 0;

  virtual void enterQuery_plan(LTLfQueryParser::Query_planContext *ctx) = 0;
  virtual void exitQuery_plan(LTLfQueryParser::Query_planContext *ctx) = 0;

  virtual void enterDeclare_syntax(LTLfQueryParser::Declare_syntaxContext *ctx) = 0;
  virtual void exitDeclare_syntax(LTLfQueryParser::Declare_syntaxContext *ctx) = 0;

  virtual void enterHas_args(LTLfQueryParser::Has_argsContext *ctx) = 0;
  virtual void exitHas_args(LTLfQueryParser::Has_argsContext *ctx) = 0;

  virtual void enterNext(LTLfQueryParser::NextContext *ctx) = 0;
  virtual void exitNext(LTLfQueryParser::NextContext *ctx) = 0;

  virtual void enterInit(LTLfQueryParser::InitContext *ctx) = 0;
  virtual void exitInit(LTLfQueryParser::InitContext *ctx) = 0;

  virtual void enterOr(LTLfQueryParser::OrContext *ctx) = 0;
  virtual void exitOr(LTLfQueryParser::OrContext *ctx) = 0;

  virtual void enterLast(LTLfQueryParser::LastContext *ctx) = 0;
  virtual void exitLast(LTLfQueryParser::LastContext *ctx) = 0;

  virtual void enterAbsence(LTLfQueryParser::AbsenceContext *ctx) = 0;
  virtual void exitAbsence(LTLfQueryParser::AbsenceContext *ctx) = 0;

  virtual void enterBox(LTLfQueryParser::BoxContext *ctx) = 0;
  virtual void exitBox(LTLfQueryParser::BoxContext *ctx) = 0;

  virtual void enterDiamond(LTLfQueryParser::DiamondContext *ctx) = 0;
  virtual void exitDiamond(LTLfQueryParser::DiamondContext *ctx) = 0;

  virtual void enterNot(LTLfQueryParser::NotContext *ctx) = 0;
  virtual void exitNot(LTLfQueryParser::NotContext *ctx) = 0;

  virtual void enterParen(LTLfQueryParser::ParenContext *ctx) = 0;
  virtual void exitParen(LTLfQueryParser::ParenContext *ctx) = 0;

  virtual void enterAnd_next_globally(LTLfQueryParser::And_next_globallyContext *ctx) = 0;
  virtual void exitAnd_next_globally(LTLfQueryParser::And_next_globallyContext *ctx) = 0;

  virtual void enterAnd(LTLfQueryParser::AndContext *ctx) = 0;
  virtual void exitAnd(LTLfQueryParser::AndContext *ctx) = 0;

  virtual void enterImplication(LTLfQueryParser::ImplicationContext *ctx) = 0;
  virtual void exitImplication(LTLfQueryParser::ImplicationContext *ctx) = 0;

  virtual void enterAnd_globally(LTLfQueryParser::And_globallyContext *ctx) = 0;
  virtual void exitAnd_globally(LTLfQueryParser::And_globallyContext *ctx) = 0;

  virtual void enterExists(LTLfQueryParser::ExistsContext *ctx) = 0;
  virtual void exitExists(LTLfQueryParser::ExistsContext *ctx) = 0;

  virtual void enterEnd(LTLfQueryParser::EndContext *ctx) = 0;
  virtual void exitEnd(LTLfQueryParser::EndContext *ctx) = 0;

  virtual void enterUntil(LTLfQueryParser::UntilContext *ctx) = 0;
  virtual void exitUntil(LTLfQueryParser::UntilContext *ctx) = 0;

  virtual void enterAnd_future(LTLfQueryParser::And_futureContext *ctx) = 0;
  virtual void exitAnd_future(LTLfQueryParser::And_futureContext *ctx) = 0;

  virtual void enterFirst(LTLfQueryParser::FirstContext *ctx) = 0;
  virtual void exitFirst(LTLfQueryParser::FirstContext *ctx) = 0;

  virtual void enterIfte(LTLfQueryParser::IfteContext *ctx) = 0;
  virtual void exitIfte(LTLfQueryParser::IfteContext *ctx) = 0;

  virtual void enterDeclare_arguments(LTLfQueryParser::Declare_argumentsContext *ctx) = 0;
  virtual void exitDeclare_arguments(LTLfQueryParser::Declare_argumentsContext *ctx) = 0;

  virtual void enterDeclare_act_target(LTLfQueryParser::Declare_act_targetContext *ctx) = 0;
  virtual void exitDeclare_act_target(LTLfQueryParser::Declare_act_targetContext *ctx) = 0;


};

