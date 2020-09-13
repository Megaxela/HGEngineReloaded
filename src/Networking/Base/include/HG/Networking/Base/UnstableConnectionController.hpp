#pragma once

// HG::Networking::Base
#include <HG/Networking/Base/PacketLayers/UnstablePacketHeader.hpp>

namespace HG::Networking::Base
{
/**
     * @brief Class, that describes controller, that manages
     * unstable connection metrics/low level logic.
     */
class UnstableConnectionController
{
public:
    /**
         * @brief Constructor.
         */
    UnstableConnectionController();

    /**
         * @brief Method, that will return next packet
         * header without data size field.
         */
    HG::Networking::Base::PacketLayers::UnstablePacketHeader getNextPacketHeader();

    /**
         * @brief Method, that process new packet header.
         * @return Does this packet is relative. If not - it must be thrown
         * away.
         */
    bool proceedReceivedPacketHeader(const HG::Networking::Base::PacketLayers::UnstablePacketHeader& header);

    /**
         * @brief Method for getting amount of lost sent packets.
         */
    [[nodiscard]] std::uint32_t lostSentPackets() const;

    /**
         * @brief Method for getting amount of lost sent packet by remote host.
         */
    [[nodiscard]] std::uint32_t lostReceivePackets() const;

private:
    std::uint8_t m_initialPacketsLeft;

    std::uint16_t m_packetNumber;
    std::uint16_t m_lastReceivedPacketNumber;
    std::uint16_t m_lastRemoteReceivedPacketNumber;

    std::uint32_t m_receiveBitfield;
    std::uint32_t m_ackBitfield;

    std::uint32_t m_lostSentPackets;
    std::uint32_t m_lostReceivePackets;
};
} // namespace HG::Networking::Base
