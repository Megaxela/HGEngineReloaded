#pragma once

// STD
#include <string>

namespace HG::Networking::Base
{
/**
     * @brief Class, that describes IPv4 address.
     */
class AddressIPv4
{
public:
    /**
         * @brief Constructor.
         */
    AddressIPv4();

    /**
         * @brief Destructor.
         * @param address 4 byte address representation.
         * @param port Port.
         */
    explicit AddressIPv4(std::uint32_t address, std::uint16_t port = 0);

    /**
         * @brief Method for converting current
         * address object to string.
         * @example "192.168.1.1:3333"
         */
    [[nodiscard]] std::string toString() const;

    /**
         * @brief Method for converting string address:port
         * to IPv4 address.
         * @param s Address. Example "123.123.123.123:4444", "123.123.123.123"
         * @throws std::invalid_argument if can't convert.
         */
    static AddressIPv4 fromString(const std::string& s);

    /**
         * @brief Method for getting 4 byte representation
         * of IPv4 address.
         */
    [[nodiscard]] std::uint32_t address() const;

    /**
         * @brief Method for getting address port.
         */
    [[nodiscard]] std::uint16_t port() const;

    /**
         * @brief Method for setting address from string.
         * No port will be parsed here. Use `fromString` static
         * function to parse whole address.
         */
    void setAddress(const std::string& s);

    /**
         * @brief Method for setting address from 4 byte
         * address representation.
         */
    void setAddress(std::uint32_t address);

    /**
         * @brief Method for setting address port.
         */
    void setPort(std::uint16_t port);

private:
    std::uint32_t m_address;
    std::uint16_t m_port;
};
} // namespace HG::Networking::Base
