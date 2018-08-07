#pragma once

#include <Material.hpp>

namespace OGL_RENDERING_MODULE_NS::Materials
{
    class MeshFallbackMaterial : public RENDERING_BASE_MODULE_NS::Material
    {
    public:
        static const char* rawShader;
    };
}


