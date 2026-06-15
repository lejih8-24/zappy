/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI incantation state
*/

#pragma once

#include "Game/Entity.hpp"

#include <vector>

namespace GUI {

/*
** Related server commands:
** - pic X Y L #n #n ... (incantation start)
** - pie X Y R (incantation end)
**??? TODO: voir si c'est vrmt utile d'avoir une class pour encapsuler l'Incantation
*/
class Incantation : public Entity {
    public:
        Incantation(int id = 0, int x = 0, int y = 0, int level = 0, const std::vector<int> &playerIds = {});
        ~Incantation() = default;

        int level = 0;
        std::vector<int> playerIds; //* voir si ça serait pas mieux des ref aux players
        bool active = false;
        bool success = false;
};

}
