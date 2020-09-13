#pragma once

// HG::Networking::Base
#include <HG/Networking/Base/AddressIPv4.hpp>

// STL
#include <atomic>

namespace HG::Networking::Base
{
class AbstractInternalData;

/**
 * @brief Class, that describes internal
 * stable TCP connection.
 */
class StableConnection
{
public:
    /**
     * @brief Constructor.
     * @param address Remote connected host address.
     */
    explicit StableConnection(HG::Networking::Base::AddressIPv4 address);

    /**
     * @brief Destructor.
     */
    ~StableConnection();

    /**
     * @brief Method for getting remote host address.
     * @return Remote host address.
     */
    [[nodiscard]] HG::Networking::Base::AddressIPv4 address() const;

    /**
     * @brief Method for notifying stable processor thread,
     * that this connection must be closed.
     * Actual disconnect may have delay.
     */
    void close();

    /**
     * @brief Method for checking is this connection
     * was marked as closed.
     */
    [[nodiscard]] bool isClosed() const;

    /**
     * @brief Method for setting any internal data.
     */
    template <typename T>
    T* createInternalData()
    {
        m_internalData = new T();
        return static_cast<T*>(m_internalData);
    }

    /**
     * @brief Method for getting internal data.
     */
    template <typename T>
    T* internalData() const
    {
        return dynamic_cast<T*>(m_internalData);
    }

private:
    std::atomic_bool m_isClosed;
    AddressIPv4 m_address;
    HG::Networking::Base::AbstractInternalData* m_internalData;
};
} // namespace HG::Networking::Base
