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
 * @file Application/Application.cpp
 *
 * @desc Class which encapsulates the program, moves the program initialisation responsibility
 *  out of main() to the library.
 */

#include <iostream>

#include <Application/Application.hpp>
#include <Configuration/ConfigurationBuilder.hpp>

namespace mct
{

Application::Application(int argc, char** argv)
 : m_config(argc, argv), m_log(m_config)
{
}

bool Application::init_configuration(std::string& msg)
{
    ConfigurationBuilder config_builder(m_config);

    if (config_builder.build_configuration(msg) == false) {
        return false;
    }

    return true;
}

bool Application::init_logger(std::string& msg)
{
    return true;
}

int Application::run()
{
    // initialize configuration and logger in a separate scope
    {
        std::string message_to_user;

        if (init_configuration(message_to_user) == false) {
            std::cerr << "[Application::run()] init_configuration failed.\nError: " << message_to_user << std::endl;
            return 1;
        }

        // Display standard message or error
        std::cout << message_to_user << std::endl;

        // The program has been run using either -h, -v or -g options, exit here.
        if (m_config.get_app_mode() == "configuration_info_special_mode") {
            return 0;
        }

        if (init_logger(message_to_user) == false) {
            std::cerr << "[Application::run()] init_logger failed.\nError: " << message_to_user << std::endl;
            return 1;
        }
    }

    return 0;
}

}
