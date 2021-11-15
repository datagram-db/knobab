
// Generated from src/yaucl/bpm/declare/DAD.g by ANTLR 4.9.3

#pragma once


#include "antlr4-runtime.h"
#include "DADParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by DADParser.
 */
class  DADVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by DADParser.
   */
    virtual antlrcpp::Any visitData_aware_declare(DADParser::Data_aware_declareContext *context) = 0;

    virtual antlrcpp::Any visitNary_prop(DADParser::Nary_propContext *context) = 0;

    virtual antlrcpp::Any visitUnary_prop(DADParser::Unary_propContext *context) = 0;

    virtual antlrcpp::Any visitFields(DADParser::FieldsContext *context) = 0;

    virtual antlrcpp::Any visitDisj(DADParser::DisjContext *context) = 0;

    virtual antlrcpp::Any visitConj_or_atom(DADParser::Conj_or_atomContext *context) = 0;

    virtual antlrcpp::Any visitTop(DADParser::TopContext *context) = 0;

    virtual antlrcpp::Any visitIn_atom(DADParser::In_atomContext *context) = 0;

    virtual antlrcpp::Any visitAtom_conj(DADParser::Atom_conjContext *context) = 0;

    virtual antlrcpp::Any visitAtom(DADParser::AtomContext *context) = 0;

    virtual antlrcpp::Any visitLt(DADParser::LtContext *context) = 0;

    virtual antlrcpp::Any visitLeq(DADParser::LeqContext *context) = 0;

    virtual antlrcpp::Any visitGt(DADParser::GtContext *context) = 0;

    virtual antlrcpp::Any visitGeq(DADParser::GeqContext *context) = 0;

    virtual antlrcpp::Any visitEq(DADParser::EqContext *context) = 0;

    virtual antlrcpp::Any visitNeq(DADParser::NeqContext *context) = 0;


};

