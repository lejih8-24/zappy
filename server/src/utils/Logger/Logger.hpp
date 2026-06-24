/*
** EPITECH PROJECT, 2026
** Project - FTP
** File description:
** Header file for
** Logger class
*/

#pragma once

#include <string_view>
#include <iostream>


namespace Zappy::Utils {
    class Logger {
        public:
            enum Level {
                ROOT,
                DEBUG,
                INFO,
                WARNING,
                ERROR,
            };

        private:
            static std::ostream& s_VoidStream;

            std::ostream* m_Output;
            Level m_LogLevel;
            bool m_Colors;
            std::string m_Name;

        public:
            Logger(std::string name, std::ostream& output = std::clog);

            static Logger& getRootLogger();

            inline void setLevel(Level newLvl) { m_LogLevel = newLvl; }
            Level logLevel() const;

            inline std::string& name() { return m_Name; }
            inline const std::string& name() const { return m_Name; }

            inline void enableColors(bool option = true) { m_Colors = option; }

            std::ostream& log(Level level);

            inline std::ostream& debug()   { return log(DEBUG); }
            inline std::ostream& info()    { return log(INFO); }
            inline std::ostream& warning() { return log(WARNING); }
            inline std::ostream& error()   { return log(ERROR); }

            static std::string escape(std::string_view input);
            static std::string escape(std::string_view input, char delim);

        private:
            std::string currentTime();

            static char unescape(char c);
            static void pushEscaped(std::string& buffer, char c);

            std::string_view levelColorSequence(Level level) const;
            std::string_view clearColorSequence() const;
    };
}

namespace Zappy {
    inline Utils::Logger& logger = Utils::Logger::getRootLogger();
}

std::ostream& operator<<(std::ostream& stream, Zappy::Utils::Logger::Level level);
