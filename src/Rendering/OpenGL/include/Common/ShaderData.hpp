#pragma once

#include <Shader.hpp>
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for shaders.
     */
    class ShaderData : public HG::Rendering::Base::Shader::ShaderExternalData
    {
    public:

        gl::program Program;
    };
}