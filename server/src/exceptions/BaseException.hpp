/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** BaseException class
*/

#pragma once

#include <string_view>
#include <string>


namespace Zappy::Exceptions {
    class BaseException : public std::exception {
        std::string m_Message;

        public:
            BaseException(std::string_view msg) : m_Message(msg) {}

            inline const char* what() const noexcept { return m_Message.c_str(); }
    };
}
