//
// Created by giacomo on 07/10/24.
//

#include "dt_mining/data/TemporaryCapnpnConstituentSerializer.h"
//#include <capnp_utils.h>

//void capnp_serialize_raw_data(Constituent::Builder& builder,  const std::unordered_map<std::string,double>& payload,
//                                size_t timeOfInterest,
//                                const std::string& clazz) {
////    capnp_message_builder<Constituent> builder(fd);
////    size_t offset = builder.get_current_offset();
//    builder.setLabel("__raw_data");
//    builder.setClazz(clazz);
//    builder.setSpan(1);
//    builder.setDisctime(timeOfInterest);
//    auto payload_ = builder.initPayload(payload.size());
////    auto payload = builder().initPayload(dimNames.size());
////    for (size_t idx = 0, N = dimNames.size(); idx<N; idx++) {
////        payload[idx].setKey(dimNames.at(idx));
////        payload[idx].setValue(allDimensions[idx][timeOfInterest]);
////    }
//    size_t idx = 0;
//    for (const auto&  [k,v] : payload) {
//        payload_[idx].setKey(k);
//        payload_[idx].setValue(v);
//        idx++;
//    }
////    return offset;
//}

//std::pair<bool,size_t> capnp_constituent_serializer::serialize(size_t object_id,
//                                                               const std::unordered_map<std::string,double>& payload,
//                                 size_t timeOfInterest,
//                                 const std::string& clazz) {
//    std::lock_guard<std::mutex> guard(g_pages_mutex);
//    if (!metObjectIds.contains(object_id)) {
//        CacheConstituent object;
//        object.label = "__raw_data";
//        object.clazz = clazz;
//        object.span = 1;
//        object.disctime = timeOfInterest;
//        object.payload = payload;
////        for (size_t idx = 0, N = dimNames.size(); idx<N; idx++) {
////            object.payload.emplace(dimNames.at(idx), allDimensions[idx][timeOfInterest]);
////        }
//        cache.put(object_id, object);
//        if (moved_for_read) {
//            lseek(fd, 0, SEEK_END);
//            moved_for_read = false;
//        }
//        off_t send;
//        size_t offset = lseek (fd, 0, SEEK_CUR);
//        ::capnp::MallocMessageBuilder message;
//        auto builder = message.initRoot<Constituent>();
//        capnp_serialize_raw_data(builder, payload, timeOfInterest, clazz);
//        writePackedMessageToFd(fd, message);
//        auto words = messageToFlatArray(message);
//        kj::ArrayPtr<kj::byte> bytes = words.asBytes();
//        write(fd, bytes.begin(), bytes.size());
//        fsync(fd);
//        metObjectIds.emplace(object_id, offset);
//        mapping_file << object_id << '\t' << offset << std::endl;
//        return {true, offset};
//    } else {
//        return {false, 0};
//    }
//}

//void capnp_serialize_event_data_data(Constituent::Builder& builder, const std::string& label,
//                                       size_t span,
//                                       size_t timeOfInterest,
//                                       const std::unordered_map<std::string,double>& payload) {
////    capnp_message_builder<Constituent> builder(fd);
////    size_t offset = builder.get_current_offset();
//
//    builder.setLabel(label);
//    builder.setClazz("");
//    builder.setSpan(span);
//    builder.setDisctime(timeOfInterest);
//    auto payload_ = builder.initPayload(payload.size());
//    size_t idx = 0;
//    for (const auto&  [k,v] : payload) {
//        payload_[idx].setKey(k);
//        payload_[idx].setValue(v);
//        idx++;
//    }
//}

//std::pair<bool,size_t> capnp_constituent_serializer::serialize(size_t object_id,
//                                 const std::string& label,
//                                 size_t span,
//                                 size_t timeOfInterest,
//                                 const std::unordered_map<std::string,double>& payload) {
//    std::lock_guard<std::mutex> guard(g_pages_mutex);
//    if (!metObjectIds.contains(object_id)) {
//        CacheConstituent object;
//        object.label = label;
//        object.clazz = "";
//        object.span = span;
//        object.disctime = timeOfInterest;
//        object.payload = payload;
//        cache.put(object_id, object);
//        if (moved_for_read) {
//            lseek(fd, 0, SEEK_END);
//            moved_for_read = false;
//        }
//        off_t send;
//        size_t offset = lseek (fd, 0, SEEK_CUR);
//        ::capnp::MallocMessageBuilder message;
//        auto builder = message.initRoot<Constituent>();
//        capnp_serialize_event_data_data(builder, label, span, timeOfInterest, payload);
//        writePackedMessageToFd(fd, message);
//        auto words = messageToFlatArray(message);
//        kj::ArrayPtr<kj::byte> bytes = words.asBytes();
//        write(fd, bytes.begin(), bytes.size());
//        fsync(fd);
//        metObjectIds.emplace(object_id, offset);
//        mapping_file << object_id << '\t' << offset << std::endl;
//        return {true, offset};
//    } else {
//        return {false, 0};
//    }
//}


//std::pair<bool,size_t> capnp_constituent_serializer::serialize(size_t object_id,
//                                 const std::string& dt_pattern_name,
//                                 const std::string& dimension_name,
//                                 size_t span,
//                                 size_t timeOfInterest,
//                                 const std::unordered_map<std::string,double>& payload) {
//    std::lock_guard<std::mutex> guard(g_pages_mutex);
//    if (!metObjectIds.contains(object_id)) {
//        CacheConstituent object;
//        object.label = dt_pattern_name + "(" + dimension_name +")";
//        object.clazz = "";
//        object.span = span;
//        object.disctime = timeOfInterest;
//        object.payload = payload;
//        cache.put(object_id, object);
//        if (moved_for_read) {
//            lseek(fd, 0, SEEK_END);
//            moved_for_read = false;
//        }
//        off_t send;
//        size_t offset = lseek (fd, 0, SEEK_CUR);
//        ::capnp::MallocMessageBuilder message;
//        auto builder = message.initRoot<Constituent>();
//        capnp_serialize_event_data_data(fd, dt_pattern_name, dimension_name, span, timeOfInterest, payload);
//        writePackedMessageToFd(fd, message);
//        auto words = messageToFlatArray(message);
//        kj::ArrayPtr<kj::byte> bytes = words.asBytes();
//        write(fd, bytes.begin(), bytes.size());
//        fsync(fd);
//        metObjectIds.emplace(object_id, offset);
//        mapping_file << object_id << '\t' << offset << std::endl;
//        return {true, offset};
//    } else {
//        return {false, 0};
//    }
//}