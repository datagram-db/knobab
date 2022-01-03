/*
 * DeclareModelParse.cpp
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
// Created by giacomo on 09/03/21.
//

#include <yaucl/bpm/structures/declare/DeclareModelParse.h>
#include <yaucl/data/json.h>
#include <yaucl/bpm/structures/declare/DADLexer.h>

DeclareModelParse::DeclareModelParse() /*: do_renaming{true}*/ {}

std::vector<DeclareDataAware> DeclareModelParse::load(std::istream &stream) {
    ///bool tmp = do_renaming;
    ///do_renaming = do_xes_renaming;
    antlr4::ANTLRInputStream input(stream);
    DADLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    DADParser parser(&tokens);
    auto result = visit(parser.data_aware_declare()).as<std::vector<DeclareDataAware>>();
    ///do_renaming = tmp;
    return result;
}

antlrcpp::Any DeclareModelParse::visitData_aware_declare(DADParser::Data_aware_declareContext *ctx) {
    std::vector<DeclareDataAware> v;
    if (ctx) {
        for (DADParser::DeclareContext* ptr : ctx->declare()) {
            v.emplace_back(visit(ptr).as<DeclareDataAware>());
        }
    }
    return {v};
}

antlrcpp::Any DeclareModelParse::visitNary_prop(DADParser::Nary_propContext *ctx) {
    DeclareDataAware dda;
    if (ctx) {
        std::tie (dda.left_act, dda.dnf_left_map) =
                visitFields(ctx->fields(0)).as<std::pair<std::string,
                std::vector<std::unordered_map<std::string, DataPredicate>>>>();
        std::tie (dda.right_act, dda.dnf_right_map) =
                visitFields(ctx->fields(1)).as<std::pair<std::string,
                        std::vector<std::unordered_map<std::string, DataPredicate>>>>();
        /*if (do_renaming)
            std::transform(dda.left_act.begin(), dda.left_act.end(), dda.left_act.begin(), ::tolower);*/
        for (auto& ref : dda.dnf_left_map) {
            for (auto& cp : ref) {
                cp.second.label = dda.left_act;
                for (auto& subitem : cp.second.BiVariableConditions) {
                    subitem.label = dda.left_act;
                    subitem.labelRHS = dda.left_act;
                }
                if (!cp.second.varRHS.empty()) {
                    cp.second.labelRHS = dda.left_act;
                }
            }
        }

        /*if (do_renaming)
            std::transform(dda.right_act.begin(), dda.right_act.end(), dda.right_act.begin(), ::tolower);*/
        for (auto& ref : dda.dnf_right_map) {
            for (auto& cp : ref) {
                cp.second.label = dda.right_act;
                for (auto& subitem : cp.second.BiVariableConditions) {
                    subitem.label = dda.right_act;
                    subitem.labelRHS = dda.right_act;
                }
                if (!cp.second.varRHS.empty()) {
                    cp.second.labelRHS = dda.right_act;
                }
            }
        }

        if (ctx->prop()) {
            dda.conjunctive_map = visit(ctx->prop()).as<std::vector<std::unordered_map<std::string, DataPredicate>>>();
            for (auto& ref : dda.conjunctive_map) {
                for (auto& cp : ref) {
                    cp.second.label = dda.left_act;
                    cp.second.labelRHS = dda.right_act;
                    for (auto& subitem : cp.second.BiVariableConditions) {
                        subitem.label = dda.left_act;
                        subitem.labelRHS = dda.right_act;
                    }
                }
            }
        }

        dda.n = 0;
        dda.casusu =
                magic_enum::enum_cast<declare_templates>(ctx->LABEL()->getText()).value();
    }
    return {dda};
}

antlrcpp::Any DeclareModelParse::visitUnary_prop(DADParser::Unary_propContext *ctx) {
    DeclareDataAware dda;
    if (ctx) {
        std::tie (dda.left_act, dda.dnf_left_map) =
                visitFields(ctx->fields()).as<std::pair<std::string,
                std::vector<std::unordered_map<std::string, DataPredicate>>>>();
        /*if (do_renaming)
            std::transform(dda.left_act.begin(), dda.left_act.end(), dda.left_act.begin(), ::tolower);*/
        for (auto& ref : dda.dnf_left_map) {
            for (auto& cp : ref) {
                cp.second.label = dda.left_act;
            }
        }
        dda.n = std::stoul(ctx->INTNUMBER()->getText());
        dda.casusu =
                magic_enum::enum_cast<declare_templates>(ctx->LABEL()->getText()).value();
    }
    return {dda};
}

antlrcpp::Any DeclareModelParse::visitFields(DADParser::FieldsContext *ctx) {
    std::pair<std::string,
            std::vector<std::unordered_map<std::string, DataPredicate>>> cp;
    if (ctx) {
        cp.first = ctx->LABEL()->getText();
        cp.second = visit(ctx->prop()).as<std::vector<std::unordered_map<std::string, DataPredicate>>>();
    }
    return {cp};
}

antlrcpp::Any DeclareModelParse::visitDisj(DADParser::DisjContext *ctx) {
    std::vector<std::unordered_map<std::string, DataPredicate>> v;
    if (ctx) {
        std::unordered_map<std::string, DataPredicate> M = visit(ctx->prop_within_dijunction()).as<std::unordered_map<std::string, DataPredicate>>();
        v = visit(ctx->prop()).as<std::vector<std::unordered_map<std::string, DataPredicate>>>();
        v.emplace_back(M);
    }
    return {v};
}

antlrcpp::Any DeclareModelParse::visitConj_or_atom(DADParser::Conj_or_atomContext *ctx) {
    std::vector<std::unordered_map<std::string, DataPredicate>> v;
    if (ctx) {
        std::unordered_map<std::string, DataPredicate> M = visit(ctx->prop_within_dijunction()).as<std::unordered_map<std::string, DataPredicate>>();
        v.emplace_back(M);
    }
    return {v};
}

antlrcpp::Any DeclareModelParse::visitTop(DADParser::TopContext *ctx) {
    return {std::vector<std::unordered_map<std::string, DataPredicate>>{}};
}

antlrcpp::Any DeclareModelParse::visitIn_atom(DADParser::In_atomContext *ctx) {
    std::unordered_map<std::string, DataPredicate> v;
    if (ctx) {
        DataPredicate pred = visitAtom(ctx->atom()).as<DataPredicate>();
        v[pred.var] = pred;
    }
    return {v};
}

antlrcpp::Any DeclareModelParse::visitAtom_conj(DADParser::Atom_conjContext *ctx) {
    std::unordered_map<std::string, DataPredicate> v;
    if (ctx) {
        v = visit(ctx->prop_within_dijunction()).as<std::unordered_map<std::string, DataPredicate>>();
        DataPredicate baseCase = visitAtom(ctx->atom()).as<DataPredicate>();
        auto it = v.find(baseCase.var);
        if (it != v.end())
            it->second.intersect_with(baseCase);
        else {
            if (baseCase.isBiVariableCondition()) {
                v[baseCase.var].var = baseCase.var;
                v[baseCase.var].BiVariableConditions.emplace_back(baseCase);
            } else {
                v[baseCase.var] = baseCase;
            }
        }
    }
    return {v};
}

antlrcpp::Any DeclareModelParse::visitAtom(DADParser::AtomContext *ctx) {
    if (ctx) {
        DataPredicate pred;
        auto vec = ctx->VAR();
        pred.var = vec.at(0)->getText();
        pred.casusu = visit(ctx->rel()).as<numeric_atom_cases>();
        if (ctx->STRING()) {
            pred.value = UNESCAPE(ctx->STRING()->getText());
        } else if (ctx->NUMBER()) {
            pred.value = std::stod(ctx->NUMBER()->getText());
        } else if (vec.size() > 1) {
            pred.varRHS = vec.at(1)->getText();
        } else {
            pred.value = 0.0;
        }
        return {pred};
    }
    return {};
}

antlrcpp::Any DeclareModelParse::visitLt(DADParser::LtContext *ctx) {
    return {LT};
}

antlrcpp::Any DeclareModelParse::visitLeq(DADParser::LeqContext *ctx) {
    return {LEQ};
}

antlrcpp::Any DeclareModelParse::visitGt(DADParser::GtContext *ctx) {
    return {GT};
}

antlrcpp::Any DeclareModelParse::visitGeq(DADParser::GeqContext *ctx) {
    return {GEQ};
}

antlrcpp::Any DeclareModelParse::visitEq(DADParser::EqContext *ctx) {
    return {EQ};
}

antlrcpp::Any DeclareModelParse::visitNeq(DADParser::NeqContext *ctx) {
    return {NEQ};
}

ltlf DeclareModelParse::load_model_to_semantics(std::istream &stream, bool is_simplified_xes) {
    ltlf formula = ltlf::True();
    std::vector<DeclareDataAware> V;
    if (is_simplified_xes) {
        V = DeclareDataAware::load_simplified_declare_model(stream);
    } else {
        V = load(stream);
    }

    bool first = true;
    for (const DeclareDataAware& x : V) {
        ///std::cout << "----" << x.toFiniteSemantics() << std::endl;
        if (first) {
            formula = x.toFiniteSemantics()/*.simplify()*/;
            first = false;
        } else {
            formula = ltlf::And(x.toFiniteSemantics()/*.simplify()*/, formula);
        }
    }
    return formula;
}
