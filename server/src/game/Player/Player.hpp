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
    class Player {
        static unsigned int s_NextId;

        unsigned int m_Id;
        std::string m_TeamName;
        std::pair<unsigned int, unsigned int> m_Position;

        public:
            Player(std::string_view team);

            std::string_view team() const noexcept { return m_TeamName; }
            constexpr unsigned int id() const noexcept { return m_Id; }
            constexpr void moveTo(unsigned int x, unsigned int y) { m_Position = { x, y }; }
    };
}
