/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** BaseEventHandler class
*/

#pragma once

#include <networking/events.hpp>


namespace Zappy::Networking {
    class BaseEventHandler {
        public:
            constexpr BaseEventHandler() noexcept = default;

            constexpr inline void operator()(const ElapsedTimeRequest& data) {}
            constexpr inline void operator()(const MapSize& data) {}
            constexpr inline void operator()(const NewPlayerConnect& data) {}
            constexpr inline void operator()(const PlayerBroadcast& data) {}
            constexpr inline void operator()(const PlayerExpulsion& data) {}
            constexpr inline void operator()(const PlayerIncantationEnd& data) {}
            constexpr inline void operator()(const PlayerIncantationStart& data) {}
            constexpr inline void operator()(const PlayerInventory& data) {}
            constexpr inline void operator()(const PlayerLevel& data) {}
            constexpr inline void operator()(const PlayerPosition& data) {}
            constexpr inline void operator()(const TeamName& data) {}
            constexpr inline void operator()(const TileContents& data) {}
            constexpr inline void operator()(const UnknownCommand& data) {}
    };
}
