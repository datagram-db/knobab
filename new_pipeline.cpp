
#include <string>
#include <unordered_set>

#include <knobab/mining/polyadic/polyadic_loading.h>
#include <knobab/mining/polyadic/polyadic_mining.h>

#define COUNT_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
#define COUNT(...)   COUNT_N(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
// Warning: COUNT() return 1 (as COUNT(A)) :-/

#define IDENTITY(N) N
#define APPLY(macro, ...) IDENTITY(macro(__VA_ARGS__))

#define STRINGIFY_1(a) #a
#define STRINGIFY_2(a, b) #a "," #b
#define STRINGIFY_3(a, b, c) #a "," #b "," #c
#define STRINGIFY_4(a, b, c, d) #a "," #b "," #c "," #d
#define STRINGIFY_5(a, b, c, d, e) #a "," #b "," #c "," #d "," #e
#define STRINGIFY_6(a, b, c, d, e, f) #a "," #b "," #c "," #d "," #e "," #f
#define STRINGIFY_7(a, b, c, d, e, f, g) #a "," #b "," #c "," #d "," #e "," #f "," #g
#define STRINGIFY_8(a, b, c, d, e, f, g, h) #a "," #b "," #c "," #d "," #e "," #f "," #g "," #h
#define STRINGIFY_9(a, b, c, d, e, f, g, h, i) #a "," #b "," #c "," #d "," #e "," #f "," #g "," #h "," #i
#define STRINGIFY_10(a, b, c, d, e, f, g, h, i, j) #a "," #b "," #c "," #d "," #e "," #f "," #g "," #h "," #i "," #j
#define BESTIA_1(a) a
#define BESTIA_2(a, b) a << "," <<  b
#define BESTIA_3(a, b, c) a << "," <<  b << "," <<  c
#define BESTIA_4(a, b, c, d) a << "," <<  b << "," <<  c << "," <<  d
#define BESTIA_5(a, b, c, d, e) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e
#define BESTIA_6(a, b, c, d, e, f) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f
#define BESTIA_7(a, b, c, d, e, f, g) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f << "," <<  g
#define BESTIA_8(a, b, c, d, e, f, g, h) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f << "," <<  g << "," <<  h
#define BESTIA_9(a, b, c, d, e, f, g, h, i) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f << "," <<  g << "," <<  h << "," <<  i
#define BESTIA_10(a, b, c, d, e, f, g, h, i, j) a << "," <<  b << "," <<  c << "," <<  d << "," <<  e << "," <<  f << "," <<  g << "," <<  h << "," <<  i << "," <<  j
#define US_1(a) a
#define US_2(a, b) a << "_" <<  b
#define US_3(a, b, c) a << "_" <<  b << "_" <<  c
#define US_4(a, b, c, d) a << "_" <<  b << "_" <<  c << "_" <<  d
#define US_5(a, b, c, d, e) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e
#define US_6(a, b, c, d, e, f) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f
#define US_7(a, b, c, d, e, f, g) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f << "_" <<  g
#define US_8(a, b, c, d, e, f, g, h) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f << "_" <<  g << "_" <<  h
#define US_9(a, b, c, d, e, f, g, h, i) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f << "_" <<  g << "_" <<  h << "_" <<  i
#define US_10(a, b, c, d, e, f, g, h, i, j) a << "_" <<  b << "_" <<  c << "_" <<  d << "_" <<  e << "_" <<  f << "_" <<  g << "_" <<  h << "_" <<  i << "_" <<  j

#define DISPATCH_S(N) STRINGIFY_ ## N
#define DISPATCH_B(N) BESTIA_ ## N
#define DISPATCH_U(N) US_ ## N

#define STRINGIFY(...) IDENTITY(APPLY(DISPATCH_S, COUNT(__VA_ARGS__)))(__VA_ARGS__)
#define BESTIA(...) IDENTITY(APPLY(DISPATCH_B, COUNT(__VA_ARGS__)))(__VA_ARGS__)
#define UNDERSCORED(...) IDENTITY(APPLY(DISPATCH_U, COUNT(__VA_ARGS__)))(__VA_ARGS__)

struct benchmarking {
    // parameters
    std::string filename_polyadic;
    double mining_supp;
    bool reduction, reclassify;
    bool isFilenamePolyadic;
    std::unordered_map<std::string, log_data_format> nonPolyadicDataset;


    // Outcomes
    double cpp_preprocess, loading, indexing, mining, refining;
    std::map<std::string, size_t> mined_model_size;

    std::string get_log_name(const std::string& name) const {
        std::stringstream ss;
        ss << UNDERSCORED(filename_polyadic,mining_supp,reduction,isFilenamePolyadic,reclassify);
        ss << "_clazz=" << name <<".txt";
        auto s = ss.str();
        return s;
    }

    std::ostream& header_csv(std::ostream& os) const {
        return os << STRINGIFY(filename_polyadic,mining_supp,reduction,reclassify,isFilenamePolyadic,cpp_preprocess,loading,indexing,mining,refining) << std::endl;
    }
    std::ostream& values_csv(std::ostream& os) const {
        return os << BESTIA(filename_polyadic,mining_supp,reduction,reclassify,isFilenamePolyadic,cpp_preprocess,loading,indexing,mining,refining) << std::endl;
    }
    template <typename MAP>
    std::ostream& header_polyadic(std::ostream& os, const MAP& field) const {
        os << STRINGIFY(filename_polyadic,mining_supp,reduction,reclassify,isFilenamePolyadic,cpp_preprocess,loading,indexing,mining,refining);
        for (const auto& [k,v] : field) {
            os << "," << k;
        }
        return os << std::endl;
    }

    template <typename MAP>
    std::ostream& values_polyadic(std::ostream& os, const MAP& field) const {
        os << BESTIA(filename_polyadic,mining_supp,reduction,reclassify,isFilenamePolyadic,cpp_preprocess,loading,indexing,mining,refining);
        for (const auto& [k,v] : field) {
            os << "," << v;
        }
        return os << std::endl;
    }
};

#include <args.hxx>
#include <filesystem>
#include <yaucl/strings/string_utils.h>

int main(int argc, char **argv) {

    // CyberSecurity configuration:
    // -s 0.8 --nonPoly=tab --nonPoly=tab --nonPoly=tab --nonPoly=tab --nonPoly=tab --nonPoly=tab --nonPoly=tab --nonPoly=tab /home/giacomo/Scaricati/classes/Adware.tab_100.tab /home/giacomo/Scaricati/classes/Backdoor.tab_100.tab /home/giacomo/Scaricati/classes/Downloader.tab_100.tab /home/giacomo/Scaricati/classes/Dropper.tab_100.tab /home/giacomo/Scaricati/classes/Spyware.tab_100.tab /home/giacomo/Scaricati/classes/Trojan.tab_100.tab /home/giacomo/Scaricati/classes/Virus.tab_100.tab /home/giacomo/Scaricati/classes/Worms.tab_100.tab

    // Polyadic mining configuration
    // -s 1.0 -d user -i day -i span -i "__class" -i "__label" -i time -i fulltime -p /home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json

    struct benchmarking result;
    result.filename_polyadic = "/home/giacomo/projects/sdd-processing/sdd-processing/log_weekly.json";
    std::string traceDistinguisher = "user";
    std::unordered_set<std::string> ignore_keys{"day","span","__class","__label","time","fulltime"};
    std::vector<std::string> log_parse_format_type{"HRF", "XES", "TAB"};
    result.mining_supp = 1.0;
    result.reduction = false;
    result.reclassify = false;


    args::ArgumentParser parser("Polyadic Mining", "This is the main entry point for the benchmarking ");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Group group(parser, "You can use the following parameters", args::Group::Validators::DontCare, args::Options::Global);
    args::ValueFlag<double>  supportVal(group, "Support Value", "If present, specifies the support value (default 1.0)", {'s', "support"});
    args::ValueFlag<std::string>  distinguisher(group, "Trace Distinguisher", "Trace payload field allowing to distinguish different users within polyadic traces", {'d', "distinguisher"});
    args::Flag red(group, "reduction", "Run the model reduction for removing mutually implying clauses", {'r', "red"});
    args::Flag rec(group, "reclassify", "Run a re-classification, thus further distiguishing each class via decision-tree induced sub-classes", {'k', "subclass"});
    args::ValueFlag<std::string> fastSat(group, "reclassify", "Performs a fast SAT given specific configuration files within a specific folder", {'f', "fastSat"});
    args::ValueFlagList<std::string> characters(parser, "ignore keys", "The payload's keys to be ignored within the loading and classification task", {'i', "ignore"});
    args::ValueFlag<std::string> polyadicJSON(parser, "polyadic JSON", "The polyadic traces represented as a json file", {'p', "polyadic"});
    args::Flag polyadicMine(parser, "usual mining", "Uses the standard linear behaviour from declarative mining, where traces are not grouped in hierarchy", {'l', "nonPolyMining"});

    std::unordered_map<std::string, log_data_format> map{
            {"hrf", log_data_format::HUMAN_READABLE_YAUCL},
            {"xes", log_data_format::XES1},
            {"tab", log_data_format::TAB_SEPARATED_EVENTS}};
    args::MapFlagList<std::string, log_data_format> use_confidence_for_clustering(parser, "nonPolyadic", "For non polyadic datasets, define the format to be associated to each file", {'n', "nonPoly"}, map);
    args::PositionalList<std::string> files(parser, "files", "non polyadic Files associated to the specific worlds");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    if (supportVal) {
        result.mining_supp = args::get(supportVal);
    }
    if (distinguisher) {
        traceDistinguisher = args::get(distinguisher);
    }
    if (red) {
        result.reduction = true;
    }
    if (rec) {
        result.reclassify = true;
    }
    if (characters) {
        ignore_keys.clear();
        for (const std::string& key : args::get(characters)) {
            ignore_keys.insert(key);
        }
    }
    std::vector<log_data_format> worlds_format_to_load = args::get(use_confidence_for_clustering);
    std::vector<std::string>     worlds_file_to_load = args::get(files);

    ServerQueryManager sqm;
    if (polyadicJSON) {
        if (polyadicMine) {
            result.isFilenamePolyadic = false;
        } else {
            result.isFilenamePolyadic = true;
        }
        std::tie(result.cpp_preprocess, result.loading, result.indexing) = polyadic_loader(ignore_keys,
                                                                                           traceDistinguisher,
                                                                                           result.filename_polyadic,
                                                                                           result.reclassify,
                                                                                           sqm);
    } else {
        if ((worlds_file_to_load.size() == worlds_file_to_load.size()) && (!worlds_file_to_load.empty())) {
            result.cpp_preprocess = 0;
            result.indexing = 0;
            result.loading= 0;
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
                result.indexing += sqm.multiple_logs[model_name].experiment_logger.log_indexing_ms;
                result.loading += sqm.multiple_logs[model_name].experiment_logger.log_loading_and_parsing_ms;
            }
        } else {
            std::cerr << "ERROR: the non-polyadic dataset needs to be with the same dataset and label mapping" << std::endl;
            exit(1);
        }
    }

    std::cout << "C++ pre-processing: " << result.cpp_preprocess << " (ms)" << std::endl;
    std::cout << "Loading and parsing: " << result.loading << " (ms)" << std::endl;
    std::cout << "Indexing: " << result.indexing << " (ms)" << std::endl;

    if (fastSat) {
        std::filesystem::path folder = args::get(fastSat);
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
            g.fast_check_and_collector_dataless(result.isFilenamePolyadic, &kb.db, acts, exists_cl, absence_cl, file);
        }
    } else {
        std::unordered_map<std::string, std::set<std::tuple<std::string,std::string,std::string>>> diff;
        std::tie(result.mining, result.refining) = polyadic_dataless_mining_and_refinement(result.mining_supp, result.isFilenamePolyadic, result.reduction, sqm, diff);
        std::cout << "Mining (min_support=" << result.mining_supp << ") : " << result.mining << " (ms)" << std::endl;
        std::cout << "Refining: " << result.refining << " (ms)" << std::endl;

        {
            std::filesystem::path benchmark_file{"benchmark_poly.csv"};
            bool writeHeader = false;
            if (!exists(benchmark_file)) {
                writeHeader = true;
            }
            std::ofstream file{benchmark_file, std::ios_base::app};
            if (writeHeader)
                result.header_csv(file);
            result.values_csv(file);
        }


        // Serialization of the model
        for (const auto& [log_name, set] : diff) {
            result.mined_model_size[log_name] = set.size();

            std::ofstream file{result.get_log_name(log_name)};
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
            if (writeHeader)
                result.header_polyadic(file, result.mined_model_size);
            result.values_polyadic(file, result.mined_model_size);
        }
    }


    return 0;
}

