#pragma once

// C++ STL
#include <cstdint>
#include <type_traits>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderSpecificData.hpp>

namespace HG::Rendering::Base
{

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
        HG::Rendering::Base::RenderSpecificData* specificData();

        /**
         * @brief Method for centralized casting abstract
         * HG::Rendering::Base::RenderSpecificData to
         * defined type.
         * @tparam T Type.
         * @return Pointer to type.
         */
        template<typename T>
        T* castSpecificDataTo()
        {
            static_assert(
                std::is_base_of<HG::Rendering::Base::RenderSpecificData, T>::value,
                "Cast type has to be derived of HG::Rendering::Base::RenderSpecificData"
            );
            return static_cast<T*>(specificData());
        }

        /**
         * @brief Method for setting specific data.
         * @tparam T Specific data type.
         * @return Created specific data.
         */
        void setSpecificData(HG::Rendering::Base::RenderSpecificData* data);

        /**
         * @brief Method for getting render data
         * type.
         */
        std::size_t dataType() const;

        /**
         * @brief Method for invalidating render data.
         * (Required reinitialization)
         */
        void invalidate();

    private:

        HG::Rendering::Base::RenderSpecificData* m_data;

        std::size_t m_type;

    };
}

