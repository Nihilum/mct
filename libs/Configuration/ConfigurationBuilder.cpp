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
 * @file Configuration/ConfigurationBuilder.cpp
 *
 * @desc Class used to parse the configuration file and construct Configuration class.
 */

#include <fstream>
#include <sstream>
#include <algorithm>

#include <boost/program_options.hpp>

#include <Configuration/Configuration.hpp>
#include <Configuration/ConfigurationBuilder.hpp>

#include <config.hpp>

namespace mct
{

ConfigurationBuilder::ConfigurationBuilder(Configuration& config)
 : m_config(config)
{
}

bool ConfigurationBuilder::build_configuration(std::string& msg)
{
    // Options allowed only with the command line
    po::options_description po_cmdline("Command Line options");
    // Options allowed with the command line and the config file
    po::options_description po_config("Configuration");
    // Hidden options allowed with the cmd line and the cfg file
    po::options_description po_hidden("Hidden options");

    // Below are different option types assembled from other types
    po::options_description cmdline_options;
    po::options_description config_file_options;
    po::options_description visible_options("Allowed options");
    po::positional_options_description positional_options;

    po::variables_map vm;

    if (setup_config_fields(msg, po_cmdline, po_config, po_hidden, cmdline_options, config_file_options,
        visible_options, positional_options, vm) == false) {
        return false;
    }

    if (load_cmdline(msg, cmdline_options, positional_options, vm) == false) {
        return false;
    }

    // If this method returns true, then the program is running in special info mode; change mode and return true
    if (handle_configuration_info_special_mode(msg, po_cmdline, po_config, vm) == true) {
        m_config.set_app_mode("configuration_info_special_mode");

        if (vm.count("show_options") == 1) {
            std::string tmp_msg = msg;
            msg.clear();
            if (build_show_options_message(msg, vm, po_hidden, po_cmdline, po_config) == false) {
                return false;
            }
            msg += tmp_msg;
        }

        return true;
    }

    if (load_cfgfile(msg, config_file_options, vm) == false) {
        return false;
    }

    if (vm.count("show_options") == 1) {
        if (build_show_options_message(msg, vm, po_hidden, po_cmdline, po_config) == false) {
            return false;
        }
    }

    // Version will be displayed upon program start-up
    build_version_message(msg);

    return true;
}

bool ConfigurationBuilder::setup_config_fields(
            std::string& msg,
            po::options_description& po_cmdline, po::options_description& po_config, po::options_description& po_hidden,
            po::options_description& cmdline_options, po::options_description& config_file_options,
            po::options_description& visible_options, po::positional_options_description& positional_options, po::variables_map& vm
) const
{
    try {
        // Options allowed only with the command line
        po_cmdline.add_options()
            ("version,v", "print version string")
            ("help,h", "show help message")
            ("generate,g", "generate config file")
            ("config,c", po::value<std::string>(&m_config.m_config_filename)->default_value(Configuration::default_config_filename),
                  "configuration filename")
            ;

        // Options allowed with the command line and the config file
        po_config.add_options()
            ("mode", po::value<std::string>(&m_config.m_mode)->default_value("proxy"),
                  "specifies the way the application is going to operate\n"
                  "possible modes: proxy")
            ("log.silent", po::value<bool>(&m_config.m_log_silent)->default_value(false),
                  "should logger be completely silent")
            ("log.nofile", po::value<bool>(&m_config.m_log_nofile)->default_value(false),
                  "tells logger if any log files or directories can be created")
            ("log.directory", po::value<std::string>(&m_config.m_log_directory)->default_value("logs"),
                  "directory name where the log files will be stored")
            ("log.filename", po::value<std::string>(&m_config.m_log_filename)->default_value("mct.log"),
                  "log filename, used only if log.rotate is set to 0")
            ("log.format", po::value<std::string>(&m_config.m_log_format)->default_value("%H:%M:%S.%f"),
                  "date_time formatting for logger, the following variables may be used:\n"
                  "www.boost.org/doc/libs/1_55_0/doc/html/date_time/date_time_io.html")
            ("log.severity.console", po::value<std::string>(&m_config.m_log_severity_console)->default_value("info"),
                  "determines which log messages will be shown in the console,\n"
                  "the following may be used:\n"
                  "debug, info, warning, error, fatal")
            ("log.severity.file", po::value<std::string>(&m_config.m_log_severity_file)->default_value("info"),
                  "determines which log messages will be shown in the file,\n"
                  "the following may be used:\n"
                  "debug, info, warning, error, fatal")
            ("log.rotate", po::value<bool>(&m_config.m_log_rotate)->default_value(false),
                  "should logger use rotating log files")
            ("log.rotate.size", po::value<uint64_t>(&m_config.m_log_rotate_size)->default_value(1048576),
                  "maximum size (in characters) of the rotating log file")
            ("log.rotate.filename", po::value<std::string>(&m_config.m_log_rotate_filename)->default_value("%Y%m%d_%H%M%S_%5N-mct.log"),
                  "filename pattern of the rotating log file,\n"
                  "the following variables may be used:\n"
                  "%Y -- year, %m -- month, %d -- day, %H -- hour, %M -- minute, %S -- second,\n"
                  "%nN -- log's ordinal number (n - number of digits)")
            ("log.rotate.all_files_max_size", po::value<uint64_t>(&m_config.m_log_rotate_all_files_max_size)->default_value(1073741824),
                  "maximum size (in bytes) of all the rotating log files combined")
            ("log.rotate.min_free_space", po::value<uint64_t>(&m_config.m_log_rotate_min_free_space)->default_value(1073741824),
                  "minimum free disk space (in bytes) to run rotating log files")
            ("mode.proxy.local_port", po::value<uint16_t>(&m_config.m_mode_proxy_local_port)->default_value(0),
                  "local port to bind to in proxy mode")
            ("mode.proxy.remote_port", po::value<uint16_t>(&m_config.m_mode_proxy_remote_port)->default_value(0),
                  "remote port to send to in proxy mode")
            ("mode.proxy.local_host", po::value<std::string>(&m_config.m_mode_proxy_local_host)->default_value("localhost"),
                  "local interface to bind to in proxy mode")
            ("mode.proxy.remote_host", po::value<std::string>(&m_config.m_mode_proxy_remote_host)->default_value("localhost"),
                  "remote host to send to in proxy mode")
            ;

        // Hidden options allowed with the command line and the config file
        po_hidden.add_options()
            ("show_options", "shows program options composed from cmdline and cfgfile")
            ;

        cmdline_options.add(po_cmdline).add(po_config).add(po_hidden);
        config_file_options.add(po_config).add(po_hidden);
        visible_options.add(po_cmdline).add(po_config);
        positional_options.add("show_options", -1);
    } catch (const std::exception& e) {
        msg = e.what();
        return false;
    }

    return true;
}

bool ConfigurationBuilder::load_cmdline(
            std::string& msg,
            po::options_description& cmdline_options, po::positional_options_description& positional_options, po::variables_map& vm
) const
{
    try {
        store(po::command_line_parser(m_config.get_app_argument_count(), m_config.get_app_argument_array()).
                options(cmdline_options).positional(positional_options).run(), vm);
        notify(vm);
    } catch (const std::exception& e) {
        msg = e.what();
        return false;
    }

    return true;
}

bool ConfigurationBuilder::handle_configuration_info_special_mode(
        std::string& msg,
        po::options_description& po_cmdline, po::options_description& po_config, po::variables_map& vm
) const
{
    if (vm.count("help") == 1) {
        build_help_message(msg, po_cmdline);
        return true;
    } else if (vm.count("version") == 1) {
        build_version_message(msg);
        return true;
    } else if (vm.count("generate") == 1) {
        build_generate_message(msg, po_config);
        return true;
    }

    return false;
}

bool ConfigurationBuilder::load_cfgfile(
        std::string& msg,
        po::options_description& config_file_options, po::variables_map& vm
) const
{
    std::fstream fs(m_config.get_config_filename().c_str());
    if (!fs) {
        msg = std::string("Could not open '") + m_config.get_config_filename() + std::string("' file");
        return false;
    }

    try {
        store(parse_config_file(fs, config_file_options), vm);
        notify(vm);
    } catch (const std::exception& e) {
        msg = e.what();
        return false;
    }

    return true;
}

void ConfigurationBuilder::build_help_message(std::string& msg, po::options_description& po_cmdline) const
{
    std::stringstream help_sstr;
    std::string version_string;
    build_version_message(version_string);

    help_sstr << version_string << std::endl;
    help_sstr << "Program will try to read '" << Configuration::default_config_filename << "' configuration file." << std::endl;
    help_sstr << "If it's not found, program will be stopped." << std::endl;
    help_sstr << "You can use '-c' flag to point to another file." << std::endl << std::endl;
    help_sstr << "Possible flags: " << std::endl;

    std::for_each(po_cmdline.options().begin(), po_cmdline.options().end(), [&](const boost::shared_ptr<po::option_description>& opt) {
        help_sstr << opt->format_name() << " - " << opt->description() << std::endl;
    });

    msg += std::move(help_sstr.str());
}

void ConfigurationBuilder::build_version_message(std::string& msg) const
{
    msg += std::string("Mattsource's Connection Tunneler v. ") + std::string(MCT_VERSION);

    if (!std::string(MCT_TAG).empty()) 
        msg += std::string("-") + std::string(MCT_TAG);
}

void ConfigurationBuilder::build_generate_message(std::string& msg, po::options_description& po_config) const
{
    std::stringstream cfgFile;
    std::string newline;

    {
        std::stringstream tmpStr;
        tmpStr << std::endl;
        newline = tmpStr.str();
    }

    cfgFile << "# This file has been auto-generated using '-g' cmdline parameter" << std::endl;
    cfgFile << "# Version: " << std::string(MCT_VERSION) << " " << std::string(MCT_TAG) << std::endl;
    cfgFile << std::endl;

    std::for_each(po_config.options().begin(), po_config.options().end(), [&](const boost::shared_ptr<po::option_description>& opt){
        cfgFile << "#" << std::endl;

        std::string comment(std::string("# ") + opt->description());
        int iPos = comment.find(newline);

        while (iPos != std::string::npos) {
            comment.replace(iPos++, 1, newline + std::string("# "));
            iPos = comment.find(newline, iPos);
        }

        cfgFile << comment << std::endl;
        cfgFile << "#" << std::endl;
        cfgFile << "# Default: " << std::string((opt->format_parameter().length() < 7) ? "" :
            opt->format_parameter().substr(6, opt->format_parameter().length() - 7)) << std::endl << std::endl;
        cfgFile << "# " << opt->long_name() << " =" << std::endl << std::endl;
    });

    std::string produced_string = cfgFile.str();
    produced_string.erase(produced_string.length() - 2, 2); // remove final endls

    msg += produced_string;
}

/**
 * Since Boost.Program_Options stores variables in a variable map using boost::any,
 * we need a way to convert them back into std::string - so they can be easily displayed.
 * These 2 classes, type_comparison_helper and auto_value_cast_helper are just helpers,
 * that allow extracting variable values during variables map iteration.
 */
class type_comparison_helper
{
public:
    bool operator()(const std::type_info* lhs, const std::type_info* rhs) const 
    { 
        return lhs->before(*rhs); 
    } 
}; 

template < typename T > 
class auto_value_cast_helper
{
public: 
    std::string operator() (const po::variable_value& v) const
    {
        std::stringstream convert_sstr;
        convert_sstr << v.as<T>(); 
        return convert_sstr.str(); 
    } 
};

bool ConfigurationBuilder::build_show_options_message(
        std::string& msg,
        po::variables_map& vm, po::options_description& po_hidden, po::options_description& po_cmdline, po::options_description& po_config
) const
{
    std::stringstream composed_sstr;

    // All variable types used in ConfigurationBuilder setup_config_fields must be listed here.
    std::map< const std::type_info*, boost::function< std::string (const po::variable_value&) >, type_comparison_helper> conversion_map; 
    conversion_map[ & typeid( std::string ) ] = auto_value_cast_helper< ::std::string >() ; 
    conversion_map[ & typeid( bool ) ] = auto_value_cast_helper< bool >() ; 
    conversion_map[ & typeid( uint16_t ) ] = auto_value_cast_helper< uint16_t >() ; 
    conversion_map[ & typeid( uint64_t ) ] = auto_value_cast_helper< uint64_t >() ;

    composed_sstr << "Program options and their current settings composed of cmdline and cfgfile:" << std::endl << std::endl;

    auto composing_helper = [&](const boost::shared_ptr<po::option_description>& opt) {
        composed_sstr << opt->format_name() << ": ";

        if (opt->format_parameter().empty()) {
            if (vm.count(opt->long_name()) == 1) {
                composed_sstr << "ON";
            } else {
                composed_sstr << "OFF";
            }
        } else {
            const po::variable_value& val = vm[opt->long_name()];
            try {
                composed_sstr << conversion_map[ &val.value().type() ](val);
            } catch (const boost::bad_function_call&) {
                msg = std::string("--show_options error: ") + val.value().type().name() + std::string(", long_name: ") + opt->long_name() + std::string(" is not listed!");
                throw std::logic_error(msg);
            }
        }

        composed_sstr << std::endl;
    };

    try {
        std::for_each(po_hidden.options().begin(), po_hidden.options().end(), composing_helper);
        std::for_each(po_cmdline.options().begin(), po_cmdline.options().end(), composing_helper);
        std::for_each(po_config.options().begin(), po_config.options().end(), composing_helper);
    } catch (const std::logic_error& e) {
        msg = e.what();
        return false;
    }

    msg += composed_sstr.str();
    return true;
}

}
