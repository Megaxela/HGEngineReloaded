// HG::Networking::Base
#include <HG/Networking/Base/AddressIPv4.hpp>

// STD
#include <stdexcept>

namespace HG::Networking::Base
{
    AddressIPv4::AddressIPv4() :
        m_address(0),
        m_port(0)
    {

    }

    AddressIPv4::AddressIPv4(std::uint32_t address, std::uint16_t port) :
        m_address(address),
        m_port(port)
    {

    }

    std::string AddressIPv4::toString() const
    {
        std::string result;
        result.reserve(21);

        result.append(std::to_string((0xFF000000u & m_address) >> 24u));
        result.append(1, '.');
        result.append(std::to_string((0x00FF0000u & m_address) >> 16u));
        result.append(1, '.');
        result.append(std::to_string((0x0000FF00u & m_address) >> 8u ));
        result.append(1, '.');
        result.append(std::to_string( 0x000000FFu & m_address));

        if (m_port)
        {
            result.append(1, ':');
            result.append(std::to_string(m_port));
        }

        return result;
    }

    AddressIPv4 AddressIPv4::fromString(const std::string& s)
    {
        std::uint32_t address = 0x00000000;
        std::uint16_t port = 0;

        std::uint8_t inc = 0;
        std::uint8_t byteN = 4;

        std::size_t index = 0;
        for (const auto& ch : s)
        {
            ++index;

            if (ch == '.' || ch == ':')
            {
                address |= (inc << (--byteN * 8u));
                inc = 0;

                if (ch == ':')
                {
                    // todo: user charconv here
                    port = std::stoi(s.data() + index);
                    break;
                }
            }
            else if (ch >= '0' && ch <= '9')
            {
                // Overflow check
                if (inc > 25 || (inc == 25 && ch > '5'))
                {
                    throw std::invalid_argument("Wrong format");
                }

                inc = inc * static_cast<std::uint8_t>(10) + (ch - '0');
            }
            else
            {
                throw std::invalid_argument("Wrong format");
            }
        }

        if (inc != 0)
        {
            address |= inc << (--byteN * 8u);
            inc = 0;
        }

        return AddressIPv4(address, port);
    }

    std::uint32_t AddressIPv4::address() const
    {
        return m_address;
    }

    std::uint16_t AddressIPv4::port() const
    {
        return m_port;
    }

    void AddressIPv4::setAddress(const std::string& s)
    {
        std::uint32_t address = 0x00000000;

        std::uint8_t inc = 0;
        std::uint8_t byteN = 4;

        for (const auto& ch : s)
        {
            if (ch == '.' || ch == ':')
            {
                address |= inc << (--byteN * 8u);
                inc = 0;
            }
            else if (ch >= '0' && ch <= '9')
            {
                // Overflow check
                if (inc > 25 || (inc == 25 && ch > '5'))
                {
                    throw std::invalid_argument("Wrong format");
                }

                inc *= static_cast<std::uint8_t>(10) + (ch - '0');
            }
            else
            {
                throw std::invalid_argument("Wrong format");
            }
        }

        address |= inc << (--byteN * 8u);

        m_address = address;
    }

    void AddressIPv4::setAddress(std::uint32_t address)
    {
        m_address = address;
    }

    void AddressIPv4::setPort(std::uint16_t port)
    {
        m_port = port;
    }
}