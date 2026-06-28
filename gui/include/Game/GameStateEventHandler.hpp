/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Apply networking events to the GUI game state
*/

#pragma once

#include "Game/GameState.hpp"
#include <networking.hpp>

namespace GUI {

class GameStateEventHandler : public Zappy::Networking::BaseEventHandler {
    public:
        GameStateEventHandler(GameState &state, float currentTime);

        using Zappy::Networking::BaseEventHandler::operator();

        void operator()(const Zappy::Networking::MapSize &event);
        void operator()(const Zappy::Networking::TileContents &event);
        void operator()(const Zappy::Networking::TeamName &event);
        void operator()(const Zappy::Networking::NewPlayerConnect &event);
        void operator()(const Zappy::Networking::PlayerPosition &event);
        void operator()(const Zappy::Networking::PlayerLevel &event);
        void operator()(const Zappy::Networking::PlayerInventory &event);
        void operator()(const Zappy::Networking::PlayerCollectResource &event);
        void operator()(const Zappy::Networking::PlayerDropResource &event);
        void operator()(const Zappy::Networking::PlayerDie &event);
        void operator()(const Zappy::Networking::PlayerBroadcast &event);
        void operator()(const Zappy::Networking::PlayerExpulsion &event);
        void operator()(const Zappy::Networking::PlayerLayEgg &event);
        void operator()(const Zappy::Networking::EggCreate &event);
        void operator()(const Zappy::Networking::EggConnect &event);
        void operator()(const Zappy::Networking::EggDie &event);
        void operator()(const Zappy::Networking::PlayerIncantationStart &event);
        void operator()(const Zappy::Networking::PlayerIncantationEnd &event);
        void operator()(const Zappy::Networking::ServerGetTime &event);
        void operator()(const Zappy::Networking::ServerSetTime &event);
        void operator()(const Zappy::Networking::ServerGameEnd &event);

    private:
        GameState &_state;
        float _currentTime;
};

}
