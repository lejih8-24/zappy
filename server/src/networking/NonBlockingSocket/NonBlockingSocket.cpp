/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** NonBlockingSocket class
*/

#include "NonBlockingSocket.hpp"
#include <poll.h>


Zappy::Networking::NonBlockingSocket::NonBlockingSocket()
    : Socket()
    , m_PollResult(0)
{

}

Zappy::Networking::NonBlockingSocket::NonBlockingSocket(Socket&& socket)
    : NonBlockingSocket()
{
    swap(socket);
}

Zappy::Networking::NonBlockingSocket::NonBlockingSocket(NonBlockingSocket&& old)
    : NonBlockingSocket()
{
    swap(old);
}

void Zappy::Networking::NonBlockingSocket::close() noexcept
{
    Socket::close();
    m_PollResult = 0;
}

/**
 * Attempts to read amount bytes
 * from the socket. If the socket
 * would block, the function fails
 * and a read operations isn't started.
 *
 * Note:
 * This always clears the specified buffer.
 */
ssize_t Zappy::Networking::NonBlockingSocket::read(std::span<char> buffer, std::size_t amount)
{
    if (!canRead())
        return -1;

    disableRead();
    ssize_t readBytes = Socket::read(buffer, amount);

    if (!isOpen())
        close();

    return readBytes;
}

/**
 * Attempts to write the specified data
 * to the socket. If the socket would block,
 * a negative value is returned and the
 * write isn't attempted.
 */
ssize_t Zappy::Networking::NonBlockingSocket::write(std::span<const char> data)
{
    if (!canWrite())
        return -1;

    disableWrite();
    return Socket::write(data);
}

std::optional<Zappy::Networking::Socket> Zappy::Networking::NonBlockingSocket::accept()
{
    if (!canRead())
        return {};

    disableRead();
    return Socket::accept();
}

bool Zappy::Networking::NonBlockingSocket::canWrite() const noexcept
{
    return m_PollResult & POLLOUT;
}

bool Zappy::Networking::NonBlockingSocket::canRead() const noexcept
{
    return m_PollResult & POLLIN;
}

short Zappy::Networking::NonBlockingSocket::requiredEvents() const noexcept
{
    return canWrite()
        ? POLLIN  // don't poll for POLLOUT if not required (spams poll)
        : POLLIN | POLLOUT;
}

void Zappy::Networking::NonBlockingSocket::registerEvents(short revents) noexcept
{
    // FD is no longer valid, so close
    if (revents & POLLNVAL) {
        close();
        return;
    }

    m_PollResult = requiredEvents() == POLLIN
        ? m_PollResult | (revents & POLLIN)
        : revents;
}

void Zappy::Networking::NonBlockingSocket::disableRead()
{
   m_PollResult &= ~POLLIN;
}

void Zappy::Networking::NonBlockingSocket::disableWrite()
{
   m_PollResult &= ~POLLOUT;
}
