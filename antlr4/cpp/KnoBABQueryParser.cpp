
// Generated from KnoBABQuery.g4 by ANTLR 4.11.1


#include "KnoBABQueryListener.h"
#include "KnoBABQueryVisitor.h"

#include "KnoBABQueryParser.h"


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
      "log", "trace", "event", "data_part", "field", "model_query", "model", 
      "atomization", "grounding", "display_qp", "query_plan", "declare_syntax", 
      "has_args", "ltlf", "data_aware_declare", "declare", "fields", "prop", 
      "prop_within_dijunction", "atom", "rel", "declare_arguments", "declare_act_target", 
      "no_preliminary_fill", "act_for_attributes", "no_cream_off", "with_data", 
      "with_missing", "no_stats", "var"
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
  	4,1,104,521,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,1,0,1,0,1,0,1,0,
  	1,0,3,0,74,8,0,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,3,2,84,8,2,1,2,1,2,3,2,
  	88,8,2,1,2,3,2,91,8,2,1,2,3,2,94,8,2,1,2,1,2,1,2,1,3,1,3,1,3,1,3,1,3,
  	3,3,104,8,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,3,3,114,8,3,1,4,1,4,1,4,1,
  	4,1,4,5,4,121,8,4,10,4,12,4,124,9,4,1,4,1,4,1,4,1,5,1,5,1,5,5,5,132,8,
  	5,10,5,12,5,135,9,5,1,6,1,6,3,6,139,8,6,1,7,1,7,5,7,143,8,7,10,7,12,7,
  	146,9,7,1,7,1,7,1,8,1,8,1,8,1,8,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,
  	9,1,9,1,9,3,9,166,8,9,1,9,1,9,3,9,170,8,9,1,9,3,9,173,8,9,1,9,3,9,176,
  	8,9,1,9,3,9,179,8,9,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,3,10,189,
  	8,10,1,11,1,11,1,11,1,11,3,11,195,8,11,1,11,1,11,1,11,3,11,200,8,11,1,
  	11,1,11,1,11,3,11,205,8,11,1,12,1,12,1,12,1,12,3,12,211,8,12,1,12,3,12,
  	214,8,12,1,12,3,12,217,8,12,1,12,3,12,220,8,12,1,13,1,13,1,13,1,14,3,
  	14,226,8,14,1,14,1,14,1,14,1,14,4,14,232,8,14,11,14,12,14,233,1,14,1,
  	14,1,15,1,15,1,15,3,15,241,8,15,1,15,1,15,1,15,1,16,1,16,1,16,1,17,1,
  	17,1,17,3,17,252,8,17,1,17,3,17,255,8,17,1,17,3,17,258,8,17,1,17,1,17,
  	3,17,262,8,17,1,17,3,17,265,8,17,1,17,3,17,268,8,17,1,17,1,17,3,17,272,
  	8,17,1,17,1,17,3,17,276,8,17,1,17,1,17,3,17,280,8,17,1,17,1,17,3,17,284,
  	8,17,1,17,3,17,287,8,17,1,17,3,17,290,8,17,1,17,1,17,1,17,3,17,295,8,
  	17,1,17,3,17,298,8,17,1,17,3,17,301,8,17,1,17,1,17,1,17,1,17,3,17,307,
  	8,17,1,17,1,17,1,17,1,17,3,17,313,8,17,1,17,3,17,316,8,17,1,17,1,17,1,
  	17,1,17,1,17,3,17,323,8,17,1,17,1,17,1,17,3,17,328,8,17,1,17,1,17,1,17,
  	3,17,333,8,17,1,17,1,17,3,17,337,8,17,1,17,1,17,1,17,1,17,3,17,343,8,
  	17,1,17,1,17,1,17,3,17,348,8,17,1,17,3,17,351,8,17,1,17,3,17,354,8,17,
  	1,17,1,17,1,17,1,17,3,17,360,8,17,1,17,3,17,363,8,17,1,17,3,17,366,8,
  	17,1,17,1,17,1,17,1,17,3,17,372,8,17,1,17,3,17,375,8,17,1,17,3,17,378,
  	8,17,1,17,1,17,1,17,1,17,3,17,384,8,17,1,17,3,17,387,8,17,1,17,3,17,390,
  	8,17,1,17,1,17,1,17,1,17,3,17,396,8,17,1,17,3,17,399,8,17,1,17,1,17,1,
  	17,1,17,3,17,405,8,17,1,17,3,17,408,8,17,1,17,1,17,1,17,1,17,3,17,414,
  	8,17,1,17,3,17,417,8,17,1,17,5,17,420,8,17,10,17,12,17,423,9,17,1,18,
  	5,18,426,8,18,10,18,12,18,429,9,18,1,19,1,19,1,19,1,19,1,19,4,19,436,
  	8,19,11,19,12,19,437,1,19,1,19,1,19,1,19,3,19,444,8,19,1,19,1,19,1,19,
  	1,19,1,19,1,19,1,19,3,19,453,8,19,1,20,1,20,1,20,1,20,1,21,1,21,1,21,
  	1,21,1,21,1,21,3,21,465,8,21,1,22,1,22,1,22,1,22,1,22,3,22,472,8,22,1,
  	23,3,23,475,8,23,1,23,1,23,1,23,1,23,1,23,3,23,482,8,23,1,24,1,24,1,24,
  	1,24,1,24,1,24,3,24,490,8,24,1,25,1,25,1,25,1,26,1,26,1,27,1,27,1,27,
  	1,27,1,28,1,28,1,28,1,28,1,29,1,29,1,29,1,29,1,30,1,30,1,30,1,31,1,31,
  	1,31,1,32,1,32,1,32,1,33,1,33,1,33,1,33,0,1,34,34,0,2,4,6,8,10,12,14,
  	16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,
  	62,64,66,0,3,1,0,64,66,1,0,100,101,1,0,67,68,594,0,73,1,0,0,0,2,75,1,
  	0,0,0,4,78,1,0,0,0,6,113,1,0,0,0,8,115,1,0,0,0,10,128,1,0,0,0,12,136,
  	1,0,0,0,14,140,1,0,0,0,16,149,1,0,0,0,18,153,1,0,0,0,20,188,1,0,0,0,22,
  	190,1,0,0,0,24,206,1,0,0,0,26,221,1,0,0,0,28,225,1,0,0,0,30,237,1,0,0,
  	0,32,245,1,0,0,0,34,342,1,0,0,0,36,427,1,0,0,0,38,452,1,0,0,0,40,454,
  	1,0,0,0,42,464,1,0,0,0,44,471,1,0,0,0,46,474,1,0,0,0,48,489,1,0,0,0,50,
  	491,1,0,0,0,52,494,1,0,0,0,54,496,1,0,0,0,56,500,1,0,0,0,58,504,1,0,0,
  	0,60,508,1,0,0,0,62,511,1,0,0,0,64,514,1,0,0,0,66,517,1,0,0,0,68,74,3,
  	4,2,0,69,74,3,6,3,0,70,74,3,18,9,0,71,74,3,28,14,0,72,74,3,2,1,0,73,68,
  	1,0,0,0,73,69,1,0,0,0,73,70,1,0,0,0,73,71,1,0,0,0,73,72,1,0,0,0,74,1,
  	1,0,0,0,75,76,5,1,0,0,76,77,5,101,0,0,77,3,1,0,0,0,78,83,5,2,0,0,79,84,
  	5,95,0,0,80,84,5,96,0,0,81,84,5,97,0,0,82,84,3,8,4,0,83,79,1,0,0,0,83,
  	80,1,0,0,0,83,81,1,0,0,0,83,82,1,0,0,0,84,85,1,0,0,0,85,87,5,101,0,0,
  	86,88,3,60,30,0,87,86,1,0,0,0,87,88,1,0,0,0,88,90,1,0,0,0,89,91,3,64,
  	32,0,90,89,1,0,0,0,90,91,1,0,0,0,91,93,1,0,0,0,92,94,3,62,31,0,93,92,
  	1,0,0,0,93,94,1,0,0,0,94,95,1,0,0,0,95,96,5,3,0,0,96,97,5,101,0,0,97,
  	5,1,0,0,0,98,103,5,4,0,0,99,104,5,61,0,0,100,104,5,62,0,0,101,102,5,63,
  	0,0,102,104,5,101,0,0,103,99,1,0,0,0,103,100,1,0,0,0,103,101,1,0,0,0,
  	104,105,1,0,0,0,105,106,5,5,0,0,106,114,5,101,0,0,107,108,5,6,0,0,108,
  	109,7,0,0,0,109,110,5,5,0,0,110,114,5,101,0,0,111,112,5,7,0,0,112,114,
  	5,101,0,0,113,98,1,0,0,0,113,107,1,0,0,0,113,111,1,0,0,0,114,7,1,0,0,
  	0,115,116,5,8,0,0,116,122,5,85,0,0,117,118,3,10,5,0,118,119,5,9,0,0,119,
  	121,1,0,0,0,120,117,1,0,0,0,121,124,1,0,0,0,122,120,1,0,0,0,122,123,1,
  	0,0,0,123,125,1,0,0,0,124,122,1,0,0,0,125,126,3,10,5,0,126,127,5,86,0,
  	0,127,9,1,0,0,0,128,129,3,14,7,0,129,133,5,10,0,0,130,132,3,12,6,0,131,
  	130,1,0,0,0,132,135,1,0,0,0,133,131,1,0,0,0,133,134,1,0,0,0,134,11,1,
  	0,0,0,135,133,1,0,0,0,136,138,5,98,0,0,137,139,3,14,7,0,138,137,1,0,0,
  	0,138,139,1,0,0,0,139,13,1,0,0,0,140,144,5,85,0,0,141,143,3,16,8,0,142,
  	141,1,0,0,0,143,146,1,0,0,0,144,142,1,0,0,0,144,145,1,0,0,0,145,147,1,
  	0,0,0,146,144,1,0,0,0,147,148,5,86,0,0,148,15,1,0,0,0,149,150,3,66,33,
  	0,150,151,5,11,0,0,151,152,7,1,0,0,152,17,1,0,0,0,153,154,5,12,0,0,154,
  	155,3,20,10,0,155,156,5,13,0,0,156,157,5,101,0,0,157,158,5,14,0,0,158,
  	159,5,101,0,0,159,160,5,15,0,0,160,161,5,101,0,0,161,165,1,0,0,0,162,
  	163,5,16,0,0,163,164,5,17,0,0,164,166,5,101,0,0,165,162,1,0,0,0,165,166,
  	1,0,0,0,166,169,1,0,0,0,167,168,5,18,0,0,168,170,5,99,0,0,169,167,1,0,
  	0,0,169,170,1,0,0,0,170,172,1,0,0,0,171,173,3,26,13,0,172,171,1,0,0,0,
  	172,173,1,0,0,0,173,175,1,0,0,0,174,176,3,22,11,0,175,174,1,0,0,0,175,
  	176,1,0,0,0,176,178,1,0,0,0,177,179,3,24,12,0,178,177,1,0,0,0,178,179,
  	1,0,0,0,179,19,1,0,0,0,180,181,5,19,0,0,181,189,5,101,0,0,182,183,5,20,
  	0,0,183,189,3,36,18,0,184,185,5,21,0,0,185,186,5,101,0,0,186,187,5,22,
  	0,0,187,189,5,99,0,0,188,180,1,0,0,0,188,182,1,0,0,0,188,184,1,0,0,0,
  	189,21,1,0,0,0,190,194,5,23,0,0,191,192,5,16,0,0,192,193,5,24,0,0,193,
  	195,5,101,0,0,194,191,1,0,0,0,194,195,1,0,0,0,195,199,1,0,0,0,196,197,
  	5,25,0,0,197,198,5,26,0,0,198,200,5,99,0,0,199,196,1,0,0,0,199,200,1,
  	0,0,0,200,204,1,0,0,0,201,202,5,16,0,0,202,203,5,27,0,0,203,205,5,101,
  	0,0,204,201,1,0,0,0,204,205,1,0,0,0,205,23,1,0,0,0,206,210,5,28,0,0,207,
  	208,5,16,0,0,208,209,5,27,0,0,209,211,5,101,0,0,210,207,1,0,0,0,210,211,
  	1,0,0,0,211,213,1,0,0,0,212,214,3,54,27,0,213,212,1,0,0,0,213,214,1,0,
  	0,0,214,216,1,0,0,0,215,217,3,56,28,0,216,215,1,0,0,0,216,217,1,0,0,0,
  	217,219,1,0,0,0,218,220,3,58,29,0,219,218,1,0,0,0,219,220,1,0,0,0,220,
  	25,1,0,0,0,221,222,5,4,0,0,222,223,5,29,0,0,223,27,1,0,0,0,224,226,5,
  	84,0,0,225,224,1,0,0,0,225,226,1,0,0,0,226,227,1,0,0,0,227,228,5,30,0,
  	0,228,229,5,101,0,0,229,231,5,85,0,0,230,232,3,30,15,0,231,230,1,0,0,
  	0,232,233,1,0,0,0,233,231,1,0,0,0,233,234,1,0,0,0,234,235,1,0,0,0,235,
  	236,5,86,0,0,236,29,1,0,0,0,237,238,5,21,0,0,238,240,5,101,0,0,239,241,
  	3,32,16,0,240,239,1,0,0,0,240,241,1,0,0,0,241,242,1,0,0,0,242,243,5,31,
  	0,0,243,244,3,34,17,0,244,31,1,0,0,0,245,246,5,32,0,0,246,247,5,99,0,
  	0,247,33,1,0,0,0,248,249,6,17,-1,0,249,251,5,69,0,0,250,252,5,88,0,0,
  	251,250,1,0,0,0,251,252,1,0,0,0,252,254,1,0,0,0,253,255,3,50,25,0,254,
  	253,1,0,0,0,254,255,1,0,0,0,255,257,1,0,0,0,256,258,3,52,26,0,257,256,
  	1,0,0,0,257,258,1,0,0,0,258,343,1,0,0,0,259,261,5,70,0,0,260,262,5,88,
  	0,0,261,260,1,0,0,0,261,262,1,0,0,0,262,264,1,0,0,0,263,265,3,50,25,0,
  	264,263,1,0,0,0,264,265,1,0,0,0,265,267,1,0,0,0,266,268,3,52,26,0,267,
  	266,1,0,0,0,267,268,1,0,0,0,268,343,1,0,0,0,269,271,5,77,0,0,270,272,
  	3,52,26,0,271,270,1,0,0,0,271,272,1,0,0,0,272,343,1,0,0,0,273,275,5,76,
  	0,0,274,276,3,52,26,0,275,274,1,0,0,0,275,276,1,0,0,0,276,343,1,0,0,0,
  	277,279,5,71,0,0,278,280,5,94,0,0,279,278,1,0,0,0,279,280,1,0,0,0,280,
  	281,1,0,0,0,281,283,5,99,0,0,282,284,5,88,0,0,283,282,1,0,0,0,283,284,
  	1,0,0,0,284,286,1,0,0,0,285,287,3,50,25,0,286,285,1,0,0,0,286,287,1,0,
  	0,0,287,289,1,0,0,0,288,290,3,52,26,0,289,288,1,0,0,0,289,290,1,0,0,0,
  	290,343,1,0,0,0,291,292,5,72,0,0,292,294,5,99,0,0,293,295,5,88,0,0,294,
  	293,1,0,0,0,294,295,1,0,0,0,295,297,1,0,0,0,296,298,3,50,25,0,297,296,
  	1,0,0,0,297,298,1,0,0,0,298,300,1,0,0,0,299,301,3,52,26,0,300,299,1,0,
  	0,0,300,301,1,0,0,0,301,343,1,0,0,0,302,303,5,73,0,0,303,343,3,34,17,
  	13,304,306,5,78,0,0,305,307,5,88,0,0,306,305,1,0,0,0,306,307,1,0,0,0,
  	307,308,1,0,0,0,308,309,3,34,17,0,309,310,5,79,0,0,310,312,3,34,17,0,
  	311,313,5,89,0,0,312,311,1,0,0,0,312,313,1,0,0,0,313,315,1,0,0,0,314,
  	316,5,91,0,0,315,314,1,0,0,0,315,316,1,0,0,0,316,317,1,0,0,0,317,318,
  	5,80,0,0,318,319,3,34,17,9,319,343,1,0,0,0,320,322,5,82,0,0,321,323,5,
  	88,0,0,322,321,1,0,0,0,322,323,1,0,0,0,323,324,1,0,0,0,324,343,3,34,17,
  	7,325,327,5,83,0,0,326,328,5,88,0,0,327,326,1,0,0,0,327,328,1,0,0,0,328,
  	329,1,0,0,0,329,343,3,34,17,6,330,332,5,94,0,0,331,333,5,88,0,0,332,331,
  	1,0,0,0,332,333,1,0,0,0,333,334,1,0,0,0,334,336,3,34,17,0,335,337,5,87,
  	0,0,336,335,1,0,0,0,336,337,1,0,0,0,337,343,1,0,0,0,338,339,5,34,0,0,
  	339,340,3,34,17,0,340,341,5,35,0,0,341,343,1,0,0,0,342,248,1,0,0,0,342,
  	259,1,0,0,0,342,269,1,0,0,0,342,273,1,0,0,0,342,277,1,0,0,0,342,291,1,
  	0,0,0,342,302,1,0,0,0,342,304,1,0,0,0,342,320,1,0,0,0,342,325,1,0,0,0,
  	342,330,1,0,0,0,342,338,1,0,0,0,343,421,1,0,0,0,344,345,10,12,0,0,345,
  	347,5,74,0,0,346,348,5,88,0,0,347,346,1,0,0,0,347,348,1,0,0,0,348,350,
  	1,0,0,0,349,351,5,89,0,0,350,349,1,0,0,0,350,351,1,0,0,0,351,353,1,0,
  	0,0,352,354,5,91,0,0,353,352,1,0,0,0,353,354,1,0,0,0,354,355,1,0,0,0,
  	355,420,3,34,17,12,356,357,10,11,0,0,357,359,5,75,0,0,358,360,5,88,0,
  	0,359,358,1,0,0,0,359,360,1,0,0,0,360,362,1,0,0,0,361,363,5,89,0,0,362,
  	361,1,0,0,0,362,363,1,0,0,0,363,365,1,0,0,0,364,366,5,91,0,0,365,364,
  	1,0,0,0,365,366,1,0,0,0,366,367,1,0,0,0,367,420,3,34,17,11,368,369,10,
  	10,0,0,369,371,5,33,0,0,370,372,5,88,0,0,371,370,1,0,0,0,371,372,1,0,
  	0,0,372,374,1,0,0,0,373,375,5,89,0,0,374,373,1,0,0,0,374,375,1,0,0,0,
  	375,377,1,0,0,0,376,378,5,91,0,0,377,376,1,0,0,0,377,378,1,0,0,0,378,
  	379,1,0,0,0,379,420,3,34,17,10,380,381,10,8,0,0,381,383,5,81,0,0,382,
  	384,5,88,0,0,383,382,1,0,0,0,383,384,1,0,0,0,384,386,1,0,0,0,385,387,
  	5,89,0,0,386,385,1,0,0,0,386,387,1,0,0,0,387,389,1,0,0,0,388,390,5,91,
  	0,0,389,388,1,0,0,0,389,390,1,0,0,0,390,391,1,0,0,0,391,420,3,34,17,8,
  	392,393,10,3,0,0,393,395,5,36,0,0,394,396,5,89,0,0,395,394,1,0,0,0,395,
  	396,1,0,0,0,396,398,1,0,0,0,397,399,5,91,0,0,398,397,1,0,0,0,398,399,
  	1,0,0,0,399,400,1,0,0,0,400,420,3,34,17,3,401,402,10,2,0,0,402,404,5,
  	37,0,0,403,405,5,89,0,0,404,403,1,0,0,0,404,405,1,0,0,0,405,407,1,0,0,
  	0,406,408,5,91,0,0,407,406,1,0,0,0,407,408,1,0,0,0,408,409,1,0,0,0,409,
  	420,3,34,17,2,410,411,10,1,0,0,411,413,5,38,0,0,412,414,5,89,0,0,413,
  	412,1,0,0,0,413,414,1,0,0,0,414,416,1,0,0,0,415,417,5,91,0,0,416,415,
  	1,0,0,0,416,417,1,0,0,0,417,418,1,0,0,0,418,420,3,34,17,1,419,344,1,0,
  	0,0,419,356,1,0,0,0,419,368,1,0,0,0,419,380,1,0,0,0,419,392,1,0,0,0,419,
  	401,1,0,0,0,419,410,1,0,0,0,420,423,1,0,0,0,421,419,1,0,0,0,421,422,1,
  	0,0,0,422,35,1,0,0,0,423,421,1,0,0,0,424,426,3,38,19,0,425,424,1,0,0,
  	0,426,429,1,0,0,0,427,425,1,0,0,0,427,428,1,0,0,0,428,37,1,0,0,0,429,
  	427,1,0,0,0,430,431,5,101,0,0,431,435,5,34,0,0,432,433,3,40,20,0,433,
  	434,5,39,0,0,434,436,1,0,0,0,435,432,1,0,0,0,436,437,1,0,0,0,437,435,
  	1,0,0,0,437,438,1,0,0,0,438,439,1,0,0,0,439,440,3,40,20,0,440,443,5,35,
  	0,0,441,442,5,40,0,0,442,444,3,42,21,0,443,441,1,0,0,0,443,444,1,0,0,
  	0,444,453,1,0,0,0,445,446,5,101,0,0,446,447,5,34,0,0,447,448,3,40,20,
  	0,448,449,5,39,0,0,449,450,5,99,0,0,450,451,5,35,0,0,451,453,1,0,0,0,
  	452,430,1,0,0,0,452,445,1,0,0,0,453,39,1,0,0,0,454,455,5,101,0,0,455,
  	456,5,39,0,0,456,457,3,42,21,0,457,41,1,0,0,0,458,459,3,44,22,0,459,460,
  	5,41,0,0,460,461,3,42,21,0,461,465,1,0,0,0,462,465,3,44,22,0,463,465,
  	5,42,0,0,464,458,1,0,0,0,464,462,1,0,0,0,464,463,1,0,0,0,465,43,1,0,0,
  	0,466,472,3,46,23,0,467,468,3,46,23,0,468,469,5,43,0,0,469,470,3,44,22,
  	0,470,472,1,0,0,0,471,466,1,0,0,0,471,467,1,0,0,0,472,45,1,0,0,0,473,
  	475,5,94,0,0,474,473,1,0,0,0,474,475,1,0,0,0,475,476,1,0,0,0,476,477,
  	3,66,33,0,477,481,3,48,24,0,478,482,5,100,0,0,479,482,5,101,0,0,480,482,
  	3,66,33,0,481,478,1,0,0,0,481,479,1,0,0,0,481,480,1,0,0,0,482,47,1,0,
  	0,0,483,490,5,44,0,0,484,490,5,45,0,0,485,490,5,46,0,0,486,490,5,47,0,
  	0,487,490,5,11,0,0,488,490,5,48,0,0,489,483,1,0,0,0,489,484,1,0,0,0,489,
  	485,1,0,0,0,489,486,1,0,0,0,489,487,1,0,0,0,489,488,1,0,0,0,490,49,1,
  	0,0,0,491,492,5,49,0,0,492,493,5,99,0,0,493,51,1,0,0,0,494,495,7,2,0,
  	0,495,53,1,0,0,0,496,497,5,50,0,0,497,498,5,51,0,0,498,499,5,52,0,0,499,
  	55,1,0,0,0,500,501,5,50,0,0,501,502,5,53,0,0,502,503,5,54,0,0,503,57,
  	1,0,0,0,504,505,5,50,0,0,505,506,5,55,0,0,506,507,5,56,0,0,507,59,1,0,
  	0,0,508,509,5,16,0,0,509,510,5,57,0,0,510,61,1,0,0,0,511,512,5,16,0,0,
  	512,513,5,58,0,0,513,63,1,0,0,0,514,515,5,50,0,0,515,516,5,59,0,0,516,
  	65,1,0,0,0,517,518,5,60,0,0,518,519,5,101,0,0,519,67,1,0,0,0,79,73,83,
  	87,90,93,103,113,122,133,138,144,165,169,172,175,178,188,194,199,204,
  	210,213,216,219,225,233,240,251,254,257,261,264,267,271,275,279,283,286,
  	289,294,297,300,306,312,315,322,327,332,336,342,347,350,353,359,362,365,
  	371,374,377,383,386,389,395,398,404,407,413,416,419,421,427,437,443,452,
  	464,471,474,481,489
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
    setState(73);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::T__1: {
        enterOuterAlt(_localctx, 1);
        setState(68);
        load_data_query();
        break;
      }

      case KnoBABQueryParser::T__3:
      case KnoBABQueryParser::T__5:
      case KnoBABQueryParser::T__6: {
        enterOuterAlt(_localctx, 2);
        setState(69);
        display_data();
        break;
      }

      case KnoBABQueryParser::T__11: {
        enterOuterAlt(_localctx, 3);
        setState(70);
        model_query();
        break;
      }

      case KnoBABQueryParser::T__29:
      case KnoBABQueryParser::AUTO_TIMED: {
        enterOuterAlt(_localctx, 4);
        setState(71);
        query_plan();
        break;
      }

      case KnoBABQueryParser::T__0: {
        enterOuterAlt(_localctx, 5);
        setState(72);
        set_benchmarking_file();
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
    setState(75);
    match(KnoBABQueryParser::T__0);
    setState(76);
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
    setState(78);
    match(KnoBABQueryParser::T__1);
    setState(83);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::HRF: {
        setState(79);
        match(KnoBABQueryParser::HRF);
        break;
      }

      case KnoBABQueryParser::TAB: {
        setState(80);
        match(KnoBABQueryParser::TAB);
        break;
      }

      case KnoBABQueryParser::XES: {
        setState(81);
        match(KnoBABQueryParser::XES);
        break;
      }

      case KnoBABQueryParser::T__7: {
        setState(82);
        log();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(85);
    antlrcpp::downCast<Load_data_queryContext *>(_localctx)->file = match(KnoBABQueryParser::STRING);
    setState(87);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      setState(86);
      with_data();
      break;
    }

    default:
      break;
    }
    setState(90);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__49) {
      setState(89);
      no_stats();
    }
    setState(93);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__15) {
      setState(92);
      with_missing();
    }
    setState(95);
    match(KnoBABQueryParser::T__2);
    setState(96);
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
    setState(113);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::T__3: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::DisplayContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(98);
        match(KnoBABQueryParser::T__3);
        setState(103);
        _errHandler->sync(this);
        switch (_input->LA(1)) {
          case KnoBABQueryParser::ACT_TABLE: {
            setState(99);
            match(KnoBABQueryParser::ACT_TABLE);
            break;
          }

          case KnoBABQueryParser::CNT_TABLE: {
            setState(100);
            match(KnoBABQueryParser::CNT_TABLE);
            break;
          }

          case KnoBABQueryParser::ATT_TABLE: {
            setState(101);
            match(KnoBABQueryParser::ATT_TABLE);
            setState(102);
            antlrcpp::downCast<DisplayContext *>(_localctx)->attr = match(KnoBABQueryParser::STRING);
            break;
          }

        default:
          throw NoViableAltException(this);
        }
        setState(105);
        match(KnoBABQueryParser::T__4);
        setState(106);
        match(KnoBABQueryParser::STRING);
        break;
      }

      case KnoBABQueryParser::T__5: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::ListContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(107);
        match(KnoBABQueryParser::T__5);
        setState(108);
        _la = _input->LA(1);
        if (!((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & 7) != 0)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(109);
        match(KnoBABQueryParser::T__4);
        setState(110);
        match(KnoBABQueryParser::STRING);
        break;
      }

      case KnoBABQueryParser::T__6: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::DroplogContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(111);
        match(KnoBABQueryParser::T__6);
        setState(112);
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
  enterRule(_localctx, 8, KnoBABQueryParser::RuleLog);

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
    setState(115);
    match(KnoBABQueryParser::T__7);
    setState(116);
    match(KnoBABQueryParser::LPAREN);
    setState(122);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(117);
        trace();
        setState(118);
        match(KnoBABQueryParser::T__8); 
      }
      setState(124);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    }
    setState(125);
    trace();
    setState(126);
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
  enterRule(_localctx, 10, KnoBABQueryParser::RuleTrace);
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
    setState(128);
    data_part();
    setState(129);
    match(KnoBABQueryParser::T__9);
    setState(133);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == KnoBABQueryParser::LABEL) {
      setState(130);
      event();
      setState(135);
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
  enterRule(_localctx, 12, KnoBABQueryParser::RuleEvent);
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
    setState(136);
    match(KnoBABQueryParser::LABEL);
    setState(138);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::LPAREN) {
      setState(137);
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
  enterRule(_localctx, 14, KnoBABQueryParser::RuleData_part);
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
    setState(140);
    match(KnoBABQueryParser::LPAREN);
    setState(144);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == KnoBABQueryParser::T__59) {
      setState(141);
      field();
      setState(146);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(147);
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
  enterRule(_localctx, 16, KnoBABQueryParser::RuleField);
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
    var();
    setState(150);
    match(KnoBABQueryParser::T__10);
    setState(151);
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
  enterRule(_localctx, 18, KnoBABQueryParser::RuleModel_query);
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
    match(KnoBABQueryParser::T__11);
    setState(154);
    model();
    setState(155);
    match(KnoBABQueryParser::T__12);
    setState(156);
    antlrcpp::downCast<Model_queryContext *>(_localctx)->ensemble = match(KnoBABQueryParser::STRING);
    setState(157);
    match(KnoBABQueryParser::T__13);
    setState(158);
    antlrcpp::downCast<Model_queryContext *>(_localctx)->env = match(KnoBABQueryParser::STRING);

    setState(159);
    match(KnoBABQueryParser::T__14);
    setState(160);
    antlrcpp::downCast<Model_queryContext *>(_localctx)->plan = match(KnoBABQueryParser::STRING);
    setState(165);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__15) {
      setState(162);
      match(KnoBABQueryParser::T__15);
      setState(163);
      match(KnoBABQueryParser::T__16);
      setState(164);
      antlrcpp::downCast<Model_queryContext *>(_localctx)->operators = match(KnoBABQueryParser::STRING);
    }
    setState(169);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__17) {
      setState(167);
      match(KnoBABQueryParser::T__17);
      setState(168);
      antlrcpp::downCast<Model_queryContext *>(_localctx)->nothreads = match(KnoBABQueryParser::INTNUMBER);
    }
    setState(172);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__3) {
      setState(171);
      display_qp();
    }
    setState(175);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__22) {
      setState(174);
      atomization();
    }
    setState(178);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__27) {
      setState(177);
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
  enterRule(_localctx, 20, KnoBABQueryParser::RuleModel);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(188);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::T__18: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::File_modelContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(180);
        match(KnoBABQueryParser::T__18);
        setState(181);
        match(KnoBABQueryParser::STRING);
        break;
      }

      case KnoBABQueryParser::T__19: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::DeclaresContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(182);
        match(KnoBABQueryParser::T__19);
        setState(183);
        data_aware_declare();
        break;
      }

      case KnoBABQueryParser::T__20: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::TopnContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(184);
        match(KnoBABQueryParser::T__20);
        setState(185);
        match(KnoBABQueryParser::STRING);
        setState(186);
        match(KnoBABQueryParser::T__21);
        setState(187);
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
  enterRule(_localctx, 22, KnoBABQueryParser::RuleAtomization);
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
    setState(190);
    match(KnoBABQueryParser::T__22);
    setState(194);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx)) {
    case 1: {
      setState(191);
      match(KnoBABQueryParser::T__15);
      setState(192);
      match(KnoBABQueryParser::T__23);
      setState(193);
      antlrcpp::downCast<AtomizationContext *>(_localctx)->label = match(KnoBABQueryParser::STRING);
      break;
    }

    default:
      break;
    }
    setState(199);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__24) {
      setState(196);
      match(KnoBABQueryParser::T__24);
      setState(197);
      match(KnoBABQueryParser::T__25);
      setState(198);
      antlrcpp::downCast<AtomizationContext *>(_localctx)->strlen = match(KnoBABQueryParser::INTNUMBER);
    }
    setState(204);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__15) {
      setState(201);
      match(KnoBABQueryParser::T__15);
      setState(202);
      match(KnoBABQueryParser::T__26);
      setState(203);
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
  enterRule(_localctx, 24, KnoBABQueryParser::RuleGrounding);
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
    setState(206);
    match(KnoBABQueryParser::T__27);
    setState(210);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__15) {
      setState(207);
      match(KnoBABQueryParser::T__15);
      setState(208);
      match(KnoBABQueryParser::T__26);
      setState(209);
      antlrcpp::downCast<GroundingContext *>(_localctx)->strategy = match(KnoBABQueryParser::STRING);
    }
    setState(213);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
    case 1: {
      setState(212);
      no_preliminary_fill();
      break;
    }

    default:
      break;
    }
    setState(216);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
    case 1: {
      setState(215);
      act_for_attributes();
      break;
    }

    default:
      break;
    }
    setState(219);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__49) {
      setState(218);
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
  enterRule(_localctx, 26, KnoBABQueryParser::RuleDisplay_qp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(221);
    match(KnoBABQueryParser::T__3);
    setState(222);
    match(KnoBABQueryParser::T__28);
   
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
  enterRule(_localctx, 28, KnoBABQueryParser::RuleQuery_plan);
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
    setState(225);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::AUTO_TIMED) {
      setState(224);
      match(KnoBABQueryParser::AUTO_TIMED);
    }
    setState(227);
    match(KnoBABQueryParser::T__29);
    setState(228);
    match(KnoBABQueryParser::STRING);
    setState(229);
    match(KnoBABQueryParser::LPAREN);
    setState(231); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(230);
      declare_syntax();
      setState(233); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == KnoBABQueryParser::T__20);
    setState(235);
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
  enterRule(_localctx, 30, KnoBABQueryParser::RuleDeclare_syntax);
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
    setState(237);
    match(KnoBABQueryParser::T__20);
    setState(238);
    match(KnoBABQueryParser::STRING);
    setState(240);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::T__31) {
      setState(239);
      has_args();
    }
    setState(242);
    match(KnoBABQueryParser::T__30);
    setState(243);
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
  enterRule(_localctx, 32, KnoBABQueryParser::RuleHas_args);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(245);
    match(KnoBABQueryParser::T__31);
    setState(246);
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
  size_t startState = 34;
  enterRecursionRule(_localctx, 34, KnoBABQueryParser::RuleLtlf, precedence);

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
    setState(342);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::INIT: {
        _localctx = _tracker.createInstance<InitContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(249);
        match(KnoBABQueryParser::INIT);
        setState(251);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 27, _ctx)) {
        case 1: {
          setState(250);
          match(KnoBABQueryParser::TIMED);
          break;
        }

        default:
          break;
        }
        setState(254);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
        case 1: {
          setState(253);
          declare_arguments();
          break;
        }

        default:
          break;
        }
        setState(257);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx)) {
        case 1: {
          setState(256);
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
        setState(259);
        match(KnoBABQueryParser::END);
        setState(261);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
        case 1: {
          setState(260);
          match(KnoBABQueryParser::TIMED);
          break;
        }

        default:
          break;
        }
        setState(264);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 31, _ctx)) {
        case 1: {
          setState(263);
          declare_arguments();
          break;
        }

        default:
          break;
        }
        setState(267);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
        case 1: {
          setState(266);
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
        setState(269);
        match(KnoBABQueryParser::LAST);
        setState(271);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 33, _ctx)) {
        case 1: {
          setState(270);
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
        setState(273);
        match(KnoBABQueryParser::FIRST);
        setState(275);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 34, _ctx)) {
        case 1: {
          setState(274);
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
        setState(277);
        match(KnoBABQueryParser::EXISTS);
        setState(279);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::NEGATED) {
          setState(278);
          match(KnoBABQueryParser::NEGATED);
        }
        setState(281);
        match(KnoBABQueryParser::INTNUMBER);
        setState(283);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 36, _ctx)) {
        case 1: {
          setState(282);
          match(KnoBABQueryParser::TIMED);
          break;
        }

        default:
          break;
        }
        setState(286);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 37, _ctx)) {
        case 1: {
          setState(285);
          declare_arguments();
          break;
        }

        default:
          break;
        }
        setState(289);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 38, _ctx)) {
        case 1: {
          setState(288);
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
        setState(291);
        match(KnoBABQueryParser::ABSENCE);
        setState(292);
        match(KnoBABQueryParser::INTNUMBER);
        setState(294);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
        case 1: {
          setState(293);
          match(KnoBABQueryParser::TIMED);
          break;
        }

        default:
          break;
        }
        setState(297);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 40, _ctx)) {
        case 1: {
          setState(296);
          declare_arguments();
          break;
        }

        default:
          break;
        }
        setState(300);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 41, _ctx)) {
        case 1: {
          setState(299);
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
        setState(302);
        match(KnoBABQueryParser::NEXT);
        setState(303);
        ltlf(13);
        break;
      }

      case KnoBABQueryParser::IF: {
        _localctx = _tracker.createInstance<IfteContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(304);
        match(KnoBABQueryParser::IF);
        setState(306);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::TIMED) {
          setState(305);
          match(KnoBABQueryParser::TIMED);
        }
        setState(308);
        ltlf(0);
        setState(309);
        match(KnoBABQueryParser::THEN);
        setState(310);
        ltlf(0);
        setState(312);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::THETA) {
          setState(311);
          match(KnoBABQueryParser::THETA);
        }
        setState(315);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::INV) {
          setState(314);
          match(KnoBABQueryParser::INV);
        }
        setState(317);
        match(KnoBABQueryParser::ELSE);
        setState(318);
        ltlf(9);
        break;
      }

      case KnoBABQueryParser::BOX: {
        _localctx = _tracker.createInstance<BoxContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(320);
        match(KnoBABQueryParser::BOX);
        setState(322);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::TIMED) {
          setState(321);
          match(KnoBABQueryParser::TIMED);
        }
        setState(324);
        ltlf(7);
        break;
      }

      case KnoBABQueryParser::DIAMOND: {
        _localctx = _tracker.createInstance<DiamondContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(325);
        match(KnoBABQueryParser::DIAMOND);
        setState(327);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::TIMED) {
          setState(326);
          match(KnoBABQueryParser::TIMED);
        }
        setState(329);
        ltlf(6);
        break;
      }

      case KnoBABQueryParser::NEGATED: {
        _localctx = _tracker.createInstance<NotContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(330);
        match(KnoBABQueryParser::NEGATED);
        setState(332);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == KnoBABQueryParser::TIMED) {
          setState(331);
          match(KnoBABQueryParser::TIMED);
        }
        setState(334);
        ltlf(0);
        setState(336);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 48, _ctx)) {
        case 1: {
          setState(335);
          match(KnoBABQueryParser::PRESERVE);
          break;
        }

        default:
          break;
        }
        break;
      }

      case KnoBABQueryParser::T__33: {
        _localctx = _tracker.createInstance<ParenContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(338);
        match(KnoBABQueryParser::T__33);
        setState(339);
        ltlf(0);
        setState(340);
        match(KnoBABQueryParser::T__34);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(421);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(419);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 68, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<OrContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(344);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(345);
          match(KnoBABQueryParser::OR);
          setState(347);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::TIMED) {
            setState(346);
            match(KnoBABQueryParser::TIMED);
          }
          setState(350);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(349);
            match(KnoBABQueryParser::THETA);
          }
          setState(353);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(352);
            match(KnoBABQueryParser::INV);
          }
          setState(355);
          ltlf(12);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<AndContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(356);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(357);
          match(KnoBABQueryParser::AND);
          setState(359);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::TIMED) {
            setState(358);
            match(KnoBABQueryParser::TIMED);
          }
          setState(362);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(361);
            match(KnoBABQueryParser::THETA);
          }
          setState(365);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(364);
            match(KnoBABQueryParser::INV);
          }
          setState(367);
          ltlf(11);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<ImplicationContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(368);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(369);
          match(KnoBABQueryParser::T__32);
          setState(371);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::TIMED) {
            setState(370);
            match(KnoBABQueryParser::TIMED);
          }
          setState(374);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(373);
            match(KnoBABQueryParser::THETA);
          }
          setState(377);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(376);
            match(KnoBABQueryParser::INV);
          }
          setState(379);
          ltlf(10);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<UntilContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(380);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(381);
          match(KnoBABQueryParser::UNTIL);
          setState(383);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::TIMED) {
            setState(382);
            match(KnoBABQueryParser::TIMED);
          }
          setState(386);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(385);
            match(KnoBABQueryParser::THETA);
          }
          setState(389);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(388);
            match(KnoBABQueryParser::INV);
          }
          setState(391);
          ltlf(8);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<And_futureContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(392);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(393);
          match(KnoBABQueryParser::T__35);
          setState(395);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(394);
            match(KnoBABQueryParser::THETA);
          }
          setState(398);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(397);
            match(KnoBABQueryParser::INV);
          }
          setState(400);
          ltlf(3);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<And_next_globallyContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(401);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(402);
          match(KnoBABQueryParser::T__36);
          setState(404);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(403);
            match(KnoBABQueryParser::THETA);
          }
          setState(407);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(406);
            match(KnoBABQueryParser::INV);
          }
          setState(409);
          ltlf(2);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<And_globallyContext>(_tracker.createInstance<LtlfContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleLtlf);
          setState(410);

          if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
          setState(411);
          match(KnoBABQueryParser::T__37);
          setState(413);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::THETA) {
            setState(412);
            match(KnoBABQueryParser::THETA);
          }
          setState(416);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == KnoBABQueryParser::INV) {
            setState(415);
            match(KnoBABQueryParser::INV);
          }
          setState(418);
          ltlf(1);
          break;
        }

        default:
          break;
        } 
      }
      setState(423);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 69, _ctx);
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
  enterRule(_localctx, 36, KnoBABQueryParser::RuleData_aware_declare);
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
    setState(427);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == KnoBABQueryParser::STRING) {
      setState(424);
      declare();
      setState(429);
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
  enterRule(_localctx, 38, KnoBABQueryParser::RuleDeclare);
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
    setState(452);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 73, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::Nary_propContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(430);
      antlrcpp::downCast<Nary_propContext *>(_localctx)->name = match(KnoBABQueryParser::STRING);
      setState(431);
      match(KnoBABQueryParser::T__33);
      setState(435); 
      _errHandler->sync(this);
      alt = 1;
      do {
        switch (alt) {
          case 1: {
                setState(432);
                fields();
                setState(433);
                match(KnoBABQueryParser::T__38);
                break;
              }

        default:
          throw NoViableAltException(this);
        }
        setState(437); 
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 71, _ctx);
      } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
      setState(439);
      fields();
      setState(440);
      match(KnoBABQueryParser::T__34);
      setState(443);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == KnoBABQueryParser::T__39) {
        setState(441);
        match(KnoBABQueryParser::T__39);
        setState(442);
        prop();
      }
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::Unary_propContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(445);
      antlrcpp::downCast<Unary_propContext *>(_localctx)->name = match(KnoBABQueryParser::STRING);
      setState(446);
      match(KnoBABQueryParser::T__33);
      setState(447);
      fields();
      setState(448);
      match(KnoBABQueryParser::T__38);
      setState(449);
      match(KnoBABQueryParser::INTNUMBER);
      setState(450);
      match(KnoBABQueryParser::T__34);
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
  enterRule(_localctx, 40, KnoBABQueryParser::RuleFields);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(454);
    antlrcpp::downCast<FieldsContext *>(_localctx)->label = match(KnoBABQueryParser::STRING);
    setState(455);
    match(KnoBABQueryParser::T__38);
    setState(456);
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
  enterRule(_localctx, 42, KnoBABQueryParser::RuleProp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(464);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 74, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::DisjContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(458);
      prop_within_dijunction();
      setState(459);
      match(KnoBABQueryParser::T__40);
      setState(460);
      prop();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::Conj_or_atomContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(462);
      prop_within_dijunction();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::TopContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(463);
      match(KnoBABQueryParser::T__41);
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
  enterRule(_localctx, 44, KnoBABQueryParser::RuleProp_within_dijunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(471);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 75, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::In_atomContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(466);
      atom();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<KnoBABQueryParser::Atom_conjContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(467);
      atom();
      setState(468);
      match(KnoBABQueryParser::T__42);
      setState(469);
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
  enterRule(_localctx, 46, KnoBABQueryParser::RuleAtom);
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
    setState(474);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KnoBABQueryParser::NEGATED) {
      setState(473);
      antlrcpp::downCast<AtomContext *>(_localctx)->isnegated = match(KnoBABQueryParser::NEGATED);
    }
    setState(476);
    var();
    setState(477);
    rel();
    setState(481);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::NUMBER: {
        setState(478);
        match(KnoBABQueryParser::NUMBER);
        break;
      }

      case KnoBABQueryParser::STRING: {
        setState(479);
        match(KnoBABQueryParser::STRING);
        break;
      }

      case KnoBABQueryParser::T__59: {
        setState(480);
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
  enterRule(_localctx, 48, KnoBABQueryParser::RuleRel);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(489);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case KnoBABQueryParser::T__43: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::LtContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(483);
        match(KnoBABQueryParser::T__43);
        break;
      }

      case KnoBABQueryParser::T__44: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::LeqContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(484);
        match(KnoBABQueryParser::T__44);
        break;
      }

      case KnoBABQueryParser::T__45: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::GtContext>(_localctx);
        enterOuterAlt(_localctx, 3);
        setState(485);
        match(KnoBABQueryParser::T__45);
        break;
      }

      case KnoBABQueryParser::T__46: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::GeqContext>(_localctx);
        enterOuterAlt(_localctx, 4);
        setState(486);
        match(KnoBABQueryParser::T__46);
        break;
      }

      case KnoBABQueryParser::T__10: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::EqContext>(_localctx);
        enterOuterAlt(_localctx, 5);
        setState(487);
        match(KnoBABQueryParser::T__10);
        break;
      }

      case KnoBABQueryParser::T__47: {
        _localctx = _tracker.createInstance<KnoBABQueryParser::NeqContext>(_localctx);
        enterOuterAlt(_localctx, 6);
        setState(488);
        match(KnoBABQueryParser::T__47);
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
  enterRule(_localctx, 50, KnoBABQueryParser::RuleDeclare_arguments);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(491);
    match(KnoBABQueryParser::T__48);
    setState(492);
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
  enterRule(_localctx, 52, KnoBABQueryParser::RuleDeclare_act_target);
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
    setState(494);
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
  enterRule(_localctx, 54, KnoBABQueryParser::RuleNo_preliminary_fill);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(496);
    match(KnoBABQueryParser::T__49);
    setState(497);
    match(KnoBABQueryParser::T__50);
    setState(498);
    match(KnoBABQueryParser::T__51);
   
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
  enterRule(_localctx, 56, KnoBABQueryParser::RuleAct_for_attributes);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(500);
    match(KnoBABQueryParser::T__49);
    setState(501);
    match(KnoBABQueryParser::T__52);
    setState(502);
    match(KnoBABQueryParser::T__53);
   
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
  enterRule(_localctx, 58, KnoBABQueryParser::RuleNo_cream_off);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(504);
    match(KnoBABQueryParser::T__49);
    setState(505);
    match(KnoBABQueryParser::T__54);
    setState(506);
    match(KnoBABQueryParser::T__55);
   
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
  enterRule(_localctx, 60, KnoBABQueryParser::RuleWith_data);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(508);
    match(KnoBABQueryParser::T__15);
    setState(509);
    match(KnoBABQueryParser::T__56);
   
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
  enterRule(_localctx, 62, KnoBABQueryParser::RuleWith_missing);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(511);
    match(KnoBABQueryParser::T__15);
    setState(512);
    match(KnoBABQueryParser::T__57);
   
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
  enterRule(_localctx, 64, KnoBABQueryParser::RuleNo_stats);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(514);
    match(KnoBABQueryParser::T__49);
    setState(515);
    match(KnoBABQueryParser::T__58);
   
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
  enterRule(_localctx, 66, KnoBABQueryParser::RuleVar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(517);
    match(KnoBABQueryParser::T__59);
    setState(518);
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
    case 17: return ltlfSempred(antlrcpp::downCast<LtlfContext *>(context), predicateIndex);

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
