
// Generated from src/knobab/queries/LTLfQuery.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  LTLfQueryLexer : public antlr4::Lexer {
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

  explicit LTLfQueryLexer(antlr4::CharStream *input);
  ~LTLfQueryLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

