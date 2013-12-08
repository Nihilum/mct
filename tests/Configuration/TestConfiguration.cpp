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
 * @desc Configuration entry point tests.
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

                                   "# log.nofile =";

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
