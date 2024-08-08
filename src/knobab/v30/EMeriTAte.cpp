//
// Created by giacomo on 08/08/24.
//

#include <knobab/v30/EMeriTAte.h>

void  original_main_entrypoint(bool reclassify,
                               bool reduction,
                               double mining_supp,
                               bool isFilenamePolyadic,
                               const std::string &traceDistinguisher,
                               const std::string &filename_polyadic,
                               const std::unordered_set<std::string> &ignore_keys,
                               bool isFastSat,
                               const std::vector<log_data_format> &worlds_format_to_load,
                               const std::vector<std::string> &worlds_file_to_load,
                               std::filesystem::path &folder) {

    std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
    ServerQueryManager sqm;
    double cpp_preprocess, loading, indexing, mining, refining;
    if (!filename_polyadic.empty()) {
//        result.filename_polyadic = args::get(polyadicJSON);

        std::tie(cpp_preprocess, loading, indexing) = polyadic_loader(ignore_keys,
                                                                      traceDistinguisher,
                                                                      filename_polyadic,
                                                                      reclassify,
                                                                      sqm);
    } else {
        if ((worlds_file_to_load.size() == worlds_file_to_load.size()) && (!worlds_file_to_load.empty())) {
            cpp_preprocess = 0;
            indexing = 0;
            loading= 0;
            for (size_t i = 0, N = std::min(worlds_format_to_load.size(), worlds_file_to_load.size()); i<N; i++) {
                std::stringstream ss;
                std::string model_name = std::filesystem::path(worlds_file_to_load.at(i)).stem().generic_string();
                ss << "load "
                   << log_parse_format_type.at((size_t)worlds_format_to_load.at(i))
                   << " "
                   << std::quoted(worlds_file_to_load.at(i))
                   <<  " no stats as " // no stats with data as
                   << std::quoted(model_name);
                std::cout << ss.str() << std::endl;
                auto tmp = sqm.runQuery(ss.str());
                indexing += sqm.multiple_logs[model_name].experiment_logger.log_indexing_ms;
                loading += sqm.multiple_logs[model_name].experiment_logger.log_loading_and_parsing_ms;
            }
        } else {
            std::cerr << "ERROR: the non-polyadic dataset needs to be with the same dataset and label mapping" << std::endl;
            exit(1);
        }
    }

    std::cout << "C++ pre-processing: " << cpp_preprocess << " (ms)" << std::endl;
    std::cout << "Loading and parsing: " << loading << " (ms)" << std::endl;
    std::cout << "Indexing: " << indexing << " (ms)" << std::endl;


    if (isFastSat) {
        if (!exists(folder)) {
            std::cout << "ERROR, the current path does not exist: " << folder;
            exit(1);
        }
        if (!is_directory(folder)) {
            std::cout << "ERROR, the current path should be a directory: " << folder;
        }
        std::filesystem::path all_acts = folder / "acts.txt";
        if (!exists(all_acts)) {
            std::cout << "ERROR: the activity file does not exists: " << all_acts;
            exit(1);
        }

        // Loading all the activity files
        std::set<std::string> acts;
        {
            std::ifstream instream{all_acts};
            std::string line;
            while (std::getline(instream, line)) {
                acts.insert(line);
            }
        }
        std::unordered_map<std::string, std::vector<event_t>> exists_cl, absence_cl;
        {
            std::filesystem::path exists_f = folder / "exists.txt";
            if (exists(exists_f)) {
                std::ifstream instream{exists_f};
                std::string input;
                while (std::getline(instream, input)) {
                    std::string act;
                    size_t len, id;
                    std::istringstream istream(input);
                    istream >> len;
                    std::vector<event_t> elements;
                    for (size_t i = 0; i<len; i++) {
                        istream >> id;
                        elements.emplace_back(id);
                    }
                    // here we extract a copy of the "remainder"
                    std::string rem(istream.str().substr(istream.tellg()));
                    yaucl::strings::trim(rem);
                    exists_cl.emplace(rem, std::move(elements));
                }
            }
        }
        {
            std::filesystem::path exists_f = folder / "absences.txt";
            if (exists(exists_f)) {
                std::ifstream instream{exists_f};
                std::string input;
                while (std::getline(instream, input)) {
                    std::string act;
                    size_t len, id;
                    std::istringstream istream(input);
                    istream >> len;
                    std::vector<event_t> elements;
                    for (size_t i = 0; i<len; i++) {
                        istream >> id;
                        elements.emplace_back(id);
                    }
                    // here we extract a copy of the "remainder"
                    std::string rem(istream.str().substr(istream.tellg()));
                    yaucl::strings::trim(rem);
                    absence_cl.emplace(rem, std::move(elements));
                }
            }
        }

        for (auto& [log_name, kb] : sqm.multiple_logs) {
            std::filesystem::path out_path = folder / ("output_csv_"+log_name+".csv");
            std::ofstream  file{out_path};
            polyadic_bolt g;
            g.fast_check_and_collector_dataless(isFilenamePolyadic, &kb.db, acts, exists_cl, absence_cl, file);
        }
    } else {
        std::unordered_map<std::string, std::set<std::tuple<std::string,std::string,std::string>>> diff;
        std::tie(mining, refining) = polyadic_dataless_mining_and_refinement(mining_supp, isFilenamePolyadic, reduction, sqm, diff);
        std::cout << "Mining (min_support=" << mining_supp << ") : " << mining << " (ms)" << std::endl;
        std::cout << "Refining: " << refining << " (ms)" << std::endl;

        {
            std::filesystem::path benchmark_file{"benchmark_poly.csv"};
            bool writeHeader = false;
            if (!exists(benchmark_file)) {
                writeHeader = true;
            }
            std::ofstream file{benchmark_file, std::ios_base::app};
            if (writeHeader) {
                file << STRINGIFY(filename_polyadic,mining_supp,reduction,reclassify,isFilenamePolyadic,cpp_preprocess,loading,indexing,mining,refining) << std::endl;
            }
            file << BESTIA(filename_polyadic,mining_supp,reduction,reclassify,isFilenamePolyadic,cpp_preprocess,loading,indexing,mining,refining) << std::endl;
        }


        std::map<std::string, size_t> mined_model_size;
        // Serialization of the model
        for (const auto& [log_name, set] : diff) {
            mined_model_size[log_name] = set.size();
            std::string logName;
            {
                std::stringstream ss;
                ss << UNDERSCORED(filename_polyadic,mining_supp,reduction,isFilenamePolyadic,reclassify);
                ss << "_clazz=" << log_name <<".txt";
                logName = ss.str();
            }
            std::cout << logName << std::endl;
            std::ofstream file{logName};
            for (const auto& cl : set) {
                auto right = std::get<2>(cl);
                if (right.empty())
                    file << std::get<0>(cl) << "(" << std::get<1>(cl) << ")" << std::endl;
                else
                    file << std::get<0>(cl) << "(" << std::get<1>(cl) << "," << right << ")"<< std::endl;
            }
        }

        {
            std::filesystem::path benchmark_file{"benchmark_model_size.csv"};
            bool writeHeader = false;
            if (!exists(benchmark_file)) {
                writeHeader = true;
            }
            std::ofstream file{benchmark_file, std::ios_base::app};
            if (writeHeader) {
                file << STRINGIFY(filename_polyadic,mining_supp,reduction,reclassify,isFilenamePolyadic,cpp_preprocess,loading,indexing,mining,refining);
                for (const auto& [k,v] : mined_model_size) {
                    file << "," << k;
                }
//                return
                file << std::endl;
            }
//            result.values_polyadic(file, result.mined_model_size);
            file << BESTIA(filename_polyadic,mining_supp,reduction,reclassify,isFilenamePolyadic,cpp_preprocess,loading,indexing,mining,refining);
            for (const auto& [k,v] : mined_model_size) {
                file << "," << v;
            }
            file << std::endl;
        }
    }
}

void  python_main_entrypoint(bool reclassify,
                             bool reduction,
                             double mining_supp,
                             bool isFilenamePolyadic,
                             const std::string &traceDistinguisher,
                             const std::string &filename_polyadic,
                             const std::vector<std::string> &ignore_keys,
                             bool isFastSat,
                             const std::vector<std::string> &worlds_format_to_load,
                             const std::vector<std::string> &worlds_file_to_load,
                             const std::string &folder) {
    std::vector<log_data_format> orig_worlds_format_to_load;
    orig_worlds_format_to_load.reserve(worlds_format_to_load.size());
    for (const auto& x : worlds_format_to_load) {
        if (x == "TAB") {
            orig_worlds_format_to_load.emplace_back(TAB_SEPARATED_EVENTS);
        } else if (x == "XES") {
            orig_worlds_format_to_load.emplace_back(XES1);
        } else {
            orig_worlds_format_to_load.emplace_back(HUMAN_READABLE_YAUCL);
        }
    }
    std::filesystem::path orig_folder;
    if (!folder.empty())
        orig_folder = folder;
    original_main_entrypoint(reclassify,
                             reduction,
                             mining_supp,
                             isFilenamePolyadic,
                             traceDistinguisher,
                             filename_polyadic,
                             {ignore_keys.begin(), ignore_keys.end()},

                             isFastSat,
                             orig_worlds_format_to_load,
                             worlds_file_to_load,
                             orig_folder
                             );
}


#ifdef NO_PYBIND
#else
namespace py = pybind11;
PYBIND11_MODULE(knobab_emeritate_support, m) {
    m.doc() = "C++ support to the EMeriTAte algorithm"; // optional module docstring

    m.def("knobab_for_emeritate", &python_main_entrypoint, "Mimicking the C++ entrypoint from the original version of the code");
}
#endif