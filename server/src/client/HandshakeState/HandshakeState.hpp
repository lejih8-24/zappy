/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** HandshakeState class
*/

#pragma once

#include "../BaseState.hpp"
#include <string_view>
#include <string>


namespace Zappy::Client {
    class HandshakeState : public BaseState {
        static constexpr std::string_view WELCOME_MESSAGE = "WELCOME\n";

        std::string m_TeamName;
        bool m_WelcomeSent;

        public:
            HandshakeState();

            void update(Client& client, std::chrono::nanoseconds dt) override;
    };
}
