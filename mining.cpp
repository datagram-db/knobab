//
// Created by giacomo on 05/10/22.
//

#include <knobab/Environment.h>
#include <knobab/algorithms/mining/pattern_mining.h>

int main() {
    Environment env;
    env.doStats = false;
    env.set_grounding_parameters(true, false, true,GroundingStrategyConf::NO_EXPANSION);
    env.set_atomization_parameters("p", 20);
    auto scripts = std::filesystem::current_path();
    auto file = scripts / "data" / "testing" / "declare" / "mining" / "response_test";
    {
        std::ifstream if_{file};
        env.load_log(TAB_SEPARATED_EVENTS, true, file.string(), true, if_);
    }
    std::filesystem::path declare_file_path, maxsat;
    for (const auto& result : pattern_mining(env.db, 0.01, false, true, true, false, true)) {
        std::cout << result << std::endl;
    }
    return 0;
}