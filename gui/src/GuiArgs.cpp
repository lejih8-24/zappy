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
    stream << "USAGE: ./zappy_gui -p port [-h machine] [--pack name] [--width W] [--height H]\n";
}

static int parseIntArg(std::string_view value, const char *name, int min, int max)
{
    int v = 0;
    const auto *begin = value.data();
    const auto *end = begin + value.size();
    const auto result = std::from_chars(begin, end, v);

    if (result.ec != std::errc() || result.ptr != end)
        throw ArgsException(std::string("invalid ") + name + ": must be a number");
    if (v < min || v > max)
        throw ArgsException(std::string("invalid ") + name + ": must be between "
            + std::to_string(min) + " and " + std::to_string(max));
    return v;
}

int GuiArgs::parsePort(std::string_view value)
{
    return parseIntArg(value, "port", 1, 65535);
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
        if (option == "--pack") {
            args._pack = std::string(requireValue(argc, argv, i));
            continue;
        }
        if (option == "--width") {
            args._width = parseIntArg(requireValue(argc, argv, i), "width", 320, 7680);
            continue;
        }
        if (option == "--height") {
            args._height = parseIntArg(requireValue(argc, argv, i), "height", 240, 4320);
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

const std::string &GuiArgs::getPack() const
{
    return _pack;
}

int GuiArgs::getWidth() const
{
    return _width;
}

int GuiArgs::getHeight() const
{
    return _height;
}

void GuiArgs::connect() const
{
    //toAdd when server lib is ready
}

}
