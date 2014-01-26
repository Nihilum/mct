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
 * @file tests/Application/TestApplication.hpp
 *
 * @desc Application entry point tests.
 */

#include <vector>
#include <sstream>

#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>

#include <boost/filesystem.hpp>

#include <Application/Application.hpp>

#include "TestApplication.hpp"

class ConfigFileHandler
{
public:
    ConfigFileHandler(const std::string& filename, const std::vector<std::string>& keys_values, const int argc, const char** argv)
    : m_filename(filename)
    {
        m_fileGuard = std::shared_ptr<std::ofstream>(&m_fs, [&](std::ofstream*) { boost::filesystem::remove(m_filename); });

        m_fs.open(m_filename);
        for (auto& key_value : keys_values) {
            m_fs << "#" << std::endl;
            m_fs << "# Standard comment support" << std::endl;
            m_fs << "#" << std::endl;
            m_fs << key_value << std::endl << std::endl;
        }
        m_fs.close();
    }

private:
    std::string m_filename;
    std::ofstream m_fs;
    std::shared_ptr<std::ofstream> m_fileGuard;
};

void TestApplication::setUp()
{
}

void TestApplication::tearDown()
{
}

void TestApplication::test_init_configuration_failed()
{
    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", "invalid.cfg" };
    const int expected_return_value = 1;
    const std::string expected_message("[Application::run()] init_configuration failed.\nError: Could not open 'invalid.cfg' file\n");

    std::streambuf* prevstr = std::cerr.rdbuf();
    std::ostringstream sStr;
    std::cerr.rdbuf(sStr.rdbuf());

    mct::Application app(argc, (char**)argv);
    const int return_value = app.run();

    std::cerr.rdbuf(prevstr);

    CPPUNIT_ASSERT_EQUAL(expected_return_value, return_value);
    CPPUNIT_ASSERT_EQUAL(expected_message, sStr.str());
}

void TestApplication::test_special_mode()
{
    const int argc = 2;
    const char* argv[argc] = { "mct", "-v" };
    const int expected_return_value = 0;
    const std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev\n\n");

    std::streambuf* prevstr = std::cout.rdbuf();
    std::ostringstream sStr;
    std::cout.rdbuf(sStr.rdbuf());

    mct::Application app(argc, (char**)argv);
    const int return_value = app.run();

    std::cout.rdbuf(prevstr);

    CPPUNIT_ASSERT_EQUAL(expected_return_value, return_value);
    CPPUNIT_ASSERT_EQUAL(expected_message, sStr.str());
}

void TestApplication::test_init_logger_failed()
{
    const std::string filename("ta_init_logger_failed.cfg");
    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };
    const int expected_return_value = 1;
    const std::string expected_message("[Application::run()] init_logger failed.\nError: Invalid log severity: 'super'\n");

    ConfigFileHandler cfg_file_hdl(filename, { "log.severity.console = super" }, argc, argv);

    std::streambuf* prevstr = std::cerr.rdbuf();
    std::ostringstream sStr;
    std::cerr.rdbuf(sStr.rdbuf());

    mct::Application app(argc, (char**)argv);
    const int return_value = app.run();

    std::cerr.rdbuf(prevstr);

    CPPUNIT_ASSERT_EQUAL(expected_return_value, return_value);
    CPPUNIT_ASSERT_EQUAL(expected_message, sStr.str());
}

void TestApplication::test_cannot_find_app_mode()
{
    const std::string filename("ta_cannot_find_app_mode.cfg");
    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str() };
    const int expected_return_value = 1;
    const std::string expected_message("Cannot find application mode 'none'");

    ConfigFileHandler cfg_file_hdl(filename, { "mode = none" }, argc, argv);

    std::streambuf* prevstr = std::clog.rdbuf();
    std::ostringstream sStr;
    std::clog.rdbuf(sStr.rdbuf());

    mct::Application app(argc, (char**)argv);
    const int return_value = app.run();

    std::clog.rdbuf(prevstr);

    CPPUNIT_ASSERT_EQUAL(expected_return_value, return_value);
    CPPUNIT_ASSERT_EQUAL(true, sStr.str().find(expected_message) != std::string::npos);
}

CPPUNIT_TEST_SUITE_REGISTRATION(TestApplication);

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
