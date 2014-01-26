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

#ifndef MCT_MODEFACTORY_MODEFACTORY_HPP
#define MCT_MODEFACTORY_MODEFACTORY_HPP

#include <string>

#include <ModeFactory/Config.hpp>

namespace mct
{

class Configuration;
class Logger;
class Mode;

class MCT_MODEFACTORY_DLL_PUBLIC ModeFactory
{
public:
	ModeFactory(Configuration& config, Logger& logger);
	~ModeFactory();

    ModeFactory(const ModeFactory&) = delete;
    ModeFactory& operator=(const ModeFactory&) = delete;

	Mode* create(const std::string& mode);

private:
	Configuration& m_config;
	Logger& m_log;
};

}

#endif // MCT_MODEFACTORY_MODEFACTORY_HPP
