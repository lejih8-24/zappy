/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ArgsException class
*/

#pragma once

#include <exception>
#include <string_view>
#include <string>


namespace Zappy::Exceptions {
    class ArgsException : public std::exception {
        std::string m_Message;

        public:
            ArgsException(std::string_view msg) : m_Message(msg) {}

            inline const char* what() const noexcept { return m_Message.c_str(); }
    };
}
