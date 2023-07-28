
// Generated from KnoBABQuery.g4 by ANTLR 4.11.1

#pragma once


#include "antlr4-runtime.h"




class  KnoBABQueryParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    T__32 = 33, T__33 = 34, T__34 = 35, T__35 = 36, T__36 = 37, T__37 = 38, 
    T__38 = 39, T__39 = 40, T__40 = 41, T__41 = 42, T__42 = 43, T__43 = 44, 
    T__44 = 45, T__45 = 46, T__46 = 47, T__47 = 48, T__48 = 49, T__49 = 50, 
    T__50 = 51, T__51 = 52, T__52 = 53, T__53 = 54, T__54 = 55, T__55 = 56, 
    T__56 = 57, T__57 = 58, T__58 = 59, T__59 = 60, T__60 = 61, T__61 = 62, 
    T__62 = 63, T__63 = 64, T__64 = 65, T__65 = 66, T__66 = 67, T__67 = 68, 
    T__68 = 69, T__69 = 70, T__70 = 71, T__71 = 72, ACT_TABLE = 73, WITH_ALIGNMENT_STRATEGY = 74, 
    CNT_TABLE = 75, ATT_TABLE = 76, ACTIVITYLABEL = 77, LOGS = 78, ATT = 79, 
    ACTIVATION = 80, TARGET = 81, INIT = 82, END = 83, EXISTS = 84, ABSENCE = 85, 
    NEXT = 86, OR = 87, AND = 88, FIRST = 89, LAST = 90, IF = 91, THEN = 92, 
    ELSE = 93, UNTIL = 94, BOX = 95, DIAMOND = 96, AUTO_TIMED = 97, LPAREN = 98, 
    RPAREN = 99, PRESERVE = 100, TIMED = 101, THETA = 102, LEFT = 103, INV = 104, 
    RIGHT = 105, MIDDLE = 106, NEGATED = 107, JOLLY = 108, HRF = 109, TAB = 110, 
    XES = 111, LABEL = 112, INTNUMBER = 113, NUMBER = 114, STRING = 115, 
    SPACE = 116, COMMENT = 117, LINE_COMMENT = 118
  };

  enum {
    RuleQueries = 0, RuleSet_benchmarking_file = 1, RuleLoad_data_query = 2, 
    RuleDisplay_data = 3, RuleDump_log = 4, RuleLog = 5, RuleTrace = 6, 
    RuleEvent = 7, RuleData_part = 8, RuleField = 9, RuleModel_query = 10, 
    RuleWith_model = 11, RuleModel = 12, RuleAtomization = 13, RuleGrounding = 14, 
    RuleDisplay_qp = 15, RuleQuery_plan = 16, RuleDeclare_syntax = 17, RuleHas_args = 18, 
    RuleLtlf = 19, RuleData_aware_declare = 20, RuleDeclare = 21, RuleFields = 22, 
    RuleProp = 23, RuleProp_within_dijunction = 24, RuleAtom = 25, RuleRel = 26, 
    RuleDeclare_arguments = 27, RuleDeclare_act_target = 28, RuleNo_preliminary_fill = 29, 
    RuleAct_for_attributes = 30, RuleNo_cream_off = 31, RuleWith_data = 32, 
    RuleWith_missing = 33, RuleNo_stats = 34, RuleRule_with_weight = 35, 
    RuleConjunctive_subrule = 36, RuleClassification_rule = 37, RuleModel_classification = 38, 
    RuleVar = 39
  };

  explicit KnoBABQueryParser(antlr4::TokenStream *input);

  KnoBABQueryParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~KnoBABQueryParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class QueriesContext;
  class Set_benchmarking_fileContext;
  class Load_data_queryContext;
  class Display_dataContext;
  class Dump_logContext;
  class LogContext;
  class TraceContext;
  class EventContext;
  class Data_partContext;
  class FieldContext;
  class Model_queryContext;
  class With_modelContext;
  class ModelContext;
  class AtomizationContext;
  class GroundingContext;
  class Display_qpContext;
  class Query_planContext;
  class Declare_syntaxContext;
  class Has_argsContext;
  class LtlfContext;
  class Data_aware_declareContext;
  class DeclareContext;
  class FieldsContext;
  class PropContext;
  class Prop_within_dijunctionContext;
  class AtomContext;
  class RelContext;
  class Declare_argumentsContext;
  class Declare_act_targetContext;
  class No_preliminary_fillContext;
  class Act_for_attributesContext;
  class No_cream_offContext;
  class With_dataContext;
  class With_missingContext;
  class No_statsContext;
  class Rule_with_weightContext;
  class Conjunctive_subruleContext;
  class Classification_ruleContext;
  class Model_classificationContext;
  class VarContext; 

  class  QueriesContext : public antlr4::ParserRuleContext {
  public:
    QueriesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Load_data_queryContext *load_data_query();
    Display_dataContext *display_data();
    Model_queryContext *model_query();
    Query_planContext *query_plan();
    Set_benchmarking_fileContext *set_benchmarking_file();
    Dump_logContext *dump_log();
    With_modelContext *with_model();
    ModelContext *model();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  QueriesContext* queries();

  class  Set_benchmarking_fileContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *file = nullptr;
    Set_benchmarking_fileContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Set_benchmarking_fileContext* set_benchmarking_file();

  class  Load_data_queryContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *file = nullptr;
    antlr4::Token *env_name = nullptr;
    Load_data_queryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *HRF();
    antlr4::tree::TerminalNode *TAB();
    antlr4::tree::TerminalNode *XES();
    LogContext *log();
    With_dataContext *with_data();
    No_statsContext *no_stats();
    With_missingContext *with_missing();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Load_data_queryContext* load_data_query();

  class  Display_dataContext : public antlr4::ParserRuleContext {
  public:
    Display_dataContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Display_dataContext() = default;
    void copyFrom(Display_dataContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  DisplayContext : public Display_dataContext {
  public:
    DisplayContext(Display_dataContext *ctx);

    antlr4::Token *attr = nullptr;
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *ACT_TABLE();
    antlr4::tree::TerminalNode *CNT_TABLE();
    antlr4::tree::TerminalNode *ATT_TABLE();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ListContext : public Display_dataContext {
  public:
    ListContext(Display_dataContext *ctx);

    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *ATT();
    antlr4::tree::TerminalNode *ACTIVITYLABEL();
    antlr4::tree::TerminalNode *LOGS();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DroplogContext : public Display_dataContext {
  public:
    DroplogContext(Display_dataContext *ctx);

    antlr4::Token *env_name = nullptr;
    antlr4::tree::TerminalNode *STRING();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Display_dataContext* display_data();

  class  Dump_logContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *env = nullptr;
    antlr4::Token *file = nullptr;
    Dump_logContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *TAB();
    antlr4::tree::TerminalNode *XES();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Dump_logContext* dump_log();

  class  LogContext : public antlr4::ParserRuleContext {
  public:
    LogContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    std::vector<TraceContext *> trace();
    TraceContext* trace(size_t i);
    antlr4::tree::TerminalNode *RPAREN();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LogContext* log();

  class  TraceContext : public antlr4::ParserRuleContext {
  public:
    TraceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Data_partContext *data_part();
    std::vector<EventContext *> event();
    EventContext* event(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TraceContext* trace();

  class  EventContext : public antlr4::ParserRuleContext {
  public:
    EventContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LABEL();
    Data_partContext *data_part();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EventContext* event();

  class  Data_partContext : public antlr4::ParserRuleContext {
  public:
    Data_partContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LPAREN();
    antlr4::tree::TerminalNode *RPAREN();
    std::vector<FieldContext *> field();
    FieldContext* field(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Data_partContext* data_part();

  class  FieldContext : public antlr4::ParserRuleContext {
  public:
    FieldContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VarContext *var();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldContext* field();

  class  Model_queryContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *ensemble = nullptr;
    antlr4::Token *env = nullptr;
    antlr4::Token *plan = nullptr;
    antlr4::Token *operators = nullptr;
    antlr4::Token *nothreads = nullptr;
    Model_queryContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ModelContext *model();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    Display_qpContext *display_qp();
    AtomizationContext *atomization();
    GroundingContext *grounding();
    antlr4::tree::TerminalNode *INTNUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Model_queryContext* model_query();

  class  With_modelContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *cachePath = nullptr;
    antlr4::Token *toFile = nullptr;
    antlr4::Token *minL = nullptr;
    antlr4::Token *maxL = nullptr;
    antlr4::Token *logSize = nullptr;
    antlr4::Token *ratio = nullptr;
    antlr4::Token *doAlign = nullptr;
    antlr4::Token *graphDot = nullptr;
    With_modelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ModelContext *model();
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *WITH_ALIGNMENT_STRATEGY();
    AtomizationContext *atomization();
    GroundingContext *grounding();
    Dump_logContext *dump_log();
    antlr4::tree::TerminalNode *TAB();
    antlr4::tree::TerminalNode *XES();
    std::vector<antlr4::tree::TerminalNode *> INTNUMBER();
    antlr4::tree::TerminalNode* INTNUMBER(size_t i);
    antlr4::tree::TerminalNode *NUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  With_modelContext* with_model();

  class  ModelContext : public antlr4::ParserRuleContext {
  public:
    ModelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ModelContext() = default;
    void copyFrom(ModelContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  DeclaresContext : public ModelContext {
  public:
    DeclaresContext(ModelContext *ctx);

    Data_aware_declareContext *data_aware_declare();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  File_modelContext : public ModelContext {
  public:
    File_modelContext(ModelContext *ctx);

    antlr4::tree::TerminalNode *STRING();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TopnContext : public ModelContext {
  public:
    TopnContext(ModelContext *ctx);

    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *INTNUMBER();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ModelContext* model();

  class  AtomizationContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *label = nullptr;
    antlr4::Token *strlen = nullptr;
    antlr4::Token *strategy = nullptr;
    AtomizationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> STRING();
    antlr4::tree::TerminalNode* STRING(size_t i);
    antlr4::tree::TerminalNode *INTNUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtomizationContext* atomization();

  class  GroundingContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *strategy = nullptr;
    GroundingContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    No_preliminary_fillContext *no_preliminary_fill();
    Act_for_attributesContext *act_for_attributes();
    No_cream_offContext *no_cream_off();
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GroundingContext* grounding();

  class  Display_qpContext : public antlr4::ParserRuleContext {
  public:
    Display_qpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Display_qpContext* display_qp();

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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Query_planContext* query_plan();

  class  Declare_syntaxContext : public antlr4::ParserRuleContext {
  public:
    Declare_syntaxContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();
    LtlfContext *ltlf();
    Has_argsContext *has_args();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Declare_syntaxContext* declare_syntax();

  class  Has_argsContext : public antlr4::ParserRuleContext {
  public:
    Has_argsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTNUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Has_argsContext* has_args();

  class  LtlfContext : public antlr4::ParserRuleContext {
  public:
    LtlfContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    LtlfContext() = default;
    void copyFrom(LtlfContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  NextContext : public LtlfContext {
  public:
    NextContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *NEXT();
    LtlfContext *ltlf();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InitContext : public LtlfContext {
  public:
    InitContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *INIT();
    antlr4::tree::TerminalNode *TIMED();
    Declare_argumentsContext *declare_arguments();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  OrContext : public LtlfContext {
  public:
    OrContext(LtlfContext *ctx);

    std::vector<LtlfContext *> ltlf();
    LtlfContext* ltlf(size_t i);
    antlr4::tree::TerminalNode *OR();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LastContext : public LtlfContext {
  public:
    LastContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *LAST();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AbsenceContext : public LtlfContext {
  public:
    AbsenceContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *ABSENCE();
    antlr4::tree::TerminalNode *INTNUMBER();
    antlr4::tree::TerminalNode *JOLLY();
    antlr4::tree::TerminalNode *TIMED();
    Declare_argumentsContext *declare_arguments();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BoxContext : public LtlfContext {
  public:
    BoxContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *BOX();
    LtlfContext *ltlf();
    antlr4::tree::TerminalNode *TIMED();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DiamondContext : public LtlfContext {
  public:
    DiamondContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *DIAMOND();
    LtlfContext *ltlf();
    antlr4::tree::TerminalNode *TIMED();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NotContext : public LtlfContext {
  public:
    NotContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *NEGATED();
    LtlfContext *ltlf();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *PRESERVE();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ParenContext : public LtlfContext {
  public:
    ParenContext(LtlfContext *ctx);

    LtlfContext *ltlf();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  And_next_globallyContext : public LtlfContext {
  public:
    And_next_globallyContext(LtlfContext *ctx);

    std::vector<LtlfContext *> ltlf();
    LtlfContext* ltlf(size_t i);
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AndContext : public LtlfContext {
  public:
    AndContext(LtlfContext *ctx);

    std::vector<LtlfContext *> ltlf();
    LtlfContext* ltlf(size_t i);
    antlr4::tree::TerminalNode *AND();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ImplicationContext : public LtlfContext {
  public:
    ImplicationContext(LtlfContext *ctx);

    std::vector<LtlfContext *> ltlf();
    LtlfContext* ltlf(size_t i);
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  And_globallyContext : public LtlfContext {
  public:
    And_globallyContext(LtlfContext *ctx);

    std::vector<LtlfContext *> ltlf();
    LtlfContext* ltlf(size_t i);
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ExistsContext : public LtlfContext {
  public:
    ExistsContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *EXISTS();
    antlr4::tree::TerminalNode *INTNUMBER();
    antlr4::tree::TerminalNode *JOLLY();
    antlr4::tree::TerminalNode *NEGATED();
    antlr4::tree::TerminalNode *TIMED();
    Declare_argumentsContext *declare_arguments();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  EndContext : public LtlfContext {
  public:
    EndContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *END();
    antlr4::tree::TerminalNode *TIMED();
    Declare_argumentsContext *declare_arguments();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UntilContext : public LtlfContext {
  public:
    UntilContext(LtlfContext *ctx);

    std::vector<LtlfContext *> ltlf();
    LtlfContext* ltlf(size_t i);
    antlr4::tree::TerminalNode *UNTIL();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  And_futureContext : public LtlfContext {
  public:
    And_futureContext(LtlfContext *ctx);

    std::vector<LtlfContext *> ltlf();
    LtlfContext* ltlf(size_t i);
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  FirstContext : public LtlfContext {
  public:
    FirstContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *FIRST();
    Declare_act_targetContext *declare_act_target();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IfteContext : public LtlfContext {
  public:
    IfteContext(LtlfContext *ctx);

    antlr4::tree::TerminalNode *IF();
    std::vector<LtlfContext *> ltlf();
    LtlfContext* ltlf(size_t i);
    antlr4::tree::TerminalNode *THEN();
    antlr4::tree::TerminalNode *ELSE();
    antlr4::tree::TerminalNode *TIMED();
    antlr4::tree::TerminalNode *THETA();
    antlr4::tree::TerminalNode *INV();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  LtlfContext* ltlf();
  LtlfContext* ltlf(int precedence);
  class  Data_aware_declareContext : public antlr4::ParserRuleContext {
  public:
    Data_aware_declareContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclareContext *> declare();
    DeclareContext* declare(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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
    antlr4::tree::TerminalNode *STRING();
    PropContext *prop();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Unary_propContext : public DeclareContext {
  public:
    Unary_propContext(DeclareContext *ctx);

    antlr4::Token *name = nullptr;
    FieldsContext *fields();
    antlr4::tree::TerminalNode *INTNUMBER();
    antlr4::tree::TerminalNode *STRING();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Conj_or_atomContext : public PropContext {
  public:
    Conj_or_atomContext(PropContext *ctx);

    Prop_within_dijunctionContext *prop_within_dijunction();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TopContext : public PropContext {
  public:
    TopContext(PropContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Atom_conjContext : public Prop_within_dijunctionContext {
  public:
    Atom_conjContext(Prop_within_dijunctionContext *ctx);

    AtomContext *atom();
    Prop_within_dijunctionContext *prop_within_dijunction();
    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Prop_within_dijunctionContext* prop_within_dijunction();

  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *isnegated = nullptr;
    KnoBABQueryParser::VarContext *leftvar = nullptr;
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<VarContext *> var();
    VarContext* var(size_t i);
    RelContext *rel();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *NEGATED();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LtContext : public RelContext {
  public:
    LtContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LeqContext : public RelContext {
  public:
    LeqContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NeqContext : public RelContext {
  public:
    NeqContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  EqContext : public RelContext {
  public:
    EqContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  GtContext : public RelContext {
  public:
    GtContext(RelContext *ctx);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  RelContext* rel();

  class  Declare_argumentsContext : public antlr4::ParserRuleContext {
  public:
    Declare_argumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTNUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Declare_act_targetContext* declare_act_target();

  class  No_preliminary_fillContext : public antlr4::ParserRuleContext {
  public:
    No_preliminary_fillContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  No_preliminary_fillContext* no_preliminary_fill();

  class  Act_for_attributesContext : public antlr4::ParserRuleContext {
  public:
    Act_for_attributesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Act_for_attributesContext* act_for_attributes();

  class  No_cream_offContext : public antlr4::ParserRuleContext {
  public:
    No_cream_offContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  No_cream_offContext* no_cream_off();

  class  With_dataContext : public antlr4::ParserRuleContext {
  public:
    With_dataContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  With_dataContext* with_data();

  class  With_missingContext : public antlr4::ParserRuleContext {
  public:
    With_missingContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  With_missingContext* with_missing();

  class  No_statsContext : public antlr4::ParserRuleContext {
  public:
    No_statsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  No_statsContext* no_stats();

  class  Rule_with_weightContext : public antlr4::ParserRuleContext {
  public:
    Rule_with_weightContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclareContext *declare();
    antlr4::tree::TerminalNode *NUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Rule_with_weightContext* rule_with_weight();

  class  Conjunctive_subruleContext : public antlr4::ParserRuleContext {
  public:
    Conjunctive_subruleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Rule_with_weightContext *> rule_with_weight();
    Rule_with_weightContext* rule_with_weight(size_t i);
    antlr4::tree::TerminalNode *NUMBER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Conjunctive_subruleContext* conjunctive_subrule();

  class  Classification_ruleContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *class_name = nullptr;
    Classification_ruleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Conjunctive_subruleContext *> conjunctive_subrule();
    Conjunctive_subruleContext* conjunctive_subrule(size_t i);
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Classification_ruleContext* classification_rule();

  class  Model_classificationContext : public antlr4::ParserRuleContext {
  public:
    Model_classificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Classification_ruleContext *> classification_rule();
    Classification_ruleContext* classification_rule(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Model_classificationContext* model_classification();

  class  VarContext : public antlr4::ParserRuleContext {
  public:
    VarContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VarContext* var();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool ltlfSempred(LtlfContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

