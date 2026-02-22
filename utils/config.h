//
// Created by ngnamhy on 18/10/2025.
//

#ifndef MAXIMAL_CONFIG_H
#define MAXIMAL_CONFIG_H

#include <string>
#include <boost/log/trivial.hpp>

namespace utils {
    class Config {
    public:
        static std::string algorithm;
        static std::string dataset;
        static std::string input_path;
        static double minsup_percent;
        static double minwus;
        static boost::log::trivial::severity_level severity;
    public:

        static void load(const std::string& filename = "config.yml");
    };


} // utils

#endif //MAXIMAL_CONFIG_H