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
 * @file Configuration/Configuration.hpp
 *
 * @desc Program options are kept here.
 */

#ifndef MCT_CONFIGURATION_CONFIGURATION_HPP
#define MCT_CONFIGURATION_CONFIGURATION_HPP

#include <string>
#include <vector>
#include <cstdint>

#include <Configuration/Config.hpp>

namespace mct
{

class ConfigurationBuilder;

class MCT_CONFIGURATION_DLL_PUBLIC Configuration
{
    friend class ConfigurationBuilder;
public:
    Configuration(int argc, char** argv);
    ~Configuration();

    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    int get_app_argument_count() const { return m_argc; }
    const char** get_app_argument_array() const { return (const char**)m_argv; }

    const std::string& get_app_name() const { return m_app_name; }
    const std::string& get_config_filename() const { return m_config_filename; }
    const std::string& get_app_mode() const { return m_mode; }
    bool get_log_silent() const { return m_log_silent; }
    bool get_log_nofile() const { return m_log_nofile; }
    const std::string& get_log_directory() const { return m_log_directory; }
    const std::string& get_log_filename() const { return m_log_filename; }
    const std::string& get_log_format() const { return m_log_format; }
    const std::string& get_log_severity_console() const { return m_log_severity_console; }
    const std::string& get_log_severity_file() const { return m_log_severity_file; }
    bool get_log_rotate() const { return m_log_rotate; }
    uint64_t get_log_rotate_size() const { return m_log_rotate_size; }
    const std::string& get_log_rotate_filename() const { return m_log_rotate_filename; }
    uint64_t get_log_rotate_all_files_max_size() const { return m_log_rotate_all_files_max_size; }
    uint64_t get_log_rotate_min_free_space() const { return m_log_rotate_min_free_space; }

    // ModeProxy module
    const std::vector<uint16_t>& get_mode_proxy_local_ports() const { return m_mode_proxy_local_ports; }
    const std::vector<uint16_t>& get_mode_proxy_remote_ports() const { return m_mode_proxy_remote_ports; }
    const std::vector<std::string>& get_mode_proxy_local_hosts() const { return m_mode_proxy_local_hosts; }
    const std::vector<std::string>& get_mode_proxy_remote_hosts() const { return m_mode_proxy_remote_hosts; }

    void set_config_filename(const std::string& filename) { m_config_filename = filename; }
    void set_app_mode(const std::string& mode) { m_mode = mode; }
    void set_log_silent(const bool log_silent) { m_log_silent = log_silent; }
    void set_log_nofile(const bool log_nofile) { m_log_nofile = log_nofile; }
    void set_log_directory(const std::string& log_directory) { m_log_directory = log_directory; }
    void set_log_filename(const std::string& log_filename) { m_log_filename = log_filename; }
    void set_log_format(const std::string& log_format) { m_log_format = log_format; }
    void set_log_severity_console(const std::string& log_severity_console) { m_log_severity_console = log_severity_console; }
    void set_log_severity_file(const std::string& log_severity_file) { m_log_severity_file = log_severity_file; }
    void set_log_rotate(const bool log_rotate) { m_log_rotate = log_rotate; }
    void set_log_rotate_size(const uint64_t log_rotate_size) { m_log_rotate_size = log_rotate_size; }
    void set_log_rotate_filename(const std::string& log_rotate_filename) { m_log_rotate_filename = log_rotate_filename; }
    void set_log_rotate_all_files_max_size(const uint64_t log_rotate_all_files_max_size) { m_log_rotate_all_files_max_size = log_rotate_all_files_max_size; }
    void set_log_rotate_min_free_space(const uint64_t log_rotate_min_free_space) { m_log_rotate_min_free_space = log_rotate_min_free_space; }

    static const std::string default_config_filename;

protected:
    int m_argc;
    char** m_argv;

    std::string m_app_name;
    std::string m_config_filename;
    std::string m_mode;
    std::string m_log_directory;
    std::string m_log_filename;
    std::string m_log_format;
    std::string m_log_severity_console;
    std::string m_log_severity_file;
    std::string m_log_rotate_filename;
    
    bool m_log_silent;
    bool m_log_nofile;
    bool m_log_rotate;

    uint64_t m_log_rotate_size;
    uint64_t m_log_rotate_all_files_max_size;
    uint64_t m_log_rotate_min_free_space;

    // ModeProxy module
    std::vector<std::string> m_mode_proxy_local_hosts;
    std::vector<std::string> m_mode_proxy_remote_hosts;
    std::vector<uint16_t> m_mode_proxy_local_ports;
    std::vector<uint16_t> m_mode_proxy_remote_ports;
};

}

#endif // MCT_CONFIGURATION_CONFIGURATION_HPP
