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
 * @file tests/Logger/TestLogger.hpp
 *
 * @desc Logger functional tests.
 */

#include <vector>

#include <config.hpp>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>

#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/core/core.hpp>
#include <boost/log/attributes/attribute_set.hpp>

#include <Logger/Logger.hpp>
#include <Configuration/Configuration.hpp>
#include <Configuration/ConfigurationBuilder.hpp>

#include "TestLogger.hpp"

class ConfigFileReaderHelper
{
public:
    ConfigFileReaderHelper(const std::string& filename, const std::vector<std::string>& keys_values, const int argc, const char** argv)
    : m_config(argc, (char**)argv), m_log(m_config), m_filename(filename), m_keys_values(keys_values), m_argc(argc), m_argv(argv)
    {
    }

    bool read_file(std::string& message_to_user)
    {
        std::ofstream fs;

        std::shared_ptr<std::ofstream> fileGuard(&fs, [&](std::ofstream*)
        {
            boost::filesystem::remove(m_filename);
        });

        fs.open(m_filename);
        for (auto& keys_values : m_keys_values) {
            fs << "#" << std::endl;
            fs << "# Standard comment support" << std::endl;
            fs << "#" << std::endl;
            fs << keys_values << std::endl << std::endl;
        }
        fs.close();

        mct::ConfigurationBuilder config_builder(m_config);

        return config_builder.build_configuration(message_to_user);
    }

    const mct::Configuration& get_config() const { return m_config; }
    mct::Logger& get_log() { return m_log; }

private:
    mct::Configuration m_config;
    mct::Logger m_log;
    std::string m_filename;
    std::vector<std::string> m_keys_values;
    const int m_argc;
    const char** m_argv;
};

void TestLogger::setUp()
{
    boost::filesystem::remove_all("logs/");
}

void TestLogger::tearDown()
{
    boost::log::core::get()->reset_filter();
    boost::log::core::get()->remove_all_sinks();
    boost::log::core::get()->get_global_attributes().clear();
    boost::log::core::get()->get_thread_attributes().clear();
    boost::filesystem::remove_all("logs/");
}

void test_generic_log_output(const std::string& filename, std::vector<std::string>&& keys_values, void (mct::Logger::*mthd_ptr)(const char*, ...), const char letter, bool expect_empty = false,
    const short int times_to_call_log = 1)
{
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str()};

    ConfigFileReaderHelper helper(filename, keys_values, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file(message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);

    message_to_user.clear();
    expected_message.clear();

    mct::Logger& logger = helper.get_log();
    CPPUNIT_ASSERT_EQUAL(expected_return_value, logger.initialize(message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);

    std::string someText;
    for (int i = 0; i < 256; ++i) {
        someText += boost::lexical_cast<std::string>(i);
    }

    std::streambuf* prevstr = std::clog.rdbuf();
    std::ostringstream sStr;
    std::clog.rdbuf(sStr.rdbuf());
    for (short int i = 0; i < times_to_call_log; ++i) {
        (logger.*mthd_ptr)(someText.c_str());
    }
    std::clog.rdbuf(prevstr);

    std::string sExpr("\\[.*\\]\\["); sExpr += letter; sExpr += "] ";
    sExpr += someText + std::string("\n");
    std::stringstream msgStr;
    msgStr << "Expected to match boost::regex: " << sExpr << std::endl;
    msgStr << "- Actual: " << sStr.str() << std::endl;

    const boost::regex expression(sExpr.c_str());

    if (expect_empty) {
        CPPUNIT_ASSERT_EQUAL(std::string(""), sStr.str());
    } else {
        CPPUNIT_ASSERT_EQUAL_MESSAGE(msgStr.str().c_str(), true, boost::regex_match(sStr.str(), expression));
    }
}

void TestLogger::test_Logger_debug()
{
    test_generic_log_output("./tpl_debug.cfg", {"log.nofile = 1", "log.severity.console = debug"}, &mct::Logger::debug, 'D');
}

void TestLogger::test_Logger_info()
{
    test_generic_log_output("./tpl_info.cfg", {"log.nofile = 1", "log.severity.console = info"}, &mct::Logger::info, 'I');
}

void TestLogger::test_Logger_warning()
{
    test_generic_log_output("./tpl_warning.cfg", {"log.nofile = 1", "log.severity.console = warning"}, &mct::Logger::warning, 'W');
}

void TestLogger::test_Logger_error()
{
    test_generic_log_output("./tpl_error.cfg", {"log.nofile = 1", "log.severity.console = error"}, &mct::Logger::error, 'E');
}

void TestLogger::test_Logger_fatal()
{
    test_generic_log_output("./tpl_fatal.cfg", {"log.nofile = 1", "log.severity.console = fatal"}, &mct::Logger::fatal, 'F');
}

void TestLogger::test_Logger_log_if_not_silent()
{ // severity is set to fatal on purpose - "notice" mode is meant to work always, even when output is set to be sparse
    test_generic_log_output("./tpl_log_if_not_silent.cfg", {"log.nofile = 1", "log.severity.console = fatal"}, &mct::Logger::log_if_not_silent, 'N');
}

void TestLogger::test_Logger_init_silent()
{
    test_generic_log_output("./tpl_init_silent.cfg", {"log.silent = 1", "log.nofile = 1"}, &mct::Logger::log_if_not_silent, 'N', true);
}

void TestLogger::test_Logger_init_nofile()
{
    test_generic_log_output("./tpl_init_nofile.cfg", {"log.nofile = 1"}, &mct::Logger::log_if_not_silent, 'N');
    boost::filesystem::path logfile(std::string("./logs/mct.log"));
    CPPUNIT_ASSERT_EQUAL(false, boost::filesystem::exists(logfile)); 
}

void TestLogger::test_Logger_init_withfile()
{
    test_generic_log_output("./tpl_init_withfile.cfg", {"log.nofile = 0"}, &mct::Logger::log_if_not_silent, 'N');
    boost::filesystem::path logfile(std::string("./logs/mct.log"));
    CPPUNIT_ASSERT_EQUAL(true, boost::filesystem::exists(logfile)); 
}

void TestLogger::test_Logger_init_log_rotate()
{
    std::string log_rotate_dir("logs");
    std::string log_rotate_file1("init_log_rotate_00.log");
    std::string log_rotate_file2("init_log_rotate_01.log");
    boost::filesystem::path first_file(std::string(std::string("./") + log_rotate_dir + std::string("/") + log_rotate_file1));
    boost::filesystem::path second_file(std::string(std::string("./") + log_rotate_dir + std::string("/") + log_rotate_file2));

    test_generic_log_output("./tpl_init_log_rotate.cfg",
        {"log.directory = logs", "log.rotate = 1", "log.rotate.filename = init_log_rotate_%2N.log", "log.rotate.size = 30"},
        &mct::Logger::log_if_not_silent, 'N', false, 2);

    CPPUNIT_ASSERT_EQUAL(true, boost::filesystem::exists(first_file));
    CPPUNIT_ASSERT_EQUAL(true, boost::filesystem::exists(second_file));
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestLogger);

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
