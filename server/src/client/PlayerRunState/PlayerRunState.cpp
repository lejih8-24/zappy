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
{

}

void Zappy::Client::PlayerRunState::update(Client& client, std::chrono::nanoseconds dt)
{
    // queuePlayerMessages();
    sendQueuedMessages(client);

    if (m_Player.inventory().food == 0 && m_Player.timeToLive() <= 0.0ms) {
        if (!hasMessages())
            client.close();
        return;
    }

    auto ttl = m_Player.reduceTimeToLive(dt);
    if (ttl <= 0.0ms) {
        if (m_Player.inventory().food == 0) {
            queueMessage("dead\n");
            return;
        }

        game().playerFeed(m_Player);
    }

    if (m_Player.cooldown() > 0.0ms) {
        // There are still some
        // messages queued, so skip
        // to next update to send
        // them before going into
        // cooldown
        if (hasMessages())
            return;
        client.setState(std::make_unique<PlayerWaitState>(m_Player));
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

void Zappy::Client::PlayerRunState::queuePlayerMessages()
{
    auto message = m_Player.popMessage();

    while (message) {
        queueMessage(*message);
        message = m_Player.popMessage();
    }
}

void Zappy::Client::PlayerRunState::addCooldown(std::chrono::duration<double, std::milli> duration)
{
    m_Player.addCooldown(duration / game().gameSpeed());
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
    game.playerUpdatePosition(state.m_Player);
    state.addCooldown(Game::PLAYER_FORWARD_COOLDOWN);
}

void Zappy::Client::PlayerRunState::rightCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    state.m_Player.turnRight();
    state.queueMessage("ok\n");
    game.playerUpdatePosition(state.m_Player);
    state.addCooldown(Game::PLAYER_RIGHT_COOLDOWN);
}

void Zappy::Client::PlayerRunState::leftCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    state.m_Player.turnLeft();
    state.queueMessage("ok\n");
    game.playerUpdatePosition(state.m_Player);
    state.addCooldown(Game::PLAYER_LEFT_COOLDOWN);
}

void Zappy::Client::PlayerRunState::lookCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    std::string message = game.playerLook(state.m_Player);
    state.queueMessage(message + "\n");
    state.addCooldown(Game::PLAYER_LOOK_COOLDOWN);
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
    state.addCooldown(Game::PLAYER_INVENTORY_COOLDOWN);
}

void Zappy::Client::PlayerRunState::broadcastCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement
    state.addCooldown(Game::PLAYER_BROADCAST_COOLDOWN);
}

void Zappy::Client::PlayerRunState::connectNbrCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    auto& team = game.playerTeam(state.m_Player);

    state.queueMessage(std::to_string(team.maxMembers - team.members) + "\n");
}

void Zappy::Client::PlayerRunState::forkCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement
    state.addCooldown(Game::PLAYER_FORK_COOLDOWN);
}

void Zappy::Client::PlayerRunState::ejectCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement
    state.addCooldown(Game::PLAYER_EJECT_COOLDOWN);
}

void Zappy::Client::PlayerRunState::takeCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game)
{
    auto resourceType = Game::toResourceType(args);
    if (!resourceType) {
        logger.warning() << "unknown resource type " << logger.escape(args) << std::endl;
        state.queueMessage("ko\n");
        return;
    }

    bool success = game.playerCollectResource(state.m_Player, *resourceType);
    state.queueMessage(success ? "ok\n" : "ko\n");
    state.addCooldown(Game::PLAYER_TAKE_COOLDOWN);
}

void Zappy::Client::PlayerRunState::setCommand(std::string_view args, PlayerRunState& state, Client& client, Game::Game& game)
{
    auto resourceType = Game::toResourceType(args);
    if (!resourceType) {
        logger.warning() << "unknown resource type " << logger.escape(args) << std::endl;
        state.queueMessage("ko\n");
        return;
    }

    bool success = game.playerDropResource(state.m_Player, *resourceType);
    state.queueMessage(success ? "ok\n" : "ko\n");
    state.addCooldown(Game::PLAYER_SET_COOLDOWN);
}

void Zappy::Client::PlayerRunState::incantationCommand(std::string_view, PlayerRunState& state, Client& client, Game::Game& game)
{
    // TODO: implement

    // Don't add cooldown, since the game already
    // adds it to all involved players
}
