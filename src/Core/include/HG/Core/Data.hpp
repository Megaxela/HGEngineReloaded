#pragma once

// C++ STL
#include <cstddef>
#include <memory>
#include <string>

namespace HG::Core
{
/**
 * @brief Class, that describes virtual data object.
 */
class Data
{
public:
    /**
     * @brief Method for getting received data
     * size.
     * @return Size in bytes.
     */
    [[nodiscard]] virtual std::size_t size() const = 0;

    /**
     * @brief Method for getting data.
     * @return Data.
     */
    [[nodiscard]] virtual const std::byte* data() const = 0;

    /**
     * @brief Method for getting data id.
     * @return ID.
     */
    [[nodiscard]] virtual std::string id() const = 0;
};

using DataPtr = std::shared_ptr<Data>;
} // namespace HG::Core
