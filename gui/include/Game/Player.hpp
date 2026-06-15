/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** GUI player state
*/

#pragma once

#include "Game/Entity.hpp"
#include "Game/Resource.hpp"

#include <string>

namespace GUI {

enum class Orientation {
    North = 1,
    East = 2,
    South = 3,
    West = 4,
};

/*
** Related server commands:
** - pnw #n X Y O L N (new player)
** - ppo #n X Y O (player position)
** - plv #n L (player level)
** - pin #n X Y q0 q1 q2 q3 q4 q5 q6 (player inventory)
** - pex #n (player expulsion)
** - pbc #n M (player broadcast)
** - pfk #n (egg laying start)
** - pdr #n i (resource drop)
** - pgt #n i (resource collect)
** - pdi #n (player death)
*/
class Player : public Entity {
    public:
        Player(int id = 0, int x = 0, int y = 0, Orientation orientation = Orientation::North, std::size_t level = 1,
            const std::string &teamName = "");
        ~Player() = default;

        Orientation getOrientation() const { return _orientation; }
        void setOrientation(Orientation orientation) { _orientation = orientation; }

        int getLevel() const { return _level; }
        void setLevel(int level) { _level = level; }

        const std::string &getTeamName() const { return _teamName; }
        void setTeamName(const std::string &teamName) { _teamName = teamName; } // maybe useless car on est pas sensé changer le nom j'imagine ?

        const ResourceStock &getInventory() const { return _inventory; }
        void setInventory(const ResourceStock &inventory) { _inventory = inventory; }

        bool isAlive() const { return _alive; }
        void setAlive(bool alive) { _alive = alive; }

        bool isBroadcasting() const { return _isBroadcasting; }
        void setBroadcasting(bool isBroadcasting) { _isBroadcasting = isBroadcasting; }

        bool isIncantating() const { return _isIncantating; }
        void setIncantating(bool isIncantating) { _isIncantating = isIncantating; }

        bool isLayingEgg() const { return _isLayingEgg; }
        void setLayingEgg(bool isLayingEgg) { _isLayingEgg = isLayingEgg; }

    private:
        Orientation _orientation = Orientation::North;
        std::size_t _level;
        std::string _teamName;
        ResourceStock _inventory = {};
        bool _alive = true;
        bool _isBroadcasting = false;
        bool _isIncantating = false;
        bool _isLayingEgg = false;
};

}
