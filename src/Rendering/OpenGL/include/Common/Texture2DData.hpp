#pragma once

// HG::Rendering::Base
#include <RenderSpecificData.hpp>

// gl
#include <gl/all.hpp>

// GLM
#include <glm/glm.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for textures.
     */
    class Texture2DData : public HG::Rendering::Base::RenderSpecificData
    {
    public:

        gl::texture_2d Texture;
        glm::ivec2 Size = {0, 0};
    };
}