
// Generated from KnoBABQuery.g4 by ANTLR 4.11.1


#include "knobab/server/query_manager/KnoBABQueryListener.h"
#include "knobab/server/query_manager/KnoBABQueryVisitor.h"

#include "knobab/server/query_manager/KnoBABQueryParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct KnoBABQueryParserStaticData final {
  KnoBABQueryParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  KnoBABQueryParserStaticData(const KnoBABQueryParserStaticData&) = delete;
  KnoBABQueryParserStaticData(KnoBABQueryParserStaticData&&) = delete;
  KnoBABQueryParserStaticData& operator=(const KnoBABQueryParserStaticData&) = delete;
  KnoBABQueryParserStaticData& operator=(KnoBABQueryParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag knobabqueryParserOnceFlag;
KnoBABQueryParserStaticData *knobabqueryParserStaticData = nullptr;

void knobabqueryParserInitialize() {
  assert(knobabqueryParserStaticData == nullptr);
  auto staticData = std::make_unique<KnoBABQueryParserStaticData>(
    std::vector<std::string>{
      "queries", "set_benchmarking_file", "load_data_query", "display_data", 
      "dump_log", "log", "trace", "event", "data_part", "field", "model_query", 
      "with_model", "model", "atomization", "grounding", "display_qp", "query_plan", 
      "declare_syntax", "has_args", "ltlf", "data_aware_declare", "declare", 
      "fields", "prop", "prop_within_dijunction", "atom", "rel", "declare_arguments", 
      "declare_act_target", "no_preliminary_fill", "act_for_attributes", 
      "no_cream_off", "with_data", "with_missing", "no_stats", "var"
    },
    std::vector<std::string>{
      "", "'benchmarking-log'", "'load'", "'as'", "'display'", "'for'", 
      "'list'", "'droplog'", "'dump-env'", "'in'", "'log'", "';'", "':'", 
      "'='", "'model-check'", "'using'", "'over'", "'plan'", "'with'", "'operators'", 
      "'multithreaded'", "'model-log'", "'from-automata-in'", "'sample-to'", 
      "'min-length'", "'max-length'", "'n-traces'", "'sample'", "'serialize-dot-to'", 
      "'file'", "'declare'", "'template'", "'logtop'", "'atomize!'", "'label'", 
      "'maximum'", "'strlen'", "'strategy'", "'grounding!'", "'query-plan'", 
      "'queryplan'", "':='", "'args'", "'=>'", "'('", "')'", "'&Ft'", "'&XGt'", 
      "'&Gt'", "','", "'where'", "'||'", "'true'", "'&&'", "'<'", "'<='", 
      "'>'", "'>='", "'!='", "'#'", "'no'", "'preliminary'", "'fill'", "'act'", 
      "'attributes'", "'cream'", "'off'", "'data'", "'missing'", "'stats'", 
      "'var'", "'ACTTABLE'", "'with-alignment-strategy'", "'COUNTTABLE'", 
      "'ATTRIBUTETABLE'", "'ACTIVITYLABEL'", "'LOGS'", "'ATTRIBUTES'", "'activation'", 
      "'target'", "'INIT'", "'END'", "'EXISTS'", "'ABSENCE'", "'NEXT'", 
      "'OR'", "'AND'", "'FIRST'", "'LAST'", "'IF'", "'THEN'", "'ELSE'", 
      "'U'", "'G'", "'F'", "'auto-timed'", "'{'", "'}'", "'PRESERVE'", "'t'", 
      "'THETA'", "'L'", "'INV'", "'R'", "'M'", "'~'", "'HRF'", "'TAB'", 
      "'XES'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "ACT_TABLE", "WITH_ALIGNMENT_STRATEGY", "CNT_TABLE", "ATT_TABLE", 
      "ACTIVITYLABEL", "LOGS", "ATT", "ACTIVATION", "TARGET", "INIT", "END", 
      "EXISTS", "ABSENCE", "NEXT", "OR", "AND", "FIRST", "LAST", "IF", "THEN", 
      "ELSE", "UNTIL", "BOX", "DIAMOND", "AUTO_TIMED", "LPAREN", "RPAREN", 
      "PRESERVE", "TIMED", "THETA", "LEFT", "INV", "RIGHT", "MIDDLE", "NEGATED", 
      "HRF", "TAB", "XES", "LABEL", "INTNUMBER", "NUMBER", "STRING", "SPACE", 
      "COMMENT", "LINE_COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,115,570,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,1,0,1,0,1,0,1,0,1,0,1,0,1,0,3,0,80,8,0,1,1,1,1,1,1,1,2,1,2,1,2,1,2,
  	1,2,3,2,90,8,2,1,2,1,2,3,2,94,8,2,1,2,3,2,97,8,2,1,2,3,2,100,8,2,1,2,
  	1,2,1,2,1,3,1,3,1,3,1,3,1,3,3,3,110,8,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,
  	3,3,3,120,8,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,5,5,134,
  	8,5,10,5,12,5,137,9,5,1,5,1,5,1,5,1,6,1,6,1,6,5,6,145,8,6,10,6,12,6,148,
  	9,6,1,7,1,7,3,7,152,8,7,1,8,1,8,5,8,156,8,8,10,8,12,8,159,9,8,1,8,1,8,
  	1,9,1,9,1,9,1,9,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,
  	1,10,3,10,179,8,10,1,10,1,10,3,10,183,8,10,1,10,3,10,186,8,10,1,10,3,
  	10,189,8,10,1,10,3,10,192,8,10,1,11,1,11,1,11,1,11,1,11,1,11,1,11,1,11,
  	1,11,1,11,1,11,1,11,1,11,1,11,1,11,3,11,209,8,11,3,11,211,8,11,1,11,1,
  	11,3,11,215,8,11,1,11,1,11,3,11,219,8,11,1,11,3,11,222,8,11,1,11,3,11,
  	225,8,11,1,11,3,11,228,8,11,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,3,
  	12,238,8,12,1,13,1,13,1,13,1,13,3,13,244,8,13,1,13,1,13,1,13,3,13,249,
  	8,13,1,13,1,13,1,13,3,13,254,8,13,1,14,1,14,1,14,1,14,3,14,260,8,14,1,
  	14,3,14,263,8,14,1,14,3,14,266,8,14,1,14,3,14,269,8,14,1,15,1,15,1,15,
  	1,16,3,16,275,8,16,1,16,1,16,1,16,1,16,4,16,281,8,16,11,16,12,16,282,
  	1,16,1,16,1,17,1,17,1,17,3,17,290,8,17,1,17,1,17,1,17,1,18,1,18,1,18,
  	1,19,1,19,1,19,3,19,301,8,19,1,19,3,19,304,8,19,1,19,3,19,307,8,19,1,
  	19,1,19,3,19,311,8,19,1,19,3,19,314,8,19,1,19,3,19,317,8,19,1,19,1,19,
  	3,19,321,8,19,1,19,1,19,3,19,325,8,19,1,19,1,19,3,19,329,8,19,1,19,1,
  	19,3,19,333,8,19,1,19,3,19,336,8,19,1,19,3,19,339,8,19,1,19,1,19,1,19,
  	3,19,344,8,19,1,19,3,19,347,8,19,1,19,3,19,350,8,19,1,19,1,19,1,19,1,
  	19,3,19,356,8,19,1,19,1,19,1,19,1,19,3,19,362,8,19,1,19,3,19,365,8,19,
  	1,19,1,19,1,19,1,19,1,19,3,19,372,8,19,1,19,1,19,1,19,3,19,377,8,19,1,
  	19,1,19,1,19,3,19,382,8,19,1,19,1,19,3,19,386,8,19,1,19,1,19,1,19,1,19,
  	3,19,392,8,19,1,19,1,19,1,19,3,19,397,8,19,1,19,3,19,400,8,19,1,19,3,
  	19,403,8,19,1,19,1,19,1,19,1,19,3,19,409,8,19,1,19,3,19,412,8,19,1,19,
  	3,19,415,8,19,1,19,1,19,1,19,1,19,3,19,421,8,19,1,19,3,19,424,8,19,1,
  	19,3,19,427,8,19,1,19,1,19,1,19,1,19,3,19,433,8,19,1,19,3,19,436,8,19,
  	1,19,3,19,439,8,19,1,19,1,19,1,19,1,19,3,19,445,8,19,1,19,3,19,448,8,
  	19,1,19,1,19,1,19,1,19,3,19,454,8,19,1,19,3,19,457,8,19,1,19,1,19,1,19,
  	1,19,3,19,463,8,19,1,19,3,19,466,8,19,1,19,5,19,469,8,19,10,19,12,19,
  	472,9,19,1,20,5,20,475,8,20,10,20,12,20,478,9,20,1,21,1,21,1,21,1,21,
  	1,21,4,21,485,8,21,11,21,12,21,486,1,21,1,21,1,21,1,21,3,21,493,8,21,
  	1,21,1,21,1,21,1,21,1,21,1,21,1,21,3,21,502,8,21,1,22,1,22,1,22,1,22,
  	1,23,1,23,1,23,1,23,1,23,1,23,3,23,514,8,23,1,24,1,24,1,24,1,24,1,24,
  	3,24,521,8,24,1,25,3,25,524,8,25,1,25,1,25,1,25,1,25,1,25,3,25,531,8,
  	25,1,26,1,26,1,26,1,26,1,26,1,26,3,26,539,8,26,1,27,1,27,1,27,1,28,1,
  	28,1,29,1,29,1,29,1,29,1,30,1,30,1,30,1,30,1,31,1,31,1,31,1,31,1,32,1,
  	32,1,32,1,33,1,33,1,33,1,34,1,34,1,34,1,35,1,35,1,35,1,35,0,1,38,36,0,
  	2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,
  	52,54,56,58,60,62,64,66,68,70,0,4,1,0,75,77,1,0,107,108,1,0,111,112,1,
  	0,78,79,650,0,79,1,0,0,0,2,81,1,0,0,0,4,84,1,0,0,0,6,119,1,0,0,0,8,121,
  	1,0,0,0,10,128,1,0,0,0,12,141,1,0,0,0,14,149,1,0,0,0,16,153,1,0,0,0,18,
  	162,1,0,0,0,20,166,1,0,0,0,22,193,1,0,0,0,24,237,1,0,0,0,26,239,1,0,0,
  	0,28,255,1,0,0,0,30,270,1,0,0,0,32,274,1,0,0,0,34,286,1,0,0,0,36,294,
  	1,0,0,0,38,391,1,0,0,0,40,476,1,0,0,0,42,501,1,0,0,0,44,503,1,0,0,0,46,
  	513,1,0,0,0,48,520,1,0,0,0,50,523,1,0,0,0,52,538,1,0,0,0,54,540,1,0,0,
  	0,56,543,1,0,0,0,58,545,1,0,0,0,60,549,1,0,0,0,62,553,1,0,0,0,64,557,
  	1,0,0,0,66,560,1,0,0,0,68,563,1,0,0,0,70,566,1,0,0,0,72,80,3,4,2,0,73,
  	80,3,6,3,0,74,80,3,20,10,0,75,80,3,32,16,0,76,80,3,2,1,0,77,80,3,8,4,
  	0,78,80,3,22,11,0,79,72,1,0,0,0,79,73,1,0,0,0,79,74,1,0,0,0,79,75,1,0,
  	0,0,79,76,1,0,0,0,79,77,1,0,0,0,79,78,1,0,0,0,80,1,1,0,0,0,81,82,5,1,
  	0,0,82,83,5,112,0,0,83,3,1,0,0,0,84,89,5,2,0,0,85,90,5,106,0,0,86,90,
  	5,107,0,0,87,90,5,108,0,0,88,90,3,10,5,0,89,85,1,0,0,0,89,86,1,0,0,0,
  	89,87,1,0,0,0,89,88,1,0,0,0,90,91,1,0,0,0,91,93,5,112,0,0,92,94,3,64,
  	32,0,93,92,1,0,0,0,93,94,1,0,0,0,94,96,1,0,0,0,95,97,3,68,34,0,96,95,
  	1,0,0,0,96,97,1,0,0,0,97,99,1,0,0,0,98,100,3,66,33,0,99,98,1,0,0,0,99,
  	100,1,0,0,0,100,101,1,0,0,0,101,102,5,3,0,0,102,103,5,112,0,0,103,5,1,
  	0,0,0,104,109,5,4,0,0,105,110,5,71,0,0,106,110,5,73,0,0,107,108,5,74,
  	0,0,108,110,5,112,0,0,109,105,1,0,0,0,109,106,1,0,0,0,109,107,1,0,0,0,
  	110,111,1,0,0,0,111,112,5,5,0,0,112,120,5,112,0,0,113,114,5,6,0,0,114,
  	115,7,0,0,0,115,116,5,5,0,0,116,120,5,112,0,0,117,118,5,7,0,0,118,120,
  	5,112,0,0,119,104,1,0,0,0,119,113,1,0,0,0,119,117,1,0,0,0,120,7,1,0,0,
  	0,121,122,5,8,0,0,122,123,5,112,0,0,123,124,5,3,0,0,124,125,7,1,0,0,125,
  	126,5,9,0,0,126,127,5,112,0,0,127,9,1,0,0,0,128,129,5,10,0,0,129,135,
  	5,96,0,0,130,131,3,12,6,0,131,132,5,11,0,0,132,134,1,0,0,0,133,130,1,
  	0,0,0,134,137,1,0,0,0,135,133,1,0,0,0,135,136,1,0,0,0,136,138,1,0,0,0,
  	137,135,1,0,0,0,138,139,3,12,6,0,139,140,5,97,0,0,140,11,1,0,0,0,141,
  	142,3,16,8,0,142,146,5,12,0,0,143,145,3,14,7,0,144,143,1,0,0,0,145,148,
  	1,0,0,0,146,144,1,0,0,0,146,147,1,0,0,0,147,13,1,0,0,0,148,146,1,0,0,
  	0,149,151,5,109,0,0,150,152,3,16,8,0,151,150,1,0,0,0,151,152,1,0,0,0,
  	152,15,1,0,0,0,153,157,5,96,0,0,154,156,3,18,9,0,155,154,1,0,0,0,156,
  	159,1,0,0,0,157,155,1,0,0,0,157,158,1,0,0,0,158,160,1,0,0,0,159,157,1,
  	0,0,0,160,161,5,97,0,0,161,17,1,0,0,0,162,163,3,70,35,0,163,164,5,13,
  	0,0,164,165,7,2,0,0,165,19,1,0,0,0,166,167,5,14,0,0,167,168,3,24,12,0,
  	168,169,5,15,0,0,169,170,5,112,0,0,170,171,5,16,0,0,171,172,5,112,0,0,
  	172,173,5,17,0,0,173,174,5,112,0,0,174,178,1,0,0,0,175,176,5,18,0,0,176,
  	177,5,19,0,0,177,179,5,112,0,0,178,175,1,0,0,0,178,179,1,0,0,0,179,182,
  	1,0,0,0,180,181,5,20,0,0,181,183,5,110,0,0,182,180,1,0,0,0,182,183,1,
  	0,0,0,183,185,1,0,0,0,184,186,3,30,15,0,185,184,1,0,0,0,185,186,1,0,0,
  	0,186,188,1,0,0,0,187,189,3,26,13,0,188,187,1,0,0,0,188,189,1,0,0,0,189,
  	191,1,0,0,0,190,192,3,28,14,0,191,190,1,0,0,0,191,192,1,0,0,0,192,21,
  	1,0,0,0,193,194,5,21,0,0,194,195,3,24,12,0,195,196,5,22,0,0,196,210,5,
  	112,0,0,197,198,5,23,0,0,198,199,7,1,0,0,199,200,5,112,0,0,200,201,5,
  	24,0,0,201,202,5,110,0,0,202,203,5,25,0,0,203,204,5,110,0,0,204,205,5,
  	26,0,0,205,208,5,110,0,0,206,207,5,27,0,0,207,209,5,111,0,0,208,206,1,
  	0,0,0,208,209,1,0,0,0,209,211,1,0,0,0,210,197,1,0,0,0,210,211,1,0,0,0,
  	211,214,1,0,0,0,212,213,5,72,0,0,213,215,5,112,0,0,214,212,1,0,0,0,214,
  	215,1,0,0,0,215,218,1,0,0,0,216,217,5,28,0,0,217,219,5,112,0,0,218,216,
  	1,0,0,0,218,219,1,0,0,0,219,221,1,0,0,0,220,222,3,26,13,0,221,220,1,0,
  	0,0,221,222,1,0,0,0,222,224,1,0,0,0,223,225,3,28,14,0,224,223,1,0,0,0,
  	224,225,1,0,0,0,225,227,1,0,0,0,226,228,3,8,4,0,227,226,1,0,0,0,227,228,
  	1,0,0,0,228,23,1,0,0,0,229,230,5,29,0,0,230,238,5,112,0,0,231,232,5,30,
  	0,0,232,238,3,40,20,0,233,234,5,31,0,0,234,235,5,112,0,0,235,236,5,32,
  	0,0,236,238,5,110,0,0,237,229,1,0,0,0,237,231,1,0,0,0,237,233,1,0,0,0,
  	238,25,1,0,0,0,239,243,5,33,0,0,240,241,5,18,0,0,241,242,5,34,0,0,242,
  	244,5,112,0,0,243,240,1,0,0,0,243,244,1,0,0,0,244,248,1,0,0,0,245,246,
  	5,35,0,0,246,247,5,36,0,0,247,249,5,110,0,0,248,245,1,0,0,0,248,249,1,
  	0,0,0,249,253,1,0,0,0,250,251,5,18,0,0,251,252,5,37,0,0,252,254,5,112,
  	0,0,253,250,1,0,0,0,253,254,1,0,0,0,254,27,1,0,0,0,255,259,5,38,0,0,256,
  	257,5,18,0,0,257,258,5,37,0,0,258,260,5,112,0,0,259,256,1,0,0,0,259,260,
  	1,0,0,0,260,262,1,0,0,0,261,263,3,58,29,0,262,261,1,0,0,0,262,263,1,0,
  	0,0,263,265,1,0,0,0,264,266,3,60,30,0,265,264,1,0,0,0,265,266,1,0,0,0,
  	266,268,1,0,0,0,267,269,3,62,31,0,268,267,1,0,0,0,268,269,1,0,0,0,269,
  	29,1,0,0,0,270,271,5,4,0,0,271,272,5,39,0,0,272,31,1,0,0,0,273,275,5,
  	95,0,0,274,273,1,0,0,0,274,275,1,0,0,0,275,276,1,0,0,0,276,277,5,40,0,
  	0,277,278,5,112,0,0,278,280,5,96,0,0,279,281,3,34,17,0,280,279,1,0,0,
  	0,281,282,1,0,0,0,282,280,1,0,0,0,282,283,1,0,0,0,283,284,1,0,0,0,284,
  	285,5,97,0,0,285,33,1,0,0,0,286,287,5,31,0,0,287,289,5,112,0,0,288,290,
  	3,36,18,0,289,288,1,0,0,0,289,290,1,0,0,0,290,291,1,0,0,0,291,292,5,41,
  	0,0,292,293,3,38,19,0,293,35,1,0,0,0,294,295,5,42,0,0,295,296,5,110,0,
  	0,296,37,1,0,0,0,297,298,6,19,-1,0,298,300,5,80,0,0,299,301,5,99,0,0,
  	300,299,1,0,0,0,300,301,1,0,0,0,301,303,1,0,0,0,302,304,3,54,27,0,303,
  	302,1,0,0,0,303,304,1,0,0,0,304,306,1,0,0,0,305,307,3,56,28,0,306,305,
  	1,0,0,0,306,307,1,0,0,0,307,392,1,0,0,0,308,310,5,81,0,0,309,311,5,99,
  	0,0,310,309,1,0,0,0,310,311,1,0,0,0,311,313,1,0,0,0,312,314,3,54,27,0,
  	313,312,1,0,0,0,313,314,1,0,0,0,314,316,1,0,0,0,315,317,3,56,28,0,316,
  	315,1,0,0,0,316,317,1,0,0,0,317,392,1,0,0,0,318,320,5,88,0,0,319,321,
  	3,56,28,0,320,319,1,0,0,0,320,321,1,0,0,0,321,392,1,0,0,0,322,324,5,87,
  	0,0,323,325,3,56,28,0,324,323,1,0,0,0,324,325,1,0,0,0,325,392,1,0,0,0,
  	326,328,5,82,0,0,327,329,5,105,0,0,328,327,1,0,0,0,328,329,1,0,0,0,329,
  	330,1,0,0,0,330,332,5,110,0,0,331,333,5,99,0,0,332,331,1,0,0,0,332,333,
  	1,0,0,0,333,335,1,0,0,0,334,336,3,54,27,0,335,334,1,0,0,0,335,336,1,0,
  	0,0,336,338,1,0,0,0,337,339,3,56,28,0,338,337,1,0,0,0,338,339,1,0,0,0,
  	339,392,1,0,0,0,340,341,5,83,0,0,341,343,5,110,0,0,342,344,5,99,0,0,343,
  	342,1,0,0,0,343,344,1,0,0,0,344,346,1,0,0,0,345,347,3,54,27,0,346,345,
  	1,0,0,0,346,347,1,0,0,0,347,349,1,0,0,0,348,350,3,56,28,0,349,348,1,0,
  	0,0,349,350,1,0,0,0,350,392,1,0,0,0,351,352,5,84,0,0,352,392,3,38,19,
  	13,353,355,5,89,0,0,354,356,5,99,0,0,355,354,1,0,0,0,355,356,1,0,0,0,
  	356,357,1,0,0,0,357,358,3,38,19,0,358,359,5,90,0,0,359,361,3,38,19,0,
  	360,362,5,100,0,0,361,360,1,0,0,0,361,362,1,0,0,0,362,364,1,0,0,0,363,
  	365,5,102,0,0,364,363,1,0,0,0,364,365,1,0,0,0,365,366,1,0,0,0,366,367,
  	5,91,0,0,367,368,3,38,19,9,368,392,1,0,0,0,369,371,5,93,0,0,370,372,5,
  	99,0,0,371,370,1,0,0,0,371,372,1,0,0,0,372,373,1,0,0,0,373,392,3,38,19,
  	7,374,376,5,94,0,0,375,377,5,99,0,0,376,375,1,0,0,0,376,377,1,0,0,0,377,
  	378,1,0,0,0,378,392,3,38,19,6,379,381,5,105,0,0,380,382,5,99,0,0,381,
  	380,1,0,0,0,381,382,1,0,0,0,382,383,1,0,0,0,383,385,3,38,19,0,384,386,
  	5,98,0,0,385,384,1,0,0,0,385,386,1,0,0,0,386,392,1,0,0,0,387,388,5,44,
  	0,0,388,389,3,38,19,0,389,390,5,45,0,0,390,392,1,0,0,0,391,297,1,0,0,
  	0,391,308,1,0,0,0,391,318,1,0,0,0,391,322,1,0,0,0,391,326,1,0,0,0,391,
  	340,1,0,0,0,391,351,1,0,0,0,391,353,1,0,0,0,391,369,1,0,0,0,391,374,1,
  	0,0,0,391,379,1,0,0,0,391,387,1,0,0,0,392,470,1,0,0,0,393,394,10,12,0,
  	0,394,396,5,85,0,0,395,397,5,99,0,0,396,395,1,0,0,0,396,397,1,0,0,0,397,
  	399,1,0,0,0,398,400,5,100,0,0,399,398,1,0,0,0,399,400,1,0,0,0,400,402,
  	1,0,0,0,401,403,5,102,0,0,402,401,1,0,0,0,402,403,1,0,0,0,403,404,1,0,
  	0,0,404,469,3,38,19,12,405,406,10,11,0,0,406,408,5,86,0,0,407,409,5,99,
  	0,0,408,407,1,0,0,0,408,409,1,0,0,0,409,411,1,0,0,0,410,412,5,100,0,0,
  	411,410,1,0,0,0,411,412,1,0,0,0,412,414,1,0,0,0,413,415,5,102,0,0,414,
  	413,1,0,0,0,414,415,1,0,0,0,415,416,1,0,0,0,416,469,3,38,19,11,417,418,
  	10,10,0,0,418,420,5,43,0,0,419,421,5,99,0,0,420,419,1,0,0,0,420,421,1,
  	0,0,0,421,423,1,0,0,0,422,424,5,100,0,0,423,422,1,0,0,0,423,424,1,0,0,
  	0,424,426,1,0,0,0,425,427,5,102,0,0,426,425,1,0,0,0,426,427,1,0,0,0,427,
  	428,1,0,0,0,428,469,3,38,19,10,429,430,10,8,0,0,430,432,5,92,0,0,431,
  	433,5,99,0,0,432,431,1,0,0,0,432,433,1,0,0,0,433,435,1,0,0,0,434,436,
  	5,100,0,0,435,434,1,0,0,0,435,436,1,0,0,0,436,438,1,0,0,0,437,439,5,102,
  	0,0,438,437,1,0,0,0,438,439,1,0,0,0,439,440,1,0,0,0,440,469,3,38,19,8,
  	441,442,10,3,0,0,442,444,5,46,0,0,443,445,5,100,0,0,444,443,1,0,0,0,444,
  	445,1,0,0,0,445,447,1,0,0,0,446,448,5,102,0,0,447,446,1,0,0,0,447,448,
  	1,0,0,0,448,449,1,0,0,0,449,469,3,38,19,3,450,451,10,2,0,0,451,453,5,
  	47,0,0,452,454,5,100,0,0,453,452,1,0,0,0,453,454,1,0,0,0,454,456,1,0,
  	0,0,455,457,5,102,0,0,456,455,1,0,0,0,456,457,1,0,0,0,457,458,1,0,0,0,
  	458,469,3,38,19,2,459,460,10,1,0,0,460,462,5,48,0,0,461,463,5,100,0,0,
  	462,461,1,0,0,0,462,463,1,0,0,0,463,465,1,0,0,0,464,466,5,102,0,0,465,
  	464,1,0,0,0,465,466,1,0,0,0,466,467,1,0,0,0,467,469,3,38,19,1,468,393,
  	1,0,0,0,468,405,1,0,0,0,468,417,1,0,0,0,468,429,1,0,0,0,468,441,1,0,0,
  	0,468,450,1,0,0,0,468,459,1,0,0,0,469,472,1,0,0,0,470,468,1,0,0,0,470,
  	471,1,0,0,0,471,39,1,0,0,0,472,470,1,0,0,0,473,475,3,42,21,0,474,473,
  	1,0,0,0,475,478,1,0,0,0,476,474,1,0,0,0,476,477,1,0,0,0,477,41,1,0,0,
  	0,478,476,1,0,0,0,479,480,5,112,0,0,480,484,5,44,0,0,481,482,3,44,22,
  	0,482,483,5,49,0,0,483,485,1,0,0,0,484,481,1,0,0,0,485,486,1,0,0,0,486,
  	484,1,0,0,0,486,487,1,0,0,0,487,488,1,0,0,0,488,489,3,44,22,0,489,492,
  	5,45,0,0,490,491,5,50,0,0,491,493,3,46,23,0,492,490,1,0,0,0,492,493,1,
  	0,0,0,493,502,1,0,0,0,494,495,5,112,0,0,495,496,5,44,0,0,496,497,3,44,
  	22,0,497,498,5,49,0,0,498,499,5,110,0,0,499,500,5,45,0,0,500,502,1,0,
  	0,0,501,479,1,0,0,0,501,494,1,0,0,0,502,43,1,0,0,0,503,504,5,112,0,0,
  	504,505,5,49,0,0,505,506,3,46,23,0,506,45,1,0,0,0,507,508,3,48,24,0,508,
  	509,5,51,0,0,509,510,3,46,23,0,510,514,1,0,0,0,511,514,3,48,24,0,512,
  	514,5,52,0,0,513,507,1,0,0,0,513,511,1,0,0,0,513,512,1,0,0,0,514,47,1,
  	0,0,0,515,521,3,50,25,0,516,517,3,50,25,0,517,518,5,53,0,0,518,519,3,
  	48,24,0,519,521,1,0,0,0,520,515,1,0,0,0,520,516,1,0,0,0,521,49,1,0,0,
  	0,522,524,5,105,0,0,523,522,1,0,0,0,523,524,1,0,0,0,524,525,1,0,0,0,525,
  	526,3,70,35,0,526,530,3,52,26,0,527,531,5,111,0,0,528,531,5,112,0,0,529,
  	531,3,70,35,0,530,527,1,0,0,0,530,528,1,0,0,0,530,529,1,0,0,0,531,51,
  	1,0,0,0,532,539,5,54,0,0,533,539,5,55,0,0,534,539,5,56,0,0,535,539,5,
  	57,0,0,536,539,5,13,0,0,537,539,5,58,0,0,538,532,1,0,0,0,538,533,1,0,
  	0,0,538,534,1,0,0,0,538,535,1,0,0,0,538,536,1,0,0,0,538,537,1,0,0,0,539,
  	53,1,0,0,0,540,541,5,59,0,0,541,542,5,110,0,0,542,55,1,0,0,0,543,544,
  	7,3,0,0,544,57,1,0,0,0,545,546,5,60,0,0,546,547,5,61,0,0,547,548,5,62,
  	0,0,548,59,1,0,0,0,549,550,5,60,0,0,550,551,5,63,0,0,551,552,5,64,0,0,
  	552,61,1,0,0,0,553,554,5,60,0,0,554,555,5,65,0,0,555,556,5,66,0,0,556,
  	63,1,0,0,0,557,558,5,18,0,0,558,559,5,67,0,0,559,65,1,0,0,0,560,561,5,
  	18,0,0,561,562,5,68,0,0,562,67,1,0,0,0,563,564,5,60,0,0,564,565,5,69,
  	0,0,565,69,1,0,0,0,566,567,5,70,0,0,567,568,5,112,0,0,568,71,1,0,0,0,
  	86,79,89,93,96,99,109,119,135,146,151,157,178,182,185,188,191,208,210,
  	214,218,221,224,227,237,243,248,253,259,262,265,268,274,282,289,300,303,
  	306,310,313,316,320,324,328,332,335,338,343,346,349,355,361,364,371,376,
  	381,385,391,396,399,402,408,411,414,420,423,426,432,435,438,444,447,453,
  	456,462,465,468,470,476,486,492,501,513,520,523,530,538
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  knobabqueryParserStaticData = staticData.release();
}

}

KnoBABQueryParser::KnoBABQueryParser(TokenStream *input) : KnoBABQueryParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

KnoBABQueryParser::KnoBABQueryParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  KnoBABQueryParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *knobabqueryParserStaticData->atn, knobabqueryParserStaticData->decisionToDFA, knobabqueryParserStaticData->sharedContextCache, options);
}

KnoBABQueryParser::~KnoBABQueryParser() {
  delete _interpreter;
}

const atn::ATN& KnoBABQueryParser::getATN() const {
  return *knobabqueryParserStaticData->atn;
}

std::string KnoBABQueryParser::getGrammarFileName() const {
  return "KnoBABQuery.g4";
}

const std::vector<std::string>& KnoBABQueryParser::getRuleNames() const {
  return knobabqueryParserStaticData->ruleNames;
}

const dfa::Vocabulary& KnoBABQueryParser::getVocabulary() const {
  return knobabqueryParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView KnoBABQueryParser::getSerializedATN() const {
  return knobabqueryParserStaticData->serializedATN;
}


//----------------- QueriesContext ------------------------------------------------------------------

KnoBABQueryParser::QueriesContext::QueriesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KnoBABQueryParser::Load_data_queryContext* KnoBABQueryParser::QueriesContext::load_data_query() {
  return getRuleContext<KnoBABQueryParser::Load_data_queryContext>(0);
}

KnoBABQueryParser::Display_dataContext* KnoBABQueryParser::QueriesContext::display_data() {
  return getRuleContext<KnoBABQueryParser::Display_dataContext>(0);
}

KnoBABQueryParser::Model_queryContext* KnoBABQueryParser::QueriesContext::model_query() {
  return getRuleContext<KnoBABQueryParser::Model_queryContext>(0);
}

KnoBABQueryParser::Query_planContext* KnoBABQueryParser::QueriesContext::query_plan() {
  return getRuleContext<KnoBABQueryParser::Query_planContext>(0);
}

KnoBABQueryParser::Set_benchmarking_fileContext* KnoBABQueryParser::QueriesContext::set_benchmarking_file() {
  return getRuleContext<KnoBABQueryParser::Set_benchmarking_fileContext>(0);
}

KnoBABQueryParser::Dump_logContext* KnoBABQueryParser::QueriesContext::dump_log() {
  return getRuleContext<KnoBABQueryParser::Dump_logContext>(0);
}

KnoBABQueryParser::With_modelContext* KnoBABQueryParser::QueriesContext::with_model() {
  return getRuleContext<KnoBABQueryParser::With_modelContext>(0);
}


size_t KnoBABQueryParser::QueriesContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleQueries;
}

void KnoBABQueryParser::QueriesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterQueries(this);
}

void KnoBABQueryParser::QueriesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitQueries(this);
}


std::any KnoBABQueryParser::QueriesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitQueries(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::QueriesContext* KnoBABQueryParser::queries() {
  QueriesContext *_localctx = _tracker.createInstance<QueriesContext>(_ctx, getState());
  enterRule(_localctx, 0, KnoBABQueryParser::RuleQueries);

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
      case KnoBABQueryParser::T__1: {
        enterOuterAlt(_localctx, 1);
        setState(72);
        load_data_query();
        break;
      }

      case KnoBABQueryParser::T__3:
      case KnoBABQueryParser::T__5:
      case KnoBABQueryParser::T__6: {
        enterOuterAlt(_localctx, 2);
        setState(73);
        display_data();
        break;
      }

      case KnoBABQueryParser::T__13: {
        enterOuterAlt(_localctx, 3);
        setState(74);
        model_query();
        break;
      }

      case KnoBABQueryParser::T__39:
      case KnoBABQueryParser::AUTO_TIMED: {
        enterOuterAlt(_localctx, 4);
        setState(75);
        query_plan();
        break;
      }

      case KnoBABQueryParser::T__0: {
        enterOuterAlt(_localctx, 5);
        setState(76);
        set_benchmarking_file();
        break;
      }

      case KnoBABQueryParser::T__7: {
        enterOuterAlt(_localctx, 6);
        setState(77);
        dump_log();
        break;
      }

      case KnoBABQueryParser::T__20: {
        enterOuterAlt(_localctx, 7);
        setState(78);
        with_model();
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

//----------------- Set_benchmarking_fileContext ------------------------------------------------------------------

KnoBABQueryParser::Set_benchmarking_fileContext::Set_benchmarking_fileContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::Set_benchmarking_fileContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}


size_t KnoBABQueryParser::Set_benchmarking_fileContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleSet_benchmarking_file;
}

void KnoBABQueryParser::Set_benchmarking_fileContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSet_benchmarking_file(this);
}

void KnoBABQueryParser::Set_benchmarking_fileContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSet_benchmarking_file(this);
}


std::any KnoBABQueryParser::Set_benchmarking_fileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitSet_benchmarking_file(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Set_benchmarking_fileContext* KnoBABQueryParser::set_benchmarking_file() {
  Set_benchmarking_fileContext *_localctx = _tracker.createInstance<Set_benchmarking_fileContext>(_ctx, getState());
  enterRule(_localctx, 2, KnoBABQueryParser::RuleSet_benchmarking_file);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(81);
    match(KnoBABQueryParser::T__0);
    setState(82);
    antlrcpp::downCast<Set_benchmarking_fileContext *>(_localctx)->file = match(KnoBABQueryParser::STRING);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Load_data_queryContext ------------------------------------------------------------------

KnoBABQueryParser::Load_data_queryContext::Load_data_queryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> KnoBABQueryParser::Load_data_queryContext::STRING() {
  return getTokens(KnoBABQueryParser::STRING);
}

tree::TerminalNode* KnoBABQueryParser::Load_data_queryContext::STRING(size_t i) {
  return getToken(KnoBABQueryParser::STRING, i);
}

tree::TerminalNode* KnoBABQueryParser::Load_data_queryContext::HRF() {
  return getToken(KnoBABQueryParser::HRF, 0);
}

tree::TerminalNode* KnoBABQueryParser::Load_data_queryContext::TAB() {
  return getToken(KnoBABQueryParser::TAB, 0);
}

tree::TerminalNode* KnoBABQueryParser::Load_data_queryContext::XES() {
  return getToken(KnoBABQueryParser::XES, 0);
}

KnoBABQueryParser::LogContext* KnoBABQueryParser::Load_data_queryContext::log() {
  return getRuleContext<KnoBABQueryParser::LogContext>(0);
}

KnoBABQueryParser::With_dataContext* KnoBABQueryParser::Load_data_queryContext::with_data() {
  return getRuleContext<KnoBABQueryParser::With_dataContext>(0);
}

KnoBABQueryParser::No_statsContext* KnoBABQueryParser::Load_data_queryContext::no_stats() {
  return getRuleContext<KnoBABQueryParser::No_statsContext>(0);
}

KnoBABQueryParser::With_missingContext* KnoBABQueryParser::Load_data_queryContext::with_missing() {
  return getRuleContext<KnoBABQueryParser::With_missingContext>(0);
}


size_t KnoBABQueryParser::Load_data_queryContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleLoad_data_query;
}

void KnoBABQueryParser::Load_data_queryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLoad_data_query(this);
}

void KnoBABQueryParser::Load_data_queryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLoad_data_query(this);
}


std::any KnoBABQueryParser::Load_data_queryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitLoad_data_query(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Load_data_queryContext* KnoBABQueryParser::load_data_query() {
  Load_data_queryContext *_localctx = _tracker.createInstance<Load_data_queryContext>(_ctx, getState());
  enterRule(_localctx, 4, KnoBABQueryParser::RuleLoad_data_query);
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
    setState(84);
    match(KnoBABQueryParser::T__1);
    setState(89);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::HRF: {
        setState(85);
        match(KnoBABQueryParser::HRF);
        break;
      }

      case KnoBABQueryParser::TAB: {
        setState(86);
        match(KnoBABQueryParser::TAB);
        break;
      }

      case KnoBABQueryParser::XES: {
        setState(87);
        match(KnoBABQueryParser::XES);
        break;
      }

      case KnoBABQueryParser::T__9: {
        setState(88);
        log();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(91);
    antlrcpp::downCast<Load_data_queryContext *>(_localctx)->file = match(KnoBABQueryParser::STRING);
    setState(93);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      setState(92);
      with_data();
      break;
    }

    default:
      break;
    }
    setState(96);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__59) {
      setState(95);
      no_stats();
    }
    setState(99);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__17) {
      setState(98);
      with_missing();
    }
    setState(101);
    match(KnoBABQueryParser::T__2);
    setState(102);
    antlrcpp::downCast<Load_data_queryContext *>(_localctx)->env_name = match(KnoBABQueryParser::STRING);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Display_dataContext ------------------------------------------------------------------

KnoBABQueryParser::Display_dataContext::Display_dataContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::Display_dataContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleDisplay_data;
}

void KnoBABQueryParser::Display_dataContext::copyFrom(Display_dataContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- DisplayContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> KnoBABQueryParser::DisplayContext::STRING() {
  return getTokens(KnoBABQueryParser::STRING);
}

tree::TerminalNode* KnoBABQueryParser::DisplayContext::STRING(size_t i) {
  return getToken(KnoBABQueryParser::STRING, i);
}

tree::TerminalNode* KnoBABQueryParser::DisplayContext::ACT_TABLE() {
  return getToken(KnoBABQueryParser::ACT_TABLE, 0);
}

tree::TerminalNode* KnoBABQueryParser::DisplayContext::CNT_TABLE() {
  return getToken(KnoBABQueryParser::CNT_TABLE, 0);
}

tree::TerminalNode* KnoBABQueryParser::DisplayContext::ATT_TABLE() {
  return getToken(KnoBABQueryParser::ATT_TABLE, 0);
}

KnoBABQueryParser::DisplayContext::DisplayContext(Display_dataContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::DisplayContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDisplay(this);
}
void KnoBABQueryParser::DisplayContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDisplay(this);
}

std::any KnoBABQueryParser::DisplayContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDisplay(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ListContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::ListContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}

tree::TerminalNode* KnoBABQueryParser::ListContext::ATT() {
  return getToken(KnoBABQueryParser::ATT, 0);
}

tree::TerminalNode* KnoBABQueryParser::ListContext::ACTIVITYLABEL() {
  return getToken(KnoBABQueryParser::ACTIVITYLABEL, 0);
}

tree::TerminalNode* KnoBABQueryParser::ListContext::LOGS() {
  return getToken(KnoBABQueryParser::LOGS, 0);
}

KnoBABQueryParser::ListContext::ListContext(Display_dataContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::ListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterList(this);
}
void KnoBABQueryParser::ListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitList(this);
}

std::any KnoBABQueryParser::ListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitList(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DroplogContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::DroplogContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}

KnoBABQueryParser::DroplogContext::DroplogContext(Display_dataContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::DroplogContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDroplog(this);
}
void KnoBABQueryParser::DroplogContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDroplog(this);
}

std::any KnoBABQueryParser::DroplogContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDroplog(this);
  else
    return visitor->visitChildren(this);
}
KnoBABQueryParser::Display_dataContext* KnoBABQueryParser::display_data() {
  Display_dataContext *_localctx = _tracker.createInstance<Display_dataContext>(_ctx, getState());
  enterRule(_localctx, 6, KnoBABQueryParser::RuleDisplay_data);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(119);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::T__3: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::DisplayContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(104);
        match(KnoBABQueryParser::T__3);
        setState(109);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case KnoBABQueryParser::ACT_TABLE: {
            setState(105);
            match(KnoBABQueryParser::ACT_TABLE);
            break;
          }

          case KnoBABQueryParser::CNT_TABLE: {
            setState(106);
            match(KnoBABQueryParser::CNT_TABLE);
            break;
          }

          case KnoBABQueryParser::ATT_TABLE: {
            setState(107);
            match(KnoBABQueryParser::ATT_TABLE);
            setState(108);
            antlrcpp::downCast<DisplayContext *>(_localctx)->attr = match(KnoBABQueryParser::STRING);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(111);
        match(KnoBABQueryParser::T__4);
        setState(112);
        match(KnoBABQueryParser::STRING);
        break;
      }

      case KnoBABQueryParser::T__5: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::ListContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(113);
        match(KnoBABQueryParser::T__5);
        setState(114);
        _la = _input->LA(1);
        if (!((((_la - 75) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 75)) & 7) != 0)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(115);
        match(KnoBABQueryParser::T__4);
        setState(116);
        match(KnoBABQueryParser::STRING);
        break;
      }

      case KnoBABQueryParser::T__6: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::DroplogContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(117);
        match(KnoBABQueryParser::T__6);
        setState(118);
        antlrcpp::downCast<DroplogContext *>(_localctx)->env_name = match(KnoBABQueryParser::STRING);
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

//----------------- Dump_logContext ------------------------------------------------------------------

KnoBABQueryParser::Dump_logContext::Dump_logContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> KnoBABQueryParser::Dump_logContext::STRING() {
  return getTokens(KnoBABQueryParser::STRING);
}

tree::TerminalNode* KnoBABQueryParser::Dump_logContext::STRING(size_t i) {
  return getToken(KnoBABQueryParser::STRING, i);
}

tree::TerminalNode* KnoBABQueryParser::Dump_logContext::TAB() {
  return getToken(KnoBABQueryParser::TAB, 0);
}

tree::TerminalNode* KnoBABQueryParser::Dump_logContext::XES() {
  return getToken(KnoBABQueryParser::XES, 0);
}


size_t KnoBABQueryParser::Dump_logContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleDump_log;
}

void KnoBABQueryParser::Dump_logContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDump_log(this);
}

void KnoBABQueryParser::Dump_logContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDump_log(this);
}


std::any KnoBABQueryParser::Dump_logContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDump_log(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Dump_logContext* KnoBABQueryParser::dump_log() {
  Dump_logContext *_localctx = _tracker.createInstance<Dump_logContext>(_ctx, getState());
  enterRule(_localctx, 8, KnoBABQueryParser::RuleDump_log);
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
    setState(121);
    match(KnoBABQueryParser::T__7);
    setState(122);
    antlrcpp::downCast<Dump_logContext *>(_localctx)->env = match(KnoBABQueryParser::STRING);
    setState(123);
    match(KnoBABQueryParser::T__2);
    setState(124);
    _la = _input->LA(1);
    if (!(_la == KnoBABQueryParser::TAB

    || _la == KnoBABQueryParser::XES)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(125);
    match(KnoBABQueryParser::T__8);
    setState(126);
    antlrcpp::downCast<Dump_logContext *>(_localctx)->file = match(KnoBABQueryParser::STRING);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogContext ------------------------------------------------------------------

KnoBABQueryParser::LogContext::LogContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::LogContext::LPAREN() {
  return getToken(KnoBABQueryParser::LPAREN, 0);
}

std::vector<KnoBABQueryParser::TraceContext *> KnoBABQueryParser::LogContext::trace() {
  return getRuleContexts<KnoBABQueryParser::TraceContext>();
}

KnoBABQueryParser::TraceContext* KnoBABQueryParser::LogContext::trace(size_t i) {
  return getRuleContext<KnoBABQueryParser::TraceContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::LogContext::RPAREN() {
  return getToken(KnoBABQueryParser::RPAREN, 0);
}


size_t KnoBABQueryParser::LogContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleLog;
}

void KnoBABQueryParser::LogContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLog(this);
}

void KnoBABQueryParser::LogContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLog(this);
}


std::any KnoBABQueryParser::LogContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitLog(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::LogContext* KnoBABQueryParser::log() {
  LogContext *_localctx = _tracker.createInstance<LogContext>(_ctx, getState());
  enterRule(_localctx, 10, KnoBABQueryParser::RuleLog);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(128);
    match(KnoBABQueryParser::T__9);
    setState(129);
    match(KnoBABQueryParser::LPAREN);
    setState(135);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(130);
        trace();
        setState(131);
        match(KnoBABQueryParser::T__10); 
      }
      setState(137);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    }
    setState(138);
    trace();
    setState(139);
    match(KnoBABQueryParser::RPAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TraceContext ------------------------------------------------------------------

KnoBABQueryParser::TraceContext::TraceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KnoBABQueryParser::Data_partContext* KnoBABQueryParser::TraceContext::data_part() {
  return getRuleContext<KnoBABQueryParser::Data_partContext>(0);
}

std::vector<KnoBABQueryParser::EventContext *> KnoBABQueryParser::TraceContext::event() {
  return getRuleContexts<KnoBABQueryParser::EventContext>();
}

KnoBABQueryParser::EventContext* KnoBABQueryParser::TraceContext::event(size_t i) {
  return getRuleContext<KnoBABQueryParser::EventContext>(i);
}


size_t KnoBABQueryParser::TraceContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleTrace;
}

void KnoBABQueryParser::TraceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTrace(this);
}

void KnoBABQueryParser::TraceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTrace(this);
}


std::any KnoBABQueryParser::TraceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitTrace(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::TraceContext* KnoBABQueryParser::trace() {
  TraceContext *_localctx = _tracker.createInstance<TraceContext>(_ctx, getState());
  enterRule(_localctx, 12, KnoBABQueryParser::RuleTrace);
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
    setState(141);
    data_part();
    setState(142);
    match(KnoBABQueryParser::T__11);
    setState(146);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == KnoBABQueryParser::LABEL) {
      setState(143);
      event();
      setState(148);
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

//----------------- EventContext ------------------------------------------------------------------

KnoBABQueryParser::EventContext::EventContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::EventContext::LABEL() {
  return getToken(KnoBABQueryParser::LABEL, 0);
}

KnoBABQueryParser::Data_partContext* KnoBABQueryParser::EventContext::data_part() {
  return getRuleContext<KnoBABQueryParser::Data_partContext>(0);
}


size_t KnoBABQueryParser::EventContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleEvent;
}

void KnoBABQueryParser::EventContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEvent(this);
}

void KnoBABQueryParser::EventContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEvent(this);
}


std::any KnoBABQueryParser::EventContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitEvent(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::EventContext* KnoBABQueryParser::event() {
  EventContext *_localctx = _tracker.createInstance<EventContext>(_ctx, getState());
  enterRule(_localctx, 14, KnoBABQueryParser::RuleEvent);
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
    setState(149);
    match(KnoBABQueryParser::LABEL);
    setState(151);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::LPAREN) {
      setState(150);
      data_part();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Data_partContext ------------------------------------------------------------------

KnoBABQueryParser::Data_partContext::Data_partContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::Data_partContext::LPAREN() {
  return getToken(KnoBABQueryParser::LPAREN, 0);
}

tree::TerminalNode* KnoBABQueryParser::Data_partContext::RPAREN() {
  return getToken(KnoBABQueryParser::RPAREN, 0);
}

std::vector<KnoBABQueryParser::FieldContext *> KnoBABQueryParser::Data_partContext::field() {
  return getRuleContexts<KnoBABQueryParser::FieldContext>();
}

KnoBABQueryParser::FieldContext* KnoBABQueryParser::Data_partContext::field(size_t i) {
  return getRuleContext<KnoBABQueryParser::FieldContext>(i);
}


size_t KnoBABQueryParser::Data_partContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleData_part;
}

void KnoBABQueryParser::Data_partContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterData_part(this);
}

void KnoBABQueryParser::Data_partContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitData_part(this);
}


std::any KnoBABQueryParser::Data_partContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitData_part(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Data_partContext* KnoBABQueryParser::data_part() {
  Data_partContext *_localctx = _tracker.createInstance<Data_partContext>(_ctx, getState());
  enterRule(_localctx, 16, KnoBABQueryParser::RuleData_part);
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
    setState(153);
    match(KnoBABQueryParser::LPAREN);
    setState(157);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == KnoBABQueryParser::T__69) {
      setState(154);
      field();
      setState(159);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(160);
    match(KnoBABQueryParser::RPAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FieldContext ------------------------------------------------------------------

KnoBABQueryParser::FieldContext::FieldContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KnoBABQueryParser::VarContext* KnoBABQueryParser::FieldContext::var() {
  return getRuleContext<KnoBABQueryParser::VarContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::FieldContext::NUMBER() {
  return getToken(KnoBABQueryParser::NUMBER, 0);
}

tree::TerminalNode* KnoBABQueryParser::FieldContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}


size_t KnoBABQueryParser::FieldContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleField;
}

void KnoBABQueryParser::FieldContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterField(this);
}

void KnoBABQueryParser::FieldContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitField(this);
}


std::any KnoBABQueryParser::FieldContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitField(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::FieldContext* KnoBABQueryParser::field() {
  FieldContext *_localctx = _tracker.createInstance<FieldContext>(_ctx, getState());
  enterRule(_localctx, 18, KnoBABQueryParser::RuleField);
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
    setState(162);
    var();
    setState(163);
    match(KnoBABQueryParser::T__12);
    setState(164);
    _la = _input->LA(1);
    if (!(_la == KnoBABQueryParser::NUMBER

    || _la == KnoBABQueryParser::STRING)) {
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

//----------------- Model_queryContext ------------------------------------------------------------------

KnoBABQueryParser::Model_queryContext::Model_queryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KnoBABQueryParser::ModelContext* KnoBABQueryParser::Model_queryContext::model() {
  return getRuleContext<KnoBABQueryParser::ModelContext>(0);
}

std::vector<tree::TerminalNode *> KnoBABQueryParser::Model_queryContext::STRING() {
  return getTokens(KnoBABQueryParser::STRING);
}

tree::TerminalNode* KnoBABQueryParser::Model_queryContext::STRING(size_t i) {
  return getToken(KnoBABQueryParser::STRING, i);
}

KnoBABQueryParser::Display_qpContext* KnoBABQueryParser::Model_queryContext::display_qp() {
  return getRuleContext<KnoBABQueryParser::Display_qpContext>(0);
}

KnoBABQueryParser::AtomizationContext* KnoBABQueryParser::Model_queryContext::atomization() {
  return getRuleContext<KnoBABQueryParser::AtomizationContext>(0);
}

KnoBABQueryParser::GroundingContext* KnoBABQueryParser::Model_queryContext::grounding() {
  return getRuleContext<KnoBABQueryParser::GroundingContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::Model_queryContext::INTNUMBER() {
  return getToken(KnoBABQueryParser::INTNUMBER, 0);
}


size_t KnoBABQueryParser::Model_queryContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleModel_query;
}

void KnoBABQueryParser::Model_queryContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterModel_query(this);
}

void KnoBABQueryParser::Model_queryContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitModel_query(this);
}


std::any KnoBABQueryParser::Model_queryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitModel_query(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Model_queryContext* KnoBABQueryParser::model_query() {
  Model_queryContext *_localctx = _tracker.createInstance<Model_queryContext>(_ctx, getState());
  enterRule(_localctx, 20, KnoBABQueryParser::RuleModel_query);
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
    setState(166);
    match(KnoBABQueryParser::T__13);
    setState(167);
    model();
    setState(168);
    match(KnoBABQueryParser::T__14);
    setState(169);
    antlrcpp::downCast<Model_queryContext *>(_localctx)->ensemble = match(KnoBABQueryParser::STRING);
    setState(170);
    match(KnoBABQueryParser::T__15);
    setState(171);
    antlrcpp::downCast<Model_queryContext *>(_localctx)->env = match(KnoBABQueryParser::STRING);

    setState(172);
    match(KnoBABQueryParser::T__16);
    setState(173);
    antlrcpp::downCast<Model_queryContext *>(_localctx)->plan = match(KnoBABQueryParser::STRING);
    setState(178);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__17) {
      setState(175);
      match(KnoBABQueryParser::T__17);
      setState(176);
      match(KnoBABQueryParser::T__18);
      setState(177);
      antlrcpp::downCast<Model_queryContext *>(_localctx)->operators = match(KnoBABQueryParser::STRING);
    }
    setState(182);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__19) {
      setState(180);
      match(KnoBABQueryParser::T__19);
      setState(181);
      antlrcpp::downCast<Model_queryContext *>(_localctx)->nothreads = match(KnoBABQueryParser::INTNUMBER);
    }
    setState(185);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__3) {
      setState(184);
      display_qp();
    }
    setState(188);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__32) {
      setState(187);
      atomization();
    }
    setState(191);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__37) {
      setState(190);
      grounding();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- With_modelContext ------------------------------------------------------------------

KnoBABQueryParser::With_modelContext::With_modelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KnoBABQueryParser::ModelContext* KnoBABQueryParser::With_modelContext::model() {
  return getRuleContext<KnoBABQueryParser::ModelContext>(0);
}

std::vector<tree::TerminalNode *> KnoBABQueryParser::With_modelContext::STRING() {
  return getTokens(KnoBABQueryParser::STRING);
}

tree::TerminalNode* KnoBABQueryParser::With_modelContext::STRING(size_t i) {
  return getToken(KnoBABQueryParser::STRING, i);
}

tree::TerminalNode* KnoBABQueryParser::With_modelContext::WITH_ALIGNMENT_STRATEGY() {
  return getToken(KnoBABQueryParser::WITH_ALIGNMENT_STRATEGY, 0);
}

KnoBABQueryParser::AtomizationContext* KnoBABQueryParser::With_modelContext::atomization() {
  return getRuleContext<KnoBABQueryParser::AtomizationContext>(0);
}

KnoBABQueryParser::GroundingContext* KnoBABQueryParser::With_modelContext::grounding() {
  return getRuleContext<KnoBABQueryParser::GroundingContext>(0);
}

KnoBABQueryParser::Dump_logContext* KnoBABQueryParser::With_modelContext::dump_log() {
  return getRuleContext<KnoBABQueryParser::Dump_logContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::With_modelContext::TAB() {
  return getToken(KnoBABQueryParser::TAB, 0);
}

tree::TerminalNode* KnoBABQueryParser::With_modelContext::XES() {
  return getToken(KnoBABQueryParser::XES, 0);
}

std::vector<tree::TerminalNode *> KnoBABQueryParser::With_modelContext::INTNUMBER() {
  return getTokens(KnoBABQueryParser::INTNUMBER);
}

tree::TerminalNode* KnoBABQueryParser::With_modelContext::INTNUMBER(size_t i) {
  return getToken(KnoBABQueryParser::INTNUMBER, i);
}

tree::TerminalNode* KnoBABQueryParser::With_modelContext::NUMBER() {
  return getToken(KnoBABQueryParser::NUMBER, 0);
}


size_t KnoBABQueryParser::With_modelContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleWith_model;
}

void KnoBABQueryParser::With_modelContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWith_model(this);
}

void KnoBABQueryParser::With_modelContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWith_model(this);
}


std::any KnoBABQueryParser::With_modelContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitWith_model(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::With_modelContext* KnoBABQueryParser::with_model() {
  With_modelContext *_localctx = _tracker.createInstance<With_modelContext>(_ctx, getState());
  enterRule(_localctx, 22, KnoBABQueryParser::RuleWith_model);
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
    setState(193);
    match(KnoBABQueryParser::T__20);
    setState(194);
    model();
    setState(195);
    match(KnoBABQueryParser::T__21);
    setState(196);
    antlrcpp::downCast<With_modelContext *>(_localctx)->cachePath = match(KnoBABQueryParser::STRING);
    setState(210);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__22) {
      setState(197);
      match(KnoBABQueryParser::T__22);
      setState(198);
      _la = _input->LA(1);
      if (!(_la == KnoBABQueryParser::TAB

      || _la == KnoBABQueryParser::XES)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(199);
      antlrcpp::downCast<With_modelContext *>(_localctx)->toFile = match(KnoBABQueryParser::STRING);
      setState(200);
      match(KnoBABQueryParser::T__23);
      setState(201);
      antlrcpp::downCast<With_modelContext *>(_localctx)->minL = match(KnoBABQueryParser::INTNUMBER);
      setState(202);
      match(KnoBABQueryParser::T__24);
      setState(203);
      antlrcpp::downCast<With_modelContext *>(_localctx)->maxL = match(KnoBABQueryParser::INTNUMBER);
      setState(204);
      match(KnoBABQueryParser::T__25);
      setState(205);
      antlrcpp::downCast<With_modelContext *>(_localctx)->logSize = match(KnoBABQueryParser::INTNUMBER);
      setState(208);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == KnoBABQueryParser::T__26) {
        setState(206);
        match(KnoBABQueryParser::T__26);
        setState(207);
        antlrcpp::downCast<With_modelContext *>(_localctx)->ratio = match(KnoBABQueryParser::NUMBER);
      }
    }
    setState(214);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::WITH_ALIGNMENT_STRATEGY) {
      setState(212);
      match(KnoBABQueryParser::WITH_ALIGNMENT_STRATEGY);
      setState(213);
      antlrcpp::downCast<With_modelContext *>(_localctx)->doAlign = match(KnoBABQueryParser::STRING);
    }
    setState(218);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__27) {
      setState(216);
      match(KnoBABQueryParser::T__27);
      setState(217);
      antlrcpp::downCast<With_modelContext *>(_localctx)->graphDot = match(KnoBABQueryParser::STRING);
    }
    setState(221);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__32) {
      setState(220);
      atomization();
    }
    setState(224);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__37) {
      setState(223);
      grounding();
    }
    setState(227);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__7) {
      setState(226);
      dump_log();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ModelContext ------------------------------------------------------------------

KnoBABQueryParser::ModelContext::ModelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::ModelContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleModel;
}

void KnoBABQueryParser::ModelContext::copyFrom(ModelContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- DeclaresContext ------------------------------------------------------------------

KnoBABQueryParser::Data_aware_declareContext* KnoBABQueryParser::DeclaresContext::data_aware_declare() {
  return getRuleContext<KnoBABQueryParser::Data_aware_declareContext>(0);
}

KnoBABQueryParser::DeclaresContext::DeclaresContext(ModelContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::DeclaresContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclares(this);
}
void KnoBABQueryParser::DeclaresContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclares(this);
}

std::any KnoBABQueryParser::DeclaresContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDeclares(this);
  else
    return visitor->visitChildren(this);
}
//----------------- File_modelContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::File_modelContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}

KnoBABQueryParser::File_modelContext::File_modelContext(ModelContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::File_modelContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFile_model(this);
}
void KnoBABQueryParser::File_modelContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFile_model(this);
}

std::any KnoBABQueryParser::File_modelContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitFile_model(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TopnContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::TopnContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}

tree::TerminalNode* KnoBABQueryParser::TopnContext::INTNUMBER() {
  return getToken(KnoBABQueryParser::INTNUMBER, 0);
}

KnoBABQueryParser::TopnContext::TopnContext(ModelContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::TopnContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTopn(this);
}
void KnoBABQueryParser::TopnContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTopn(this);
}

std::any KnoBABQueryParser::TopnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitTopn(this);
  else
    return visitor->visitChildren(this);
}
KnoBABQueryParser::ModelContext* KnoBABQueryParser::model() {
  ModelContext *_localctx = _tracker.createInstance<ModelContext>(_ctx, getState());
  enterRule(_localctx, 24, KnoBABQueryParser::RuleModel);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(237);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::T__28: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::File_modelContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(229);
        match(KnoBABQueryParser::T__28);
        setState(230);
        match(KnoBABQueryParser::STRING);
        break;
      }

      case KnoBABQueryParser::T__29: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::DeclaresContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(231);
        match(KnoBABQueryParser::T__29);
        setState(232);
        data_aware_declare();
        break;
      }

      case KnoBABQueryParser::T__30: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::TopnContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(233);
        match(KnoBABQueryParser::T__30);
        setState(234);
        match(KnoBABQueryParser::STRING);
        setState(235);
        match(KnoBABQueryParser::T__31);
        setState(236);
        match(KnoBABQueryParser::INTNUMBER);
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

//----------------- AtomizationContext ------------------------------------------------------------------

KnoBABQueryParser::AtomizationContext::AtomizationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> KnoBABQueryParser::AtomizationContext::STRING() {
  return getTokens(KnoBABQueryParser::STRING);
}

tree::TerminalNode* KnoBABQueryParser::AtomizationContext::STRING(size_t i) {
  return getToken(KnoBABQueryParser::STRING, i);
}

tree::TerminalNode* KnoBABQueryParser::AtomizationContext::INTNUMBER() {
  return getToken(KnoBABQueryParser::INTNUMBER, 0);
}


size_t KnoBABQueryParser::AtomizationContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleAtomization;
}

void KnoBABQueryParser::AtomizationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtomization(this);
}

void KnoBABQueryParser::AtomizationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtomization(this);
}


std::any KnoBABQueryParser::AtomizationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitAtomization(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::AtomizationContext* KnoBABQueryParser::atomization() {
  AtomizationContext *_localctx = _tracker.createInstance<AtomizationContext>(_ctx, getState());
  enterRule(_localctx, 26, KnoBABQueryParser::RuleAtomization);
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
    setState(239);
    match(KnoBABQueryParser::T__32);
    setState(243);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
    case 1: {
      setState(240);
      match(KnoBABQueryParser::T__17);
      setState(241);
      match(KnoBABQueryParser::T__33);
      setState(242);
      antlrcpp::downCast<AtomizationContext *>(_localctx)->label = match(KnoBABQueryParser::STRING);
      break;
    }

    default:
      break;
    }
    setState(248);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__34) {
      setState(245);
      match(KnoBABQueryParser::T__34);
      setState(246);
      match(KnoBABQueryParser::T__35);
      setState(247);
      antlrcpp::downCast<AtomizationContext *>(_localctx)->strlen = match(KnoBABQueryParser::INTNUMBER);
    }
    setState(253);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__17) {
      setState(250);
      match(KnoBABQueryParser::T__17);
      setState(251);
      match(KnoBABQueryParser::T__36);
      setState(252);
      antlrcpp::downCast<AtomizationContext *>(_localctx)->strategy = match(KnoBABQueryParser::STRING);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GroundingContext ------------------------------------------------------------------

KnoBABQueryParser::GroundingContext::GroundingContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KnoBABQueryParser::No_preliminary_fillContext* KnoBABQueryParser::GroundingContext::no_preliminary_fill() {
  return getRuleContext<KnoBABQueryParser::No_preliminary_fillContext>(0);
}

KnoBABQueryParser::Act_for_attributesContext* KnoBABQueryParser::GroundingContext::act_for_attributes() {
  return getRuleContext<KnoBABQueryParser::Act_for_attributesContext>(0);
}

KnoBABQueryParser::No_cream_offContext* KnoBABQueryParser::GroundingContext::no_cream_off() {
  return getRuleContext<KnoBABQueryParser::No_cream_offContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::GroundingContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}


size_t KnoBABQueryParser::GroundingContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleGrounding;
}

void KnoBABQueryParser::GroundingContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGrounding(this);
}

void KnoBABQueryParser::GroundingContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGrounding(this);
}


std::any KnoBABQueryParser::GroundingContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitGrounding(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::GroundingContext* KnoBABQueryParser::grounding() {
  GroundingContext *_localctx = _tracker.createInstance<GroundingContext>(_ctx, getState());
  enterRule(_localctx, 28, KnoBABQueryParser::RuleGrounding);
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
    setState(255);
    match(KnoBABQueryParser::T__37);
    setState(259);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__17) {
      setState(256);
      match(KnoBABQueryParser::T__17);
      setState(257);
      match(KnoBABQueryParser::T__36);
      setState(258);
      antlrcpp::downCast<GroundingContext *>(_localctx)->strategy = match(KnoBABQueryParser::STRING);
    }
    setState(262);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
    case 1: {
      setState(261);
      no_preliminary_fill();
      break;
    }

    default:
      break;
    }
    setState(265);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
    case 1: {
      setState(264);
      act_for_attributes();
      break;
    }

    default:
      break;
    }
    setState(268);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__59) {
      setState(267);
      no_cream_off();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Display_qpContext ------------------------------------------------------------------

KnoBABQueryParser::Display_qpContext::Display_qpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::Display_qpContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleDisplay_qp;
}

void KnoBABQueryParser::Display_qpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDisplay_qp(this);
}

void KnoBABQueryParser::Display_qpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDisplay_qp(this);
}


std::any KnoBABQueryParser::Display_qpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDisplay_qp(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Display_qpContext* KnoBABQueryParser::display_qp() {
  Display_qpContext *_localctx = _tracker.createInstance<Display_qpContext>(_ctx, getState());
  enterRule(_localctx, 30, KnoBABQueryParser::RuleDisplay_qp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(270);
    match(KnoBABQueryParser::T__3);
    setState(271);
    match(KnoBABQueryParser::T__38);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Query_planContext ------------------------------------------------------------------

KnoBABQueryParser::Query_planContext::Query_planContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::Query_planContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}

tree::TerminalNode* KnoBABQueryParser::Query_planContext::LPAREN() {
  return getToken(KnoBABQueryParser::LPAREN, 0);
}

tree::TerminalNode* KnoBABQueryParser::Query_planContext::RPAREN() {
  return getToken(KnoBABQueryParser::RPAREN, 0);
}

tree::TerminalNode* KnoBABQueryParser::Query_planContext::AUTO_TIMED() {
  return getToken(KnoBABQueryParser::AUTO_TIMED, 0);
}

std::vector<KnoBABQueryParser::Declare_syntaxContext *> KnoBABQueryParser::Query_planContext::declare_syntax() {
  return getRuleContexts<KnoBABQueryParser::Declare_syntaxContext>();
}

KnoBABQueryParser::Declare_syntaxContext* KnoBABQueryParser::Query_planContext::declare_syntax(size_t i) {
  return getRuleContext<KnoBABQueryParser::Declare_syntaxContext>(i);
}


size_t KnoBABQueryParser::Query_planContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleQuery_plan;
}

void KnoBABQueryParser::Query_planContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterQuery_plan(this);
}

void KnoBABQueryParser::Query_planContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitQuery_plan(this);
}


std::any KnoBABQueryParser::Query_planContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitQuery_plan(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Query_planContext* KnoBABQueryParser::query_plan() {
  Query_planContext *_localctx = _tracker.createInstance<Query_planContext>(_ctx, getState());
  enterRule(_localctx, 32, KnoBABQueryParser::RuleQuery_plan);
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
    setState(274);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::AUTO_TIMED) {
      setState(273);
      match(KnoBABQueryParser::AUTO_TIMED);
    }
    setState(276);
    match(KnoBABQueryParser::T__39);
    setState(277);
    match(KnoBABQueryParser::STRING);
    setState(278);
    match(KnoBABQueryParser::LPAREN);
    setState(280); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(279);
      declare_syntax();
      setState(282); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == KnoBABQueryParser::T__30);
    setState(284);
    match(KnoBABQueryParser::RPAREN);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Declare_syntaxContext ------------------------------------------------------------------

KnoBABQueryParser::Declare_syntaxContext::Declare_syntaxContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::Declare_syntaxContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::Declare_syntaxContext::ltlf() {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(0);
}

KnoBABQueryParser::Has_argsContext* KnoBABQueryParser::Declare_syntaxContext::has_args() {
  return getRuleContext<KnoBABQueryParser::Has_argsContext>(0);
}


size_t KnoBABQueryParser::Declare_syntaxContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleDeclare_syntax;
}

void KnoBABQueryParser::Declare_syntaxContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclare_syntax(this);
}

void KnoBABQueryParser::Declare_syntaxContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclare_syntax(this);
}


std::any KnoBABQueryParser::Declare_syntaxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDeclare_syntax(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Declare_syntaxContext* KnoBABQueryParser::declare_syntax() {
  Declare_syntaxContext *_localctx = _tracker.createInstance<Declare_syntaxContext>(_ctx, getState());
  enterRule(_localctx, 34, KnoBABQueryParser::RuleDeclare_syntax);
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
    setState(286);
    match(KnoBABQueryParser::T__30);
    setState(287);
    match(KnoBABQueryParser::STRING);
    setState(289);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__41) {
      setState(288);
      has_args();
    }
    setState(291);
    match(KnoBABQueryParser::T__40);
    setState(292);
    ltlf(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Has_argsContext ------------------------------------------------------------------

KnoBABQueryParser::Has_argsContext::Has_argsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::Has_argsContext::INTNUMBER() {
  return getToken(KnoBABQueryParser::INTNUMBER, 0);
}


size_t KnoBABQueryParser::Has_argsContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleHas_args;
}

void KnoBABQueryParser::Has_argsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHas_args(this);
}

void KnoBABQueryParser::Has_argsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHas_args(this);
}


std::any KnoBABQueryParser::Has_argsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitHas_args(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Has_argsContext* KnoBABQueryParser::has_args() {
  Has_argsContext *_localctx = _tracker.createInstance<Has_argsContext>(_ctx, getState());
  enterRule(_localctx, 36, KnoBABQueryParser::RuleHas_args);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(294);
    match(KnoBABQueryParser::T__41);
    setState(295);
    match(KnoBABQueryParser::INTNUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LtlfContext ------------------------------------------------------------------

KnoBABQueryParser::LtlfContext::LtlfContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::LtlfContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleLtlf;
}

void KnoBABQueryParser::LtlfContext::copyFrom(LtlfContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- NextContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::NextContext::NEXT() {
  return getToken(KnoBABQueryParser::NEXT, 0);
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::NextContext::ltlf() {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(0);
}

KnoBABQueryParser::NextContext::NextContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::NextContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNext(this);
}
void KnoBABQueryParser::NextContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNext(this);
}

std::any KnoBABQueryParser::NextContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitNext(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InitContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::InitContext::INIT() {
  return getToken(KnoBABQueryParser::INIT, 0);
}

tree::TerminalNode* KnoBABQueryParser::InitContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

KnoBABQueryParser::Declare_argumentsContext* KnoBABQueryParser::InitContext::declare_arguments() {
  return getRuleContext<KnoBABQueryParser::Declare_argumentsContext>(0);
}

KnoBABQueryParser::Declare_act_targetContext* KnoBABQueryParser::InitContext::declare_act_target() {
  return getRuleContext<KnoBABQueryParser::Declare_act_targetContext>(0);
}

KnoBABQueryParser::InitContext::InitContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::InitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInit(this);
}
void KnoBABQueryParser::InitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInit(this);
}

std::any KnoBABQueryParser::InitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitInit(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrContext ------------------------------------------------------------------

std::vector<KnoBABQueryParser::LtlfContext *> KnoBABQueryParser::OrContext::ltlf() {
  return getRuleContexts<KnoBABQueryParser::LtlfContext>();
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::OrContext::ltlf(size_t i) {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::OrContext::OR() {
  return getToken(KnoBABQueryParser::OR, 0);
}

tree::TerminalNode* KnoBABQueryParser::OrContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

tree::TerminalNode* KnoBABQueryParser::OrContext::THETA() {
  return getToken(KnoBABQueryParser::THETA, 0);
}

tree::TerminalNode* KnoBABQueryParser::OrContext::INV() {
  return getToken(KnoBABQueryParser::INV, 0);
}

KnoBABQueryParser::OrContext::OrContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::OrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOr(this);
}
void KnoBABQueryParser::OrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOr(this);
}

std::any KnoBABQueryParser::OrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitOr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LastContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::LastContext::LAST() {
  return getToken(KnoBABQueryParser::LAST, 0);
}

KnoBABQueryParser::Declare_act_targetContext* KnoBABQueryParser::LastContext::declare_act_target() {
  return getRuleContext<KnoBABQueryParser::Declare_act_targetContext>(0);
}

KnoBABQueryParser::LastContext::LastContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::LastContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLast(this);
}
void KnoBABQueryParser::LastContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLast(this);
}

std::any KnoBABQueryParser::LastContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitLast(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AbsenceContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::AbsenceContext::ABSENCE() {
  return getToken(KnoBABQueryParser::ABSENCE, 0);
}

tree::TerminalNode* KnoBABQueryParser::AbsenceContext::INTNUMBER() {
  return getToken(KnoBABQueryParser::INTNUMBER, 0);
}

tree::TerminalNode* KnoBABQueryParser::AbsenceContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

KnoBABQueryParser::Declare_argumentsContext* KnoBABQueryParser::AbsenceContext::declare_arguments() {
  return getRuleContext<KnoBABQueryParser::Declare_argumentsContext>(0);
}

KnoBABQueryParser::Declare_act_targetContext* KnoBABQueryParser::AbsenceContext::declare_act_target() {
  return getRuleContext<KnoBABQueryParser::Declare_act_targetContext>(0);
}

KnoBABQueryParser::AbsenceContext::AbsenceContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::AbsenceContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAbsence(this);
}
void KnoBABQueryParser::AbsenceContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAbsence(this);
}

std::any KnoBABQueryParser::AbsenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitAbsence(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BoxContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::BoxContext::BOX() {
  return getToken(KnoBABQueryParser::BOX, 0);
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::BoxContext::ltlf() {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::BoxContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

KnoBABQueryParser::BoxContext::BoxContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::BoxContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBox(this);
}
void KnoBABQueryParser::BoxContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBox(this);
}

std::any KnoBABQueryParser::BoxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitBox(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DiamondContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::DiamondContext::DIAMOND() {
  return getToken(KnoBABQueryParser::DIAMOND, 0);
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::DiamondContext::ltlf() {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::DiamondContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

KnoBABQueryParser::DiamondContext::DiamondContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::DiamondContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDiamond(this);
}
void KnoBABQueryParser::DiamondContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDiamond(this);
}

std::any KnoBABQueryParser::DiamondContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDiamond(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NotContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::NotContext::NEGATED() {
  return getToken(KnoBABQueryParser::NEGATED, 0);
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::NotContext::ltlf() {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::NotContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

tree::TerminalNode* KnoBABQueryParser::NotContext::PRESERVE() {
  return getToken(KnoBABQueryParser::PRESERVE, 0);
}

KnoBABQueryParser::NotContext::NotContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::NotContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNot(this);
}
void KnoBABQueryParser::NotContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNot(this);
}

std::any KnoBABQueryParser::NotContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitNot(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenContext ------------------------------------------------------------------

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::ParenContext::ltlf() {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(0);
}

KnoBABQueryParser::ParenContext::ParenContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::ParenContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParen(this);
}
void KnoBABQueryParser::ParenContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParen(this);
}

std::any KnoBABQueryParser::ParenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitParen(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_next_globallyContext ------------------------------------------------------------------

std::vector<KnoBABQueryParser::LtlfContext *> KnoBABQueryParser::And_next_globallyContext::ltlf() {
  return getRuleContexts<KnoBABQueryParser::LtlfContext>();
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::And_next_globallyContext::ltlf(size_t i) {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::And_next_globallyContext::THETA() {
  return getToken(KnoBABQueryParser::THETA, 0);
}

tree::TerminalNode* KnoBABQueryParser::And_next_globallyContext::INV() {
  return getToken(KnoBABQueryParser::INV, 0);
}

KnoBABQueryParser::And_next_globallyContext::And_next_globallyContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::And_next_globallyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnd_next_globally(this);
}
void KnoBABQueryParser::And_next_globallyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnd_next_globally(this);
}

std::any KnoBABQueryParser::And_next_globallyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitAnd_next_globally(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AndContext ------------------------------------------------------------------

std::vector<KnoBABQueryParser::LtlfContext *> KnoBABQueryParser::AndContext::ltlf() {
  return getRuleContexts<KnoBABQueryParser::LtlfContext>();
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::AndContext::ltlf(size_t i) {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::AndContext::AND() {
  return getToken(KnoBABQueryParser::AND, 0);
}

tree::TerminalNode* KnoBABQueryParser::AndContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

tree::TerminalNode* KnoBABQueryParser::AndContext::THETA() {
  return getToken(KnoBABQueryParser::THETA, 0);
}

tree::TerminalNode* KnoBABQueryParser::AndContext::INV() {
  return getToken(KnoBABQueryParser::INV, 0);
}

KnoBABQueryParser::AndContext::AndContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::AndContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnd(this);
}
void KnoBABQueryParser::AndContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnd(this);
}

std::any KnoBABQueryParser::AndContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitAnd(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ImplicationContext ------------------------------------------------------------------

std::vector<KnoBABQueryParser::LtlfContext *> KnoBABQueryParser::ImplicationContext::ltlf() {
  return getRuleContexts<KnoBABQueryParser::LtlfContext>();
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::ImplicationContext::ltlf(size_t i) {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::ImplicationContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

tree::TerminalNode* KnoBABQueryParser::ImplicationContext::THETA() {
  return getToken(KnoBABQueryParser::THETA, 0);
}

tree::TerminalNode* KnoBABQueryParser::ImplicationContext::INV() {
  return getToken(KnoBABQueryParser::INV, 0);
}

KnoBABQueryParser::ImplicationContext::ImplicationContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::ImplicationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterImplication(this);
}
void KnoBABQueryParser::ImplicationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitImplication(this);
}

std::any KnoBABQueryParser::ImplicationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitImplication(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_globallyContext ------------------------------------------------------------------

std::vector<KnoBABQueryParser::LtlfContext *> KnoBABQueryParser::And_globallyContext::ltlf() {
  return getRuleContexts<KnoBABQueryParser::LtlfContext>();
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::And_globallyContext::ltlf(size_t i) {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::And_globallyContext::THETA() {
  return getToken(KnoBABQueryParser::THETA, 0);
}

tree::TerminalNode* KnoBABQueryParser::And_globallyContext::INV() {
  return getToken(KnoBABQueryParser::INV, 0);
}

KnoBABQueryParser::And_globallyContext::And_globallyContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::And_globallyContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnd_globally(this);
}
void KnoBABQueryParser::And_globallyContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnd_globally(this);
}

std::any KnoBABQueryParser::And_globallyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitAnd_globally(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExistsContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::ExistsContext::EXISTS() {
  return getToken(KnoBABQueryParser::EXISTS, 0);
}

tree::TerminalNode* KnoBABQueryParser::ExistsContext::INTNUMBER() {
  return getToken(KnoBABQueryParser::INTNUMBER, 0);
}

tree::TerminalNode* KnoBABQueryParser::ExistsContext::NEGATED() {
  return getToken(KnoBABQueryParser::NEGATED, 0);
}

tree::TerminalNode* KnoBABQueryParser::ExistsContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

KnoBABQueryParser::Declare_argumentsContext* KnoBABQueryParser::ExistsContext::declare_arguments() {
  return getRuleContext<KnoBABQueryParser::Declare_argumentsContext>(0);
}

KnoBABQueryParser::Declare_act_targetContext* KnoBABQueryParser::ExistsContext::declare_act_target() {
  return getRuleContext<KnoBABQueryParser::Declare_act_targetContext>(0);
}

KnoBABQueryParser::ExistsContext::ExistsContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::ExistsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExists(this);
}
void KnoBABQueryParser::ExistsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExists(this);
}

std::any KnoBABQueryParser::ExistsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitExists(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EndContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::EndContext::END() {
  return getToken(KnoBABQueryParser::END, 0);
}

tree::TerminalNode* KnoBABQueryParser::EndContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

KnoBABQueryParser::Declare_argumentsContext* KnoBABQueryParser::EndContext::declare_arguments() {
  return getRuleContext<KnoBABQueryParser::Declare_argumentsContext>(0);
}

KnoBABQueryParser::Declare_act_targetContext* KnoBABQueryParser::EndContext::declare_act_target() {
  return getRuleContext<KnoBABQueryParser::Declare_act_targetContext>(0);
}

KnoBABQueryParser::EndContext::EndContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::EndContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEnd(this);
}
void KnoBABQueryParser::EndContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEnd(this);
}

std::any KnoBABQueryParser::EndContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitEnd(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UntilContext ------------------------------------------------------------------

std::vector<KnoBABQueryParser::LtlfContext *> KnoBABQueryParser::UntilContext::ltlf() {
  return getRuleContexts<KnoBABQueryParser::LtlfContext>();
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::UntilContext::ltlf(size_t i) {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::UntilContext::UNTIL() {
  return getToken(KnoBABQueryParser::UNTIL, 0);
}

tree::TerminalNode* KnoBABQueryParser::UntilContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

tree::TerminalNode* KnoBABQueryParser::UntilContext::THETA() {
  return getToken(KnoBABQueryParser::THETA, 0);
}

tree::TerminalNode* KnoBABQueryParser::UntilContext::INV() {
  return getToken(KnoBABQueryParser::INV, 0);
}

KnoBABQueryParser::UntilContext::UntilContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::UntilContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUntil(this);
}
void KnoBABQueryParser::UntilContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUntil(this);
}

std::any KnoBABQueryParser::UntilContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitUntil(this);
  else
    return visitor->visitChildren(this);
}
//----------------- And_futureContext ------------------------------------------------------------------

std::vector<KnoBABQueryParser::LtlfContext *> KnoBABQueryParser::And_futureContext::ltlf() {
  return getRuleContexts<KnoBABQueryParser::LtlfContext>();
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::And_futureContext::ltlf(size_t i) {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::And_futureContext::THETA() {
  return getToken(KnoBABQueryParser::THETA, 0);
}

tree::TerminalNode* KnoBABQueryParser::And_futureContext::INV() {
  return getToken(KnoBABQueryParser::INV, 0);
}

KnoBABQueryParser::And_futureContext::And_futureContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::And_futureContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAnd_future(this);
}
void KnoBABQueryParser::And_futureContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAnd_future(this);
}

std::any KnoBABQueryParser::And_futureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitAnd_future(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FirstContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::FirstContext::FIRST() {
  return getToken(KnoBABQueryParser::FIRST, 0);
}

KnoBABQueryParser::Declare_act_targetContext* KnoBABQueryParser::FirstContext::declare_act_target() {
  return getRuleContext<KnoBABQueryParser::Declare_act_targetContext>(0);
}

KnoBABQueryParser::FirstContext::FirstContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::FirstContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFirst(this);
}
void KnoBABQueryParser::FirstContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFirst(this);
}

std::any KnoBABQueryParser::FirstContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitFirst(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfteContext ------------------------------------------------------------------

tree::TerminalNode* KnoBABQueryParser::IfteContext::IF() {
  return getToken(KnoBABQueryParser::IF, 0);
}

std::vector<KnoBABQueryParser::LtlfContext *> KnoBABQueryParser::IfteContext::ltlf() {
  return getRuleContexts<KnoBABQueryParser::LtlfContext>();
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::IfteContext::ltlf(size_t i) {
  return getRuleContext<KnoBABQueryParser::LtlfContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::IfteContext::THEN() {
  return getToken(KnoBABQueryParser::THEN, 0);
}

tree::TerminalNode* KnoBABQueryParser::IfteContext::ELSE() {
  return getToken(KnoBABQueryParser::ELSE, 0);
}

tree::TerminalNode* KnoBABQueryParser::IfteContext::TIMED() {
  return getToken(KnoBABQueryParser::TIMED, 0);
}

tree::TerminalNode* KnoBABQueryParser::IfteContext::THETA() {
  return getToken(KnoBABQueryParser::THETA, 0);
}

tree::TerminalNode* KnoBABQueryParser::IfteContext::INV() {
  return getToken(KnoBABQueryParser::INV, 0);
}

KnoBABQueryParser::IfteContext::IfteContext(LtlfContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::IfteContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfte(this);
}
void KnoBABQueryParser::IfteContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfte(this);
}

std::any KnoBABQueryParser::IfteContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitIfte(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::ltlf() {
   return ltlf(0);
}

KnoBABQueryParser::LtlfContext* KnoBABQueryParser::ltlf(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  KnoBABQueryParser::LtlfContext *_localctx = _tracker.createInstance<LtlfContext>(_ctx, parentState);
  KnoBABQueryParser::LtlfContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 38;
  enterRecursionRule(_localctx, 38, KnoBABQueryParser::RuleLtlf, precedence);

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
    setState(391);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::INIT: {
        _localctx = _tracker.createInstance<InitContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(298);
        match(KnoBABQueryParser::INIT);
        setState(300);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
        case 1: {
          setState(299);
          match(KnoBABQueryParser::TIMED);
          break;
        }

        default:
          break;
        }
        setState(303);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 35, _ctx)) {
        case 1: {
          setState(302);
          declare_arguments();
          break;
        }

        default:
          break;
        }
        setState(306);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx)) {
        case 1: {
          setState(305);
          declare_act_target();
          break;
        }

        default:
          break;
        }
        break;
      }

      case KnoBABQueryParser::END: {
        _localctx = _tracker.createInstance<EndContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(308);
        match(KnoBABQueryParser::END);
        setState(310);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
        case 1: {
          setState(309);
          match(KnoBABQueryParser::TIMED);
          break;
        }

        default:
          break;
        }
        setState(313);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
        case 1: {
          setState(312);
          declare_arguments();
          break;
        }

        default:
          break;
        }
        setState(316);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
        case 1: {
          setState(315);
          declare_act_target();
          break;
        }

        default:
          break;
        }
        break;
      }

      case KnoBABQueryParser::LAST: {
        _localctx = _tracker.createInstance<LastContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(318);
        match(KnoBABQueryParser::LAST);
        setState(320);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
        case 1: {
          setState(319);
          declare_act_target();
          break;
        }

        default:
          break;
        }
        break;
      }

      case KnoBABQueryParser::FIRST: {
        _localctx = _tracker.createInstance<FirstContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(322);
        match(KnoBABQueryParser::FIRST);
        setState(324);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
        case 1: {
          setState(323);
          declare_act_target();
          break;
        }

        default:
          break;
        }
        break;
      }

      case KnoBABQueryParser::EXISTS: {
        _localctx = _tracker.createInstance<ExistsContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(326);
        match(KnoBABQueryParser::EXISTS);
        setState(328);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::NEGATED) {
          setState(327);
          match(KnoBABQueryParser::NEGATED);
        }
        setState(330);
        match(KnoBABQueryParser::INTNUMBER);
        setState(332);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 43, _ctx)) {
        case 1: {
          setState(331);
          match(KnoBABQueryParser::TIMED);
          break;
        }

        default:
          break;
        }
        setState(335);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 44, _ctx)) {
        case 1: {
          setState(334);
          declare_arguments();
          break;
        }

        default:
          break;
        }
        setState(338);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 45, _ctx)) {
        case 1: {
          setState(337);
          declare_act_target();
          break;
        }

        default:
          break;
        }
        break;
      }

      case KnoBABQueryParser::ABSENCE: {
        _localctx = _tracker.createInstance<AbsenceContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(340);
        match(KnoBABQueryParser::ABSENCE);
        setState(341);
        match(KnoBABQueryParser::INTNUMBER);
        setState(343);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 46, _ctx)) {
        case 1: {
          setState(342);
          match(KnoBABQueryParser::TIMED);
          break;
        }

        default:
          break;
        }
        setState(346);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 47, _ctx)) {
        case 1: {
          setState(345);
          declare_arguments();
          break;
        }

        default:
          break;
        }
        setState(349);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 48, _ctx)) {
        case 1: {
          setState(348);
          declare_act_target();
          break;
        }

        default:
          break;
        }
        break;
      }

      case KnoBABQueryParser::NEXT: {
        _localctx = _tracker.createInstance<NextContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(351);
        match(KnoBABQueryParser::NEXT);
        setState(352);
        ltlf(13);
        break;
      }

      case KnoBABQueryParser::IF: {
        _localctx = _tracker.createInstance<IfteContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(353);
        match(KnoBABQueryParser::IF);
        setState(355);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::TIMED) {
          setState(354);
          match(KnoBABQueryParser::TIMED);
        }
        setState(357);
        ltlf(0);
        setState(358);
        match(KnoBABQueryParser::THEN);
        setState(359);
        ltlf(0);
        setState(361);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::THETA) {
          setState(360);
          match(KnoBABQueryParser::THETA);
        }
        setState(364);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::INV) {
          setState(363);
          match(KnoBABQueryParser::INV);
        }
        setState(366);
        match(KnoBABQueryParser::ELSE);
        setState(367);
        ltlf(9);
        break;
      }

      case KnoBABQueryParser::BOX: {
        _localctx = _tracker.createInstance<BoxContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(369);
        match(KnoBABQueryParser::BOX);
        setState(371);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::TIMED) {
          setState(370);
          match(KnoBABQueryParser::TIMED);
        }
        setState(373);
        ltlf(7);
        break;
      }

      case KnoBABQueryParser::DIAMOND: {
        _localctx = _tracker.createInstance<DiamondContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(374);
        match(KnoBABQueryParser::DIAMOND);
        setState(376);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::TIMED) {
          setState(375);
          match(KnoBABQueryParser::TIMED);
        }
        setState(378);
        ltlf(6);
        break;
      }

      case KnoBABQueryParser::NEGATED: {
        _localctx = _tracker.createInstance<NotContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(379);
        match(KnoBABQueryParser::NEGATED);
        setState(381);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::TIMED) {
          setState(380);
          match(KnoBABQueryParser::TIMED);
        }
        setState(383);
        ltlf(0);
        setState(385);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 55, _ctx)) {
        case 1: {
          setState(384);
          match(KnoBABQueryParser::PRESERVE);
          break;
        }

        default:
          break;
        }
        break;
      }

      case KnoBABQueryParser::T__43: {
        _localctx = _tracker.createInstance<ParenContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(387);
        match(KnoBABQueryParser::T__43);
        setState(388);
        ltlf(0);
        setState(389);
        match(KnoBABQueryParser::T__44);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(470);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 76, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(468);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<OrContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(393);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(394);
          match(KnoBABQueryParser::OR);
          setState(396);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::TIMED) {
            setState(395);
            match(KnoBABQueryParser::TIMED);
          }
          setState(399);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(398);
            match(KnoBABQueryParser::THETA);
          }
          setState(402);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(401);
            match(KnoBABQueryParser::INV);
          }
          setState(404);
          ltlf(12);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<AndContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(405);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(406);
          match(KnoBABQueryParser::AND);
          setState(408);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::TIMED) {
            setState(407);
            match(KnoBABQueryParser::TIMED);
          }
          setState(411);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(410);
            match(KnoBABQueryParser::THETA);
          }
          setState(414);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(413);
            match(KnoBABQueryParser::INV);
          }
          setState(416);
          ltlf(11);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<ImplicationContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(417);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(418);
          match(KnoBABQueryParser::T__42);
          setState(420);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::TIMED) {
            setState(419);
            match(KnoBABQueryParser::TIMED);
          }
          setState(423);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(422);
            match(KnoBABQueryParser::THETA);
          }
          setState(426);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(425);
            match(KnoBABQueryParser::INV);
          }
          setState(428);
          ltlf(10);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<UntilContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(429);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(430);
          match(KnoBABQueryParser::UNTIL);
          setState(432);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::TIMED) {
            setState(431);
            match(KnoBABQueryParser::TIMED);
          }
          setState(435);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(434);
            match(KnoBABQueryParser::THETA);
          }
          setState(438);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(437);
            match(KnoBABQueryParser::INV);
          }
          setState(440);
          ltlf(8);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<And_futureContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(441);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(442);
          match(KnoBABQueryParser::T__45);
          setState(444);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(443);
            match(KnoBABQueryParser::THETA);
          }
          setState(447);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(446);
            match(KnoBABQueryParser::INV);
          }
          setState(449);
          ltlf(3);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<And_next_globallyContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(450);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(451);
          match(KnoBABQueryParser::T__46);
          setState(453);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(452);
            match(KnoBABQueryParser::THETA);
          }
          setState(456);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(455);
            match(KnoBABQueryParser::INV);
          }
          setState(458);
          ltlf(2);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<And_globallyContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(459);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(460);
          match(KnoBABQueryParser::T__47);
          setState(462);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(461);
            match(KnoBABQueryParser::THETA);
          }
          setState(465);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(464);
            match(KnoBABQueryParser::INV);
          }
          setState(467);
          ltlf(1);
          break;
        }

        default:
          break;
        } 
      }
      setState(472);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 76, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- Data_aware_declareContext ------------------------------------------------------------------

KnoBABQueryParser::Data_aware_declareContext::Data_aware_declareContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<KnoBABQueryParser::DeclareContext *> KnoBABQueryParser::Data_aware_declareContext::declare() {
  return getRuleContexts<KnoBABQueryParser::DeclareContext>();
}

KnoBABQueryParser::DeclareContext* KnoBABQueryParser::Data_aware_declareContext::declare(size_t i) {
  return getRuleContext<KnoBABQueryParser::DeclareContext>(i);
}


size_t KnoBABQueryParser::Data_aware_declareContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleData_aware_declare;
}

void KnoBABQueryParser::Data_aware_declareContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterData_aware_declare(this);
}

void KnoBABQueryParser::Data_aware_declareContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitData_aware_declare(this);
}


std::any KnoBABQueryParser::Data_aware_declareContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitData_aware_declare(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Data_aware_declareContext* KnoBABQueryParser::data_aware_declare() {
  Data_aware_declareContext *_localctx = _tracker.createInstance<Data_aware_declareContext>(_ctx, getState());
  enterRule(_localctx, 40, KnoBABQueryParser::RuleData_aware_declare);
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
    setState(476);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == KnoBABQueryParser::STRING) {
      setState(473);
      declare();
      setState(478);
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

KnoBABQueryParser::DeclareContext::DeclareContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::DeclareContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleDeclare;
}

void KnoBABQueryParser::DeclareContext::copyFrom(DeclareContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- Nary_propContext ------------------------------------------------------------------

std::vector<KnoBABQueryParser::FieldsContext *> KnoBABQueryParser::Nary_propContext::fields() {
  return getRuleContexts<KnoBABQueryParser::FieldsContext>();
}

KnoBABQueryParser::FieldsContext* KnoBABQueryParser::Nary_propContext::fields(size_t i) {
  return getRuleContext<KnoBABQueryParser::FieldsContext>(i);
}

tree::TerminalNode* KnoBABQueryParser::Nary_propContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}

KnoBABQueryParser::PropContext* KnoBABQueryParser::Nary_propContext::prop() {
  return getRuleContext<KnoBABQueryParser::PropContext>(0);
}

KnoBABQueryParser::Nary_propContext::Nary_propContext(DeclareContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::Nary_propContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNary_prop(this);
}
void KnoBABQueryParser::Nary_propContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNary_prop(this);
}

std::any KnoBABQueryParser::Nary_propContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitNary_prop(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Unary_propContext ------------------------------------------------------------------

KnoBABQueryParser::FieldsContext* KnoBABQueryParser::Unary_propContext::fields() {
  return getRuleContext<KnoBABQueryParser::FieldsContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::Unary_propContext::INTNUMBER() {
  return getToken(KnoBABQueryParser::INTNUMBER, 0);
}

tree::TerminalNode* KnoBABQueryParser::Unary_propContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}

KnoBABQueryParser::Unary_propContext::Unary_propContext(DeclareContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::Unary_propContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnary_prop(this);
}
void KnoBABQueryParser::Unary_propContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnary_prop(this);
}

std::any KnoBABQueryParser::Unary_propContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitUnary_prop(this);
  else
    return visitor->visitChildren(this);
}
KnoBABQueryParser::DeclareContext* KnoBABQueryParser::declare() {
  DeclareContext *_localctx = _tracker.createInstance<DeclareContext>(_ctx, getState());
  enterRule(_localctx, 42, KnoBABQueryParser::RuleDeclare);
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
    setState(501);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 80, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::Nary_propContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(479);
      antlrcpp::downCast<Nary_propContext *>(_localctx)->name = match(KnoBABQueryParser::STRING);
      setState(480);
      match(KnoBABQueryParser::T__43);
      setState(484); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(481);
                fields();
                setState(482);
                match(KnoBABQueryParser::T__48);
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(486); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 78, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      setState(488);
      fields();
      setState(489);
      match(KnoBABQueryParser::T__44);
      setState(492);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == KnoBABQueryParser::T__49) {
        setState(490);
        match(KnoBABQueryParser::T__49);
        setState(491);
        prop();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::Unary_propContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(494);
      antlrcpp::downCast<Unary_propContext *>(_localctx)->name = match(KnoBABQueryParser::STRING);
      setState(495);
      match(KnoBABQueryParser::T__43);
      setState(496);
      fields();
      setState(497);
      match(KnoBABQueryParser::T__48);
      setState(498);
      match(KnoBABQueryParser::INTNUMBER);
      setState(499);
      match(KnoBABQueryParser::T__44);
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

KnoBABQueryParser::FieldsContext::FieldsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KnoBABQueryParser::PropContext* KnoBABQueryParser::FieldsContext::prop() {
  return getRuleContext<KnoBABQueryParser::PropContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::FieldsContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}


size_t KnoBABQueryParser::FieldsContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleFields;
}

void KnoBABQueryParser::FieldsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFields(this);
}

void KnoBABQueryParser::FieldsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFields(this);
}


std::any KnoBABQueryParser::FieldsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitFields(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::FieldsContext* KnoBABQueryParser::fields() {
  FieldsContext *_localctx = _tracker.createInstance<FieldsContext>(_ctx, getState());
  enterRule(_localctx, 44, KnoBABQueryParser::RuleFields);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(503);
    antlrcpp::downCast<FieldsContext *>(_localctx)->label = match(KnoBABQueryParser::STRING);
    setState(504);
    match(KnoBABQueryParser::T__48);
    setState(505);
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

KnoBABQueryParser::PropContext::PropContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::PropContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleProp;
}

void KnoBABQueryParser::PropContext::copyFrom(PropContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- DisjContext ------------------------------------------------------------------

KnoBABQueryParser::Prop_within_dijunctionContext* KnoBABQueryParser::DisjContext::prop_within_dijunction() {
  return getRuleContext<KnoBABQueryParser::Prop_within_dijunctionContext>(0);
}

KnoBABQueryParser::PropContext* KnoBABQueryParser::DisjContext::prop() {
  return getRuleContext<KnoBABQueryParser::PropContext>(0);
}

KnoBABQueryParser::DisjContext::DisjContext(PropContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::DisjContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDisj(this);
}
void KnoBABQueryParser::DisjContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDisj(this);
}

std::any KnoBABQueryParser::DisjContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDisj(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Conj_or_atomContext ------------------------------------------------------------------

KnoBABQueryParser::Prop_within_dijunctionContext* KnoBABQueryParser::Conj_or_atomContext::prop_within_dijunction() {
  return getRuleContext<KnoBABQueryParser::Prop_within_dijunctionContext>(0);
}

KnoBABQueryParser::Conj_or_atomContext::Conj_or_atomContext(PropContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::Conj_or_atomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConj_or_atom(this);
}
void KnoBABQueryParser::Conj_or_atomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConj_or_atom(this);
}

std::any KnoBABQueryParser::Conj_or_atomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitConj_or_atom(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TopContext ------------------------------------------------------------------

KnoBABQueryParser::TopContext::TopContext(PropContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::TopContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTop(this);
}
void KnoBABQueryParser::TopContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTop(this);
}

std::any KnoBABQueryParser::TopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitTop(this);
  else
    return visitor->visitChildren(this);
}
KnoBABQueryParser::PropContext* KnoBABQueryParser::prop() {
  PropContext *_localctx = _tracker.createInstance<PropContext>(_ctx, getState());
  enterRule(_localctx, 46, KnoBABQueryParser::RuleProp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(513);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 81, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::DisjContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(507);
      prop_within_dijunction();
      setState(508);
      match(KnoBABQueryParser::T__50);
      setState(509);
      prop();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::Conj_or_atomContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(511);
      prop_within_dijunction();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::TopContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(512);
      match(KnoBABQueryParser::T__51);
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

KnoBABQueryParser::Prop_within_dijunctionContext::Prop_within_dijunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::Prop_within_dijunctionContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleProp_within_dijunction;
}

void KnoBABQueryParser::Prop_within_dijunctionContext::copyFrom(Prop_within_dijunctionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- In_atomContext ------------------------------------------------------------------

KnoBABQueryParser::AtomContext* KnoBABQueryParser::In_atomContext::atom() {
  return getRuleContext<KnoBABQueryParser::AtomContext>(0);
}

KnoBABQueryParser::In_atomContext::In_atomContext(Prop_within_dijunctionContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::In_atomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIn_atom(this);
}
void KnoBABQueryParser::In_atomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIn_atom(this);
}

std::any KnoBABQueryParser::In_atomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitIn_atom(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Atom_conjContext ------------------------------------------------------------------

KnoBABQueryParser::AtomContext* KnoBABQueryParser::Atom_conjContext::atom() {
  return getRuleContext<KnoBABQueryParser::AtomContext>(0);
}

KnoBABQueryParser::Prop_within_dijunctionContext* KnoBABQueryParser::Atom_conjContext::prop_within_dijunction() {
  return getRuleContext<KnoBABQueryParser::Prop_within_dijunctionContext>(0);
}

KnoBABQueryParser::Atom_conjContext::Atom_conjContext(Prop_within_dijunctionContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::Atom_conjContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtom_conj(this);
}
void KnoBABQueryParser::Atom_conjContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtom_conj(this);
}

std::any KnoBABQueryParser::Atom_conjContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitAtom_conj(this);
  else
    return visitor->visitChildren(this);
}
KnoBABQueryParser::Prop_within_dijunctionContext* KnoBABQueryParser::prop_within_dijunction() {
  Prop_within_dijunctionContext *_localctx = _tracker.createInstance<Prop_within_dijunctionContext>(_ctx, getState());
  enterRule(_localctx, 48, KnoBABQueryParser::RuleProp_within_dijunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(520);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 82, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::In_atomContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(515);
      atom();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::Atom_conjContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(516);
      atom();
      setState(517);
      match(KnoBABQueryParser::T__52);
      setState(518);
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

KnoBABQueryParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<KnoBABQueryParser::VarContext *> KnoBABQueryParser::AtomContext::var() {
  return getRuleContexts<KnoBABQueryParser::VarContext>();
}

KnoBABQueryParser::VarContext* KnoBABQueryParser::AtomContext::var(size_t i) {
  return getRuleContext<KnoBABQueryParser::VarContext>(i);
}

KnoBABQueryParser::RelContext* KnoBABQueryParser::AtomContext::rel() {
  return getRuleContext<KnoBABQueryParser::RelContext>(0);
}

tree::TerminalNode* KnoBABQueryParser::AtomContext::NUMBER() {
  return getToken(KnoBABQueryParser::NUMBER, 0);
}

tree::TerminalNode* KnoBABQueryParser::AtomContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}

tree::TerminalNode* KnoBABQueryParser::AtomContext::NEGATED() {
  return getToken(KnoBABQueryParser::NEGATED, 0);
}


size_t KnoBABQueryParser::AtomContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleAtom;
}

void KnoBABQueryParser::AtomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtom(this);
}

void KnoBABQueryParser::AtomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtom(this);
}


std::any KnoBABQueryParser::AtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitAtom(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::AtomContext* KnoBABQueryParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 50, KnoBABQueryParser::RuleAtom);
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
    setState(523);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::NEGATED) {
      setState(522);
      antlrcpp::downCast<AtomContext *>(_localctx)->isnegated = match(KnoBABQueryParser::NEGATED);
    }
    setState(525);
    var();
    setState(526);
    rel();
    setState(530);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::NUMBER: {
        setState(527);
        match(KnoBABQueryParser::NUMBER);
        break;
      }

      case KnoBABQueryParser::STRING: {
        setState(528);
        match(KnoBABQueryParser::STRING);
        break;
      }

      case KnoBABQueryParser::T__69: {
        setState(529);
        antlrcpp::downCast<AtomContext *>(_localctx)->leftvar = var();
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

KnoBABQueryParser::RelContext::RelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::RelContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleRel;
}

void KnoBABQueryParser::RelContext::copyFrom(RelContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- GeqContext ------------------------------------------------------------------

KnoBABQueryParser::GeqContext::GeqContext(RelContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::GeqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGeq(this);
}
void KnoBABQueryParser::GeqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGeq(this);
}

std::any KnoBABQueryParser::GeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitGeq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LtContext ------------------------------------------------------------------

KnoBABQueryParser::LtContext::LtContext(RelContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::LtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLt(this);
}
void KnoBABQueryParser::LtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLt(this);
}

std::any KnoBABQueryParser::LtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitLt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LeqContext ------------------------------------------------------------------

KnoBABQueryParser::LeqContext::LeqContext(RelContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::LeqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLeq(this);
}
void KnoBABQueryParser::LeqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLeq(this);
}

std::any KnoBABQueryParser::LeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitLeq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NeqContext ------------------------------------------------------------------

KnoBABQueryParser::NeqContext::NeqContext(RelContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::NeqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNeq(this);
}
void KnoBABQueryParser::NeqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNeq(this);
}

std::any KnoBABQueryParser::NeqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitNeq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- EqContext ------------------------------------------------------------------

KnoBABQueryParser::EqContext::EqContext(RelContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::EqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEq(this);
}
void KnoBABQueryParser::EqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEq(this);
}

std::any KnoBABQueryParser::EqContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitEq(this);
  else
    return visitor->visitChildren(this);
}
//----------------- GtContext ------------------------------------------------------------------

KnoBABQueryParser::GtContext::GtContext(RelContext *ctx) { copyFrom(ctx); }

void KnoBABQueryParser::GtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGt(this);
}
void KnoBABQueryParser::GtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGt(this);
}

std::any KnoBABQueryParser::GtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitGt(this);
  else
    return visitor->visitChildren(this);
}
KnoBABQueryParser::RelContext* KnoBABQueryParser::rel() {
  RelContext *_localctx = _tracker.createInstance<RelContext>(_ctx, getState());
  enterRule(_localctx, 52, KnoBABQueryParser::RuleRel);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(538);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::T__53: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::LtContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(532);
        match(KnoBABQueryParser::T__53);
        break;
      }

      case KnoBABQueryParser::T__54: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::LeqContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(533);
        match(KnoBABQueryParser::T__54);
        break;
      }

      case KnoBABQueryParser::T__55: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::GtContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(534);
        match(KnoBABQueryParser::T__55);
        break;
      }

      case KnoBABQueryParser::T__56: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::GeqContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(535);
        match(KnoBABQueryParser::T__56);
        break;
      }

      case KnoBABQueryParser::T__12: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::EqContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(536);
        match(KnoBABQueryParser::T__12);
        break;
      }

      case KnoBABQueryParser::T__57: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::NeqContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(537);
        match(KnoBABQueryParser::T__57);
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

//----------------- Declare_argumentsContext ------------------------------------------------------------------

KnoBABQueryParser::Declare_argumentsContext::Declare_argumentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::Declare_argumentsContext::INTNUMBER() {
  return getToken(KnoBABQueryParser::INTNUMBER, 0);
}


size_t KnoBABQueryParser::Declare_argumentsContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleDeclare_arguments;
}

void KnoBABQueryParser::Declare_argumentsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclare_arguments(this);
}

void KnoBABQueryParser::Declare_argumentsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclare_arguments(this);
}


std::any KnoBABQueryParser::Declare_argumentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDeclare_arguments(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Declare_argumentsContext* KnoBABQueryParser::declare_arguments() {
  Declare_argumentsContext *_localctx = _tracker.createInstance<Declare_argumentsContext>(_ctx, getState());
  enterRule(_localctx, 54, KnoBABQueryParser::RuleDeclare_arguments);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(540);
    match(KnoBABQueryParser::T__58);
    setState(541);
    match(KnoBABQueryParser::INTNUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Declare_act_targetContext ------------------------------------------------------------------

KnoBABQueryParser::Declare_act_targetContext::Declare_act_targetContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::Declare_act_targetContext::ACTIVATION() {
  return getToken(KnoBABQueryParser::ACTIVATION, 0);
}

tree::TerminalNode* KnoBABQueryParser::Declare_act_targetContext::TARGET() {
  return getToken(KnoBABQueryParser::TARGET, 0);
}


size_t KnoBABQueryParser::Declare_act_targetContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleDeclare_act_target;
}

void KnoBABQueryParser::Declare_act_targetContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclare_act_target(this);
}

void KnoBABQueryParser::Declare_act_targetContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclare_act_target(this);
}


std::any KnoBABQueryParser::Declare_act_targetContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitDeclare_act_target(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Declare_act_targetContext* KnoBABQueryParser::declare_act_target() {
  Declare_act_targetContext *_localctx = _tracker.createInstance<Declare_act_targetContext>(_ctx, getState());
  enterRule(_localctx, 56, KnoBABQueryParser::RuleDeclare_act_target);
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
    setState(543);
    _la = _input->LA(1);
    if (!(_la == KnoBABQueryParser::ACTIVATION

    || _la == KnoBABQueryParser::TARGET)) {
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

//----------------- No_preliminary_fillContext ------------------------------------------------------------------

KnoBABQueryParser::No_preliminary_fillContext::No_preliminary_fillContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::No_preliminary_fillContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleNo_preliminary_fill;
}

void KnoBABQueryParser::No_preliminary_fillContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNo_preliminary_fill(this);
}

void KnoBABQueryParser::No_preliminary_fillContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNo_preliminary_fill(this);
}


std::any KnoBABQueryParser::No_preliminary_fillContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitNo_preliminary_fill(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::No_preliminary_fillContext* KnoBABQueryParser::no_preliminary_fill() {
  No_preliminary_fillContext *_localctx = _tracker.createInstance<No_preliminary_fillContext>(_ctx, getState());
  enterRule(_localctx, 58, KnoBABQueryParser::RuleNo_preliminary_fill);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(545);
    match(KnoBABQueryParser::T__59);
    setState(546);
    match(KnoBABQueryParser::T__60);
    setState(547);
    match(KnoBABQueryParser::T__61);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Act_for_attributesContext ------------------------------------------------------------------

KnoBABQueryParser::Act_for_attributesContext::Act_for_attributesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::Act_for_attributesContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleAct_for_attributes;
}

void KnoBABQueryParser::Act_for_attributesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAct_for_attributes(this);
}

void KnoBABQueryParser::Act_for_attributesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAct_for_attributes(this);
}


std::any KnoBABQueryParser::Act_for_attributesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitAct_for_attributes(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::Act_for_attributesContext* KnoBABQueryParser::act_for_attributes() {
  Act_for_attributesContext *_localctx = _tracker.createInstance<Act_for_attributesContext>(_ctx, getState());
  enterRule(_localctx, 60, KnoBABQueryParser::RuleAct_for_attributes);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(549);
    match(KnoBABQueryParser::T__59);
    setState(550);
    match(KnoBABQueryParser::T__62);
    setState(551);
    match(KnoBABQueryParser::T__63);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- No_cream_offContext ------------------------------------------------------------------

KnoBABQueryParser::No_cream_offContext::No_cream_offContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::No_cream_offContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleNo_cream_off;
}

void KnoBABQueryParser::No_cream_offContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNo_cream_off(this);
}

void KnoBABQueryParser::No_cream_offContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNo_cream_off(this);
}


std::any KnoBABQueryParser::No_cream_offContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitNo_cream_off(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::No_cream_offContext* KnoBABQueryParser::no_cream_off() {
  No_cream_offContext *_localctx = _tracker.createInstance<No_cream_offContext>(_ctx, getState());
  enterRule(_localctx, 62, KnoBABQueryParser::RuleNo_cream_off);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(553);
    match(KnoBABQueryParser::T__59);
    setState(554);
    match(KnoBABQueryParser::T__64);
    setState(555);
    match(KnoBABQueryParser::T__65);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- With_dataContext ------------------------------------------------------------------

KnoBABQueryParser::With_dataContext::With_dataContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::With_dataContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleWith_data;
}

void KnoBABQueryParser::With_dataContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWith_data(this);
}

void KnoBABQueryParser::With_dataContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWith_data(this);
}


std::any KnoBABQueryParser::With_dataContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitWith_data(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::With_dataContext* KnoBABQueryParser::with_data() {
  With_dataContext *_localctx = _tracker.createInstance<With_dataContext>(_ctx, getState());
  enterRule(_localctx, 64, KnoBABQueryParser::RuleWith_data);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(557);
    match(KnoBABQueryParser::T__17);
    setState(558);
    match(KnoBABQueryParser::T__66);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- With_missingContext ------------------------------------------------------------------

KnoBABQueryParser::With_missingContext::With_missingContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::With_missingContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleWith_missing;
}

void KnoBABQueryParser::With_missingContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWith_missing(this);
}

void KnoBABQueryParser::With_missingContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWith_missing(this);
}


std::any KnoBABQueryParser::With_missingContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitWith_missing(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::With_missingContext* KnoBABQueryParser::with_missing() {
  With_missingContext *_localctx = _tracker.createInstance<With_missingContext>(_ctx, getState());
  enterRule(_localctx, 66, KnoBABQueryParser::RuleWith_missing);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(560);
    match(KnoBABQueryParser::T__17);
    setState(561);
    match(KnoBABQueryParser::T__67);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- No_statsContext ------------------------------------------------------------------

KnoBABQueryParser::No_statsContext::No_statsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t KnoBABQueryParser::No_statsContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleNo_stats;
}

void KnoBABQueryParser::No_statsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNo_stats(this);
}

void KnoBABQueryParser::No_statsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNo_stats(this);
}


std::any KnoBABQueryParser::No_statsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitNo_stats(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::No_statsContext* KnoBABQueryParser::no_stats() {
  No_statsContext *_localctx = _tracker.createInstance<No_statsContext>(_ctx, getState());
  enterRule(_localctx, 68, KnoBABQueryParser::RuleNo_stats);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(563);
    match(KnoBABQueryParser::T__59);
    setState(564);
    match(KnoBABQueryParser::T__68);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarContext ------------------------------------------------------------------

KnoBABQueryParser::VarContext::VarContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KnoBABQueryParser::VarContext::STRING() {
  return getToken(KnoBABQueryParser::STRING, 0);
}


size_t KnoBABQueryParser::VarContext::getRuleIndex() const {
  return KnoBABQueryParser::RuleVar;
}

void KnoBABQueryParser::VarContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVar(this);
}

void KnoBABQueryParser::VarContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<KnoBABQueryListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVar(this);
}


std::any KnoBABQueryParser::VarContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KnoBABQueryVisitor*>(visitor))
    return parserVisitor->visitVar(this);
  else
    return visitor->visitChildren(this);
}

KnoBABQueryParser::VarContext* KnoBABQueryParser::var() {
  VarContext *_localctx = _tracker.createInstance<VarContext>(_ctx, getState());
  enterRule(_localctx, 70, KnoBABQueryParser::RuleVar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(566);
    match(KnoBABQueryParser::T__69);
    setState(567);
    match(KnoBABQueryParser::STRING);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool KnoBABQueryParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 19: return ltlfSempred(antlrcpp::downCast<LtlfContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool KnoBABQueryParser::ltlfSempred(LtlfContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 12);
    case 1: return precpred(_ctx, 11);
    case 2: return precpred(_ctx, 10);
    case 3: return precpred(_ctx, 8);
    case 4: return precpred(_ctx, 3);
    case 5: return precpred(_ctx, 2);
    case 6: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void KnoBABQueryParser::initialize() {
  ::antlr4::internal::call_once(knobabqueryParserOnceFlag, knobabqueryParserInitialize);
}
