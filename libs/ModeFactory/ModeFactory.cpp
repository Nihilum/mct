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
 * @file ModeFactory/ModeFactory.hpp
 *
 * @desc Factory of program modes, used to construct Mode objects.
 */

#include <Mode/Mode.hpp>
#include <Logger/Logger.hpp>
#include <ModeFactory/ModeFactory.hpp>
#include <Configuration/Configuration.hpp>

#include <ModeProxy/ModeProxy.hpp>

namespace mct
{

ModeFactory::ModeFactory(Configuration& config, Logger& logger) : m_config(config), m_log(logger)
{
}

ModeFactory::~ModeFactory()
{
}

Mode* ModeFactory::create(const std::string& mode)
{
	if (mode == "proxy") {
		return new ModeProxy(m_config, m_log);
	}

	return nullptr;
}

}
