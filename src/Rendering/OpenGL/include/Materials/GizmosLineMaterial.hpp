#pragma once

#include <Material.hpp>

namespace HG::Rendering::OpenGL::Materials
{
    /**
     * @brief Material for gizmos line rendering.
     */
    class GizmosLineMaterial : public HG::Rendering::Base::Material
    {
    public:

        static const char* rawShader;

    };
}

