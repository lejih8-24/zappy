/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** ResponseParser class
*/

#include "ResponseParser.hpp"
#include <exceptions.hpp>
#include <charconv>


auto Zappy::Networking::ResponseParser::parse(std::string_view line) -> Event
{
    if (line.empty())
        return UnknownCommand();

    switch (line[0]) {
        case 'b': return parseTileContents(line);
        case 'e': return parseEggCommand(line);
        case 'm': return parseMapSize(line);
        case 'p': return parsePlayerCommand(line);
        case 't': return parseTeamName(line);
        case 's': return parseServerCommand(line);
        default:  return UnknownCommand();
    }
}

auto Zappy::Networking::ResponseParser::parseEggCommand(std::string_view line) -> Event
{
    switch (line[1]) {
        // TODO: Implement
        default: return UnknownCommand();
    }
}

auto Zappy::Networking::ResponseParser::parsePlayerCommand(std::string_view line) -> Event
{
    if (line.length() != 3)
        return UnknownCommand();

    switch (line[1]) {
        case 'b': return parsePlayerBroadcast(line);
        case 'd': // TODO
        case 'e': return parsePlayerExpulsion(line);
        case 'f': // TODO
        case 'g': // TODO
        case 'i': {
            switch (line[2]) {
                case 'c': return parsePlayerIncantationStart(line);
                case 'e': return parsePlayerIncantationEnd(line);
                case 'n': return parsePlayerInventory(line);
                default:  return UnknownCommand();
            }
        }
        case 'l': return parsePlayerLevel(line);
        case 'n': return parseNewPlayerConnect(line);
        case 'p': return parsePlayerPosition(line);
        default: return UnknownCommand();
    }
}

auto Zappy::Networking::ResponseParser::parseServerCommand(std::string_view line) -> Event
{
    switch (line[1]) {
        // TODO: Implement
        default: return UnknownCommand();
    }
}

auto Zappy::Networking::ResponseParser::parseMapSize(std::string_view line) -> MapSize
{
    std::string_view response = line;

    std::string_view command = extractWord(response);
    if (command != "msz")
        throw Exceptions::ServerException("invalid map size response: '" + std::string(line) + "'");

    std::string_view width = extractWord(response);
    std::string_view height = extractWord(response);

    return { toInteger(width), toInteger(height) };
}

auto Zappy::Networking::ResponseParser::parseTileContents(std::string_view line) -> Zappy::Networking::TileContents
{
    std::string_view response = line;

    std::string_view command = extractWord(response);
    if (command != "bct")
        throw Exceptions::ServerException("invalid tile contents response: '" + std::string(line) + "'");

    auto tileX = extractInteger(response);
    auto tileY = extractInteger(response);

    Game::Resources resources = {
        .food      = extractInteger(response),
        .linemate  = extractInteger(response),
        .deraumere = extractInteger(response),
        .sibur     = extractInteger(response),
        .mendiane  = extractInteger(response),
        .phiras    = extractInteger(response),
        .thystame  = extractInteger(response),
    };

    return { tileX, tileY, resources };
}

auto Zappy::Networking::ResponseParser::parseTeamName(std::string_view response) -> TeamName
{
    return {};
}

auto Zappy::Networking::ResponseParser::parseNewPlayerConnect(std::string_view response) -> NewPlayerConnect
{
    return {};  // TODO: implement
}

auto Zappy::Networking::ResponseParser::parsePlayerPosition(std::string_view response) -> PlayerPosition
{
    return {};  // TODO: implement
}

auto Zappy::Networking::ResponseParser::parsePlayerLevel(std::string_view response) -> PlayerLevel
{
    return {};  // TODO: implement
}

auto Zappy::Networking::ResponseParser::parsePlayerInventory(std::string_view response) -> PlayerInventory
{
    return {};  // TODO: implement
}

auto Zappy::Networking::ResponseParser::parsePlayerBroadcast(std::string_view response) -> PlayerBroadcast
{
    return {};  // TODO: implement
}

auto Zappy::Networking::ResponseParser::parsePlayerExpulsion(std::string_view response) -> PlayerExpulsion
{
    return {};  // TODO: implement
}

auto Zappy::Networking::ResponseParser::parsePlayerIncantationStart(std::string_view response) -> PlayerIncantationStart
{
    return {};  // TODO: implement
}

auto Zappy::Networking::ResponseParser::parsePlayerIncantationEnd(std::string_view response) -> PlayerIncantationEnd
{
    return {};  // TODO: implement
}

std::pair<std::string_view, std::string_view> Zappy::Networking::ResponseParser::splitWord(std::string_view line)
{
    if (line.length() == 0)
        return { line, "" };

    auto wordEnd = line.find(' ');
    if (wordEnd >= line.length() - 1)
        return { line, "" };

    std::string_view word = line.substr(0, wordEnd);
    std::string_view remainder = line.substr(wordEnd + 1);

    return { word, remainder };
}

std::string_view Zappy::Networking::ResponseParser::extractWord(std::string_view& line)
{
    auto [word, remainder] = splitWord(line);

    line = remainder;
    return word;
}

unsigned int Zappy::Networking::ResponseParser::toInteger(std::string_view repr)
{
    unsigned int value;

    auto [ptr, errc] = std::from_chars(repr.begin(), repr.end(), value);
    if (errc == std::errc())
        return value;

    throw Exceptions::ServerException("invalid number from server: " + std::string(repr));
}
