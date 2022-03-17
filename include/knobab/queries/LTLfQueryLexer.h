
// Generated from ../src/knobab/queries/LTLfQuery.g4 by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"




class  LTLfQueryLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, ACTIVATION = 10, TARGET = 11, INIT = 12, END = 13, 
    EXISTS = 14, ABSENCE = 15, NEXT = 16, OR = 17, AND = 18, IF = 19, THEN = 20, 
    ELSE = 21, UNTIL = 22, BOX = 23, DIAMOND = 24, AUTO_TIMED = 25, LPAREN = 26, 
    RPAREN = 27, PRESERVE = 28, TIMED = 29, THETA = 30, LEFT = 31, RIGHT = 32, 
    MIDDLE = 33, NEGATED = 34, INTNUMBER = 35, STRING = 36, SPACE = 37, 
    COMMENT = 38, LINE_COMMENT = 39
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

