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

#ifndef MCT_TESTS_CONFIGURATION_TEST_CONFIGURATION_HPP
#define MCT_TESTS_CONFIGURATION_TEST_CONFIGURATION_HPP

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

class TestConfiguration : public CPPUNIT_NS::TestCase
{
    CPPUNIT_TEST_SUITE(TestConfiguration);
    CPPUNIT_TEST(test_Configuration_ctor);
    CPPUNIT_TEST(test_ConfigurationBuilder_ctor);
    CPPUNIT_TEST(test_ConfigurationBuilder_build_configuration_false);
    CPPUNIT_TEST(test_ConfigurationBuilder_build_configuration_help);
    CPPUNIT_TEST(test_ConfigurationBuilder_build_configuration_version);
    CPPUNIT_TEST(test_ConfigurationBuilder_build_configuration_generate);
    CPPUNIT_TEST(test_ConfigurationBuilder_build_configuration_show_options);
    CPPUNIT_TEST(test_ConfigurationFileGenerator_generate);
    CPPUNIT_TEST(test_ConfigurationBuilder_build_configuration_load_cmd_mode);
    CPPUNIT_TEST(test_ConfigurationBuilder_build_configuration_load_cfg_mode);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test_Configuration_ctor();
    void test_ConfigurationBuilder_ctor();
    void test_ConfigurationBuilder_build_configuration_false();
    void test_ConfigurationBuilder_build_configuration_help();
    void test_ConfigurationBuilder_build_configuration_version();
    void test_ConfigurationBuilder_build_configuration_generate();
    void test_ConfigurationBuilder_build_configuration_show_options();
    void test_ConfigurationFileGenerator_generate();

    void test_ConfigurationBuilder_build_configuration_load_cmd_mode();
    void test_ConfigurationBuilder_build_configuration_load_cfg_mode();
};

#endif // MCT_TESTS_CONFIGURATION_TEST_CONFIGURATION_HPP
