#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/Material.hpp>

namespace HG::Rendering::OpenGL::Materials
{
    /**
     * @brief Material for texture blitting.
     */
    class BlitMaterial : public HG::Rendering::Base::Material
    {
    public:

        static const char* rawShader;
    };
}
