//
// Created by giacomo on 05/10/24.
//

#ifndef DTMINING_MULTIVARIATETIMESERIES_H
#define DTMINING_MULTIVARIATETIMESERIES_H

#include <vector>
#include <span>
#include <string>
#include <unordered_map>
#include <RecordHandling.h>


struct ClassSegment {
    int withClass;
    std::vector<std::tuple<RecordHandling,RecordHandling,RecordHandling,RecordHandling>> dimensions;
    std::span<double> timestamps;
    std::span<std::string> dimNames;

    ClassSegment(std::vector<std::string>& dim_name,
                 int withClass,
                 std::vector<std::vector<double>>& values,
                 std::vector<double>& timestamps_vals,
                 size_t begin, size_t end,
                 const double epsilon = 0.0001,
                 const double maxval = 1000000000.0);

    /**
     * Loading a map with the original raw data associated with the current interval
     */
    void retrieve_raw_data(size_t event_t, std::unordered_map<std::string,double>& payload) const {
        payload.clear();
        for (size_t i = 0, N = dimNames.size(); i<N; i++) {
            payload.emplace(dimNames[i], std::get<0>(dimensions.at(i)).orig[event_t]);
        }
    }

    /**
     * Retrieving the number of events that are stored in the interval
     * @return
     */
    size_t size() const {
        return std::get<0>(dimensions.at(0)).orig.size();
    }
};

#include <filesystem>
#include <parser.hpp>
#include <unordered_map>
#include "RecordHandling.h"


struct MultivariateTimeSeries {
    std::string envName;
    std::vector<std::string> dimName;
    std::vector<std::vector<double>> dimensions;
    std::vector<double> timestamp;
    std::vector<ClassSegment> classSegments;

    MultivariateTimeSeries() {};
    MultivariateTimeSeries(const MultivariateTimeSeries &) = default;
    MultivariateTimeSeries(MultivariateTimeSeries &&) = default;
    MultivariateTimeSeries &operator=(const MultivariateTimeSeries &) = default;
    MultivariateTimeSeries &operator=(MultivariateTimeSeries &&) = default;

//    inline
//    std::tuple<RecordHandling,RecordHandling,RecordHandling,RecordHandling>
//    serialize_segment_and_dimension(size_t segment_number, size_t dimension_number,
//                                    const double epsilon = 0.0001,
//                                    const double maxval = 1000000000.0) {
//        return transform_series(dimName.at(dimension_number),
//                         classSegments.at(segment_number).dimensions.at(dimension_number),
//                                timestamp,
//                         epsilon,
//                         maxval);
//    }

    /**
     * Loading one single environment from a single CSV file structured as follows:
     * all the columns are numerical, and the second last column is the class information (an integer)
     * and the last column is the timestamp date (either an integer or an ISO timestamp).
     * @param path  The CSV file containing the multivariate time series.
     * @return  The multivariate timeseries loaded from the CSV file
     */
    static inline MultivariateTimeSeries fromFile(const std::filesystem::path &path,
                                                  const double epsilon,
                                                  const double maxval) {
        auto env = path.stem().string();
        MultivariateTimeSeries result;
        {
            std::ifstream input_file(path);
            aria::csv::CsvParser parser = aria::csv::CsvParser(input_file);
            result = {env, parser, epsilon, maxval};
        }
        return result;
    }

    /**
     * Loading a folder of different timeseries environment, where each timeseries is a csv file
     * structured as described in the former method. This method will now cralw the children/descendant folders
     *
     * @param folder
     * @return      A map associating the environment name (filename without extension) to the environment itself
     */
    static inline std::unordered_map<std::string, MultivariateTimeSeries> fromDirectory(const std::filesystem::path& folder,
    const double epsilon,
    const double maxval,
    double& runtime) {
        using std::chrono::high_resolution_clock;
        using std::chrono::duration_cast;
        using std::chrono::duration;
        using std::chrono::milliseconds;

        auto t1 = high_resolution_clock::now();
        std::string ext(".csv");
        std::unordered_map<std::string, MultivariateTimeSeries> result;
        for (auto &p : std::filesystem::directory_iterator(folder))
        {
            if (p.path().extension() == ext) {
                auto f = fromFile(p, epsilon, maxval);
                result.emplace(f.envName, std::move(f));
            }
        }
        auto t2 = high_resolution_clock::now();
        /* Getting number of milliseconds as a double. */
        duration<double, std::milli> ms_double = t2 - t1;
        runtime = ms_double.count();

        return result;
    }

    MultivariateTimeSeries(const std::string& environment, aria::csv::CsvParser& parser,
    const double epsilon,
    const double maxval);
};


#endif //DTMINING_MULTIVARIATETIMESERIES_H
