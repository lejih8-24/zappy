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

    return {
        .x         = extractInteger(response),
        .y         = extractInteger(response),
        .resources = extractResources(response),
    };
}

auto Zappy::Networking::ResponseParser::parseTeamName(std::string_view line) -> TeamName
{
    std::string_view response = line;

    std::string_view command = extractWord(response);
    if (command != "tna")
        throw Exceptions::ServerException("invalid team name response: '" + std::string(line) + "'");

    return { .name = std::string(response) };
}

auto Zappy::Networking::ResponseParser::parseNewPlayerConnect(std::string_view line) -> NewPlayerConnect
{
    std::string_view response = line;

    std::string_view command = extractWord(response);
    if (command != "pnw")
        throw Exceptions::ServerException("invalid player connection response: '" + std::string(line) + "'");

    return {
        .id          = extractId(response),
        .x           = extractInteger(response),
        .y           = extractInteger(response),
        .rotationDeg = extractOrientation(response),
        .level       = extractInteger(response),
        .teamName    = std::string(response)
    };
}

auto Zappy::Networking::ResponseParser::parsePlayerPosition(std::string_view line) -> PlayerPosition
{
    std::string_view response = line;

    std::string_view command = extractWord(response);
    if (command != "ppo")
        throw Exceptions::ServerException("invalid player position response: '" + std::string(line) + "'");

    return {
        .id          = extractId(response),
        .x           = extractInteger(response),
        .y           = extractInteger(response),
        .rotationDeg = extractOrientation(response),
    };
}

auto Zappy::Networking::ResponseParser::parsePlayerLevel(std::string_view line) -> PlayerLevel
{
    std::string_view response = line;

    std::string_view command = extractWord(response);
    if (command != "plv")
        throw Exceptions::ServerException("invalid player level response: '" + std::string(line) + "'");

    return {
        .id    = extractId(response),
        .level = extractInteger(response),
    };
}

auto Zappy::Networking::ResponseParser::parsePlayerInventory(std::string_view line) -> PlayerInventory
{
    std::string_view response = line;

    std::string_view command = extractWord(response);
    if (command != "pin")
        throw Exceptions::ServerException("invalid player inventory response: '" + std::string(line) + "'");

    return {
        .id        = extractId(response),
        .x         = extractInteger(response),
        .y         = extractInteger(response),
        .inventory = extractResources(response),
    };
}

auto Zappy::Networking::ResponseParser::parsePlayerBroadcast(std::string_view line) -> PlayerBroadcast
{
    std::string_view response = line;

    std::string_view command = extractWord(response);
    if (command != "pbc")
        throw Exceptions::ServerException("invalid player inventory response: '" + std::string(line) + "'");

    return {
        .id      = extractId(response),
        .message = std::string(response),
    };
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

int Zappy::Networking::ResponseParser::extractId(std::string_view& line)
{
    std::string_view id = extractWord(line);

    if (!id.starts_with('#'))
        throw Exceptions::ServerException("invalid id from server: " + std::string(id));

    id = id.substr(1);  // remove leading '#'

    int idValue;
    auto [ptr, errc] = std::from_chars(id.begin(), id.end(), idValue);
    if (errc == std::errc())
        return idValue;

    throw Exceptions::ServerException("invalid id from server: " + std::string(id));
}

auto Zappy::Networking::ResponseParser::extractResources(std::string_view& line) -> Game::Resources
{
    return {
        .food      = extractInteger(line),
        .linemate  = extractInteger(line),
        .deraumere = extractInteger(line),
        .sibur     = extractInteger(line),
        .mendiane  = extractInteger(line),
        .phiras    = extractInteger(line),
        .thystame  = extractInteger(line),
    };
}

float Zappy::Networking::ResponseParser::extractOrientation(std::string_view& line)
{
    unsigned int orientation = extractInteger(line);

    switch (orientation) {
        case NORTH: return   0.f;
        case WEST:  return  90.f;
        case SOUTH: return 180.f;
        case EAST:  return 270.f;
        default: throw Exceptions::ServerException("invalid orientation from server: " + std::to_string(orientation));
    }
}
