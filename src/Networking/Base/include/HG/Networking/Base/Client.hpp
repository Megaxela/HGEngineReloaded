#pragma once

// STL
#include <atomic>

namespace HG::Core
{
class Application;
}

namespace HG::Networking::Base
{
struct AbstractInternalData;

/**
     * @brief Class, that describes engine client.
     */
class Client
{
public:
    /**
         * @brief Constructor.
         * @param application Base application.
         */
    explicit Client(HG::Core::Application* application);

    /**
         * @brief Destructor. If client was connected - connection will be closed.
         */
    ~Client();

private:
    template <typename T>
    [[nodiscard]] T* clientData() const
    {
        return static_cast<T*>(m_data);
    }

    HG::Core::Application* m_application;
    HG::Networking::Base::AbstractInternalData* m_data;
    std::atomic_bool m_isRunning;
};
} // namespace HG::Networking::Base
