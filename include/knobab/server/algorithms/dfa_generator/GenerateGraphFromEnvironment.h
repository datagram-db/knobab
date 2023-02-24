//
// Created by giacomo on 23/01/23.
//

#ifndef KNOBAB_SERVER_GENERATEGRAPHFROMENVIRONMENT_H
#define KNOBAB_SERVER_GENERATEGRAPHFROMENVIRONMENT_H

#include "knobab/server/query_manager/Environment.h"
#include "GenerateGraphFromAtomisedClause.h"
#include <random>
#include <yaucl/data/SecondaryMemoryMalloc.h>


FlexibleFA<size_t, std::string> generateGraphFromEnvironment(Environment& env,
                                  const std::filesystem::path& cache_path
                                  );
#include <bits/stdc++.h>

template <typename T>
struct QNode {
    T data;
    QNode<T>* next;

    void init(const T& d) {
        data = d;
        next = nullptr;
        this = new(this)QNode;
    }
    QNode(const T& d) {
        data = d;
        next = NULL;
    }
};
template <typename T>
struct Queue {
    QNode<T> *front, *rear;
    Queue() { front = rear = NULL; }

    void enQueue(T x){
        // Create a new LL node
        enQueue(x, new QNode(x));
    }

    void enQueue(T x, QNode<T>* temp) {
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
    QNode<T>* pop(bool doDelete = true) {
        // If queue is empty, return NULL.
        if (front == NULL)
            return nullptr;

        // Store previous front and
        // move front one node ahead
        QNode<T>* temp = front;
        front = front->next;

        // If front becomes NULL, then
        // change rear also as NULL
        if (front == NULL)
            rear = NULL;

        if (doDelete) {
            delete (temp);
            return nullptr;
        } else
            return temp;
    }


};

template <typename EdgeLabel>
struct serialiser_deserialiser {
    std::function<void(const EdgeLabel&, size_t* memo, char** ptr)> serialise;
    std::function<EdgeLabel(size_t,char*)> deserialise;
    std::function<size_t(const EdgeLabel&)> size_estimation;
    char* memo;
    size_t curr_size;
    char* memo_fi;
    size_t curr_size_fi;

    serialiser_deserialiser(const std::function<void(const EdgeLabel&, size_t* memo, char** ptr)>& s,
                            const std::function<EdgeLabel(size_t,char*)>& d,
                            const std::function<size_t(const EdgeLabel&)>& se) {
        serialise = s;
        deserialise = d;
        size_estimation = se;
        curr_size = curr_size_fi = 0;
        memo = memo_fi = nullptr;
    }

    serialiser_deserialiser() {
        serialise = [](const auto& x) {return;};
        deserialise = [](size_t x, char* y) { return EdgeLabel(); };
        size_estimation = [](const auto& x){return 0;};
        curr_size = curr_size_fi = 0;
        memo = memo_fi = nullptr;
    }

    std::pair<size_t, char*> do_serialise(const std::vector<EdgeLabel>& v) {
        size_t N = v.size();
        size_t precalc_tot = curr_size_fi = sizeof(size_t)*(N+1);
        for (size_t i = 0; i<N; i++) {
            precalc_tot += size_estimation(v.at(i));
        }
        memo_fi = (char*)realloc((char*)memo_fi, precalc_tot);
        size_t* header = (size_t*)memo_fi;
        header[0] = v.size();
        for (size_t i = 0; i<N; i++) {
            header[i+1] = curr_size_fi;
            curr_size_fi += size_estimation(v.at(i));
        }

        char* forcopy = ((char*)memo_fi)+sizeof(size_t)*(N+1);
        for (size_t i = 0; i<N; i++) {
            serialise(v.at(i), &curr_size, &memo);
            memcpy(forcopy, memo, curr_size);
            forcopy+=curr_size;
        }
        return std::pair<size_t, char*>{curr_size_fi, memo_fi};
    }

    void do_deserialise(size_t len, char* data, std::vector<EdgeLabel>& result) {
        size_t* header = (size_t*)data;
        result.clear();
        size_t N = header[0];
        result.reserve(N);
        for (size_t i = 0; i<N; i++) {
            char* begin = data+header[i+1];
            if (i!= (N-1)) {
                size_t curr_len = header[i+2]-header[i+1];
                result.emplace_back(deserialise(curr_len, begin));
            } else {
                size_t curr_len = len-header[i+1];
                result.emplace_back(deserialise(curr_len, begin));
            }
        }
    }
};

template <typename NodeLabel, typename EdgeLabel>
static inline void generative(const FlexibleFA<NodeLabel, EdgeLabel>& graph,
                              std::vector<std::vector<std::string>>& tab,
                              size_t min_len = 3,
                              size_t max_len = 3,
                              size_t max_per_limit = 1,
                              bool doSample = false,
                              double sampleRate = 0.5) {
    if (max_per_limit == 0) return;
    auto ref = graph.getNodeIds();
    if (ref.empty()) return;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);
    std::vector<size_t> Arg(*std::max_element(ref.begin(), ref.end())+1, 0);
    Queue<std::pair<size_t, std::vector<EdgeLabel>>> Q;
    std::pair<size_t, std::vector<EdgeLabel>> cp;
    for (size_t i : graph.init())
        Q.enQueue(std::make_pair(i, std::vector<EdgeLabel>{}));
    while (!Q.empty()) {
        std::swap(cp, Q.front->data);
        Q.pop();
        if ((cp.second.size() <= max_len)) {
            if (graph.final_nodes.contains(cp.first) && (cp.second.size() > 0) && (Arg[cp.first] <= max_per_limit)) {
                Arg[cp.first]++;
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
}

#endif //KNOBAB_SERVER_GENERATEGRAPHFROMENVIRONMENT_H
