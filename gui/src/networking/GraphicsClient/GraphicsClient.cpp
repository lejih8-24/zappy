/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** GraphicsClient class
*/

#include "GraphicsClient.hpp"


Zappy::Networking::GraphicsClient::GraphicsClient(std::string_view ip, std::uint16_t port)
    : m_Server()
    , m_TeamNames()
{

}

auto Zappy::Networking::GraphicsClient::mapSize() -> Position
{
   return { 1, 1 };
}
