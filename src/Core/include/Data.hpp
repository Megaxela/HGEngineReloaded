#pragma once
#include <cstddef>
#include <string>
#include <memory>

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
        virtual std::size_t size() const = 0;

        /**
         * @brief Method for getting data.
         * @return Data.
         */
        virtual const std::byte* data() const = 0;

        /**
         * @brief Method for getting data id.
         * @return ID.
         */
        virtual std::string id() const = 0;

    };

    using DataPtr = std::shared_ptr<Data>;
}
