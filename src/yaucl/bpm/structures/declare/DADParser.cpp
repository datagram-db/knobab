
// Generated from /home/giacomo/projects/knobab/src/yaucl/bpm/structures/declare/DAD.g by ANTLR 4.9.3


#include "yaucl/bpm/structures/declare/DADListener.h"
#include "yaucl/bpm/structures/declare/DADVisitor.h"

#include "yaucl/bpm/structures/declare/DADParser.h"


using namespace antlrcpp;
using namespace antlr4;

DADParser::DADParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

DADParser::~DADParser() {
  delete _interpreter;
}

std::string DADParser::getGrammarFileName() const {
  return "DAD.g";
}

const std::vector<std::string>& DADParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& DADParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- Data_aware_declareContext ------------------------------------------------------------------

DADParser::Data_aware_declareContext::Data_aware_declareContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<DADParser::DeclareContext *> DADParser::Data_aware_declareContext::declare() {
  return getRuleContexts<DADParser::DeclareContext>();
}

DADParser::DeclareContext* DADParser::Data_aware_declareContext::declare(size_t i) {
  return getRuleContext<DADParser::DeclareContext>(i);
}


size_t DADParser::Data_aware_declareContext::getRuleIndex() const {
  return DADParser::RuleData_aware_declare;
}

void DADParser::Data_aware_declareContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterData_aware_declare(this);
}

void DADParser::Data_aware_declareContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitData_aware_declare(this);
}


antlrcpp::Any DADParser::Data_aware_declareContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitData_aware_declare(this);
  else
    return visitor->visitChildren(this);
}

DADParser::Data_aware_declareContext* DADParser::data_aware_declare() {
  Data_aware_declareContext *_localctx = _tracker.createInstance<Data_aware_declareContext>(_ctx, getState());
  enterRule(_localctx, 0, DADParser::RuleData_aware_declare);
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
    setState(17);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == DADParser::LABEL) {
      setState(14);
      declare();
      setState(19);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclareContext ------------------------------------------------------------------

DADParser::DeclareContext::DeclareContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t DADParser::DeclareContext::getRuleIndex() const {
  return DADParser::RuleDeclare;
}

void DADParser::DeclareContext::copyFrom(DeclareContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Nary_propContext ------------------------------------------------------------------

std::vector<DADParser::FieldsContext *> DADParser::Nary_propContext::fields() {
  return getRuleContexts<DADParser::FieldsContext>();
}

DADParser::FieldsContext* DADParser::Nary_propContext::fields(size_t i) {
  return getRuleContext<DADParser::FieldsContext>(i);
}

tree::TerminalNode* DADParser::Nary_propContext::LABEL() {
  return getToken(DADParser::LABEL, 0);
}

DADParser::PropContext* DADParser::Nary_propContext::prop() {
  return getRuleContext<DADParser::PropContext>(0);
}

DADParser::Nary_propContext::Nary_propContext(DeclareContext *ctx) { copyFrom(ctx); }

void DADParser::Nary_propContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNary_prop(this);
}
void DADParser::Nary_propContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNary_prop(this);
}

antlrcpp::Any DADParser::Nary_propContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitNary_prop(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Unary_propContext ------------------------------------------------------------------

DADParser::FieldsContext* DADParser::Unary_propContext::fields() {
  return getRuleContext<DADParser::FieldsContext>(0);
}

tree::TerminalNode* DADParser::Unary_propContext::INTNUMBER() {
  return getToken(DADParser::INTNUMBER, 0);
}

tree::TerminalNode* DADParser::Unary_propContext::LABEL() {
  return getToken(DADParser::LABEL, 0);
}

DADParser::Unary_propContext::Unary_propContext(DeclareContext *ctx) { copyFrom(ctx); }

void DADParser::Unary_propContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnary_prop(this);
}
void DADParser::Unary_propContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnary_prop(this);
}

antlrcpp::Any DADParser::Unary_propContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitUnary_prop(this);
  else
    return visitor->visitChildren(this);
}
DADParser::DeclareContext* DADParser::declare() {
  DeclareContext *_localctx = _tracker.createInstance<DeclareContext>(_ctx, getState());
  enterRule(_localctx, 2, DADParser::RuleDeclare);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    setState(42);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<DADParser::Nary_propContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(20);
      antlrcpp::downCast<Nary_propContext *>(_localctx)->name = match(DADParser::LABEL);
      setState(21);
      match(DADParser::T__0);
      setState(25); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(22);
                fields();
                setState(23);
                match(DADParser::T__1);
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(27); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      setState(29);
      fields();
      setState(30);
      match(DADParser::T__2);
      setState(33);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == DADParser::T__3) {
        setState(31);
        match(DADParser::T__3);
        setState(32);
        prop();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<DADParser::Unary_propContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(35);
      antlrcpp::downCast<Unary_propContext *>(_localctx)->name = match(DADParser::LABEL);
      setState(36);
      match(DADParser::T__0);
      setState(37);
      fields();
      setState(38);
      match(DADParser::T__1);
      setState(39);
      match(DADParser::INTNUMBER);
      setState(40);
      match(DADParser::T__2);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldsContext ------------------------------------------------------------------

DADParser::FieldsContext::FieldsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

DADParser::PropContext* DADParser::FieldsContext::prop() {
  return getRuleContext<DADParser::PropContext>(0);
}

tree::TerminalNode* DADParser::FieldsContext::LABEL() {
  return getToken(DADParser::LABEL, 0);
}


size_t DADParser::FieldsContext::getRuleIndex() const {
  return DADParser::RuleFields;
}

void DADParser::FieldsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFields(this);
}

void DADParser::FieldsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFields(this);
}


antlrcpp::Any DADParser::FieldsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitFields(this);
  else
    return visitor->visitChildren(this);
}

DADParser::FieldsContext* DADParser::fields() {
  FieldsContext *_localctx = _tracker.createInstance<FieldsContext>(_ctx, getState());
  enterRule(_localctx, 4, DADParser::RuleFields);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(44);
    antlrcpp::downCast<FieldsContext *>(_localctx)->label = match(DADParser::LABEL);
    setState(45);
    match(DADParser::T__1);
    setState(46);
    prop();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropContext ------------------------------------------------------------------

DADParser::PropContext::PropContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t DADParser::PropContext::getRuleIndex() const {
  return DADParser::RuleProp;
}

void DADParser::PropContext::copyFrom(PropContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- DisjContext ------------------------------------------------------------------

DADParser::Prop_within_dijunctionContext* DADParser::DisjContext::prop_within_dijunction() {
  return getRuleContext<DADParser::Prop_within_dijunctionContext>(0);
}

DADParser::PropContext* DADParser::DisjContext::prop() {
  return getRuleContext<DADParser::PropContext>(0);
}

DADParser::DisjContext::DisjContext(PropContext *ctx) { copyFrom(ctx); }

void DADParser::DisjContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDisj(this);
}
void DADParser::DisjContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDisj(this);
}

antlrcpp::Any DADParser::DisjContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitDisj(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Conj_or_atomContext ------------------------------------------------------------------

DADParser::Prop_within_dijunctionContext* DADParser::Conj_or_atomContext::prop_within_dijunction() {
  return getRuleContext<DADParser::Prop_within_dijunctionContext>(0);
}

DADParser::Conj_or_atomContext::Conj_or_atomContext(PropContext *ctx) { copyFrom(ctx); }

void DADParser::Conj_or_atomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConj_or_atom(this);
}
void DADParser::Conj_or_atomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConj_or_atom(this);
}

antlrcpp::Any DADParser::Conj_or_atomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitConj_or_atom(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TopContext ------------------------------------------------------------------

DADParser::TopContext::TopContext(PropContext *ctx) { copyFrom(ctx); }

void DADParser::TopContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTop(this);
}
void DADParser::TopContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTop(this);
}

antlrcpp::Any DADParser::TopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitTop(this);
  else
    return visitor->visitChildren(this);
}
DADParser::PropContext* DADParser::prop() {
  PropContext *_localctx = _tracker.createInstance<PropContext>(_ctx, getState());
  enterRule(_localctx, 6, DADParser::RuleProp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(54);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<DADParser::DisjContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(48);
      prop_within_dijunction();
      setState(49);
      match(DADParser::T__4);
      setState(50);
      prop();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<DADParser::Conj_or_atomContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(52);
      prop_within_dijunction();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<DADParser::TopContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(53);
      match(DADParser::T__5);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Prop_within_dijunctionContext ------------------------------------------------------------------

DADParser::Prop_within_dijunctionContext::Prop_within_dijunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t DADParser::Prop_within_dijunctionContext::getRuleIndex() const {
  return DADParser::RuleProp_within_dijunction;
}

void DADParser::Prop_within_dijunctionContext::copyFrom(Prop_within_dijunctionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- In_atomContext ------------------------------------------------------------------

DADParser::AtomContext* DADParser::In_atomContext::atom() {
  return getRuleContext<DADParser::AtomContext>(0);
}

DADParser::In_atomContext::In_atomContext(Prop_within_dijunctionContext *ctx) { copyFrom(ctx); }

void DADParser::In_atomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIn_atom(this);
}
void DADParser::In_atomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIn_atom(this);
}

antlrcpp::Any DADParser::In_atomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitIn_atom(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Atom_conjContext ------------------------------------------------------------------

DADParser::AtomContext* DADParser::Atom_conjContext::atom() {
  return getRuleContext<DADParser::AtomContext>(0);
}

DADParser::Prop_within_dijunctionContext* DADParser::Atom_conjContext::prop_within_dijunction() {
  return getRuleContext<DADParser::Prop_within_dijunctionContext>(0);
}

DADParser::Atom_conjContext::Atom_conjContext(Prop_within_dijunctionContext *ctx) { copyFrom(ctx); }

void DADParser::Atom_conjContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtom_conj(this);
}
void DADParser::Atom_conjContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtom_conj(this);
}

antlrcpp::Any DADParser::Atom_conjContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitAtom_conj(this);
  else
    return visitor->visitChildren(this);
}
DADParser::Prop_within_dijunctionContext* DADParser::prop_within_dijunction() {
  Prop_within_dijunctionContext *_localctx = _tracker.createInstance<Prop_within_dijunctionContext>(_ctx, getState());
  enterRule(_localctx, 8, DADParser::RuleProp_within_dijunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(61);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<DADParser::In_atomContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(56);
      atom();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<DADParser::Atom_conjContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(57);
      atom();
      setState(58);
      match(DADParser::T__6);
      setState(59);
      prop_within_dijunction();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

DADParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> DADParser::AtomContext::VAR() {
  return getTokens(DADParser::VAR);
}

tree::TerminalNode* DADParser::AtomContext::VAR(size_t i) {
  return getToken(DADParser::VAR, i);
}

DADParser::RelContext* DADParser::AtomContext::rel() {
  return getRuleContext<DADParser::RelContext>(0);
}

tree::TerminalNode* DADParser::AtomContext::NUMBER() {
  return getToken(DADParser::NUMBER, 0);
}

tree::TerminalNode* DADParser::AtomContext::STRING() {
  return getToken(DADParser::STRING, 0);
}


size_t DADParser::AtomContext::getRuleIndex() const {
  return DADParser::RuleAtom;
}

void DADParser::AtomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtom(this);
}

void DADParser::AtomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtom(this);
}


antlrcpp::Any DADParser::AtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitAtom(this);
  else
    return visitor->visitChildren(this);
}

DADParser::AtomContext* DADParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 10, DADParser::RuleAtom);
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
    setState(64);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == DADParser::T__7) {
      setState(63);
      antlrcpp::downCast<AtomContext *>(_localctx)->isnegated = match(DADParser::T__7);
    }
    setState(66);
    match(DADParser::VAR);
    setState(67);
    rel();
    setState(71);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case DADParser::NUMBER: {
        setState(68);
        match(DADParser::NUMBER);
        break;
      }

      case DADParser::STRING: {
        setState(69);
        match(DADParser::STRING);
        break;
      }

      case DADParser::VAR: {
        setState(70);
        antlrcpp::downCast<AtomContext *>(_localctx)->leftvar = match(DADParser::VAR);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelContext ------------------------------------------------------------------

DADParser::RelContext::RelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t DADParser::RelContext::getRuleIndex() const {
  return DADParser::RuleRel;
}

void DADParser::RelContext::copyFrom(RelContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- GeqContext ------------------------------------------------------------------

DADParser::GeqContext::GeqContext(RelContext *ctx) { copyFrom(ctx); }

void DADParser::GeqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGeq(this);
}
void DADParser::GeqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGeq(this);
}

antlrcpp::Any DADParser::GeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitGeq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LtContext ------------------------------------------------------------------

DADParser::LtContext::LtContext(RelContext *ctx) { copyFrom(ctx); }

void DADParser::LtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLt(this);
}
void DADParser::LtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLt(this);
}

antlrcpp::Any DADParser::LtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitLt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LeqContext ------------------------------------------------------------------

DADParser::LeqContext::LeqContext(RelContext *ctx) { copyFrom(ctx); }

void DADParser::LeqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLeq(this);
}
void DADParser::LeqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLeq(this);
}

antlrcpp::Any DADParser::LeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitLeq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NeqContext ------------------------------------------------------------------

DADParser::NeqContext::NeqContext(RelContext *ctx) { copyFrom(ctx); }

void DADParser::NeqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNeq(this);
}
void DADParser::NeqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNeq(this);
}

antlrcpp::Any DADParser::NeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitNeq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EqContext ------------------------------------------------------------------

DADParser::EqContext::EqContext(RelContext *ctx) { copyFrom(ctx); }

void DADParser::EqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEq(this);
}
void DADParser::EqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEq(this);
}

antlrcpp::Any DADParser::EqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitEq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- GtContext ------------------------------------------------------------------

DADParser::GtContext::GtContext(RelContext *ctx) { copyFrom(ctx); }

void DADParser::GtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGt(this);
}
void DADParser::GtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<DADListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGt(this);
}

antlrcpp::Any DADParser::GtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<DADVisitor*>(visitor))
    return parserVisitor->visitGt(this);
  else
    return visitor->visitChildren(this);
}
DADParser::RelContext* DADParser::rel() {
  RelContext *_localctx = _tracker.createInstance<RelContext>(_ctx, getState());
  enterRule(_localctx, 12, DADParser::RuleRel);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(79);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case DADParser::T__8: {
        _localctx = _tracker.createInstance<DADParser::LtContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(73);
        match(DADParser::T__8);
        break;
      }

      case DADParser::T__9: {
        _localctx = _tracker.createInstance<DADParser::LeqContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(74);
        match(DADParser::T__9);
        break;
      }

      case DADParser::T__10: {
        _localctx = _tracker.createInstance<DADParser::GtContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(75);
        match(DADParser::T__10);
        break;
      }

      case DADParser::T__11: {
        _localctx = _tracker.createInstance<DADParser::GeqContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(76);
        match(DADParser::T__11);
        break;
      }

      case DADParser::T__12: {
        _localctx = _tracker.createInstance<DADParser::EqContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(77);
        match(DADParser::T__12);
        break;
      }

      case DADParser::T__13: {
        _localctx = _tracker.createInstance<DADParser::NeqContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(78);
        match(DADParser::T__13);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> DADParser::_decisionToDFA;
atn::PredictionContextCache DADParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN DADParser::_atn;
std::vector<uint16_t> DADParser::_serializedATN;

std::vector<std::string> DADParser::_ruleNames = {
  "data_aware_declare", "declare", "fields", "prop", "prop_within_dijunction", 
  "atom", "rel"
};

std::vector<std::string> DADParser::_literalNames = {
  "", "'('", "','", "')'", "'where'", "'||'", "'true'", "'&&'", "'~'", "'<'", 
  "'<='", "'>'", "'>='", "'='", "'!='"
};

std::vector<std::string> DADParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "VAR", "LABEL", 
  "INTNUMBER", "NUMBER", "STRING", "SPACE", "COMMENT", "LINE_COMMENT"
};

dfa::Vocabulary DADParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> DADParser::_tokenNames;

DADParser::Initializer::Initializer() {
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
       0x3, 0x18, 0x54, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x3, 0x2, 0x7, 0x2, 0x12, 0xa, 0x2, 0xc, 
       0x2, 0xe, 0x2, 0x15, 0xb, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
       0x3, 0x3, 0x3, 0x6, 0x3, 0x1c, 0xa, 0x3, 0xd, 0x3, 0xe, 0x3, 0x1d, 
       0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x24, 0xa, 0x3, 
       0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
       0x3, 0x5, 0x3, 0x2d, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
       0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
       0x5, 0x5, 0x39, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
       0x3, 0x6, 0x5, 0x6, 0x40, 0xa, 0x6, 0x3, 0x7, 0x5, 0x7, 0x43, 0xa, 
       0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 
       0x4a, 0xa, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
       0x3, 0x8, 0x5, 0x8, 0x52, 0xa, 0x8, 0x3, 0x8, 0x2, 0x2, 0x9, 0x2, 
       0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x2, 0x2, 0x2, 0x5b, 0x2, 0x13, 0x3, 
       0x2, 0x2, 0x2, 0x4, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x6, 0x2e, 0x3, 0x2, 
       0x2, 0x2, 0x8, 0x38, 0x3, 0x2, 0x2, 0x2, 0xa, 0x3f, 0x3, 0x2, 0x2, 
       0x2, 0xc, 0x42, 0x3, 0x2, 0x2, 0x2, 0xe, 0x51, 0x3, 0x2, 0x2, 0x2, 
       0x10, 0x12, 0x5, 0x4, 0x3, 0x2, 0x11, 0x10, 0x3, 0x2, 0x2, 0x2, 0x12, 
       0x15, 0x3, 0x2, 0x2, 0x2, 0x13, 0x11, 0x3, 0x2, 0x2, 0x2, 0x13, 0x14, 
       0x3, 0x2, 0x2, 0x2, 0x14, 0x3, 0x3, 0x2, 0x2, 0x2, 0x15, 0x13, 0x3, 
       0x2, 0x2, 0x2, 0x16, 0x17, 0x7, 0x12, 0x2, 0x2, 0x17, 0x1b, 0x7, 
       0x3, 0x2, 0x2, 0x18, 0x19, 0x5, 0x6, 0x4, 0x2, 0x19, 0x1a, 0x7, 0x4, 
       0x2, 0x2, 0x1a, 0x1c, 0x3, 0x2, 0x2, 0x2, 0x1b, 0x18, 0x3, 0x2, 0x2, 
       0x2, 0x1c, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x1d, 0x1b, 0x3, 0x2, 0x2, 0x2, 
       0x1d, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x1f, 
       0x20, 0x5, 0x6, 0x4, 0x2, 0x20, 0x23, 0x7, 0x5, 0x2, 0x2, 0x21, 0x22, 
       0x7, 0x6, 0x2, 0x2, 0x22, 0x24, 0x5, 0x8, 0x5, 0x2, 0x23, 0x21, 0x3, 
       0x2, 0x2, 0x2, 0x23, 0x24, 0x3, 0x2, 0x2, 0x2, 0x24, 0x2d, 0x3, 0x2, 
       0x2, 0x2, 0x25, 0x26, 0x7, 0x12, 0x2, 0x2, 0x26, 0x27, 0x7, 0x3, 
       0x2, 0x2, 0x27, 0x28, 0x5, 0x6, 0x4, 0x2, 0x28, 0x29, 0x7, 0x4, 0x2, 
       0x2, 0x29, 0x2a, 0x7, 0x13, 0x2, 0x2, 0x2a, 0x2b, 0x7, 0x5, 0x2, 
       0x2, 0x2b, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x16, 0x3, 0x2, 0x2, 0x2, 
       0x2c, 0x25, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2e, 
       0x2f, 0x7, 0x12, 0x2, 0x2, 0x2f, 0x30, 0x7, 0x4, 0x2, 0x2, 0x30, 
       0x31, 0x5, 0x8, 0x5, 0x2, 0x31, 0x7, 0x3, 0x2, 0x2, 0x2, 0x32, 0x33, 
       0x5, 0xa, 0x6, 0x2, 0x33, 0x34, 0x7, 0x7, 0x2, 0x2, 0x34, 0x35, 0x5, 
       0x8, 0x5, 0x2, 0x35, 0x39, 0x3, 0x2, 0x2, 0x2, 0x36, 0x39, 0x5, 0xa, 
       0x6, 0x2, 0x37, 0x39, 0x7, 0x8, 0x2, 0x2, 0x38, 0x32, 0x3, 0x2, 0x2, 
       0x2, 0x38, 0x36, 0x3, 0x2, 0x2, 0x2, 0x38, 0x37, 0x3, 0x2, 0x2, 0x2, 
       0x39, 0x9, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x40, 0x5, 0xc, 0x7, 0x2, 0x3b, 
       0x3c, 0x5, 0xc, 0x7, 0x2, 0x3c, 0x3d, 0x7, 0x9, 0x2, 0x2, 0x3d, 0x3e, 
       0x5, 0xa, 0x6, 0x2, 0x3e, 0x40, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x3a, 0x3, 
       0x2, 0x2, 0x2, 0x3f, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x40, 0xb, 0x3, 0x2, 
       0x2, 0x2, 0x41, 0x43, 0x7, 0xa, 0x2, 0x2, 0x42, 0x41, 0x3, 0x2, 0x2, 
       0x2, 0x42, 0x43, 0x3, 0x2, 0x2, 0x2, 0x43, 0x44, 0x3, 0x2, 0x2, 0x2, 
       0x44, 0x45, 0x7, 0x11, 0x2, 0x2, 0x45, 0x49, 0x5, 0xe, 0x8, 0x2, 
       0x46, 0x4a, 0x7, 0x14, 0x2, 0x2, 0x47, 0x4a, 0x7, 0x15, 0x2, 0x2, 
       0x48, 0x4a, 0x7, 0x11, 0x2, 0x2, 0x49, 0x46, 0x3, 0x2, 0x2, 0x2, 
       0x49, 0x47, 0x3, 0x2, 0x2, 0x2, 0x49, 0x48, 0x3, 0x2, 0x2, 0x2, 0x4a, 
       0xd, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x52, 0x7, 0xb, 0x2, 0x2, 0x4c, 0x52, 
       0x7, 0xc, 0x2, 0x2, 0x4d, 0x52, 0x7, 0xd, 0x2, 0x2, 0x4e, 0x52, 0x7, 
       0xe, 0x2, 0x2, 0x4f, 0x52, 0x7, 0xf, 0x2, 0x2, 0x50, 0x52, 0x7, 0x10, 
       0x2, 0x2, 0x51, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x51, 0x4c, 0x3, 0x2, 0x2, 
       0x2, 0x51, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x51, 0x4e, 0x3, 0x2, 0x2, 0x2, 
       0x51, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x51, 0x50, 0x3, 0x2, 0x2, 0x2, 0x52, 
       0xf, 0x3, 0x2, 0x2, 0x2, 0xb, 0x13, 0x1d, 0x23, 0x2c, 0x38, 0x3f, 
       0x42, 0x49, 0x51, 
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

DADParser::Initializer DADParser::_init;
