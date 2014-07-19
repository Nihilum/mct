/** 
 * @file
 *
 * @brief PortBlocker.cpp - small port blocker application used to run tests.
 */

/**
 * Copyright (C) 2014 by Mateusz Kolodziejski (MattSource).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <iostream>
#include <chrono>
#include <thread>

#include <Asio/ip/tcp.hpp>
#include <boost/lexical_cast.hpp>

#include "PortBlocker.hpp"

void PortBlocker::block_port(const uint16_t port)
{
	std::cout << "[PortBlocker] started - port: " << port << std::endl;

	while (true) {
		run_acceptor(port);
	}
}

void PortBlocker::run_acceptor(const uint16_t port)
{
	try {
		tcp::acceptor acceptor(m_ios, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port), false);

/*#ifdef WIN32
		int iOptval = 1;
		int iResult = setsockopt(acceptor.native(), SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char *)&iOptval, sizeof (iOptval));
		if (iResult == SOCKET_ERROR) {
		std::cout << "setsockopt for SO_EXCLUSIVEADDRUSE failed with error: " << WSAGetLastError() << std::endl;
		exit(0);
		}
#endif*/

		for (;;) {
    		tcp::socket socket(m_ios);
    		acceptor.accept(socket);
    	}
  	} catch (const std::exception& /*e*/) {
  		std::this_thread::sleep_for(std::chrono::seconds(1));
  	}
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "[PortBlocker] Usage: " << argv[0] << " <port>" << std::endl;
		return 1;
	}

	const uint16_t port = boost::lexical_cast<uint16_t>(argv[1]);

	std::cout << "[PortBlocker] Starting" << std::endl;
	PortBlocker pb;
    pb.block_port(port);
}
