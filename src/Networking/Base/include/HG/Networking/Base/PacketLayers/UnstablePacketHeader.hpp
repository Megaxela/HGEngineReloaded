#pragma once

// STL
#include <cstdint>
#include <vector>
#include <cstddef>

namespace HG::Networking::Base::PacketLayers
{
    /**
     * @brief Unstable packet header.
     *
     * Format:
     * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
     * |   MAGIC   |  Packet N | RPacketN  |Remote Receive Bitfield|  Remote Ack Bitfield  | Data Size |
     * #-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#-----#
     *
     * [ 0 -  2] - any magic number to identify, that it's packet from engine client.
     * [ 2 -  4] - packet number [0 - 65'535]
     * [ 4 -  6] - last received packet number by remote host [0 - 65'535]
     * [ 6 - 10] - bitfield with remote received packets info (each bit represents packet, 1 - received, 0 - not)
     * [10 - 14] - bitfield with remote ack packets received info (each bit represents packet, 1 - received, 0 - not)
     * [14 - 16] - amount of actual data in bytes
     */
    struct UnstablePacketHeader
    {
        static constexpr std::size_t kSize = 16;

        std::uint16_t magic;

        std::uint16_t packetIndex;
        std::uint16_t lastReceivedPacketIndex;

        std::uint32_t remoteReceiveBitfield;
        std::uint32_t remoteAckBitfield;

        std::uint16_t dataSize;

        static UnstablePacketHeader parse(std::vector<std::byte>& buffer);
    };
}