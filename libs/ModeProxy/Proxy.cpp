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
 * @file ModeProxy/Proxy.cpp
 *
 * @desc Proxy holds one session.
 */

#include <functional>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <Logger/Logger.hpp>
#include <ModeProxy/Proxy.hpp>

namespace mct
{

Proxy::Proxy(Logger& logger, boost::asio::io_service& ios, const std::string& remote_host, uint16_t remote_port)
 : m_log(logger), m_ios(ios), m_remote_host(remote_host), m_remote_port(remote_port), m_client_host("none"), m_client_port(0),
   m_client_socket(new boost::asio::ip::tcp::socket(m_ios)), m_remote_socket(new boost::asio::ip::tcp::socket(m_ios)), m_has_started(false)
{
}

Proxy::~Proxy()
{
	m_log.info("Releasing client %s:%u.", m_client_host.c_str(), m_client_port);
}

void Proxy::start(const std::string& listen_host, uint16_t listen_port)
{
	if (has_started()) {
		return;
	}

	m_has_started = true;
    m_client_host = m_client_socket->remote_endpoint().address().to_string();
    m_client_port = m_client_socket->remote_endpoint().port();

    m_log.info("Accepted client %s:%u with listener %s:%u. Redirecting connection to %s:%u.", m_client_host.c_str(), m_client_port, listen_host.c_str(), listen_port, m_remote_host.c_str(), m_remote_port);

	m_remote_socket->async_connect(
		boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(m_remote_host), m_remote_port),
		std::bind(&Proxy::handle_remote_connect, shared_from_this(), std::placeholders::_1)
	);
}

void Proxy::close()
{
	m_log.debug("Closing sockets for client %s:%u.", m_client_host.c_str(), m_client_port);

    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_client_socket->is_open()) {
        m_client_socket->close();
    }

    if (m_remote_socket->is_open()) {
        m_remote_socket->close();
    }
}

void Proxy::handle_remote_connect(const boost::system::error_code& error)
{
	if (!error) {
		m_log.warning("Tunnel for client %s:%u to remote endpoint %s:%u is now up and running.", m_client_host.c_str(), m_client_port, m_remote_host.c_str(), m_remote_port);

		m_remote_socket->async_read_some(
			boost::asio::buffer(m_remote_data, m_max_data_length),
			std::bind(&Proxy::handle_remote_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2)
		);

		m_client_socket->async_read_some(
			boost::asio::buffer(m_client_data, m_max_data_length),
			std::bind(&Proxy::handle_client_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2)
		);
    } else {
    	m_log.error("Cannot create tunnel for client %s:%u to remote endpoint %s:%u. Error: %s", m_client_host.c_str(), m_client_port, m_remote_host.c_str(), m_remote_port, error.message().c_str());
        close();
    }
}

void Proxy::handle_remote_read(const boost::system::error_code& error, const size_t& bytes_transferred)
{
    if (!error) {
    	m_log.debug("[Client %s:%u] Read %u bytes from remote endpoint.", m_client_host.c_str(), m_client_port, bytes_transferred);

        boost::asio::async_write(
        	*m_client_socket, boost::asio::buffer(m_remote_data, bytes_transferred),
        	std::bind(&Proxy::handle_client_write, shared_from_this(), std::placeholders::_1)
        );
    } else {
    	m_log.warning("Client %s:%u cannot read data from remote endpoint %s:%u, because: %s", m_client_host.c_str(), m_client_port, m_remote_host.c_str(), m_remote_port, error.message().c_str());
        close();
    }
}

void Proxy::handle_client_read(const boost::system::error_code& error, const size_t& bytes_transferred)
{
    if (!error) {
    	m_log.debug("[Client %s:%u] Read %u bytes from client endpoint.", m_client_host.c_str(), m_client_port, bytes_transferred);

        boost::asio::async_write(
        	*m_remote_socket, boost::asio::buffer(m_client_data, bytes_transferred),
        	std::bind(&Proxy::handle_remote_write, shared_from_this(), std::placeholders::_1)
        );
    } else {
    	m_log.warning("Client %s:%u cannot read data from client endpoint, because: %s", m_client_host.c_str(), m_client_port, error.message().c_str());
        close();
    }
}

void Proxy::handle_remote_write(const boost::system::error_code& error)
{
	if (!error) {
        m_client_socket->async_read_some(
            boost::asio::buffer(m_client_data, m_max_data_length),
            std::bind(&Proxy::handle_client_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2)
        );
    } else {
    	m_log.warning("Client %s:%u cannot write data to remote endpoint %s:%u, because: %s", m_client_host.c_str(), m_client_port, m_remote_host.c_str(), m_remote_port, error.message().c_str());
        close();
    }
}

void Proxy::handle_client_write(const boost::system::error_code& error)
{
	if (!error) {
        m_remote_socket->async_read_some(
            boost::asio::buffer(m_remote_data, m_max_data_length),
            std::bind(&Proxy::handle_remote_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2)
        );
    } else {
    	m_log.warning("Client %s:%u cannot write data to client endpoint, because: %s", m_client_host.c_str(), m_client_port, error.message().c_str());
        close();
    }
}

}
