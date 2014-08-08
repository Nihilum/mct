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
 * @file tests/Configuration/TestConfiguration.hpp
 *
 * @desc Configuration functional tests.
 */

#include <config.hpp>

#include <boost/filesystem.hpp>

#include <Configuration/Configuration.hpp>
#include <Configuration/ConfigurationBuilder.hpp>

#include "TestConfiguration.hpp"

namespace testconfig
{

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

        auto insert_value = [&] (const std::string& val) {
            fs << "#" << std::endl;
            fs << "# Standard comment support" << std::endl;
            fs << "#" << std::endl;
            fs << m_key << " = " << val << std::endl;
        };

        size_t start_pos = 0;
        size_t sep_pos = given_default.find(" ", start_pos);

        if (sep_pos == std::string::npos) {
            insert_value(given_default);
        } else {
            while (sep_pos != std::string::npos) {
                insert_value(given_default.substr(start_pos, sep_pos - start_pos));
                start_pos = ++sep_pos;
                sep_pos = given_default.find(" ", start_pos);

                if (sep_pos == std::string::npos) {
                    insert_value(given_default.substr(start_pos, given_default.length() - start_pos));
                }
            }
        }

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

}

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

void TestConfiguration::test_ctor()
{
    const int argc = 1;
    const char* argv[argc] = { "mct" };
    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);

    CPPUNIT_ASSERT_EQUAL(true, &config_builder.get_configuration() == &config);
}

void TestConfiguration::test_false()
{
    const int argc = 1;
    const char* argv[argc] = { "mct" };
    const bool expected_return_value = false;
    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string error_msg;

    CPPUNIT_ASSERT_EQUAL(expected_return_value, config_builder.build_configuration(error_msg));
#ifdef WIN32
    CPPUNIT_ASSERT_EQUAL(std::string("Could not open 'mct.cfg' file"), error_msg);
#else
    CPPUNIT_ASSERT_EQUAL(std::string("Could not open '/etc/mct.cfg' file"), error_msg);
#endif
}

void TestConfiguration::test_help()
{
    const int argc = 2;
    const char* argv[argc] = { "mct", "-h" };
    const bool expected_return_value = true;
    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string message_to_user;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev\n"
#ifdef WIN32
                                "Program will try to read 'mct.cfg' configuration file.\n"
#else
                                "Program will try to read '/etc/mct.cfg' configuration file.\n"
#endif
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

void TestConfiguration::test_version()
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

void TestConfiguration::test_generate()
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
                                   "# possible modes: proxy\n"
                                   "#\n"
                                   "# Default: proxy\n\n"

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

                                   "# log.rotate.min_free_space =\n\n"

                                   "#\n"
                                   "# a set of local ports to bind to in proxy mode, separated by spaces\n"
                                   "#\n"
                                   "# Default: 8080\n\n"

                                   "# mode.proxy.local_port =\n\n"

                                   "#\n"
                                   "# a set of remote ports to send to in proxy mode, separated by spaces\n"
                                   "#\n"
                                   "# Default: 80\n\n"

                                   "# mode.proxy.remote_port =\n\n"

                                   "#\n"
                                   "# a set of local interfaces to bind to in proxy mode, separated by spaces\n"
                                   "#\n"
                                   "# Default: localhost\n\n"

                                   "# mode.proxy.local_host =\n\n"

                                   "#\n"
                                   "# a set of remote hosts to send to in proxy mode, separated by spaces\n"
                                   "#\n"
                                   "# Default: 127.0.0.1\n\n"

                                   "# mode.proxy.remote_host =";

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, config_builder.build_configuration(message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
}

void TestConfiguration::test_show_options()
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
#ifdef WIN32
        "-c [ --config ]: mct.cfg\n"
#else
        "-c [ --config ]: /etc/mct.cfg\n"
#endif
        "--mode: proxy\n"
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
        "--mode.proxy.local_port: \n"
        "--mode.proxy.remote_port: \n"
        "--mode.proxy.local_host: \n"
        "--mode.proxy.remote_host: \n"
        "Mattsource's Connection Tunneler v. 0.1.0-dev"
        ;

        CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, config_builder.build_configuration(message_to_user));
        CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
}

void TestConfiguration::test_load_cmd_mode()
{
    std::string param("mode");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_mode.cfg");
    std::string expected_mode("proxy");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), expected_mode.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("proxy", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_mode, helper.get_config().get_app_mode());
}

void TestConfiguration::test_load_cfg_mode()
{
    std::string param("mode");
    std::string filename("./tbc_mode.cfg");
    std::string expected_mode("proxy");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("proxy", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_mode, helper.get_config().get_app_mode());
}

void TestConfiguration::test_load_cmd_log_silent()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("false", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_silent());
}

void TestConfiguration::test_load_cfg_log_silent()
{
    std::string param("log.silent");
    std::string filename("./tbc_log_silent.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("true", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_silent());
}

void TestConfiguration::test_load_cmd_log_nofile()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("false", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_nofile());
}

void TestConfiguration::test_load_cfg_log_nofile()
{
    std::string param("log.nofile");
    std::string filename("./tbc_log_nofile.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("true", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_nofile());
}

void TestConfiguration::test_load_cmd_log_directory()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("logs", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_directory());
}

void TestConfiguration::test_load_cfg_log_directory()
{
    std::string param("log.directory");
    std::string filename("./tbc_log_directory.cfg");
    std::string expected_value("log");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_directory());
}

void TestConfiguration::test_load_cmd_log_filename()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("tunnel.log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_filename());
}

void TestConfiguration::test_load_cfg_log_filename()
{
    std::string param("log.filename");
    std::string filename("./tbc_log_filename.cfg");
    std::string expected_value("conn.log");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("conn.log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_filename());
}

void TestConfiguration::test_load_cmd_log_format()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("%H:%M:%S", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_format());
}

void TestConfiguration::test_load_cfg_log_format()
{
    std::string param("log.format");
    std::string filename("./tbc_log_format.cfg");
    std::string expected_value("%H:%M");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("%H:%M", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_format());
}

void TestConfiguration::test_load_cmd_log_severity_console()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("warning", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_severity_console());
}

void TestConfiguration::test_load_cfg_log_severity_console()
{
    std::string param("log.severity.console");
    std::string filename("./tbc_log_severity_console.cfg");
    std::string expected_value("debug");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("debug", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_severity_console());
}

void TestConfiguration::test_load_cmd_log_severity_file()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("warning", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_severity_file());
}

void TestConfiguration::test_load_cfg_log_severity_file()
{
    std::string param("log.severity.file");
    std::string filename("./tbc_log_severity_file.cfg");
    std::string expected_value("debug");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("debug", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_severity_file());
}

void TestConfiguration::test_load_cmd_log_rotate()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("false", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate());
}

void TestConfiguration::test_load_cfg_log_rotate()
{
    std::string param("log.rotate");
    std::string filename("./tbc_log_rotate.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("true", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate());
}

void TestConfiguration::test_load_cmd_log_rotate_size()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("4096", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_size());
}

void TestConfiguration::test_load_cfg_log_rotate_size()
{
    std::string param("log.rotate.size");
    std::string filename("./tbc_log_rotate_size.cfg");
    uint64_t expected_value = 1024;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("1024", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_size());
}

void TestConfiguration::test_load_cmd_log_rotate_filename()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("%M-mct.log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_filename());
}

void TestConfiguration::test_load_cfg_log_rotate_filename()
{
    std::string param("log.rotate.filename");
    std::string filename("./tbc_log_rotate_filename.cfg");
    std::string expected_value("%H_%M-mct.log");
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("%H_%M-mct.log", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_filename());
}

void TestConfiguration::test_load_cmd_log_rotate_all_files_max_size()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("4096", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_all_files_max_size());
}

void TestConfiguration::test_load_cfg_log_rotate_all_files_max_size()
{
    std::string param("log.rotate.all_files_max_size");
    std::string filename("./tbc_log_rotate_all_files_max_size.cfg");
    uint64_t expected_value = 1024;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("1024", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_all_files_max_size());
}

void TestConfiguration::test_load_cmd_log_rotate_min_free_space()
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

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("4096", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_min_free_space());
}

void TestConfiguration::test_load_cfg_log_rotate_min_free_space()
{
    std::string param("log.rotate.min_free_space");
    std::string filename("./tbc_log_rotate_min_free_space.cfg");
    uint64_t expected_value = 1024;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("1024", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_log_rotate_min_free_space());
}

void TestConfiguration::test_load_cmd_mode_proxy_local_port()
{
    std::string param("mode.proxy.local_port");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_mode_proxy_local_port.cfg");
    uint16_t expected_value = 6513;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "6513" };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("7543", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_mode_proxy_local_ports()[0]);
}

void TestConfiguration::test_load_cfg_mode_proxy_local_port()
{
    std::string param("mode.proxy.local_port");
    std::string filename("./tbc_mode_proxy_local_port.cfg");
    uint16_t expected_value = 6513;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("6513", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_mode_proxy_local_ports()[0]);
}

void TestConfiguration::test_load_cfg_mode_proxy_local_port_multiple()
{
    std::string param("mode.proxy.local_port");
    std::string filename("./tbc_mode_proxy_local_port.cfg");
    uint16_t expected_value_1 = 6513;
    uint16_t expected_value_2 = 6514;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("6513 6514", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value_1, helper.get_config().get_mode_proxy_local_ports()[0]);
    CPPUNIT_ASSERT_EQUAL(expected_value_2, helper.get_config().get_mode_proxy_local_ports()[1]);
}

void TestConfiguration::test_load_cmd_mode_proxy_remote_port()
{
    std::string param("mode.proxy.remote_port");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_mode_proxy_remote_port.cfg");
    uint16_t expected_value = 6513;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "6513" };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("7543", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_mode_proxy_remote_ports()[0]);
}

void TestConfiguration::test_load_cfg_mode_proxy_remote_port()
{
    std::string param("mode.proxy.remote_port");
    std::string filename("./tbc_mode_proxy_remote_port.cfg");
    uint16_t expected_value = 6513;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("6513", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_mode_proxy_remote_ports()[0]);
}

void TestConfiguration::test_load_cfg_mode_proxy_remote_port_multiple()
{
    std::string param("mode.proxy.remote_port");
    std::string filename("./tbc_mode_proxy_remote_port.cfg");
    uint16_t expected_value_1 = 6513;
    uint16_t expected_value_2 = 6514;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("6513 6514", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value_1, helper.get_config().get_mode_proxy_remote_ports()[0]);
    CPPUNIT_ASSERT_EQUAL(expected_value_2, helper.get_config().get_mode_proxy_remote_ports()[1]);
}

void TestConfiguration::test_load_cmd_mode_proxy_local_host()
{
    std::string param("mode.proxy.local_host");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_mode_proxy_local_port.cfg");
    std::string expected_value = "127.0.0.1";
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "127.0.0.1" };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("localhost", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_mode_proxy_local_hosts()[0]);
}

void TestConfiguration::test_load_cfg_mode_proxy_local_host()
{
    std::string param("mode.proxy.local_host");
    std::string filename("./tbc_mode_proxy_local_host.cfg");
    std::string expected_value = "127.0.0.1";
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("127.0.0.1", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_mode_proxy_local_hosts()[0]);
}

void TestConfiguration::test_load_cfg_mode_proxy_local_host_multiple()
{
    std::string param("mode.proxy.local_host");
    std::string filename("./tbc_mode_proxy_local_host.cfg");
    std::string expected_value_1 = "127.0.0.1";
    std::string expected_value_2 = "0.0.0.0";
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("127.0.0.1 0.0.0.0", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value_1, helper.get_config().get_mode_proxy_local_hosts()[0]);
    CPPUNIT_ASSERT_EQUAL(expected_value_2, helper.get_config().get_mode_proxy_local_hosts()[1]);
}

void TestConfiguration::test_load_cmd_mode_proxy_remote_host()
{
    std::string param("mode.proxy.remote_host");
    std::string cmd_param("--"); cmd_param += param;
    std::string filename("./tbc_mode_proxy_remote_port.cfg");
    std::string expected_value = "127.0.0.1";
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 5;
    const char* argv[argc] = { "mct", "-c", filename.c_str(), cmd_param.c_str(), "127.0.0.1" };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("localhost", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_mode_proxy_remote_hosts()[0]);
}

void TestConfiguration::test_load_cfg_mode_proxy_remote_host()
{
    std::string param("mode.proxy.remote_host");
    std::string filename("./tbc_mode_proxy_remote_host.cfg");
    std::string expected_value = "127.0.0.1";
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("127.0.0.1", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value, helper.get_config().get_mode_proxy_remote_hosts()[0]);
}

void TestConfiguration::test_load_cfg_mode_proxy_remote_host_multiple()
{
    std::string param("mode.proxy.remote_host");
    std::string filename("./tbc_mode_proxy_remote_host.cfg");
    std::string expected_value_1 = "127.0.0.1";
    std::string expected_value_2 = "0.0.0.0";
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };

    testconfig::ConfigFileReaderHelper helper(filename, param, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file("127.0.0.1 0.0.0.0", message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);
    CPPUNIT_ASSERT_EQUAL(expected_value_1, helper.get_config().get_mode_proxy_remote_hosts()[0]);
    CPPUNIT_ASSERT_EQUAL(expected_value_2, helper.get_config().get_mode_proxy_remote_hosts()[1]);
}
