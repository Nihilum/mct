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
 * @file ModeProxy/ModeProxy.hpp
 *
 * @desc ModeProxy class which is one of the possible program runtime modes.
 */

#ifndef MCT_MODEPROXY_MODEPROXY_HPP
#define MCT_MODEPROXY_MODEPROXY_HPP

#include <string>

#include <Mode/Mode.hpp>
#include <ModeProxy/Config.hpp>

namespace mct
{

class Configuration;
class Logger;

class MCT_MODEPROXY_DLL_PUBLIC ModeProxy : public Mode
{
public:
    ModeProxy(Configuration& config, Logger& logger);
    virtual ~ModeProxy();

    ModeProxy(const ModeProxy&) = delete;
    ModeProxy& operator=(const ModeProxy&) = delete;

    virtual const std::string& get_name() const;

    virtual bool run();
};

}

#endif // MCT_MODEPROXY_MODEPROXY_HPP
