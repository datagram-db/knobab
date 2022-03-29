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
#include <yaucl/functional/assert.h>

DeclareModelParse::DeclareModelParse() /*: do_renaming{true}*/ {}

std::vector<DeclareDataAware> DeclareModelParse::load(std::istream &stream, bool asConjunctiveModel) {
    ///bool tmp = do_renaming;
    ///do_renaming = do_xes_renaming;
    this->asConjunctiveModel = asConjunctiveModel;
    antlr4::ANTLRInputStream input(stream);
    DADLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    DADParser parser(&tokens);
    auto t1 = std::chrono::high_resolution_clock::now();
    auto result = visitData_aware_declare(parser.data_aware_declare()).as<std::vector<DeclareDataAware>>();
    auto t2 = std::chrono::high_resolution_clock::now();
    /* Getting number of milliseconds as a double. */
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    parsing_time_ms = ms_double.count();
    ///do_renaming = tmp;
    return result;
}

antlrcpp::Any DeclareModelParse::visitData_aware_declare(DADParser::Data_aware_declareContext *ctx) {
    std::vector<DeclareDataAware> v;
    if (ctx) {
        for (DADParser::DeclareContext* ptr : ctx->declare()) {
            auto currVisit = visit(ptr);
            if (currVisit.isNull()) {
                // If the log file has to be interpreted as a model, and therefore as conjunctive,
                // then since one clause fails, all the other will fail in conjunction: therefore,
                // the model is empty
                if (asConjunctiveModel)
                    return {std::vector<DeclareDataAware>{}};
                // Otherwise, discard the invalid element
            } else{
                // Add the valid element, nevertheless!
                v.emplace_back(currVisit.as<DeclareDataAware>());
            }
        }
    }
    return {v};
}

antlrcpp::Any DeclareModelParse::visitNary_prop(DADParser::Nary_propContext *ctx) {
    DeclareDataAware dda;
    if (ctx) {
        auto leftAny = visitFields(ctx->fields(0));
        if (leftAny.isNull())
            return {};
        std::tie (dda.left_act, dda.dnf_left_map) =
                leftAny.as<std::pair<std::string, std::vector<std::unordered_map<std::string, DataPredicate>>>>();

        auto rightAny = visitFields(ctx->fields(1));
        if (rightAny.isNull())
            return {};
        std::tie (dda.right_act, dda.dnf_right_map) =
                rightAny.as<std::pair<std::string,
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
            auto conjAny = visit(ctx->prop());
            if (conjAny.isNull())
                return {};
            dda.conjunctive_map = conjAny.as<std::vector<std::unordered_map<std::string, DataPredicate>>>();
            for (auto& ref : dda.conjunctive_map) {
                for (auto& cp : ref) {
                    DEBUG_ASSERT(cp.second.casusu == numeric_atom_cases::TTRUE); // That is, no data interval should be provided in here!
                    cp.second.label = dda.left_act;
                    cp.second.labelRHS = dda.right_act;
                    for (auto& subitem : cp.second.BiVariableConditions) {
                        DEBUG_ASSERT(subitem.isBiVariableCondition()); // in the conjunction after where, all the predicates should be among variables, only!
                        subitem.label = dda.left_act;
                        subitem.labelRHS = dda.right_act;
                    }
                }
            }
        }

        dda.n = 0;
        dda.casusu = UNESCAPE(ctx->STRING()->getText());
    }
    return {dda};
}

antlrcpp::Any DeclareModelParse::visitUnary_prop(DADParser::Unary_propContext *ctx) {
    DeclareDataAware dda;
    if (ctx) {
        auto leftAny = visitFields(ctx->fields());
        if (leftAny.isNull())
            return {};
        std::tie (dda.left_act, dda.dnf_left_map) =
                leftAny.as<std::pair<std::string, std::vector<std::unordered_map<std::string, DataPredicate>>>>();
        for (auto& ref : dda.dnf_left_map) {
            for (auto& cp : ref) {
                cp.second.label = dda.left_act;
            }
        }
        dda.n = std::stoul(ctx->INTNUMBER()->getText());
        dda.casusu = UNESCAPE(ctx->STRING()->getText());
    }
    return {dda};
}

antlrcpp::Any DeclareModelParse::visitFields(DADParser::FieldsContext *ctx) {
    std::pair<std::string,
            std::vector<std::unordered_map<std::string, DataPredicate>>> cp;
    if (ctx) {
        auto prop = visit(ctx->prop());
        if (prop.isNotNull()) {
            std::pair<std::string,
                    std::vector<std::unordered_map<std::string, DataPredicate>>> cp;
            cp.first = UNESCAPE(ctx->STRING()->getText());
            cp.second = visit(ctx->prop()).as<std::vector<std::unordered_map<std::string, DataPredicate>>>();
            return {cp};
        } else {
            return {};
        }
    }
    return {cp};
}

antlrcpp::Any DeclareModelParse::visitDisj(DADParser::DisjContext *ctx) {
    if (ctx) {
        std::unordered_map<std::string, DataPredicate> M = visit(ctx->prop_within_dijunction()).as<std::unordered_map<std::string, DataPredicate>>();
        auto vAny = visit(ctx->prop());
        if (vAny.isNotNull()) {
            auto v = vAny.as<std::vector<std::unordered_map<std::string, DataPredicate>>>();
            if (v.empty()) {
                // if the element is empty, it means that it evaluates to true, so this can be immediately returned
                return vAny;
            } else {
                // Otherwise, I can add the conjunctive condition if it does not evaluate to false
                if (!M.empty())
                    v.emplace_back(M);
                return {v};
            }
        } else {
            // if it evaluates to false, it can be only changed by M
            if (!M.empty()) {
                std::vector<std::unordered_map<std::string, DataPredicate>> v;
                v.emplace_back(M);
                return {v};
            } else {
                // evaluates to false!
                return {};
            }
        }

    }
    return {};
}

antlrcpp::Any DeclareModelParse::visitConj_or_atom(DADParser::Conj_or_atomContext *ctx) {
    std::vector<std::unordered_map<std::string, DataPredicate>> v;
    if (ctx) {
        std::unordered_map<std::string, DataPredicate> M = visit(ctx->prop_within_dijunction()).as<std::unordered_map<std::string, DataPredicate>>();
        if (!M.empty()) {
            // inserting an element to the disjunction, only if the element is not empty
            // that is, only if it does not evaluate to false
            v.emplace_back(M);
            return {v};
        } else {
            // Otherwise, evaluate that to false as a whole
            return {};
        }
    }
    return {};
}

antlrcpp::Any DeclareModelParse::visitTop(DADParser::TopContext *ctx) {
    return {std::vector<std::unordered_map<std::string, DataPredicate>>{}};
}

antlrcpp::Any DeclareModelParse::visitIn_atom(DADParser::In_atomContext *ctx) {
    std::unordered_map<std::string, DataPredicate> v;
    if (ctx) {
        DataPredicate pred = visitAtom(ctx->atom()).as<DataPredicate>();
        if (pred.isBiVariableCondition()) {
            v[pred.var].var = pred.var;
            v[pred.var].BiVariableConditions.emplace_back(pred);
        } else {
            v[pred.var] = pred;
        }

    }
    return {v};
}

antlrcpp::Any DeclareModelParse::visitAtom_conj(DADParser::Atom_conjContext *ctx) {
    std::unordered_map<std::string, DataPredicate> v;
    if (ctx) {
        v = visit(ctx->prop_within_dijunction()).as<std::unordered_map<std::string, DataPredicate>>();
        // by the reason of grammar, if v is empty, it means that one of the predicates is empty
        // Given that we are in conjunction, this means all the elements of the conjunction will be
        // evaluated to false, too!
        if (v.empty())
            return {v};

        DataPredicate baseCase = visitAtom(ctx->atom()).as<DataPredicate>();
        auto it = v.find(baseCase.var);
        if (it != v.end()) {
            if (!it->second.intersect_with(baseCase)) {
                // if the evaluation reduces to false, the whole conjunction is false! therefore, returning it
                v.clear();
            } // otherwise, the intersection is already effective!
        } else {
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
        auto vars = ctx->var();
        pred.var = UNESCAPE(vars.at(0)->STRING()->getText());
        pred.casusu = visit(ctx->rel()).as<numeric_atom_cases>();
        if (ctx->STRING()) {
            pred.value = UNESCAPE(ctx->STRING()->getText());
        } else if (ctx->NUMBER()) {
            pred.value = std::stod(ctx->NUMBER()->getText());
        } else if (vars.size() > 1) {
            pred.varRHS = UNESCAPE(vars.at(1)->STRING()->getText());
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
    DEBUG_ASSERT(false);
    ltlf formula = ltlf::True();
#if 0
    std::vector<DeclareDataAware> V;
    if (is_simplified_xes) {
        V = DeclareDataAware::load_simplified_declare_model(stream);
    } else {
        V = load(stream, true);
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
#endif
    return formula;
}
