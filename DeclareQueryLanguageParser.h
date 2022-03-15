//
// Created by giacomo on 15/03/2022.
//

#ifndef KNOBAB_DECLAREQUERYLANGUAGEPARSER_H
#define KNOBAB_DECLAREQUERYLANGUAGEPARSER_H

#include <istream>
#include <knobab/queries/LTLfQuery.h>
#include <knobab/queries/LTLfQueryParser.h>

class DeclareQueryLanguageParser {


    void parse(std::istream& stream);

    LTLfQuery visitQuery(LTLfQueryParser::QueryContext *pContext) {
        if (!pContext) return LTLfQuery{}; // if the
    }
};


#endif //KNOBAB_DECLAREQUERYLANGUAGEPARSER_H
