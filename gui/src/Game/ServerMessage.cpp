/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI server message state
*/

#include "Game/ServerMessage.hpp"

namespace GUI {

ServerMessage::ServerMessage(ServerMessageType type, const std::string &msg, int playerId, float timestamp)
    : type(type)
    , msg(msg)
    , playerId(playerId)
    , timestamp(timestamp)
{
}

}
