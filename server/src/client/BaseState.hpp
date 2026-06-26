/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** BaseState class
*/

#pragma once

#include "IState.hpp"


namespace Zappy::Client {
    class BaseState : public IState {
        Game::Game* m_Game;

        public:
            constexpr BaseState() noexcept : m_Game(nullptr) {}

            inline void init() override {}
            inline void setGame(Game::Game& game) override { m_Game = &game; }
            inline void disconnect(Game::Game&) override {}

        protected:
            inline Game::Game& game() noexcept { return *m_Game; }
            inline const Game::Game& game() const noexcept { return *m_Game; }
    };
}
