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

#ifndef MCT_TESTS_LOGGER_TEST_LOGGER_HPP
#define MCT_TESTS_LOGGER_TEST_LOGGER_HPP

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

class TestLogger : public CPPUNIT_NS::TestCase
{
    CPPUNIT_TEST_SUITE(TestLogger);
    CPPUNIT_TEST(test_Logger_debug);
    CPPUNIT_TEST(test_Logger_info);
    CPPUNIT_TEST(test_Logger_warning);
    CPPUNIT_TEST(test_Logger_error);
    CPPUNIT_TEST(test_Logger_fatal);
    CPPUNIT_TEST(test_Logger_log_if_not_silent);
    CPPUNIT_TEST(test_Logger_init_silent);
    CPPUNIT_TEST(test_Logger_init_nofile);
    CPPUNIT_TEST(test_Logger_init_withfile);
    CPPUNIT_TEST(test_Logger_init_log_rotate);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test_Logger_debug();
    void test_Logger_info();
    void test_Logger_warning();
    void test_Logger_error();
    void test_Logger_fatal();
    void test_Logger_log_if_not_silent();
    void test_Logger_init_silent();
    void test_Logger_init_nofile();
    void test_Logger_init_withfile();
    void test_Logger_init_log_rotate();
};

#endif // MCT_TESTS_LOGGER_TEST_LOGGER_HPP
