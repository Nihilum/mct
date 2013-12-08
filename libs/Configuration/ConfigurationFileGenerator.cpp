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

#include <string>
#include <sstream>
#include <algorithm>

#include <config.hpp>

#include <Configuration/ConfigurationFileGenerator.hpp>

namespace mct
{

std::string ConfigurationFileGenerator::generate()
{
	std::stringstream cfgFile;
	std::string newline;

	{
		std::stringstream tmpStr;
		tmpStr << std::endl;
		newline = tmpStr.str();
	}

	cfgFile << "# This file has been auto-generated using '-g' cmdline parameter" << std::endl;
	cfgFile << "# Version: " << std::string(MCT_VERSION) << " " << std::string(MCT_TAG) << std::endl;
	cfgFile << std::endl;

	std::for_each(m_entries.begin(), m_entries.end(), [&](const ConfigurationEntry& item){
		cfgFile << "#" << std::endl;

		std::string comment(item.get_comment());
		int iPos = comment.find(newline);

		while (iPos != std::string::npos) {
			comment.replace(iPos++, 1, newline + std::string("# "));
			iPos = comment.find(newline, iPos);
		}

		cfgFile << comment << std::endl;
		cfgFile << "#" << std::endl;
		cfgFile << "# Default: " << item.get_default() << std::endl << std::endl;
		cfgFile << "# " << item.get_name() << " =" << std::endl << std::endl;
	});

	std::string produced_string = cfgFile.str();
	produced_string.erase(produced_string.length() - 2, 2); // remove final endls

	return produced_string;
}

void ConfigurationFileGenerator::add_entry(ConfigurationEntry&& entry)
{
	m_entries.push_back(std::move(entry));
}

}
