/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ServerException class
*/

#pragma once

#include <string_view>
#include <string>


namespace Zappy::Exceptions {
    class ServerException {
        std::string m_Message;

        public:
            ServerException(std::string_view msg) : m_Message(msg) {}

            inline const char* what() const noexcept { return m_Message.c_str(); }
    };
}
