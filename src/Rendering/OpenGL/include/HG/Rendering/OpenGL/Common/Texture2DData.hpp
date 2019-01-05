#pragma once

// HG::Core
#include <HG/Core/CachableResource.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderSpecificData.hpp>

// gl
#include <gl/all.hpp>

// GLM
#include <glm/glm.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for textures.
     */
    class Texture2DData : public HG::Core::CachableResource<Texture2DData>,
                          public HG::Rendering::Base::RenderSpecificData
    {
    public:

        gl::texture_2d Texture = gl::texture_2d(gl::invalid_id);
        bool Allocated = false;
        glm::ivec2 Size = {0, 0};
    };
}