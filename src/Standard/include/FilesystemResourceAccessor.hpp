#pragma once

#include <ResourceAccessor.hpp>
#include <bits/shared_ptr.h>

namespace STD_MODULE_NS
{
    /**
     * @brief Class, that describes standard filesystem
     * resource accessor.
     */
    class FilesystemResourceAccessor : public ::CORE_MODULE_NS::ResourceAccessor
    {
    public:

        /**
         * @brief Class, that describes data object with
         * vector inside.
         */
        class VectorData : public ::CORE_MODULE_NS::ResourceAccessor::Data
        {
        public:

            /**
             * @brief Constructor.
             * @param data Movable data.
             */
            explicit VectorData(std::vector<std::byte> data);

            /**
             * @brief Method for getting amount of data
             * in bytes.
             */
            std::size_t size() const override;

            /**
             * @brief Method for getting pointer to data.
             * @return Pointer to array of bytes.
             */
            const std::byte* data() const override;

        private:
            std::vector<std::byte> m_data;
        };

        std::shared_ptr<Data> loadRaw(const std::string &id) override;
    };
}
