#pragma once

#include <gl/all.hpp>
#include <RenderSpecificData.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for textures.
     */
    class Texture2DData : public HG::Rendering::Base::RenderSpecificData
    {
    public:

        gl::texture_2d Texture;
    };
}