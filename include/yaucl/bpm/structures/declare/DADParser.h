
// Generated from ./src/yaucl/bpm/structures/declare/DAD.g by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  DADParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, LABEL = 16, INTNUMBER = 17, NUMBER = 18, STRING = 19, SPACE = 20, 
    COMMENT = 21, LINE_COMMENT = 22
  };

  enum {
    RuleData_aware_declare = 0, RuleDeclare = 1, RuleFields = 2, RuleProp = 3, 
    RuleProp_within_dijunction = 4, RuleAtom = 5, RuleRel = 6, RuleVar = 7
  };

  explicit DADParser(antlr4::TokenStream *input);
  ~DADParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class Data_aware_declareContext;
  class DeclareContext;
  class FieldsContext;
  class PropContext;
  class Prop_within_dijunctionContext;
  class AtomContext;
  class RelContext;
  class VarContext; 

  class  Data_aware_declareContext : public antlr4::ParserRuleContext {
  public:
    Data_aware_declareContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclareContext *> declare();
    DeclareContext* declare(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Data_aware_declareContext* data_aware_declare();

  class  DeclareContext : public antlr4::ParserRuleContext {
  public:
    DeclareContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    DeclareContext() = default;
    void copyFrom(DeclareContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Nary_propContext : public DeclareContext {
  public:
    Nary_propContext(DeclareContext *ctx);

    antlr4::Token *name = nullptr;
    std::vector<FieldsContext *> fields();
    FieldsContext* fields(size_t i);
    antlr4::tree::TerminalNode *LABEL();
    PropContext *prop();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Unary_propContext : public DeclareContext {
  public:
    Unary_propContext(DeclareContext *ctx);

    antlr4::Token *name = nullptr;
    FieldsContext *fields();
    antlr4::tree::TerminalNode *INTNUMBER();
    antlr4::tree::TerminalNode *LABEL();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  DeclareContext* declare();

  class  FieldsContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *label = nullptr;
    FieldsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PropContext *prop();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldsContext* fields();

  class  PropContext : public antlr4::ParserRuleContext {
  public:
    PropContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    PropContext() = default;
    void copyFrom(PropContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  DisjContext : public PropContext {
  public:
    DisjContext(PropContext *ctx);

    Prop_within_dijunctionContext *prop_within_dijunction();
    PropContext *prop();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Conj_or_atomContext : public PropContext {
  public:
    Conj_or_atomContext(PropContext *ctx);

    Prop_within_dijunctionContext *prop_within_dijunction();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TopContext : public PropContext {
  public:
    TopContext(PropContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  PropContext* prop();

  class  Prop_within_dijunctionContext : public antlr4::ParserRuleContext {
  public:
    Prop_within_dijunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Prop_within_dijunctionContext() = default;
    void copyFrom(Prop_within_dijunctionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  In_atomContext : public Prop_within_dijunctionContext {
  public:
    In_atomContext(Prop_within_dijunctionContext *ctx);

    AtomContext *atom();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Atom_conjContext : public Prop_within_dijunctionContext {
  public:
    Atom_conjContext(Prop_within_dijunctionContext *ctx);

    AtomContext *atom();
    Prop_within_dijunctionContext *prop_within_dijunction();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Prop_within_dijunctionContext* prop_within_dijunction();

  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *isnegated = nullptr;
    DADParser::VarContext *leftvar = nullptr;
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<VarContext *> var();
    VarContext* var(size_t i);
    RelContext *rel();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtomContext* atom();

  class  RelContext : public antlr4::ParserRuleContext {
  public:
    RelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    RelContext() = default;
    void copyFrom(RelContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  GeqContext : public RelContext {
  public:
    GeqContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LtContext : public RelContext {
  public:
    LtContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LeqContext : public RelContext {
  public:
    LeqContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NeqContext : public RelContext {
  public:
    NeqContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  EqContext : public RelContext {
  public:
    EqContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GtContext : public RelContext {
  public:
    GtContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  RelContext* rel();

  class  VarContext : public antlr4::ParserRuleContext {
  public:
    VarContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarContext* var();


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

