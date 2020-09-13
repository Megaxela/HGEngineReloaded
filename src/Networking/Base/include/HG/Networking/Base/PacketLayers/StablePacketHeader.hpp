#pragma once

// STL
#include <cstddef>
#include <cstdint>
#include <vector>

namespace HG::Networking::Base::PacketLayers
{
/**
     * @brief Stable packet header.
     *
     * `magic` - any magic number to identify, that it's packet is from engine client.
     * `dataSize` - size of following useful load.
     */
struct StablePacketHeader
{
    static constexpr std::size_t kSize = 10; // bytes

    std::uint16_t magic;
    std::uint64_t dataSize;

    static StablePacketHeader parse(std::vector<std::byte>& buffer);
};
} // namespace HG::Networking::Base::PacketLayers
