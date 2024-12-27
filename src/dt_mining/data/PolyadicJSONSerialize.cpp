//
// Created by giacomo on 05/10/24.
//

#include "dt_mining/data/PolyadicJSONSerialize.h"

PolyadicJSONSerialize::PolyadicJSONSerialize(std::ostream& os): jw{os, JsonWriter::flags::F_AUTO_CLOSE | JsonWriter::flags::F_STRICT | JsonWriter::flags::F_VERIFY_ON_EXIT} {
conversion_recto["literal"] = XES_Literal;
conversion_verso[XES_Literal] = "literal";

conversion_recto["continuous"] = XES_Continuous;
conversion_verso[XES_Continuous] = "continuous";

conversion_recto["boolean"] = XES_Boolean;
conversion_verso[XES_Boolean] = "boolean";

conversion_recto["discrete"] = XES_Discrete;
conversion_verso[XES_Discrete] = "discrete";

types["__label"] = types["__class"] = XES_Literal;
types["span"] = types["time"] = XES_Discrete;

hierarchy["__raw_data"].emplace("__raw_data");
hierarchy["__trace_payload"].emplace("__trace_payload");
hierarchy["__payload"].emplace("__payload");

}

#include <capnp_utils.h>

//size_t capnp_serialize_raw_data(int fd, const std::vector<std::string>& dimNames,
//                 const std::vector<std::vector<double>>& allDimensions,
//                 size_t timeOfInterest,
//                 const std::string& clazz) {
//    capnp_message_builder<Constituent> builder(fd);
//    size_t offset = builder.get_current_offset();
//    builder().setLabel("__raw_data");
//    builder().setClazz(clazz);
//    builder().setSpan(1);
//    builder().setDisctime(timeOfInterest);
//    auto payload = builder().initPayload(dimNames.size());
//    for (size_t idx = 0, N = dimNames.size(); idx<N; idx++) {
//        payload[idx].setKey(dimNames.at(idx));
//        payload[idx].setValue(allDimensions[idx][timeOfInterest]);
//    }
//    return offset;
//}

//size_t capnp_serialize_event_data_data(int fd, const std::string& dt_pattern_name,
//                                const std::string& dimension_name,
//                                size_t span,
//                                size_t timeOfInterest,
//                                const std::unordered_map<std::string,double>& payload) {
//    capnp_message_builder<Constituent> builder(fd);
//    size_t offset = builder.get_current_offset();
//    auto label = dt_pattern_name + "(" + dimension_name +")";
//    builder().setLabel(label);
//    builder().setClazz("");
//    builder().setSpan(span);
//    builder().setDisctime(timeOfInterest);
//    auto payload_ = builder().initPayload(payload.size());
//    size_t idx = 0;
//    for (const auto&  [k,v] : payload) {
//        payload_[idx].setKey(k);
//        payload_[idx].setValue(v);
//        idx++;
//    }
//    return offset;
//}