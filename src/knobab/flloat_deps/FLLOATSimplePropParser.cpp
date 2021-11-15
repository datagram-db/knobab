/*
 * FLLOATSimplePropParser.cpp
 * This file is part of bpm21
 *
 * Copyright (C) 2021 - Giacomo Bergami
 *
 * bpm21 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * bpm21 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bpm21. If not, see <http://www.gnu.org/licenses/>.
 */


//
// Created by giacomo on 13/03/21.
//

#include "bzdb/flloat_deps/FLLOATSimplePropParser.h"
#include <bzdb/flloat_deps/FLLOATPropLexer.h>

ltlf FLLOATSimplePropParser::parse(std::istream &stream) {
    antlr4::ANTLRInputStream input(stream);
    FLLOATPropLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();
    FLLOATPropParser parser(&tokens);
    // Returning the simplified formula, thus in DFN and with the negation pushed towards the leaves
    return visit(parser.statement()).as<ltlf>().nnf().simplify()/*.reduce().oversimplify()*/;
}

antlrcpp::Any FLLOATSimplePropParser::visitAtom(FLLOATPropParser::AtomContext *context) {
    if (context) {
        return ltlf::Act(context->getText());
    }
    return {ltlf::True().negate()};
}

antlrcpp::Any FLLOATSimplePropParser::visitNegation(FLLOATPropParser::NegationContext *context) {
    if (context) {
        return ltlf::Neg(visit(context->statement()).as<ltlf>());
    }
    return {ltlf::True().negate()};
}

antlrcpp::Any FLLOATSimplePropParser::visitParen(FLLOATPropParser::ParenContext *context) {
    if (context) {
        return visit(context->statement());
    }
    return {ltlf::True().negate()};
}

antlrcpp::Any FLLOATSimplePropParser::visitOr(FLLOATPropParser::OrContext *context) {
    if (context) {
        ltlf stmt = ltlf::True().negate();
        bool first = true;
        for (const auto& childPtr: context->statement()) {
            if (first) {
                stmt = visit(childPtr).as<ltlf>();
                first = false;
            } else {
                stmt = ltlf::Or(visit(childPtr).as<ltlf>(), stmt);
            }
        }
        return {stmt};
    }
    return {ltlf::True().negate()};
}

antlrcpp::Any FLLOATSimplePropParser::visitTop(FLLOATPropParser::TopContext *context) {
    return {ltlf::True()};
}

antlrcpp::Any FLLOATSimplePropParser::visitBot(FLLOATPropParser::BotContext *context) {
    return {ltlf::True().negate()};
}

antlrcpp::Any FLLOATSimplePropParser::visitAnd(FLLOATPropParser::AndContext *context) {
    if (context) {
        ltlf stmt = ltlf::True();
        bool first = true;
        for (const auto& childPtr: context->statement()) {
            if (first) {
                stmt = visit(childPtr).as<ltlf>();
                first = false;
            } else {
                stmt = ltlf::And(visit(childPtr).as<ltlf>(), stmt);
            }
        }
        return {stmt};
    }
    return {ltlf::True().negate()};
}
