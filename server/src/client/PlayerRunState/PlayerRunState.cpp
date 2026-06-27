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


using std::chrono_literals::operator ""ms;


const std::unordered_map<std::string_view, Zappy::Client::PlayerRunState::CommandHandler> Zappy::Client::PlayerRunState::COMMAND_HANDLERS = {
    { "forward",     forwardCommand },
    { "right",       rightCommand },
    { "left",        leftCommand },
    { "look",        lookCommand },
    { "inventory",   inventoryCommand },
    { "broadcast",   broadcastCommand },
    { "connect_nbr", connectNbrCommand },
    { "fork",        forkCommand },
    { "eject",       ejectCommand },
    { "take",        takeCommand },
    { "set",         setCommand },
    { "incantation", incantationCommand },
};


Zappy::Client::PlayerRunState::PlayerRunState(Game::Player& player)
    : QueueState()
    , m_Player(player)
    , m_Cooldown(0)
{

}

void Zappy::Client::PlayerRunState::update(Client& client, std::chrono::nanoseconds dt)
{
    sendQueuedMessages(client);

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

void Zappy::Client::PlayerRunState::disconnect(Game::Game& game)
{
    game.killPlayer(m_Player);
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

void Zappy::Client::PlayerRunState::runCommand(Client& client, std::string& line)
{
    auto commandEnd = line.find(' ');
    std::string_view lineView = line;
    std::string_view command = lineView.substr(0, commandEnd);
    std::string_view args = commandEnd + 1 >= lineView.size() ? std::string_view() : lineView.substr(commandEnd + 1);

    try {
        auto handler = COMMAND_HANDLERS.at(command);
        logger.info() << std::string(client) << ": running command " << command << std::endl;
        handler(args, *this, client, game());
    } catch (std::out_of_range) {
        logger.warning() << std::string(client) << ": unknown command " << std::quoted(command) << std::endl;
        queueMessage("ko\n");
        return;
    }
}

void Zappy::Client::PlayerRunState::forwardCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    state.m_Player.moveForward(game.mapSize());
    state.queueMessage("ok\n");
    state.m_Cooldown = 7'000.0ms / game.gameSpeed();
}

void Zappy::Client::PlayerRunState::rightCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    state.m_Player.turnRight();
    state.queueMessage("ok\n");
    state.m_Cooldown = 7'000.0ms / game.gameSpeed();
}

void Zappy::Client::PlayerRunState::leftCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    state.m_Player.turnLeft();
    state.queueMessage("ok\n");
    state.m_Cooldown = 7'000.0ms / game.gameSpeed();
}

void Zappy::Client::PlayerRunState::lookCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement
}

void Zappy::Client::PlayerRunState::inventoryCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    const auto& inventory = state.m_Player.inventory();

    std::ostringstream msg;

    msg << "[ "
        << "food "      << inventory.food      << ", "
        << "linemate "  << inventory.linemate  << ", "
        << "deraumere " << inventory.deraumere << ", "
        << "sibur "     << inventory.sibur     << ", "
        << "mendiane "  << inventory.mendiane  << ", "
        << "phiras "    << inventory.phiras    << ", "
        << "thystame "  << inventory.thystame  << " "
        << "]\n";

    state.queueMessage(msg.str());
    state.m_Cooldown = 1'000.0ms / game.gameSpeed();
}

void Zappy::Client::PlayerRunState::broadcastCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement
}

void Zappy::Client::PlayerRunState::connectNbrCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    auto& team = game.playerTeam(state.m_Player);

    state.queueMessage(std::to_string(team.maxMembers - team.members) + "\n");
}

void Zappy::Client::PlayerRunState::forkCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement
}

void Zappy::Client::PlayerRunState::ejectCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement
}

void Zappy::Client::PlayerRunState::takeCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{

}

void Zappy::Client::PlayerRunState::setCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement
}

void Zappy::Client::PlayerRunState::incantationCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement
}
