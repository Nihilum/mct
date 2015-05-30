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
 * @file ModeProxy/IPResolver.hpp
 *
 * @desc IPResolver can be used to translate a hostname to IP address (using DNS).
 */

#ifndef MCT_MODEPROXY_IPRESOLVER_HPP
#define MCT_MODEPROXY_IPRESOLVER_HPP

#include <string>

#include <ModeProxy/Config.hpp>

namespace boost
{
    namespace asio
    {
        class io_service;
    }
}

namespace mct
{

class Logger;
class IPResolverImpl;

class MCT_MODEPROXY_DLL_PUBLIC IPResolver
{
public:
    IPResolver(Logger& logger, boost::asio::io_service& ios);
    ~IPResolver();

    IPResolver(const IPResolver&) = delete;
    IPResolver& operator=(const IPResolver&) = delete;

    std::string resolve_only_first_ip(const std::string& address);

private:
    IPResolverImpl* m_pImpl;
};

}

#endif // MCT_MODEPROXY_IPRESOLVER_HPP
