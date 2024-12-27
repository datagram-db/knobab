//
// Created by giacomo on 27/12/24.
//

#ifndef DTMINING_DT_MINING_H
#define DTMINING_DT_MINING_H

#include <string>

 namespace DTMining {
     void dt_mine_and_ts_to_polyadic(const std::string& benchmark_result_file,
                                     const std::string& folder,
                                     double precision,
                                     double max_val,
                                     bool isAlgo3);
}

#endif //DTMINING_DT_MINING_H
