/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI server message state
*/

#pragma once

#include <string>

namespace GUI {

enum class ServerMessageType {
    Info,
    Broadcast,
    Error,
    EndGame,
};

/*
** Related server commands:
** - pbc #n M (player broadcast)
** - smg M (server message)
** - suc (unknown command)
** - sbp (bad parameter)
** - seg N (end of game)
*/
class ServerMessage {
    public:
        ServerMessageType type = ServerMessageType::Info;
        std::string msg;
        int playerId = -1;
        float timestamp = 0.0f;

    public:
        ServerMessage(ServerMessageType type = ServerMessageType::Info, const std::string &msg = "",
            int playerId = -1, float timestamp = 0.0f);
        ~ServerMessage() = default;
};

}
