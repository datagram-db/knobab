//
// Created by giacomo on 26/07/21.
//

#include "yaucl/bpm/declare_to_dfa/DeclareNoDataTemplateCollect.h"
#include <magic_enum.hpp>
#include <yaucl/graphs/graph_join_pm_conversion.h>


DeclareNoDataTemplateCollect::DeclareNoDataTemplateCollect(bool doPrune, const std::string &base_serialization_path) : doPrune(doPrune), isAdding(true), base_serialization_path{base_serialization_path} {
    if (std::filesystem::exists(base_serialization_path)) {
        assert(std::filesystem::is_directory(base_serialization_path));
    } else {
        std::filesystem::create_directories(base_serialization_path);
    }
}


void DeclareNoDataTemplateCollect::add(const DeclareDataAware &left) {
    isAdding = true;
    allTemplates[std::make_pair(left.casusu, left.n)].insert(left);
}

#include <fstream>
#include <bzdb/flloat_deps/ParseFFLOATDot.h>


static inline void conditionalPruningGraph(bool doPrune, bool firstInsertion, TemplateCollectResult& result, graph_join_pm& currGraph) {
    if (!doPrune) {
        result.distinct_graph_model.emplace_back(currGraph);
        if (firstInsertion) {
            result.joined_graph_model = currGraph;
        } else {
            result.joined_graph_model = graph_join(result.joined_graph_model, currGraph);
        }
    } else {
        graph_join_pm result_;
        remove_unaccepting_states(currGraph, result_);
        result.distinct_graph_model.emplace_back(result_);
        if (firstInsertion) {
            result.joined_graph_model = result_;
        } else {
            result.joined_graph_model = graph_join(result.joined_graph_model, result_);
        }
    }
}

TemplateCollectResult DeclareNoDataTemplateCollect::buildUpModel(const std::vector<DeclareDataAware> &Model) {
    std::vector<graph_join_pm> distinct_graph_model;
    graph_join_pm              joined_graph_model;
    size_t M = Model.size();
    if (M == 0) {
        return {};
    } else {
        auto& zeroModel = Model.at(0);
        assert(allTemplates.contains(std::make_pair(zeroModel.casusu, zeroModel.n)));
        assert(allTemplates[std::make_pair(zeroModel.casusu, zeroModel.n)].contains(zeroModel));
        auto& currGraph = template_to_graph.at(zeroModel);
        TemplateCollectResult result;
        conditionalPruningGraph(doPrune, true, result, currGraph);
        for (size_t j = 1; j<M; j++) {
            auto& zeroModelJ = Model.at(j);
            assert(allTemplates.contains(std::make_pair(zeroModelJ.casusu, zeroModelJ.n)));
            assert(allTemplates[std::make_pair(zeroModelJ.casusu, zeroModelJ.n)].contains(zeroModelJ));
            auto& currGraph2 = template_to_graph.at(zeroModelJ);
            conditionalPruningGraph(doPrune, false, result, currGraph2);
        }
        if (doPrune) {
            graph_join_pm result_;
            remove_unaccepting_states(result.joined_graph_model, result_);
            result.joined_graph_model = result_;
        }
        return result;
    }
}

graph_join_pm operatore(const std::string &dot, std::unordered_map<std::string, std::string> *ptr,
                        const std::unordered_set<std::string> &SigmaAll) {
    graph_join_pm result;
    ParseFFLOATDot graph_loader;
    graph_loader.need_back_conversion = ptr != nullptr;
    graph_loader.back_conv = ptr;
    {
        graph_join_pm g2;
        {
            ///std::cout << tmp << std::endl << std::endl;
            std::istringstream strm{dot};
            auto g = graph_loader
                    .parse(strm, SigmaAll)
                    .makeDFAAsInTheory(SigmaAll)
                    .shiftLabelsToNodes();
            g.pruneUnreachableNodes();
            g.dot(std::cout);

            convert_to_novel_graph(g, g2);
        }
        minimize(g2, result);
    }
    return result;
}