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
#include <sstream>
#include <cstdlib>
#include <cstddef>
#include <algorithm>

#include <Logger/Logger.hpp>
#include <Configuration/Configuration.hpp>

#include <Asio/io_service.hpp>
#include <Asio/ip/tcp.hpp>
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

bool ModeProxy::validate_configuration() const
{
    if (m_config.get_mode_proxy_local_hosts().size() != m_config.get_mode_proxy_remote_hosts().size() != m_config.get_mode_proxy_local_ports().size() != m_config.get_mode_proxy_remote_ports().size()) {
        uint16_t lh = m_config.get_mode_proxy_local_hosts().size();
        uint16_t rh = m_config.get_mode_proxy_remote_hosts().size();
        uint16_t lp = m_config.get_mode_proxy_local_ports().size();
        uint16_t rp = m_config.get_mode_proxy_remote_ports().size();
        uint16_t max = (std::max)({lh, rh, lp, rp});

        auto report_conf_problem = [&](const std::string& conf_field, uint16_t expected_val, uint16_t actual_val) {
            m_log.fatal("There is a problem with the configuration field '%s'. Since it's a set, it should have %d entries (repeats) - while it only has %d.", conf_field.c_str(), expected_val, actual_val);
        };

        if (lh != max) {
            report_conf_problem("mode_proxy_local_hosts", lh, max);
        }

        if (rh != max) {
            report_conf_problem("mode_proxy_remote_hosts", rh, max);
        }

        if (lp != max) {
            report_conf_problem("mode_proxy_local_ports", lp, max);
        }

        if (rp != max) {
            report_conf_problem("mode_proxy_remote_ports", rp, max);
        }

        return false;
    }

    for (auto&& port : m_config.get_mode_proxy_local_ports()) {
        if (port <= 1023) {
            m_log.warning("One of supplied mode_proxy_local_ports: %d is a 'well-known port' (its value is <= 1023). It means that the program might need additional privileges to run correctly.", port);
        }
    }

    return true;
}

uint16_t ModeProxy::get_num_of_all_proxies() const
{   // since all vectors are equal (checked with validate_configuration()), return the size of the first one
    return m_config.get_mode_proxy_local_hosts().size();
}

bool ModeProxy::run()
{
    m_log.log_if_not_silent("Initialized mode '%s'.", get_name().c_str());

    if (!validate_configuration()) {
        return false;
    }

    // provides the core I/O functionality (OS calls etc.)
    boost::asio::io_service ios;

    ProxyManager manager(m_log);
    {
        boost::asio::ip::tcp::resolver resolver(ios);

        const uint16_t num_of_all_proxies = get_num_of_all_proxies();

        for (uint16_t proxy_num = 0; proxy_num < num_of_all_proxies; ++proxy_num) {
            std::string local_interface = m_config.get_mode_proxy_local_hosts()[proxy_num];
            uint16_t local_port = m_config.get_mode_proxy_local_ports()[proxy_num];
            std::string remote_host = m_config.get_mode_proxy_remote_hosts()[proxy_num];
            uint16_t remote_port = m_config.get_mode_proxy_remote_ports()[proxy_num];

            boost::asio::ip::tcp::resolver::query query_local(local_interface, "");
            auto i = resolver.resolve(query_local);
            boost::asio::ip::tcp::endpoint iend = *i;
            std::string local_ip = iend.address().to_string();

            m_log.debug("Resolved local_ip: %s from local_interface: %s.", local_ip.c_str(), local_interface.c_str());

            boost::asio::ip::tcp::resolver::query query_remote(remote_host, "");
            i = resolver.resolve(query_remote);
            boost::asio::ip::tcp::endpoint jend = *i;
            std::string remote_ip = jend.address().to_string();

            m_log.debug("Resolved remote_ip: %s from remote_host: %s.", remote_ip.c_str(), remote_host.c_str());

            try {
                manager.add_listener(std::make_shared<ProxyListener>(ios, m_log, local_ip, local_port, remote_ip, remote_port));
            } catch (const boost::system::system_error& e) {
                std::stringstream sStr;
                sStr << "Cannot start listener using given address and port: (" << local_interface << ") " << local_ip << ":" << local_port << std::endl;
                sStr << "Error code: " << e.code().value() << std::endl;
                sStr << "System message: " << e.what() << std::endl;
                throw std::runtime_error(sStr.str());
            }
        }
    }

    // gives control away to Boost.Asio to asynchronously handle connections
    ios.run();

    return true;
}

}
