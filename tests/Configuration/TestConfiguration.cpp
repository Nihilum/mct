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

#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>

#include <Configuration/Configuration.hpp>
#include <Configuration/ConfigurationBuilder.hpp>

#include "TestConfiguration.hpp"

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
    CPPUNIT_ASSERT_EQUAL(std::string("Could not find 'mct.cfg' file"), error_msg);
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
