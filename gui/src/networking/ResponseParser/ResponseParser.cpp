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


auto Zappy::Networking::ResponseParser::parseMapSize(std::string_view response) -> Position
{
    std::string_view command = extractWord(response);
    if (command != "msz")
        throw Exceptions::ServerException("invalid map size response: '" + std::string(response) + "'");

    std::string_view width = extractWord(response);
    std::string_view height = extractWord(response);

    return { toInteger(width), toInteger(height) };
}

auto Zappy::Networking::ResponseParser::parseTileContents(std::string_view response) -> Zappy::Networking::TileContents
{
    std::string_view command = extractWord(response);
    if (command != "bct")
        throw Exceptions::ServerException("invalid tile contents response: '" + std::string(response) + "'");

    std::string_view tileX = extractWord(response);
    std::string_view tileY = extractWord(response);
    Position tilePos = { toInteger(tileX), toInteger(tileY) };

    Game::Resources resources = {
        .food      = toInteger(extractWord(response)),
        .linemate  = toInteger(extractWord(response)),
        .deraumere = toInteger(extractWord(response)),
        .sibur     = toInteger(extractWord(response)),
        .mendiane  = toInteger(extractWord(response)),
        .phiras    = toInteger(extractWord(response)),
        .thystame  = toInteger(extractWord(response)),
    };

    return { tilePos, resources };
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
