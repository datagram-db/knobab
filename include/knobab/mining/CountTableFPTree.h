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

/*
ForAll(Item) struct FPNode {
    const Item                           item;
    uint64_t                             frequency;
    std::shared_ptr<FPNode<Item>>        node_link;
    std::weak_ptr<FPNode<Item>>          parent;
    std::vector<std::shared_ptr<FPNode>> children;

    FPNode(const Item& item, const std::shared_ptr<FPNode<Item>>& parent) :
            item( item ), frequency( 1 ), node_link( nullptr ), parent( parent ), children() {
    }
};



struct FPTree {
    std::shared_ptr<FPNode<act_t>>                 root;
//    std::map<act_t, std::shared_ptr<FPNode<act_t>>> header_table;
    std::vector<std::shared_ptr<FPNode<act_t>>> header_table;
    uint64_t                                      minimum_support_threshold;
    size_t max_act_id;

    // order items by decreasing frequency
    struct frequency_comparator
    {
        bool operator()(const std::pair<size_t, uint64_t> &lhs, const std::pair<size_t, uint64_t> &rhs) const
        {
            return std::tie(lhs.second, lhs.first) > std::tie(rhs.second, rhs.first);
        }
    };


    FPTree(const std::vector<Transaction>& transactions,
           uint64_t minimum_support_threshold,
           uint64_t max_act_id) :
            root(std::make_shared<FPNode<act_t>>( 0, nullptr )), header_table(max_act_id, nullptr), max_act_id(max_act_id),
            minimum_support_threshold( minimum_support_threshold ) {
        // scan the transactions counting the frequency of each item
        std::map<act_t, uint64_t> frequency_by_item;
        for ( const Transaction& transaction : transactions ) {
            for ( const auto& item : transaction ) {
                if (!frequency_by_item.contains(item))
                    frequency_by_item[item] = 1;
                else
                    frequency_by_item[item] += 1;
            }
        }

        // keep only items which have a frequency greater or equal than the minimum support threshold
        for ( auto it = frequency_by_item.cbegin(); it != frequency_by_item.cend(); ) {
            const uint64_t item_frequency = (*it).second;
            if ( item_frequency < minimum_support_threshold ) { frequency_by_item.erase( it++ ); }
            else { ++it; }
        }


        std::set<std::pair<act_t , uint64_t>, frequency_comparator> items_ordered_by_frequency(frequency_by_item.cbegin(), frequency_by_item.cend());

        // start tree construction

        // scan the transactions again
        for ( const Transaction& transaction : transactions ) {
            auto curr_fpnode = root;

            // select and sort the frequent items in transaction according to the order of items_ordered_by_frequency
            for ( const auto& pair : items_ordered_by_frequency ) {
                const auto& item = pair.first;

                // check if item is contained in the current transaction
                if ( std::find( transaction.cbegin(), transaction.cend(), item ) != transaction.cend() ) {
                    // insert item in the tree

                    // check if curr_fpnode has a child curr_fpnode_child such that curr_fpnode_child.item = item
                    const auto it = std::find_if(
                            curr_fpnode->children.cbegin(), curr_fpnode->children.cend(),  [item](const std::shared_ptr<FPNode<act_t>>& fpnode) {
                                return fpnode->item == item;
                            } );
                    if ( it == curr_fpnode->children.cend() ) {
                        // the child doesn't exist, create a new node
                        const auto curr_fpnode_new_child = std::make_shared<FPNode<act_t>>( item, curr_fpnode );

                        // add the new node to the tree
                        curr_fpnode->children.push_back( curr_fpnode_new_child );

                        // update the node-link structure
                        if ( header_table.at( curr_fpnode_new_child->item )  != nullptr ) {
                            auto prev_fpnode = header_table[curr_fpnode_new_child->item];
                            while ( prev_fpnode->node_link ) { prev_fpnode = prev_fpnode->node_link; }
                            prev_fpnode->node_link = curr_fpnode_new_child;
                        }
                        else {
                            header_table[curr_fpnode_new_child->item] = curr_fpnode_new_child;
                        }

                        // advance to the next node of the current transaction
                        curr_fpnode = curr_fpnode_new_child;
                    }
                    else {
                        // the child exist, increment its frequency
                        auto curr_fpnode_child = *it;
                        ++curr_fpnode_child->frequency;

                        // advance to the next node of the current transaction
                        curr_fpnode = curr_fpnode_child;
                    }
                }
            }
        }
    }

    FPTree(const CountTemplate& transactions,
           uint64_t minimum_support_threshold,
           uint64_t max_act_id) :
            root(std::make_shared<FPNode<act_t>>( 0, nullptr )),
            header_table(max_act_id, nullptr),
            max_act_id(max_act_id),
            minimum_support_threshold( minimum_support_threshold ) {
        // scan the transactions counting the frequency of each item

        std::map<act_t, uint64_t> frequency_by_item;
        for ( const auto& transaction_item : transactions.table ) {
            if (transaction_item.id.parts.event_id > 0) {
                frequency_by_item[transaction_item.id.parts.act]++;
            }
        }

        // keep only items which have a frequency greater or equal than the minimum support threshold
        for ( auto it = frequency_by_item.cbegin(); it != frequency_by_item.cend(); ) {
            const uint64_t item_frequency = (*it).second;
            if ( item_frequency < minimum_support_threshold ) { frequency_by_item.erase( it++ ); }
            else { ++it; }
        }


        std::set<std::pair<act_t, uint64_t>, frequency_comparator> items_ordered_by_frequency(frequency_by_item.cbegin(), frequency_by_item.cend());

        // start tree construction

        // scan the transactions again
        for (uint32_t transaction_id = 0, N = transactions.nTraces(); transaction_id<N; transaction_id++ ) {
            auto curr_fpnode = root;

            // select and sort the frequent items in transaction according to the order of items_ordered_by_frequency
            for ( const auto& pair : items_ordered_by_frequency ) {
                const act_t& item = pair.first;

                // check if item is contained in the current transaction
                if (transactions.resolve_length(item, transaction_id) > 0) {
                    // insert item in the tree

                    // check if curr_fpnode has a child curr_fpnode_child such that curr_fpnode_child.item = item
                    const auto it = std::find_if(
                            curr_fpnode->children.cbegin(), curr_fpnode->children.cend(),  [item](const std::shared_ptr<FPNode<act_t>>& fpnode) {
                                return fpnode->item == item;
                            } );
                    if ( it == curr_fpnode->children.cend() ) {
                        // the child doesn't exist, create a new node
                        const auto curr_fpnode_new_child = std::make_shared<FPNode<act_t>>( item, curr_fpnode );

                        // add the new node to the tree
                        curr_fpnode->children.push_back( curr_fpnode_new_child );

                        // update the node-link structure
                        if ( header_table.at( curr_fpnode_new_child->item ) != nullptr ) {
                            auto prev_fpnode = header_table[curr_fpnode_new_child->item];
                            while ( prev_fpnode->node_link ) { prev_fpnode = prev_fpnode->node_link; }
                            prev_fpnode->node_link = curr_fpnode_new_child;
                        }
                        else {
                            header_table[curr_fpnode_new_child->item] = curr_fpnode_new_child;
                        }

                        // advance to the next node of the current transaction
                        curr_fpnode = curr_fpnode_new_child;
                    }
                    else {
                        // the child exist, increment its frequency
                        auto curr_fpnode_child = *it;
                        curr_fpnode_child->frequency+=1;

                        // advance to the next node of the current transaction
                        curr_fpnode = curr_fpnode_child;
                    }
                }
            }
        }
    }

    bool empty() const {
        assert( root );
        return root->children.size() == 0;
    }
};

ForAll(Item) bool contains_single_path(const std::shared_ptr<FPNode<Item>>& fpnode) {
    assert( fpnode );
    if ( fpnode->children.size() == 0 ) { return true; }
    if ( fpnode->children.size() > 1 ) { return false; }
    return contains_single_path( fpnode->children.front() );
}



std::set<Pattern> fptree_growth(const FPTree& fptree, size_t max_depth = 2);*/

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
                                   size_t min_length = 2) ;


static inline std::vector<std::pair<size_t, std::unordered_set<act_t>>> fpgrowth(const CountTemplate& transactions,
                                                                                    uint64_t max_act_id,
                                                                                    std::vector<std::pair<act_t, size_t>>& final_element_for_scan,
                                                                                    size_t minsupport = 1,
                                                                                    size_t min_length = 2) {
    struct fpgrowth_node tree{0};
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
        fpgrowth_expand(ptr, rit->first, rit->second, std::unordered_set<act_t>{}, in_tree, firstpointer, results,
                minsupport, min_length);
    }
    size_t N = transactions.nAct();
    for (const auto& ref : results) {
        for (const auto& i : ref.second) {
            if (i>=N)
                exit(2);
        }
    }


    return results;
}

#endif  // FPTREE_HPP


#endif //KNOBAB_COUNTTABLEFPTREE_H
