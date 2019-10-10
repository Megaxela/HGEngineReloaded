#pragma once

// HG::Core
#include <HG/Core/CachableResource.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/RenderSpecificData.hpp> // Required for inheritance

// gl
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Common
{
/**
  * @brief External data implementation for mesh rendering behaviour
  */
class MeshData
    : public HG::Core::CachableResource<MeshData>
    , public HG::Rendering::Base::RenderSpecificData
{
public:
    uint32_t Count;

    gl::vertex_array VAO;
    gl::buffer VBO;
    gl::buffer EBO;
};
} // namespace HG::Rendering::OpenGL::Common
