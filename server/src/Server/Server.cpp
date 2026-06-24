/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Server class
*/

#include "Server.hpp"
#include <zappy/exceptions.hpp>
#include <zappy/utils.hpp>
#include <iostream>


Zappy::Server::Server(std::string_view ip, std::uint16_t port)
    : Lattice::Server<Client>(ip, port)
{

}

Zappy::Server::Server(Server&& other)
    : Lattice::Server<Client>()
{
    swap(other);
}

std::optional<std::string> Zappy::Server::popEvent()
{
    if (m_EventQueue.empty())
        return {};

    std::optional<std::string> event = std::string();

    event->swap(m_EventQueue.back());
    m_EventQueue.pop_back();

    return event;
}

void Zappy::Server::onStart()
{
    Zappy::logger.info() << "starting server on " << hostname() << ":" << port() << std::endl;
}

void Zappy::Server::onShutdown()
{
    Zappy::logger.info() << "server shutting down..." << std::endl;
}

void Zappy::Server::onClientAccepted(const Client& client)
{
    Zappy::logger.info() << client.name() << ": connected" << std::endl;
}

void Zappy::Server::onClientDisconnected(const Client& client)
{
    Zappy::logger.info() << client.name() << ": disconnected" << std::endl;
}

void Zappy::Server::updateServer()
{
    Zappy::logger.debug() << "updating server" << std::endl;
}

void Zappy::Server::updateClient(Client& client)
{
    Zappy::logger.debug() << "updating " << client.name() << std::endl;
    client.update(m_DeltaTime);
}

Zappy::Server::Builder::Builder()
    : m_Hostname("127.0.0.1")
    , m_Port(0)
    , m_MapSize{ 0, 0 }
    , m_TickSpeed(100)
    , m_ClientsPerTeam(0)
    , m_TeamNames()
{

}

auto Zappy::Server::Builder::fromArguments(std::span<const char*> args) -> Builder&
{
    if (args.empty())
        return *this;

    std::string_view progName = args[0];
    args = args.subspan(1);

    while (!args.empty()) {
        args = parseOption(progName, args);
    }

    // TODO: ensure values are in correct range

    return *this;
}

auto Zappy::Server::Builder::setTeamNames(std::span<std::string_view> names) -> Builder&
{
    for (auto& name : names)
        addTeamName(name);

    return *this;
}

auto Zappy::Server::Builder::setTeamNames(std::span<const char*> names) -> Builder&
{
    for (auto& name : names)
        addTeamName(name);

    return *this;
}

auto Zappy::Server::Builder::build() -> Server
{
    return Server(m_Hostname, m_Port);
}

using std::string_literals::operator ""s;

/**
 * Note:
 * Assumes args is not empty and
 * contains null-terminated strings.
 */
std::span<const char*> Zappy::Server::Builder::parseOption(std::string_view progName, std::span<const char*> args)
{
    std::string_view option = args[0];
    args = args.subspan(1);

    if (!option.starts_with('-'))
        throw Exceptions::ArgumentParseException(
            progName,
            "invalid positional argument '" + std::string(option) + "'"
        );

    switch (option[1]) {
        case 'p': return parsePortOption(progName, args);
        case 'x': return parseMapWidthOption(progName, args);
        case 'y': return parseMapHeightOption(progName, args);
        case 'n': return parseTeamNamesOption(progName, args);
        case 'f': return parseTickSpeedOption(progName, args);
        case 'c': return parseMaxClientsOption(progName, args);

        default: throw Exceptions::ArgumentParseException(progName, "invalid option -- '"s + option[1] + "'");
    }
}

std::span<const char*> Zappy::Server::Builder::parsePortOption(std::string_view progName, std::span<const char*> args)
{
    if (args.empty() || args[0][0] == '-')
        throw Exceptions::ArgumentParseException(progName, "option -p requires an argument");

    std::uint16_t port;
    toInt(args[0], port);
    args = args.subspan(1);

    setPort(port);
    return args;
}

std::span<const char*> Zappy::Server::Builder::parseMapWidthOption(std::string_view progName, std::span<const char*> args)
{
    if (args.empty() || args[0][0] == '-')
        throw Exceptions::ArgumentParseException(progName, + "option -x requires an argument");

    std::uint32_t width;
    toInt(args[0], width);
    args = args.subspan(1);

    setMapSize(width, m_MapSize.second);
    return args;
}

std::span<const char*> Zappy::Server::Builder::parseMapHeightOption(std::string_view progName, std::span<const char*> args)
{
    if (args.empty() || args[0][0] == '-')
        throw Exceptions::ArgumentParseException(progName, "option -y requires an argument");

    std::uint32_t height;
    toInt(args[0], height);
    args = args.subspan(1);

    setMapSize(m_MapSize.first, height);
    return args;
}

std::span<const char*> Zappy::Server::Builder::parseTeamNamesOption(std::string_view progName, std::span<const char*> args)
{
    if (args.empty() || args[0][0] == '-')
        throw Exceptions::ArgumentParseException(progName, "option -n requires at least 1 argument");

    while (!args.empty() && args[0][0] != '-') {
        addTeamName(args[0]);
        args = args.subspan(1);
    }

    return args;
}

std::span<const char*> Zappy::Server::Builder::parseTickSpeedOption(std::string_view progName, std::span<const char*> args)
{
    if (args.empty() || args[0][0] == '-')
        throw Exceptions::ArgumentParseException(progName, "option -f requires an argument");

    std::uint32_t speed;
    toInt(args[0], speed);
    args = args.subspan(1);

    setTickSpeed(speed);
    return args;
}

std::span<const char*> Zappy::Server::Builder::parseMaxClientsOption(std::string_view progName, std::span<const char*> args)
{
    if (args.empty() || args[0][0] == '-')
        throw Exceptions::ArgumentParseException(progName, "option -c requires an argument");

    std::uint32_t maxClients;
    toInt(args[0], maxClients);
    args = args.subspan(1);

    setMaxClientsPerTeam(maxClients);
    return args;
}
