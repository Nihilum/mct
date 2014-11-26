/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2013-2014 Mateusz Kolodziejski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file Logger/Logger.hpp
 *
 * @desc Class handling console and file output.
 */

#include <iostream>
#include <stdexcept>

#include <boost/shared_ptr.hpp>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/core/null_deleter.hpp>

#include <moccpp/System/cstdio.hpp>

#include <Logger/Logger.hpp>
#include <Configuration/Configuration.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

namespace mct
{

enum severity_level
{
    debug,
    info,
    notice, // special type, used by log_if_not_silent()
    warning,
    error,
    fatal
};

// The formatting logic for the severity level
template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
{
    static const char* const str[] =
    {
        "D",
        "I",
        "N", // special type, used by log_if_not_silent() 
        "W",
        "E",
        "F"
    };
    if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
        strm << str[lvl];
    else
        strm << static_cast< int >(lvl);
    return strm;
}

class LoggerImpl
{
public:
    LoggerImpl(Configuration& config);
    ~LoggerImpl();

    LoggerImpl(const LoggerImpl&) = delete;
    LoggerImpl& operator=(const LoggerImpl&) = delete;

    bool initialize(std::string& msg);
    void print_helper(va_list& args, const char* format, severity_level level);

protected:
    Configuration& m_config;
    bool m_is_initialized;
    src::severity_logger< severity_level > m_log;
};

Logger::Logger(Configuration& config) : m_pImpl(nullptr)
{
    m_pImpl = new LoggerImpl(config);
}

Logger::~Logger()
{
    delete m_pImpl;
}

bool Logger::initialize(std::string& msg)
{
    return m_pImpl->initialize(msg);
}

void Logger::debug(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    m_pImpl->print_helper(args, format, mct::debug);
    va_end(args);
}

void Logger::info(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    m_pImpl->print_helper(args, format, mct::info);
    va_end(args);
}

void Logger::warning(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    m_pImpl->print_helper(args, format, mct::warning);
    va_end(args);
}

void Logger::error(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    m_pImpl->print_helper(args, format, mct::error);
    va_end(args);
}

void Logger::fatal(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    m_pImpl->print_helper(args, format, mct::fatal);
    va_end(args);
}

void Logger::log_if_not_silent(const char* format, ...)
{
    BOOST_LOG_SCOPED_THREAD_TAG("Not_Silent", "IMPORTANT");
    va_list args;
    va_start(args, format);
    m_pImpl->print_helper(args, format, mct::notice);
    va_end(args);
}

template <typename T>
void formatting_setup(boost::shared_ptr<T>& pSink, const std::string& log_format, const std::string& severity)
{
    mct::severity_level sev_lev = mct::notice;
    if (severity == std::string("debug")) {
        sev_lev = mct::debug;
    } else if (severity == std::string("info")) {
        sev_lev = mct::info;
    } else if (severity == std::string("warning")) {
        sev_lev = mct::warning;
    } else if (severity == std::string("error")) {
        sev_lev = mct::error;
    } else if (severity == std::string("fatal")) {
        sev_lev = mct::fatal;
    } else {
        throw std::runtime_error(std::string("Invalid log severity: '") + severity + std::string("'"));
    }

    pSink->set_formatter(expr::stream
        << "["  << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", log_format.c_str())
        << "][" << expr::attr< severity_level >("Severity")
        << "] "  << expr::if_(expr::has_attr("Not_Silent"))
            [
                expr::stream << ""
            ]
        << expr::smessage);

    pSink->set_filter(
        expr::attr< severity_level >("Severity").or_default(mct::notice) >= sev_lev
        || expr::begins_with(expr::attr< std::string >("Not_Silent").or_default(std::string()), "IMPORTANT"));
}

LoggerImpl::LoggerImpl(Configuration& config)
: m_config(config), m_is_initialized(false)
{
}

LoggerImpl::~LoggerImpl()
{
}

bool LoggerImpl::initialize(std::string& msg)
{
    if (m_is_initialized) {
        msg = "Logger has already been initialized";
        return false;
    }

    m_is_initialized = true;

    try {
    	if (!m_config.get_log_nofile()) {
            if (m_config.get_log_rotate()) {
                typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;
                boost::shared_ptr< file_sink > rotate_file_sink(new file_sink(
                    keywords::file_name = std::string(m_config.get_log_directory() + std::string("/") + m_config.get_log_rotate_filename()),  // file name pattern
                    keywords::rotation_size = m_config.get_log_rotate_size()   // rotation size, in characters
                    ));

                rotate_file_sink->locked_backend()->set_file_collector(sinks::file::make_collector(
                    keywords::target = m_config.get_log_directory(),                    // where to store rotated files
                    keywords::max_size = m_config.get_log_rotate_all_files_max_size(),  // maximum total size of the stored files, in bytes
                    keywords::min_free_space = m_config.get_log_rotate_min_free_space() // minimum free space on the drive, in bytes
                    ));

                // Upon restart, scan the target directory for files matching the file_name pattern
                rotate_file_sink->locked_backend()->scan_for_files();

                formatting_setup(rotate_file_sink, m_config.get_log_format(), m_config.get_log_severity_file());

                logging::core::get()->add_sink(rotate_file_sink);
            } else {
                typedef sinks::synchronous_sink< sinks::text_multifile_backend > file_sink;
                boost::shared_ptr< file_sink > one_file_sink(new file_sink);

                one_file_sink->locked_backend()->set_file_name_composer(sinks::file::as_file_name_composer(
                    expr::stream << m_config.get_log_directory() << "/" << m_config.get_log_filename()));

                formatting_setup(one_file_sink, m_config.get_log_format(), m_config.get_log_severity_file());

                logging::core::get()->add_sink(one_file_sink);
            }
    	}

    	typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    	boost::shared_ptr< text_sink > console_sink(new text_sink);
    	boost::shared_ptr< std::ostream > log_stream(&std::clog, boost::null_deleter());
    	console_sink->locked_backend()->add_stream(log_stream);

    	formatting_setup(console_sink, m_config.get_log_format(), m_config.get_log_severity_console());

    	logging::core::get()->add_sink(console_sink);
    	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
    } catch (const std::exception& e) {
    	msg = e.what();
    	return false;
    }

    return true;
}

void LoggerImpl::print_helper(va_list& args, const char* format, severity_level level)
{
    if (m_config.get_log_silent()) {
        return;
    }

    const uint16_t max_standard_length = 256;
    char buffer[max_standard_length];
    char* pBuffer = buffer;

    // try with a small buffer first (which should be very quick)
    int32_t ret = moccpp::System::vsnprintf(pBuffer, max_standard_length, max_standard_length - 1, format, args);

    if (ret < 0) { // encoding error
        BOOST_LOG_SEV(m_log, level) << format << "[LoggerImpl::print_helper] encoding error!";
        return;
    }

    if (ret < max_standard_length) { // returned value is non-negative and less than max_standard_length, the string has been completely written.
        BOOST_LOG_SEV(m_log, level) << pBuffer;
        return;
    }

    // small buffer has failed, because the data is too large - alloc a new buffer with suggested size (ret)
    std::shared_ptr<char> buff_to_write(new char[ret + 1], std::default_delete<char[]>());
    pBuffer = &(*buff_to_write);
    pBuffer[ret] = '\0'; // NOTE: this is actually ok, we are not exceeding here, since we've done new char[ret + 1], so we have that 1 extra character

    int32_t final_ret = moccpp::System::vsnprintf(pBuffer, ret + 1, ret, format, args);

    if (final_ret < 0) { // encoding error
        BOOST_LOG_SEV(m_log, level) << format << "[LoggerImpl::print_helper] extended encoding error!";
        return;
    }

    if (final_ret < (ret + 1)) { // returned value is non-negative and less than ret + 1, the string has been completely written.
        BOOST_LOG_SEV(m_log, level) << pBuffer;
        return;
    }

    BOOST_LOG_SEV(m_log, level) << format << "[LoggerImpl::print_helper] write error!";
}

}
