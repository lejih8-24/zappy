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
#include <chrono>
#include <deque>


namespace Zappy::Game {
    enum class Orientation {
        NORTH = 1,
        EAST  = 2,
        SOUTH = 3,
        WEST  = 4,
    };

    class Player {
        using Duration = std::chrono::duration<double, std::milli>;

        static int s_NextId;

        int m_Id;
        std::string m_TeamName;
        std::pair<unsigned int, unsigned int> m_Position;
        Orientation m_Orientation;
        unsigned char m_Level;
        Resources m_Inventory;
        Duration m_TimeToLive;
        Duration m_Cooldown;
        std::deque<std::string> m_Messages;

        public:
            Player() noexcept;
            Player(std::string_view team);

            void addMessage(std::string&& msg);
            std::optional<std::string> popMessage();

            inline Duration timeToLive() const noexcept { return m_TimeToLive; }
            inline Duration reduceTimeToLive(Duration amnt) noexcept { m_TimeToLive -= amnt; return m_TimeToLive; }
            inline Duration addTimeToLive(Duration amnt) noexcept { m_TimeToLive += amnt; return m_TimeToLive; }

            inline Duration cooldown() const noexcept { return m_Cooldown; }
            inline Duration reduceCooldown(Duration amnt) noexcept { m_Cooldown -= amnt; return m_Cooldown; }
            inline Duration addCooldown(Duration amnt) noexcept { m_Cooldown += amnt; return m_Cooldown; }

            std::string_view team() const noexcept { return m_TeamName; }
            inline int id() const noexcept { return m_Id; }
            inline std::pair<unsigned int, unsigned int> position() const noexcept { return m_Position; }
            inline Orientation orientation() const noexcept { return m_Orientation; }
            inline void setOrientation(Orientation orientation) noexcept { m_Orientation = orientation; }
            inline unsigned char level() const noexcept { return m_Level; }
            inline void levelUp() noexcept { m_Level++; }
            inline Resources& inventory() noexcept { return m_Inventory; }
            inline const Resources& inventory() const noexcept { return m_Inventory; }
            inline void moveTo(unsigned int x, unsigned int y) { m_Position = { x, y }; }

            void moveForward(std::pair<unsigned int, unsigned int> size);
            void turnRight();
            void turnLeft();
    };
}
