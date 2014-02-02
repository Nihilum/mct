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
 * @file ModeProxy/ProxyListener.hpp
 *
 * @desc ProxyListener listens on a given interface and starts Proxy sessions when connection is accepted.
 */

#ifndef MCT_MODEPROXY_PROXYLISTENER_HPP
#define MCT_MODEPROXY_PROXYLISTENER_HPP

#include <mutex>
#include <memory>
#include <vector>
#include <cstdint>

namespace boost
{
    namespace system
    {
        class error_code;
    }

    namespace asio
    {
        class io_service;

        namespace ip
        {
            class tcp;
        }

        template <typename Protocol>
        class socket_acceptor_service;

		template <typename Protocol, typename SocketAcceptorService = socket_acceptor_service<Protocol> >
		class basic_socket_acceptor;
    }
}

namespace mct
{

class Proxy;
class Logger;

class ProxyListener : public std::enable_shared_from_this<ProxyListener>
{
public:
	ProxyListener(boost::asio::io_service& ios, Logger& logger, const std::string& listen_host, uint16_t listen_port, const std::string& remote_host, uint16_t remote_port);
	~ProxyListener();

	void async_listen();

	const std::string& get_listen_host() const { return m_listen_host; }
	const uint16_t get_listen_port() const { return m_listen_port; }
	const std::string& get_remote_host() const { return m_remote_host; }
	const uint16_t get_remote_port() const { return m_remote_port; }

	bool is_dead() const { return m_is_dead; }

	void remove_dead_sessions();

protected:
	std::shared_ptr<Proxy> create_session();
	void handle_accept(const boost::system::error_code& error);

protected:
	boost::asio::io_service& m_ios;
	Logger& m_log;

	const std::string m_listen_host;
	const uint16_t m_listen_port;
	const std::string m_remote_host;
	const uint16_t m_remote_port;

	bool m_is_dead;

	std::unique_ptr< boost::asio::basic_socket_acceptor<boost::asio::ip::tcp> > m_acceptor;

	std::mutex m_sessions_access;
	std::vector< std::shared_ptr< Proxy > > m_sessions;
	std::shared_ptr<Proxy> m_next_session;
};

}

#endif // MCT_MODEPROXY_PROXYLISTENER_HPP
