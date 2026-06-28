/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Unit tests for ResponseParser
*/

#include <criterion/criterion.h>
#include "networking/ResponseParser/ResponseParser.hpp"
#include <variant>

using Zappy::Networking::ResponseParser;
using namespace Zappy::Networking;

Test(response_parser, map_size)
{
    auto ev = ResponseParser::parseMapSize("msz 10 20");
    cr_assert_eq(ev.x, 10u);
    cr_assert_eq(ev.y, 20u);
}

Test(response_parser, team_name)
{
    auto ev = ResponseParser::parseTeamName("tna alpha");
    cr_assert_str_eq(ev.name.c_str(), "alpha");
}

Test(response_parser, player_position)
{
    auto ev = ResponseParser::parsePlayerPosition("ppo #3 7 2 1");
    cr_assert_eq(ev.id, 3);
    cr_assert_eq(ev.x, 7u);
    cr_assert_eq(ev.y, 2u);
}

Test(response_parser, new_player)
{
    auto ev = ResponseParser::parseNewPlayerConnect("pnw #1 0 0 1 2 myteam");
    cr_assert_eq(ev.id, 1);
    cr_assert_eq(ev.x, 0u);
    cr_assert_eq(ev.y, 0u);
    cr_assert_eq(ev.level, 2u);
    cr_assert_str_eq(ev.teamName.c_str(), "myteam");
}

Test(response_parser, player_die)
{
    auto ev = ResponseParser::parsePlayerDie("pdi #42");
    cr_assert_eq(ev.playerId, 42);
}

Test(response_parser, player_level)
{
    auto ev = ResponseParser::parsePlayerLevel("plv #5 3");
    cr_assert_eq(ev.id, 5);
    cr_assert_eq(ev.level, 3u);
}

Test(response_parser, egg_create)
{
    auto ev = ResponseParser::parseEggCreate("enw #1 #2 3 4");
    cr_assert_eq(ev.eggId, 1);
    cr_assert_eq(ev.playerId, 2);
    cr_assert_eq(ev.x, 3u);
    cr_assert_eq(ev.y, 4u);
}

Test(response_parser, egg_die)
{
    auto ev = ResponseParser::parseEggDie("edi #7");
    cr_assert_eq(ev.eggId, 7);
}

Test(response_parser, egg_connect)
{
    auto ev = ResponseParser::parseEggConnect("ebo #9");
    cr_assert_eq(ev.eggId, 9);
}

Test(response_parser, dispatch_msz)
{
    auto ev = ResponseParser::parse("msz 5 8");
    cr_assert(std::holds_alternative<MapSize>(ev));
    cr_assert_eq(std::get<MapSize>(ev).x, 5u);
    cr_assert_eq(std::get<MapSize>(ev).y, 8u);
}

Test(response_parser, dispatch_ppo)
{
    auto ev = ResponseParser::parse("ppo #1 2 3 1");
    cr_assert(std::holds_alternative<PlayerPosition>(ev));
    cr_assert_eq(std::get<PlayerPosition>(ev).id, 1);
}

Test(response_parser, dispatch_tna)
{
    auto ev = ResponseParser::parse("tna beta");
    cr_assert(std::holds_alternative<TeamName>(ev));
    cr_assert_str_eq(std::get<TeamName>(ev).name.c_str(), "beta");
}

Test(response_parser, dispatch_pdi)
{
    auto ev = ResponseParser::parse("pdi #10");
    cr_assert(std::holds_alternative<PlayerDie>(ev));
    cr_assert_eq(std::get<PlayerDie>(ev).playerId, 10);
}

Test(response_parser, dispatch_unknown_command)
{
    auto ev = ResponseParser::parse("zzz unknown");
    cr_assert(std::holds_alternative<ServerUnknownCommand>(ev));
}

Test(response_parser, dispatch_pnw)
{
    auto ev = ResponseParser::parse("pnw #2 1 1 2 1 red");
    cr_assert(std::holds_alternative<NewPlayerConnect>(ev));
    cr_assert_eq(std::get<NewPlayerConnect>(ev).id, 2);
}
