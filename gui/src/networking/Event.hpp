/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Server Event class
*/

#pragma once

#include <networking/events.hpp>
#include <variant>


namespace Zappy::Networking {
    using Event = std::variant<ElapsedTimeRequest,
                               MapSize,
                               NewPlayerConnect,
                               PlayerBroadcast,
                               PlayerExpulsion,
                               PlayerIncantationEnd,
                               PlayerIncantationStart,
                               PlayerInventory,
                               PlayerLevel,
                               PlayerPosition,
                               TeamName,
                               TileContents,
                               UnknownCommand>;
}
