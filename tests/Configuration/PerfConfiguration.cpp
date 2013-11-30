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
 * @file tests/Configuration/PerfConfiguration.cpp
 *
 * @desc Configuration entry point performance tests.
 */

#include <ctime>
#include <cmath>
#include <string>
#include <cstdint>
#include <iostream>

#include <boost/timer/timer.hpp>

#include <Configuration/Configuration.hpp>
#include <Configuration/ConfigurationBuilder.hpp>

using boost::timer::cpu_timer;
using boost::timer::cpu_times;
using boost::timer::nanosecond_type;

void display_stats(const cpu_timer& timer, const nanosecond_type& expected_time)
{
    cpu_times const elapsed_times(timer.elapsed());
    nanosecond_type const elapsed(elapsed_times.wall);

    if (elapsed > expected_time) {
        std::cout << "WARN  PERFORMANCE DECREASE, expected_time: " << expected_time << "(ns) vs elapsed_time: " << elapsed << " (ns)\n";
    }

    std::cout << timer.format();
}

void perf_test_Configuration_ctor()
{
    nanosecond_type const expected_time(1000000); // 1 millisecond
    std::cout << "START perf_test_Configuration_ctor()\n";

    const int argc = 1;
    const char* argv[argc] = { "mct" };

    cpu_timer timer;

    std::string test;
    mct::Configuration config(argc, (char**)argv);
    test = config.get_app_name();

    timer.stop();
    display_stats(timer, expected_time);
    std::cout << "END   perf_test_Configuration_ctor()\n" << std::endl;
}

void perf_test_ConfigurationBuilder_ctor()
{
    nanosecond_type const expected_time(1000000); // 1 millisecond
    std::cout << "START perf_test_ConfigurationBuilder_ctor()\n";

    const int argc = 1;
    const char* argv[argc] = { "mct" };

    mct::Configuration config(argc, (char**)argv);

    cpu_timer timer;

    volatile bool test = false;
    mct::ConfigurationBuilder config_builder(config);
    test = (&config_builder.get_configuration() == &config);

    timer.stop();
    display_stats(timer, expected_time);
    std::cout << "END   perf_test_ConfigurationBuilder_ctor()\n" << std::endl;
}

void perf_test_ConfigurationBuilder_build_configuration_false()
{
    nanosecond_type const expected_time(1000000); // 1 millisecond
    std::cout << "START perf_test_ConfigurationBuilder_build_configuration_false()\n";

    const int argc = 1;
    const char* argv[argc] = { "mct" };

    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string message_to_user;

    cpu_timer timer;

    volatile bool test = false;
    test = config_builder.build_configuration(message_to_user);

    timer.stop();
    display_stats(timer, expected_time);
    std::cout << "END   perf_test_ConfigurationBuilder_build_configuration_false()\n" << std::endl;
}

void perf_test_ConfigurationBuilder_build_configuration_help()
{
    nanosecond_type const expected_time(1000000); // 1 millisecond
    std::cout << "START perf_test_ConfigurationBuilder_build_configuration_help()\n";

    const int argc = 2;
    const char* argv[argc] = { "mct", "-h" };

    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string message_to_user;

    cpu_timer timer;

    volatile bool test = false;
    test = config_builder.build_configuration(message_to_user);

    timer.stop();
    display_stats(timer, expected_time);
    std::cout << "END   perf_test_ConfigurationBuilder_build_configuration_help()\n" << std::endl;
}

void perf_test_ConfigurationBuilder_build_configuration_version()
{
    nanosecond_type const expected_time(1000000); // 1 millisecond
    std::cout << "START perf_test_ConfigurationBuilder_build_configuration_version()\n";

    const int argc = 2;
    const char* argv[argc] = { "mct", "-v" };

    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string message_to_user;

    cpu_timer timer;

    volatile bool test = false;
    test = config_builder.build_configuration(message_to_user);

    timer.stop();
    display_stats(timer, expected_time);
    std::cout << "END   perf_test_ConfigurationBuilder_build_configuration_version()\n" << std::endl;
}

void perf_test_ConfigurationBuilder_build_configuration_generate()
{
    nanosecond_type const expected_time(1000000); // 1 millisecond
    std::cout << "START perf_test_ConfigurationBuilder_build_configuration_generate()\n";

    const int argc = 2;
    const char* argv[argc] = { "mct", "-g" };

    mct::Configuration config(argc, (char**)argv);
    mct::ConfigurationBuilder config_builder(config);
    std::string message_to_user;

    cpu_timer timer;

    volatile bool test = false;
    test = config_builder.build_configuration(message_to_user);

    timer.stop();
    display_stats(timer, expected_time);
    std::cout << "END   perf_test_ConfigurationBuilder_build_configuration_generate()\n" << std::endl;
}

int main(int argc, char* argv[])
{
    perf_test_Configuration_ctor();
    perf_test_ConfigurationBuilder_ctor();
    perf_test_ConfigurationBuilder_build_configuration_false();
    perf_test_ConfigurationBuilder_build_configuration_help();
    perf_test_ConfigurationBuilder_build_configuration_version();
    perf_test_ConfigurationBuilder_build_configuration_generate();
}
