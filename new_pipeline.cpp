
#include <string>
#include <unordered_set>

#include <knobab/mining/polyadic/polyadic_loading.h>
#include <knobab/mining/polyadic/polyadic_mining.h>

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
};

#include <args.hxx>

int main(int argc, char **argv) {
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
    args::Flag rec(group, "reclassify", "Run a re-classification, thus further distiguishing each class via decision-tree induced sub-classes", {'s', "subclass"});
    args::ValueFlagList<std::string> characters(parser, "ignore keys", "The payload's keys to be ignored within the loading and classification task", {'i', "ignore"});
    args::ValueFlag<std::string> polyadicJSON(parser, "polyadic JSON", "The polyadic traces represented as a json file", {'p', "polyadic"});

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
        result.isFilenamePolyadic = true;
        std::tie(result.cpp_preprocess, result.loading, result.indexing) = polyadic_loader(ignore_keys,
                                                                                           traceDistinguisher,
                                                                                           result.filename_polyadic,
                                                                                           result.reclassify,
                                                                                           sqm);
    } else {
        result.isFilenamePolyadic = false;
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

    std::unordered_map<std::string, std::set<std::tuple<std::string,std::string,std::string>>> diff;
    std::tie(result.mining, result.refining) = polyadic_dataless_mining_and_refinement(result.mining_supp, result.isFilenamePolyadic, result.reduction, sqm, diff);
    std::cout << "Mining (min_support=" << result.mining_supp << ") : " << result.mining << " (ms)" << std::endl;
    std::cout << "Refining: " << result.refining << " (ms)" << std::endl;

    // Serialization of the model
    for (const auto& [log_name, set] : diff) {
        result.mined_model_size[log_name] = set.size();
        std::ofstream file{log_name+"_clazz.txt"};
        for (const auto& cl : set) {
            auto right = std::get<2>(cl);
            if (right.empty())
                file << std::get<0>(cl) << "(" << std::get<1>(cl) << ")" << std::endl;
            else
                 file << std::get<0>(cl) << "(" << std::get<1>(cl) << "," << right << ")"<< std::endl;
        }
    }

    return 0;
}

