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
 * @file Configuration/ConfigurationBuilder.cpp
 *
 * @desc Class used to parse the configuration file into Configuration class.
 */

#include <sstream>

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
		return true;
	}

	msg = std::string("Could not find '") + m_config.get_config_filename() + std::string("' file");
	return false;
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
            ("mode", po::value<std::string>(&m_config.m_mode)->default_value("ggclient"),
                  "specifies the way the application is going to operate\n"
                  "possible modes: ggserver, ggclient")
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

    msg = std::move(help_sstr.str());
}

void ConfigurationBuilder::build_version_message(std::string& msg) const
{
    msg = std::string("Mattsource's Connection Tunneler v. ") + std::string(MCT_VERSION);

    if (!std::string(MCT_TAG).empty()) 
        msg += std::string("-") + std::string(MCT_TAG);
}

void ConfigurationBuilder::build_generate_message(std::string& msg, po::options_description& po_config) const
{
}

}
