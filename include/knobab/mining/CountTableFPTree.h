//
// Created by giacomo on 15/09/22.
//

#ifndef KNOBAB_COUNTTABLEFPTREE_H
#define KNOBAB_COUNTTABLEFPTREE_H

#ifndef TUTORIALS_FPTREE_H
#define TUTORIALS_FPTREE_H

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_set>
#include "knobab/server/tables/CountTemplate.h"

#define ForAll(T)       template <typename T>

using Transaction           = std::vector<act_t>;
using TransformedPrefixPath = std::pair<std::vector<act_t>, uint64_t>;
using Pattern               = std::pair<std::unordered_set<act_t>, uint64_t>;
ForAll(T) using GenPattern  = std::pair<std::set<T>, uint64_t>;



#include <map>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <deque>
#include <queue>
#include <cassert>
#include <stack>

/**
 * Trie node required by the fpgrowth algorithm
 * @tparam T
 */
struct fpgrowth_node {
    size_t count, // Number of instances being observed
    tmp_count, // Temporary count while pruning the graph: instead of creating another fpgrowth tree, I'll use views to accelerate the task
    height; // height inducing the visiting order from the leaves
    act_t value; // Pointer to the actual value: this is done so to alleviate the memory from having multiple copies of the object. (the actual singleton will reside in the main fpgrowth table)
    struct fpgrowth_node* parent; // Root for the current node (if any)
    std::unordered_map<act_t, struct fpgrowth_node*> children; // Children mapped by key value
    struct fpgrowth_node* list; // single linked list connecting all the nodes associated to the same object
//    bool current_leaf;  // whether this element is currently a leaf
    fpgrowth_node(act_t k) : value{k}, count{1}, tmp_count{0}, height{0}, parent{nullptr}, children{}, list{nullptr}  {}

    /**
     * Adding a child to the curent node or, if it already exists, incrementing the counter for the existing one
     * @param value     Value associated to the child
     * @return  Either the already-existing child or the newly allocated one
     */
    struct fpgrowth_node* add_child(act_t value);

    ~fpgrowth_node();
};


void fpgrowth_expand(struct fpgrowth_node* tree,
                                   act_t current,
                                   size_t curr_support,
                                   std::unordered_set<act_t> prefix,
                                   const std::unordered_set<struct fpgrowth_node*>& view,
                                   std::vector<struct fpgrowth_node*>& traverse,
                                   std::vector<std::pair<size_t, std::unordered_set<act_t>>>& results,
                                   size_t minsupport = 1,
                                   size_t max_length = 2) ;


#include <limits>
#include <iostream>

static inline std::vector<std::pair<size_t, std::unordered_set<act_t>>> fpgrowth(const CountTemplate& transactions,
                                                                                    uint64_t max_act_id,
                                                                                    std::vector<std::pair<act_t, size_t>>& final_element_for_scan,
                                                                                    size_t minsupport = 1,
                                                                                    size_t min_length = 2) {
    struct fpgrowth_node tree{std::numeric_limits<act_t>::max()};
    final_element_for_scan.clear();

    std::unordered_set<struct fpgrowth_node*> in_tree;
    std::vector<struct fpgrowth_node*> last_pointer(max_act_id, nullptr), firstpointer(max_act_id, nullptr);
    trace_t nTransactions = transactions.nTraces();
    std::vector<std::unordered_set<struct fpgrowth_node*>> createdElements(max_act_id,std::unordered_set<struct fpgrowth_node*> {});
    {
        std::map<act_t, size_t> element; // Fast representation for the support table, for efficiently associating the item to the support value
        for ( const auto& transaction_item : transactions.table ) {
            if (transaction_item.id.parts.event_id > 0) {
                auto it2 = element.emplace(transaction_item.id.parts.act, 1);
                if (!it2.second) it2.first->second++;
            }
        }

        // moving this to a vector, while keeping the items with adequate support
        for (auto it = element.begin(), en = element.end(); it != en; it++) {
            if (it->second >= minsupport)
                final_element_for_scan.emplace_back(*it);
        }
    }
    // Sorting the elements in the table by decreasing support
    std::sort(final_element_for_scan.begin(), final_element_for_scan.end(), [](const std::pair<act_t, size_t>& l, const std::pair<act_t , size_t>& r) {
        return l.second>r.second;
    });
    struct fpgrowth_node* ptr ;
    for (trace_t tId = 0; tId < nTransactions; tId++) { // For each transaction
//        auto e = itx->end();
        ptr = &tree;
        auto it_left = final_element_for_scan.begin();
        auto e_left = final_element_for_scan.end();
        while ((it_left != e_left)) {
            auto f = transactions.resolve_primary_index2(it_left->first).first+tId;
//            auto f = itx->find(it_left->first);
            if (f->id.parts.event_id == 0) {it_left++; continue;}
            else {
                // For each element in the transaction which is also in the support table
                auto& it3 = last_pointer[it_left->first];
                // Expanding the trie with a new child
                ptr = ptr->add_child(it_left->first);
                // Using a set of visited nodes to avoid loops in the list
                bool newInsertion = createdElements[it_left->first].emplace(ptr).second;
                in_tree.insert(ptr); // This node shoudl be part of the view
                if (it3 == nullptr) {
                    // First pointer of the list for traversing the newly established leaves at each novel iteration efficiently
                    firstpointer[it_left->first] = (ptr);
                    it3 = ptr;
                } else {
                    if (!it3) {
                        // First pointer of the list for traversing the newly established leaves at each novel iteration efficiently
                        it3 = ptr;
                    } else if (newInsertion) {
                        // Avoiding loops: adding an element in the list only if required
                        it3->list = ptr;
                        it3 = ptr;
                    }
                }
                it_left++;
            }
        }
    }

    createdElements.clear();

    // results of the itemsets with their support to be returned
    std::vector<std::pair<size_t, std::unordered_set<act_t>>> results;
    // Setting up the new projection of the tree, so to avoid the re-creation of the trees multiple times:
    // setting up a view with the allowed nodes will be more efficient (as it will save the extra allocation cost for
    // the nodes, and the only memory overhead is just the pointer to the nodes in the FPGrowth tree)
    for (auto rit = final_element_for_scan.rbegin(), ren = final_element_for_scan.rend(); rit != ren; rit++) {
        fpgrowth_expand(&tree, rit->first, rit->second, std::unordered_set<act_t>{}, in_tree, firstpointer, results,
                minsupport, min_length);
    }
    size_t N = transactions.nAct();
    for (const auto& ref : results) {
        for (const auto& i : ref.second) {
            if (i>=N){
                std::cerr << "ERROR: " <<i << " => " <<N << std::endl;
                exit(2);
            }
        }
    }


    return results;
}

#endif  // FPTREE_HPP


#endif //KNOBAB_COUNTTABLEFPTREE_H
