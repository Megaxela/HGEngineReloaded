#pragma once

#include <gl/all.hpp>
#include <Shader.hpp>

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