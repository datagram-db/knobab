//
// Created by giacomo on 07/10/24.
//

#ifndef DTMINING_TEMPORARYCAPNPNCONSTITUENTSERIALIZER_H
#define DTMINING_TEMPORARYCAPNPNCONSTITUENTSERIALIZER_H

#include <unordered_map>
#include <string>
#include <filesystem>
#include <fstream>
extern "C" {
#include <unistd.h>
#include <fcntl.h>
};
//#include <capnp_utils.h>
//#include "schema.propo.h"
#include <dt_mining/data/CacheConstituent.h>
//#include <lrucache.hpp>
#include <limits.h>


struct capnp_constituent_serializer {
    capnp_constituent_serializer(const std::string& filename, size_t cache_size=1000) : /*cache(cache_size),*/ filename{filename}, moved_for_read{false}{
        size_t tmpsize = 0;
        if (std::filesystem::exists(filename)) {
//            std::ifstream infile(filename+"_map.tab");
//            std::string line;
//            while (std::getline(infile, line))
//            {
//                std::stringstream ss;
//                ss.str(line);
//                std::string item;
//                size_t key, value;
//                if (std::getline(ss, item, '\t')) {
//                    sscanf(item.c_str(), "%zu", &key);
//                    greatestObjectId = std::max((ssize_t)key, greatestObjectId);
//                    if (std::getline(ss, item)) {
//                        sscanf(item.c_str(), "%zu", &value);
//                        metObjectIds.emplace(key, value);
//                    }
//                }
//                tmpsize = std::filesize(filename);
//
//            }
        }
        fd=open(filename.c_str(), O_CREAT|O_RDWR|O_TRUNC, 0600);
        ::close(fd);
        fd = -1;
//        if (tmpsize>0)
//            lseek(fd, tmpsize, SEEK_SET);
//        mapping_file.open(filename+"_map.tab");
    }


    ssize_t getGreatestObjectId() const {
        return greatestObjectId;
    }

//    std::pair<bool,size_t> serialize(size_t object_id,
//                                     const std::unordered_map<std::string,double>& payload,
//                                     size_t timeOfInterest,
//                                     const std::string& clazz);
//    std::pair<bool,size_t> serialize(size_t object_id,
//                                     const std::string& dt_pattern_name,
//                                     const std::string& dimension_name,
//                                     size_t span,
//                                     size_t timeOfInterest,
//                                     const std::unordered_map<std::string,double>& payload);
//    std::pair<bool,size_t> serialize(size_t object_id,
//                                     const std::string& label,
//                                     size_t span,
//                                     size_t timeOfInterest,
//                                     const std::unordered_map<std::string,double>& payload);

//
    static inline CacheConstituent build(size_t object_id,
                                                                   const std::string& label,
                                                                   size_t span,
                                                                   size_t timeOfInterest,
                                                                   const std::unordered_map<std::string,double>& payload,
                                                                   std::string clazz_ = "") {
        CacheConstituent object;
        object.label = label;
        object.clazz = clazz_;
        object.span = span;
        object.disctime = timeOfInterest;
        object.payload = payload;
//        constexpr double eps = 2.220446049250313080847e-16;
//        for (const auto& [k,v] : payload) {
//            if (std::abs(v) > eps)
//                object.payload.emplace(k, v);
//        }
        return object;
    }
//
    static inline CacheConstituent build(size_t object_id,
                                                                   const std::unordered_map<std::string,double>& payload,
                                                                   size_t timeOfInterest,
                                                                   const std::string& clazz) {
//        std::lock_guard<std::mutex> guard(g_pages_mutex);
//        if (!metObjectIds.contains(object_id)) {
            CacheConstituent object;
            object.label = "__raw_data";
            object.clazz = clazz;
            object.span = 1;
            object.disctime = timeOfInterest;
            object.payload = payload;
            return object;
//        for (size_t idx = 0, N = dimNames.size(); idx<N; idx++) {
//            object.payload.emplace(dimNames.at(idx), allDimensions[idx][timeOfInterest]);
//        }
//            cache.put(object_id, object);
//            if (moved_for_read) {
//                lseek(fd, 0, SEEK_END);
//                moved_for_read = false;
//            }
//            off_t send;
//            size_t offset = lseek (fd, 0, SEEK_CUR);
//            ::capnp::MallocMessageBuilder message;
//            auto builder = message.initRoot<Constituent>();
//            capnp_serialize_raw_data(builder, payload, timeOfInterest, clazz);
//            writePackedMessageToFd(fd, message);
//            auto words = messageToFlatArray(message);
//            kj::ArrayPtr<kj::byte> bytes = words.asBytes();
//            write(fd, bytes.begin(), bytes.size());
//            fsync(fd);
//            metObjectIds.emplace(object_id, offset);
//            mapping_file << object_id << '\t' << offset << std::endl;
//            return {true, offset};
//        } else {
//            return {false, 0};
//        }
    }
//
//    const CacheConstituent& retrieve(size_t id) {
//        std::lock_guard<std::mutex> guard(g_pages_mutex);
//        auto it = metObjectIds.find(id);
//        if (it == metObjectIds.end()) {
//            return empty_object;
//        } else {
//            if (cache.exists(id)) {
//                return cache.get(id);
//            } else {
//                if (fd != -1) {
//                    moved_for_read = true;
//                    lseek(fd, it->second, SEEK_SET);
//                    ::capnp::PackedFdMessageReader message(fd);
//                    auto reader = message.getRoot<Constituent>();
//                    CacheConstituent obj;
//                    obj.clazz = reader.getClazz();
//                    obj.label = reader.getLabel();
//                    obj.disctime = reader.getDisctime();
//                    obj.span = reader.getSpan();
//                    for (const auto& payload_entry: reader.getPayload()) {
//                        obj.payload.emplace(payload_entry.getKey(),payload_entry.getValue());
//                    }
//                    cache.put(id, obj);
//                    return cache.get(id);
//                } else {
//                    return empty_object;
//                }
//            }
//
//        }
//    }

    void close() {
//        ::close(fd);
//        mapping_file.close();
//        fd = -1;
    }

private:
//    CacheConstituent empty_object;
//    std::ofstream mapping_file;
//    std::mutex g_pages_mutex;
    bool moved_for_read;
//    std::unordered_map<size_t, size_t> metObjectIds;
    int fd;
    std::string filename;
    ssize_t greatestObjectId = -1;
//    cache::lru_cache<size_t, CacheConstituent> cache;
};


#endif //DTMINING_TEMPORARYCAPNPNCONSTITUENTSERIALIZER_H
