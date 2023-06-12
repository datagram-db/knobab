//
// Created by giacomo on 11/02/23.
//


#include "knobab/server/query_manager/Environment.h"
#include "yaucl/graphs/flloat/ParseFFLOATDot.h"
#include "knobab/server/query_manager/ServerQueryManager.h"
#include "yaucl/graphs/algorithms/all_cycles.h"
#include "yaucl/graphs/algorithms/AllDirectedPaths.h"
#include "yaucl/graphs/algorithms/KosarajuSCC.h"
#include "yaucl/graphs/algorithms/JohnsonAllCircuits.h"
#include "yaucl/bpm/SimpleXESSerializer.h"
#include <yaucl/graphs/flloat/ParseFFLOATDot.h>
#include <knobab/server/algorithms/dfa_generator/GenerateGraphFromAtomisedClause.h>
#include <knobab/server/algorithms/atomization/AtomizingPipeline.h>
#include <yaucl/graphs/algorithms/minimizeDFA.h>

/***
 * Dynamic change-making algorithm for reconstructing the minimum amount of
 * coins for providing a given sum n.
 *
 * @param lens  Vector of the value associated to the coins
 * @param n     Total sum that we want to reach by using the coins
 * @param res   Map where the association coin->number is stored. If empty, it means
 *              that no solution exists
 * @result      This returns true if there exists at least one result associated to the vector
 */
 // Da usare per determinare il numero di cicli!
bool _change_making(const std::vector<size_t>& lens, size_t n, std::vector<std::pair<size_t,size_t>>& res) {
    size_t L = lens.size();
    std::vector< std::vector<int> > m(L+1, std::vector<int>(n+1,0));
    for (size_t i = 0; i<=n; i++)
        m[0][i] = n+1;
    for (size_t c = 1; c<L+1; c++)
        for (size_t r = 1; r<n+1; r++)
            if (lens.at(c-1) == r)
                m[c][r] = 1;
            else if (lens.at(c-1)>r)
                m[c][r] = m.at(c-1).at(r);
            else
                m[c][r] = std::min(m.at(c-1).at(r), 1+m.at(c).at(r-lens.at(c-1)));
    size_t i = L;
    size_t j = n;
    res.clear();
    bool hasSol = false;
    while (j>0 && i>0) {
        if (j<lens.at(i-1)) i--; else
        if (m.at(i).at(j-lens.at(i-1)) == m.at(i).at(j)-1) {
            size_t K = lens.at(i-1);
            if (res.empty())
                res.emplace_back(K, 1);
            else if (res.back().first == K)
                res.back().second = res.back().second + 1;
            else
                res.emplace_back(K, 1);
            j -= lens.at(i-1);
            hasSol = true;
        } else
            i--;
    }
    return hasSol;
}

struct trie {
    struct node {
        size_t id, offset, parent, len;
        size_t fromEdge = -1;
//        std::vector<std::string> admissible_edges;
        std::unordered_map<size_t, size_t> children;
        node(size_t x, size_t offset): id{x}, offset{offset}, children{}, parent{x}, len{0} {};
        node(size_t x, size_t offset, size_t parent, size_t incr): id{x}, offset{offset}, children{}, parent{parent}, len{incr} {};
        node(const node&) = default;
        node(node&&) = default;
        node& operator=(const node&) = default;
        node& operator=(node&&) = default;
    };
    std::vector<node> nodes;

    trie() = default;
    trie(const trie&) = default;
    trie(trie&&) = default;
    trie& operator=(const trie&) = default;
    trie& operator=(trie&&) = default;

    size_t init(size_t root_id) {
        nodes.emplace_back(root_id, 0);
        return 0;
    }

    size_t extend_with(size_t current, size_t next) {
        auto& ref = nodes[current];
        size_t val = ref.len+1;
        auto it = ref.children.find(next);
        if (it != ref.children.end()) return it->second;
        size_t next_id = nodes.size();
        ref.children[next] = next_id;
        nodes.emplace_back(next, next_id, current, val);
        return next_id;
    }

    node* resolve_node_ptr(size_t i) { return &nodes[i]; }

    inline bool reconstructor(size_t x, const std::function<bool(const std::vector<size_t>&, size_t)>& observer) {
        if (start) {
            auto ptr = resolve_node_ptr(x);
            DEBUG_ASSERT(ptr->fromEdge != -1);
            while (ptr->offset != 0) {
                DEBUG_ASSERT(ptr->fromEdge != -1);
                node_values.emplace_back(ptr->fromEdge);
                parent_offsets.emplace_back(ptr->offset);
                current_size++;
                ptr = resolve_node_ptr(ptr->parent);
            }
            std::reverse(node_values.begin(), node_values.end());
            std::reverse(parent_offsets.begin(), parent_offsets.end());
            DEBUG_ASSERT(resolve_node_ptr(x)->fromEdge == node_values.at(current_size-1));
            if (!observer(node_values, current_size)) return false;
            start = false;
        } else {
            auto ptr = resolve_node_ptr(x);
            size_t size_before_insertion = node_values.size();
            bool wasWSet = false;
            if (ptr->len > size_before_insertion+1) {
                current_size = size_before_insertion;
                while (ptr->len != size_before_insertion+1) {
                    DEBUG_ASSERT(ptr->fromEdge != -1);
                    node_values.emplace_back(ptr->fromEdge);
                    parent_offsets.emplace_back(ptr->offset);
                    current_size++;
                    ptr = resolve_node_ptr(ptr->parent);
                }
                std::reverse(node_values.begin()+size_before_insertion, node_values.end());
                std::reverse(parent_offsets.begin()+size_before_insertion, parent_offsets.end());
            } else {
                wasWSet = true;
                current_size = ptr->len/*-1*/;
            }
            bool found = false;
            for (ssize_t s = 1; s<=size_before_insertion; s++) {
                auto& ref = parent_offsets[size_before_insertion-s];
                if (ref == ptr->offset) {
                    DEBUG_ASSERT(resolve_node_ptr(x)->fromEdge == node_values.at(current_size-1));
                    if (!observer(node_values, current_size)) return false;
                    found = true;
                    break;
                } else {
                    ref = ptr->offset; // assignment, value update
                    DEBUG_ASSERT(ptr->fromEdge != -1);
                    node_values[size_before_insertion-s] = ptr->fromEdge;
                    ptr = resolve_node_ptr(ptr->parent);
                }
            }
            if (!found) {
                DEBUG_ASSERT(resolve_node_ptr(x)->fromEdge == node_values.at(current_size-1));
                if (!observer(node_values, current_size)) return false;
            }
        }
        return true;
    }

    inline void  single_reconstructor_init() {
        node_values.clear();
        parent_offsets.clear();
        current_size = 0;
        start = true;
    }

    /**
     * Efficiencly reconstructs the strings from the trie, without re-allocating the nodes that were already
     * visited, if possible
     * @tparam Iterator     Iterator type, containing the nodes from which start the ascent to the root
     * @param begin         Beginning of such nodes
     * @param end           End of such nodes
     * @param observer      Observer, that is going to be notified by the node-id as retrieved from the trie
     */
    template <typename Iterator>
    void reconstructor(Iterator begin,
                       Iterator end,
                       const std::function<bool(const std::vector<size_t>&, size_t)>& observer) {
        single_reconstructor_init();
        while (begin != end) {
            size_t x = *begin;
            if (!reconstructor(x, observer)) return;
            begin++;
        }
    }



private:
    std::vector<size_t> node_values;
    std::vector<size_t> parent_offsets;
    size_t current_size = 0;
    bool start = true;
};

inline
semantic_atom_set evaluate_easy_prop_to_atoms2(const easy_prop &prop,
                                              const std::unordered_map<std::string, semantic_atom_set> &bogus_act_to_set,
                                              const std::unordered_set<std::string>& sigma) {
    switch (prop.casusu) {
        case easy_prop::E_P_AND:
            DEBUG_ASSERT(prop.args.size() == 2);
            DEBUG_ASSERT(!prop.isAtomNegated);
            return unordered_intersection(evaluate_easy_prop_to_atoms( prop.args.at(0)/*, bogus_act_to_atom*/, bogus_act_to_set, sigma),
                                          evaluate_easy_prop_to_atoms( prop.args.at(1)/*, bogus_act_to_atom*/, bogus_act_to_set, sigma) );
        case easy_prop::E_P_OR: {
            DEBUG_ASSERT(prop.args.size() == 2);
            DEBUG_ASSERT(!prop.isAtomNegated);
            semantic_atom_set S = evaluate_easy_prop_to_atoms( prop.args.at(0)/*, bogus_act_to_atom*/, bogus_act_to_set, sigma);
            auto tmp = evaluate_easy_prop_to_atoms( prop.args.at(1)/*, bogus_act_to_atom*/, bogus_act_to_set, sigma);
            S.insert(tmp.begin(), tmp.end());
            return S;
        }
        case easy_prop::E_P_ATOM:
            DEBUG_ASSERT(prop.args.empty());
            DEBUG_ASSERT(bogus_act_to_set.contains(prop.single_atom_if_any));
            if (prop.isAtomNegated) {
                return unordered_difference(sigma, bogus_act_to_set.at(prop.single_atom_if_any));
            } else {
                return bogus_act_to_set.at(prop.single_atom_if_any);
            }
        case easy_prop::E_P_TRUE:
            return sigma;
        default: //case easy_prop::E_P_FALSE:
            return {};
    }
}


struct graph_holder {
    std::unordered_map<std::string, NodeLabelBijectionFA<std::string, easy_prop>> graph_cache;

    graph_holder(const std::filesystem::path& path_cache) {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(path_cache)) {
            ParseFFLOATDot graph_loader;
            std::ifstream strm(dirEntry.path().c_str());
            graph_cache.emplace(dirEntry.path().filename(), graph_loader.parse(strm));
        }
    }

    inline void generateGraphFromPattern(const DeclareDataAware& atomised_clause,
                                  const std::unordered_set<std::string>& sigma,
                                  FlexibleFA<size_t, std::string>& result) {
        ParseFFLOATDot graph_loader;
        std::string full_name = atomised_clause.casusu;
        if (atomised_clause.right_act.empty())
            full_name = full_name+"_"+std::to_string(atomised_clause.n);
        auto& patternGraphToInstantiate = graph_cache.at(full_name);
//        patternGraphToInstantiate.dot(std::cout); std::cout;
        std::unordered_map<std::string, semantic_atom_set> bogus_act_to_set;
        bogus_act_to_set["a"] = atomised_clause.left_decomposed_atoms;
        if (!atomised_clause.right_act.empty())
            bogus_act_to_set["b"] = atomised_clause.right_decomposed_atoms;
        std::unordered_map<size_t, size_t> idConv;
        for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId<N; nodeId++) {
            size_t src = result.addNewNodeWithLabel(nodeId);
            idConv[nodeId] = src;
        }
        for (size_t finalNodes : patternGraphToInstantiate.fini()) {
            result.addToFinalNodesFromId(idConv.at(finalNodes));
        }
        for (size_t initialNodes : patternGraphToInstantiate.init()) {
            result.addToInitialNodesFromId(idConv.at(initialNodes));
        }
        for (size_t nodeId = 0, N = patternGraphToInstantiate.maximumNodeId(); nodeId<N; nodeId++) {
            size_t src = idConv.at(nodeId);
            for (const auto&edge : patternGraphToInstantiate.outgoingEdges(nodeId)) {
                size_t dst = idConv.at(edge.second);
                for (const std::string& act : evaluate_easy_prop_to_atoms(edge.first, bogus_act_to_set, sigma)) {
                    result.addNewEdgeFromId(src, dst, act);
                }
            }
        }
//        result.makeDFAAsInTheory(sigma);
    }

    void generateFromEnvironment(FlexibleFA<size_t, std::string>& result,
                                 Environment* env,
                                 const std::vector<std::string>& other_atoms) {
        std::unordered_set<std::string> sigma = env->getSigmaAll();
        sigma.insert(other_atoms.begin(), other_atoms.end());
        size_t result_count = 0;
        FlexibleFA<std::string, size_t> resultG;
        std::vector<FlexibleFA<std::string, size_t>> resultV(env->grounding.singleElementOfConjunction.size()-1);
        for (const auto& ref : env->grounding.singleElementOfConjunction) {
            for (const auto& clause : ref.elementsInDisjunction) {
                FlexibleFA<size_t, std::string> curr;
                if (env->grounding.singleElementOfConjunction.size() == 1) {
                    generateGraphFromPattern(clause,  sigma, result);
                    return;
                }
                generateGraphFromPattern(clause,  sigma, curr);
//                curr.dot(std::cout); std::cout << std::endl;
                if (result_count == 0) {
                    resultG = curr.shiftLabelsToNodes();
                    result_count++;
//                    resultG.removeStatesNotLeadingToAcceptance();
                } else {
                    resultV[result_count-1] = curr.shiftLabelsToNodes();
//                    result[result_count-1].removeStatesNotLeadingToAcceptance();
                    result_count++;
                }
            }
        }
//    result[0].dot(std::cout); std::cout << std::endl;
        for (size_t i = 0; i<resultV.size(); i++) {
//        std::cout << "join" << i << std::endl;
//        resultV[i].dot(std::cout); std::cout << std::endl;
//            resultG.dot(std::cout); std::cout << std::endl;
            resultG = FlexibleFA<std::string,size_t>::crossProductWithNodeLabels(resultG, resultV[i], false);
        }
        result = resultG.shiftLabelsToEdges();
//        return result[0];
        {
//            auto g = result[0].shiftLabelsToEdges();
//            result.clear();
//            g.makeDFAAsInTheory(sigma);
//        g.removeStatesNotLeadingToAcceptance();
//        g.pruneUnreachableNodes();
//        g.dot(std::cout); std::cout <<"HIS"<< std::endl;

//            return g;
//        g.dot(std::cout); std::cout << std::endl;
//        std::vector<std::vector<std::string>> tab;
//        generative(g, tab, min_len, max_len, max_per_limit, doSample, sampleRate);
//        return tab;
        }
    }
};

struct status {
    Environment env;
    ServerQueryManager sqm;
    graph_holder holder;
    size_t minL, maxL, nTraces;
//    size_t max_visited;
    double sampleRatio;
//    bool  useJohnson;
//    size_t useJohnsonLimit;
    bool isTab; bool doSkip;
    status(const std::filesystem::path& path_cache) : holder{path_cache} {
        sqm.tmpEnv = &env;
    }
};

using bfs_path = size_t;
struct cycle_injection {
    size_t subpath_id_replace;
    size_t n_times_replace;
    size_t cycle_id;
    size_t cycle_id_offset_start;

    cycle_injection(size_t cycleId, size_t cycleIdOffsetStart, size_t subpathIdReplace, size_t nTimesReplace)
            : cycle_id(cycleId), cycle_id_offset_start(cycleIdOffsetStart), subpath_id_replace(subpathIdReplace),
              n_times_replace(nTimesReplace) {}
    DEFAULT_CONSTRUCTORS(cycle_injection)
};

using extension_path = std::vector<size_t>;
struct extending_path_after_bfs {
    size_t                       extending_node;
    extension_path               depth_path;
    std::map<size_t, std::vector<cycle_injection>>   loop_extension;

    DEFAULT_CONSTRUCTORS(extending_path_after_bfs)
    void clear(size_t new_id) {
        extending_node = new_id;
        depth_path.clear();
        loop_extension.clear();
    }
};

template<typename Range1, typename Range2, typename OutputIterator>
void cartesian_product(Range1 const &r1, Range2 const &r2, OutputIterator& out) {
    using std::begin; using std::end;
    for (auto i = std::begin(r1);i != std::end(r1); ++i) {
        for (auto j = std::begin(r2); j != std::end(r2); ++j) {
            auto& ref = out.emplace_back(*i);
            ref.emplace_back(*j);
        }
    }
}

void for_each_test(status& env,
                   const std::string& filename,
                   const std::string& sigma,
                   const std::string& csv) {
    std::string line;
    bool hasHeader = false;
    if ( access( csv.c_str(), F_OK ) != -1 )
        hasHeader = true;
    std::ofstream csvFile{csv, std::ios_base::app};
    if (!hasHeader) {
        csvFile << "filename,GraphGenerationTime,GraphVisitTime,SerialisationTime,Total,AtomSize,minL,maxL,S,Vsize,Esize" << std::endl;
    }
    size_t minL = env.minL;
    size_t maxL = env.maxL;
    size_t logS = env.nTraces;
    std::ifstream infile{sigma};
    std::fstream model{filename};
    env.sqm.tmpEnv->clearModel();
//    env.env.clearModel();
    env.sqm.loadModel(model);
    bool doPreliminaryFill = true;
    bool ignoreActForAttributes = false;
    bool creamOffSingleValues = true;
    GroundingStrategyConf::pruning_strategy grounding_strategy = GroundingStrategyConf::NO_EXPANSION;
    env.sqm.tmpEnv->set_grounding_parameters(doPreliminaryFill, ignoreActForAttributes, creamOffSingleValues,
                                     grounding_strategy);
    env.sqm.tmpEnv->doGrounding();
    std::string atomj{"p"};
    AtomizationStrategy atom_strategy = AtomizationStrategy::AtomizeEverythingIfAnyDataPredicate;
    size_t n = 3;
    env.sqm.tmpEnv->set_atomization_parameters(atomj, n, atom_strategy);
    env.sqm.tmpEnv->init_atomize_tables();
    env.sqm.tmpEnv->first_atomize_model();
    std::vector<std::string> such_sigma;
    while (std::getline(infile, line)) {
        env.sqm.tmpEnv->ap.atom_decomposition(line, false, such_sigma);
    }
    env.sqm.tmpEnv->ap.finalise();
//    env.sqm.tmpEnv->print_grounding_tables(std::cout);
    remove_duplicates(such_sigma);
    size_t atom_size = such_sigma.size();
    std::cout << "Atom size: " << atom_size << "; " << such_sigma << std::endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    FlexibleFA<size_t, std::string> graph;
    env.holder.generateFromEnvironment(graph, env.sqm.tmpEnv, such_sigma);
//    graph.dot(std::cout); std::cout << std::endl;
    size_t v_size = graph.vSize();
    size_t e_size = graph.eSize();
    // preparing the one-sink node for running the all single paths when this will be required
//    size_t sink = graph.addNewNodeWithLabel(-2);
//    for (size_t f : graph.final_nodes)
//        graph.addNewEdgeFromId(f, sink, ".");

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> graph_gen_time_c = t2 - t1;
    double graph_gen_time = graph_gen_time_c.count();
    std::cout << "Graph Generation Time (ms): " << graph_gen_time << std::endl;
//    {
//        // JUST FOR DEBUGGING PURPOSES
//        std::string dotFile = filename+".g.dot";
//        std::ofstream file{dotFile};
//        graph.dot(file, false, true); file << std::endl;
//        file.close();
//        std::cout << filename+".g.dot" << std::endl;
//    }
    AllCycles<size_t, std::string> ac{graph};
    std::string dest_name = filename+".out";
    bool isSampleRatioSet = env.sampleRatio <= 0.0;
//    std::vector<std::vector<std::string>> atom_lists;
    std::default_random_engine generator;

//    {
//        // JUST FOR DEBUGGING PURPOSES
//        std::string dotFile = filename+".g.dot";
//        std::ofstream file{dotFile};
//        graph.shiftLabelsToEdges().dot(file); file << std::endl;
//        file.close();
//    }
//    graph.removed_nodes.add(sink);
//    std::cout << filename<<".g.dot" << std::endl;

//    TODO: AllCycles running
//          ----------------
//#if 1
//    // Please note that Johnson's algorithm for getting all circuits in a graph is too damn slow.
//    // It provides complete responses, but this enumeration takes too much time
//    JohnsonAllCircuits<size_t, std::string> ac{graph};
//    ac.run(2); // Still, the computational complexity can be reduced by pruning the cyclces to the minimal interesting ones.
//    std::sort(ac.circuits.begin(), ac.circuits.end(), [](const auto&x , const auto& y) {
//        return x.size() < y.size();
//    });
//    for (const auto& ref : ac.circuits)
//    {
//        for (size_t x : ref)
//            std::cout << graph.getNodeLabel(x)<< ",";
//        std::cout << std::endl;
//    }
//#else
//    AllCycles<size_t, std::string> ac{graph};
//    for (std::vector<size_t>& v : ac(false)) {
//        for (size_t x : v) std::cout << graph.getNodeLabel(x)<<",";
//        std::cout << std::endl;
//    }
//#endif


//    TODO: AllSimplePaths running
//          ----------------------


//exit(1);
    size_t len;
    size_t count;
    size_t count_sol = 0;
//    roaring::Roaring64Map layerAddrSet;
//    roaring::Roaring64Map layerIdSet;
    t1 = std::chrono::high_resolution_clock::now();

    // Having these vectors should not be required, as all the DFAs should have only one initial node by definition
    // (Things will differ if we change the representation of such graphs).
//    std::vector<trie> linear_trie_storage;
    trie trie_storage;
//    std::vector<std::vector<size_t>> trie_finals;
    std::vector<size_t> finals;
//    std::vector<roaring::Roaring64Map> layerIdSet_storage, layerAddrSet_storage;
    roaring::Roaring64Map layerAddrSetL, layerIdSetL;

    std::unordered_map<size_t, std::vector<size_t>> final_node_to_trie_offset;
    std::unordered_map<size_t, std::vector<extending_path_after_bfs>> extensions_to_final_node;
//    bool enforce = true;
    if (env.nTraces > 0) {
        bool doStopDeepeningPhase = false;
        auto ref = graph.getNodeIds();
        if (!ref.empty()) {
            size_t max_per_limit = env.nTraces;
            std::uniform_real_distribution<double> distribution(0.0, 1.0);
            auto &s = graph.init();
            DEBUG_ASSERT(s.cardinality() == 1);
            for (size_t i: s) {
                count = 0;
                len = 0;
//                auto& layerAddrSetL = layerAddrSet_storage.emplace_back();
//                auto& layerIdSetL = layerIdSet_storage.emplace_back();
                std::queue<size_t> Q;
                size_t ptr = trie_storage.init(i);
                Q.emplace(ptr);
//                std::vector<size_t>& finals = trie_finals.emplace_back();
                while (!Q.empty()) {
                    size_t addr = Q.front();
                    Q.pop();
                    trie::node *ptr = trie_storage.resolve_node_ptr(addr);
                    if (ptr->len > len) {
//                        std::cout << ptr->len << std::endl;
                        len = ptr->len;
                        layerAddrSetL.clear();
                        layerIdSetL.clear();
                        count = 1;
                    } else {
                        layerAddrSetL.add(addr);
                        layerIdSetL.add(ptr->id);
                        count++;
                    }
                    if ((ptr->len <= env.maxL)) {
                        if (/*enforce &&*/ (layerAddrSetL.cardinality() == env.nTraces)) {
                            doStopDeepeningPhase = true;
                            break;
                        }
                        if (graph.final_nodes.contains(ptr->id)) {
                            if (env.minL <= ptr->len) {
                                count_sol++;
                                finals.emplace_back(ptr->offset);
                                max_per_limit--;
                                if (max_per_limit == 0) break;
                            }
                        }
                        size_t off = ptr->offset;
                        size_t kid = ptr->id;
                        std::shuffle(graph.g.nodes[kid].begin(), graph.g.nodes[kid].end(), generator);
                        size_t N = std::ceil(((double) graph.g.nodes.at(kid).size()) * env.sampleRatio);
                        for (size_t edge_off = 0; edge_off < N; edge_off++) {
                            size_t edge_id = graph.g.nodes.at(kid).at(edge_off);
                            DEBUG_ASSERT(edge_id < graph.g.E_size);
                            size_t dst_id = graph.g.edge_ids.at(edge_id).second;
                            double number = distribution(generator);
                            if (((!isSampleRatioSet) ||
                                 number <= env.sampleRatio) /*&& (Arg[dst_id] < graph.g.nodes.at(kid).size())*/) {
                                size_t coeff = trie_storage.extend_with(off, dst_id);
                                trie_storage.resolve_node_ptr(coeff)->fromEdge = edge_id;
                                Q.push(trie_storage.extend_with(off, dst_id));
                            }
                        }
                    }
                }


                if (doStopDeepeningPhase) {
                    break;
                }

//                for (size_t x : finals) {
//                    auto& ref = atom_lists.emplace_back();
//                    auto ptr = trie_storage.resolve_node_ptr(x);
//                    while (ptr->offset != 0) {
//                        ref.emplace_back(graph.getEdgeLabel(ptr->fromEdge));
//                        ptr = trie_storage.resolve_node_ptr(ptr->parent);
//                    }
//                    std::reverse(ref.begin(), ref.end());
//                }
            }
        }


//        for (size_t trie_id = 0, N = linear_trie_storage.size(); trie_id<N; trie_id++) {
//            const trie& trie_storage = linear_trie_storage.at(trie_id);
        bool first = true;
        size_t ds;
        for (auto b = trie_storage.nodes.rbegin(), e = trie_storage.nodes.rend(); b != e; b++) {
            if (first) {
                ds = b->len;
                first = false;
            }
            if (ds == b->len) {
                DEBUG_ASSERT(graph.g.edge_ids.at(b->fromEdge).second == b->id);
                final_node_to_trie_offset[b->id].emplace_back(b->offset);
            }
#ifdef DEBUG
                else
                    if (ds < b->len) DEBUG_ASSERT(false);
#endif
            else break;
        }
//        }


        if (doStopDeepeningPhase) {
//            roaring::Roaring64Map layerIdSet;
//            for (const auto &ref: layerIdSet_storage) layerIdSet |= ref;
#ifdef DEBUG
//            std::cout << "doStopDeepeningPhase" << std::endl;
#endif
//            std::pair<const std::vector<std::vector<size_t>> *,
//                    const std::vector<std::vector<size_t>> *> candidate_edge_cycles;
            std::vector<size_t> cycles_length;
            roaring::Roaring64Map nodes;
            std::unordered_map<size_t, roaring::Roaring64Map> node_id_to_cycle_id;
            std::unordered_map<size_t, roaring::Roaring64Map> cycle_len_to_cycle_id;


            // For supporting Johnson, we have to also extend the algorithm to provide edge information.
//            if (env.useJohnson) {
//                // Please note that Johnson's algorithm for getting all circuits in a graph is too damn slow.
//                // It provides complete responses, but this enumeration takes too much time
//                JohnsonAllCircuits<size_t, std::string> ac{graph};
//                ac.run(env.useJohnsonLimit); // Still, the computational complexity can be reduced by pruning the cyclces to the minimal interesting ones.
//                std::sort(ac.circuits.begin(), ac.circuits.end(), [](const auto&x , const auto& y) {
//                    return x.size() < y.size();
//                });
//                std::swap(candidate_cycles, ac.circuits);
//
//            } else
//            {
//            }
            ac.edge_loops();


            for (size_t cycle_id = 0, Ls = ac.cycles_of_edges.size(); cycle_id < Ls; cycle_id++) {
                auto& R = ac.cycles_of_edges.at(cycle_id);
                auto& N = ac.cycles_of_vertices.at(cycle_id);
                size_t M = N.size();
                bool decrease = (M>0) && (N.at(0) == N.at(M-1));
                if (decrease) M--;
                size_t cycle_len = R.size();
                cycles_length.emplace_back(cycle_len);
                cycle_len_to_cycle_id[cycle_len].add(cycle_id);
                for (size_t node_id = 0; node_id<M; node_id++) {
                    auto& ref_node = N.at(node_id);
                    nodes.add(ref_node);
                    node_id_to_cycle_id[ref_node].add(cycle_id);
                }
            }
            std::vector<size_t> ans;
            std::vector<std::pair<size_t,size_t>> coin_problem_result;

//#ifdef DEBUG
//            std::cout << "Node to cycles::" << std::endl;
//            for (const auto& [k,v] : node_id_to_cycle_id) {
//                std::cout << k << " --> " << v.toString() << std::endl;
//            }
//            std::cout << "Cycle lens to cycles::" << std::endl;
//            for (const auto& [k,v] : cycle_len_to_cycle_id) {
//                std::cout << k << " --> " << v.toString() << std::endl;
//            }
//            std::cout << "iterating for: " << layerIdSetL.toString() << std::endl;
//#endif
            // TODO: associate the cycle to each node in the graph
            size_t prev_size = 0;
//            extending_path_after_bfs preliminary_result;
                // TODO: finalise shortest path computation
            for (size_t candidate: layerIdSetL) {
                AllDirectedPaths<size_t, std::string> ad{graph};
//#ifdef DEBUG
//                std::cout << "For node: " << candidate << std::endl;
//#endif
//                preliminary_result.clear(candidate);
//                graph.removed_nodes.remove(sink); /// REQUIRED! for making the algorithm run successfully
                auto f = [&extensions_to_final_node,&graph,&candidate,&ac,&ans,&coin_problem_result,&cycles_length,&node_id_to_cycle_id,&len,&env,&cycle_len_to_cycle_id](std::pair<const std::vector<size_t>*, size_t> path_to_max_len) {
                    const std::vector<size_t>* dfs_path;
                    size_t dfs_path_len;
                    dfs_path = path_to_max_len.first;
                    dfs_path_len = path_to_max_len.second;
                    if (dfs_path_len == 0) return false;
                    if (dfs_path_len+len>env.maxL) return false;
                    DEBUG_ASSERT(graph.g.edge_ids.at(dfs_path->at(0)).first == candidate);
                    if (dfs_path_len>=env.minL) {
                        auto& preliminary_result = extensions_to_final_node[candidate].emplace_back();
                        preliminary_result.extending_node = (candidate);
                        preliminary_result.depth_path = {dfs_path->begin(), dfs_path->begin()+dfs_path_len};
                        // If with the random path navigation we can immediately yield a path, then go for it!
//#ifdef DEBUG
//                        std::cout << "Found: +";
//                        for (size_t i = 0; i<dfs_path_len; i++) {
//                            std::cout << dfs_path->at(i) << ",";
//                        }
//                        std::cout << std::endl << std::endl;
//#endif
                        return true;
                    } else {
                        // If I still need to traverse some more, then exploit some loops for the matter
                        roaring::Roaring64Map cyclesId, cyclesLen;
                        std::unordered_map<size_t, roaring::Roaring64Map> size_inv_map;
                        for (size_t i = 0; i<dfs_path_len; i++) {
                            auto& curr_ref_i = graph.g.edge_ids.at(dfs_path->at(i));
                            auto it = node_id_to_cycle_id.find(curr_ref_i.first);
                            if (it != node_id_to_cycle_id.end())
                                cyclesId |= it->second;
                            it = node_id_to_cycle_id.find(curr_ref_i.second);
                            if (it != node_id_to_cycle_id.end())
                                cyclesId |= it->second;
                        }
                        for (size_t cycleId : cyclesId)
                            cyclesLen.add(cycles_length.at(cycleId));
                        ans.clear();
                        ans.reserve(cyclesLen.cardinality());
                        for (size_t x : cyclesLen) ans.emplace_back(x);
                        for (size_t i = env.minL; i<=env.maxL; i++) {
                            if (_change_making(ans, i - dfs_path_len - len, coin_problem_result)) {
                                // If a solution was found for the current range, then yield it!
                                auto& preliminary_result = extensions_to_final_node[candidate].emplace_back();
                                preliminary_result.extending_node = (candidate);
                                preliminary_result.depth_path = {dfs_path->begin(), dfs_path->begin()+dfs_path_len};





//#ifdef DEBUG
//                                // The simple path that shall be extended with some visit to the loops
//                                std::cout << "Found edges forming a path to extend: +[";
//                                for (size_t j = 0; j< dfs_path_len; j++) {
//                                    std::cout << dfs_path->at(j) << graph.g.edge_ids.at(dfs_path->at(j)) << ", ";
//                                    if (j<(dfs_path_len-1)) {
//                                        DEBUG_ASSERT(graph.g.edge_ids.at(dfs_path->at(j)).second == graph.g.edge_ids.at(dfs_path->at(j+1)).first);
//                                    }
//                                }
//                                std::cout << "]" << std::endl;
//#endif

                                for (const auto& ref : coin_problem_result) {
                                    // Among all the cycles with a given length, selecting the ones
                                    // admitted/reachable from the nodes of the current DFS path
                                    // For simplicity's sake, I'm just choosing one loop of a given length
                                    // If I want to add more variety, I might decide to select some of them
                                    // for each replica, thus adding more behavioural variety!

                                    for (size_t cycle_id : cycle_len_to_cycle_id.at(ref.first) & cyclesId) {
                                        auto& vertex_cycle = ac.cycles_of_vertices.at(cycle_id);
//#ifdef DEBUG
//                                        std::cout << "CYCLE #" << cycle_id << ": " << vertex_cycle << " @E: " << ac.cycles_of_edges.at(cycle_id) << std::endl;
//#endif
                                        auto cb = vertex_cycle.begin();
                                        auto ce = vertex_cycle.end();
                                        roaring::Roaring64Map SV;
                                        SV.addMany(vertex_cycle.size(), vertex_cycle.data());
                                        bool found = false;
                                        size_t subpath_offset = 0;
                                        size_t dst = 0;
                                        size_t cycle_offset;
                                        for (;
                                             subpath_offset < dfs_path_len;
                                             subpath_offset++) {
                                            size_t edge_id = dfs_path->at(subpath_offset);
                                            dst = graph.g.edge_ids.at(edge_id).first;
                                            auto it = std::find_if(cb, ce, [dst](const auto &xref) {
                                                return xref == dst;
                                            });
                                            if (it != ce) {
                                                cycle_offset = it - cb;
                                                found = true;
                                                break;
                                            }
                                        }
                                        if (!found) {
                                            dst = graph.g.edge_ids.at(dfs_path->at(dfs_path_len-1)).second;
                                            auto it = std::find_if(cb, ce, [dst](const auto &xref) {
                                                return xref == dst;
                                            });
//                                            DEBUG_ASSERT(it != ce);
                                            cycle_offset = it - cb;
                                            found = true;
                                            subpath_offset = dfs_path_len;
                                        }
//                                        DEBUG_ASSERT(found);
                                        preliminary_result.loop_extension[subpath_offset].emplace_back(cycle_id, cycle_offset, subpath_offset, ref.second);
//#ifdef DEBUG
//                                        auto& v = preliminary_result.loop_extension[subpath_offset];
////                                        std::cout <<"..." << std::endl;
//                                        for (const auto& ref_d : v) {
////                                            std::cout << ac.cycles_of_vertices.at(ref_d.cycle_id) << "@" << ref_d.cycle_id_offset_start <<  std::endl;
//                                            DEBUG_ASSERT(ref_d.subpath_id_replace == subpath_offset);
//                                            DEBUG_ASSERT(dst == *(ac.cycles_of_vertices.at(ref_d.cycle_id).begin()+ref_d.cycle_id_offset_start));
//                                            DEBUG_ASSERT(dst == graph.g.edge_ids.at(ac.cycles_of_edges.at(ref_d.cycle_id).at(ref_d.cycle_id_offset_start)).first);
//                                        }
//#endif
                                        break;
                                    }
                                }
                                return true;
                            }
                        }
                        return false;
                    }
                };

                ad.attachObserver(f);
                ad.getAllPathsAsEdges(candidate, graph.final_nodes, (env.maxL-env.minL+1)*2, env.minL-len);
            }
        }
    }

    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> graph_visit_time_c = t2 - t1;
    double graph_visit_time = graph_visit_time_c.count();
    std::cout << "Graph Visit Time (ms): " << graph_visit_time << std::endl;
    size_t all = env.nTraces;
    double ser_time = -1;

    if (!env.doSkip){
        t1 = std::chrono::high_resolution_clock::now();
        if (env.isTab) {
            std::ofstream file{dest_name};
            if (all > 0) {
                // First, getting this from the ending traces for all of the tries out there
                trie_storage.reconstructor(finals.begin(), finals.end(), [&all,&file,&graph](const std::vector<size_t>& ref, size_t len) {
                    if (all<=0) return false;
                    for (size_t i = 0; i<len; i++) {
                        file << graph.getEdgeLabel(ref.at(i));
                        if (i<(len-1))
                            file << "\t";
                    }
                    all--;
                    if (all>0)
                        file << std::endl;
                    return (all>0);
                });
            }
            trie_storage.single_reconstructor_init();
            if (all > 0) {
                // If I still need to visit something, then go for the join between BFS and DFS paths
                for (const auto& cp : extensions_to_final_node) {
//                std::cout << "for Candidate: " << cp.first << std::endl;
                    auto& bfs_paths = final_node_to_trie_offset[cp.first];
                    DEBUG_ASSERT(!bfs_paths.empty());
                    for (const size_t& final_trie_node_id : bfs_paths) {
                        trie_storage.reconstructor(final_trie_node_id, [&file,&all,&cp,&graph,&ac](const std::vector<size_t>& bfs_path, size_t max_len) {
                            std::vector<std::string> tmp_bfs;
                            for (size_t i = 0; i<max_len; i++) {
                                DEBUG_ASSERT(bfs_path.at(i) < graph.g.E_size);
                                tmp_bfs.emplace_back(graph.getEdgeLabel(bfs_path.at(i)));
                            }
                            DEBUG_ASSERT(tmp_bfs.size() == max_len);
                            for (const auto& extending_path : cp.second) { //
                                // Reconstruction of the BFS prefix path
                                for (const auto& label : tmp_bfs) {
                                    file << label << "\t";
                                }

#ifdef DEBUG
                                //                            std::cout << "BFS:" << std::endl;
//                            for (size_t i = 0; i <max_len; i++) {
//                                std::cout << graph.g.edge_ids.at(bfs_path.at(i)) << std::endl;
//                            }
//                            std::cout << "DFS:" << std::endl;
//                            for (size_t i = 0, N = extending_path.depth_path.size(); i <N; i++) {
//                                std::cout << graph.g.edge_ids.at(extending_path.depth_path.at(i)) << std::endl;
//                            }
//                            std::cout << "~~~" << std::endl;
                            auto dst_bfs = graph.g.edge_ids.at(bfs_path.at(max_len-1)).second;
                            auto src_dfs = graph.g.edge_ids.at(extending_path.depth_path.at(0)).first;
                            DEBUG_ASSERT(dst_bfs == src_dfs);
#endif

                                // Reconstruction of the DFS suffix path with loops
                                size_t MMM = extending_path.depth_path.size();
                                for (size_t i = 0, N = MMM; i<N; i++) {
                                    auto it = extending_path.loop_extension.find(i);
#ifdef DEBUG
                                    size_t prev = -1;
#endif

                                    // The information that needs to the provided before the edge
                                    if (it != extending_path.loop_extension.end()) {
                                        for (auto& loop_ref : it->second) {
                                            const auto& loop_cycle_ref = ac.cycles_of_edges.at(loop_ref.cycle_id);
#ifdef DEBUG
                                            auto expected = graph.g.edge_ids.at(extending_path.depth_path.at(i)).first;
                                        auto cycle_point = graph.g.edge_ids.at(ac.cycles_of_edges.at(loop_ref.cycle_id).at(loop_ref.cycle_id_offset_start)).first;
                                        auto cycle_b = graph.g.edge_ids.at(loop_cycle_ref.at(0)).first;
                                        auto cycle_e = graph.g.edge_ids.at(loop_cycle_ref.at(loop_cycle_ref.size()-1)).second;
                                        DEBUG_ASSERT(cycle_b == cycle_e);
                                        DEBUG_ASSERT(expected == cycle_point);
                                        if (prev != -1)
                                            DEBUG_ASSERT(expected == prev);
                                        prev = cycle_point;
#endif

                                            std::vector<std::string> tmp_loop;
                                            for (size_t j_element = 0, L = loop_cycle_ref.size(); j_element < L; j_element++) {
                                                tmp_loop.emplace_back(graph.getEdgeLabel(loop_cycle_ref.at((j_element + loop_ref.cycle_id_offset_start) % L)));
                                            }
                                            for (size_t n_times = 0; n_times < loop_ref.n_times_replace; n_times++) {
                                                for (const auto& label : tmp_loop)
                                                    file << label << "\t";
                                            }
                                        }
                                    }

#ifdef DEBUG
                                    if (prev != -1)
                                    DEBUG_ASSERT(graph.g.edge_ids.at(extending_path.depth_path.at(i)).first == prev);
#endif

                                    // Visiting the edge
                                    file << graph.getEdgeLabel(extending_path.depth_path.at(i)) << "\t";
                                }

                                auto it = extending_path.loop_extension.find(MMM);
                                if (it != extending_path.loop_extension.end()) {
                                    for (auto& loop_ref : it->second) {
                                        const auto& loop_cycle_ref = ac.cycles_of_edges.at(loop_ref.cycle_id);
#ifdef DEBUG
                                        auto expected = graph.g.edge_ids.at(extending_path.depth_path.at(MMM-1)).second;
                                    auto cycle_point = graph.g.edge_ids.at(ac.cycles_of_edges.at(loop_ref.cycle_id).at(loop_ref.cycle_id_offset_start)).first;
                                    auto cycle_b = graph.g.edge_ids.at(loop_cycle_ref.at(0)).first;
                                    auto cycle_e = graph.g.edge_ids.at(loop_cycle_ref.at(loop_cycle_ref.size()-1)).second;
                                    DEBUG_ASSERT(cycle_b == cycle_e);
                                    DEBUG_ASSERT(expected == cycle_point);
#endif

                                        std::vector<std::string> tmp_loop;
                                        for (size_t j_element = 0, L = loop_cycle_ref.size(); j_element < L; j_element++) {
                                            tmp_loop.emplace_back(graph.getEdgeLabel(loop_cycle_ref.at((j_element + loop_ref.cycle_id_offset_start) % L)));
                                        }
                                        for (size_t n_times = 0; n_times < loop_ref.n_times_replace; n_times++) {
                                            for (const auto& label : tmp_loop)
                                                file << label << "\t";
                                        }
                                    }
                                }

                                file << std::endl;
                                all--;
                                if (all == 0) break;
                            }
                            return all>0; // Returning true only if I need to visit some more.
                        });
                        if (all == 0) break;
                    }
                    if (all == 0) break;
                }
            }
            file.close();
        }
        else {
            // Printing the log in its expanded representation, by randomly generating some data!
            std::ofstream xes{dest_name};
            std::vector<std::vector<std::string>> log_collected;

//            std::ofstream file{dest_name};
            if (all > 0) {
                // First, getting this from the ending traces for all of the tries out there
                trie_storage.reconstructor(finals.begin(), finals.end(), [&all,&log_collected,&graph](const std::vector<size_t>& ref, size_t len) {
                    if (all<=0) return false;
                    auto& trace = log_collected.emplace_back();
                    for (size_t i = 0; i<len; i++) {
                        trace.emplace_back(graph.getEdgeLabel(ref.at(i)));
                    }
                    all--;
                    return (all>0);
                });
            }
            trie_storage.single_reconstructor_init();
            if (all > 0) {
                // If I still need to visit something, then go for the join between BFS and DFS paths
                for (const auto& cp : extensions_to_final_node) {
//                std::cout << "for Candidate: " << cp.first << std::endl;
                    auto& bfs_paths = final_node_to_trie_offset[cp.first];
                    DEBUG_ASSERT(!bfs_paths.empty());
                    for (const size_t& final_trie_node_id : bfs_paths) {
                        trie_storage.reconstructor(final_trie_node_id, [&log_collected,&all,&cp,&graph,&ac](const std::vector<size_t>& bfs_path, size_t max_len) {
                            std::vector<std::string> tmp_bfs;
                            for (size_t i = 0; i<max_len; i++) {
                                DEBUG_ASSERT(bfs_path.at(i) < graph.g.E_size);
                                tmp_bfs.emplace_back(graph.getEdgeLabel(bfs_path.at(i)));
                            }
                            DEBUG_ASSERT(tmp_bfs.size() == max_len);
                            std::vector<std::string> trace;
                            for (const auto& extending_path : cp.second) { //
                                // Reconstruction of the BFS prefix path
                                for (const auto& label : tmp_bfs) {
                                    trace.emplace_back(label);
//                                    file << label << "\t";
                                }

#ifdef DEBUG
                                //                            std::cout << "BFS:" << std::endl;
//                            for (size_t i = 0; i <max_len; i++) {
//                                std::cout << graph.g.edge_ids.at(bfs_path.at(i)) << std::endl;
//                            }
//                            std::cout << "DFS:" << std::endl;
//                            for (size_t i = 0, N = extending_path.depth_path.size(); i <N; i++) {
//                                std::cout << graph.g.edge_ids.at(extending_path.depth_path.at(i)) << std::endl;
//                            }
//                            std::cout << "~~~" << std::endl;
                                auto dst_bfs = graph.g.edge_ids.at(bfs_path.at(max_len-1)).second;
                                auto src_dfs = graph.g.edge_ids.at(extending_path.depth_path.at(0)).first;
                                DEBUG_ASSERT(dst_bfs == src_dfs);
#endif

                                // Reconstruction of the DFS suffix path with loops
                                size_t MMM = extending_path.depth_path.size();
                                for (size_t i = 0, N = MMM; i<N; i++) {
                                    auto it = extending_path.loop_extension.find(i);
#ifdef DEBUG
                                    size_t prev = -1;
#endif

                                    // The information that needs to the provided before the edge
                                    if (it != extending_path.loop_extension.end()) {
                                        for (auto& loop_ref : it->second) {
                                            const auto& loop_cycle_ref = ac.cycles_of_edges.at(loop_ref.cycle_id);
#ifdef DEBUG
                                            auto expected = graph.g.edge_ids.at(extending_path.depth_path.at(i)).first;
                                            auto cycle_point = graph.g.edge_ids.at(ac.cycles_of_edges.at(loop_ref.cycle_id).at(loop_ref.cycle_id_offset_start)).first;
                                            auto cycle_b = graph.g.edge_ids.at(loop_cycle_ref.at(0)).first;
                                            auto cycle_e = graph.g.edge_ids.at(loop_cycle_ref.at(loop_cycle_ref.size()-1)).second;
                                            DEBUG_ASSERT(cycle_b == cycle_e);
                                            DEBUG_ASSERT(expected == cycle_point);
                                            if (prev != -1)
                                                DEBUG_ASSERT(expected == prev);
                                            prev = cycle_point;
#endif

                                            std::vector<std::string> tmp_loop;
                                            for (size_t j_element = 0, L = loop_cycle_ref.size(); j_element < L; j_element++) {
                                                tmp_loop.emplace_back(graph.getEdgeLabel(loop_cycle_ref.at((j_element + loop_ref.cycle_id_offset_start) % L)));
                                            }
                                            for (size_t n_times = 0; n_times < loop_ref.n_times_replace; n_times++) {
                                                for (const auto& label : tmp_loop)
                                                    trace.emplace_back(label);
//                                                    file << label << "\t";
                                            }
                                        }
                                    }

#ifdef DEBUG
                                    if (prev != -1)
                                        DEBUG_ASSERT(graph.g.edge_ids.at(extending_path.depth_path.at(i)).first == prev);
#endif

                                    // Visiting the edge
                                    trace.emplace_back(graph.getEdgeLabel(extending_path.depth_path.at(i)));
//                                    file << graph.getEdgeLabel(extending_path.depth_path.at(i)) << "\t";
                                }

                                auto it = extending_path.loop_extension.find(MMM);
                                if (it != extending_path.loop_extension.end()) {
                                    for (auto& loop_ref : it->second) {
                                        const auto& loop_cycle_ref = ac.cycles_of_edges.at(loop_ref.cycle_id);
#ifdef DEBUG
                                        auto expected = graph.g.edge_ids.at(extending_path.depth_path.at(MMM-1)).second;
                                        auto cycle_point = graph.g.edge_ids.at(ac.cycles_of_edges.at(loop_ref.cycle_id).at(loop_ref.cycle_id_offset_start)).first;
                                        auto cycle_b = graph.g.edge_ids.at(loop_cycle_ref.at(0)).first;
                                        auto cycle_e = graph.g.edge_ids.at(loop_cycle_ref.at(loop_cycle_ref.size()-1)).second;
                                        DEBUG_ASSERT(cycle_b == cycle_e);
                                        DEBUG_ASSERT(expected == cycle_point);
#endif

                                        std::vector<std::string> tmp_loop;
                                        for (size_t j_element = 0, L = loop_cycle_ref.size(); j_element < L; j_element++) {
                                            tmp_loop.emplace_back(graph.getEdgeLabel(loop_cycle_ref.at((j_element + loop_ref.cycle_id_offset_start) % L)));
                                        }
                                        for (size_t n_times = 0; n_times < loop_ref.n_times_replace; n_times++) {
                                            for (const auto& label : tmp_loop)
                                                trace.emplace_back(label);
//                                                file << label << "\t";
                                        }
                                    }
                                }

                                log_collected.emplace_back(trace);
                                trace.clear();
//                                file << std::endl;
                                all--;
                                if (all == 0) break;
                            }
                            return all>0; // Returning true only if I need to visit some more.
                        });
                        if (all == 0) break;
                    }
                    if (all == 0) break;
                }
            }
//            file.close();

            begin_log(xes);
            std::mt19937_64 eng{0};
            for (size_t trace_id = 0, LOGS = log_collected.size(); trace_id < LOGS; trace_id++) {
                const auto& trace = log_collected.at(trace_id);
                begin_trace_serialize(xes, std::to_string(all));
                for (size_t i = 0, ts = trace.size(); i<ts; i++) {
                    env.env.ap.serialise_event_in_trace(xes, eng, trace.at(i));
                }
                end_trace_serialize(xes);
            }
//            if (all > 0) {
//                // First, getting this from the ending traces for all of the tries out there
//                trie_storage.reconstructor(finals.begin(), finals.end(), [&all,&xes,&eng,&env,&graph](const std::vector<size_t>& ref, size_t len) {
//                    if (all<=0) return false;
//                    begin_trace_serialize(xes, std::to_string(all));
//                    for (size_t i = 0; i<len; i++) {
//                        env.env.ap.serialise_event_in_trace(xes, eng, graph.getEdgeLabel(ref.at(i)));
//                    }
//                    all--;
//                    end_trace_serialize(xes);
//                    return (all>0);
//                });
//            }
//            trie_storage.single_reconstructor_init();
//            if (all > 0) {
//                // If I still need to visit something, then go for the join between BFS and DFS paths
//                for (const auto& cp : extensions_to_final_node) {
//                    auto& bfs_paths = final_node_to_trie_offset[cp.first];
//                    DEBUG_ASSERT(!bfs_paths.empty());
//                    for (const size_t& final_trie_node_id : bfs_paths) {
//                        trie_storage.reconstructor(final_trie_node_id, [&xes,&all,&cp,&graph,&ac,&eng,&env](const std::vector<size_t>& bfs_path, size_t max_len) {
//                            std::vector<std::string> tmp_bfs;
//                            for (size_t i = 0; i<max_len; i++) {
//                                tmp_bfs.emplace_back(graph.getEdgeLabel(bfs_path.at(i)));
//                            }
//                            DEBUG_ASSERT(tmp_bfs.size() == max_len);
//                            for (const auto& extending_path : cp.second) { //
//                                begin_trace_serialize(xes, std::to_string(all));
//                                // Reconstruction of the BFS prefix path
//                                for (const auto& label : tmp_bfs) {
//                                    env.env.ap.serialise_event_in_trace(xes, eng, label);
//                                }
//                                // Reconstruction of the DFS suffix path with loops
//                                for (size_t i = 0, N = extending_path.depth_path.size(); i<N; i++) {
//                                    auto it = extending_path.loop_extension.find(i);
//                                    // The information that needs to the provided before the edge
//                                    if (it != extending_path.loop_extension.end()) {
//                                        for (auto& loop_ref : it->second) {
//                                            const auto& loop_cycle_ref = ac.cycles_of_edges.at(loop_ref.cycle_id);
//                                            std::vector<std::string> tmp_loop;
//                                            for (size_t j_element = 0, L = loop_cycle_ref.size(); j_element < L; j_element++) {
//                                                tmp_loop.emplace_back(graph.getEdgeLabel(loop_cycle_ref.at((j_element + loop_ref.cycle_id_offset_start) % L)));
//                                            }
//                                            for (size_t n_times = 0; n_times < loop_ref.n_times_replace; n_times++) {
//                                                for (const auto& label : tmp_loop)
//                                                    env.env.ap.serialise_event_in_trace(xes, eng, label);
//                                            }
//                                        }
//                                    }
//
//
//                                    // Visiting the edge
//                                    env.env.ap.serialise_event_in_trace(xes, eng, graph.getEdgeLabel(extending_path.depth_path.at(i)));
//                                }
//                                end_trace_serialize(xes);
//                                all--;
//                                if (all == 0) break;
//                            }
//                            return all>0; // Returning true only if I need to visit some more.
//                        });
//                        if (all == 0)
//                            break;
//                    }
//                    if (all == 0) break;
//                }
//            }
            end_log(xes);
//        env.env.ap.serialize_atom_list_to_xes(atom_lists, file);
            xes.close();
        }
        t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ser_time_c = t2 - t1;
        ser_time = ser_time_c.count();
    }

    std::cout << "Serialisation Time (ms): " << ser_time << std::endl;
    std::cout << "Total Time (ms): " << (graph_gen_time+graph_visit_time+(env.doSkip ? 0.0 : ser_time)) << std::endl;
    csvFile << filename << "," << graph_gen_time << "," <<  graph_visit_time << "," << ser_time << "," << (graph_gen_time+graph_visit_time+(env.doSkip ? 0.0 : ser_time)) << "," << atom_size << "," << minL << "," << maxL << "," << logS << "," << v_size <<"," << e_size << std::endl;
    csvFile.close();
//    std::cout << "#Generated traces: " << std::max(atom_lists.size(), count_sol) << std::endl;
}

#include <args.hxx>

int main(int argc, char* argv[]) {
    args::ArgumentParser parser("This is KnoBAB's Log Generator.", "This log generator exploits the algorithm showcased at GRADES-NDA'23.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Positional<std::string> cache(parser, "cache", "The position where the cache path resides, where each single automata refers to one declarative clause");
    args::Positional<std::string> declare(parser, "declare", "The file describing the declarative model from which extract the clauses");
    args::Positional<std::string> sigma(parser, "sigma", "The set of (non-atomised) symbols to be associated to the model. If the model is atomised, these sets is expanded with the corresponding full set of atomised symbols");
    args::Positional<std::string> csvPath(parser, "csv", "The CSV file where to dump the result of the experiments");
    args::ValueFlag<int> minL(parser, "minL", "The minimum trace length", {'m'});
    args::ValueFlag<int> maxL(parser, "maxL", "The maximum trace length", {'M'});
    args::ValueFlag<int> logS(parser, "L", "The number of distinct traces in the log", {'L'});
    args::ValueFlag<double> sampleRatio(parser, "sample", "The sample ratio for the preliminar random walk", {'s'});
    args::Group group(parser, "This group is all exclusive:", args::Group::Validators::Xor);
    args::Flag tab(group, "tab", "Serialising in a tab separated format", {'t', "tab"});
    args::Flag xes(group, "xes", "Serialising in a XES format", {'x', "xes"});
    args::Flag doSkip(group, "no", "Skips the serialization part", {'N', "no-serialize"});

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help&) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    std::string cache_path = "/home/giacomo/projects/knobab2/data/cache/graph_pattern_cache2";
    std::string model = "/home/giacomo/projects/loggen/convert_same/data/test_gen_2_10_10_10_dataless.powerdecl";
    std::string symbols = "/home/giacomo/projects/loggen/convert_same/data/test_gen_2_10_10_10_knobab_sigma.txt";
    std::string csv = "/home/giacomo/projects/knobab2/data/testing/results/loggen/dump.csv";
    if (cache) cache_path = args::get(cache);
    if (declare) model = args::get(declare);
    if (sigma) symbols = args::get(sigma);
    if (csvPath) csv = args::get(csvPath);

    status s{cache_path};
    if (doSkip) s.doSkip = true;
    else {
        s.doSkip = false;
        s.isTab = (tab);
    }
    if (minL) s.minL = args::get(minL); else s.minL = 10000;
    if (maxL) s.maxL = args::get(maxL); else s.maxL = 10000;
    if (logS) s.nTraces = args::get(logS); else s.nTraces = 10000;
    if (sampleRatio) s.sampleRatio = args::get(sampleRatio); else s.sampleRatio = 0.9;
//    s.max_visited = 1;
//    s.useJohnson = false;
//    s.useJohnsonLimit = 0;
    for_each_test(s, model, symbols, csv);
}