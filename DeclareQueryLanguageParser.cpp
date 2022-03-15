//
// Created by giacomo on 15/03/2022.
//

#include "DeclareQueryLanguageParser.h"
#include <knobab/queries/LTLfQueryParser.h>
#include <knobab/queries/LTLfQueryLexer.h>
#include <yaucl/data/json.h>

void DeclareQueryLanguageParser::parse(std::istream &stream) {
    antlr4::ANTLRInputStream input(stream);
    LTLfQueryLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    LTLfQueryParser parser(&tokens);
    auto ptr = parser.query_plans();
    if (ptr) {
        for (const auto& query_plan : ptr->query_plan()) {
            std::string query_plan_name = UNESCAPE(query_plan->STRING()->getText());
            bool doAutoTimed = query_plan->AUTO_TIMED();
            for (const auto& declare : query_plan->declare_syntax()) {
                std::string declare_template_name = UNESCAPE(declare->STRING()->getText());

                LTLfQuery q = visitQuery(declare->query());
            }
        }
    }
}
