//
// Created by giacomo on 07/01/2022.
//

#include <magic_enum.hpp>
#include "knobab/flloat_deps/DeclareTemplateCollect.h"

const  std::string DeclareTemplateCollect::left_act{"a"};
const  std::string DeclareTemplateCollect::right_act{"b"};

DeclareTemplateCollect::DeclareTemplateCollect(const std::filesystem::path& cachePath) : element{cachePath / "graph_pattern_cache"}{
    if (!std::filesystem::exists(element)) {
       if (! std::filesystem::create_directories(element)) {
           throw std::runtime_error(element.string() + "path does not exists");
       }
    }
}

NodeLabelBijectionFA<std::string, easy_prop>
DeclareTemplateCollect::getDeclareTemplate(const declare_templates type, size_t nargs) {
    auto cp = std::make_pair(type, nargs);
    auto it = graph_map.find(cp);
    if (it == graph_map.end()) {
        // Checking if the file is in cache
        std::string_view w = magic_enum::enum_name(type);
        std::string name{w.data(), w.size()};
        name = name + "_" + std::to_string(nargs) ;
        std::filesystem::path cache_file = element / name;
        if (!std::filesystem::exists(cache_file)) {
            // if the file is not in cache: create it!
            ltlf input_forumla;
            if (isUnaryPredicate(type)) {
                input_forumla = toFiniteSemantics(type, nargs, left_act);
            } else {
                input_forumla = toFiniteSemantics(type, nargs,  left_act, right_act);
            }

            std::stringstream s;
            s << input_forumla;
            std::string dot_graph = callable(s.str());
            {
                std::ofstream file{cache_file};
                file << dot_graph;
            }
            ParseFFLOATDot graph_loader;
            std::istringstream strm{dot_graph};
            auto tmp = graph_loader.parse(strm);
            graph_map[cp] = tmp;
            return tmp;
        } else {
            ParseFFLOATDot graph_loader;
            std::ifstream  strm{cache_file};
            auto tmp = graph_loader.parse(strm);
            graph_map[cp] = tmp;
            return tmp;
        }
    } else return it->second;
}