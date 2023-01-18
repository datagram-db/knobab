
// Generated from KnoBABQuery.g4 by ANTLR 4.11.1


#include "knobab/server/query_manager/KnoBABQueryLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct KnoBABQueryLexerStaticData final {
  KnoBABQueryLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  KnoBABQueryLexerStaticData(const KnoBABQueryLexerStaticData&) = delete;
  KnoBABQueryLexerStaticData(KnoBABQueryLexerStaticData&&) = delete;
  KnoBABQueryLexerStaticData& operator=(const KnoBABQueryLexerStaticData&) = delete;
  KnoBABQueryLexerStaticData& operator=(KnoBABQueryLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag knobabquerylexerLexerOnceFlag;
KnoBABQueryLexerStaticData *knobabquerylexerLexerStaticData = nullptr;

void knobabquerylexerLexerInitialize() {
  assert(knobabquerylexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<KnoBABQueryLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "T__9", "T__10", "T__11", "T__12", "T__13", "T__14", "T__15", "T__16", 
      "T__17", "T__18", "T__19", "T__20", "T__21", "T__22", "T__23", "T__24", 
      "T__25", "T__26", "T__27", "T__28", "T__29", "T__30", "T__31", "T__32", 
      "T__33", "T__34", "T__35", "T__36", "T__37", "T__38", "T__39", "T__40", 
      "T__41", "T__42", "T__43", "T__44", "T__45", "T__46", "T__47", "T__48", 
      "T__49", "T__50", "T__51", "T__52", "T__53", "T__54", "T__55", "T__56", 
      "T__57", "T__58", "T__59", "ACT_TABLE", "CNT_TABLE", "ATT_TABLE", 
      "ACTIVITYLABEL", "LOGS", "ATT", "ACTIVATION", "TARGET", "INIT", "END", 
      "EXISTS", "ABSENCE", "NEXT", "OR", "AND", "FIRST", "LAST", "IF", "THEN", 
      "ELSE", "UNTIL", "BOX", "DIAMOND", "AUTO_TIMED", "LPAREN", "RPAREN", 
      "PRESERVE", "TIMED", "THETA", "LEFT", "INV", "RIGHT", "MIDDLE", "NEGATED", 
      "HRF", "TAB", "XES", "LABEL", "INTNUMBER", "NUMBER", "STRING", "SPACE", 
      "COMMENT", "LINE_COMMENT"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "'benchmarking-log'", "'load'", "'as'", "'display'", "'for'", 
      "'list'", "'droplog'", "'log'", "';'", "':'", "'='", "'model-check'", 
      "'using'", "'over'", "'plan'", "'with'", "'operators'", "'multithreaded'", 
      "'file'", "'declare'", "'template'", "'logtop'", "'atomize!'", "'label'", 
      "'maximum'", "'strlen'", "'strategy'", "'grounding!'", "'query-plan'", 
      "'queryplan'", "':='", "'args'", "'=>'", "'('", "')'", "'&Ft'", "'&XGt'", 
      "'&Gt'", "','", "'where'", "'||'", "'true'", "'&&'", "'<'", "'<='", 
      "'>'", "'>='", "'!='", "'#'", "'no'", "'preliminary'", "'fill'", "'act'", 
      "'attributes'", "'cream'", "'off'", "'data'", "'missing'", "'stats'", 
      "'var'", "'ACTTABLE'", "'COUNTTABLE'", "'ATTRIBUTETABLE'", "'ACTIVITYLABEL'", 
      "'LOGS'", "'ATTRIBUTES'", "'activation'", "'target'", "'INIT'", "'END'", 
      "'EXISTS'", "'ABSENCE'", "'NEXT'", "'OR'", "'AND'", "'FIRST'", "'LAST'", 
      "'IF'", "'THEN'", "'ELSE'", "'U'", "'G'", "'F'", "'auto-timed'", "'{'", 
      "'}'", "'PRESERVE'", "'t'", "'THETA'", "'L'", "'INV'", "'R'", "'M'", 
      "'~'", "'HRF'", "'TAB'", "'XES'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "ACT_TABLE", "CNT_TABLE", 
      "ATT_TABLE", "ACTIVITYLABEL", "LOGS", "ATT", "ACTIVATION", "TARGET", 
      "INIT", "END", "EXISTS", "ABSENCE", "NEXT", "OR", "AND", "FIRST", 
      "LAST", "IF", "THEN", "ELSE", "UNTIL", "BOX", "DIAMOND", "AUTO_TIMED", 
      "LPAREN", "RPAREN", "PRESERVE", "TIMED", "THETA", "LEFT", "INV", "RIGHT", 
      "MIDDLE", "NEGATED", "HRF", "TAB", "XES", "LABEL", "INTNUMBER", "NUMBER", 
      "STRING", "SPACE", "COMMENT", "LINE_COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,104,824,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,
  	7,6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,
  	14,7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,
  	21,7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,
  	28,7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,
  	35,7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,
  	42,7,42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,
  	49,7,49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,
  	56,7,56,2,57,7,57,2,58,7,58,2,59,7,59,2,60,7,60,2,61,7,61,2,62,7,62,2,
  	63,7,63,2,64,7,64,2,65,7,65,2,66,7,66,2,67,7,67,2,68,7,68,2,69,7,69,2,
  	70,7,70,2,71,7,71,2,72,7,72,2,73,7,73,2,74,7,74,2,75,7,75,2,76,7,76,2,
  	77,7,77,2,78,7,78,2,79,7,79,2,80,7,80,2,81,7,81,2,82,7,82,2,83,7,83,2,
  	84,7,84,2,85,7,85,2,86,7,86,2,87,7,87,2,88,7,88,2,89,7,89,2,90,7,90,2,
  	91,7,91,2,92,7,92,2,93,7,93,2,94,7,94,2,95,7,95,2,96,7,96,2,97,7,97,2,
  	98,7,98,2,99,7,99,2,100,7,100,2,101,7,101,2,102,7,102,2,103,7,103,1,0,
  	1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,
  	1,1,1,1,1,1,1,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,4,1,4,1,4,
  	1,4,1,5,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,
  	7,1,8,1,8,1,9,1,9,1,10,1,10,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,
  	11,1,11,1,11,1,11,1,12,1,12,1,12,1,12,1,12,1,12,1,13,1,13,1,13,1,13,1,
  	13,1,14,1,14,1,14,1,14,1,14,1,15,1,15,1,15,1,15,1,15,1,16,1,16,1,16,1,
  	16,1,16,1,16,1,16,1,16,1,16,1,16,1,17,1,17,1,17,1,17,1,17,1,17,1,17,1,
  	17,1,17,1,17,1,17,1,17,1,17,1,17,1,18,1,18,1,18,1,18,1,18,1,19,1,19,1,
  	19,1,19,1,19,1,19,1,19,1,19,1,20,1,20,1,20,1,20,1,20,1,20,1,20,1,20,1,
  	20,1,21,1,21,1,21,1,21,1,21,1,21,1,21,1,22,1,22,1,22,1,22,1,22,1,22,1,
  	22,1,22,1,22,1,23,1,23,1,23,1,23,1,23,1,23,1,24,1,24,1,24,1,24,1,24,1,
  	24,1,24,1,24,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,26,1,26,1,26,1,26,1,
  	26,1,26,1,26,1,26,1,26,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,
  	27,1,27,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,28,1,29,1,
  	29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,29,1,30,1,30,1,30,1,31,1,31,1,
  	31,1,31,1,31,1,32,1,32,1,32,1,33,1,33,1,34,1,34,1,35,1,35,1,35,1,35,1,
  	36,1,36,1,36,1,36,1,36,1,37,1,37,1,37,1,37,1,38,1,38,1,39,1,39,1,39,1,
  	39,1,39,1,39,1,40,1,40,1,40,1,41,1,41,1,41,1,41,1,41,1,42,1,42,1,42,1,
  	43,1,43,1,44,1,44,1,44,1,45,1,45,1,46,1,46,1,46,1,47,1,47,1,47,1,48,1,
  	48,1,49,1,49,1,49,1,50,1,50,1,50,1,50,1,50,1,50,1,50,1,50,1,50,1,50,1,
  	50,1,50,1,51,1,51,1,51,1,51,1,51,1,52,1,52,1,52,1,52,1,53,1,53,1,53,1,
  	53,1,53,1,53,1,53,1,53,1,53,1,53,1,53,1,54,1,54,1,54,1,54,1,54,1,54,1,
  	55,1,55,1,55,1,55,1,56,1,56,1,56,1,56,1,56,1,57,1,57,1,57,1,57,1,57,1,
  	57,1,57,1,57,1,58,1,58,1,58,1,58,1,58,1,58,1,59,1,59,1,59,1,59,1,60,1,
  	60,1,60,1,60,1,60,1,60,1,60,1,60,1,60,1,61,1,61,1,61,1,61,1,61,1,61,1,
  	61,1,61,1,61,1,61,1,61,1,62,1,62,1,62,1,62,1,62,1,62,1,62,1,62,1,62,1,
  	62,1,62,1,62,1,62,1,62,1,62,1,63,1,63,1,63,1,63,1,63,1,63,1,63,1,63,1,
  	63,1,63,1,63,1,63,1,63,1,63,1,64,1,64,1,64,1,64,1,64,1,65,1,65,1,65,1,
  	65,1,65,1,65,1,65,1,65,1,65,1,65,1,65,1,66,1,66,1,66,1,66,1,66,1,66,1,
  	66,1,66,1,66,1,66,1,66,1,67,1,67,1,67,1,67,1,67,1,67,1,67,1,68,1,68,1,
  	68,1,68,1,68,1,69,1,69,1,69,1,69,1,70,1,70,1,70,1,70,1,70,1,70,1,70,1,
  	71,1,71,1,71,1,71,1,71,1,71,1,71,1,71,1,72,1,72,1,72,1,72,1,72,1,73,1,
  	73,1,73,1,74,1,74,1,74,1,74,1,75,1,75,1,75,1,75,1,75,1,75,1,76,1,76,1,
  	76,1,76,1,76,1,77,1,77,1,77,1,78,1,78,1,78,1,78,1,78,1,79,1,79,1,79,1,
  	79,1,79,1,80,1,80,1,81,1,81,1,82,1,82,1,83,1,83,1,83,1,83,1,83,1,83,1,
  	83,1,83,1,83,1,83,1,83,1,84,1,84,1,85,1,85,1,86,1,86,1,86,1,86,1,86,1,
  	86,1,86,1,86,1,86,1,87,1,87,1,88,1,88,1,88,1,88,1,88,1,88,1,89,1,89,1,
  	90,1,90,1,90,1,90,1,91,1,91,1,92,1,92,1,93,1,93,1,94,1,94,1,94,1,94,1,
  	95,1,95,1,95,1,95,1,96,1,96,1,96,1,96,1,97,1,97,5,97,764,8,97,10,97,12,
  	97,767,9,97,1,98,4,98,770,8,98,11,98,12,98,771,1,99,3,99,775,8,99,1,99,
  	1,99,1,99,3,99,780,8,99,1,100,1,100,1,100,1,100,5,100,786,8,100,10,100,
  	12,100,789,9,100,1,100,1,100,1,101,4,101,794,8,101,11,101,12,101,795,
  	1,101,1,101,1,102,1,102,1,102,1,102,5,102,804,8,102,10,102,12,102,807,
  	9,102,1,102,1,102,1,102,1,102,1,102,1,103,1,103,1,103,1,103,5,103,818,
  	8,103,10,103,12,103,821,9,103,1,103,1,103,1,805,0,104,1,1,3,2,5,3,7,4,
  	9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,
  	17,35,18,37,19,39,20,41,21,43,22,45,23,47,24,49,25,51,26,53,27,55,28,
  	57,29,59,30,61,31,63,32,65,33,67,34,69,35,71,36,73,37,75,38,77,39,79,
  	40,81,41,83,42,85,43,87,44,89,45,91,46,93,47,95,48,97,49,99,50,101,51,
  	103,52,105,53,107,54,109,55,111,56,113,57,115,58,117,59,119,60,121,61,
  	123,62,125,63,127,64,129,65,131,66,133,67,135,68,137,69,139,70,141,71,
  	143,72,145,73,147,74,149,75,151,76,153,77,155,78,157,79,159,80,161,81,
  	163,82,165,83,167,84,169,85,171,86,173,87,175,88,177,89,179,90,181,91,
  	183,92,185,93,187,94,189,95,191,96,193,97,195,98,197,99,199,100,201,101,
  	203,102,205,103,207,104,1,0,4,2,0,65,90,97,122,2,0,34,34,92,92,3,0,9,
  	10,13,13,32,32,2,0,10,10,13,13,832,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,
  	0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,
  	1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,
  	0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,
  	0,39,1,0,0,0,0,41,1,0,0,0,0,43,1,0,0,0,0,45,1,0,0,0,0,47,1,0,0,0,0,49,
  	1,0,0,0,0,51,1,0,0,0,0,53,1,0,0,0,0,55,1,0,0,0,0,57,1,0,0,0,0,59,1,0,
  	0,0,0,61,1,0,0,0,0,63,1,0,0,0,0,65,1,0,0,0,0,67,1,0,0,0,0,69,1,0,0,0,
  	0,71,1,0,0,0,0,73,1,0,0,0,0,75,1,0,0,0,0,77,1,0,0,0,0,79,1,0,0,0,0,81,
  	1,0,0,0,0,83,1,0,0,0,0,85,1,0,0,0,0,87,1,0,0,0,0,89,1,0,0,0,0,91,1,0,
  	0,0,0,93,1,0,0,0,0,95,1,0,0,0,0,97,1,0,0,0,0,99,1,0,0,0,0,101,1,0,0,0,
  	0,103,1,0,0,0,0,105,1,0,0,0,0,107,1,0,0,0,0,109,1,0,0,0,0,111,1,0,0,0,
  	0,113,1,0,0,0,0,115,1,0,0,0,0,117,1,0,0,0,0,119,1,0,0,0,0,121,1,0,0,0,
  	0,123,1,0,0,0,0,125,1,0,0,0,0,127,1,0,0,0,0,129,1,0,0,0,0,131,1,0,0,0,
  	0,133,1,0,0,0,0,135,1,0,0,0,0,137,1,0,0,0,0,139,1,0,0,0,0,141,1,0,0,0,
  	0,143,1,0,0,0,0,145,1,0,0,0,0,147,1,0,0,0,0,149,1,0,0,0,0,151,1,0,0,0,
  	0,153,1,0,0,0,0,155,1,0,0,0,0,157,1,0,0,0,0,159,1,0,0,0,0,161,1,0,0,0,
  	0,163,1,0,0,0,0,165,1,0,0,0,0,167,1,0,0,0,0,169,1,0,0,0,0,171,1,0,0,0,
  	0,173,1,0,0,0,0,175,1,0,0,0,0,177,1,0,0,0,0,179,1,0,0,0,0,181,1,0,0,0,
  	0,183,1,0,0,0,0,185,1,0,0,0,0,187,1,0,0,0,0,189,1,0,0,0,0,191,1,0,0,0,
  	0,193,1,0,0,0,0,195,1,0,0,0,0,197,1,0,0,0,0,199,1,0,0,0,0,201,1,0,0,0,
  	0,203,1,0,0,0,0,205,1,0,0,0,0,207,1,0,0,0,1,209,1,0,0,0,3,226,1,0,0,0,
  	5,231,1,0,0,0,7,234,1,0,0,0,9,242,1,0,0,0,11,246,1,0,0,0,13,251,1,0,0,
  	0,15,259,1,0,0,0,17,263,1,0,0,0,19,265,1,0,0,0,21,267,1,0,0,0,23,269,
  	1,0,0,0,25,281,1,0,0,0,27,287,1,0,0,0,29,292,1,0,0,0,31,297,1,0,0,0,33,
  	302,1,0,0,0,35,312,1,0,0,0,37,326,1,0,0,0,39,331,1,0,0,0,41,339,1,0,0,
  	0,43,348,1,0,0,0,45,355,1,0,0,0,47,364,1,0,0,0,49,370,1,0,0,0,51,378,
  	1,0,0,0,53,385,1,0,0,0,55,394,1,0,0,0,57,405,1,0,0,0,59,416,1,0,0,0,61,
  	426,1,0,0,0,63,429,1,0,0,0,65,434,1,0,0,0,67,437,1,0,0,0,69,439,1,0,0,
  	0,71,441,1,0,0,0,73,445,1,0,0,0,75,450,1,0,0,0,77,454,1,0,0,0,79,456,
  	1,0,0,0,81,462,1,0,0,0,83,465,1,0,0,0,85,470,1,0,0,0,87,473,1,0,0,0,89,
  	475,1,0,0,0,91,478,1,0,0,0,93,480,1,0,0,0,95,483,1,0,0,0,97,486,1,0,0,
  	0,99,488,1,0,0,0,101,491,1,0,0,0,103,503,1,0,0,0,105,508,1,0,0,0,107,
  	512,1,0,0,0,109,523,1,0,0,0,111,529,1,0,0,0,113,533,1,0,0,0,115,538,1,
  	0,0,0,117,546,1,0,0,0,119,552,1,0,0,0,121,556,1,0,0,0,123,565,1,0,0,0,
  	125,576,1,0,0,0,127,591,1,0,0,0,129,605,1,0,0,0,131,610,1,0,0,0,133,621,
  	1,0,0,0,135,632,1,0,0,0,137,639,1,0,0,0,139,644,1,0,0,0,141,648,1,0,0,
  	0,143,655,1,0,0,0,145,663,1,0,0,0,147,668,1,0,0,0,149,671,1,0,0,0,151,
  	675,1,0,0,0,153,681,1,0,0,0,155,686,1,0,0,0,157,689,1,0,0,0,159,694,1,
  	0,0,0,161,699,1,0,0,0,163,701,1,0,0,0,165,703,1,0,0,0,167,705,1,0,0,0,
  	169,716,1,0,0,0,171,718,1,0,0,0,173,720,1,0,0,0,175,729,1,0,0,0,177,731,
  	1,0,0,0,179,737,1,0,0,0,181,739,1,0,0,0,183,743,1,0,0,0,185,745,1,0,0,
  	0,187,747,1,0,0,0,189,749,1,0,0,0,191,753,1,0,0,0,193,757,1,0,0,0,195,
  	761,1,0,0,0,197,769,1,0,0,0,199,774,1,0,0,0,201,781,1,0,0,0,203,793,1,
  	0,0,0,205,799,1,0,0,0,207,813,1,0,0,0,209,210,5,98,0,0,210,211,5,101,
  	0,0,211,212,5,110,0,0,212,213,5,99,0,0,213,214,5,104,0,0,214,215,5,109,
  	0,0,215,216,5,97,0,0,216,217,5,114,0,0,217,218,5,107,0,0,218,219,5,105,
  	0,0,219,220,5,110,0,0,220,221,5,103,0,0,221,222,5,45,0,0,222,223,5,108,
  	0,0,223,224,5,111,0,0,224,225,5,103,0,0,225,2,1,0,0,0,226,227,5,108,0,
  	0,227,228,5,111,0,0,228,229,5,97,0,0,229,230,5,100,0,0,230,4,1,0,0,0,
  	231,232,5,97,0,0,232,233,5,115,0,0,233,6,1,0,0,0,234,235,5,100,0,0,235,
  	236,5,105,0,0,236,237,5,115,0,0,237,238,5,112,0,0,238,239,5,108,0,0,239,
  	240,5,97,0,0,240,241,5,121,0,0,241,8,1,0,0,0,242,243,5,102,0,0,243,244,
  	5,111,0,0,244,245,5,114,0,0,245,10,1,0,0,0,246,247,5,108,0,0,247,248,
  	5,105,0,0,248,249,5,115,0,0,249,250,5,116,0,0,250,12,1,0,0,0,251,252,
  	5,100,0,0,252,253,5,114,0,0,253,254,5,111,0,0,254,255,5,112,0,0,255,256,
  	5,108,0,0,256,257,5,111,0,0,257,258,5,103,0,0,258,14,1,0,0,0,259,260,
  	5,108,0,0,260,261,5,111,0,0,261,262,5,103,0,0,262,16,1,0,0,0,263,264,
  	5,59,0,0,264,18,1,0,0,0,265,266,5,58,0,0,266,20,1,0,0,0,267,268,5,61,
  	0,0,268,22,1,0,0,0,269,270,5,109,0,0,270,271,5,111,0,0,271,272,5,100,
  	0,0,272,273,5,101,0,0,273,274,5,108,0,0,274,275,5,45,0,0,275,276,5,99,
  	0,0,276,277,5,104,0,0,277,278,5,101,0,0,278,279,5,99,0,0,279,280,5,107,
  	0,0,280,24,1,0,0,0,281,282,5,117,0,0,282,283,5,115,0,0,283,284,5,105,
  	0,0,284,285,5,110,0,0,285,286,5,103,0,0,286,26,1,0,0,0,287,288,5,111,
  	0,0,288,289,5,118,0,0,289,290,5,101,0,0,290,291,5,114,0,0,291,28,1,0,
  	0,0,292,293,5,112,0,0,293,294,5,108,0,0,294,295,5,97,0,0,295,296,5,110,
  	0,0,296,30,1,0,0,0,297,298,5,119,0,0,298,299,5,105,0,0,299,300,5,116,
  	0,0,300,301,5,104,0,0,301,32,1,0,0,0,302,303,5,111,0,0,303,304,5,112,
  	0,0,304,305,5,101,0,0,305,306,5,114,0,0,306,307,5,97,0,0,307,308,5,116,
  	0,0,308,309,5,111,0,0,309,310,5,114,0,0,310,311,5,115,0,0,311,34,1,0,
  	0,0,312,313,5,109,0,0,313,314,5,117,0,0,314,315,5,108,0,0,315,316,5,116,
  	0,0,316,317,5,105,0,0,317,318,5,116,0,0,318,319,5,104,0,0,319,320,5,114,
  	0,0,320,321,5,101,0,0,321,322,5,97,0,0,322,323,5,100,0,0,323,324,5,101,
  	0,0,324,325,5,100,0,0,325,36,1,0,0,0,326,327,5,102,0,0,327,328,5,105,
  	0,0,328,329,5,108,0,0,329,330,5,101,0,0,330,38,1,0,0,0,331,332,5,100,
  	0,0,332,333,5,101,0,0,333,334,5,99,0,0,334,335,5,108,0,0,335,336,5,97,
  	0,0,336,337,5,114,0,0,337,338,5,101,0,0,338,40,1,0,0,0,339,340,5,116,
  	0,0,340,341,5,101,0,0,341,342,5,109,0,0,342,343,5,112,0,0,343,344,5,108,
  	0,0,344,345,5,97,0,0,345,346,5,116,0,0,346,347,5,101,0,0,347,42,1,0,0,
  	0,348,349,5,108,0,0,349,350,5,111,0,0,350,351,5,103,0,0,351,352,5,116,
  	0,0,352,353,5,111,0,0,353,354,5,112,0,0,354,44,1,0,0,0,355,356,5,97,0,
  	0,356,357,5,116,0,0,357,358,5,111,0,0,358,359,5,109,0,0,359,360,5,105,
  	0,0,360,361,5,122,0,0,361,362,5,101,0,0,362,363,5,33,0,0,363,46,1,0,0,
  	0,364,365,5,108,0,0,365,366,5,97,0,0,366,367,5,98,0,0,367,368,5,101,0,
  	0,368,369,5,108,0,0,369,48,1,0,0,0,370,371,5,109,0,0,371,372,5,97,0,0,
  	372,373,5,120,0,0,373,374,5,105,0,0,374,375,5,109,0,0,375,376,5,117,0,
  	0,376,377,5,109,0,0,377,50,1,0,0,0,378,379,5,115,0,0,379,380,5,116,0,
  	0,380,381,5,114,0,0,381,382,5,108,0,0,382,383,5,101,0,0,383,384,5,110,
  	0,0,384,52,1,0,0,0,385,386,5,115,0,0,386,387,5,116,0,0,387,388,5,114,
  	0,0,388,389,5,97,0,0,389,390,5,116,0,0,390,391,5,101,0,0,391,392,5,103,
  	0,0,392,393,5,121,0,0,393,54,1,0,0,0,394,395,5,103,0,0,395,396,5,114,
  	0,0,396,397,5,111,0,0,397,398,5,117,0,0,398,399,5,110,0,0,399,400,5,100,
  	0,0,400,401,5,105,0,0,401,402,5,110,0,0,402,403,5,103,0,0,403,404,5,33,
  	0,0,404,56,1,0,0,0,405,406,5,113,0,0,406,407,5,117,0,0,407,408,5,101,
  	0,0,408,409,5,114,0,0,409,410,5,121,0,0,410,411,5,45,0,0,411,412,5,112,
  	0,0,412,413,5,108,0,0,413,414,5,97,0,0,414,415,5,110,0,0,415,58,1,0,0,
  	0,416,417,5,113,0,0,417,418,5,117,0,0,418,419,5,101,0,0,419,420,5,114,
  	0,0,420,421,5,121,0,0,421,422,5,112,0,0,422,423,5,108,0,0,423,424,5,97,
  	0,0,424,425,5,110,0,0,425,60,1,0,0,0,426,427,5,58,0,0,427,428,5,61,0,
  	0,428,62,1,0,0,0,429,430,5,97,0,0,430,431,5,114,0,0,431,432,5,103,0,0,
  	432,433,5,115,0,0,433,64,1,0,0,0,434,435,5,61,0,0,435,436,5,62,0,0,436,
  	66,1,0,0,0,437,438,5,40,0,0,438,68,1,0,0,0,439,440,5,41,0,0,440,70,1,
  	0,0,0,441,442,5,38,0,0,442,443,5,70,0,0,443,444,5,116,0,0,444,72,1,0,
  	0,0,445,446,5,38,0,0,446,447,5,88,0,0,447,448,5,71,0,0,448,449,5,116,
  	0,0,449,74,1,0,0,0,450,451,5,38,0,0,451,452,5,71,0,0,452,453,5,116,0,
  	0,453,76,1,0,0,0,454,455,5,44,0,0,455,78,1,0,0,0,456,457,5,119,0,0,457,
  	458,5,104,0,0,458,459,5,101,0,0,459,460,5,114,0,0,460,461,5,101,0,0,461,
  	80,1,0,0,0,462,463,5,124,0,0,463,464,5,124,0,0,464,82,1,0,0,0,465,466,
  	5,116,0,0,466,467,5,114,0,0,467,468,5,117,0,0,468,469,5,101,0,0,469,84,
  	1,0,0,0,470,471,5,38,0,0,471,472,5,38,0,0,472,86,1,0,0,0,473,474,5,60,
  	0,0,474,88,1,0,0,0,475,476,5,60,0,0,476,477,5,61,0,0,477,90,1,0,0,0,478,
  	479,5,62,0,0,479,92,1,0,0,0,480,481,5,62,0,0,481,482,5,61,0,0,482,94,
  	1,0,0,0,483,484,5,33,0,0,484,485,5,61,0,0,485,96,1,0,0,0,486,487,5,35,
  	0,0,487,98,1,0,0,0,488,489,5,110,0,0,489,490,5,111,0,0,490,100,1,0,0,
  	0,491,492,5,112,0,0,492,493,5,114,0,0,493,494,5,101,0,0,494,495,5,108,
  	0,0,495,496,5,105,0,0,496,497,5,109,0,0,497,498,5,105,0,0,498,499,5,110,
  	0,0,499,500,5,97,0,0,500,501,5,114,0,0,501,502,5,121,0,0,502,102,1,0,
  	0,0,503,504,5,102,0,0,504,505,5,105,0,0,505,506,5,108,0,0,506,507,5,108,
  	0,0,507,104,1,0,0,0,508,509,5,97,0,0,509,510,5,99,0,0,510,511,5,116,0,
  	0,511,106,1,0,0,0,512,513,5,97,0,0,513,514,5,116,0,0,514,515,5,116,0,
  	0,515,516,5,114,0,0,516,517,5,105,0,0,517,518,5,98,0,0,518,519,5,117,
  	0,0,519,520,5,116,0,0,520,521,5,101,0,0,521,522,5,115,0,0,522,108,1,0,
  	0,0,523,524,5,99,0,0,524,525,5,114,0,0,525,526,5,101,0,0,526,527,5,97,
  	0,0,527,528,5,109,0,0,528,110,1,0,0,0,529,530,5,111,0,0,530,531,5,102,
  	0,0,531,532,5,102,0,0,532,112,1,0,0,0,533,534,5,100,0,0,534,535,5,97,
  	0,0,535,536,5,116,0,0,536,537,5,97,0,0,537,114,1,0,0,0,538,539,5,109,
  	0,0,539,540,5,105,0,0,540,541,5,115,0,0,541,542,5,115,0,0,542,543,5,105,
  	0,0,543,544,5,110,0,0,544,545,5,103,0,0,545,116,1,0,0,0,546,547,5,115,
  	0,0,547,548,5,116,0,0,548,549,5,97,0,0,549,550,5,116,0,0,550,551,5,115,
  	0,0,551,118,1,0,0,0,552,553,5,118,0,0,553,554,5,97,0,0,554,555,5,114,
  	0,0,555,120,1,0,0,0,556,557,5,65,0,0,557,558,5,67,0,0,558,559,5,84,0,
  	0,559,560,5,84,0,0,560,561,5,65,0,0,561,562,5,66,0,0,562,563,5,76,0,0,
  	563,564,5,69,0,0,564,122,1,0,0,0,565,566,5,67,0,0,566,567,5,79,0,0,567,
  	568,5,85,0,0,568,569,5,78,0,0,569,570,5,84,0,0,570,571,5,84,0,0,571,572,
  	5,65,0,0,572,573,5,66,0,0,573,574,5,76,0,0,574,575,5,69,0,0,575,124,1,
  	0,0,0,576,577,5,65,0,0,577,578,5,84,0,0,578,579,5,84,0,0,579,580,5,82,
  	0,0,580,581,5,73,0,0,581,582,5,66,0,0,582,583,5,85,0,0,583,584,5,84,0,
  	0,584,585,5,69,0,0,585,586,5,84,0,0,586,587,5,65,0,0,587,588,5,66,0,0,
  	588,589,5,76,0,0,589,590,5,69,0,0,590,126,1,0,0,0,591,592,5,65,0,0,592,
  	593,5,67,0,0,593,594,5,84,0,0,594,595,5,73,0,0,595,596,5,86,0,0,596,597,
  	5,73,0,0,597,598,5,84,0,0,598,599,5,89,0,0,599,600,5,76,0,0,600,601,5,
  	65,0,0,601,602,5,66,0,0,602,603,5,69,0,0,603,604,5,76,0,0,604,128,1,0,
  	0,0,605,606,5,76,0,0,606,607,5,79,0,0,607,608,5,71,0,0,608,609,5,83,0,
  	0,609,130,1,0,0,0,610,611,5,65,0,0,611,612,5,84,0,0,612,613,5,84,0,0,
  	613,614,5,82,0,0,614,615,5,73,0,0,615,616,5,66,0,0,616,617,5,85,0,0,617,
  	618,5,84,0,0,618,619,5,69,0,0,619,620,5,83,0,0,620,132,1,0,0,0,621,622,
  	5,97,0,0,622,623,5,99,0,0,623,624,5,116,0,0,624,625,5,105,0,0,625,626,
  	5,118,0,0,626,627,5,97,0,0,627,628,5,116,0,0,628,629,5,105,0,0,629,630,
  	5,111,0,0,630,631,5,110,0,0,631,134,1,0,0,0,632,633,5,116,0,0,633,634,
  	5,97,0,0,634,635,5,114,0,0,635,636,5,103,0,0,636,637,5,101,0,0,637,638,
  	5,116,0,0,638,136,1,0,0,0,639,640,5,73,0,0,640,641,5,78,0,0,641,642,5,
  	73,0,0,642,643,5,84,0,0,643,138,1,0,0,0,644,645,5,69,0,0,645,646,5,78,
  	0,0,646,647,5,68,0,0,647,140,1,0,0,0,648,649,5,69,0,0,649,650,5,88,0,
  	0,650,651,5,73,0,0,651,652,5,83,0,0,652,653,5,84,0,0,653,654,5,83,0,0,
  	654,142,1,0,0,0,655,656,5,65,0,0,656,657,5,66,0,0,657,658,5,83,0,0,658,
  	659,5,69,0,0,659,660,5,78,0,0,660,661,5,67,0,0,661,662,5,69,0,0,662,144,
  	1,0,0,0,663,664,5,78,0,0,664,665,5,69,0,0,665,666,5,88,0,0,666,667,5,
  	84,0,0,667,146,1,0,0,0,668,669,5,79,0,0,669,670,5,82,0,0,670,148,1,0,
  	0,0,671,672,5,65,0,0,672,673,5,78,0,0,673,674,5,68,0,0,674,150,1,0,0,
  	0,675,676,5,70,0,0,676,677,5,73,0,0,677,678,5,82,0,0,678,679,5,83,0,0,
  	679,680,5,84,0,0,680,152,1,0,0,0,681,682,5,76,0,0,682,683,5,65,0,0,683,
  	684,5,83,0,0,684,685,5,84,0,0,685,154,1,0,0,0,686,687,5,73,0,0,687,688,
  	5,70,0,0,688,156,1,0,0,0,689,690,5,84,0,0,690,691,5,72,0,0,691,692,5,
  	69,0,0,692,693,5,78,0,0,693,158,1,0,0,0,694,695,5,69,0,0,695,696,5,76,
  	0,0,696,697,5,83,0,0,697,698,5,69,0,0,698,160,1,0,0,0,699,700,5,85,0,
  	0,700,162,1,0,0,0,701,702,5,71,0,0,702,164,1,0,0,0,703,704,5,70,0,0,704,
  	166,1,0,0,0,705,706,5,97,0,0,706,707,5,117,0,0,707,708,5,116,0,0,708,
  	709,5,111,0,0,709,710,5,45,0,0,710,711,5,116,0,0,711,712,5,105,0,0,712,
  	713,5,109,0,0,713,714,5,101,0,0,714,715,5,100,0,0,715,168,1,0,0,0,716,
  	717,5,123,0,0,717,170,1,0,0,0,718,719,5,125,0,0,719,172,1,0,0,0,720,721,
  	5,80,0,0,721,722,5,82,0,0,722,723,5,69,0,0,723,724,5,83,0,0,724,725,5,
  	69,0,0,725,726,5,82,0,0,726,727,5,86,0,0,727,728,5,69,0,0,728,174,1,0,
  	0,0,729,730,5,116,0,0,730,176,1,0,0,0,731,732,5,84,0,0,732,733,5,72,0,
  	0,733,734,5,69,0,0,734,735,5,84,0,0,735,736,5,65,0,0,736,178,1,0,0,0,
  	737,738,5,76,0,0,738,180,1,0,0,0,739,740,5,73,0,0,740,741,5,78,0,0,741,
  	742,5,86,0,0,742,182,1,0,0,0,743,744,5,82,0,0,744,184,1,0,0,0,745,746,
  	5,77,0,0,746,186,1,0,0,0,747,748,5,126,0,0,748,188,1,0,0,0,749,750,5,
  	72,0,0,750,751,5,82,0,0,751,752,5,70,0,0,752,190,1,0,0,0,753,754,5,84,
  	0,0,754,755,5,65,0,0,755,756,5,66,0,0,756,192,1,0,0,0,757,758,5,88,0,
  	0,758,759,5,69,0,0,759,760,5,83,0,0,760,194,1,0,0,0,761,765,2,65,90,0,
  	762,764,7,0,0,0,763,762,1,0,0,0,764,767,1,0,0,0,765,763,1,0,0,0,765,766,
  	1,0,0,0,766,196,1,0,0,0,767,765,1,0,0,0,768,770,2,48,57,0,769,768,1,0,
  	0,0,770,771,1,0,0,0,771,769,1,0,0,0,771,772,1,0,0,0,772,198,1,0,0,0,773,
  	775,5,45,0,0,774,773,1,0,0,0,774,775,1,0,0,0,775,776,1,0,0,0,776,779,
  	3,197,98,0,777,778,5,46,0,0,778,780,3,197,98,0,779,777,1,0,0,0,779,780,
  	1,0,0,0,780,200,1,0,0,0,781,787,5,34,0,0,782,786,8,1,0,0,783,784,5,92,
  	0,0,784,786,7,1,0,0,785,782,1,0,0,0,785,783,1,0,0,0,786,789,1,0,0,0,787,
  	785,1,0,0,0,787,788,1,0,0,0,788,790,1,0,0,0,789,787,1,0,0,0,790,791,5,
  	34,0,0,791,202,1,0,0,0,792,794,7,2,0,0,793,792,1,0,0,0,794,795,1,0,0,
  	0,795,793,1,0,0,0,795,796,1,0,0,0,796,797,1,0,0,0,797,798,6,101,0,0,798,
  	204,1,0,0,0,799,800,5,47,0,0,800,801,5,42,0,0,801,805,1,0,0,0,802,804,
  	9,0,0,0,803,802,1,0,0,0,804,807,1,0,0,0,805,806,1,0,0,0,805,803,1,0,0,
  	0,806,808,1,0,0,0,807,805,1,0,0,0,808,809,5,42,0,0,809,810,5,47,0,0,810,
  	811,1,0,0,0,811,812,6,102,0,0,812,206,1,0,0,0,813,814,5,47,0,0,814,815,
  	5,47,0,0,815,819,1,0,0,0,816,818,8,3,0,0,817,816,1,0,0,0,818,821,1,0,
  	0,0,819,817,1,0,0,0,819,820,1,0,0,0,820,822,1,0,0,0,821,819,1,0,0,0,822,
  	823,6,103,0,0,823,208,1,0,0,0,10,0,765,771,774,779,785,787,795,805,819,
  	1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  knobabquerylexerLexerStaticData = staticData.release();
}

}

KnoBABQueryLexer::KnoBABQueryLexer(CharStream *input) : Lexer(input) {
  KnoBABQueryLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *knobabquerylexerLexerStaticData->atn, knobabquerylexerLexerStaticData->decisionToDFA, knobabquerylexerLexerStaticData->sharedContextCache);
}

KnoBABQueryLexer::~KnoBABQueryLexer() {
  delete _interpreter;
}

std::string KnoBABQueryLexer::getGrammarFileName() const {
  return "KnoBABQuery.g4";
}

const std::vector<std::string>& KnoBABQueryLexer::getRuleNames() const {
  return knobabquerylexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& KnoBABQueryLexer::getChannelNames() const {
  return knobabquerylexerLexerStaticData->channelNames;
}

const std::vector<std::string>& KnoBABQueryLexer::getModeNames() const {
  return knobabquerylexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& KnoBABQueryLexer::getVocabulary() const {
  return knobabquerylexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView KnoBABQueryLexer::getSerializedATN() const {
  return knobabquerylexerLexerStaticData->serializedATN;
}

const atn::ATN& KnoBABQueryLexer::getATN() const {
  return *knobabquerylexerLexerStaticData->atn;
}




void KnoBABQueryLexer::initialize() {
  ::antlr4::internal::call_once(knobabquerylexerLexerOnceFlag, knobabquerylexerLexerInitialize);
}
