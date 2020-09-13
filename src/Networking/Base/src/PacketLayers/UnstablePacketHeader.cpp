// HG::Networking::Base
#include <HG/Networking/Base/PacketLayers/UnstablePacketHeader.hpp>

// ByteArray
#include <bytearray_processor.hpp>

#define READ_FIELD(FIELD)                     \
    FIELD = bytearray.read<typeof(FIELD)>(i); \
    i += sizeof(FIELD)

namespace HG::Networking::Base::PacketLayers
{
UnstablePacketHeader UnstablePacketHeader::parse(std::vector<std::byte>& buffer)
{
    bytearray_processor bytearray(buffer);

    UnstablePacketHeader result{0};

    std::size_t i = 0;
    READ_FIELD(result.magic);
    READ_FIELD(result.packetIndex);
    READ_FIELD(result.lastReceivedPacketIndex);
    READ_FIELD(result.remoteReceiveBitfield);
    READ_FIELD(result.remoteAckBitfield);
    READ_FIELD(result.dataSize);

    return result;
}
} // namespace HG::Networking::Base::PacketLayers
