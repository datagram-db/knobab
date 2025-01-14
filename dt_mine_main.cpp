



//#include <nlohmann/json.hpp>
//struct FineGrainedStats {
//    size_t experiment_number;
//    std::string env_name;
//    size_t event_id;
//    size_t nConstituents;
//    double event_wise_mine_time;
//    double event_wise_serial_time;
//    size_t unique_event_label;
//
//    nlohmann::json asJson() const {
//        nlohmann::json dom = nlohmann::json::object();
//        dom["experiment_number"] = experiment_number;
//        dom["env_name"] = env_name;
//        dom["event_id"] = event_id;
//        dom["nConstituents"] = nConstituents;
//        dom["event_wise_mine_time"] = event_wise_mine_time;
//        dom["event_wise_serial_time"] = event_wise_serial_time;
//        dom["unique_event_label"] = unique_event_label;
//        return dom;
//    };
//};

//struct SegmentStats {
//    size_t experiment_number;
//    std::string env_name;
//    size_t unique_event_label;
//    size_t clazzSegmentSize;
//    size_t nConstituents;
//    double event_wise_mine_time;
//    double event_wise_serial_time;
//    std::vector<FineGrainedStats> fineStats;
//
//    nlohmann::json asJson() const {
//        nlohmann::json dom = nlohmann::json::object();
//        dom["experiment_number"] = experiment_number;
//        dom["env_name"] = env_name;
//        dom["clazzSegmentSize"] = clazzSegmentSize;
//        dom["nConstituents"] = nConstituents;
//        dom["event_wise_mine_time"] = event_wise_mine_time;
//        dom["event_wise_serial_time"] = event_wise_serial_time;
//        dom["unique_event_label"] = unique_event_label;
//        auto fs = nlohmann::json::array();
//        for (const auto& item : fineStats) {
//            fs.emplace_back(item.asJson());
//        }
//        dom["fineStats"] = fs;
//        return dom;
//    };
//};
#include <dt_mining/dt_mining.h>


int main() {

    // Function parameters
    std::string benchmark_result_file = "benchmarks.json";
    std::string folder = "/home/giacomo/projects/knobab2_loggen/EMeriTAte/italy_power_demand"; //"/home/giacomo/projects/knobab2_loggen/EMeriTAte/test";
    double precision = 0.0001;
    double max_val = 1000000000.0;
    bool isAlgo3 = false;

    DTMining::dt_mine_and_ts_to_polyadic(benchmark_result_file, folder, precision, max_val, isAlgo3, true);

    // Function

    return 0;
}
