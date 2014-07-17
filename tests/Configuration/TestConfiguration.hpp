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
 * @file tests/Configuration/TestConfiguration.hpp
 *
 * @desc Configuration functional tests.
 */

#ifndef MCT_TESTS_CONFIGURATION_TEST_CONFIGURATION_HPP
#define MCT_TESTS_CONFIGURATION_TEST_CONFIGURATION_HPP

#include <moctest/moctest.hpp>

class TestConfiguration : public CPPUNIT_NS::TestCase
{
    CPPUNIT_TEST_SUITE(TestConfiguration);
    CPPUNIT_TEST(test_Configuration_ctor);
    CPPUNIT_TEST(test_ctor);
    CPPUNIT_TEST(test_false);
    CPPUNIT_TEST(test_help);
    CPPUNIT_TEST(test_version);
    CPPUNIT_TEST(test_generate);
    CPPUNIT_TEST(test_show_options);

    CPPUNIT_TEST(test_load_cmd_mode);
    CPPUNIT_TEST(test_load_cfg_mode);
    CPPUNIT_TEST(test_load_cmd_log_silent);
    CPPUNIT_TEST(test_load_cfg_log_silent);
    CPPUNIT_TEST(test_load_cmd_log_nofile);
    CPPUNIT_TEST(test_load_cfg_log_nofile);
    CPPUNIT_TEST(test_load_cmd_log_directory);
    CPPUNIT_TEST(test_load_cfg_log_directory);
    CPPUNIT_TEST(test_load_cmd_log_filename);
    CPPUNIT_TEST(test_load_cfg_log_filename);
    CPPUNIT_TEST(test_load_cmd_log_format);
    CPPUNIT_TEST(test_load_cfg_log_format);
    CPPUNIT_TEST(test_load_cmd_log_severity_console);
    CPPUNIT_TEST(test_load_cfg_log_severity_console);
    CPPUNIT_TEST(test_load_cmd_log_severity_file);
    CPPUNIT_TEST(test_load_cfg_log_severity_file);
    CPPUNIT_TEST(test_load_cmd_log_rotate);
    CPPUNIT_TEST(test_load_cfg_log_rotate);
    CPPUNIT_TEST(test_load_cmd_log_rotate_size);
    CPPUNIT_TEST(test_load_cfg_log_rotate_size);
    CPPUNIT_TEST(test_load_cmd_log_rotate_filename);
    CPPUNIT_TEST(test_load_cfg_log_rotate_filename);
    CPPUNIT_TEST(test_load_cmd_log_rotate_all_files_max_size);
    CPPUNIT_TEST(test_load_cfg_log_rotate_all_files_max_size);
    CPPUNIT_TEST(test_load_cmd_log_rotate_min_free_space);
    CPPUNIT_TEST(test_load_cfg_log_rotate_min_free_space);
    CPPUNIT_TEST(test_load_cmd_mode_proxy_local_port);
    CPPUNIT_TEST(test_load_cfg_mode_proxy_local_port);
    CPPUNIT_TEST(test_load_cfg_mode_proxy_local_port_multiple);
    CPPUNIT_TEST(test_load_cmd_mode_proxy_remote_port);
    CPPUNIT_TEST(test_load_cfg_mode_proxy_remote_port);
    CPPUNIT_TEST(test_load_cfg_mode_proxy_remote_port_multiple);
    CPPUNIT_TEST(test_load_cmd_mode_proxy_local_host);
    CPPUNIT_TEST(test_load_cfg_mode_proxy_local_host);
    CPPUNIT_TEST(test_load_cfg_mode_proxy_local_host_multiple);
    CPPUNIT_TEST(test_load_cmd_mode_proxy_remote_host);
    CPPUNIT_TEST(test_load_cfg_mode_proxy_remote_host);
    CPPUNIT_TEST(test_load_cfg_mode_proxy_remote_host_multiple);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void test_Configuration_ctor();
    void test_ctor();
    void test_false();
    void test_help();
    void test_version();
    void test_generate();
    void test_show_options();

    void test_load_cmd_mode();
    void test_load_cfg_mode();
    void test_load_cmd_log_silent();
    void test_load_cfg_log_silent();
    void test_load_cmd_log_nofile();
    void test_load_cfg_log_nofile();
    void test_load_cmd_log_directory();
    void test_load_cfg_log_directory();
    void test_load_cmd_log_filename();
    void test_load_cfg_log_filename();
    void test_load_cmd_log_format();
    void test_load_cfg_log_format();
    void test_load_cmd_log_severity_console();
    void test_load_cfg_log_severity_console();
    void test_load_cmd_log_severity_file();
    void test_load_cfg_log_severity_file();
    void test_load_cmd_log_rotate();
    void test_load_cfg_log_rotate();
    void test_load_cmd_log_rotate_size();
    void test_load_cfg_log_rotate_size();
    void test_load_cmd_log_rotate_filename();
    void test_load_cfg_log_rotate_filename();
    void test_load_cmd_log_rotate_all_files_max_size();
    void test_load_cfg_log_rotate_all_files_max_size();
    void test_load_cmd_log_rotate_min_free_space();
    void test_load_cfg_log_rotate_min_free_space();
    void test_load_cmd_mode_proxy_local_port();
    void test_load_cfg_mode_proxy_local_port();
    void test_load_cfg_mode_proxy_local_port_multiple();
    void test_load_cmd_mode_proxy_remote_port();
    void test_load_cfg_mode_proxy_remote_port();
    void test_load_cfg_mode_proxy_remote_port_multiple();
    void test_load_cmd_mode_proxy_local_host();
    void test_load_cfg_mode_proxy_local_host();
    void test_load_cfg_mode_proxy_local_host_multiple();
    void test_load_cmd_mode_proxy_remote_host();
    void test_load_cfg_mode_proxy_remote_host();
    void test_load_cfg_mode_proxy_remote_host_multiple();
};

#endif // MCT_TESTS_CONFIGURATION_TEST_CONFIGURATION_HPP
