#pragma once

#include <gl/all.hpp>
#include <Texture.hpp>

namespace OGL_RENDERING_MODULE_NS::Common
{
    /**
     * @brief External data implementation for textures.
     */
    class Texture2DData : public ::RENDERING_BASE_MODULE_NS::Texture::TextureExternalData
    {
    public:

        gl::texture_2d Texture;
    };
}