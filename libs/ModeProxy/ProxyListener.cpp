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
 * @file ModeProxy/ProxyListener.cpp
 *
 * @desc ProxyListener listens on a given interface and starts Proxy sessions when connection is accepted.
 */

#include <thread>

#include <functional>

#include <boost/asio/basic_socket_acceptor.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <Logger/Logger.hpp>
#include <ModeProxy/Proxy.hpp>
#include <ModeProxy/ProxyListener.hpp>

namespace mct
{

ProxyListener::ProxyListener(boost::asio::io_service& ios, Logger& logger, const std::string& listen_host, uint16_t listen_port, const std::string& remote_host, uint16_t remote_port)
: m_ios(ios), m_log(logger), m_listen_host(listen_host), m_listen_port(listen_port), m_remote_host(remote_host), m_remote_port(remote_port), m_is_dead(false),
  m_acceptor(new boost::asio::ip::tcp::acceptor(m_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(m_listen_host), m_listen_port)))
{
	m_log.debug("Creating listener %s:%u.", m_listen_host.c_str(), m_listen_port);
}

ProxyListener::~ProxyListener()
{
	m_log.info("Releasing listener %s:%u.", get_listen_host().c_str(), get_listen_port());
}

void ProxyListener::async_listen()
{
	m_next_session = create_session();
	m_acceptor->async_accept(*m_next_session->get_client_socket(), std::bind(&ProxyListener::handle_accept, shared_from_this(), std::placeholders::_1));
}

std::shared_ptr<Proxy> ProxyListener::create_session()
{
	std::lock_guard<std::mutex> lock(m_sessions_access);
	m_sessions.push_back(std::make_shared<Proxy>(m_log, m_ios, get_remote_host(), get_remote_port()));
	return m_sessions.back();
}

void ProxyListener::handle_accept(const boost::system::error_code& error)
{
	if (!error) {
		m_next_session->start(get_listen_host(), get_listen_port());
		async_listen();
	} else {
		m_log.error("Listener at %s:%u which redirects to %s:%u could not accept connection. No more connections will be accepted by this listener. Error: %s",
			         get_listen_host().c_str(), get_listen_port(), get_remote_host().c_str(), get_remote_port(), error.message().c_str());
		m_is_dead = true;
	}
}

void ProxyListener::remove_dead_sessions()
{
	std::lock_guard<std::mutex> lock(m_sessions_access);

	for (auto it = m_sessions.begin(); it != m_sessions.end(); ++it) {
#if 0		
		m_log.debug("\tremove_dead_sessions(), processing %s:%u. use_count(): %u",
			(*it)->get_client_host().c_str(), (*it)->get_client_port(), (*it).use_count());
#endif

		if ((*it)->has_started() && !(*it)->get_client_socket()->is_open() && !(*it)->get_remote_socket()->is_open() && (*it).unique()) {
			m_log.warning("Removing dead session %s:%u.", (*it)->get_client_host().c_str(), (*it)->get_client_port());
			it = m_sessions.erase(it);
		}
	}
}

}
