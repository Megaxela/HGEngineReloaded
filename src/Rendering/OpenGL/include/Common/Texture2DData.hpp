#pragma once

#include <Texture.hpp>
#include <gl/all.hpp>

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