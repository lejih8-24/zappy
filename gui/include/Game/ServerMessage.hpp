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
        ServerMessage(ServerMessageType type = ServerMessageType::Info, const std::string &msg = "",
            int playerId = -1, float timestamp = 0.0f);
        ~ServerMessage() = default;

        ServerMessageType getType() const { return _type; }
        void setType(ServerMessageType type) { _type = type; }

        const std::string &getMsg() const { return _msg; }
        void setMsg(const std::string &text) { _msg = text; }

        int getPlayerId() const { return _playerId; }
        void setPlayerId(int playerId) { _playerId = playerId; }

        float getTimestamp() const { return _timestamp; }
        void setTimestamp(float timestamp) { _timestamp = timestamp; }

    private:
        ServerMessageType _type = ServerMessageType::Info;
        std::string _msg;
        int _playerId = -1;
        float _timestamp = 0.0f;
};

}
