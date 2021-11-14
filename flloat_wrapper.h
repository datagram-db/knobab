//
// Created by giacomo on 14/11/2021.
//

#ifndef KNOBAB_FLLOAT_WRAPPER_H
#define KNOBAB_FLLOAT_WRAPPER_H

#include <boost/python.hpp>
#include <yaucl/graphs/graph_join_pm.h>
#include <bzdb/flloat_deps/ParseFFLOATDot.h>
#include <yaucl/graphs/graph_join_pm_conversion.h>
#include <yaucl/graphs/graph_join_pm_algorithms.h>


class FLLOAT {
    boost::python::object flloat_parser_ltlf, flloat_ltlf, parser;
    std::string parseToGraphViz(const std::string& ltlf);
public:
    FLLOAT();

    graph_join_pm operator()(const std::string& ltlf, const std::unordered_set<std::string>& SigmaAll) {
        graph_join_pm result;
        ParseFFLOATDot graph_loader;
        {
            graph_join_pm g2;
            {
                std::istringstream strm{parseToGraphViz(ltlf)};
                auto g = graph_loader
                        .parse(strm, SigmaAll)
                        .makeDFAAsInTheory(SigmaAll)
                        .shiftLabelsToNodes();
                g.pruneUnreachableNodes();

                convert_to_novel_graph(g, g2);
            }
            minimize(g2, result);
        }
        return result;
    }
};

#endif //KNOBAB_FLLOAT_WRAPPER_H
