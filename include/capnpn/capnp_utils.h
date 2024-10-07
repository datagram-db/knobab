//
// Created by giacomo on 07/10/24.
//

#ifndef DTMINING_UTILS_H
#define DTMINING_UTILS_H

#include <capnp/message.h>
#include <capnp/serialize-packed.h>

template <typename T>
struct capnp_message_builder  {

    capnp_message_builder(int fd) : fd{fd},
                                    builder{message.initRoot<T>()} {
        off_t send;
        scur = lseek (fd, 0, SEEK_CUR);
    }

    ~capnp_message_builder() {
        writePackedMessageToFd(fd, message);
        auto words = messageToFlatArray(message);
        kj::ArrayPtr<kj::byte> bytes = words.asBytes();
        write(fd, bytes.begin(), bytes.size());
        fsync(fd);
    }

    inline off_t get_current_offset() const {
        return scur;
    }

    T::Builder& operator()() {
        return builder;
    }

private:
    T::Builder builder;
    ::capnp::MallocMessageBuilder message;
    int fd;
    off_t scur;
};



template <typename T>
struct capnp_message_deserializer {
    capnp_message_deserializer(int fd) : fd{fd} {}

    T::Reader getReader() {
        ::capnp::PackedFdMessageReader message(fd);
        return message.getRoot<T>();
    }

private:
    int fd;
};

#include <fstream>
#include <filesystem>
#include <functional>
extern "C" {
#include <unistd.h>
#include <fcntl.h>
}

static inline
size_t filesize(const std::filesystem::path& filename)
{
    if (std::filesystem::exists(filename)) {
        return std::filesystem::file_size(filename);
    }
    return 0;
}

#include "lrucache.hpp"

template <typename T, typename K>
        struct capnp_message_serializer {
            capnp_message_serializer(const std::string& filename) : filename{filename}, moved_for_read{false}{
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

            template <typename KToTBuilder>
            std::pair<bool,size_t> serialize(size_t object_id, const K& object, KToTBuilder& msg) {
                std::lock_guard<std::mutex> guard(g_pages_mutex);
                if (!metObjectIds.contains(object_id)) {
                    cache.put(object_id, object);
                    if (moved_for_read) {
                        lseek(fd, 0, SEEK_END);
                        moved_for_read = false;
                    }
                    off_t send;
                    size_t offset = lseek (fd, 0, SEEK_CUR);
                    ::capnp::MallocMessageBuilder message;
                    auto builder = message.initRoot<T>();
                    msg(object, builder);
                    metObjectIds.emplace(object_id, offset);
                    mapping_file << object.id << '\t' << offset << std::endl;
                    return {true, offset};
                } else {
                    return {false, 0};
                }
            }

            template <typename TReaderToK>
            K retrieve(size_t id, TReaderToK& fun) {
                std::lock_guard<std::mutex> guard(g_pages_mutex);
                auto it = metObjectIds.find(id);
                if (it == metObjectIds.end()) {
                    return empty_object;
                } else {
                    if (cache.exists(id) || (fd == -1)) {
                        return cache.get(id);
                    } else if (fd != -1) {
                        moved_for_read = true;
                        lseek(fd, it->second, SEEK_SET);
                        ::capnp::PackedFdMessageReader message(fd);
                        return TReaderToK(message.getRoot<T>());
                    } else {
                        return empty_object;
                    }
                }
            }

            void close() {
                ::close(fd);
                mapping_file.close();
                fd = -1;
            }

        private:
            K empty_object;
            std::ofstream mapping_file;
            std::mutex g_pages_mutex;
            bool moved_for_read;
            std::unordered_map<size_t, size_t> metObjectIds;
            int fd;
            std::string filename;
            ssize_t greatestObjectId = -1;
            cache::lru_cache<size_t, K> cache;
};

#endif //DTMINING_UTILS_H
