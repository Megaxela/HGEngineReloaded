#pragma once

// HG::Rendering::Base
#include <Lights/AbstractLight.hpp> // Required for inheritance

namespace HG::Rendering::Base::Lights
{
    /**
     * @brief Class, that describes directional light.
     */
    class DirectionalLight : public AbstractLight
    {
    public:

        /**
         * @brief Constructor.
         */
        DirectionalLight();


    };
}


