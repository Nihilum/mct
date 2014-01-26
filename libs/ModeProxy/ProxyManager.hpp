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
 * @file ModeProxy/ProxyManager.hpp
 *
 * @desc ProxyManager manages all registered listeners.
 */

#ifndef MCT_MODEPROXY_PROXYMANAGER_HPP
#define MCT_MODEPROXY_PROXYMANAGER_HPP

#include <mutex>
#include <memory>
#include <vector>
#include <thread>

namespace mct
{

class Logger;
class ProxyListener;

class ProxyManager
{
public:
	ProxyManager(Logger& logger);
	~ProxyManager();

	void add_listener(std::shared_ptr<ProxyListener> listener);

protected:
	void cleanup_listeners();

protected:
	Logger& m_log;

	std::mutex m_listeners_access;
	std::vector< std::shared_ptr<ProxyListener> > m_listeners;

	volatile bool m_cleanup_listeners_run;
	std::unique_ptr< std::thread > m_cleanup_listeners;
};

}

#endif // MCT_MODEPROXY_PROXYMANAGER_HPP
