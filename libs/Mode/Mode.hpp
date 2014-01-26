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
 * @file Mode/Mode.hpp
 *
 * @desc Abstract class used as a base for all possible program modes.
 */

#ifndef MCT_MODE_MODE_HPP
#define MCT_MODE_MODE_HPP

#include <string>

#include <Mode/Config.hpp>

namespace mct
{

class Configuration;
class Logger;

class MCT_MODE_DLL_PUBLIC Mode
{
public:
    Mode(Configuration& config, Logger& logger);
    virtual ~Mode();

    Mode(const Mode&) = delete;
    Mode& operator=(const Mode&) = delete;

    virtual const std::string& get_name() const = 0;

    virtual bool run() = 0;

protected:
    Configuration& m_config;
    Logger& m_log;
};

}

#endif // MCT_MODE_MODE_HPP
