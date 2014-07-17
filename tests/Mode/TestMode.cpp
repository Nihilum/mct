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
 * @file tests/Mode/TestMode.cpp
 *
 * @desc Mode abstract class tests.
 */

#include <memory>
#include <vector>

#include <boost/filesystem.hpp>

#include <Mode/Mode.hpp>
#include <Logger/Logger.hpp>
#include <Configuration/Configuration.hpp>
#include <Configuration/ConfigurationBuilder.hpp>

#include "TestMode.hpp"

void TestMode::setUp()
{
}

void TestMode::tearDown()
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

class ModeSample : public mct::Mode
{
public:
    ModeSample(mct::Configuration& config, mct::Logger& logger) : mct::Mode(config, logger)
    {
    }

    virtual ~ModeSample() {}

    ModeSample(const ModeSample&) = delete;
    ModeSample& operator=(const ModeSample&) = delete;

    virtual const std::string& get_name() const
    {
        static std::string sample_name("sample");
        return sample_name;
    }

    virtual bool run()
    {
        return true;
    }
};

void TestMode::test_mode_sample()
{
    std::string filename("./tm_mode_sample.cfg");
    bool expected_value = true;
    std::string expected_message("Mattsource's Connection Tunneler v. 0.1.0-dev");
    std::string message_to_user;
    const bool expected_return_value = true;

    const int argc = 3;
    const char* argv[argc] = { "mct", "-c", filename.c_str()};

    ConfigFileReaderHelper helper(filename, {"log.nofile = 1", "log.silent = 1", "mode = sample"}, argc, argv);

    CPPUNIT_ASSERT_EQUAL_MESSAGE(message_to_user, expected_return_value, helper.read_file(message_to_user));
    CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);

    message_to_user.clear();
	expected_message.clear();

	{
		mct::Logger logger(helper.get_config());
		CPPUNIT_ASSERT_EQUAL(expected_return_value, logger.initialize(message_to_user));
		CPPUNIT_ASSERT_EQUAL(expected_message, message_to_user);

		std::unique_ptr<mct::Mode> app_mode(new ModeSample(helper.get_config(), logger));

		CPPUNIT_ASSERT_EQUAL(false, !app_mode);
		CPPUNIT_ASSERT_EQUAL(std::string("sample"), app_mode->get_name());
		CPPUNIT_ASSERT_EQUAL(true, app_mode->run());
	}
}
