#pragma once

// HG::Networking::Base
#include <HG/Networking/Base/AddressIPv4.hpp>
#include <HG/Networking/Base/UnstableConnectionController.hpp>

namespace HG::Networking::Base
{
class AbstractInternalData;

/**
 * @brief Class, that describes
 * unstable UDP connection.
 */
class UnstableConnection
{
public:
    /**
     * @brief Constructor.
     * @param address Remove connected host address.
     */
    explicit UnstableConnection(HG::Networking::Base::AddressIPv4 address);

    /**
     * @brief Destructor.
     */
    ~UnstableConnection();

    /**
     * @brief Method for getting remote host address.
     * @return Remote host address.
     */
    [[nodiscard]] HG::Networking::Base::AddressIPv4 address() const;

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
    HG::Networking::Base::AddressIPv4 m_address;
    HG::Networking::Base::UnstableConnectionController m_connectionController;
    HG::Networking::Base::AbstractInternalData* m_internalData;
};
} // namespace HG::Networking::Base
