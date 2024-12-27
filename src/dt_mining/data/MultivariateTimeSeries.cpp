//
// Created by giacomo on 05/10/24.
//

#include "dt_mining/data/MultivariateTimeSeries.h"
#include <unordered_set>
#include "dt_mining/data/dateparse.h"

MultivariateTimeSeries::MultivariateTimeSeries(const std::string& environment, aria::csv::CsvParser& parser,
                                               const double epsilon,
                                               const double maxval) : envName{environment}{
    size_t nDimensions = 0;
//    std::vector<std::string> dimName;
//    std::vector<std::vector<double>> dimensions;
//    std::vector<std::string> timestamp;
//    std::vector<int> clazz;
//    size_t clazzOffset = 0;
//    size_t timeOffset = 0;
    bool first = true;
    bool firstData = true;
    size_t len = 0;
    int prevClazz;
    size_t latestDataOffset;
    for (const auto& row : parser) {
        if (first) {
            nDimensions = row.size()-2;
            dimName.reserve(nDimensions);
            for (size_t idx = 0; idx<nDimensions; idx++) {
                dimName.emplace_back(row[idx]);
            }
//            clazzOffset = row.size()-2;
//            timeOffset = row.size()-1;
            first = false;
            dimensions.resize(nDimensions);
            timestamp.reserve(nDimensions);
//            clazz.reserve(nDimensions);
        } else {
            size_t countNone = 0;
            std::unordered_set<size_t> missing_elements;
            for (size_t idx = 0; idx<nDimensions; idx++) {
                const auto& entry = row[idx];
                if ((!entry.empty()) && (entry != "NA")) {
                    dimensions[idx].emplace_back(std::stod(entry));
                } else {
                    missing_elements.emplace(idx);
                    countNone++;
                }
            }
            if (countNone < nDimensions) {
                auto currentClazz = std::stoi(row[nDimensions]);
                timestamp.emplace_back(parse8601(row[nDimensions+1]));
                for (size_t missingReplace : missing_elements)
                    dimensions[missingReplace].emplace_back(0.0);
                if (firstData) {
                    firstData = false;
                    prevClazz = currentClazz;
                    latestDataOffset = len;
                } else if (prevClazz != currentClazz) {
                    classSegments.emplace_back(dimName, prevClazz, dimensions, timestamp, latestDataOffset, len-1, epsilon,maxval);
                    prevClazz = currentClazz;
                    latestDataOffset = len;
                }
                len++;
            }
        }
    }
    classSegments.emplace_back(dimName, prevClazz, dimensions, timestamp, latestDataOffset, len-1, epsilon,maxval);
}

ClassSegment::ClassSegment(std::vector<std::string>& dim_name,
             int withClass,
             std::vector<std::vector<double>>& values,
             std::vector<double>& timestamps_vals,
             size_t begin, size_t end,
             const double epsilon,
             const double maxval) : withClass{withClass}, timestamps{timestamps_vals.begin()+begin,end-begin+1}, dimNames{dim_name.begin()+0, dim_name.size()} {
    for (size_t idx = 0, N = values.size(); idx<N; idx++) {
        std::span<double> SIDX{values[idx].begin()+begin, end-begin+1};
        dimensions.emplace_back(transform_series(dim_name[idx],
                                                 SIDX,
                                                 timestamps,
                                                 epsilon,
                                                 maxval));
    }

}