#pragma once

// C++ STL
#include <cstddef>
#include <string>
#include <memory>

namespace HG::Core
{
    class Data;

    using DataPtr = std::shared_ptr<Data>;
}

namespace HG::Core
{
    /**
     * @brief Interface for classes, that
     * has to provide access to resources.
     */
    class ResourceAccessor
    {
    public:

        /**
         * @brief Virtual destructor.
         */
        virtual ~ResourceAccessor() = default;

        /**
         * @brief Method for loading raw data.
         * @param id ID of resource.
         * @return Loaded data.
         */
        virtual HG::Core::DataPtr loadRaw(const std::string& id) = 0;
    };
}

