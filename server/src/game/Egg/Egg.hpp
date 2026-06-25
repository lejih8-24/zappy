/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Egg class
*/

#pragma once

#include <utility>


namespace Zappy::Game {
    class Egg {
        static int s_NextId;

        int m_Id;
        int m_PlayerId;  // id of the player that laid the egg
        std::pair<unsigned int, unsigned int> m_Position;

        public:
            Egg(int playerId, unsigned int tileX, unsigned int tileY) noexcept;

            inline int id() const noexcept { return m_Id; }
            inline int parentId() const noexcept { return m_PlayerId; }
            inline void moveTo(unsigned int x, unsigned int y) { m_Position = { x, y }; }
            inline std::pair<unsigned int, unsigned int> getPosition() const noexcept { return m_Position; }
    };
}
