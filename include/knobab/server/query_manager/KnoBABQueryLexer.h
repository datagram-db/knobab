
// Generated from KnoBABQuery.g4 by ANTLR 4.11.1

#pragma once


#include "antlr4-runtime.h"




class  KnoBABQueryLexer : public antlr4::Lexer {
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
    T__68 = 69, T__69 = 70, T__70 = 71, T__71 = 72, T__72 = 73, T__73 = 74, 
    VIOLATED = 75, VAC_SAT = 76, SAT = 77, ACT_TABLE = 78, WITH_ALIGNMENT_STRATEGY = 79, 
    CNT_TABLE = 80, ATT_TABLE = 81, ACTIVITYLABEL = 82, LOGS = 83, ATT = 84, 
    ACTIVATION = 85, TARGET = 86, INIT = 87, END = 88, EXISTS = 89, ABSENCE = 90, 
    NEXT = 91, OR = 92, AND = 93, FIRST = 94, LAST = 95, IF = 96, THEN = 97, 
    ELSE = 98, UNTIL = 99, BOX = 100, DIAMOND = 101, AUTO_TIMED = 102, LPAREN = 103, 
    RPAREN = 104, PRESERVE = 105, TIMED = 106, THETA = 107, LEFT = 108, 
    INV = 109, RIGHT = 110, MIDDLE = 111, NEGATED = 112, JOLLY = 113, HRF = 114, 
    TAB = 115, XES = 116, LABEL = 117, INTNUMBER = 118, NUMBER = 119, STRING = 120, 
    SPACE = 121, COMMENT = 122, LINE_COMMENT = 123
  };

  explicit KnoBABQueryLexer(antlr4::CharStream *input);

  ~KnoBABQueryLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

