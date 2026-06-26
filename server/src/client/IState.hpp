/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** IState interface
*/

#pragma once

#include <type_traits>
#include <chrono>


namespace Zappy::Game {
    class Game;
}

namespace Zappy::Client {
    class Client;

    class IState {
        public:
            virtual ~IState() = default;

            virtual void init() = 0;

            virtual void setGame(Game::Game& game) = 0;
            virtual void update(Client& client, std::chrono::nanoseconds dt) = 0;
            virtual void disconnect(Game::Game& game) = 0;
    };

    template <typename T>
    concept StateType = std::is_base_of_v<IState, T>;
}
