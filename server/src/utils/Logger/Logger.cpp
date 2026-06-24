/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Implementation for
** Logger class
*/

#include "Logger.hpp"
#include <format>
#include <chrono>
#include <cassert>


static class : public std::ostream {} VOID_STREAM;
std::ostream& Zappy::Utils::Logger::s_VoidStream = VOID_STREAM;


Zappy::Utils::Logger::Logger(std::string name, Level level, std::ostream& output)
    : m_Output(&output)
    , m_LogLevel(level)
    , m_Colors(true)
    , m_Name(std::move(name))
{

}

auto Zappy::Utils::Logger::getRootLogger() -> Logger&
{
    static Logger root("", DEBUG);

    return root;
}

Zappy::Utils::Logger::Level Zappy::Utils::Logger::logLevel() const
{
    return m_LogLevel == ROOT
        ? getRootLogger().m_LogLevel
        : m_LogLevel;
}

std::ostream& Zappy::Utils::Logger::log(Level level)
{
    assert(level != ROOT);
    if (level < logLevel())
        return s_VoidStream;

    *m_Output << currentTime()
              << " "
              << "["
              << levelColorSequence(level)
              << level
              << clearColorSequence()
              << "]";

    if (!m_Name.empty())
        *m_Output << " - " << m_Name;

    return *m_Output << ": ";
}

std::string Zappy::Utils::Logger::escape(std::string_view input)
{
    std::string result;
    result.reserve(input.length());

    for (char c : input) {
        pushEscaped(result, c);
    }

    return result;
}

std::string Zappy::Utils::Logger::escape(std::string_view input, char delim)
{
    std::string result;
    result.reserve(input.length() + 2);  // +2 to account for delims

    result.push_back(delim);

    for (char c : input) {
        if (c == delim) {
            result.push_back('\\');
            result.push_back(c);
            continue;
        }

        pushEscaped(result, c);
    }

    result.push_back(delim);

    return result;
}

char Zappy::Utils::Logger::unescape(char c)
{
    switch (c) {
        case '\a': return 'a';
        case '\b': return 'b';
        case '\f': return 'f';
        case '\n': return 'n';
        case '\r': return 'r';
        case '\t': return 't';
        case '\v': return 'v';
        case '\0': return '0';

        default: return '\0';
    }
}

void Zappy::Utils::Logger::pushEscaped(std::string& buffer, char c)
{
    char unescaped = unescape(c);
    if (unescaped == '\0') {
        buffer.push_back(c);
        return;
    }

    buffer.push_back('\\');
    buffer.push_back(unescaped);
}

std::string Zappy::Utils::Logger::currentTime()
{
    auto time = std::chrono::system_clock::now();
    auto timeSeconds = std::chrono::floor<std::chrono::seconds>(time);

    return std::format("{:%T}", timeSeconds);
}

std::string_view Zappy::Utils::Logger::levelColorSequence(Level level) const
{
    if (!m_Colors) return "";

    switch (level) {
        case DEBUG: return "\033[35m";
        case INFO: return "\033[34m";
        case WARNING: return "\033[33m";
        case ERROR: return "\033[31m";

        default: return "";
    }
}

std::string_view Zappy::Utils::Logger::clearColorSequence() const
{
    return m_Colors
        ? "\033[0m"
        : "";
}

std::ostream& operator<<(std::ostream& stream, Zappy::Utils::Logger::Level level)
{
    switch (level) {
        case Zappy::Utils::Logger::DEBUG:   return stream << "DEBUG";
        case Zappy::Utils::Logger::INFO:    return stream << "INFO";
        case Zappy::Utils::Logger::WARNING: return stream << "WARNING";
        case Zappy::Utils::Logger::ERROR:   return stream << "ERROR";

        default: return stream << "???";
    }
}
