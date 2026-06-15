/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI CLI arguments
*/

#include "GuiArgs.hpp"
#include "exceptions/ArgsException.hpp"

#include <charconv>
#include <cstdlib>
#include <iostream>
#include <string>
#include <system_error>

namespace GUI {

using ArgsException = Zappy::Exceptions::ArgsException;

void GuiArgs::printUsage(std::ostream &stream)
{
    stream << "USAGE: ./zappy_gui -p port [-h machine]\n";
}

int GuiArgs::parsePort(std::string_view value)
{
    int port = 0;
    const auto *begin = value.data();
    const auto *end = begin + value.size();
    const auto result = std::from_chars(begin, end, port);

    if (result.ec != std::errc() || result.ptr != end)
        throw ArgsException("invalid port: must be a number");
    if (port < 1 || port > 65535)
        throw ArgsException("invalid port: must be between 1 and 65535");
    return port;
}

std::string_view GuiArgs::requireValue(int argc, char **argv, int &index)
{
    if (index + 1 >= argc)
        throw ArgsException(std::string("missing value for ") + argv[index]);
    ++index;
    return argv[index];
}

GuiArgs GuiArgs::parseArgs(int argc, char **argv)
{
    GuiArgs args;

    for (int i = 1; i < argc; ++i) {
        const std::string_view option = argv[i];

        if (option == "--help") {
            printUsage(std::cout);
            std::exit(0);
        }
        if (option == "-p") {
            args._port = parsePort(requireValue(argc, argv, i));
            continue;
        }
        if (option == "-h") {
            args._host = requireValue(argc, argv, i);
            continue;
        }
        throw ArgsException(std::string("unknown argument: ").append(option));
    }
    if (args._port == -1)
        throw ArgsException("missing required argument: -p port");
    return args;
}

const std::string &GuiArgs::getHost() const
{
    return _host;
}

int GuiArgs::getPort() const
{
    return _port;
}

void GuiArgs::connect() const
{
    //toAdd when server lib is ready
}

}
