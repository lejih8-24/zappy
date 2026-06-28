/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** OutOfRange exception
*/

#pragma once

#include "BaseException.hpp"


namespace Zappy::Exceptions {
    class OutOfRange : public BaseException {
        public:
            using BaseException::BaseException;
    };
}
