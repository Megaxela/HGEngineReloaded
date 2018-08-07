#pragma once

#include <Material.hpp>

namespace OGL_RENDERING_MODULE_NS::Materials
{
    /**
     * @brief Material for gizmos mesh rendering.
     */
    class GizmosMeshMaterial : public RENDERING_BASE_MODULE_NS::Material
    {
    public:
        static const char* rawShader;
    };
}

