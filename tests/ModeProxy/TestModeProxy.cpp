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
 * @file tests/ModeProxy/TestModeProxy.cpp
 *
 * @desc ModeProxy application mode tests.
 */

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

#include <Mode/Mode.hpp>
#include <Logger/Logger.hpp>
#include <ModeFactory/ModeFactory.hpp>
#include <Configuration/Configuration.hpp>
#include <Configuration/ConfigurationBuilder.hpp>

#include "TestModeProxy.hpp"

void TestModeProxy::setUp()
{
}

void TestModeProxy::tearDown()
{
}

class ConfigFileReaderHelper
{
public:
    ConfigFileReaderHelper(const std::string& filename, const std::vector<std::string>& keys_values, const int argc, const char** argv)
    : m_config(argc, (char**)argv), m_filename(filename), m_keys_values(keys_values), m_argc(argc), m_argv(argv)
    {
    }

    bool read_file(std::string& message_to_user)
    {
        std::ofstream fs;

        std::shared_ptr<std::ofstream> fileGuard(&fs, [&](std::ofstream*)
        {
            boost::filesystem::remove(m_filename);
        });

        fs.open(m_filename);
        for (auto& keys_values : m_keys_values) {
            fs << "#" << std::endl;
            fs << "# Standard comment support" << std::endl;
            fs << "#" << std::endl;
            fs << keys_values << std::endl << std::endl;
        }
        fs.close();

        mct::ConfigurationBuilder config_builder(m_config);

        return config_builder.build_configuration(message_to_user);
    }

    mct::Configuration& get_config() { return m_config; }

private:
    mct::Configuration m_config;
    std::string m_filename;
    std::vector<std::string> m_keys_values;
    const int m_argc;
    const char** m_argv;
};

void TestModeProxy::test_modeproxy_error_local_port_already_bound()
{
    std::string filename("./tmp_modeproxy_error_local_port_already_bound.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str()};

    ConfigFileReaderHelper helper(filename, 
        { 
            "log.nofile = 1",
            //"log.silent = 1",
            "mode = proxy",
            "mode.proxy.local_port = 0",
            "mode.proxy.local_host = 0",
            "mode.proxy.remote_port = 0",
            "mode.proxy.remote_host = 0"
        }, 
    argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file(message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);

    message_to_user.clear();
	expected_message.clear();

	{
		mct::Logger logger(helper.get_config());
		CPPUNIT_ASSERT_EQUAL(expected_return_value, logger.initialize(message_to_user));
		CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);

		mct::ModeFactory mode_factory(helper.get_config(), logger);
		std::unique_ptr<mct::Mode> app_mode(mode_factory.create(helper.get_config().get_app_mode()));

		CPPUNIT_ASSERT_EQUAL(false, !app_mode);
		CPPUNIT_ASSERT_EQUAL(std::string("proxy"), app_mode->get_name());

		expected_message = "Local port already bound!";

		/*std::streambuf* prevstr = std::clog.rdbuf();
		std::ostringstream sStr;
		std::clog.rdbuf(sStr.rdbuf());*/

		bool app_result = false;

		try {
			app_result = app_mode->run();
		}
		catch (const std::exception& e) {
			//std::clog.rdbuf(prevstr);
			std::cerr << "Error: " << e.what() /*<< ". Log: " << sStr.str() */ << std::endl;
			return;
		}

		//std::clog.rdbuf(prevstr);

		CPPUNIT_ASSERT_EQUAL(false, app_result);
		//CPPUNIT_ASSERT_EQUAL(true, sStr.str().find(expected_message) != std::string::npos);
	}
}
