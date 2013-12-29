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
 * @file tests/Configuration/TestConfiguration.hpp
 *
 * @desc Configuration functional tests.
 */

#include <config.hpp>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>

#include <boost/filesystem.hpp>

#include <Configuration/Configuration.hpp>
#include <Configuration/ConfigurationBuilder.hpp>

#include "TestConfiguration.hpp"

class ConfigFileReaderHelper
{
public:
    ConfigFileReaderHelper(const std::string& filename, const std::string& key, const int argc, const char** argv)
    : m_config(argc, (char**)argv), m_filename(filename), m_key(key), m_argc(argc), m_argv(argv)
    {
        std::cout << std::endl << "params: ";
        for (int i = 0; i < argc; ++i) {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;
    }

    bool read_file(const std::string& given_default, std::string& message_to_user)
    {
        std::ofstream fs;

        std::shared_ptr<std::ofstream> fileGuard(&fs, [&](std::ofstream*)
        {
            boost::filesystem::remove(m_filename);
        });

        fs.open(m_filename);
        fs << "#" << std::endl;
        fs << "# Standard comment support" << std::endl;
        fs << "#" << std::endl;
        fs << m_key << " = " << given_default << std::endl;
        fs.close();

        mct::ConfigurationBuilder config_builder(m_config);

        return config_builder.build_configuration(message_to_user);
    }

    const mct::Configuration& get_config() const { return m_config; }

private:
    mct::Configuration m_config;
    std::string m_filename;
    std::string m_key;
    const int m_argc;
    const char** m_argv;
};

void TestConfiguration::setUp()
{
}

void TestConfiguration::tearDown()
{
}

void TestConfiguration::test_Configuration_ctor()
{
    const int argc = 1;
    const char* argv[argc] = { "mct" };
    mct::Configuration config(argc, (char**)argv);

    CPPUNIT_ASSERT_EQUAL(argc, config.get_app_argument_count());
    CPPUNIT_ASSERT_EQUAL(std::string(argv[0]), std::string(config.get_app_argument_array()[0]));
    CPPUNIT_ASSERT_EQUAL(std::string(argv[0]), config.get_app_name());
}

void TestConfiguration::test_ConfigurationBuilder_ctor()
{
    const int argc = 1;
    const char* argv[argc] = { "mct" };
    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);

    CPPUNIT_ASSERT_EQUAL(true, &config_builder.get_configuration() == &config);
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_false()
{
    const int argc = 1;
    const char* argv[argc] = { "mct" };
    const bool expected_return_value = false;
    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string error_msg;

    CPPUNIT_ASSERT_EQUAL(expected_return_value, config_builder.build_configuration(error_msg));
    CPPUNIT_ASSERT_EQUAL(std::string("Could not open 'mct.cfg' file"), error_msg);
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_help()
{
    const int argc = 2;
    const char* argv[argc] = { "mct", "-h" };
    const bool expected_return_value = true;
    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string message_to_user;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev\n"
                                "Program will try to read 'mct.cfg' configuration file.\n"
                                "If it's not found, program will be stopped.\n"
                                "You can use '-c' flag to point to another file.\n\n"

                                "Possible flags: \n"
                                "-v [ --version ] - print version string\n"
                                "-h [ --help ] - show help message\n"
                                "-g [ --generate ] - generate config file\n"
                                "-c [ --config ] - configuration filename\n");

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, config_builder.build_configuration(message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_version()
{
    const int argc = 2;
    const char* argv[argc] = { "mct", "-v" };
    const bool expected_return_value = true;
    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string message_to_user;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, config_builder.build_configuration(message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_generate()
{
    const int argc = 2;
    const char* argv[argc] = { "mct", "-g" };
    const bool expected_return_value = true;
    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string message_to_user;
    std::string expected_message = "# This file has been auto-generated using '-g' cmdline parameter\n"
                                   "# Version: 0.1.0 dev\n\n"

                                   "#\n"
                                   "# specifies the way the application is going to operate\n"
                                   "# possible modes: ggserver, ggclient\n"
                                   "#\n"
                                   "# Default: ggclient\n\n"

                                   "# mode =\n\n"

                                   "#\n"
                                   "# should logger be completely silent\n"
                                   "#\n"
                                   "# Default: 0\n\n"

                                   "# log.silent =\n\n"

                                   "#\n"
                                   "# tells logger if any log files or directories can be created\n"
                                   "#\n"
                                   "# Default: 0\n\n"

                                   "# log.nofile =\n\n"

                                   "#\n"
                                   "# directory name where the log files will be stored\n"
                                   "#\n"
                                   "# Default: logs\n\n"

                                   "# log.directory =\n\n"

                                   "#\n"
                                   "# log filename, used only if log.rotate is set to 0\n"
                                   "#\n"
                                   "# Default: mct.log\n\n"

                                   "# log.filename =\n\n"

                                   "#\n"
                                   "# date_time formatting for logger, the following variables may be used:\n"
                                   "# www.boost.org/doc/libs/1_55_0/doc/html/date_time/date_time_io.html\n"
                                   "#\n"
                                   "# Default: %H:%M:%S.%f\n\n"

                                   "# log.format =\n\n"

                                   "#\n"
                                   "# determines which log messages will be shown in the console,\n"
                                   "# the following may be used:\n"
                                   "# debug, info, warning, error, fatal\n"
                                   "#\n"
                                   "# Default: info\n\n"

                                   "# log.severity.console =\n\n"

                                   "#\n"
                                   "# determines which log messages will be shown in the file,\n"
                                   "# the following may be used:\n"
                                   "# debug, info, warning, error, fatal\n"
                                   "#\n"
                                   "# Default: info\n\n"

                                   "# log.severity.file =\n\n"

                                   "#\n"
                                   "# should logger use rotating log files\n"
                                   "#\n"
                                   "# Default: 0\n\n"

                                   "# log.rotate =\n\n"

                                   "#\n"
                                   "# maximum size (in characters) of the rotating log file\n"
                                   "#\n"
                                   "# Default: 1048576\n\n"

                                   "# log.rotate.size =\n\n"

                                   "#\n"
                                   "# filename pattern of the rotating log file,\n"
                                   "# the following variables may be used:\n"
                                   "# %Y -- year, %m -- month, %d -- day, %H -- hour, %M -- minute, %S -- second,\n"
                                   "# %nN -- log's ordinal number (n - number of digits)\n"
                                   "#\n"
                                   "# Default: %Y%m%d_%H%M%S_%5N-mct.log\n\n"

                                   "# log.rotate.filename =\n\n"

                                   "#\n"
                                   "# maximum size (in bytes) of all the rotating log files combined\n"
                                   "#\n"
                                   "# Default: 1073741824\n\n"

                                   "# log.rotate.all_files_max_size =\n\n"

                                   "#\n"
                                   "# minimum free disk space (in bytes) to run rotating log files\n"
                                   "#\n"
                                   "# Default: 1073741824\n\n"

                                   "# log.rotate.min_free_space =";

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, config_builder.build_configuration(message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_show_options()
{
    const int argc = 3;
    const char* argv[argc] = { "mct", "-v", "--show_options" };
    const bool expected_return_value = true;
    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string message_to_user;
    std::string expected_message = 
        "Program options and their current settings composed of cmdline and cfgfile:\n\n"

        "--show_options: ON\n"
        "-v [ --version ]: ON\n"
        "-h [ --help ]: OFF\n"
        "-g [ --generate ]: OFF\n"
        "-c [ --config ]: mct.cfg\n"
        "--mode: ggclient\n"
        "--log.silent: 0\n"
        "--log.nofile: 0\n"
        "--log.directory: logs\n"
        "--log.filename: mct.log\n"
        "--log.format: %H:%M:%S.%f\n"
        "--log.severity.console: info\n"
        "--log.severity.file: info\n"
        "--log.rotate: 0\n"
        "--log.rotate.size: 1048576\n"
        "--log.rotate.filename: %Y%m%d_%H%M%S_%5N-mct.log\n"
        "--log.rotate.all_files_max_size: 1073741824\n"
        "--log.rotate.min_free_space: 1073741824\n"
        "Mattsource's Connection Tunneler v. 0.1.0-dev"
        ;

        CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, config_builder.build_configuration(message_to_user));
        CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_mode()
{
    std::string param("mode");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_mode.cfg");
    std::string expected_mode("ggclient");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), expected_mode.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("ggserver", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_mode, helper.get_config().get_app_mode());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_mode()
{
    std::string param("mode");
    std::string filename("./tbc_mode.cfg");
    std::string expected_mode("ggclient");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("ggclient", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_mode, helper.get_config().get_app_mode());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_silent()
{
    std::string param("log.silent");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_silent.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "true" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("false", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_silent());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_silent()
{
    std::string param("log.silent");
    std::string filename("./tbc_log_silent.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("true", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_silent());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_nofile()
{
    std::string param("log.nofile");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_nofile.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "true" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("false", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_nofile());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_nofile()
{
    std::string param("log.nofile");
    std::string filename("./tbc_log_nofile.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("true", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_nofile());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_directory()
{
    std::string param("log.directory");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_directory.cfg");
    std::string expected_value("log");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "log" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("logs", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_directory());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_directory()
{
    std::string param("log.directory");
    std::string filename("./tbc_log_directory.cfg");
    std::string expected_value("log");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_directory());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_filename()
{
    std::string param("log.filename");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_filename.cfg");
    std::string expected_value("conn.log");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "conn.log" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("tunnel.log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_filename());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_filename()
{
    std::string param("log.filename");
    std::string filename("./tbc_log_filename.cfg");
    std::string expected_value("conn.log");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("conn.log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_filename());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_format()
{
    std::string param("log.format");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_format.cfg");
    std::string expected_value("%H:%M");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "%H:%M" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("%H:%M:%S", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_format());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_format()
{
    std::string param("log.format");
    std::string filename("./tbc_log_format.cfg");
    std::string expected_value("%H:%M");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("%H:%M", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_format());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_severity_console()
{
    std::string param("log.severity.console");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_severity_console.cfg");
    std::string expected_value("debug");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "debug" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("warning", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_severity_console());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_severity_console()
{
    std::string param("log.severity.console");
    std::string filename("./tbc_log_severity_console.cfg");
    std::string expected_value("debug");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("debug", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_severity_console());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_severity_file()
{
    std::string param("log.severity.file");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_severity_file.cfg");
    std::string expected_value("debug");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "debug" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("warning", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_severity_file());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_severity_file()
{
    std::string param("log.severity.file");
    std::string filename("./tbc_log_severity_file.cfg");
    std::string expected_value("debug");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("debug", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_severity_file());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_rotate()
{
    std::string param("log.rotate");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_rotate.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "true" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("false", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_rotate()
{
    std::string param("log.rotate");
    std::string filename("./tbc_log_rotate.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("true", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_rotate_size()
{
    std::string param("log.rotate.size");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_rotate_size.cfg");
    uint64_t expected_value = 1024;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "1024" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("4096", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_size());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_rotate_size()
{
    std::string param("log.rotate.size");
    std::string filename("./tbc_log_rotate_size.cfg");
    uint64_t expected_value = 1024;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("1024", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_size());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_rotate_filename()
{
    std::string param("log.rotate.filename");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_rotate_filename.cfg");
    std::string expected_value("%H_%M-mct.log");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "%H_%M-mct.log" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("%M-mct.log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_filename());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_rotate_filename()
{
    std::string param("log.rotate.filename");
    std::string filename("./tbc_log_rotate_filename.cfg");
    std::string expected_value("%H_%M-mct.log");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("%H_%M-mct.log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_filename());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_rotate_all_files_max_size()
{
    std::string param("log.rotate.all_files_max_size");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_rotate_all_files_max_size.cfg");
    uint64_t expected_value = 1024;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "1024" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("4096", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_all_files_max_size());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_rotate_all_files_max_size()
{
    std::string param("log.rotate.all_files_max_size");
    std::string filename("./tbc_log_rotate_all_files_max_size.cfg");
    uint64_t expected_value = 1024;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("1024", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_all_files_max_size());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cmd_log_rotate_min_free_space()
{
    std::string param("log.rotate.min_free_space");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_log_rotate_min_free_space.cfg");
    uint64_t expected_value = 1024;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "1024" };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("4096", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_min_free_space());
}

void TestConfiguration::test_ConfigurationBuilder_build_configuration_load_cfg_log_rotate_min_free_space()
{
    std::string param("log.rotate.min_free_space");
    std::string filename("./tbc_log_rotate_min_free_space.cfg");
    uint64_t expected_value = 1024;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("1024", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_min_free_space());
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestConfiguration);

int main(int argc, char* argv[])
{
    CPPUNIT_NS::TestResult controller;

    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);

    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener(&progress);

    CPPUNIT_NS::TextUi::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    CPPUNIT_NS::CompilerOutputter outputter(&result, std::cerr);
    outputter.write();

    return result.wasSuccessful() ? 0 : 1;
}
