#ifndef LOGGER_H
#define LOGGER_H

#include <boost/log/trivial.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <string>

#define LOG(severity) BOOST_LOG_TRIVIAL(severity)

namespace logger {
    void init_logging();
}

#endif // LOGGER_H