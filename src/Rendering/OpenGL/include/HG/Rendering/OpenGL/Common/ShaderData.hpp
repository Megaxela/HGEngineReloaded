#pragma once

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderSpecificData.hpp>

// gl
#include <gl/all.hpp>

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