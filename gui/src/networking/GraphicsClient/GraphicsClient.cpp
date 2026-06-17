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
    , m_ResponseBuffer()
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

auto Zappy::Networking::GraphicsClient::mapSize() -> Position
{
    send("msz\n");

    std::string_view response = getline();
    return ResponseParser::parseMapSize(response);
}

auto Zappy::Networking::GraphicsClient::tileContents(coordinate x, coordinate y) -> TileContents
{
    std::string msg = "bct ";
    msg += std::to_string(x) + ' ' + std::to_string(y) + '\n';
    send(msg);

    std::string_view response = getline();
    return ResponseParser::parseTileContents(response);
}

void Zappy::Networking::GraphicsClient::mapContents()
{

}

const std::vector<std::string>& Zappy::Networking::GraphicsClient::teamNames()
{
    // TODO: fetch team names
    return m_TeamNames;
}

auto Zappy::Networking::GraphicsClient::playerPosition(unsigned int playerId) -> PlayerPosition
{
    return { 0, { 0, 0 }, 0.f };
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

    send("GRAPHIC\n");

    line = getline(false);
    while (!line.empty()) {
        line = getline(false);
    }
}

std::string_view Zappy::Networking::GraphicsClient::getline(bool wait)
{
    m_ResponseBuffer.popLine();

    auto bufferLine = m_ResponseBuffer.getLine();
    if (bufferLine)
        return *bufferLine;

    if (!wait) {
        pollfd pollEvents = { m_Server.fileno(), POLL_IN, 0 };

        if (::poll(&pollEvents, 1, 100) < 0)
            return std::string_view();

        if (!(pollEvents.revents & POLL_IN))
            return std::string_view();
    }

    std::array<char, 1024> buffer;
    auto bytesRead = m_Server.read(buffer, buffer.size());
    if (bytesRead <= 0)
        return std::string_view();

    m_ResponseBuffer.pushBack(std::span(buffer.data(), bytesRead));
    bufferLine = m_ResponseBuffer.getLine();

    return bufferLine ? *bufferLine : std::string_view();
}

void Zappy::Networking::GraphicsClient::send(std::string_view msg)
{
    std::string sanitized;

    if (!msg.ends_with('\n')) {
        sanitized = msg;
        sanitized += '\n';
        msg = sanitized;
    }

    m_Server.write(msg);
}
