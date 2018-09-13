#pragma once

// C++ STL
#include <memory>
#include <vector>

// HG::Core
#include <ResourceAccessor.hpp> // Required for inheritance
#include <Data.hpp> // Required for inheritance

namespace HG::Standard
{
    /**
     * @brief Class, that describes standard filesystem
     * resource accessor.
     */
    class FilesystemResourceAccessor : public HG::Core::ResourceAccessor
    {
    public:

        /**
         * @brief Class, that describes data object with
         * vector inside.
         */
        class VectorData : public HG::Core::Data
        {
        public:

            /**
             * @brief Constructor.
             * @param data Movable data.
             */
            explicit VectorData(std::vector<std::byte> data, std::string id);

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

            /**
             * @brief Method for getting data id.
             * @return ID.
             */
            std::string id() const override;

        private:
            std::vector<std::byte> m_data;

            std::string m_id;
        };

        HG::Core::DataPtr loadRaw(const std::string &id) override;
    };
}
