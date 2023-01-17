//
// Created by giacomo on 05/10/22.
//

#include <knobab/server/query_manager/Environment.h>
#include <knobab/algorithms/mining/pattern_mining.h>

int main() {
    Environment env;
    env.doStats = false;
    auto scripts = std::filesystem::current_path();
    std::filesystem::path file{"/home/giacomo/projects/knobab2/data/benchmarking/completed/1000_1000_log.tab"};
//    auto file = scripts / "data" / "testing" / "declare" / "mining" / "response_test";
    {
        std::ifstream if_{file};
        env.load_log(TAB_SEPARATED_EVENTS, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    std::cout << "Starting from now!" << std::endl;
    auto list = pattern_mining(env.db, 0.01, false, true, true, false, false);
    for (const auto& result : list) {
        std::cout << result << std::endl;
    }
    return 0;
}