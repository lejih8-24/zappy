/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ParseException class
*/

#pragma once

#include "BaseException.hpp"


namespace Zappy::Exceptions {
   class ParseException : public BaseException {
       public:
           using BaseException::BaseException;
   };
}
