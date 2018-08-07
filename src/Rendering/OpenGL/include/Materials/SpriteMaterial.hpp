#pragma once

#include <Material.hpp>

namespace OGL_RENDERING_MODULE_NS::Materials
{
    /**
     * @brief Material, that performs sprite rendering.
     */
    class SpriteMaterial : public RENDERING_BASE_MODULE_NS::Material
    {
    public:

        static const char* rawShader;
    };
}


