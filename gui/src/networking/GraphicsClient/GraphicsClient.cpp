/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** GraphicsClient class
*/

#include "GraphicsClient.hpp"
#include <networking.hpp>
#include <exceptions.hpp>
#include <thread>


using std::chrono_literals::operator""ms;


Zappy::Networking::GraphicsClient::GraphicsClient()
    : m_Server()
    , m_ResponseBuffer()
    , m_EventQueue()
{
}

Zappy::Networking::GraphicsClient::GraphicsClient(std::string_view ip, std::uint16_t port)
    : GraphicsClient()
{
    m_Server.open();
    m_Server.connect(ip, port);
    doHandshake();
}

Zappy::Networking::GraphicsClient::GraphicsClient(GraphicsClient&& other)
    : GraphicsClient()
{
    swap(other);
}

auto Zappy::Networking::GraphicsClient::mapSize() -> MapSize
{
    send("msz\n");

    std::string_view response = getResponse("msz");
    return ResponseParser::parseMapSize(response);
}

auto Zappy::Networking::GraphicsClient::tileContents(coordinate x, coordinate y) -> TileContents
{
    std::string msg = "bct ";
    msg += std::to_string(x) + ' ' + std::to_string(y) + '\n';
    send(msg);

    std::string_view response = getResponse("bct");
    return ResponseParser::parseTileContents(response);
}

/**
 * Requests the server to send the contents
 * of the whole map as events.
 *
 * Usage:
 * GraphicsClient client;
 * client.mapContents();
 *
 * // map contents are sent via events
 * auto event = client.pollEvent();
 * while (event) {
 *     std::visit(EventHandler(), *event);
 *     event = client.pollEvent();
 * }
 */
void Zappy::Networking::GraphicsClient::mapContents()
{
    send("mct\n");
}

/**
 * Fills the given vector with
 * the names of every team registered
 * in the zappy server.
 *
 * This process clears any existing
 * values in the vector.
 */
void Zappy::Networking::GraphicsClient::teamNames(std::vector<std::string>& names)
{
    names.clear();
    send("tna\n");

    std::string_view response = getResponse("tna");
    while (response.starts_with("tna")) {
        auto team = ResponseParser::parseTeamName(response);
        names.push_back(std::move(team.name));

        response = getResponse("tna", false);
    }
}

auto Zappy::Networking::GraphicsClient::playerPosition(int playerId) -> PlayerPosition
{
    send("ppo #" + std::to_string(playerId) + "\n");

    std::string_view response = getResponse("ppo");
    return ResponseParser::parsePlayerPosition(response);
}

unsigned int Zappy::Networking::GraphicsClient::playerLevel(int playerId)
{
    send("plv #" + std::to_string(playerId) + "\n");

    std::string_view response = getResponse("plv");
    auto player = ResponseParser::parsePlayerLevel(response);
    return player.level;
}

auto Zappy::Networking::GraphicsClient::playerInventory(int playerId) -> Game::Resources
{
    send("pin #" + std::to_string(playerId) + "\n");

    std::string_view response = getResponse("pin");
    auto player = ResponseParser::parsePlayerInventory(response);
    return player.inventory;
}

unsigned int Zappy::Networking::GraphicsClient::getTime()
{
    // send("sgt\n");

    // std::string_view response = getResponse("sgt");
    // auto server = ResponseParser::parseElapsedTimeRequest(response);
    // return server.elapsed_time;
    return 0;
}

void Zappy::Networking::GraphicsClient::setTime(unsigned int units)
{
    return;
}

auto Zappy::Networking::GraphicsClient::pollEvent() -> std::optional<Event>
{
    refreshEvents();

    if (m_EventQueue.empty())
        return {};

    Event event = m_EventQueue.back();
    m_EventQueue.pop_back();
    return event;
}

void Zappy::Networking::GraphicsClient::swap(GraphicsClient& other)
{
    std::swap(m_Server, other.m_Server);
    std::swap(m_ResponseBuffer, other.m_ResponseBuffer);
}

void Zappy::Networking::GraphicsClient::refreshEvents()
{
    std::string_view line = getline(false);

    while (!line.empty()) {
        m_EventQueue.push_front(ResponseParser::parse(line));
        line = getline(false);
    }
}

void Zappy::Networking::GraphicsClient::doHandshake()
{
    std::string_view line = getline();
    if (line != "WELCOME")
        throw Lattice::Exceptions::SocketException("invalid response in zappy server handshake");

    send("GRAPHIC\n");

    // Sleep for 100ms to let server send all
    // initial events. This is not necessary,
    // since the events will get handled anyway,
    // but it increases runtime performance.
    std::this_thread::sleep_for(100ms);

    refreshEvents();
}

std::string_view Zappy::Networking::GraphicsClient::getline(bool wait)
{
    m_ResponseBuffer.popLine();

    auto bufferLine = m_ResponseBuffer.getLine();
    if (bufferLine)
        return *bufferLine;

    if (!wait) {
        pollfd pollEvents = { m_Server.fileno(), POLL_IN, 0 };

        if (::poll(&pollEvents, 1, 0) < 0)
            return std::string_view();

        if (!(pollEvents.revents & POLL_IN))
            return std::string_view();
    }

    std::array<char, 1024> buffer;
    auto bytesRead = m_Server.read(buffer, buffer.size());
    if (bytesRead <= 0)
        return std::string_view();

    m_ResponseBuffer.pushBack(std::span(buffer.data(), bytesRead));
    bufferLine = m_ResponseBuffer.getLine();

    return bufferLine ? *bufferLine : std::string_view();
}

bool Zappy::Networking::GraphicsClient::isValidResponse(std::string_view response)
{
    static constexpr std::array<std::string_view, 3> failedResponses = {"seg", "suc", "sbp"};

    for (auto fail : failedResponses) {
        if (!response.starts_with(fail))
            continue;
        return false;
    }

    return true;
}

std::string_view Zappy::Networking::GraphicsClient::getResponse(std::string_view cmd, bool wait)
{
    std::string_view response = getline(wait);

    while (!response.empty() && isValidResponse(response) && !response.starts_with(cmd)) {
        m_EventQueue.push_front(ResponseParser::parse(cmd));
        response = getline(wait);
    }

    return response;
}

void Zappy::Networking::GraphicsClient::send(std::string_view msg)
{
    std::string sanitized;

    if (!msg.ends_with('\n')) {
        sanitized = msg;
        sanitized += '\n';
        msg = sanitized;
    }

    m_Server.write(msg);
}
