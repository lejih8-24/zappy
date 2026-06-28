/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Pack manifest.json loader (read once, parse on demand)
*/

#pragma once

#include "Graphics/Types.hpp"

#include <array>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace GUI {

// Parsed view of a pack's manifest.json. The file is read once at construction;
// every accessor parses that cached buffer, so a missing or partial manifest just
// yields the provided defaults instead of re-reading the file for each field.
class PackManifest {
    public:
        struct ResourceOverride {
            std::optional<float> scale;
            std::optional<Vec3> rotation;
            std::optional<Vec3> translation;
        };

        explicit PackManifest(std::string_view packName);

        std::unordered_map<std::string, int> animations() const;
        Color backgroundColor() const;
        Vec3 rotation(const char *blockName) const;
        float number(const char *key, float defaultValue,
            float minValue = -1e30f, float maxValue = 1e30f) const;
        std::array<ResourceOverride, 7> resourceOverrides() const;

    private:
        std::string _json;
};

}
