#include "config.h"

#include <yaml-cpp/yaml.h>

#include "logger.h"
#include "iostream"

namespace utils {

void Config::load(const std::string& filename) {
    // LOG(info) << filename << '\n';
    YAML::Node config = YAML::LoadFile(filename);

    input_path     = config["input_path"].as<std::string>();
    dataset        = config["dataset"].as<std::string>();
    minsup_percent = config["threshold"].as<double>();
    algorithm      = config["algorithm"].as<std::string>();

    std::string sev = config["severity"] ? 
                      config["severity"].as<std::string>() : "info";


    if (sev == "debug")      severity = boost::log::trivial::debug;
    else if (sev == "info") severity = boost::log::trivial::info;
    else if (sev == "warning") severity = boost::log::trivial::warning;
    else if (sev == "error")   severity = boost::log::trivial::error;
    else severity = boost::log::trivial::info;
}

} // namespace utils
