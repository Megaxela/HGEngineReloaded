#pragma once

#include "RenderBehaviour.hpp"

namespace RENDERING_BASE_MODULE_NS::Behaviours
{
    /**
     * @brief Class, that describes cubemap rendering
     * behaviour.
     */
    class CubeMap : public RenderBehaviour
    {
    public:

        // Mesh type id
        static constexpr std::size_t Id = __COUNTER__;

        /**
         * @brief Constructor.
         */
        CubeMap();

        /**
         * @brief Destructor.
         */
        ~CubeMap() override;

        
    };
}


