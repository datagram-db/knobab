//
// Created by giacomo on 26/07/21.
//

#include "yaucl/graphs/graph_join_pm_conversion.h"
#include <unordered_map>

void convert_to_novel_graph(const FlexibleFA<std::string, size_t> &old, graph_join_pm& result) {
    std::unordered_map<size_t, size_t> node_remapping_old_to_novel;

    auto V = old.getNodeIds();
    for (size_t node_id : V) {
        size_t new_id = add_node(result, old.getNodeLabel(node_id), old.isInitialNodeByID(node_id), old.isFinalNodeByID(node_id));
        node_remapping_old_to_novel[node_id] = new_id;
    }
    for (size_t node_id : V) {
        for (auto cp : old.outgoingEdges(node_id)) {
            add_edge(result, node_remapping_old_to_novel.at(node_id), node_remapping_old_to_novel.at(cp.second), 1.0);
        }
    }
}

#if 0
#include <filesystem>
#include <graphs/third-party-wrappers/ParseFFLOATDot.h>
#include <novel_graph/graph_join_pm_algorithms.h>

void parseToGraph(graph_join_pm &result, ltlf &formulas_to_dfas, FLLOATScriptRunner &pyscript,
                  const std::string single_line_clause_file, std::unordered_map<std::string, std::string> *ptr,
                  const std::unordered_set<std::string> &SigmaAll) {

    // Serializing the formula to secondary memory
    if (!std::filesystem::exists(single_line_clause_file)) {
        std::ofstream file{single_line_clause_file};
        file << formulas_to_dfas << std::endl;
        file.close();
    }
    std::string gof = single_line_clause_file + "_graph_1.dot";

    // Running the conversion to a graph, in dot file
    if (!std::filesystem::exists(gof)) {
        // Observe: invoking an external program makes it harder to debug:
        // so, I persist the intermediate result, and skip the execution, so
        // to avoid running the external invocation.
        std::filesystem::path slcf_path = single_line_clause_file;
        pyscript.process_expression(std::filesystem::absolute(slcf_path).string());
    }


    // Getting the graphs from FLLOAT
    // Plus, ensuring that all the nodes that have no edges with a specific
    // label expected from the label set, have thus edges to a new sink node
    /*for (size_t i = 1; i <= formulas_to_dfas.size(); i++)*/ {
        ParseFFLOATDot graph_loader;
        graph_loader.need_back_conversion = ptr != nullptr;
        graph_loader.back_conv = ptr;
        std::ifstream graph_operand_file{single_line_clause_file + "_graph_1.dot"};
        {
            graph_join_pm g2;
            {
                auto g = graph_loader
                        .parse(graph_operand_file, SigmaAll)
                        .makeDFAAsInTheory(SigmaAll)
                        .shiftLabelsToNodes();
                g.pruneUnreachableNodes();

                convert_to_novel_graph(g, g2);
            }
            minimize(g2, result);
        }

    }
}
#endif