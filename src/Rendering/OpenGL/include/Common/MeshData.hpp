#pragma once

#include <gl/all.hpp>
#include <RenderSpecificData.hpp>

namespace HG::Rendering::OpenGL::Common
{
    /**
      * @brief External data implementation for mesh rendering behaviour
      */
    class MeshData : public HG::Rendering::Base::RenderSpecificData
    {
    public:

        gl::vertex_array VAO;
        gl::buffer VBO;
        gl::buffer EBO;
    };
}