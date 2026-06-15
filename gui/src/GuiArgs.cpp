/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI CLI arguments
*/

#include "GuiArgs.hpp"
#include "exceptions/ArgsException.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

namespace GUI {

using ArgsException = Zappy::Exceptions::ArgsException;

void GuiArgs::printUsage(std::ostream &stream)
{
    stream << "USAGE: ./zappy_gui -p port [-h machine]\n";
}

int GuiArgs::parsePort(const std::string &value)
{
    std::size_t parsed = 0;
    int port = 0;

    try {
        port = std::stoi(value, &parsed);
    } catch (const std::exception &) {
        throw ArgsException("invalid port: must be a number");
    }
    if (parsed != value.size())
        throw ArgsException("invalid port: must be a number");
    if (port < 1 || port > 65535)
        throw ArgsException("invalid port: must be between 1 and 65535");
    return port;
}

std::string GuiArgs::requireValue(int argc, char **argv, int &index)
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
        const std::string option = argv[i];

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
        throw ArgsException("unknown argument: " + option);
    }
    if (args._port == 0)
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
