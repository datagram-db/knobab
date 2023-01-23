//
// Created by giacomo on 23/01/23.
//

#ifndef KNOBAB_SERVER_GENERATEGRAPHFROMENVIRONMENT_H
#define KNOBAB_SERVER_GENERATEGRAPHFROMENVIRONMENT_H

#include "knobab/server/query_manager/Environment.h"
#include "GenerateGraphFromAtomisedClause.h"
#include <random>

FlexibleFA<size_t, std::string> generateGraphFromEnvironment(Environment& env,
                                  const std::filesystem::path& cache_path
                                  );
#include <bits/stdc++.h>

template <typename T>
struct QNode {
    T data;
    QNode<T>* next;
    QNode(const T& d)
    {
        data = d;
        next = NULL;
    }
};
template <typename T>
struct Queue {
    QNode<T> *front, *rear;
    Queue() { front = rear = NULL; }

    void enQueue(T x)
    {

        // Create a new LL node
        QNode<T>* temp = new QNode(x);

        // If queue is empty, then
        // new node is front and rear both
        if (rear == NULL) {
            front = rear = temp;
            return;
        }

        // Add the new node at
        // the end of queue and change rear
        rear->next = temp;
        rear = temp;
    }

    bool empty() const { return front == NULL; }
    const T& top() const { return front->data; }

    // Function to remove
    // a key from given queue q
    void pop()
    {
        // If queue is empty, return NULL.
        if (front == NULL)
            return;

        // Store previous front and
        // move front one node ahead
        QNode<T>* temp = front;
        front = front->next;

        // If front becomes NULL, then
        // change rear also as NULL
        if (front == NULL)
            rear = NULL;

        delete (temp);
    }
};


template <typename NodeLabel, typename EdgeLabel>
static inline void generative(const FlexibleFA<NodeLabel, EdgeLabel>& graph,
                              std::vector<std::vector<std::string>>& tab,
                              size_t min_len = 3,
                              size_t max_len = 3,
                              size_t max_per_limit = std::numeric_limits<size_t>::max(),
                              bool doSample = false,
                              double sampleRate = 0.5) {
    if (max_per_limit == 0) return;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);
//        std::vector<size_t> Arg(getNodeIds().size(), 0);
    Queue<std::pair<size_t, std::vector<EdgeLabel>>> Q;
    std::pair<size_t, std::vector<EdgeLabel>> cp;
//        std::set<std::vector<EdgeLabel>> EL;
    for (size_t i : graph.init())
        Q.enQueue(std::make_pair(i, std::vector<EdgeLabel>{}));
    while (!Q.empty()) {
        std::swap(cp, Q.front->data);
        Q.pop();
        if ((cp.second.size() <= max_len)) {
            if (graph.final_nodes.contains(cp.first) && (cp.second.size() > 0) /*&& (Arg[cp.first] < max_per_limit)*/) {
//                    Arg[cp.first]++;
//                    EL.emplace(cp.second);
                if (min_len <= cp.second.size()) {
                    auto& ref = tab.emplace_back();
                    for (size_t j = 0, N = cp.second.size()-1; j<=N; j++) {
                        ref.emplace_back(cp.second[j]);
                    }
                    max_per_limit--;
                    if (max_per_limit == 0) return;
                }
            }
            for (auto cp2 : graph.outgoingEdges(cp.first)) {
                double number = distribution(generator);
                if ((!doSample) || number <= sampleRate) {
                    std::vector<EdgeLabel> el{cp.second};
                    el.emplace_back(cp2.first);
                    Q.enQueue(std::make_pair(cp2.second, el));
                }
            }
        }
    }
//        return EL;
}

#endif //KNOBAB_SERVER_GENERATEGRAPHFROMENVIRONMENT_H
