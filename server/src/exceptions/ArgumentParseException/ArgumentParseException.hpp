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
        public:
            ArgumentParseException(std::string_view progName, std::string_view msg);
    };
}
