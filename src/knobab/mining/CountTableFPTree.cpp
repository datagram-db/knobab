
//
// Created by giacomo on 15/09/22.
//

#include "knobab/mining/CountTableFPTree.h"
#include "yaucl/structures/setoids/basics.h"

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
                     size_t max_length) {


    prefix.insert(current); // Adding the current node as part of the prefixù
    if (prefix.size() <= max_length) {
        results.emplace_back(curr_support, prefix); // setting this as another result
    }
    if (prefix.size() >= max_length) return;

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
            fpgrowth_expand(tree, next, as, prefix, in_tree, new_firsttraverse, results, minsupport, max_length);
    }
}
