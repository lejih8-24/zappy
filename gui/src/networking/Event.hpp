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
    using Event = std::variant<EggConnect,
                               EggCreate,
                               EggDie,
                               ElapsedTimeRequest,
                               MapSize,
                               NewPlayerConnect,
                               PlayerBroadcast,
                               PlayerCollectResource,
                               PlayerDie,
                               PlayerDropResource,
                               PlayerExpulsion,
                               PlayerIncantationEnd,
                               PlayerIncantationStart,
                               PlayerInventory,
                               PlayerLayEgg,
                               PlayerLevel,
                               PlayerPosition,
                               TeamName,
                               TileContents,
                               UnknownCommand>;
}
