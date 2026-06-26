/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** PlayerRunState class
*/

#include "PlayerRunState.hpp"
#include <zappy/client.hpp>
#include <zappy/utils.hpp>
#include <zappy/game.hpp>
#include <unordered_map>
#include <memory>


const std::unordered_map<std::string_view, Zappy::Client::PlayerRunState::CommandHandler> Zappy::Client::PlayerRunState::COMMAND_HANDLERS = {
    { "forward", forwardCommand },
    { "right",   rightCommand },
    { "left",    leftCommand },
};


Zappy::Client::PlayerRunState::PlayerRunState(Game::Player& player)
    : QueueState()
    , m_Player(player)
    , m_Cooldown(0)
{

}

void Zappy::Client::PlayerRunState::init()
{

}

void Zappy::Client::PlayerRunState::update(Client& client, std::chrono::nanoseconds dt)
{
    if (hasMessages()) {
        std::string events = popMessages();
        client.send(events);
    }

    if (m_Cooldown > m_Cooldown.zero()) {
        // There are still some
        // messages queued, so skip
        // to next update to send
        // them before going into
        // cooldown
        if (hasMessages())
            return;
        client.setState(std::make_unique<PlayerWaitState>(m_Player, m_Cooldown));
        return;
    }

    std::string command;
    if (!client.readline(command))
        return;

    toLowercase(command);
    runCommand(client, command);

    if (hasMessages())
        client.refresh();
}

void Zappy::Client::PlayerRunState::toLowercase(std::string& repr)
{
    // See https://en.cppreference.com/cpp/string/byte/tolower
    std::transform(
        repr.begin(),
        repr.end(),
        repr.begin(),
        [](unsigned char c){ return std::tolower(c); }
    );
}

void Zappy::Client::PlayerRunState::runCommand(Client& client, std::string& command)
{
    try {
        auto handler = COMMAND_HANDLERS.at(command);
        logger.debug() << std::string(client) << ": running command " << command << std::endl;
        handler(*this, client, game());
    } catch (std::out_of_range) {
        logger.debug() << std::string(client) << ": unknown command " << std::quoted(command) << std::endl;
        queueMessage("ko\n");
        return;
    }
}

void Zappy::Client::PlayerRunState::forwardCommand(PlayerRunState& state, Client& client, Game::Game& game)
{
    using std::chrono_literals::operator ""ms;

    state.m_Player.moveForward(game.mapSize());
    state.queueMessage("ok\n");
    state.m_Cooldown = 7'000.0ms / game.gameSpeed();
}

void Zappy::Client::PlayerRunState::rightCommand(PlayerRunState& state, Client& client, Game::Game& game)
{
    using std::chrono_literals::operator ""ms;

    state.m_Player.turnRight();
    state.queueMessage("ok\n");
    state.m_Cooldown = 7'000.0ms / game.gameSpeed();
}

void Zappy::Client::PlayerRunState::leftCommand(PlayerRunState& state, Client& client, Game::Game& game)
{
    using std::chrono_literals::operator ""ms;

    state.m_Player.turnLeft();
    state.queueMessage("ok\n");
    state.m_Cooldown = 7'000.0ms / game.gameSpeed();
}
