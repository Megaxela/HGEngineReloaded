// HG::Networking::Base
#include <HG/Networking/Base/UnstableConnectionController.hpp>

// GTest
#include <gtest/gtest.h>

HG::Networking::Base::PacketLayers::UnstablePacketHeader createHeader(std::uint16_t packetNumber)
{
    HG::Networking::Base::PacketLayers::UnstablePacketHeader header{0};

    header.magic       = 0xDDFF;
    header.packetIndex = packetNumber;

    return header;
}

TEST(NetworkingBase, UnstableConnetionController)
{
    HG::Networking::Base::UnstableConnectionController controller;

    ASSERT_EQ(controller.lostReceivePackets(), 0);
    ASSERT_EQ(controller.lostSentPackets(), 0);

    // Skip second packet
    ASSERT_EQ(controller.proceedReceivedPacketHeader(createHeader(0)), true);

    auto generatedHeader = controller.getNextPacketHeader();
    ASSERT_EQ(generatedHeader.packetIndex, 0);
    ASSERT_EQ(generatedHeader.lastReceivedPacketIndex, 0);
    ASSERT_EQ(generatedHeader.remoteReceiveBitfield, 0b00000000'00000000'00000000'00000001);
    ASSERT_EQ(generatedHeader.remoteAckBitfield, 0x00000000);
    ASSERT_EQ(controller.lostReceivePackets(), 0);
    ASSERT_EQ(controller.lostSentPackets(), 0);

    ASSERT_EQ(controller.proceedReceivedPacketHeader(createHeader(2)), true);

    generatedHeader = controller.getNextPacketHeader();
    ASSERT_EQ(generatedHeader.packetIndex, 1);
    ASSERT_EQ(generatedHeader.lastReceivedPacketIndex, 2);
    ASSERT_EQ(generatedHeader.remoteReceiveBitfield, 0b00000000'00000000'00000000'00000101);
    ASSERT_EQ(generatedHeader.remoteAckBitfield, 0x00000000);
    ASSERT_EQ(controller.lostReceivePackets(), 0);
    ASSERT_EQ(controller.lostSentPackets(), 0);

    for (std::uint16_t i = 3; i < 32; ++i)
    {
        ASSERT_EQ(controller.proceedReceivedPacketHeader(createHeader(i)), true);
    }

    generatedHeader = controller.getNextPacketHeader();
    ASSERT_EQ(generatedHeader.packetIndex, 2);
    ASSERT_EQ(generatedHeader.lastReceivedPacketIndex, 31);
    ASSERT_EQ(generatedHeader.remoteReceiveBitfield, 0b10111111'11111111'11111111'11111111);
    ASSERT_EQ(generatedHeader.remoteAckBitfield, 0x00000000);
    ASSERT_EQ(controller.lostReceivePackets(), 0);
    ASSERT_EQ(controller.lostSentPackets(), 0);

    ASSERT_EQ(controller.proceedReceivedPacketHeader(createHeader(32)), true);

    generatedHeader = controller.getNextPacketHeader();
    ASSERT_EQ(generatedHeader.packetIndex, 3);
    ASSERT_EQ(generatedHeader.lastReceivedPacketIndex, 32);
    ASSERT_EQ(generatedHeader.remoteReceiveBitfield, 0b01111111'11111111'11111111'11111111);
    ASSERT_EQ(generatedHeader.remoteAckBitfield, 0x00000000);
    ASSERT_EQ(controller.lostReceivePackets(), 0);
    ASSERT_EQ(controller.lostSentPackets(), 0);

    ASSERT_EQ(controller.proceedReceivedPacketHeader(createHeader(33)), true);

    generatedHeader = controller.getNextPacketHeader();
    ASSERT_EQ(generatedHeader.packetIndex, 4);
    ASSERT_EQ(generatedHeader.lastReceivedPacketIndex, 33);
    ASSERT_EQ(generatedHeader.remoteReceiveBitfield, 0b11111111'11111111'11111111'11111111);
    ASSERT_EQ(generatedHeader.remoteAckBitfield, 0x00000000);
    ASSERT_EQ(controller.lostReceivePackets(), 1);
    ASSERT_EQ(controller.lostSentPackets(), 0);

    ASSERT_EQ(controller.proceedReceivedPacketHeader(createHeader(38)), true);

    generatedHeader = controller.getNextPacketHeader();
    ASSERT_EQ(generatedHeader.packetIndex, 5);
    ASSERT_EQ(generatedHeader.lastReceivedPacketIndex, 38);
    ASSERT_EQ(generatedHeader.remoteReceiveBitfield, 0b11111111'11111111'11111111'11100001);
    ASSERT_EQ(generatedHeader.remoteAckBitfield, 0x00000000);
    ASSERT_EQ(controller.lostReceivePackets(), 1);
    ASSERT_EQ(controller.lostSentPackets(), 0);

    for (std::uint16_t i = 39; i < 66; ++i)
    {
        ASSERT_EQ(controller.proceedReceivedPacketHeader(createHeader(i)), true);
    }

    generatedHeader = controller.getNextPacketHeader();
    ASSERT_EQ(generatedHeader.packetIndex, 6);
    ASSERT_EQ(generatedHeader.lastReceivedPacketIndex, 65);
    ASSERT_EQ(generatedHeader.remoteReceiveBitfield, 0b00001111'11111111'11111111'11111111);
    ASSERT_EQ(generatedHeader.remoteAckBitfield, 0x00000000);
    ASSERT_EQ(controller.lostReceivePackets(), 1);
    ASSERT_EQ(controller.lostSentPackets(), 0);

    ASSERT_EQ(controller.proceedReceivedPacketHeader(createHeader(69)), true);

    generatedHeader = controller.getNextPacketHeader();
    ASSERT_EQ(generatedHeader.packetIndex, 7);
    ASSERT_EQ(generatedHeader.lastReceivedPacketIndex, 69);
    ASSERT_EQ(generatedHeader.remoteReceiveBitfield, 0b11111111'11111111'11111111'11110001);
    ASSERT_EQ(generatedHeader.remoteAckBitfield, 0x00000000);
    ASSERT_EQ(controller.lostReceivePackets(), 5);
    ASSERT_EQ(controller.lostSentPackets(), 0);
}
