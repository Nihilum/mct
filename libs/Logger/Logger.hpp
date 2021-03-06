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
 * @file Logger/Logger.hpp
 *
 * @desc Class handling console and file output.
 */

#ifndef MCT_LOGGER_LOGGER_HPP
#define MCT_LOGGER_LOGGER_HPP

#include <string>

#include <Logger/Config.hpp>

namespace mct
{

class Configuration;
class LoggerImpl;

class MCT_LOGGER_DLL_PUBLIC Logger
{
public:
    Logger(Configuration& config);
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    bool initialize(std::string& msg);

    void debug(const char* format, ...);
    void info(const char* format, ...);
    void warning(const char* format, ...);
    void error(const char* format, ...);
    void fatal(const char* format, ...);

    void log_if_not_silent(const char* format, ...);

private:
    LoggerImpl* m_pImpl;
};

}

#endif // MCT_LOGGER_LOGGER_HPP
