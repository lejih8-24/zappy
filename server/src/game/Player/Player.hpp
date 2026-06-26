/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Player class
*/

#pragma once

#include <string_view>
#include <string>


namespace Zappy::Game {
    enum class Orientation {
        NORTH,
        EAST,
        SOUTH,
        WEST,
    };

    class Player {
        static int s_NextId;

        int m_Id;
        std::string m_TeamName;
        std::pair<unsigned int, unsigned int> m_Position;
        Orientation m_Orientation;
        unsigned char m_Level;

        public:
            Player(std::string_view team);

            std::string_view team() const noexcept { return m_TeamName; }
            constexpr int id() const noexcept { return m_Id; }
            constexpr std::pair<unsigned int, unsigned int> position() const noexcept { return m_Position; }
            constexpr void moveTo(unsigned int x, unsigned int y) { m_Position = { x, y }; }

            void moveForward(std::pair<unsigned int, unsigned int> size);
            void turnRight();
            void turnLeft();
    };
}
