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
 * @file Configuration/Configuration.cpp
 *
 * @desc Program options are kept here.
 */

#include <Configuration/Configuration.hpp>

namespace mct
{

#if defined(WIN32)
const std::string Configuration::default_config_filename = "mct.cfg";
#else
const std::string Configuration::default_config_filename = "/etc/mct.cfg";
#endif

Configuration::Configuration(int argc, char** argv)
 : m_argc(argc), m_argv(argv), m_app_name(m_argv[0]),
 m_log_silent(false), m_log_nofile(false), m_log_rotate(false),
 m_log_rotate_size(0), m_log_rotate_all_files_max_size(0), m_log_rotate_min_free_space(0)
{
}

Configuration::~Configuration()
{
	m_argc = 0;
	m_argv = nullptr;
}

}
