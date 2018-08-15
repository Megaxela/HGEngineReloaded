#pragma once

#include <RenderSpecificData.hpp>
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for cube map.
     */
    class CubeMapTextureData : public HG::Rendering::Base::RenderSpecificData
    {
    public:

        gl::cubemap_texture Texture;
    };
}
