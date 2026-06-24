/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** PlayerRunState class
*/

#pragma once

#include "../IState.hpp"


namespace Zappy::Client {
    class Client;

    class PlayerRunState : public IState {
        Client& m_Client;

        public:
            PlayerRunState(Client& client);

            void update(Tick elapsedTicks) override;
    };
}
