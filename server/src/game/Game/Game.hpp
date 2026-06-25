/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Game class
*/

#pragma once

#include "../Resources/Resources.hpp"
#include "../Player/Player.hpp"
#include "../Team/Team.hpp"
#include "../Egg/Egg.hpp"
#include <zappy/utils.hpp>
#include <unordered_map>
#include <random>
#include <chrono>
#include <span>


namespace Zappy::Game {
    class Game {
        static std::mt19937 s_RNG;

        public:
            Utils::Array2D<Resources> m_Map;
            std::vector<Player> m_Players;
            std::vector<Egg> m_Eggs;
            std::unordered_map<std::string, Team> m_Teams;

            std::vector<std::string> m_GraphicsEvents;

        public:
            Game() noexcept;

            // Game runtime
            void update(std::chrono::nanoseconds dt);

            // Game resources
            const Utils::Array2D<Resources>& tiles() const noexcept { return m_Map; }
            std::span<const Player> players() const noexcept { return m_Players; }
            std::span<const Egg> eggs() const noexcept { return m_Eggs; }
            const std::unordered_map<std::string, Team>& teams() const noexcept { return m_Teams; }
            inline std::span<const std::string> graphicsEvents() const noexcept { return m_GraphicsEvents; }

            // Game config and events
            inline void setMapSize(std::pair<unsigned int, unsigned int> size) { m_Map.resize(size.first, size.second); }
            inline std::pair<unsigned int, unsigned int> mapSize() noexcept { return m_Map.size(); }

            void setTeams(std::span<std::string> names, std::size_t maxMembers);

            void playerLayEgg(int playerId) { return playerLayEgg(m_Players[playerId]); }
            void playerLayEgg(const Player& player);

        private:
            template <std::integral T>
            static T random(T start, std::type_identity_t<T> end)
            {
                std::uniform_int_distribution<T> dist(start, end);
                return dist(s_RNG);
            }

            std::pair<unsigned int, unsigned int> randomPosition() const
            {
                auto [x, y] = m_Map.size();
                return { random(0u, x), random(0u, y) };
            }
    };
}
