/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** SocketException class
*/

#pragma once

#include "BaseException.hpp"

namespace Zappy::Exceptions {
    class SocketException : public BaseException {
        public:
            using BaseException::BaseException;
    };
}
