#include "logger.h"
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time.hpp> // Added for ptime
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/text_file_backend.hpp>

#include <boost/log/utility/setup/file.hpp>

#include "config.h"

namespace logging = boost::log;


void logger::init_logging() {
    // Add common attributes (timestamp, severity)
    logging::add_common_attributes();

    // Log format for console
    // Define log format similar to Log4j
    auto fmt = logging::expressions::stream
               << logging::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
               << " - "
               << logging::expressions::attr<logging::trivial::severity_level>("Severity")
               << " - " << logging::expressions::smessage;


    // Sink: Output all logs to console
    // logging::add_console_log(
    //     std::cout,
    //     logging::keywords::format = fmt
    // );
    logging::add_file_log(
        logging::keywords::file_name = "app.log",   // tên file log
        logging::keywords::open_mode = std::ios::app, // append thay vì overwrite
        logging::keywords::format = fmt
    );

    // Set minimum log level
    logging::core::get()->set_filter(
        logging::trivial::severity >= utils::Config::severity
    );
}
