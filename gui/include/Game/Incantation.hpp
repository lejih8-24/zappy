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

        int getLevel() const { return _level; }
        void setLevel(int level) { _level = level; }

        const std::vector<int> &getPlayerIds() const { return _playerIds; }
        void setPlayerIds(const std::vector<int> &playerIds) { _playerIds = playerIds; }

        bool isActive() const { return _active; }
        void setActive(bool active) { _active = active; }

        bool isSuccess() const { return _success; }
        void setSuccess(bool success) { _success = success; }

    private:
        int _level = 0;
        std::vector<int> _playerIds; //* voir si ça serait pas mieux des ref aux players
        bool _active = false;
        bool _success = false;
};

}
