
// Generated from src/knobab/queries/LTLfQuery.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  LTLfQueryParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, ACTIVATION = 11, TARGET = 12, INIT = 13, 
    END = 14, EXISTS = 15, ABSENCE = 16, NEXT = 17, OR = 18, AND = 19, FIRST = 20, 
    LAST = 21, IF = 22, THEN = 23, ELSE = 24, UNTIL = 25, BOX = 26, DIAMOND = 27, 
    AUTO_TIMED = 28, LPAREN = 29, RPAREN = 30, PRESERVE = 31, TIMED = 32, 
    THETA = 33, LEFT = 34, INV = 35, RIGHT = 36, MIDDLE = 37, NEGATED = 38, 
    INTNUMBER = 39, STRING = 40, SPACE = 41, COMMENT = 42, LINE_COMMENT = 43
  };

  enum {
    RuleQuery_plans = 0, RuleQuery_plan = 1, RuleDeclare_syntax = 2, RuleHas_args = 3, 
    RuleQuery = 4, RuleDeclare_arguments = 5, RuleDeclare_act_target = 6
  };

  explicit LTLfQueryParser(antlr4::TokenStream *input);
  ~LTLfQueryParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class Query_plansContext;
  class Query_planContext;
  class Declare_syntaxContext;
  class Has_argsContext;
  class QueryContext;
  class Declare_argumentsContext;
  class Declare_act_targetContext; 

  class  Query_plansContext : public antlr4::ParserRuleContext {
  public:
    Query_plansContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Query_planContext *> query_plan();
    Query_planContext* query_plan(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Query_plansContext* query_plans();

  class  Query_planContext : public antlr4::ParserRuleContext {
  public:
    Query_planContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    antlr4::tree::TerminalNode *AUTO_TIMED();
    std::vector<Declare_syntaxContext *> declare_syntax();
    Declare_syntaxContext* declare_syntax(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Query_planContext* query_plan();

  class  Declare_syntaxContext : public antlr4::ParserRuleContext {
  public:
    Declare_syntaxContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    QueryContext *query();
    Has_argsContext *has_args();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Declare_syntaxContext* declare_syntax();

  class  Has_argsContext : public antlr4::ParserRuleContext {
  public:
    Has_argsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTNUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Has_argsContext* has_args();

  class  QueryContext : public antlr4::ParserRuleContext {
  public:
    QueryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    QueryContext() = default;
    void copyFrom(QueryContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  NextContext : public QueryContext {
  public:
    NextContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *NEXT();
    QueryContext *query();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InitContext : public QueryContext {
  public:
    InitContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *INIT();
    antlr4::tree::TerminalNode *TIMED();
    Declare_argumentsContext *declare_arguments();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  OrContext : public QueryContext {
  public:
    OrContext(QueryContext *ctx);

    std::vector<QueryContext *> query();
    QueryContext* query(size_t i);
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LastContext : public QueryContext {
  public:
    LastContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *LAST();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AbsenceContext : public QueryContext {
  public:
    AbsenceContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *ABSENCE();
    antlr4::tree::TerminalNode *INTNUMBER();
    antlr4::tree::TerminalNode *TIMED();
    Declare_argumentsContext *declare_arguments();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BoxContext : public QueryContext {
  public:
    BoxContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *BOX();
    QueryContext *query();
    antlr4::tree::TerminalNode *TIMED();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DiamondContext : public QueryContext {
  public:
    DiamondContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *DIAMOND();
    QueryContext *query();
    antlr4::tree::TerminalNode *TIMED();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NotContext : public QueryContext {
  public:
    NotContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *NEGATED();
    QueryContext *query();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *PRESERVE();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ParenContext : public QueryContext {
  public:
    ParenContext(QueryContext *ctx);

    QueryContext *query();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  And_next_globallyContext : public QueryContext {
  public:
    And_next_globallyContext(QueryContext *ctx);

    std::vector<QueryContext *> query();
    QueryContext* query(size_t i);
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AndContext : public QueryContext {
  public:
    AndContext(QueryContext *ctx);

    std::vector<QueryContext *> query();
    QueryContext* query(size_t i);
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ImplicationContext : public QueryContext {
  public:
    ImplicationContext(QueryContext *ctx);

    std::vector<QueryContext *> query();
    QueryContext* query(size_t i);
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  And_globallyContext : public QueryContext {
  public:
    And_globallyContext(QueryContext *ctx);

    std::vector<QueryContext *> query();
    QueryContext* query(size_t i);
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExistsContext : public QueryContext {
  public:
    ExistsContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *INTNUMBER();
    antlr4::tree::TerminalNode *NEGATED();
    antlr4::tree::TerminalNode *TIMED();
    Declare_argumentsContext *declare_arguments();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  EndContext : public QueryContext {
  public:
    EndContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *END();
    antlr4::tree::TerminalNode *TIMED();
    Declare_argumentsContext *declare_arguments();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UntilContext : public QueryContext {
  public:
    UntilContext(QueryContext *ctx);

    std::vector<QueryContext *> query();
    QueryContext* query(size_t i);
    antlr4::tree::TerminalNode *UNTIL();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  And_futureContext : public QueryContext {
  public:
    And_futureContext(QueryContext *ctx);

    std::vector<QueryContext *> query();
    QueryContext* query(size_t i);
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  FirstContext : public QueryContext {
  public:
    FirstContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *FIRST();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IfteContext : public QueryContext {
  public:
    IfteContext(QueryContext *ctx);

    antlr4::tree::TerminalNode *IF();
    std::vector<QueryContext *> query();
    QueryContext* query(size_t i);
    antlr4::tree::TerminalNode *THEN();
    antlr4::tree::TerminalNode *ELSE();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  QueryContext* query();
  QueryContext* query(int precedence);
  class  Declare_argumentsContext : public antlr4::ParserRuleContext {
  public:
    Declare_argumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTNUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Declare_argumentsContext* declare_arguments();

  class  Declare_act_targetContext : public antlr4::ParserRuleContext {
  public:
    Declare_act_targetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ACTIVATION();
    antlr4::tree::TerminalNode *TARGET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Declare_act_targetContext* declare_act_target();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool querySempred(QueryContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

