#pragma once

// HG::Core
#include <HG/Core/CachableResource.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderSpecificData.hpp> // Required for inheritance

// GLM
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for cube map.
     */
    class CubeMapTextureData : public HG::Core::CachableResource<CubeMapTextureData>,
                               public HG::Rendering::Base::RenderSpecificData
    {
    public:

        gl::cubemap_texture_array Texture;

        bool StoragePrepared = false;

        /**
         * It's bitfield.
         * ->
         * index | 0     | 1    | 2   | 3      | 4     | 5
         * side  | Right | Left | Top | Bottom | Front | Back
         */
        uint8_t LoadedSides = 0;
    };
}
