#pragma once

#include <Behaviours/Mesh.hpp>
#include <gl/all.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
      * @brief External data implementation for mesh rendering behaviour
      */
    class MeshData : public ::HG::Rendering::Base::Behaviours::Mesh::ExternalData
    {
    public:

        gl::vertex_array VAO;
        gl::buffer VBO;
        gl::buffer EBO;
    };
}