/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Team struct
*/

#pragma once

#include <string>


namespace Zappy::Game {
    class Team {
        private:
            std::string m_Name;

        public:
            std::size_t members;
            std::size_t maxMembers;
            std::size_t maxLevelMembers;

        public:
            Team() noexcept;
            Team(std::string_view name, std::size_t maxMembers);

            std::string_view name() const noexcept { return m_Name; }
    };
}
