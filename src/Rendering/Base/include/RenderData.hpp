#pragma once

// C++ STL
#include <cstdint>

namespace HG::Rendering::Base
{
    class RenderSpecificData;

    /**
     * @brief Class, that describes some data,
     * that can be initialized in rendering pipeline.
     */
    class RenderData
    {
    public:

        /**
         * @brief Constructor.
         */
        explicit RenderData(std::size_t type);

        /**
         * @brief Virtual destructor.
         */
        virtual ~RenderData();

        /**
         * @brief Method for getting specific data
         * of some type.
         * @tparam T Type of expected specific data.
         * @return Pointer to specific data.
         */
        RenderSpecificData* specificData();

        /**
         * @brief Method for setting specific data.
         * @tparam T Specific data type.
         * @return Created specific data.
         */
        void setSpecificData(RenderSpecificData* data);

        /**
         * @brief Method for getting render data
         * type.
         */
        std::size_t dataType() const;

    private:

        RenderSpecificData* m_data;

        std::size_t m_type;

    };
}

