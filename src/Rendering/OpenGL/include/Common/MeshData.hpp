#pragma once

#include <Behaviours/Mesh.hpp>
#include <gl/all.hpp>

namespace OGL_RENDERING_MODULE_NS::Common
{
    /**
      * @brief External data implementation for mesh rendering behaviour
      */
    class MeshData : public ::RENDERING_BASE_MODULE_NS::Behaviours::Mesh::ExternalData
    {
    public:

        gl::vertex_array VAO;
        gl::buffer VBO;
        gl::buffer EBO;
    };
}