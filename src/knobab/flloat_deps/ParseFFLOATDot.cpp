//
// Created by giacomo on 14/11/2021.
//

#include <knobab/flloat_deps/DOTLexer.h>
#include <yaucl/data/json.h>
#include <knobab/flloat_deps/FLLOATSimplePropParser.h>
#include "knobab/flloat_deps/ParseFFLOATDot.h"


FlexibleFA<size_t, std::string> getFAFromLTLFEdges(const std::unordered_set<std::string> &SigmaAll,
                                                   const NodeLabelBijectionFA<std::string, easy_prop> &parsing_result2) {
    throw std::runtime_error("ERROR: TO IMPLEMENT!");
    FlexibleFA<size_t, std::string> result;
    std::unordered_map<size_t, size_t> idConv;
    for (size_t nodeId = 0, N = parsing_result2.maximumNodeId(); nodeId<N; nodeId++) {
        size_t src = result.addNewNodeWithLabel(nodeId);
        idConv[nodeId] = src;
    }
    for (size_t finalNodes : parsing_result2.fini()) {
        result.addToFinalNodesFromId(idConv.at(finalNodes));
    }
    for (size_t initialNodes : parsing_result2.init()) {
        result.addToInitialNodesFromId(idConv.at(initialNodes));
    }
    for (size_t nodeId = 0, N = parsing_result2.maximumNodeId(); nodeId<N; nodeId++) {
        size_t src = idConv.at(nodeId);
        for (const auto&edge : parsing_result2.outgoingEdges(nodeId)) {
            // std::cerr << edge.first << std::endl;
            size_t dst = idConv.at(edge.second);
            for (const std::string& act : SigmaAll) {
#if TODO
                if (edge.first.easy_interpret(act)) {
                    //   std::cerr << '\t' << act << std::endl;
                    result.addNewEdgeFromId(src, dst, act);
                }
#endif
            }
        }
    }
    return result;
}


FlexibleFA<size_t, std::string> ParseFFLOATDot::parse(std::istream& stream,
                                                      const std::unordered_set<std::string>& SigmaAll) {
    throw std::runtime_error("DEPRECATED!");
    antlr4::ANTLRInputStream input(stream);
    DOTLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();
    DOTParser parser(&tokens);
    for (const auto& ptr : parser.graph()->stmt_list()->stmt()) {
        visitStmt(ptr);
    }

    return getFAFromLTLFEdges(SigmaAll, parsing_result);
}

antlrcpp::Any ParseFFLOATDot::visitGraph(DOTParser::GraphContext *context) {
    throw std::runtime_error("Unexpected invocation: visitGraph");
}

antlrcpp::Any ParseFFLOATDot::visitStmt_list(DOTParser::Stmt_listContext *context) {
    throw std::runtime_error("Unexpected invocation: visitStmt_list");
}

antlrcpp::Any ParseFFLOATDot::visitStmt(DOTParser::StmtContext *context) {
    if (context) {
        if (context->node_stmt()) {
            return visitNode_stmt(context->node_stmt());
        } else if (context->edge_stmt()) {
            return visitEdge_stmt(context->edge_stmt());
        }
    }
    return {};
}

antlrcpp::Any ParseFFLOATDot::visitAttr_stmt(DOTParser::Attr_stmtContext *context) {
    throw std::runtime_error("Unexpected invocation: visitAttr_stmt");
}

antlrcpp::Any ParseFFLOATDot::visitAttr_list(DOTParser::Attr_listContext *context) {
    throw std::runtime_error("Unexpected invocation: visitAttr_list");
}

antlrcpp::Any ParseFFLOATDot::visitA_list(DOTParser::A_listContext *context) {
    throw std::runtime_error("Unexpected invocation: visitA_list");
}

antlrcpp::Any ParseFFLOATDot::visitEdgeRHS(DOTParser::EdgeRHSContext *context) {
    throw std::runtime_error("Unexpected invocation: visitEdgeRHS");
}

antlrcpp::Any ParseFFLOATDot::visitEdgeop(DOTParser::EdgeopContext *context) {
    throw std::runtime_error("Unexpected invocation: visitEdgeop");

}

antlrcpp::Any ParseFFLOATDot::visitNode_stmt(DOTParser::Node_stmtContext *context) {
    if (context) {
        std::string id = context->node_id()->getText();
        if (id == "fake") return {};
        int idV = parsing_result.addUniqueStateOrGetExisting(id);
        if (context->attr_list())
            for (const auto& ls : context->attr_list()->a_list()) {
                auto v = ls->id();
                for (size_t i = 0, N = v.size()/2; i<N; i++) {
                    size_t offset_attr = i*2;
                    size_t offset_val = i*2+1;
                    std::string key = v.at(offset_attr)->getText();
                    std::string val = v.at(offset_val)->getText();
                    if ((key == "root") && (val == "true")) {
                        parsing_result.addToInitialNodesFromId(idV);
                    } else if ((key == "shape") && (val == "doublecircle")) {
                        parsing_result.addToFinalNodesFromId(idV);
                    }
                }
            }
    }
    return {};
}

antlrcpp::Any ParseFFLOATDot::visitNode_id(DOTParser::Node_idContext *context) {
    throw std::runtime_error("Unexpected invocation: visitNode_id");
}

antlrcpp::Any ParseFFLOATDot::visitPort(DOTParser::PortContext *context) {
    throw std::runtime_error("Unexpected invocation: visitPort");
}

antlrcpp::Any ParseFFLOATDot::visitSubgraph(DOTParser::SubgraphContext *context) {
    throw std::runtime_error("Unexpected invocation: visitSubgraph");
}

antlrcpp::Any ParseFFLOATDot::visitId(DOTParser::IdContext *context) {
    throw std::runtime_error("Unexpected invocation: visitId");
}

antlrcpp::Any ParseFFLOATDot::visitEdge_stmt(DOTParser::Edge_stmtContext *context) {
    if (context) {
        std::string src = context->node_id()->getText();
        if (src == "fake") return {};
        std::string dst = context->edgeRHS()->node_id(0)->getText();
        if (dst == "fake") return {};
        int srcId = parsing_result.getId(src);
        int dstId = parsing_result.getId(dst);
        for (const auto& ls : context->attr_list()->a_list()) {
            auto v = ls->id();
            for (size_t i = 0, N = v.size()/2; i<N; i++) {
                size_t offset_attr = i*2;
                size_t offset_val = i*2+1;
                std::string key = v.at(offset_attr)->getText();
                std::string val = v.at(offset_val)->getText();
                if ((key == "label")) {
                    try {
                        val = UNESCAPE(val);
                    } catch(...) {
                        //
                    }
                    FLLOATSimplePropParser parser;
                    std::stringstream ss;
                    ss.str(val);
                    auto f = parser.parse(ss);
                    if (this->need_back_conversion) {
                        throw std::runtime_error("ERROR: TO IMPLEMENT!");
#ifdef TODO
                        parsing_result.addNewEdgeFromId(srcId, dstId, f.replace_with_unique_name(*this->back_conv));
#endif
                    } else {
                        parsing_result.addNewEdgeFromId(srcId, dstId, f);
                    }
                }
            }
        }
    }
    return {};
}