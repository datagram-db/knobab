//
// Created by giacomo on 07/10/24.
//

#ifndef DTMINING_CACHECONSTITUENT_H
#define DTMINING_CACHECONSTITUENT_H

#include <string>
#include <unordered_map>

struct CacheConstituent {
    std::string label;
    std::string clazz;
    size_t span;
    size_t disctime;
    std::unordered_map<std::string,double> payload;
};

#endif //DTMINING_CACHECONSTITUENT_H
