//
// Created by giacomo on 07/10/24.
//

#ifndef DTMINING_TEMPORARYCAPNPNCONSTITUENTSERIALIZER_H
#define DTMINING_TEMPORARYCAPNPNCONSTITUENTSERIALIZER_H

#include <string>
#include <filesystem>
#include <fstream>
#include <capnp_utils.h>
#include "schema.propo.h"
#include <data/CacheConstituent.h>
#include <lrucache.hpp>

struct capnp_constituent_serializer {
    capnp_constituent_serializer(const std::string& filename, size_t cache_size=1000) : cache(cache_size), filename{filename}, moved_for_read{false}{
        size_t tmpsize = 0;
        if (std::filesystem::exists(filename)) {
            std::ifstream infile(filename+"_map.tab");
            std::string line;
            while (std::getline(infile, line))
            {
                std::stringstream ss;
                ss.str(line);
                std::string item;
                size_t key, value;
                if (std::getline(ss, item, '\t')) {
                    sscanf(item.c_str(), "%zu", &key);
                    greatestObjectId = std::max((ssize_t)key, greatestObjectId);
                    if (std::getline(ss, item)) {
                        sscanf(item.c_str(), "%zu", &value);
                        metObjectIds.emplace(key, value);
                    }
                }
                tmpsize = filesize(filename);

            }
        }
        fd=open(filename.c_str(), O_CREAT|O_RDWR|O_TRUNC, 0600);
        if (tmpsize>0)
            lseek(fd, tmpsize, SEEK_SET);
        mapping_file.open(filename+"_map.tab");
    }


    ssize_t getGreatestObjectId() const {
        return greatestObjectId;
    }

    std::pair<bool,size_t> serialize(size_t object_id,
                                     const std::unordered_map<std::string,double>& payload,
                                     size_t timeOfInterest,
                                     const std::string& clazz);
//    std::pair<bool,size_t> serialize(size_t object_id,
//                                     const std::string& dt_pattern_name,
//                                     const std::string& dimension_name,
//                                     size_t span,
//                                     size_t timeOfInterest,
//                                     const std::unordered_map<std::string,double>& payload);
    std::pair<bool,size_t> serialize(size_t object_id,
                                     const std::string& label,
                                     size_t span,
                                     size_t timeOfInterest,
                                     const std::unordered_map<std::string,double>& payload);


    const CacheConstituent& retrieve(size_t id) {
        std::lock_guard<std::mutex> guard(g_pages_mutex);
        auto it = metObjectIds.find(id);
        if (it == metObjectIds.end()) {
            return empty_object;
        } else {
            if (cache.exists(id)) {
                return cache.get(id);
            } else {
                if (fd != -1) {
                    moved_for_read = true;
                    lseek(fd, it->second, SEEK_SET);
                    ::capnp::PackedFdMessageReader message(fd);
                    auto reader = message.getRoot<Constituent>();
                    CacheConstituent obj;
                    obj.clazz = reader.getClazz();
                    obj.label = reader.getLabel();
                    obj.disctime = reader.getDisctime();
                    obj.span = reader.getSpan();
                    for (const auto& payload_entry: reader.getPayload()) {
                        obj.payload.emplace(payload_entry.getKey(),payload_entry.getValue());
                    }
                    cache.put(id, obj);
                    return cache.get(id);
                } else {
                    return empty_object;
                }
            }

        }
    }

    void close() {
        ::close(fd);
        mapping_file.close();
        fd = -1;
    }

private:
    CacheConstituent empty_object;
    std::ofstream mapping_file;
    std::mutex g_pages_mutex;
    bool moved_for_read;
    std::unordered_map<size_t, size_t> metObjectIds;
    int fd;
    std::string filename;
    ssize_t greatestObjectId = -1;
    cache::lru_cache<size_t, CacheConstituent> cache;
};


#endif //DTMINING_TEMPORARYCAPNPNCONSTITUENTSERIALIZER_H
