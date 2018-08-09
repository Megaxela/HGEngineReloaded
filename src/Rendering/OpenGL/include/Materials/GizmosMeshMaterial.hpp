#pragma once

#include <Material.hpp>

namespace HG::Rendering::OpenGL::Materials
{
    /**
     * @brief Material for gizmos mesh rendering.
     */
    class GizmosMeshMaterial : public HG::Rendering::Base::Material
    {
    public:
        static const char* rawShader;
    };
}

