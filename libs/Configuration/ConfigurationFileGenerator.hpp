/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Mateusz Kolodziejski
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
 * @file Configuration/ConfigurationFileGenerator.hpp
 *
 * @desc With supplying this class with proper entries, it can generate a config file with comments.
 */

#ifndef MCT_CONFIGURATION_CONFIGURATION_FILE_GENERATOR_HPP
#define MCT_CONFIGURATION_CONFIGURATION_FILE_GENERATOR_HPP

#include <string>
#include <vector>
#include <utility>

#include <Configuration/Config.hpp>

namespace mct
{

class MCT_CONFIGURATION_DLL_PUBLIC ConfigurationEntry
{
public:
	ConfigurationEntry(const std::string& name, const std::string& comment, const std::string& _default)
	 : m_name(name), m_comment(comment), m_default(_default)
	{
	}

	const std::string& get_name() const { return m_name; }
	const std::string& get_comment() const { return m_comment; }
	const std::string& get_default() const { return m_default; }

private:
	std::string m_name;
	std::string m_comment;
	std::string m_default;
};

class MCT_CONFIGURATION_DLL_PUBLIC ConfigurationFileGenerator
{
public:
    /**
     * Generates a configuration file according to stored entries.
     *
     * return:
     * - string containing configuration file
     */
	std::string generate();

	/**
	 * Adds configuration entry which will be used to generate the config file.
	 *
	 */
	void add_entry(const ConfigurationEntry& entry);

private:
	std::vector<ConfigurationEntry> m_entries;
};

}

#endif // MCT_CONFIGURATION_CONFIGURATION_FILE_GENERATOR_HPP
