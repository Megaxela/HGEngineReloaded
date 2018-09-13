#pragma once

// HG::Rendering::Base
#include <Material.hpp>

namespace HG::Rendering::OpenGL::Materials
{
    class MeshFallbackMaterial : public HG::Rendering::Base::Material
    {
    public:
        static const char* rawShader;
    };
}


