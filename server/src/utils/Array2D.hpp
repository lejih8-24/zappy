/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Header file for
** Array2D class
*/

#pragma once

#include <zappy/exceptions.hpp>
#include <type_traits>
#include <vector>


namespace Zappy::Utils {
    /**
     * 2D array class.
     *
     * Note:
     * The reason we use vectors
     * instead of arrays to allow for
     * vectors with sizes only known
     * at runtime.
     * Despite the array's dynamic
     * storage, dynamically resizing
     * the array is disallowed for
     * simplicity.
     */
    template <typename T>
    requires (std::is_copy_constructible_v<T>)
    class Array2D {
        public:
            using Element = T;

        private:
            std::size_t m_SizeX;
            std::size_t m_SizeY;
            std::vector<Element> m_Data;

        public:
            Array2D(std::size_t width, std::size_t height) requires (std::is_default_constructible_v<Element>)
                : m_SizeX(width)
                , m_SizeY(height)
                , m_Data(width * height)
            {}

            Array2D(std::size_t width, std::size_t height, const Element& fill)
                : m_SizeX(width)
                , m_SizeY(height)
                , m_Data(width * height, fill)
            {}

            constexpr std::pair<std::size_t, std::size_t> size() const noexcept { return { m_SizeX, m_SizeY }; }

            Element& operator[](std::pair<std::size_t, std::size_t> index) noexcept
            {
                auto [x, y] = index;
                return m_Data[y * m_SizeX + x];
            }

            const Element& operator[](std::pair<std::size_t, std::size_t> index) const noexcept
            {
                auto [x, y] = index;
                return m_Data[y * m_SizeX + x];
            }

            Element& at(std::pair<std::size_t, std::size_t> index) { return at(index.first, index.second); }
            Element& at(std::size_t x, std::size_t y)
            {
                if (x >= m_SizeX)
                    throw Exceptions::OutOfRange("Zappy::Utils::Array2D::at(): x >= m_SizeX");
                if (y >= m_SizeY)
                    throw Exceptions::OutOfRange("Zappy::Utils::Array2D::at(): y >= m_SizeY");

                return m_Data[y * m_SizeX + x];
            }

            const Element& at(std::pair<std::size_t, std::size_t> index) const { return at(index.first, index.second); }
            const Element& at(std::size_t x, std::size_t y) const
            {
                if (x >= m_SizeX)
                    throw Exceptions::OutOfRange("Zappy::Utils::Array2D::at(): x >= m_SizeX");
                if (y >= m_SizeY)
                    throw Exceptions::OutOfRange("Zappy::Utils::Array2D::at(): y >= m_SizeY");

                return m_Data[y * m_SizeX + x];
            }

            auto begin() { return m_Data.begin(); }
            auto end() { return m_Data.end(); }

            auto begin() const { return m_Data.begin(); }
            auto end() const { return m_Data.end(); }

            auto cbegin() const { return m_Data.cbegin(); }
            auto cend() const { return m_Data.cbegin(); }
    };
}
