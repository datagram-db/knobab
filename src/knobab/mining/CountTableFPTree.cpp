
//
// Created by giacomo on 15/09/22.
//

#include "knobab/mining/CountTableFPTree.h"
#include "yaucl/structures/setoids/basics.h"
//
//static inline bool contains_single_path(const FPTree& fptree) {
//    return fptree.empty() || contains_single_path( fptree.root );
//}
//
//std::set<Pattern> fptree_growth(const FPTree& fptree,
//                                size_t max_depth) {
//    if ( (max_depth == 0) || (fptree.empty()) ) { return {}; }
//
//    if ( contains_single_path( fptree ) ) {
//        // generate all possible combinations of the items in the tree
//
//        std::set<Pattern> single_path_patterns;
//
//        // for each node in the tree
//        assert( fptree.root->children.size() == 1 );
//        auto curr_fpnode = fptree.root->children.front();
//        while ( curr_fpnode ) {
//            const auto& curr_fpnode_item = curr_fpnode->item;
//            const uint64_t curr_fpnode_frequency = curr_fpnode->frequency;
//
//            // add a pattern formed only by the item of the current node
//            Pattern new_pattern{ { curr_fpnode_item }, curr_fpnode_frequency };
//            single_path_patterns.insert( new_pattern );
//
//            // create a new pattern by adding the item of the current node to each pattern generated until now
//            for ( const Pattern& pattern : single_path_patterns ) {
//                if (pattern.first.size() == max_depth) continue;
//                Pattern new_pattern{ pattern };
//                new_pattern.first.insert( curr_fpnode_item );
//                assert( curr_fpnode_frequency <= pattern.second );
//                new_pattern.second = curr_fpnode_frequency;
//
//                single_path_patterns.insert( new_pattern );
//            }
//
//            // advance to the next node until the end of the tree
//            assert( curr_fpnode->children.size() <= 1 );
//            if ( curr_fpnode->children.size() == 1 ) { curr_fpnode = curr_fpnode->children.front(); }
//            else { curr_fpnode = nullptr; }
//        }
//
//        return single_path_patterns;
//    }
//    else {
//        // generate conditional fptrees for each different item in the fptree, then join the results
//
//        std::set<Pattern> multi_path_patterns;
//
//        // for each item in the fptree
//        for (act_t curr_item = 0, N = fptree.header_table.size(); curr_item<N; curr_item++ ) {
////            const auto & curr_item = pair.first;
//            auto path_starting_fpnode = fptree.header_table.at(curr_item);
//            auto pair_second = path_starting_fpnode;
//            if (!path_starting_fpnode) continue;
//
//            // build the conditional fptree relative to the current item
//
//            // start by generating the conditional pattern base
//            std::vector<TransformedPrefixPath> conditional_pattern_base;
//
//            // for each path in the header_table (relative to the current item)
//            while ( path_starting_fpnode ) {
//                // construct the transformed prefix path
//
//                // each item in th transformed prefix path has the same frequency (the frequency of path_starting_fpnode)
//                const uint64_t path_starting_fpnode_frequency = path_starting_fpnode->frequency;
//
//                auto curr_path_fpnode = path_starting_fpnode->parent.lock();
//                // check if curr_path_fpnode is already the root of the fptree
//                if ( curr_path_fpnode->parent.lock() ) {
//                    // the path has at least one node (excluding the starting node and the root)
//                    TransformedPrefixPath transformed_prefix_path{ {}, path_starting_fpnode_frequency };
//
//                    while ( curr_path_fpnode->parent.lock() ) {
//                        assert( curr_path_fpnode->frequency >= path_starting_fpnode_frequency );
//                        transformed_prefix_path.first.push_back( curr_path_fpnode->item );
//
//                        // advance to the next node in the path
//                        curr_path_fpnode = curr_path_fpnode->parent.lock();
//                    }
//
//                    conditional_pattern_base.push_back( transformed_prefix_path );
//                }
//
//                // advance to the next path
//                path_starting_fpnode = path_starting_fpnode->node_link;
//            }
//
//            // generate the transactions that represent the conditional pattern base
//            std::vector<Transaction> conditional_fptree_transactions;
//            for ( const TransformedPrefixPath& transformed_prefix_path : conditional_pattern_base ) {
//                const auto& transformed_prefix_path_items = transformed_prefix_path.first;
//                const uint64_t transformed_prefix_path_items_frequency = transformed_prefix_path.second;
//
//                Transaction transaction = transformed_prefix_path_items;
//
//                // add the same transaction transformed_prefix_path_items_frequency times
//                for ( auto i = 0; i < transformed_prefix_path_items_frequency; ++i ) {
//                    conditional_fptree_transactions.push_back( transaction );
//                }
//            }
//
//            // build the conditional fptree relative to the current item with the transactions just generated
//            const FPTree conditional_fptree( conditional_fptree_transactions,
//                                             fptree.minimum_support_threshold ,
//                                             fptree.max_act_id);
//            // call recursively fptree_growth on the conditional fptree (empty fptree: no patterns)
//            std::set<Pattern> conditional_patterns = fptree_growth( conditional_fptree, max_depth-1 );
//
//            // construct patterns relative to the current item using both the current item and the conditional patterns
//            std::set<Pattern> curr_item_patterns;
//
//            // the first pattern is made only by the current item
//            // compute the frequency of this pattern by summing the frequency of the nodes which have the same item (follow the node links)
//            uint64_t curr_item_frequency = 0;
//            auto fpnode = pair_second;
//            while ( fpnode ) {
//                curr_item_frequency += fpnode->frequency;
//                fpnode = fpnode->node_link;
//            }
//            // add the pattern as a result
//            Pattern pattern{ { curr_item }, curr_item_frequency };
//            curr_item_patterns.insert( pattern );
//
//            // the next patterns are generated by adding the current item to each conditional pattern
//            for ( const Pattern& pattern : conditional_patterns ) {
//                Pattern new_pattern{ pattern };
//                new_pattern.first.insert( curr_item );
//                assert( curr_item_frequency >= pattern.second );
//                new_pattern.second = pattern.second;
//                curr_item_patterns.insert( { new_pattern } );
//            }
//
//            // join the patterns generated by the current item with all the other items of the fptree
//            multi_path_patterns.insert( curr_item_patterns.cbegin(), curr_item_patterns.cend() );
//        }
//
//        return multi_path_patterns;
//    }
//}

struct fpgrowth_node* fpgrowth_node::add_child(act_t value) {
    auto it = children.emplace(value, nullptr);
    if (it.second) { // If I am adding a new child
        it.first->second = new fpgrowth_node(value); // Allocating the new child
        it.first->second->parent = this;  // Setting the parent to the current node
        it.first->second->height = height+1;    // Increasing the height, so to exploit topological sort for efficiently scanning the tree from the leaves
    } else {
        it.first->second->height = std::max(height+1,it.first->second->height); // Otherwise, update the nodes' depth depending on my current one
        it.first->second->count++;  // If the child was already there, increment its count
    }
    return it.first->second; // returning the child already being created in the trie
}

fpgrowth_node::~fpgrowth_node() {
    for (const auto& [k,v] : children) delete v; // deleting all of the allocated children so to save memory and avoid memory leaks
}

void fpgrowth_expand(struct fpgrowth_node* tree,
                     act_t current,
                     size_t curr_support,
                     std::unordered_set<act_t> prefix,
                     const std::unordered_set<struct fpgrowth_node*>& view,
                     std::vector<struct fpgrowth_node*>& traverse,
                     std::vector<std::pair<size_t, std::unordered_set<act_t>>>& results,
                     size_t minsupport,
                     size_t min_length) {


    prefix.insert(current); // Adding the current node as part of the prefixù
    if (prefix.size() <= min_length) {
        results.emplace_back(curr_support, prefix); // setting this as another result
    }
    if (prefix.size() >= min_length) return;

//    std::cout << "Step: ";
//    for (const auto& ptr :prefix)
//        std::cout << *ptr << " ";
//    std::cout << *current << std::endl;

    std::unordered_set<struct fpgrowth_node*> visitable,  // Whether the element was already visited in a previous iteration
    in_tree     // Whether the node, for its good support, might be part of the next pruning and therefore part of the view
    ;
    auto comp = []( struct fpgrowth_node* a, struct fpgrowth_node* b ) { return a->height < b->height; }; // As a tree is a DAG, I can exploit the topological order to know in which order extract the nodes from the queue
    std::priority_queue<struct fpgrowth_node*, std::vector<struct fpgrowth_node*>, decltype(comp)> dq; // Priority queue for storing all the nodes, being sorted by order of visit
    auto& it = traverse[current];                                         // Checking whether we are allowed to traverse from this node
    std::vector<struct fpgrowth_node*> new_firsttraverse(traverse.size(), nullptr); // Updating the first_to_traverse table depending on the nodes being available
    std::vector<size_t> toTraverseNext(traverse.size(), 0);                      // Updating the support table while keeping the support of the previous nodes
    std::vector<struct fpgrowth_node*> visitedChild;                       // For each non-leaf node, keeping all the childs that were visited within the view
    std::unordered_set<struct fpgrowth_node*> nView;
    if (it != nullptr) {
        {
            std::unordered_set<struct fpgrowth_node*> visitedFromLeaves;
            struct fpgrowth_node* ptr = it;
            while (ptr) {
                if (view.contains(ptr)) { // If the current element in the linked list is actually part of the view
                    dq.emplace(ptr);
                }
                auto toVisit = ptr;
                while (toVisit != tree) {
                    visitedFromLeaves.insert(toVisit);
                    toVisit = toVisit->parent;
                }
                ptr = ptr->list; // Scanning all the elements in the list
            }
            nView = unordered_intersection(view, visitedFromLeaves);
        }

        while (!dq.empty()) {
            struct fpgrowth_node* dq_ptr = dq.top(); // Next element to be visited
            dq.pop();
            if (!visitable.emplace(dq_ptr).second) continue; // Discarding visiting the node if visited already
            {
                dq_ptr->tmp_count = 0; // Re-setting the counter to zero
//                dq_ptr->current_leaf = true;
                bool isLeaf = (dq_ptr->value == current);
                bool goodToTest = false;
                if (isLeaf) {
                    dq_ptr->tmp_count = dq_ptr->count;// As this is the leaf, it needs to hold the tmp_count for the visit its own support count.
//                    dq_ptr->current_leaf = true;// Setting the current node as leaf, from which start the visit (from the bottom!)
                    goodToTest = true; // Yes, I can proceed with the visit
                } else {
                    size_t totalChild = 0; // Counting the number of allowed children to be visited within the view as in (A)

                    visitedChild.clear(); // Clearing the set of the previous children
                    for (const auto& [k,v] : dq_ptr->children) {
                        if (nView.contains(v)) {
                            totalChild++; // (A)
                            if (visitable.contains(v)) {
                                visitedChild.emplace_back(v); // Yes, this child has been visited in a previous iteration
                            }
                        }
                    }
                    if (totalChild == visitedChild.size()) { // I can do the counting only if I have already visited all of the childs that are there in the
                        for (const auto& v : visitedChild) { // After visiting all the childs that I could, *then*, I set my tmp_count to the sum of my child's tmp_count
                            goodToTest = true;
                            dq_ptr->tmp_count += v->tmp_count; // Then, setting up the count to the sum of the supports from my fully visited children
//                            dq_ptr->current_leaf = dq_ptr->current_leaf && ((v->current_leaf) && (v->tmp_count < minsupport)); // Setting myself as a leaf
                        }
                    } else
                        visitable.erase(dq_ptr); // B: If I am not allowed to visit all the children, as they have not fully visited yet, re-set myself in the visiting queue
                    // the queue guarantees that, if I have not visited another child, then, I will eventually put back in the queue by it in (C), so this does not misses a node
                }
                if (visitable.contains(dq_ptr)) { // If the visit was successful and (B) did not happen
                    if (goodToTest && (dq_ptr->tmp_count >= minsupport)) { // testing the support only if I am either a leaf or whether I have already visited all my childs, so to not
                        toTraverseNext[dq_ptr->value] += dq_ptr->tmp_count; // Setting the current support in the table for the current item
//                        if (!it4.second)
//                            it4.first->second += dq_ptr->tmp_count; // If a previous value was already there, then just update it!
                        if (current != dq_ptr->value) new_firsttraverse[dq_ptr->value]= dq_ptr; // If this was not met before, then setting the current node as first step in the list to be visited
                        in_tree.insert(dq_ptr); // Definitively, if the support is good, this should be part of the next iteration
                    }
                    if (nView.contains(dq_ptr->parent)) // If the parent is an allowed node (still, it should be)
                        dq.push(dq_ptr->parent); // C: putting the parent as the next step to visit
                }

            }
        }
    }

    for (act_t next = 0; next < toTraverseNext.size(); next++) { // Using the updated support table to traverse the remaining nodes
        size_t as = toTraverseNext.at(next);
        if ((next != current) && ( !prefix.contains(next)) && (as >= minsupport)) // Visiting next only other strings that were not part of the prefix
            fpgrowth_expand(tree, next, as, prefix, in_tree, new_firsttraverse, results, minsupport, min_length);
    }
}