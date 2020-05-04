// HG::Networking::Base
#include <HG/Networking/Base/StableConnection.hpp>
#include <HG/Networking/Base/AbstractInternalData.hpp>

namespace HG::Networking::Base
{
    StableConnection::StableConnection(AddressIPv4 address) :
        m_isClosed(false),
        m_address(address),
        m_internalData(nullptr)
    {

    }

    StableConnection::~StableConnection()
    {
        delete m_internalData;
    }

    void StableConnection::close()
    {
        m_isClosed = true;
    }

    bool StableConnection::isClosed() const
    {
        return m_isClosed;
    }

    AddressIPv4 StableConnection::address() const
    {
        return m_address;
    }
}