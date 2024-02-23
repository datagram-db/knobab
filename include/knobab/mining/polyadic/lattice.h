//
// Created by giacomo on 22/02/24.
//

#ifndef KNOBAB_SERVER_LATTICE_H
#define KNOBAB_SERVER_LATTICE_H

#include <map>
#include <set>
#include <vector>
#include <unordered_set>
#include <limits>

template <typename URI, typename T>
struct lattice {
    std::map<URI, T> id_to_node;
    std::map<URI, std::set<URI>> node_to_children_totallers;
    std::map<URI, std::set<URI>> node_to_children_refiners;
    std::map<URI, std::set<URI>> node_to_parents;

    template <typename Command>
    void reset(Command x) {
        for (auto& [k, v] : id_to_node) {
            x(v);
        }
    }

    lattice() {}
    lattice(const T& empty) : empty{empty} {}

    bool add_node(const URI& key, const T& value) {
        return id_to_node.emplace(key, value).second;
    }

    bool add_node(const URI& key) {
        return id_to_node.emplace(key, empty).second;
    }

    T& get(const URI& key) {
        auto it = id_to_node.find(key);
        if (it == id_to_node.end())
            return empty;
        return it->second;
    }

    const T& get(const URI& key) const {
        auto it = id_to_node.find(key);
        if (it == id_to_node.end())
            return empty;
        return it->second;
    }

    bool add_edge(const URI& parent, const URI& child, bool isChildTotalling = false) {
        if ((!id_to_node.contains(parent)) || (!id_to_node.contains(child)))
            return false;
        if (isChildTotalling)
            node_to_children_totallers[parent].emplace(child);
        else
            node_to_children_refiners[parent].emplace(child);
        node_to_parents[child].insert(parent);
        return true;
    }

    const std::set<URI>& getGeneralisations(const URI& node) const {
        auto it = node_to_parents.find(node);
        if (it == node_to_parents.end())
            return empty_set;
        else
            return it->second;
    }

    std::set<URI> getRefinements(const URI& node) const {
        auto it = node_to_children_totallers.find(node);
        auto it2 = node_to_children_refiners.find(node);
        if ((it == node_to_children_totallers.end()) && (it2 == node_to_children_refiners.end()))
            return empty_set;
        else {
            std::set<URI> result;
            result.insert(it->second.begin(), it->second.end());
            result.insert(it2->second.begin(), it2->second.end());
            return result;
        }
    }

    bool isLeaf(const URI& x ) const {
        return id_to_node.contains(x) &&
               (((!node_to_children_totallers.contains(x) || node_to_children_totallers.at(x).empty()) && (!node_to_children_refiners.contains(x) || node_to_children_refiners.at(x).empty()))
               );
    }


    template <typename QualityMeasure, typename R>
    void visit(const URI& root, const QualityMeasure& Q, R& output) {
        visited_uris.clear();
        visit_rec(root, Q, output);
    }

private:
    std::map<URI, bool> visited_uris;
    template <typename QualityMeasure, typename R>
    bool visit_rec(const URI& x, const QualityMeasure& Q, R& output) {
        auto f = visited_uris.find(x);
        if (f != visited_uris.end())
            return f->second;
        auto it = id_to_node.find(x);
        if (it == id_to_node.end())
            return false;
        if (!Q(x, it->second)) {
            visited_uris[x] = false;
            return false;
        } else if (isLeaf(x)) {
            output(x, it->second);
            visited_uris[x] = true;
            return true;
        } else {
            bool all = true;
            if (node_to_children_refiners.find(x) != node_to_children_refiners.end()) {
                for (const auto& child : node_to_children_refiners.at(x)) {
                    bool testOutcome = visit_rec(child, Q, output);
                    all = all && testOutcome;
                }
            } else {
                all = false;
            }
            if (all) {
                visited_uris[x] = false;
                return false;
            }
            bool some = false;
            if (node_to_children_totallers.find(x) != node_to_children_totallers.end()) {
                for (const auto& child : node_to_children_totallers.at(x)) {
                    if (visit_rec(child, Q, output)) {
                        some = true;
                    }
                }
            }
            if (some) {
                visited_uris[x] = false;
                return false;
            } else {
                output(x, it->second);
                visited_uris[x] = true;
                return true;
            }
        }
    }
    std::set<URI> empty_set;
    T empty;
};


#endif //KNOBAB_SERVER_LATTICE_H
