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
 * @file ModeProxy/Proxy.hpp
 *
 * @desc Proxy holds one session.
 */

#ifndef MCT_MODEPROXY_PROXY_HPP
#define MCT_MODEPROXY_PROXY_HPP

#include <mutex>
#include <memory>
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
        class stream_socket_service;

        template <typename Protocol, typename StreamSocketService >
        class basic_stream_socket;
    }
}

namespace mct
{

class Logger;

class Proxy : public std::enable_shared_from_this<Proxy>
{
public:
    Proxy(Logger& logger, boost::asio::io_service& ios, const std::string& remote_host, uint16_t remote_port);
    ~Proxy();

    const std::unique_ptr< boost::asio::basic_stream_socket<boost::asio::ip::tcp> >& get_client_socket() const { return m_client_socket; }
    const std::unique_ptr< boost::asio::basic_stream_socket<boost::asio::ip::tcp> >& get_remote_socket() const { return m_remote_socket; }

    bool has_started() const { return m_has_started; }

    void start(const std::string& listen_host, uint16_t listen_port);
    void close();

    const std::string& get_client_host() const { return m_client_host; }
    const uint16_t get_client_port() const { return m_client_port; }

protected:
	void handle_remote_connect(const boost::system::error_code& error);
	void handle_remote_read(const boost::system::error_code& error, const size_t& bytes_transferred);
	void handle_client_read(const boost::system::error_code& error, const size_t& bytes_transferred);
	void handle_remote_write(const boost::system::error_code& error);
	void handle_client_write(const boost::system::error_code& error);

protected:
	Logger& m_log;
	boost::asio::io_service& m_ios;

	const std::string m_remote_host;
	const uint16_t m_remote_port;
	std::string m_client_host;
	uint16_t m_client_port;

    enum { m_max_data_length = 8192 }; //8KB
    unsigned char m_remote_data[m_max_data_length];
    unsigned char m_client_data[m_max_data_length];

    std::unique_ptr< boost::asio::basic_stream_socket<boost::asio::ip::tcp> > m_client_socket;
    std::unique_ptr< boost::asio::basic_stream_socket<boost::asio::ip::tcp> > m_remote_socket;

    bool m_has_started;
    std::mutex m_mutex;
};

}

#endif // MCT_MODEPROXY_PROXY_HPP
