#pragma once

#include <gl/all.hpp>
#include <Texture.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for textures.
     */
    class Texture2DData : public HG::Rendering::Base::Texture::TextureExternalData
    {
    public:

        gl::texture_2d Texture;
    };
}