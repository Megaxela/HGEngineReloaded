#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Lights/AbstractLight.hpp> // Required for inheritance

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
} // namespace HG::Rendering::Base::Lights
