#pragma once

// HG::Rendering::Base
#include <Material.hpp>

namespace HG::Rendering::OpenGL::Materials
{
    /**
     * @brief Material, that performs sprite rendering.
     */
    class SpriteMaterial : public HG::Rendering::Base::Material
    {
    public:

        static const char* rawShader;
    };
}


