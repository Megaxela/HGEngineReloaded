#pragma once

// HG::Networking::Base
#include <HG/Networking/Base/AddressIPv4.hpp>

// HG::Utils
#include <HG/Utils/Platform.hpp>

// STL
#include <functional>

// Platform dependent
#ifdef OS_WINDOWS
#    include <windows.h>
#    include <winsock2.h>
#endif

namespace HG::Networking::Base::LowLevel
{
// Platform dependent definitions
#ifdef OS_WINDOWS
using Socket          = SOCKET;
using InternalAddress = SOCKADDR_IN;

struct DescriptorSet
{
    fd_set set{0};
    typeof(Socket) maxFD = 0;
};

static constexpr Socket kInvalidSocket     = INVALID_SOCKET;
static constexpr std::uint32_t kAnyAddress = INADDR_ANY;

struct SystemInfo
{
    SystemInfo();
    ~SystemInfo();

    WSADATA wsaData;
};
#endif

/**
 * @brief Structure, that describes new connection
 * from stable connection.
 */
struct NewConnection
{
    InternalAddress internalAddress;
    Socket socket;
};

/**
 * @brief Function for creating unstable connection
 * socket.
 */
Socket createUDPSocket();

/**
 * @brief Function for creating stable connection
 * listening socket.
 */
Socket createTCPSocket();

/**
 * @brief Function for binding address to socket.
 * @return Success.
 */
bool bindSocketWithAddress(Socket socket, const InternalAddress& addr);

/**
 * @brief Function for closing socket.
 */
void closeSocket(Socket sock);

/**
 * @brief Function for creating internal address structure.
 */
InternalAddress createInternalIPv4Address(std::uint32_t addr, std::uint16_t port);

/**
 * @brief Function for converting internal address to external address structure.
 */
HG::Networking::Base::AddressIPv4 internalToExternalAddress(const InternalAddress& address);

/**
 * @brief Function for creating descriptor set.
 */
DescriptorSet createDescriptorSet();

/**
 * @brief Function for adding socket to descriptor set.
 */
void addToDescriptorSet(DescriptorSet& set, Socket sock);

/**
 * @brief Function for waiting until one of sockets in descriptor set
 * will be available for read.
 * @return Is any of sockets is available for read? False if timeout happened.
 */
bool waitDescriptorSet(DescriptorSet& set, std::chrono::milliseconds timeout);

/**
 * @brief Function for checking is socket inside descriptor set.
 */
bool isDescriptorReady(Socket socket, DescriptorSet& set);

/**
 * @brief Function that performs visiting all sockets in descriptor set.
 */
void applyAtDescriptors(const DescriptorSet& set, std::function<void(Socket sock)> func);

/**
 * @brief Function for getting size of descriptor set.
 */
int descriptorSetSize(const DescriptorSet& set);

/**
 * @brief Function for receiving new connection from stable listening socket.
 * Socket requires `setSocketToListeningMode` before.
 */
NewConnection acceptNewConnection(Socket sock);

/**
 * @brief Function for reading data from connected nonblocking stable socket.
 * It reads data to the end of buffer and extend it's size due to real read amount.
 * @return Was read successful.
 */
bool readFromStableSocket(Socket socket, std::vector<std::byte>& buffer, std::size_t size);

/**
 * @brief Function for reading data from unstable socket.
 * @return Was read successful.
 */
bool readFromUnstableSocket(Socket sock, InternalAddress& address, std::vector<std::byte>& buffer, std::size_t size);

/**
 * @brief Function for setting socket to nonblocking mode.
 */
void setSocketToNonblockingMode(Socket sock);

/**
 * @brief Function for setting socket to listening mode.
 */
bool setSocketToListeningMode(Socket sock);
} // namespace HG::Networking::Base::LowLevel
