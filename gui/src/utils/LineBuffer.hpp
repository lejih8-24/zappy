/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** LineBuffer utility class
*/

#pragma once

#include <string_view>
#include <algorithm>
#include <optional>
#include <cstdint>
#include <string>
#include <array>
#include <span>


namespace Zappy::Utils {
    template <std::size_t S>
    class LineBuffer {
        std::array<char, S> m_Buffer;
        std::size_t m_BytesFilled;

        static constexpr char DEFAULT_DELIMITER = '\n';

        public:
            constexpr LineBuffer() : m_Buffer(), m_BytesFilled(0) {}

            /**
             * Returns the amount of occupied
             * bytes in the line buffer.
             */
            constexpr std::size_t size() const noexcept { return m_BytesFilled; }

            /**
             * Returns the maximum capacity
             * of the line buffer.
             */
            constexpr std::size_t capacity() const noexcept { return S; }

            /**
             * Pushes the given data to the
             * end of the line buffer.
             */
            constexpr void pushBack(std::span<char> data)
            {
                std::size_t remainingBytes = S - m_BytesFilled;
                if (remainingBytes == 0)
                    return;

                // Only copy at most as many bytes as are available
                std::size_t dataSize = std::min(data.size(), remainingBytes);
                auto dataEnd = data.begin();
                std::advance(dataEnd, dataSize);

                auto bufferStart = m_Buffer.begin();
                std::advance(bufferStart, m_BytesFilled);

                std::copy(data.begin(), dataEnd, bufferStart);
                m_BytesFilled += dataSize;
            }

            /**
             * Retrieves the first line of
             * the buffer, if at least a single
             * line is present.
             */
            constexpr std::optional<std::string_view> getLine(char delimiter = DEFAULT_DELIMITER) const
            {
                auto lineLength = findDelimiter(delimiter);

                if (lineLength < 0)
                    return {};

                return std::string_view(m_Buffer.data(), lineLength);
            }

            /**
             * Removes the first line in the buffer,
             * if any is present. Does nothing if no
             * complete line is present.
             *
             * Note:
             * This will invalidate all string views
             * returned by this class.
             */
            constexpr void popLine(char delimiter = DEFAULT_DELIMITER)
            {
                auto lineLength = findDelimiter(delimiter);
                if (lineLength < 0)
                    return;

                *this <<= lineLength + 1;  // +1 to also get rid of delimiter
            }

            /**
             * Retrieves and pops the first line
             * of the character buffer, if at least
             * a single line is present.
             */
            std::optional<std::string> extractLine(char delimiter = DEFAULT_DELIMITER)
            {
                auto line = getLine();
                if (!line)
                    return {};

                std::optional<std::string> ownedLine = line;
                *this <<= line->length() + 1;
                return ownedLine;
            }

            /**
             * Shifts the filled part of the buffer by the
             * specified offset to the left, effectively
             * ridding the buffer of the first `offset` bytes.
             */
            constexpr void operator<<=(std::size_t offset)
            {
                offset = std::min(offset, m_BytesFilled);
                if (offset == 0)
                    return;

                m_BytesFilled -= offset;

                for (std::size_t i = 0; i < m_BytesFilled; ++i) {
                    m_Buffer[i] = m_Buffer[i + offset];
                }
            }

            /**
             * Shifts the filled part of the buffer by the
             * specified offset to the right, effectively
             * filling pushing `offset` null bytes to the
             * start of the buffer.
             */
            constexpr void operator>>=(std::size_t offset)
            {
                if (offset == 0)
                    return;

                m_BytesFilled += offset;

                for (std::size_t i = m_BytesFilled; i >= offset; --i) {
                    m_Buffer[i] = m_Buffer[i - offset];
                }

                for (std::size_t i = 0; i < offset; ++i) {
                    m_Buffer[i] = '\0';
                }
            }

        private:
            /**
             * Returns the index of the specified
             * delimiter in the character buffer,
             * or a negative value if the delimiter
             * isn't present.
             *
             * Note:
             * A return value of 0 is valid.
             */
            constexpr std::int64_t findDelimiter(char delimiter) const
            {
                if (m_BytesFilled == 0)
                    return -1;

                auto bufferEnd = m_Buffer.begin();
                std::advance(bufferEnd, m_BytesFilled);

                auto lineEnd = std::find(m_Buffer.begin(), bufferEnd, delimiter);
                if (lineEnd == bufferEnd)
                    return -1;

                return std::distance(m_Buffer.begin(), lineEnd);
            }
    };
}
