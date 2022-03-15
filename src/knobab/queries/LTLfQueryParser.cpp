
// Generated from ../src/knobab/queries/LTLfQuery.g4 by ANTLR 4.9.3


#include "LTLfQueryListener.h"
#include "LTLfQueryVisitor.h"

#include "LTLfQueryParser.h"


using namespace antlrcpp;
using namespace antlr4;

LTLfQueryParser::LTLfQueryParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

LTLfQueryParser::~LTLfQueryParser() {
  delete _interpreter;
}

std::string LTLfQueryParser::getGrammarFileName() const {
  return "LTLfQuery.g4";
}

const std::vector<std::string>& LTLfQueryParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& LTLfQueryParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- Query_plansContext ------------------------------------------------------------------

LTLfQueryParser::Query_plansContext::Query_plansContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<LTLfQueryParser::Query_planContext *> LTLfQueryParser::Query_plansContext::query_plan() {
  return getRuleContexts<LTLfQueryParser::Query_planContext>();
}

LTLfQueryParser::Query_planContext* LTLfQueryParser::Query_plansContext::query_plan(size_t i) {
  return getRuleContext<LTLfQueryParser::Query_planContext>(i);
}


size_t LTLfQueryParser::Query_plansContext::getRuleIndex() const {
  return LTLfQueryParser::RuleQuery_plans;
}

void LTLfQueryParser::Query_plansContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterQuery_plans(this);
}

void LTLfQueryParser::Query_plansContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitQuery_plans(this);
}


antlrcpp::Any LTLfQueryParser::Query_plansContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitQuery_plans(this);
  else
    return visitor->visitChildren(this);
}

LTLfQueryParser::Query_plansContext* LTLfQueryParser::query_plans() {
  Query_plansContext *_localctx = _tracker.createInstance<Query_plansContext>(_ctx, getState());
  enterRule(_localctx, 0, LTLfQueryParser::RuleQuery_plans);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(13); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(12);
      query_plan();
      setState(15); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LTLfQueryParser::AUTO_TIMED

    || _la == LTLfQueryParser::STRING);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Query_planContext ------------------------------------------------------------------

LTLfQueryParser::Query_planContext::Query_planContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLfQueryParser::Query_planContext::STRING() {
  return getToken(LTLfQueryParser::STRING, 0);
}

tree::TerminalNode* LTLfQueryParser::Query_planContext::LPAREN() {
  return getToken(LTLfQueryParser::LPAREN, 0);
}

tree::TerminalNode* LTLfQueryParser::Query_planContext::RPAREN() {
  return getToken(LTLfQueryParser::RPAREN, 0);
}

tree::TerminalNode* LTLfQueryParser::Query_planContext::AUTO_TIMED() {
  return getToken(LTLfQueryParser::AUTO_TIMED, 0);
}

std::vector<LTLfQueryParser::Declare_syntaxContext *> LTLfQueryParser::Query_planContext::declare_syntax() {
  return getRuleContexts<LTLfQueryParser::Declare_syntaxContext>();
}

LTLfQueryParser::Declare_syntaxContext* LTLfQueryParser::Query_planContext::declare_syntax(size_t i) {
  return getRuleContext<LTLfQueryParser::Declare_syntaxContext>(i);
}


size_t LTLfQueryParser::Query_planContext::getRuleIndex() const {
  return LTLfQueryParser::RuleQuery_plan;
}

void LTLfQueryParser::Query_planContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterQuery_plan(this);
}

void LTLfQueryParser::Query_planContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitQuery_plan(this);
}


antlrcpp::Any LTLfQueryParser::Query_planContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitQuery_plan(this);
  else
    return visitor->visitChildren(this);
}

LTLfQueryParser::Query_planContext* LTLfQueryParser::query_plan() {
  Query_planContext *_localctx = _tracker.createInstance<Query_planContext>(_ctx, getState());
  enterRule(_localctx, 2, LTLfQueryParser::RuleQuery_plan);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(18);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LTLfQueryParser::AUTO_TIMED) {
      setState(17);
      match(LTLfQueryParser::AUTO_TIMED);
    }
    setState(20);
    match(LTLfQueryParser::STRING);
    setState(21);
    match(LTLfQueryParser::LPAREN);
    setState(23); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(22);
      declare_syntax();
      setState(25); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LTLfQueryParser::T__0);
    setState(27);
    match(LTLfQueryParser::RPAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Declare_syntaxContext ------------------------------------------------------------------

LTLfQueryParser::Declare_syntaxContext::Declare_syntaxContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLfQueryParser::Declare_syntaxContext::STRING() {
  return getToken(LTLfQueryParser::STRING, 0);
}

LTLfQueryParser::QueryContext* LTLfQueryParser::Declare_syntaxContext::query() {
  return getRuleContext<LTLfQueryParser::QueryContext>(0);
}

LTLfQueryParser::Has_argsContext* LTLfQueryParser::Declare_syntaxContext::has_args() {
  return getRuleContext<LTLfQueryParser::Has_argsContext>(0);
}


size_t LTLfQueryParser::Declare_syntaxContext::getRuleIndex() const {
  return LTLfQueryParser::RuleDeclare_syntax;
}

void LTLfQueryParser::Declare_syntaxContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclare_syntax(this);
}

void LTLfQueryParser::Declare_syntaxContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclare_syntax(this);
}


antlrcpp::Any LTLfQueryParser::Declare_syntaxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitDeclare_syntax(this);
  else
    return visitor->visitChildren(this);
}

LTLfQueryParser::Declare_syntaxContext* LTLfQueryParser::declare_syntax() {
  Declare_syntaxContext *_localctx = _tracker.createInstance<Declare_syntaxContext>(_ctx, getState());
  enterRule(_localctx, 4, LTLfQueryParser::RuleDeclare_syntax);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(29);
    match(LTLfQueryParser::T__0);
    setState(30);
    match(LTLfQueryParser::STRING);
    setState(32);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LTLfQueryParser::T__2) {
      setState(31);
      has_args();
    }
    setState(34);
    match(LTLfQueryParser::T__1);
    setState(35);
    query(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Has_argsContext ------------------------------------------------------------------

LTLfQueryParser::Has_argsContext::Has_argsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLfQueryParser::Has_argsContext::INTNUMBER() {
  return getToken(LTLfQueryParser::INTNUMBER, 0);
}


size_t LTLfQueryParser::Has_argsContext::getRuleIndex() const {
  return LTLfQueryParser::RuleHas_args;
}

void LTLfQueryParser::Has_argsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHas_args(this);
}

void LTLfQueryParser::Has_argsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHas_args(this);
}


antlrcpp::Any LTLfQueryParser::Has_argsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitHas_args(this);
  else
    return visitor->visitChildren(this);
}

LTLfQueryParser::Has_argsContext* LTLfQueryParser::has_args() {
  Has_argsContext *_localctx = _tracker.createInstance<Has_argsContext>(_ctx, getState());
  enterRule(_localctx, 6, LTLfQueryParser::RuleHas_args);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(37);
    match(LTLfQueryParser::T__2);
    setState(38);
    match(LTLfQueryParser::INTNUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- QueryContext ------------------------------------------------------------------

LTLfQueryParser::QueryContext::QueryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLfQueryParser::QueryContext::getRuleIndex() const {
  return LTLfQueryParser::RuleQuery;
}

void LTLfQueryParser::QueryContext::copyFrom(QueryContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- NextContext ------------------------------------------------------------------

LTLfQueryParser::QueryContext* LTLfQueryParser::NextContext::query() {
  return getRuleContext<LTLfQueryParser::QueryContext>(0);
}

LTLfQueryParser::NextContext::NextContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::NextContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNext(this);
}
void LTLfQueryParser::NextContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNext(this);
}

antlrcpp::Any LTLfQueryParser::NextContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitNext(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InitContext ------------------------------------------------------------------

LTLfQueryParser::Declare_argumentsContext* LTLfQueryParser::InitContext::declare_arguments() {
  return getRuleContext<LTLfQueryParser::Declare_argumentsContext>(0);
}

LTLfQueryParser::InitContext::InitContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::InitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInit(this);
}
void LTLfQueryParser::InitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInit(this);
}

antlrcpp::Any LTLfQueryParser::InitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitInit(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrContext ------------------------------------------------------------------

std::vector<LTLfQueryParser::QueryContext *> LTLfQueryParser::OrContext::query() {
  return getRuleContexts<LTLfQueryParser::QueryContext>();
}

LTLfQueryParser::QueryContext* LTLfQueryParser::OrContext::query(size_t i) {
  return getRuleContext<LTLfQueryParser::QueryContext>(i);
}

tree::TerminalNode* LTLfQueryParser::OrContext::TIMED() {
  return getToken(LTLfQueryParser::TIMED, 0);
}

tree::TerminalNode* LTLfQueryParser::OrContext::THETA() {
  return getToken(LTLfQueryParser::THETA, 0);
}

LTLfQueryParser::OrContext::OrContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::OrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOr(this);
}
void LTLfQueryParser::OrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOr(this);
}

antlrcpp::Any LTLfQueryParser::OrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitOr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AbsenceContext ------------------------------------------------------------------

LTLfQueryParser::Declare_argumentsContext* LTLfQueryParser::AbsenceContext::declare_arguments() {
  return getRuleContext<LTLfQueryParser::Declare_argumentsContext>(0);
}

tree::TerminalNode* LTLfQueryParser::AbsenceContext::INTNUMBER() {
  return getToken(LTLfQueryParser::INTNUMBER, 0);
}

LTLfQueryParser::AbsenceContext::AbsenceContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::AbsenceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAbsence(this);
}
void LTLfQueryParser::AbsenceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAbsence(this);
}

antlrcpp::Any LTLfQueryParser::AbsenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitAbsence(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BoxContext ------------------------------------------------------------------

LTLfQueryParser::QueryContext* LTLfQueryParser::BoxContext::query() {
  return getRuleContext<LTLfQueryParser::QueryContext>(0);
}

tree::TerminalNode* LTLfQueryParser::BoxContext::TIMED() {
  return getToken(LTLfQueryParser::TIMED, 0);
}

LTLfQueryParser::BoxContext::BoxContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::BoxContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBox(this);
}
void LTLfQueryParser::BoxContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBox(this);
}

antlrcpp::Any LTLfQueryParser::BoxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitBox(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DiamondContext ------------------------------------------------------------------

LTLfQueryParser::QueryContext* LTLfQueryParser::DiamondContext::query() {
  return getRuleContext<LTLfQueryParser::QueryContext>(0);
}

tree::TerminalNode* LTLfQueryParser::DiamondContext::TIMED() {
  return getToken(LTLfQueryParser::TIMED, 0);
}

LTLfQueryParser::DiamondContext::DiamondContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::DiamondContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDiamond(this);
}
void LTLfQueryParser::DiamondContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDiamond(this);
}

antlrcpp::Any LTLfQueryParser::DiamondContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitDiamond(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NotContext ------------------------------------------------------------------

LTLfQueryParser::QueryContext* LTLfQueryParser::NotContext::query() {
  return getRuleContext<LTLfQueryParser::QueryContext>(0);
}

tree::TerminalNode* LTLfQueryParser::NotContext::TIMED() {
  return getToken(LTLfQueryParser::TIMED, 0);
}

tree::TerminalNode* LTLfQueryParser::NotContext::PRESERVE() {
  return getToken(LTLfQueryParser::PRESERVE, 0);
}

LTLfQueryParser::NotContext::NotContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::NotContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNot(this);
}
void LTLfQueryParser::NotContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNot(this);
}

antlrcpp::Any LTLfQueryParser::NotContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitNot(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenContext ------------------------------------------------------------------

LTLfQueryParser::QueryContext* LTLfQueryParser::ParenContext::query() {
  return getRuleContext<LTLfQueryParser::QueryContext>(0);
}

LTLfQueryParser::ParenContext::ParenContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::ParenContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParen(this);
}
void LTLfQueryParser::ParenContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParen(this);
}

antlrcpp::Any LTLfQueryParser::ParenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitParen(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_next_globallyContext ------------------------------------------------------------------

std::vector<LTLfQueryParser::QueryContext *> LTLfQueryParser::And_next_globallyContext::query() {
  return getRuleContexts<LTLfQueryParser::QueryContext>();
}

LTLfQueryParser::QueryContext* LTLfQueryParser::And_next_globallyContext::query(size_t i) {
  return getRuleContext<LTLfQueryParser::QueryContext>(i);
}

tree::TerminalNode* LTLfQueryParser::And_next_globallyContext::THETA() {
  return getToken(LTLfQueryParser::THETA, 0);
}

LTLfQueryParser::And_next_globallyContext::And_next_globallyContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::And_next_globallyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnd_next_globally(this);
}
void LTLfQueryParser::And_next_globallyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnd_next_globally(this);
}

antlrcpp::Any LTLfQueryParser::And_next_globallyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitAnd_next_globally(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AndContext ------------------------------------------------------------------

std::vector<LTLfQueryParser::QueryContext *> LTLfQueryParser::AndContext::query() {
  return getRuleContexts<LTLfQueryParser::QueryContext>();
}

LTLfQueryParser::QueryContext* LTLfQueryParser::AndContext::query(size_t i) {
  return getRuleContext<LTLfQueryParser::QueryContext>(i);
}

tree::TerminalNode* LTLfQueryParser::AndContext::TIMED() {
  return getToken(LTLfQueryParser::TIMED, 0);
}

tree::TerminalNode* LTLfQueryParser::AndContext::THETA() {
  return getToken(LTLfQueryParser::THETA, 0);
}

LTLfQueryParser::AndContext::AndContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::AndContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnd(this);
}
void LTLfQueryParser::AndContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnd(this);
}

antlrcpp::Any LTLfQueryParser::AndContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitAnd(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ImplicationContext ------------------------------------------------------------------

std::vector<LTLfQueryParser::QueryContext *> LTLfQueryParser::ImplicationContext::query() {
  return getRuleContexts<LTLfQueryParser::QueryContext>();
}

LTLfQueryParser::QueryContext* LTLfQueryParser::ImplicationContext::query(size_t i) {
  return getRuleContext<LTLfQueryParser::QueryContext>(i);
}

tree::TerminalNode* LTLfQueryParser::ImplicationContext::TIMED() {
  return getToken(LTLfQueryParser::TIMED, 0);
}

tree::TerminalNode* LTLfQueryParser::ImplicationContext::THETA() {
  return getToken(LTLfQueryParser::THETA, 0);
}

LTLfQueryParser::ImplicationContext::ImplicationContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::ImplicationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterImplication(this);
}
void LTLfQueryParser::ImplicationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitImplication(this);
}

antlrcpp::Any LTLfQueryParser::ImplicationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitImplication(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExistsContext ------------------------------------------------------------------

LTLfQueryParser::Declare_argumentsContext* LTLfQueryParser::ExistsContext::declare_arguments() {
  return getRuleContext<LTLfQueryParser::Declare_argumentsContext>(0);
}

tree::TerminalNode* LTLfQueryParser::ExistsContext::INTNUMBER() {
  return getToken(LTLfQueryParser::INTNUMBER, 0);
}

LTLfQueryParser::ExistsContext::ExistsContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::ExistsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExists(this);
}
void LTLfQueryParser::ExistsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExists(this);
}

antlrcpp::Any LTLfQueryParser::ExistsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitExists(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EndContext ------------------------------------------------------------------

LTLfQueryParser::Declare_argumentsContext* LTLfQueryParser::EndContext::declare_arguments() {
  return getRuleContext<LTLfQueryParser::Declare_argumentsContext>(0);
}

LTLfQueryParser::EndContext::EndContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::EndContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEnd(this);
}
void LTLfQueryParser::EndContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEnd(this);
}

antlrcpp::Any LTLfQueryParser::EndContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitEnd(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UntilContext ------------------------------------------------------------------

std::vector<LTLfQueryParser::QueryContext *> LTLfQueryParser::UntilContext::query() {
  return getRuleContexts<LTLfQueryParser::QueryContext>();
}

LTLfQueryParser::QueryContext* LTLfQueryParser::UntilContext::query(size_t i) {
  return getRuleContext<LTLfQueryParser::QueryContext>(i);
}

tree::TerminalNode* LTLfQueryParser::UntilContext::TIMED() {
  return getToken(LTLfQueryParser::TIMED, 0);
}

tree::TerminalNode* LTLfQueryParser::UntilContext::THETA() {
  return getToken(LTLfQueryParser::THETA, 0);
}

LTLfQueryParser::UntilContext::UntilContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::UntilContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUntil(this);
}
void LTLfQueryParser::UntilContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUntil(this);
}

antlrcpp::Any LTLfQueryParser::UntilContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitUntil(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_futureContext ------------------------------------------------------------------

std::vector<LTLfQueryParser::QueryContext *> LTLfQueryParser::And_futureContext::query() {
  return getRuleContexts<LTLfQueryParser::QueryContext>();
}

LTLfQueryParser::QueryContext* LTLfQueryParser::And_futureContext::query(size_t i) {
  return getRuleContext<LTLfQueryParser::QueryContext>(i);
}

tree::TerminalNode* LTLfQueryParser::And_futureContext::THETA() {
  return getToken(LTLfQueryParser::THETA, 0);
}

LTLfQueryParser::And_futureContext::And_futureContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::And_futureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnd_future(this);
}
void LTLfQueryParser::And_futureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnd_future(this);
}

antlrcpp::Any LTLfQueryParser::And_futureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitAnd_future(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfteContext ------------------------------------------------------------------

std::vector<LTLfQueryParser::QueryContext *> LTLfQueryParser::IfteContext::query() {
  return getRuleContexts<LTLfQueryParser::QueryContext>();
}

LTLfQueryParser::QueryContext* LTLfQueryParser::IfteContext::query(size_t i) {
  return getRuleContext<LTLfQueryParser::QueryContext>(i);
}

tree::TerminalNode* LTLfQueryParser::IfteContext::TIMED() {
  return getToken(LTLfQueryParser::TIMED, 0);
}

tree::TerminalNode* LTLfQueryParser::IfteContext::THETA() {
  return getToken(LTLfQueryParser::THETA, 0);
}

LTLfQueryParser::IfteContext::IfteContext(QueryContext *ctx) { copyFrom(ctx); }

void LTLfQueryParser::IfteContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfte(this);
}
void LTLfQueryParser::IfteContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfte(this);
}

antlrcpp::Any LTLfQueryParser::IfteContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitIfte(this);
  else
    return visitor->visitChildren(this);
}

LTLfQueryParser::QueryContext* LTLfQueryParser::query() {
   return query(0);
}

LTLfQueryParser::QueryContext* LTLfQueryParser::query(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  LTLfQueryParser::QueryContext *_localctx = _tracker.createInstance<QueryContext>(_ctx, parentState);
  LTLfQueryParser::QueryContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 8;
  enterRecursionRule(_localctx, 8, LTLfQueryParser::RuleQuery, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(90);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LTLfQueryParser::T__3: {
        _localctx = _tracker.createInstance<InitContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(41);
        match(LTLfQueryParser::T__3);
        setState(42);
        declare_arguments();
        break;
      }

      case LTLfQueryParser::T__4: {
        _localctx = _tracker.createInstance<EndContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(43);
        match(LTLfQueryParser::T__4);
        setState(44);
        declare_arguments();
        break;
      }

      case LTLfQueryParser::T__5: {
        _localctx = _tracker.createInstance<ExistsContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(45);
        match(LTLfQueryParser::T__5);
        setState(46);
        declare_arguments();
        setState(47);
        match(LTLfQueryParser::INTNUMBER);
        break;
      }

      case LTLfQueryParser::T__6: {
        _localctx = _tracker.createInstance<AbsenceContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(49);
        match(LTLfQueryParser::T__6);
        setState(50);
        declare_arguments();
        setState(51);
        match(LTLfQueryParser::INTNUMBER);
        break;
      }

      case LTLfQueryParser::T__7: {
        _localctx = _tracker.createInstance<NextContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(53);
        match(LTLfQueryParser::T__7);
        setState(54);
        query(12);
        break;
      }

      case LTLfQueryParser::T__11: {
        _localctx = _tracker.createInstance<IfteContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(55);
        match(LTLfQueryParser::T__11);
        setState(57);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == LTLfQueryParser::TIMED) {
          setState(56);
          match(LTLfQueryParser::TIMED);
        }
        setState(59);
        query(0);
        setState(60);
        match(LTLfQueryParser::T__12);
        setState(61);
        query(0);
        setState(63);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == LTLfQueryParser::THETA) {
          setState(62);
          match(LTLfQueryParser::THETA);
        }
        setState(65);
        match(LTLfQueryParser::T__13);
        setState(66);
        query(8);
        break;
      }

      case LTLfQueryParser::T__15: {
        _localctx = _tracker.createInstance<BoxContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(68);
        match(LTLfQueryParser::T__15);
        setState(70);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == LTLfQueryParser::TIMED) {
          setState(69);
          match(LTLfQueryParser::TIMED);
        }
        setState(72);
        query(6);
        break;
      }

      case LTLfQueryParser::T__16: {
        _localctx = _tracker.createInstance<DiamondContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(73);
        match(LTLfQueryParser::T__16);
        setState(75);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == LTLfQueryParser::TIMED) {
          setState(74);
          match(LTLfQueryParser::TIMED);
        }
        setState(77);
        query(5);
        break;
      }

      case LTLfQueryParser::T__17: {
        _localctx = _tracker.createInstance<NotContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(78);
        match(LTLfQueryParser::T__17);
        setState(80);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == LTLfQueryParser::TIMED) {
          setState(79);
          match(LTLfQueryParser::TIMED);
        }
        setState(82);
        query(0);
        setState(84);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx)) {
        case 1: {
          setState(83);
          match(LTLfQueryParser::PRESERVE);
          break;
        }

        default:
          break;
        }
        break;
      }

      case LTLfQueryParser::T__18: {
        _localctx = _tracker.createInstance<ParenContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(86);
        match(LTLfQueryParser::T__18);
        setState(87);
        query(0);
        setState(88);
        match(LTLfQueryParser::T__19);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(142);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(140);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<OrContext>(_tracker.createInstance<QueryContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleQuery);
          setState(92);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(93);
          match(LTLfQueryParser::T__8);
          setState(95);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::TIMED) {
            setState(94);
            match(LTLfQueryParser::TIMED);
          }
          setState(98);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::THETA) {
            setState(97);
            match(LTLfQueryParser::THETA);
          }
          setState(100);
          query(11);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<AndContext>(_tracker.createInstance<QueryContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleQuery);
          setState(101);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(102);
          match(LTLfQueryParser::T__9);
          setState(104);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::TIMED) {
            setState(103);
            match(LTLfQueryParser::TIMED);
          }
          setState(107);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::THETA) {
            setState(106);
            match(LTLfQueryParser::THETA);
          }
          setState(109);
          query(10);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<ImplicationContext>(_tracker.createInstance<QueryContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleQuery);
          setState(110);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(111);
          match(LTLfQueryParser::T__10);
          setState(113);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::TIMED) {
            setState(112);
            match(LTLfQueryParser::TIMED);
          }
          setState(116);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::THETA) {
            setState(115);
            match(LTLfQueryParser::THETA);
          }
          setState(118);
          query(9);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<UntilContext>(_tracker.createInstance<QueryContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleQuery);
          setState(119);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(120);
          match(LTLfQueryParser::T__14);
          setState(122);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::TIMED) {
            setState(121);
            match(LTLfQueryParser::TIMED);
          }
          setState(125);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::THETA) {
            setState(124);
            match(LTLfQueryParser::THETA);
          }
          setState(127);
          query(7);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<And_futureContext>(_tracker.createInstance<QueryContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleQuery);
          setState(128);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(129);
          match(LTLfQueryParser::T__20);
          setState(131);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::THETA) {
            setState(130);
            match(LTLfQueryParser::THETA);
          }
          setState(133);
          query(2);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<And_next_globallyContext>(_tracker.createInstance<QueryContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleQuery);
          setState(134);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(135);
          match(LTLfQueryParser::T__21);
          setState(137);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LTLfQueryParser::THETA) {
            setState(136);
            match(LTLfQueryParser::THETA);
          }
          setState(139);
          query(1);
          break;
        }

        default:
          break;
        } 
      }
      setState(144);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- Declare_argumentsContext ------------------------------------------------------------------

LTLfQueryParser::Declare_argumentsContext::Declare_argumentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLfQueryParser::Declare_argumentsContext::LEFT() {
  return getToken(LTLfQueryParser::LEFT, 0);
}

tree::TerminalNode* LTLfQueryParser::Declare_argumentsContext::MIDDLE() {
  return getToken(LTLfQueryParser::MIDDLE, 0);
}

tree::TerminalNode* LTLfQueryParser::Declare_argumentsContext::RIGHT() {
  return getToken(LTLfQueryParser::RIGHT, 0);
}


size_t LTLfQueryParser::Declare_argumentsContext::getRuleIndex() const {
  return LTLfQueryParser::RuleDeclare_arguments;
}

void LTLfQueryParser::Declare_argumentsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclare_arguments(this);
}

void LTLfQueryParser::Declare_argumentsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLfQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclare_arguments(this);
}


antlrcpp::Any LTLfQueryParser::Declare_argumentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLfQueryVisitor*>(visitor))
    return parserVisitor->visitDeclare_arguments(this);
  else
    return visitor->visitChildren(this);
}

LTLfQueryParser::Declare_argumentsContext* LTLfQueryParser::declare_arguments() {
  Declare_argumentsContext *_localctx = _tracker.createInstance<Declare_argumentsContext>(_ctx, getState());
  enterRule(_localctx, 10, LTLfQueryParser::RuleDeclare_arguments);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(145);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLfQueryParser::LEFT)
      | (1ULL << LTLfQueryParser::RIGHT)
      | (1ULL << LTLfQueryParser::MIDDLE))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool LTLfQueryParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 4: return querySempred(antlrcpp::downCast<QueryContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool LTLfQueryParser::querySempred(QueryContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 11);
    case 1: return precpred(_ctx, 10);
    case 2: return precpred(_ctx, 9);
    case 3: return precpred(_ctx, 7);
    case 4: return precpred(_ctx, 2);
    case 5: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> LTLfQueryParser::_decisionToDFA;
atn::PredictionContextCache LTLfQueryParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN LTLfQueryParser::_atn;
std::vector<uint16_t> LTLfQueryParser::_serializedATN;

std::vector<std::string> LTLfQueryParser::_ruleNames = {
  "query_plans", "query_plan", "declare_syntax", "has_args", "query", "declare_arguments"
};

std::vector<std::string> LTLfQueryParser::_literalNames = {
  "", "'template'", "':='", "'args'", "'INIT'", "'END'", "'EXISTS'", "'ABSENCE'", 
  "'NEXT'", "'OR'", "'AND'", "'=>'", "'IF'", "'THEN'", "'ELSE'", "'U'", 
  "'G'", "'F'", "'!'", "'('", "')'", "'&Ft'", "'&XGt'", "'auto-timed'", 
  "", "'{'", "'}'", "'PRESERVE'", "'t'", "'THETA'", "'L'", "'R'", "'M'"
};

std::vector<std::string> LTLfQueryParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "AUTO_TIMED", "LABEL", "LPAREN", "RPAREN", "PRESERVE", 
  "TIMED", "THETA", "LEFT", "RIGHT", "MIDDLE", "INTNUMBER", "STRING", "SPACE", 
  "COMMENT", "LINE_COMMENT"
};

dfa::Vocabulary LTLfQueryParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> LTLfQueryParser::_tokenNames;

LTLfQueryParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x27, 0x96, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x3, 0x2, 0x6, 0x2, 0x10, 0xa, 0x2, 0xd, 0x2, 0xe, 0x2, 0x11, 
       0x3, 0x3, 0x5, 0x3, 0x15, 0xa, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
       0x6, 0x3, 0x1a, 0xa, 0x3, 0xd, 0x3, 0xe, 0x3, 0x1b, 0x3, 0x3, 0x3, 
       0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x23, 0xa, 0x4, 0x3, 
       0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
       0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x3c, 0xa, 0x6, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x42, 0xa, 0x6, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x49, 0xa, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x4e, 0xa, 0x6, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x53, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 
       0x5, 0x6, 0x57, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
       0x5, 0x6, 0x5d, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 
       0x62, 0xa, 0x6, 0x3, 0x6, 0x5, 0x6, 0x65, 0xa, 0x6, 0x3, 0x6, 0x3, 
       0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x6b, 0xa, 0x6, 0x3, 0x6, 0x5, 
       0x6, 0x6e, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 
       0x6, 0x74, 0xa, 0x6, 0x3, 0x6, 0x5, 0x6, 0x77, 0xa, 0x6, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x7d, 0xa, 0x6, 0x3, 0x6, 
       0x5, 0x6, 0x80, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
       0x5, 0x6, 0x86, 0xa, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
       0x5, 0x6, 0x8c, 0xa, 0x6, 0x3, 0x6, 0x7, 0x6, 0x8f, 0xa, 0x6, 0xc, 
       0x6, 0xe, 0x6, 0x92, 0xb, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x2, 
       0x3, 0xa, 0x8, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0x2, 0x3, 0x3, 0x2, 
       0x20, 0x22, 0x2, 0xb2, 0x2, 0xf, 0x3, 0x2, 0x2, 0x2, 0x4, 0x14, 0x3, 
       0x2, 0x2, 0x2, 0x6, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x8, 0x27, 0x3, 0x2, 
       0x2, 0x2, 0xa, 0x5c, 0x3, 0x2, 0x2, 0x2, 0xc, 0x93, 0x3, 0x2, 0x2, 
       0x2, 0xe, 0x10, 0x5, 0x4, 0x3, 0x2, 0xf, 0xe, 0x3, 0x2, 0x2, 0x2, 
       0x10, 0x11, 0x3, 0x2, 0x2, 0x2, 0x11, 0xf, 0x3, 0x2, 0x2, 0x2, 0x11, 
       0x12, 0x3, 0x2, 0x2, 0x2, 0x12, 0x3, 0x3, 0x2, 0x2, 0x2, 0x13, 0x15, 
       0x7, 0x19, 0x2, 0x2, 0x14, 0x13, 0x3, 0x2, 0x2, 0x2, 0x14, 0x15, 
       0x3, 0x2, 0x2, 0x2, 0x15, 0x16, 0x3, 0x2, 0x2, 0x2, 0x16, 0x17, 0x7, 
       0x24, 0x2, 0x2, 0x17, 0x19, 0x7, 0x1b, 0x2, 0x2, 0x18, 0x1a, 0x5, 
       0x6, 0x4, 0x2, 0x19, 0x18, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x1b, 0x3, 0x2, 
       0x2, 0x2, 0x1b, 0x19, 0x3, 0x2, 0x2, 0x2, 0x1b, 0x1c, 0x3, 0x2, 0x2, 
       0x2, 0x1c, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x1e, 0x7, 0x1c, 0x2, 
       0x2, 0x1e, 0x5, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x20, 0x7, 0x3, 0x2, 0x2, 
       0x20, 0x22, 0x7, 0x24, 0x2, 0x2, 0x21, 0x23, 0x5, 0x8, 0x5, 0x2, 
       0x22, 0x21, 0x3, 0x2, 0x2, 0x2, 0x22, 0x23, 0x3, 0x2, 0x2, 0x2, 0x23, 
       0x24, 0x3, 0x2, 0x2, 0x2, 0x24, 0x25, 0x7, 0x4, 0x2, 0x2, 0x25, 0x26, 
       0x5, 0xa, 0x6, 0x2, 0x26, 0x7, 0x3, 0x2, 0x2, 0x2, 0x27, 0x28, 0x7, 
       0x5, 0x2, 0x2, 0x28, 0x29, 0x7, 0x23, 0x2, 0x2, 0x29, 0x9, 0x3, 0x2, 
       0x2, 0x2, 0x2a, 0x2b, 0x8, 0x6, 0x1, 0x2, 0x2b, 0x2c, 0x7, 0x6, 0x2, 
       0x2, 0x2c, 0x5d, 0x5, 0xc, 0x7, 0x2, 0x2d, 0x2e, 0x7, 0x7, 0x2, 0x2, 
       0x2e, 0x5d, 0x5, 0xc, 0x7, 0x2, 0x2f, 0x30, 0x7, 0x8, 0x2, 0x2, 0x30, 
       0x31, 0x5, 0xc, 0x7, 0x2, 0x31, 0x32, 0x7, 0x23, 0x2, 0x2, 0x32, 
       0x5d, 0x3, 0x2, 0x2, 0x2, 0x33, 0x34, 0x7, 0x9, 0x2, 0x2, 0x34, 0x35, 
       0x5, 0xc, 0x7, 0x2, 0x35, 0x36, 0x7, 0x23, 0x2, 0x2, 0x36, 0x5d, 
       0x3, 0x2, 0x2, 0x2, 0x37, 0x38, 0x7, 0xa, 0x2, 0x2, 0x38, 0x5d, 0x5, 
       0xa, 0x6, 0xe, 0x39, 0x3b, 0x7, 0xe, 0x2, 0x2, 0x3a, 0x3c, 0x7, 0x1e, 
       0x2, 0x2, 0x3b, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x3c, 0x3, 0x2, 0x2, 
       0x2, 0x3c, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0x5, 0xa, 0x6, 0x2, 
       0x3e, 0x3f, 0x7, 0xf, 0x2, 0x2, 0x3f, 0x41, 0x5, 0xa, 0x6, 0x2, 0x40, 
       0x42, 0x7, 0x1f, 0x2, 0x2, 0x41, 0x40, 0x3, 0x2, 0x2, 0x2, 0x41, 
       0x42, 0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x3, 0x2, 0x2, 0x2, 0x43, 0x44, 
       0x7, 0x10, 0x2, 0x2, 0x44, 0x45, 0x5, 0xa, 0x6, 0xa, 0x45, 0x5d, 
       0x3, 0x2, 0x2, 0x2, 0x46, 0x48, 0x7, 0x12, 0x2, 0x2, 0x47, 0x49, 
       0x7, 0x1e, 0x2, 0x2, 0x48, 0x47, 0x3, 0x2, 0x2, 0x2, 0x48, 0x49, 
       0x3, 0x2, 0x2, 0x2, 0x49, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x5d, 0x5, 
       0xa, 0x6, 0x8, 0x4b, 0x4d, 0x7, 0x13, 0x2, 0x2, 0x4c, 0x4e, 0x7, 
       0x1e, 0x2, 0x2, 0x4d, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x3, 
       0x2, 0x2, 0x2, 0x4e, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x5d, 0x5, 0xa, 
       0x6, 0x7, 0x50, 0x52, 0x7, 0x14, 0x2, 0x2, 0x51, 0x53, 0x7, 0x1e, 
       0x2, 0x2, 0x52, 0x51, 0x3, 0x2, 0x2, 0x2, 0x52, 0x53, 0x3, 0x2, 0x2, 
       0x2, 0x53, 0x54, 0x3, 0x2, 0x2, 0x2, 0x54, 0x56, 0x5, 0xa, 0x6, 0x2, 
       0x55, 0x57, 0x7, 0x1d, 0x2, 0x2, 0x56, 0x55, 0x3, 0x2, 0x2, 0x2, 
       0x56, 0x57, 0x3, 0x2, 0x2, 0x2, 0x57, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x58, 
       0x59, 0x7, 0x15, 0x2, 0x2, 0x59, 0x5a, 0x5, 0xa, 0x6, 0x2, 0x5a, 
       0x5b, 0x7, 0x16, 0x2, 0x2, 0x5b, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x5c, 
       0x2a, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x2f, 
       0x3, 0x2, 0x2, 0x2, 0x5c, 0x33, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x37, 0x3, 
       0x2, 0x2, 0x2, 0x5c, 0x39, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x46, 0x3, 0x2, 
       0x2, 0x2, 0x5c, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x50, 0x3, 0x2, 0x2, 
       0x2, 0x5c, 0x58, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x90, 0x3, 0x2, 0x2, 0x2, 
       0x5e, 0x5f, 0xc, 0xd, 0x2, 0x2, 0x5f, 0x61, 0x7, 0xb, 0x2, 0x2, 0x60, 
       0x62, 0x7, 0x1e, 0x2, 0x2, 0x61, 0x60, 0x3, 0x2, 0x2, 0x2, 0x61, 
       0x62, 0x3, 0x2, 0x2, 0x2, 0x62, 0x64, 0x3, 0x2, 0x2, 0x2, 0x63, 0x65, 
       0x7, 0x1f, 0x2, 0x2, 0x64, 0x63, 0x3, 0x2, 0x2, 0x2, 0x64, 0x65, 
       0x3, 0x2, 0x2, 0x2, 0x65, 0x66, 0x3, 0x2, 0x2, 0x2, 0x66, 0x8f, 0x5, 
       0xa, 0x6, 0xd, 0x67, 0x68, 0xc, 0xc, 0x2, 0x2, 0x68, 0x6a, 0x7, 0xc, 
       0x2, 0x2, 0x69, 0x6b, 0x7, 0x1e, 0x2, 0x2, 0x6a, 0x69, 0x3, 0x2, 
       0x2, 0x2, 0x6a, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x6d, 0x3, 0x2, 0x2, 
       0x2, 0x6c, 0x6e, 0x7, 0x1f, 0x2, 0x2, 0x6d, 0x6c, 0x3, 0x2, 0x2, 
       0x2, 0x6d, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x6f, 0x3, 0x2, 0x2, 0x2, 
       0x6f, 0x8f, 0x5, 0xa, 0x6, 0xc, 0x70, 0x71, 0xc, 0xb, 0x2, 0x2, 0x71, 
       0x73, 0x7, 0xd, 0x2, 0x2, 0x72, 0x74, 0x7, 0x1e, 0x2, 0x2, 0x73, 
       0x72, 0x3, 0x2, 0x2, 0x2, 0x73, 0x74, 0x3, 0x2, 0x2, 0x2, 0x74, 0x76, 
       0x3, 0x2, 0x2, 0x2, 0x75, 0x77, 0x7, 0x1f, 0x2, 0x2, 0x76, 0x75, 
       0x3, 0x2, 0x2, 0x2, 0x76, 0x77, 0x3, 0x2, 0x2, 0x2, 0x77, 0x78, 0x3, 
       0x2, 0x2, 0x2, 0x78, 0x8f, 0x5, 0xa, 0x6, 0xb, 0x79, 0x7a, 0xc, 0x9, 
       0x2, 0x2, 0x7a, 0x7c, 0x7, 0x11, 0x2, 0x2, 0x7b, 0x7d, 0x7, 0x1e, 
       0x2, 0x2, 0x7c, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7d, 0x3, 0x2, 0x2, 
       0x2, 0x7d, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x80, 0x7, 0x1f, 0x2, 
       0x2, 0x7f, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x80, 0x3, 0x2, 0x2, 0x2, 
       0x80, 0x81, 0x3, 0x2, 0x2, 0x2, 0x81, 0x8f, 0x5, 0xa, 0x6, 0x9, 0x82, 
       0x83, 0xc, 0x4, 0x2, 0x2, 0x83, 0x85, 0x7, 0x17, 0x2, 0x2, 0x84, 
       0x86, 0x7, 0x1f, 0x2, 0x2, 0x85, 0x84, 0x3, 0x2, 0x2, 0x2, 0x85, 
       0x86, 0x3, 0x2, 0x2, 0x2, 0x86, 0x87, 0x3, 0x2, 0x2, 0x2, 0x87, 0x8f, 
       0x5, 0xa, 0x6, 0x4, 0x88, 0x89, 0xc, 0x3, 0x2, 0x2, 0x89, 0x8b, 0x7, 
       0x18, 0x2, 0x2, 0x8a, 0x8c, 0x7, 0x1f, 0x2, 0x2, 0x8b, 0x8a, 0x3, 
       0x2, 0x2, 0x2, 0x8b, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8d, 0x3, 0x2, 
       0x2, 0x2, 0x8d, 0x8f, 0x5, 0xa, 0x6, 0x3, 0x8e, 0x5e, 0x3, 0x2, 0x2, 
       0x2, 0x8e, 0x67, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x70, 0x3, 0x2, 0x2, 0x2, 
       0x8e, 0x79, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x82, 0x3, 0x2, 0x2, 0x2, 0x8e, 
       0x88, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x92, 0x3, 0x2, 0x2, 0x2, 0x90, 0x8e, 
       0x3, 0x2, 0x2, 0x2, 0x90, 0x91, 0x3, 0x2, 0x2, 0x2, 0x91, 0xb, 0x3, 
       0x2, 0x2, 0x2, 0x92, 0x90, 0x3, 0x2, 0x2, 0x2, 0x93, 0x94, 0x9, 0x2, 
       0x2, 0x2, 0x94, 0xd, 0x3, 0x2, 0x2, 0x2, 0x19, 0x11, 0x14, 0x1b, 
       0x22, 0x3b, 0x41, 0x48, 0x4d, 0x52, 0x56, 0x5c, 0x61, 0x64, 0x6a, 
       0x6d, 0x73, 0x76, 0x7c, 0x7f, 0x85, 0x8b, 0x8e, 0x90, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

LTLfQueryParser::Initializer LTLfQueryParser::_init;
