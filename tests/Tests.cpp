/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Mateusz Kolodziejski
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
 * @file tests/Tests.cpp
 *
 * @desc Entry point for all mct tests.
 */

#include <moctest/moctest.hpp>

#include "Application/TestApplication.hpp"
#include "Configuration/TestConfiguration.hpp"
#include "Logger/TestLogger.hpp"
#include "Mode/TestMode.hpp"
#include "ModeFactory/TestModeFactory.hpp"
#include "ModeProxy/TestModeProxy.hpp"


int main(int argc, char* argv[])
{
    moctest::Framework tests(argc, argv);
    tests.register_suite<TestApplication>();
    tests.register_suite<TestConfiguration>();
    tests.register_suite<TestLogger>();
    tests.register_suite<TestMode>();
    tests.register_suite<TestModeFactory>();
    tests.register_suite<TestModeProxy>();
    return tests.run();
}
