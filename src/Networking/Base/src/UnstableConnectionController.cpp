// HG::Networking::Base
#include <HG/Networking/Base/UnstableConnectionController.hpp>
#include <iostream>

namespace HG::Networking::Base
{
static constexpr std::uint16_t kMagic = 0xDDFF;

UnstableConnectionController::UnstableConnectionController() :
    m_initialPacketsLeft(sizeof(m_receiveBitfield) * 8),
    m_packetNumber(0),
    m_lastReceivedPacketNumber(0),
    m_lastRemoteReceivedPacketNumber(0),
    m_receiveBitfield(0),
    m_ackBitfield(0),
    m_lostSentPackets(0),
    m_lostReceivePackets(0)
{
}

HG::Networking::Base::PacketLayers::UnstablePacketHeader UnstableConnectionController::getNextPacketHeader()
{
    HG::Networking::Base::PacketLayers::UnstablePacketHeader result{0};

    result.magic                   = kMagic;
    result.packetIndex             = m_packetNumber++;
    result.lastReceivedPacketIndex = m_lastReceivedPacketNumber;
    result.remoteReceiveBitfield   = m_receiveBitfield;
    result.remoteAckBitfield       = m_ackBitfield;

    return result;
}

std::uint32_t UnstableConnectionController::lostSentPackets() const
{
    return m_lostSentPackets;
}

std::uint32_t UnstableConnectionController::lostReceivePackets() const
{
    return m_lostReceivePackets;
}

bool UnstableConnectionController::proceedReceivedPacketHeader(const PacketLayers::UnstablePacketHeader& header)
{
    // wrong magic
    if (header.magic != kMagic)
    {
        return false;
    }

    // todo: check if it could be rounded
    auto receivePacketOffset = std::int32_t(header.packetIndex) - std::int32_t(m_lastReceivedPacketNumber);

    // First packet is 0, so we need to proceed it
    if (m_initialPacketsLeft == sizeof(m_receiveBitfield) * 8)
    {
        receivePacketOffset = 1;
    }

    // If it's packet from past
    if (receivePacketOffset < 0)
    {
        // Mark, that we actually received it
        m_receiveBitfield |= (1u << std::uint32_t(receivePacketOffset * -1));
        return false;
    }

    // If we skipped some packets before this one
    // mark all that as skipped
    for (std::int32_t i = 0; i < receivePacketOffset; ++i)
    {
        // Counting actually skipped packets, if we already received
        // enough packets to decide
        if (m_initialPacketsLeft == 0)
        {
            // Was 32nd packet behind last lost?
            if ((m_receiveBitfield & (1u << ((sizeof(m_receiveBitfield) * 8) - 1u))) == 0)
            {
                m_lostReceivePackets += 1;
            }
        }
        else
        {
            --m_initialPacketsLeft;
        }

        m_receiveBitfield <<= 1u;
    }

    // Mark this one as received
    m_receiveBitfield |= 1u;

    // Setting last received packet as this one
    m_lastReceivedPacketNumber = header.packetIndex;

    // Processing remote info
    return true;
}
} // namespace HG::Networking::Base
