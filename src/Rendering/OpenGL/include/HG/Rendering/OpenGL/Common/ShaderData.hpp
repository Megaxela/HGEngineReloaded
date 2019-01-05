#pragma once

// HG::Core
#include <HG/Core/CachableResource.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderSpecificData.hpp>

// gl
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
     * @brief External data implementation for shaders.
     */
    class ShaderData : public HG::Core::CachableResource<ShaderData>,
                       public HG::Rendering::Base::RenderSpecificData
    {
    public:

        gl::program Program;

        std::unordered_map<
            std::string,
            GLint
        > UniformLocations;

        std::unordered_map<
            GLint,
            std::size_t
        > CurrentUniformValueHashes;
    };
}