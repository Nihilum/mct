/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Mateusz Kolodziejski
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

    int get_app_argument_count() const { return m_argc; }
    const char** get_app_argument_array() const { return (const char**)m_argv; }

    const std::string get_app_name() const { return m_argv[0]; }
    const std::string& get_config_filename() const { return m_config_filename; }
    const std::string& get_app_mode() const { return m_mode; }

    void set_config_filename(const std::string& filename) { m_config_filename = filename; }
    void set_app_mode(const std::string& mode) { m_mode = mode; }

    static const std::string default_config_filename;

protected:
    int m_argc;
    char** m_argv;

    std::string m_config_filename;
    std::string m_mode;
};

}

#endif // MCT_CONFIGURATION_CONFIGURATION_HPP
