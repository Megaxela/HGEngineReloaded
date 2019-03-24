#pragma once

#include <filesystem>
#include <functional>

namespace std
{
    template<>
    struct hash<std::filesystem::path>
    {
        std::size_t operator()(const std::filesystem::path& p) const
        {
            return std::hash<std::string>()(p.string());
        }
    };
}

