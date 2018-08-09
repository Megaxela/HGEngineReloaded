#pragma once

#include "RenderBehaviour.hpp"

namespace HG::Rendering::Base::Behaviours
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


