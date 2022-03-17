//
// Created by giacomo on 15/03/2022.
//

#ifndef KNOBAB_DECLAREQUERYLANGUAGEPARSER_H
#define KNOBAB_DECLAREQUERYLANGUAGEPARSER_H

#include <istream>
#include <knobab/queries/LTLfQuery.h>
#include <knobab/queries/LTLfQueryParser.h>
#include <knobab/queries/LTLfQueryVisitor.h>
#include <yaucl/bpm/structures/declare/DeclareDataAware.h>
#include <stack>

inline unsigned char decleare_templates_determine(LTLfQueryParser::Declare_argumentsContext* ptr) {
    if (!ptr) return DECLARE_TYPE_NONE;
    else return std::stoull(ptr->INTNUMBER()->getText());
}

inline LeafType decleare_leaf_determine(LTLfQueryParser::Declare_act_targetContext* ptr) {
    if (!ptr) return NoneLeaf;
    else if (ptr->ACTIVATION()) return ActivationLeaf;
    else if (ptr->TARGET()) return TargetLeaf;
    else return NoneLeaf;
}

#include <unordered_map>
#include <yaucl/functional/assert.h>

#define GET_TIMING(context)     (((context) && ((context)->TIMED() != nullptr)) || (isAutoTimed && fromNowOnTimed))
#define DEFAULT_TIMING()     ((isAutoTimed && fromNowOnTimed))
#define ASSERT_ON_TIMING(context)       DEBUG_ASSERT((!isAutoTimed) || (GET_TIMING(context) == fromNowOnTimed))
#define ASSERT_ON_GIVEN_TIMING(b)       DEBUG_ASSERT(((b) == fromNowOnTimed))

struct DeclareQueryLanguageParser : public LTLfQueryVisitor {
    bool fromNowOnTimed = false;
    bool isAutoTimed = false;
    std::stack<bool> fromNowOnTimedStack;
    unsigned char max_aspect;
    std::unordered_map<std::string, std::unordered_map<std::string, LTLfQuery>> planname_to_declare_to_ltlf;

    /// VISITORS
    LTLfQuery visitQuery(LTLfQueryParser::QueryContext *pContext);
    void parse(std::istream& stream);
    void analyse(const std::string& approach, const DeclareDataAware& clause);

    /// Inherited visitors
    antlrcpp::Any visitInit(LTLfQueryParser::InitContext *context) override;
    antlrcpp::Any visitEnd(LTLfQueryParser::EndContext *context) override;
    antlrcpp::Any visitAbsence(LTLfQueryParser::AbsenceContext *context) override;
    antlrcpp::Any visitExists(LTLfQueryParser::ExistsContext *context) override;
    antlrcpp::Any visitNext(LTLfQueryParser::NextContext *context) override;
    antlrcpp::Any visitOr(LTLfQueryParser::OrContext *context) override;
    antlrcpp::Any visitAnd(LTLfQueryParser::AndContext *context) override;
    antlrcpp::Any visitBox(LTLfQueryParser::BoxContext *context) override;
    antlrcpp::Any visitDiamond(LTLfQueryParser::DiamondContext *context) override;
    antlrcpp::Any visitNot(LTLfQueryParser::NotContext *context) override;
    antlrcpp::Any visitAnd_next_globally(LTLfQueryParser::And_next_globallyContext *context) override;
    antlrcpp::Any visitAnd_future(LTLfQueryParser::And_futureContext *context) override;
    antlrcpp::Any visitParen(LTLfQueryParser::ParenContext *context) override;
    antlrcpp::Any visitImplication(LTLfQueryParser::ImplicationContext *context) override;
    antlrcpp::Any visitUntil(LTLfQueryParser::UntilContext *context) override;
    antlrcpp::Any visitIfte(LTLfQueryParser::IfteContext *context) override;

    /// Useless inherited visitors
    antlrcpp::Any visitQuery_plan(LTLfQueryParser::Query_planContext *context) override { return {}; }
    antlrcpp::Any visitDeclare_syntax(LTLfQueryParser::Declare_syntaxContext *context) override { return {}; }
    antlrcpp::Any visitHas_args(LTLfQueryParser::Has_argsContext *context) override { return {}; }
    antlrcpp::Any visitDeclare_arguments(LTLfQueryParser::Declare_argumentsContext *context) override { return {}; }
    antlrcpp::Any visitQuery_plans(LTLfQueryParser::Query_plansContext *context) override { return {}; }
    antlrcpp::Any visitDeclare_act_target(LTLfQueryParser::Declare_act_targetContext *context) override { return {}; }
};


#endif //KNOBAB_DECLAREQUERYLANGUAGEPARSER_H
