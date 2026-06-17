/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** GraphicsClient class
*/

#include "GraphicsClient.hpp"

Zappy::Networking::GraphicsClient::GraphicsClient()
    : m_Server()
    , m_TeamNames()
{
}

Zappy::Networking::GraphicsClient::GraphicsClient(std::string_view ip, std::uint16_t port)
    : GraphicsClient()
{
    m_Server.open();
    m_Server.connect(ip, port);
    doHandshake();
}

Zappy::Networking::GraphicsClient::GraphicsClient(GraphicsClient&& other)
    : GraphicsClient()
{
    swap(other);
}

#include <charconv>

auto Zappy::Networking::GraphicsClient::mapSize() -> Position
{
    send("msz");

    std::string_view response = getline();
    if (!response.starts_with("msz "))
        return { 0, 0 };

    std::string_view mapSize = response.substr(4);

    Position result = { 0, 0 };
    auto [ptr, ec] = std::from_chars(mapSize.begin(), mapSize.end(), result.x);
    std::from_chars(ptr + 1, mapSize.end(), result.y);

    return result;
}

void Zappy::Networking::GraphicsClient::tileContents(coordinate x, coordinate y)
{

}

void Zappy::Networking::GraphicsClient::mapContents()
{

}

const std::vector<std::string>& Zappy::Networking::GraphicsClient::teamNames()
{
    // TODO: fetch team names
    return m_TeamNames;
}

auto Zappy::Networking::GraphicsClient::playerPosition(unsigned int playerId) -> Position
{
    return { 1, 1 };
}

unsigned int Zappy::Networking::GraphicsClient::playerLevel(unsigned int playerId)
{
    return 0;
}

void Zappy::Networking::GraphicsClient::playerInventory(unsigned int playerId)
{

}

unsigned int Zappy::Networking::GraphicsClient::getTime()
{
    return 0;
}

auto Zappy::Networking::GraphicsClient::pollEvent() -> std::optional<int>
{
    return {};
}

void Zappy::Networking::GraphicsClient::swap(GraphicsClient& other)
{
    std::swap(m_Server, other.m_Server);
    std::swap(m_TeamNames, other.m_TeamNames);
}

void Zappy::Networking::GraphicsClient::doHandshake()
{
    std::string_view line = getline();
    if (line != "WELCOME")
        throw Lattice::Exceptions::SocketException("invalid response in zappy server handshake");

    send("GRAPHIC");

    line = getline(false);
    while (!line.empty()) {
        line = getline(false);
    }
}

std::string_view Zappy::Networking::GraphicsClient::getline(bool wait)
{
    // TODO: add caching to avoid losing data on cutoff
    constexpr std::size_t size = 4096;
    static char buffer[size];

    if (!wait) {
        pollfd pollEvents = { m_Server.fileno(), POLL_IN, 0 };

        if (::poll(&pollEvents, 1, -1) != 0)
            return std::string_view();

        if (!(pollEvents.revents & POLL_IN))
            return std::string_view();
    }

    auto bytesRead = m_Server.read(buffer, size);
    if (bytesRead <= 0)
        return std::string_view();

    std::string_view line(buffer, bytesRead);
    auto end = line.find('\n');

    return std::string_view(line.begin(), end);
}

void Zappy::Networking::GraphicsClient::send(std::string msg)
{
    if (!msg.ends_with('\n'))
        msg += '\n';

    m_Server.write(msg);
}
