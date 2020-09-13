#include <HG/Utils/Platform.hpp>
#ifdef OS_LINUX
// HG::Networking::Base
#    include <HG/Networking/Base/LowLevel.hpp>

// system
#    include <sys/ioctl.h>
#    include <sys/select.h>

namespace HG::Networking::Base::LowLevel
{
SystemInfo::SystemInfo()
{
}

SystemInfo::~SystemInfo()
{
}

Socket createUDPSocket()
{
    return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

Socket createTCPSocket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

bool bindSocketWithAddress(Socket socket, const InternalAddress& addr)
{
    return bind(socket, (sockaddr*)&addr, sizeof(InternalAddress)) != -1;
}

void closeSocket(Socket sock)
{
    close(sock);
}

InternalAddress createInternalIPv4Address(std::uint32_t addr, std::uint16_t port)
{
    InternalAddress address = {0};
    address.sin_family      = AF_INET;
    address.sin_port        = htons(port);
    address.sin_addr.s_addr = htonl(addr);

    return address;
}

HG::Networking::Base::AddressIPv4 internalToExternalAddress(const InternalAddress& address)
{
    return HG::Networking::Base::AddressIPv4(ntohl(address.sin_addr.s_addr), ntohs(address.sin_port));
}

DescriptorSet createDescriptorSet()
{
    DescriptorSet set;
    FD_ZERO(&set.set);

    return set;
}

void addToDescriptorSet(DescriptorSet& set, Socket sock)
{
    FD_SET(sock, &set.set);
    set.maxFD = std::max(set.maxFD, sock);
}

bool waitDescriptorSet(DescriptorSet& set, std::chrono::milliseconds timeout)
{
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);

    timeval timeoutStruct{0};
    timeoutStruct.tv_sec = seconds.count();

    return select(set.maxFD, &set.set, nullptr, nullptr, &timeoutStruct) != 0;
}

bool isDescriptorReady(Socket socket, DescriptorSet& set)
{
    return FD_ISSET(socket, &set);
}

void applyAtDescriptors(const DescriptorSet& set, std::function<void(Socket sock)> func)
{
    for (std::uint32_t i = 0; i < set.set.fd_count; ++i)
    {
        func(set.set.fd_array[i]);
    }
}

int descriptorSetSize(const DescriptorSet& set)
{
    return set.set.fd_count;
}

NewConnection acceptNewConnection(Socket sock)
{
    NewConnection newConnection{0};
    socklen_t len = static_cast<socklen_t>(sizeof(newConnection.internalAddress));

    newConnection.socket = accept(sock, (sockaddr*)&newConnection.internalAddress, &len);

    return newConnection;
}

bool readFromStableSocket(Socket socket, std::vector<std::byte>& buffer, std::size_t size)
{
    auto oldSize = buffer.size();

    buffer.resize(size);

    int actuallyReceived = recv(socket, (char*)(buffer.data() + oldSize), (int)(size - oldSize), 0);

    switch (actuallyReceived)
    {
    case 0:
        buffer.resize(oldSize);
        return true;
    case -1:
        buffer.resize(oldSize);
        return false;
    default:
        break;
    }

    buffer.resize(oldSize + actuallyReceived);

    return true;
}

bool readFromUnstableSocket(Socket sock, InternalAddress& address, std::vector<std::byte>& buffer, std::size_t size)
{
    auto oldSize = buffer.size();
    buffer.resize(oldSize + size);

    int len = sizeof(InternalAddress);

    recvfrom(sock, (char*)(buffer.data() + oldSize), size, 0, (sockaddr*)&address, &len);

    return true;
}

void setSocketToNonblockingMode(Socket sock)
{
    u_long mode = 0;
    ioctl(sock, FIONBIO, &mode);
}

bool setSocketToListeningMode(Socket sock)
{
    return listen(sock, 10) == 0;
}

} // namespace HG::Networking::Base::LowLevel

#endif
