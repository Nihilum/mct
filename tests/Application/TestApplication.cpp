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
 * @file tests/Application/TestApplication.hpp
 *
 * @desc Application entry point tests.
 */

#include <cppunit/CompilerOutputter.h>
#include <cppunit/ui/text/TestRunner.h>

#include <Application/Application.hpp>

#include "TestApplication.hpp"

void TestApplication::setUp()
{
}

void TestApplication::tearDown()
{
}

void TestApplication::test_init_configuration_fail()
{
    std::cout << std::endl;
    const int argc = 1;
    const char* argv[argc] = { "mct" };
    const bool expected_return_value = false;
    const std::string expected_message("");
    std::string str_error;
    mct::Application app(argc, (char**)argv);

    CPPUNIT_ASSERT_EQUAL(expected_return_value, app.init_configuration(str_error));
    CPPUNIT_ASSERT_EQUAL(expected_message, str_error);
}

void TestApplication::test_run()
{
    std::cout << std::endl;
    const int argc = 2;
    const char* argv[argc] = { "mct", "-c" };
	const int expected_return_value = 0;
	mct::Application app(argc, (char**)argv);
	int ret = app.run();
	CPPUNIT_ASSERT_EQUAL(expected_return_value, ret);
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
