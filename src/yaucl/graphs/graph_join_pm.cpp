//
// Created by giacomo on 16/07/21.
//

#include "yaucl/graphs/graph_join_pm.h"

graph_join_pm::graph_join_pm() : V_size{0}, E_size{0} {}

std::ostream &operator<<(std::ostream &os, const graph_join_pm &pm) {
    os << "Graph(|V|=" << pm.V_size << ")" << std::endl;
    for (const auto& cp : pm.vertex_hash_primary_index) {
        os << " - Bucket: " << cp.first << std::endl;
        for (const auto& entry: cp.second) {
            os << entry << std::endl;
        }
    }
    return os;
}

size_t graph_join_pm::size() const {
    return V_size+E_size;
}

#include <cassert>

size_t add_node(graph_join_pm &graph, const std::string &node_label, bool is_starting, bool is_final) {
    size_t init_id = (graph).V_size;
    auto& elem = (graph).vertex_hash_primary_index[(node_label)];
    graph.vertex_id_secondary_index.emplace_back(node_label, elem.size());
    auto& result =  elem.emplace_back(init_id, is_starting, is_final);
    if (is_starting) {
        graph.starting_point.emplace(init_id);
    }
    (graph).V_size++;
    ///assert((graph).V_size == result.id+1);
    assert(init_id == result.id);
    return result.id;
}



#include <cassert>

adjacency_entry &resolve_node(graph_join_pm &g, size_t id) {
    auto& ref = g.vertex_id_secondary_index.at(id);
    assert(g.vertex_hash_primary_index.at(ref.first).at(ref.second).id == id);
    return g.vertex_hash_primary_index.at(ref.first).at(ref.second);
}

void add_edge(graph_join_pm &g, adjacency_entry &ref_src, size_t dst, double edge_weight) {
    auto& ref_dst = g.vertex_id_secondary_index.at(dst);
    (ref_src).outgoing_hash_to_outgoingNodeIdWithWeight[(ref_dst.first)].emplace_back((dst), (edge_weight));
    g.E_size++;
}

void add_edge(graph_join_pm &g, size_t src, size_t dst, double edge_weight) {
    add_edge(g, resolve_node(g, src), dst, edge_weight);
}

void dot(graph_join_pm &graph, std::ostream &os, bool ignoreEdgeLabels)  {
    os << "digraph {\nrankdir=LR;\n";
    for (size_t node_id : graph.starting_point) {
        os << "\tfake" << node_id << " [style=invisible]" << std::endl;
    }
    for (int node_id /*: getNodeIds()*/ = 0; node_id<graph.V_size; node_id++) {
        os << '\t' << node_id;
        auto& node = resolve_node(graph, node_id);
        bool hasFinal = node.is_final;
        bool hasInitial = node.is_starting;
        os << " [";
        if (hasFinal || hasInitial) {
            if (hasInitial)
                os << "root=true ";
            if (hasFinal) {
                os << "shape=doublecircle ";
            }

        }
        os << "label=\"" << graph.vertex_id_secondary_index.at(node_id).first << ":" << node_id <<"\"";
        os << "]" << std::endl;
    }
    for (size_t node_id : graph.starting_point) {
        os << "\tfake" << node_id << " -> " << node_id << " [style=bold]" << std::endl;
    }
    for (int node_id = 0; node_id<graph.V_size; node_id++) {

        auto& node = resolve_node(graph, node_id);
        for (const auto& label_and_subadj :
                node.outgoing_hash_to_outgoingNodeIdWithWeight) {
            for (const auto v : label_and_subadj.second) {
                os << '\t' << node_id << " -> " << v.first;
                if (!ignoreEdgeLabels)
                    os << " [label=" << v.first << "]";
                os << std::endl;
            }
        }
    }
    os << "}";
}

const adjacency_entry &resolve_node_const(const graph_join_pm &g, size_t id) {
    auto& ref = g.vertex_id_secondary_index.at(id);
    return g.vertex_hash_primary_index.at(ref.first).at(ref.second);
}

#include <iostream>

void do_heavy_debug(graph_join_pm &graph, size_t id) {
    static size_t jjjj = 0;
    bool test = (graph.vertex_id_secondary_index.size() > id);
    if (!test) {
        std::cerr << "ERROR! (1) the Id is greater than the maximum id for a vertex in the current graph!" << std::endl;
        test = false;
    }
    std::string label;
    size_t offset = 0;
    bool found = false;
    jjjj++;
    if ((jjjj) == 68638)
        std::cerr << "BREAK" << std::endl;
    for (const auto& ref : graph.vertex_hash_primary_index) {
        if (found) break;
        for (size_t i = 0, N = ref.second.size(); i<N; i++) {
            const auto& v_out = ref.second.at(i);
            if (v_out.id == id) {
                ///std::cout << id << " Found @" << ref.first << " element #" << i << std::endl;
                label = ref.first;
                offset = i;
                found = true;
                break;
            }
        }
    }
    if (!found) {
        std::cerr << " ERROR: id " << id << " was not found! " << std::endl;
        assert(found);
    }
    if (test) {
        auto& ref = graph.vertex_id_secondary_index.at(id);
        if (ref.first != label) {
            std::cerr << "ERROR (2): node at position " << id << " has not the expected label of '" << label << "', but rather the following: " << ref.first << std::endl;
            test = false;
        }
        if (!graph.vertex_hash_primary_index.contains(ref.first)) {
            std::cerr << "ERROR (3): label addressed by node with id " << id << ", i.e.," << ref.first << ", is not in the graph!" << std::endl;
            test = false;
        } else {
            auto&    ref2 = graph.vertex_hash_primary_index.at(label);
            if (ref.first != label) {
                ref2 = graph.vertex_hash_primary_index.at(ref.first);
                if (ref2.size() <= offset) {
                    std::cerr << "ERROR (4bis): the offset " << offset << " for retrieved label " << ref.first << " is not there, as there are less elements!" << std::endl;
                    test = false;
                } else if (ref2.at(offset).id != id) {
                    std::cerr << "ERROR (5bis): the offset " << offset << " for retrieved label " << ref.first << " is there, but pointing to a different id, " << ref2.at(offset).id << std::endl;
                    test = false;
                }
            }
            if (ref2.size() <= offset) {
                std::cerr << "ERROR (4): the offset " << offset << " for expected label " << label << " is not there, as there are less elements!" << std::endl;
                test = false;
            } else if (ref2.at(offset).id != id) {
                std::cerr << "ERROR (5): the offset " << offset << " for expected label " << label << " is there, but pointing to a different id, " << ref2.at(offset).id << std::endl;
                test = false;
            }
        }
    }
    assert(test);
}

void do_heavy_debug(graph_join_pm &graph) {
    assert(graph.V_size == graph.vertex_id_secondary_index.size());
    ///std::cout << "{[";
    for (size_t i = 0, N = graph.V_size; i<N; i++) {
        ///std::cout << i << ", ";
        do_heavy_debug(graph, i);
    }
    ///std::cout << "]}" << std::endl;
}
