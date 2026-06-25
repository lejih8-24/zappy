/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Egg class
*/

#pragma once

#include <string_view>
#include <string>


namespace Zappy::Game {
    class Egg {
        static int s_NextId;

        int m_Id;
        int m_PlayerId;  // id of the player that laid the egg
        std::string m_TeamName;
        std::pair<unsigned int, unsigned int> m_Position;

        public:
            Egg(int playerId, std::string_view teamName, unsigned int tileX, unsigned int tileY) noexcept;

            inline int id() const noexcept { return m_Id; }
            inline int parentId() const noexcept { return m_PlayerId; }
            inline void moveTo(unsigned int x, unsigned int y) { m_Position = { x, y }; }
            inline std::string_view team() const noexcept { return m_TeamName; }
            inline std::pair<unsigned int, unsigned int> getPosition() const noexcept { return m_Position; }
    };
}
