//
// Created by giacomo on 16/07/21.
//

#ifndef AUTOMATA_JOIN_GRAPH_JOIN_PM_H
#define AUTOMATA_JOIN_GRAPH_JOIN_PM_H

#include <map>
#include <vector>
#include <string>
#include <yaucl/graphs//adjacency_entry.h>
#include <unordered_set>

/**
 * Definition of a graph automaton as an efficient representation to be combined with other graphs, too!
 */
struct graph_join_pm {
    ssize_t V_size, E_size;
    std::map<std::string, std::vector<adjacency_entry>> vertex_hash_primary_index;
    std::vector<std::pair<std::string, size_t>> vertex_id_secondary_index;
    std::unordered_set<size_t> starting_point;
    size_t size() const;

    graph_join_pm();
    virtual ~graph_join_pm() {
        vertex_hash_primary_index.clear();
        vertex_id_secondary_index.clear();
        starting_point.clear();
    };
    graph_join_pm(const graph_join_pm&) = default;
    graph_join_pm& operator=(const graph_join_pm&) = default;
    graph_join_pm(graph_join_pm&&) = default;
    graph_join_pm& operator=(graph_join_pm&&) = default;

    friend std::ostream &operator<<(std::ostream &os, const graph_join_pm &pm);
};

/**
 * Adds a node inside a graph
 */
size_t add_node(graph_join_pm& graph, const std::string& node_label, bool is_starting = false, bool is_final = false);

/**
 * Resolves the node inside a grpah by its id
 */
adjacency_entry& resolve_node(graph_join_pm& g, size_t id);

const adjacency_entry &resolve_node_const(const graph_join_pm &g, size_t id);

void add_edge(graph_join_pm& g, adjacency_entry& ref_src, size_t dst, double edge_weight);

void add_edge(graph_join_pm& g, size_t src, size_t dst, double edge_weight);

void dot(struct graph_join_pm& graph, std::ostream& os, bool ignoreEdgeLabels = false);

void do_heavy_debug(struct graph_join_pm& graph, size_t id);
void do_heavy_debug(struct graph_join_pm& graph);

#endif //AUTOMATA_JOIN_GRAPH_JOIN_PM_H
