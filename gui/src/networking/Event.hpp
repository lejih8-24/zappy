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
                               ServerBadParameter,
                               ServerGameEnd,
                               ServerGetTime,
                               ServerMessage,
                               ServerSetTime,
                               ServerUnknownCommand,
                               TeamName,
                               TileContents>;
}
