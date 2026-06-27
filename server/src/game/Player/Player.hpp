/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Player class
*/

#pragma once

#include "../Resources/Resources.hpp"
#include <string_view>
#include <string>


namespace Zappy::Game {
    enum class Orientation {
        NORTH = 1,
        EAST  = 2,
        SOUTH = 3,
        WEST  = 4,
    };

    class Player {
        static int s_NextId;

        int m_Id;
        std::string m_TeamName;
        std::pair<unsigned int, unsigned int> m_Position;
        Orientation m_Orientation;
        unsigned char m_Level;
        Resources m_Inventory;

        public:
            Player() noexcept;
            Player(std::string_view team);

            std::string_view team() const noexcept { return m_TeamName; }
            inline int id() const noexcept { return m_Id; }
            inline std::pair<unsigned int, unsigned int> position() const noexcept { return m_Position; }
            inline Orientation orientation() const noexcept { return m_Orientation; }
            inline unsigned char level() const noexcept { return m_Level; }
            inline Resources& inventory() noexcept { return m_Inventory; }
            inline const Resources& inventory() const noexcept { return m_Inventory; }
            inline void moveTo(unsigned int x, unsigned int y) { m_Position = { x, y }; }

            void moveForward(std::pair<unsigned int, unsigned int> size);
            void turnRight();
            void turnLeft();
    };
}
