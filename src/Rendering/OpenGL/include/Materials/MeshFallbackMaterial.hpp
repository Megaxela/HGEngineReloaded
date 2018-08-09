#pragma once

#include <Material.hpp>

namespace HG::Rendering::OpenGL::Materials
{
    class MeshFallbackMaterial : public HG::Rendering::Base::Material
    {
    public:
        static const char* rawShader;
    };
}


