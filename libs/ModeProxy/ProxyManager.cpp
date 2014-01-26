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
 * @file ModeProxy/ProxyManager.cpp
 *
 * @desc ProxyManager manages all registered listeners.
 */

#include <chrono>

#include <Logger/Logger.hpp>
#include <ModeProxy/ProxyManager.hpp>
#include <ModeProxy/ProxyListener.hpp>

namespace mct
{

ProxyManager::ProxyManager(Logger& logger)
 : m_log(logger), m_cleanup_listeners_run(true), m_cleanup_listeners(new std::thread(&ProxyManager::cleanup_listeners, this))
{
}

ProxyManager::~ProxyManager()
{
	m_cleanup_listeners_run = false;

	if (m_cleanup_listeners) {
		m_cleanup_listeners->join();
	}
}

void ProxyManager::add_listener(std::shared_ptr<ProxyListener> listener)
{
	m_log.info("Registering listener at %s:%u which will redirect to %s:%u.", listener->get_listen_host().c_str(), listener->get_listen_port(), listener->get_remote_host().c_str(), listener->get_remote_port());

	std::lock_guard<std::mutex> lock(m_listeners_access);
	m_listeners.push_back(listener);
	listener->async_listen();
}

void ProxyManager::cleanup_listeners()
{
	std::chrono::seconds sleep_duration(10);

	while (m_cleanup_listeners_run) {
		std::this_thread::sleep_for(sleep_duration);
		std::lock_guard<std::mutex> lock(m_listeners_access);

		for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it) {
#if 0
			m_log.debug("cleanup_listeners(), processing %s:%u. use_count(): %u", (*it)->get_listen_host().c_str(), (*it)->get_listen_port(), (*it).use_count());
#endif
			(*it)->remove_dead_sessions();

			if ((*it)->is_dead() && (*it).unique()) {
				m_log.info("Removing dead listener %s:%u.", (*it)->get_listen_host().c_str(), (*it)->get_listen_port());
				it = m_listeners.erase(it);
			}
		}
	}
}

}
