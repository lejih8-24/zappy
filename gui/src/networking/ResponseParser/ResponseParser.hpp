/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** ResponseParser class
*/

#pragma once

#include <networking/events.hpp>
#include "../Event.hpp"


namespace Zappy::Networking {
    class ResponseParser {
        public:
            static Event parse(std::string_view response);

            static MapSize parseMapSize(std::string_view response);
            static TileContents parseTileContents(std::string_view response);
            static TeamName parseTeamName(std::string_view response);

            static NewPlayerConnect parseNewPlayerConnect(std::string_view response);
            static PlayerPosition parsePlayerPosition(std::string_view response);
            static PlayerLevel parsePlayerLevel(std::string_view response);
            static PlayerInventory parsePlayerInventory(std::string_view response);
            static PlayerBroadcast parsePlayerBroadcast(std::string_view response);
            static PlayerExpulsion parsePlayerExpulsion(std::string_view response);
            static PlayerIncantationStart parsePlayerIncantationStart(std::string_view response);
            static PlayerIncantationEnd parsePlayerIncantationEnd(std::string_view response);

        private:
            static Event parseEggCommand(std::string_view response);
            static Event parsePlayerCommand(std::string_view response);
            static Event parseServerCommand(std::string_view response);

            static std::pair<std::string_view, std::string_view> splitWord(std::string_view line);
            static std::string_view extractWord(std::string_view& line);
            static unsigned int toInteger(std::string_view repr);

            static inline unsigned int extractInteger(std::string_view& line) { return toInteger(extractWord(line)); }
            static int extractId(std::string_view& line);
            static float extractOrientation(std::string_view& line);

            enum Orientation {
                NORTH = 1,
                EAST  = 2,
                SOUTH = 3,
                WEST  = 4,
            };
    };
}
