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
 * @file ModeProxy/IPResolver.cpp
 *
 * @desc IPResolver can be used to translate a hostname to IP address (using DNS).
 */

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <ModeProxy/IPResolver.hpp>
#include <Logger/Logger.hpp>

namespace mct
{

class IPResolverImpl
{
public:
    IPResolverImpl(Logger& logger, boost::asio::io_service& ios);

    std::string resolve_only_first_ip(const std::string& address);

private:
    Logger& m_log;
    boost::asio::io_service& m_ios;
    boost::asio::ip::tcp::resolver m_resolver;
};

IPResolver::IPResolver(Logger& logger, boost::asio::io_service& ios)
 : m_pImpl(new IPResolverImpl(logger, ios))
{
}

IPResolver::~IPResolver()
{
    delete m_pImpl;
    m_pImpl = nullptr;
}

std::string IPResolver::resolve_only_first_ip(const std::string& address)
{
    return m_pImpl->resolve_only_first_ip(address);
}

/***************************************************************************
 *
 * IMPLEMENTATION
 *
 **************************************************************************/

IPResolverImpl::IPResolverImpl(Logger& logger, boost::asio::io_service& ios)
 : m_log(logger), m_ios(ios), m_resolver(m_ios)
{
}

std::string IPResolverImpl::resolve_only_first_ip(const std::string& address)
{
    boost::asio::ip::tcp::resolver::query query_local(address, "");
    auto i = m_resolver.resolve(query_local);
    boost::asio::ip::tcp::endpoint iend = *i;
    std::string ip = iend.address().to_string();

    m_log.debug("Resolved ip: %s from address: %s.", ip.c_str(), address.c_str());

    return ip;
}

}
