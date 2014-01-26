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
 * @file Configuration/ConfigurationBuilder.hpp
 *
 * @desc Class used to parse the configuration file and construct Configuration class.
 */

#ifndef MCT_CONFIGURATION_CONFIGURATION_BUILDER_HPP
#define MCT_CONFIGURATION_CONFIGURATION_BUILDER_HPP

#include <string>
#include <cstdint>

#include <Configuration/Config.hpp>

namespace boost
{

namespace program_options
{
    class options_description;
    class positional_options_description;
    class variables_map;
}

}

namespace po = boost::program_options;

namespace mct
{

class Configuration;

class MCT_CONFIGURATION_DLL_PUBLIC ConfigurationBuilder
{
public:
    ConfigurationBuilder(Configuration& config);

    /**
     * Builds configuration and stores the result into m_config.
     *
     * return:
     * - true on success
     * - false on failure; msg stores an error message which can be displayed
     */
    bool build_configuration(std::string& msg);

    Configuration& get_configuration() { return m_config; }

protected:
    bool setup_config_fields(
            std::string& msg,
            po::options_description& po_cmdline, po::options_description& po_config, po::options_description& po_hidden,
            po::options_description& cmdline_options, po::options_description& config_file_options,
            po::options_description& visible_options, po::positional_options_description& positional_options, po::variables_map& vm
        ) const;

    bool load_cmdline(
            std::string& msg,
            po::options_description& cmdline_options, po::positional_options_description& positional_options, po::variables_map& vm
        ) const;

    bool handle_configuration_info_special_mode(
            std::string& msg,
            po::options_description& po_cmdline, po::options_description& po_config, po::variables_map& vm
        ) const;

    bool load_cfgfile(
            std::string& msg,
            po::options_description& config_file_options, po::variables_map& vm
        ) const;

    void build_help_message(std::string& msg, po::options_description& po_cmdline) const;
    void build_version_message(std::string& msg) const;
    void build_generate_message(std::string& msg, po::options_description& po_config) const;
    bool build_show_options_message(
            std::string& msg,
            po::variables_map& vm, po::options_description& po_hidden, po::options_description& po_cmdline, po::options_description& po_config
        ) const;

protected:
    Configuration& m_config;
};

}

#endif // MCT_CONFIGURATION_CONFIGURATION_BUILDER_HPP
