// HG::Core
#include <HG/Core/Application.hpp>
#include <HG/Core/ThreadPool.hpp>

// HG::Networking::Base
#include <HG/Networking/Base/Server.hpp>
#include <HG/Networking/Base/LowLevel.hpp>
#include <HG/Networking/Base/AbstractInternalData.hpp>
#include <HG/Networking/Base/StableConnection.hpp>
#include <HG/Networking/Base/PacketLayers/StablePacketHeader.hpp>
#include <HG/Networking/Base/PacketLayers/UnstablePacketHeader.hpp>

// HG::Utils
#include <HG/Utils/Platform.hpp>
#include <HG/Utils/Logging.hpp>
#include <HG/Utils/LockFree/SPMCQueue.hpp>

// STD
#include <stdexcept>
#include <set>
#include <unordered_map>

namespace
{
    struct PosixServerData : public HG::Networking::Base::AbstractInternalData
    {
        struct StableConnectionsThreadData
        {
            ~StableConnectionsThreadData()
            {
                for (const auto& [sock, connection] : connections)
                {
                    HG::Networking::Base::LowLevel::closeSocket(sock);
                    delete connection;
                }
            }

            std::unordered_map<
                HG::Networking::Base::LowLevel::Socket,
                HG::Networking::Base::StableConnection*
            > connections;
            mutable std::shared_mutex connectionsMutex;
        };

        PosixServerData() :
            systemInfo(),
            listenTCPSocket(HG::Networking::Base::LowLevel::kInvalidSocket),
            listenUDPSocket(HG::Networking::Base::LowLevel::kInvalidSocket),
            stableConnectionDataByThread()
        {

        }

        ~PosixServerData() override
        {
            HG::Networking::Base::LowLevel::closeSocket(listenTCPSocket);
            HG::Networking::Base::LowLevel::closeSocket(listenUDPSocket);
        }

        HG::Networking::Base::LowLevel::SystemInfo systemInfo;

        HG::Networking::Base::LowLevel::Socket listenTCPSocket;
        HG::Networking::Base::LowLevel::Socket listenUDPSocket;

        std::vector<std::unique_ptr<StableConnectionsThreadData>> stableConnectionDataByThread;
    };

    struct StableConnectionInternalData : public HG::Networking::Base::AbstractInternalData
    {
        enum class ReadState
        {
            Initial,
            ReadingBody,
        };

        ~StableConnectionInternalData() override
        {
            HG::Networking::Base::LowLevel::closeSocket(socket);
        }

        HG::Networking::Base::LowLevel::Socket socket;
        std::vector<std::byte> buffer;
        HG::Networking::Base::PacketLayers::StablePacketHeader lastPacketHeader;
        ReadState state = ReadState::Initial;
    };

    PosixServerData* createServerData()
    {
        return new PosixServerData();
    }

    static constexpr std::size_t kMaxPacketSize = 1 * 1024 * 1024;
    static constexpr std::size_t kMaxUnstablePacketSize = 1500; // mtu related
}

namespace HG::Networking::Base
{
    Server::Server(HG::Core::Application* application) :
        m_application(application),
        m_data(nullptr),
        m_isRunning(false)
    {

    }

    Server::~Server()
    {
        if (isRunning())
        {
            stop();
        }

        delete serverData<PosixServerData>();
    }

    void Server::stop()
    {
        if (!isRunning())
        {
            throw std::runtime_error("Server is not running");
        }

        m_isRunning = false;
        application()->threadPool()->stopPool(HG::Core::ThreadPool::Type::NetworkingThread);
        application()->threadPool()->joinPool(HG::Core::ThreadPool::Type::NetworkingThread);

        auto* currentServerData = serverData<PosixServerData>();
        m_data = nullptr;
        delete currentServerData;
    }

    void Server::start(std::uint16_t udpPort, std::uint16_t tcpPort, std::size_t stableThreads)
    {
        if (isRunning())
        {
            throw std::runtime_error("Server already running");
        }

        m_data = createServerData();
        auto* currentServerData = serverData<PosixServerData>();

        // Preparing UDP listening
        auto udpAddress = HG::Networking::Base::LowLevel::createInternalIPv4Address(HG::Networking::Base::LowLevel::kAnyAddress, udpPort);

        currentServerData->listenUDPSocket = HG::Networking::Base::LowLevel::createUDPSocket();

        if (!HG::Networking::Base::LowLevel::bindSocketWithAddress(currentServerData->listenUDPSocket, udpAddress))
        {
            stop();
            throw std::runtime_error(fmt::format("Can't bind udp socket at port {}", udpPort));
        }

        // Preparing TCP listening
        auto tcpAddress = HG::Networking::Base::LowLevel::createInternalIPv4Address(HG::Networking::Base::LowLevel::kAnyAddress, tcpPort);

        currentServerData->listenTCPSocket = HG::Networking::Base::LowLevel::createTCPSocket();

        if (!HG::Networking::Base::LowLevel::bindSocketWithAddress(currentServerData->listenTCPSocket, tcpAddress))
        {
            stop();
            throw std::runtime_error(fmt::format("Can't bind tcp socket at port {}", tcpPort));
        }

        if (!HG::Networking::Base::LowLevel::setSocketToListeningMode(currentServerData->listenTCPSocket))
        {
            stop();
            throw std::runtime_error(fmt::format("Can't set socket to listening mode at port {}", tcpPort));
        }

        auto* threadPool = application()->threadPool();
        if (threadPool == nullptr)
        {
            stop();
            throw std::runtime_error("Application instance must have thread pool");
        }

        m_isRunning = true;

        currentServerData->stableConnectionDataByThread.reserve(stableThreads);
        for (std::uint32_t i = 0; i < stableThreads; ++i)
        {
            currentServerData->stableConnectionDataByThread.emplace_back(
                std::move(std::make_unique<PosixServerData::StableConnectionsThreadData>())
            );
        }

        threadPool->startPool(HG::Core::ThreadPool::Type::NetworkingThread, stableThreads + 1);
        threadPool->push([this]() { listenJob(); }, HG::Core::ThreadPool::Type::NetworkingThread);

        for (std::uint32_t i = 0; i < stableThreads; ++i)
        {
            threadPool->push([this, i](){ stableConnectionsProcessor(i); }, HG::Core::ThreadPool::Type::NetworkingThread);
        }
    }

    bool Server::isRunning() const
    {
        return m_isRunning;
    }

    HG::Core::Application* Server::application() const
    {
        return m_application;
    }

    void Server::listenJob()
    {
        std::vector<std::byte> buffer;
        while (m_isRunning)
        {
            auto* currentServerData = serverData<PosixServerData>();

            auto readSet = HG::Networking::Base::LowLevel::createDescriptorSet();
            addToDescriptorSet(readSet, currentServerData->listenUDPSocket);
            addToDescriptorSet(readSet, currentServerData->listenTCPSocket);

            if (!waitDescriptorSet(readSet, std::chrono::seconds(1)))
            {
                continue;
            }

            if (HG::Networking::Base::LowLevel::isDescriptorReady(currentServerData->listenTCPSocket, readSet))
            {
                auto newConnection = HG::Networking::Base::LowLevel::acceptNewConnection(currentServerData->listenTCPSocket);
                HG::Networking::Base::LowLevel::setSocketToNonblockingMode(newConnection.socket);

                auto* stableConnection = new HG::Networking::Base::StableConnection(
                    HG::Networking::Base::LowLevel::internalToExternalAddress(newConnection.internalAddress)
                );
                HGInfo("TCP connection received from {}", stableConnection->address().toString());

                auto* internalData = stableConnection->createInternalData<StableConnectionInternalData>();
                internalData->socket = newConnection.socket;

                auto unloadedThreadIter = std::min_element(
                    currentServerData->stableConnectionDataByThread.begin(),
                    currentServerData->stableConnectionDataByThread.end(),
                    [](const std::unique_ptr<PosixServerData::StableConnectionsThreadData>& a,
                       const std::unique_ptr<PosixServerData::StableConnectionsThreadData>& b)
                    {
                        // acquire internal mutexes here
                        std::shared_lock aLock(a->connectionsMutex);
                        std::shared_lock bLock(b->connectionsMutex);

                        return a->connections.size() < b->connections.size();
                    }
                );

                // acquire internal mutex
                std::unique_lock lock((*unloadedThreadIter)->connectionsMutex);
                (*unloadedThreadIter)->connections[newConnection.socket] = stableConnection;
            }

            if (HG::Networking::Base::LowLevel::isDescriptorReady(currentServerData->listenUDPSocket, readSet))
            {
                HG::Networking::Base::LowLevel::InternalAddress internalAddress{0};
                // Accept new unstable packet
                HG::Networking::Base::LowLevel::readFromUnstableSocket(
                    currentServerData->listenUDPSocket,
                    internalAddress,
                    buffer,
                    kMaxUnstablePacketSize
                );

                auto address = HG::Networking::Base::LowLevel::internalToExternalAddress(internalAddress);

                auto packetHeader = HG::Networking::Base::PacketLayers::UnstablePacketHeader::parse(
                    buffer
                );

                if (packetHeader.dataSize > kMaxUnstablePacketSize)
                {
                    HGWarning("Host {} sent packet with size {} but maximum packet data size {}, skip this packet",
                              address.toString(), packetHeader.dataSize,
                              (kMaxUnstablePacketSize - HG::Networking::Base::PacketLayers::UnstablePacketHeader::kSize));
                    buffer.clear();
                    continue;
                }

                HGInfo("Received UDP header from {}, data size: {}", address.toString(), packetHeader.dataSize);
                HGInfo("Data: {}", (const char*) (buffer.data() + HG::Networking::Base::PacketLayers::UnstablePacketHeader::kSize));

                buffer.clear();
            }
        }
    }

    void Server::stableConnectionsProcessor(std::uint32_t index)
    {
        std::vector<HG::Networking::Base::LowLevel::Socket> socketsToBeClosed;
        while (m_isRunning)
        {
            auto* currentServerData = serverData<PosixServerData>();
            auto& connectionsThreadData = (*currentServerData->stableConnectionDataByThread[index]);

            auto readSet = HG::Networking::Base::LowLevel::createDescriptorSet();

            // Observe current available connections
            {
                std::shared_lock lock(connectionsThreadData.connectionsMutex);
                for (const auto& [socket, connection] : connectionsThreadData.connections)
                {
                    auto* internalData = connection->internalData<StableConnectionInternalData>();

                    if (connection->isClosed())
                    {
                        socketsToBeClosed.push_back(socket);
                        continue;
                    }

                    addToDescriptorSet(readSet, internalData->socket);
                }
            }

            // Don't spend cpu time, until any connection
            // will be available.
            if (descriptorSetSize(readSet) == 0)
            {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(200)
                );
            }

            // Close connections to desired sockets and remove them from
            // thread data
            {
                for (const auto& sock : socketsToBeClosed)
                {
                    HG::Networking::Base::LowLevel::closeSocket(sock);
                }

                std::unique_lock lock(connectionsThreadData.connectionsMutex);
                for (const auto& sock : socketsToBeClosed)
                {
                    auto connectionIter = connectionsThreadData.connections.find(sock);
                    HGInfo("Closing connection to {}", connectionIter->second->address().toString());
                    delete connectionIter->second;
                    connectionsThreadData.connections.erase(connectionIter);
                }
                socketsToBeClosed.clear();
            }

            // If no data was received on any client
            // just try again.
            if (!waitDescriptorSet(readSet, std::chrono::seconds(1)))
            {
                continue;
            }

            // Iterate all sockets, that's available with data
            applyAtDescriptors(
                readSet,
                [&connectionsThreadData](HG::Networking::Base::LowLevel::Socket sock)
                {
                    HG::Networking::Base::StableConnection* stableConnection;
                    {
                        std::shared_lock lock(connectionsThreadData.connectionsMutex);
                        stableConnection = connectionsThreadData.connections.find(sock)->second;
                    }

                    auto* internalData = stableConnection->internalData<StableConnectionInternalData>();

                    if (internalData->state == StableConnectionInternalData::ReadState::Initial) // Packet initial read
                    {
                        // Reading packet header
                        if (!HG::Networking::Base::LowLevel::readFromStableSocket(
                            internalData->socket,
                            internalData->buffer,
                            HG::Networking::Base::PacketLayers::StablePacketHeader::kSize
                        ))
                        {
                            HGInfo("Connection to {} was closed on remote host", stableConnection->address().toString());
                            stableConnection->close();
                            return;
                        }

                        if (internalData->buffer.size() < HG::Networking::Base::PacketLayers::StablePacketHeader::kSize)
                        {
                            return;
                        }

                        internalData->lastPacketHeader = HG::Networking::Base::PacketLayers::StablePacketHeader::parse(
                            internalData->buffer
                        );

                        internalData->buffer.resize(0);
                        // todo: add magic validation somewhere here

                        if (internalData->lastPacketHeader.dataSize > kMaxPacketSize)
                        {
                            HGWarning("Connection from {} sent packet size {}, when maximum size {}, closing connection",
                                      stableConnection->address().toString(), internalData->lastPacketHeader.dataSize,
                                      kMaxPacketSize);
                            stableConnection->close();
                            return;
                        }

                        internalData->state = StableConnectionInternalData::ReadState::ReadingBody;
                    }

                    if (internalData->state == StableConnectionInternalData::ReadState::ReadingBody)
                    {
                        if (!HG::Networking::Base::LowLevel::readFromStableSocket(
                            internalData->socket,
                            internalData->buffer,
                            internalData->lastPacketHeader.dataSize
                        ))
                        {
                            HGInfo("Connection to {} was closed on remote host", stableConnection->address().toString());
                            stableConnection->close();
                            return;
                        }

                        if (internalData->buffer.size() < internalData->lastPacketHeader.dataSize)
                        {
                            return;
                        }

                        // todo: Send this data to processing
                        HGInfo("Received data: {}", (const char*) internalData->buffer.data());

                        internalData->buffer.resize(0);
                        internalData->state = StableConnectionInternalData::ReadState::Initial;
                    }
                }
            );
        }
    }
}