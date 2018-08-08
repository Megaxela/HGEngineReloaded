#pragma once

#include <gl/all.hpp>
#include <Shader.hpp>

namespace OGL_RENDERING_MODULE_NS::Common
{
    /**
     * @brief External data implementation for shaders.
     */
    class ShaderData : public ::RENDERING_BASE_MODULE_NS::Shader::ShaderExternalData
    {
    public:

        gl::program Program;
    };
}