// HG::Networking::Base
#include <HG/Networking/Base/UnstableConnection.hpp>
#include <HG/Networking/Base/AbstractInternalData.hpp>

namespace HG::Networking::Base
{
    UnstableConnection::UnstableConnection(HG::Networking::Base::AddressIPv4 address) :
        m_address(address),
        m_connectionController(),
        m_internalData(nullptr)
    {

    }

    UnstableConnection::~UnstableConnection()
    {
        delete m_internalData;
    }

    HG::Networking::Base::AddressIPv4 UnstableConnection::address() const
    {
        return HG::Networking::Base::AddressIPv4();
    }
}