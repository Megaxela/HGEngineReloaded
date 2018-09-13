#pragma once

// HG::Rendering::Base
#include <RenderSpecificData.hpp> // Requierd for inheritance

// gl
#include <gl/all.hpp>

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