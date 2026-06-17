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
    m_Server.connect(ip, port);
}

Zappy::Networking::GraphicsClient::GraphicsClient(GraphicsClient&& other)
    : GraphicsClient()
{
    swap(other);
}


auto Zappy::Networking::GraphicsClient::mapSize() -> Position
{
   return { 1, 1 };
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
