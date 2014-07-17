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
 * @file tests/ModeProxy/TestModeProxy.hpp
 *
 * @desc ModeProxy application mode tests.
 */

#ifndef MCT_TESTS_MODEPROXY_TEST_MODEPROXY_HPP
#define MCT_TESTS_MODEPROXY_TEST_MODEPROXY_HPP

#include <moctest/moctest.hpp>

class TestModeProxy : public CPPUNIT_NS::TestCase
{
    CPPUNIT_TEST_SUITE(TestModeProxy);
    CPPUNIT_TEST(test_modeproxy_error_local_port_already_bound);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test_modeproxy_error_local_port_already_bound();
};

#endif // MCT_TESTS_MODEPROXY_TEST_MODEPROXY_HPP
