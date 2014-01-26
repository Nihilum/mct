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
 * @file ModeProxy/ModeProxy.cpp
 *
 * @desc ModeProxy class which is one of the possible program runtime modes.
 */


#include <memory>
#include <cstdlib>
#include <cstddef>

#include <Logger/Logger.hpp>
#include <Configuration/Configuration.hpp>

#include <ModeProxy/ModeProxy.hpp>
#include <ModeProxy/ProxyManager.hpp>
#include <ModeProxy/ProxyListener.hpp>

namespace mct
{

ModeProxy::ModeProxy(Configuration& config, Logger& logger) : Mode(config, logger)
{
}

ModeProxy::~ModeProxy()
{
}

const std::string& ModeProxy::get_name() const
{
    static std::string proxy_name("proxy");
    return proxy_name;
}

bool ModeProxy::run()
{
    m_log.log_if_not_silent("Initialized mode '%s'.", get_name().c_str());

    // provides the core I/O functionality (OS calls etc.)
    boost::asio::io_service ios;

    ProxyManager manager(m_log);

    // const uint16_t all_proxies = m_config.get_mode_proxy_all_proxies();
    const uint16_t all_proxies = 1;

    {
        //HostResolver resolver(m_log);

        for (uint16_t proxy_num = 0; proxy_num < all_proxies; ++proxy_num) {
            std::string local_interface = m_config.get_mode_proxy_local_host();
            uint16_t local_port = m_config.get_mode_proxy_local_port();
            std::string remote_host = m_config.get_mode_proxy_remote_host();
            uint16_t remote_port = m_config.get_mode_proxy_remote_port();

            // local_interface = resolver.resolve(local_interface);
            // remote_host = resolver.resolve(remote_host);

            manager.add_listener(std::make_shared<ProxyListener>(ios, m_log, local_interface, local_port, remote_host, remote_port));
        }
    }

    // gives control away to Boost.Asio to asynchronously handle connections
    ios.run();

    return true;
}

}
