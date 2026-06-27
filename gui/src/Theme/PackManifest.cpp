/*
** EPITECH PROJECT, 2026
** Project - Zappy
** File description:
** Pack manifest.json loader (read once, parse on demand)
*/

#include "Theme/PackManifest.hpp"

#include <algorithm>
#include <fstream>
#include <regex>
#include <sstream>

static constexpr std::array<const char *, 7> RESOURCE_NAMES = {
    "food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"
};

static std::size_t findMatchingBrace(const std::string &json, std::size_t openPos)
{
    int depth = 0;
    for (std::size_t i = openPos; i < json.size(); ++i) {
        if (json[i] == '{') ++depth;
        else if (json[i] == '}') { --depth; if (depth == 0) return i; }
    }
    return std::string::npos;
}

static std::string parseResourceBlock(const std::string &json, std::string_view resourceName)
{
    std::string key = std::string("\"") + std::string(resourceName) + "\"";
    auto pos = json.find(key);
    if (pos == std::string::npos)
        return {};
    auto open = json.find('{', pos + key.size());
    if (open == std::string::npos)
        return {};
    auto close = findMatchingBrace(json, open);
    if (close == std::string::npos)
        return {};
    return json.substr(open + 1, close - open - 1);
}

// Fills a Vec3 from the `"x"/"y"/"z": <signed float>` pairs found in [begin, end).
static GUI::Vec3 parseAxes(std::string::const_iterator begin, std::string::const_iterator end)
{
    GUI::Vec3 v = {0, 0, 0};
    // matches `"x": 180` or `"z": -90.5`; group 1 = axis letter, group 2 = signed float
    static const std::regex axis("\"([xyz])\"\\s*:\\s*(-?\\d+(?:\\.\\d+)?)");
    for (std::sregex_iterator it(begin, end, axis), stop; it != stop; ++it) {
        char a = (*it)[1].str()[0];
        float f = std::stof((*it)[2].str());
        if (a == 'x') v.x = f;
        else if (a == 'y') v.y = f;
        else if (a == 'z') v.z = f;
    }
    return v;
}

namespace GUI {

PackManifest::PackManifest(std::string_view packName)
{
    std::string manifestPath = std::string(PACKS_DIR) + std::string(packName) + "/manifest.json";
    std::ifstream file(manifestPath);
    if (!file)
        return;
    std::ostringstream buf;
    buf << file.rdbuf();
    _json = buf.str();
}

std::unordered_map<std::string, int> PackManifest::animations() const
{
    std::unordered_map<std::string, int> result;

    auto animStart = _json.find("\"animations\"");
    if (animStart == std::string::npos)
        return result;
    auto blockStart = _json.find('{', animStart);
    auto blockEnd = _json.find('}', blockStart);
    if (blockStart == std::string::npos || blockEnd == std::string::npos)
        return result;

    std::string block = _json.substr(blockStart + 1, blockEnd - blockStart - 1);
    static const std::regex entry("\"([^\"]+)\"\\s*:\\s*(\\d+)");
    for (std::sregex_iterator it(block.begin(), block.end(), entry), end; it != end; ++it)
        result[(*it)[1].str()] = std::stoi((*it)[2].str());

    return result;
}

Color PackManifest::backgroundColor() const
{
    auto blockStart = _json.find("\"backgroundColor\"");
    if (blockStart == std::string::npos)
        return Colors::DarkBlue;
    auto blockOpen = _json.find('{', blockStart);
    auto blockClose = _json.find('}', blockOpen);
    if (blockOpen == std::string::npos || blockClose == std::string::npos)
        return Colors::DarkBlue;

    Color c = Colors::DarkBlue;
    static const std::regex channel("\"([rgb])\"\\s*:\\s*(\\d+)");
    for (std::sregex_iterator it(_json.cbegin() + blockOpen, _json.cbegin() + blockClose, channel), end;
        it != end; ++it) {
        char ch = (*it)[1].str()[0];
        int v = std::clamp(std::stoi((*it)[2].str()), 0, 255);
        if (ch == 'r') c.r = static_cast<unsigned char>(v);
        else if (ch == 'g') c.g = static_cast<unsigned char>(v);
        else if (ch == 'b') c.b = static_cast<unsigned char>(v);
    }
    c.a = 255;
    return c;
}

Vec3 PackManifest::rotation(const char *blockName) const
{
    auto blockStart = _json.find(std::string("\"") + blockName + "\"");
    if (blockStart == std::string::npos)
        return {0, 0, 0};
    // clamp search to the {...} block so later fields with the same axis keys don't overwrite
    auto blockOpen = _json.find('{', blockStart);
    auto blockClose = _json.find('}', blockOpen);
    if (blockOpen == std::string::npos || blockClose == std::string::npos)
        return {0, 0, 0};
    return parseAxes(_json.cbegin() + blockOpen, _json.cbegin() + blockClose);
}

float PackManifest::number(const char *key, float defaultValue, float minValue, float maxValue) const
{
    // clamp the default too so a bad default can't bypass the range check
    defaultValue = std::clamp(defaultValue, minValue, maxValue);

    std::regex r(std::string("\"") + key + "\"\\s*:\\s*(\\d+(?:\\.\\d+)?)");
    std::smatch m;
    if (std::regex_search(_json, m, r))
        return std::clamp(std::stof(m[1].str()), minValue, maxValue);
    return defaultValue;
}

std::array<PackManifest::ResourceOverride, 7> PackManifest::resourceOverrides() const
{
    std::array<ResourceOverride, 7> result{};

    static const std::regex scaleRe("\"scale\"\\s*:\\s*(\\d+(?:\\.\\d+)?)");

    for (std::size_t i = 0; i < RESOURCE_NAMES.size(); ++i) {
        std::string block = parseResourceBlock(_json, RESOURCE_NAMES[i]);
        if (block.empty())
            continue;

        std::smatch sm;
        if (std::regex_search(block, sm, scaleRe))
            result[i].scale = std::clamp(std::stof(sm[1].str()), 0.000001f, 100.0f);

        auto subBlock = [&](const char *subKey) -> std::string {
            auto pos = block.find(std::string("\"") + subKey + "\"");
            if (pos == std::string::npos) return {};
            auto open = block.find('{', pos);
            if (open == std::string::npos) return {};
            auto close = findMatchingBrace(block, open);
            if (close == std::string::npos) return {};
            return block.substr(open + 1, close - open - 1);
        };

        std::string rotBlock = subBlock("rotation");
        if (!rotBlock.empty())
            result[i].rotation = parseAxes(rotBlock.cbegin(), rotBlock.cend());

        std::string transBlock = subBlock("translation");
        if (!transBlock.empty())
            result[i].translation = parseAxes(transBlock.cbegin(), transBlock.cend());
    }
    return result;
}

}
