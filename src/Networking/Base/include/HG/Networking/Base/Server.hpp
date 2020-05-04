#pragma once

// STD
#include <cstdint>
#include <atomic>


namespace HG::Core
{
    class Application;
}

namespace HG::Networking::Base
{
    struct AbstractInternalData;

    /**
     * @brief Class, that describes server for external
     * requests processing by stable and unstable connections.
     */
    class Server
    {
    public:
        /**
         * @brief Constructor.
         * @param application Base application.
         */
        explicit Server(HG::Core::Application* application);

        /**
         * @brief Destructor. If server was running - it will be stopped.
         */
        ~Server();

        /**
         * @brief Method for stopping working server.
         * If server was not working exception will be thrown.
         * @throws std::runtime_error
         */
        void stop();

        /**
         * @brief Method for starting not working server.
         * If server was already started exception will be thrown.
         * @throws std::runtime_error
         */
        void start(std::uint16_t udpPort, std::uint16_t tcpPort, std::size_t stableThreads=2);

        /**
         * @brief Method for checking is server running.
         */
        [[nodiscard]] bool isRunning() const;

        /**
         * @brief Method for getting parent application.
         * @return Pointer to parent application.
         */
        [[nodiscard]] HG::Core::Application* application() const;

    private:
        template<typename T>
        [[nodiscard]] T* serverData() const {
            return static_cast<T*>(m_data);
        }

        void listenJob();

        void stableConnectionsProcessor(std::uint32_t index);

        HG::Core::Application* m_application;
        HG::Networking::Base::AbstractInternalData* m_data;
        std::atomic_bool m_isRunning;
    };
}

