/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ArgumentParseException class
*/

#pragma once

#include "../ParseException.hpp"


namespace Zappy::Exceptions {
    class ArgumentParseException : public ParseException {
        unsigned char m_Status;

        public:
            ArgumentParseException(std::string_view progName);
            ArgumentParseException(std::string_view progName, std::string_view msg);

            inline unsigned char status() const noexcept override { return m_Status; }
    };
}
