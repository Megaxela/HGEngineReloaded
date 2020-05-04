// HG::Networking::Base
#include <HG/Networking/Base/PacketLayers/StablePacketHeader.hpp>

// ByteArray
#include <bytearray_processor.hpp>

namespace HG::Networking::Base::PacketLayers
{
    StablePacketHeader StablePacketHeader::parse(std::vector<std::byte>& buffer)
    {
        bytearray_processor bytearray(buffer);

        StablePacketHeader result{0};

        std::size_t i = 0;
        result.magic = bytearray.read<typeof(result.magic)>(i);
        i += sizeof(result.magic);
        result.dataSize = bytearray.read<typeof(result.dataSize)>(i);

        return result;
    }
}