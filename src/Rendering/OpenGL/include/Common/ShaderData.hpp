#pragma once

#include <gl/all.hpp>
#include <RenderSpecificData.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for shaders.
     */
    class ShaderData : public HG::Rendering::Base::RenderSpecificData
    {
    public:

        gl::program Program;
    };
}